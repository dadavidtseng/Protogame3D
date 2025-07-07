//----------------------------------------------------------------------------------------------------
// Blinn-Phong (lit) shader for Squirrel Eiserloh's C34 SD student Engine (Spring 2025)
//
// Requires Vertex_PCUTBN vertex data (including valid tangent, bitangent, normal).
//----------------------------------------------------------------------------------------------------
// D3D11 basic rendering pipeline stages (and D3D11 function prefixes):
//	IA = Input Assembly (grouping verts 3 at a time to form triangles, or N to form lines, fans, chains, etc.)
//	VS = Vertex Shader (transforming vertexes; moving them around, and computing them in different spaces)
//	RS = Rasterization Stage (converting math triangles into discrete pixels covered, interpolating values within)
//	PS = Pixel Shader (a.k.a. Fragment Shader, computing the actual output color(s) at each pixel being drawn)
//	OM = Output Merger (combining PS output with existing colors, using the current blend mode: additive, alpha, etc.)
//
// D3D11 C++ functions are prefixed with the stage they apply to, so for example:
//	m_d3dContext->IASetInputLayout( layout );							// Input Assembly knows to expect verts as PCU or PCUTBN or...
//	m_d3dContext->IASetVertexBuffers( 0, 1, &vbo.m_gpuBuffer, &vbo.m_vertexSize, &offset ); // Bind VBOs for Input Assembly
//	m_d3dContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );	// Triangles vs. TriStrips, TriFans, LineList, etc.
//	m_d3dContext->VSSetShader( shader->m_vertexShader, nullptr, 0 );	// Set current Vertex Shader program
//	m_d3dContext->VSSetConstantBuffers( 3, 1, &cbo->m_gpuBuffer );		// CBO is accessible in Vertex Shader as register(b3)
//	m_d3dContext->RSSetViewports( 1, &viewport );						// Set viewport(s) to use in Rasterization Stage
//	m_d3dContext->RSSetState( m_rasterState );							// Set Rasterization Stage states, e.g. cull, fill, winding
//	m_d3dContext->PSSetShader( shader->m_pixelShader, nullptr, 0 );		// Set current Pixel Shader program
//	m_d3dContext->PSSetConstantBuffers( 3, 1, &cbo->m_gpuBuffer );		// CBO is accessible in Pixel Shader as register(b3)
//	m_d3dContext->PSSetShaderResources( 3, 1, &texture->m_shaderResourceView );	// Texture available in Pixel Shader as register(t3)
//	m_d3dContext->PSSetSamplers( 3, 1, &samplerState );					// Sampler is used in Pixel Shader as register(s3)
//	m_d3dContext->OMSetBlendState( m_blendStateAlpha, nullptr, 0xFFFFFFFF ); // Set alpha blend state in Output Merger
//	m_d3dContext->OMSetRenderTargets( 1, &m_backBufferRTV, dsv );		// Set render target texture(s) for Output Merger
//	m_d3dContext->OMSetDepthStencilState( m_depthStencilState, 0 );		// Set depth & stencil mode for Output Merger
//----------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------
// Input to the Vertex shader stage.
// Information contained per vertex, pulled from the VBO being drawn.
//----------------------------------------------------------------------------------------------------
struct VertexInput
{
	// "v_" stands for for "Vertex" attribute which comes directly from VBO data (Squirrel's convention)
	// The all-caps "semantic names" are arbitrary symbol to associate CPU-GPU and other linkages.
	float3	a_position		: VERTEX_POSITION;
	float4	a_color			: VERTEX_COLOR; // Expanded to float[0.f,1.f] from byte[0,255] because "UNORM" in DXGI_FORMAT_R8G8B8A8_UNORM
	float2	a_uvTexCoords	: VERTEX_UVTEXCOORDS;
	float3	a_tangent		: VERTEX_TANGENT;
	float3	a_bitangent		: VERTEX_BITANGENT;
	float3	a_normal		: VERTEX_NORMAL;

	// Built-in / automatic attributes (not part of incoming VBO data)
	// "SV_" means "System Variable" and is a built-in special reserved semantic
	uint	a_vertexID	: SV_VertexID; // Which vertex number in the VBO collection this is (automatic variable)
};

//----------------------------------------------------------------------------------------------------
// Output passed from the Vertex shader into the Pixel/fragment shader.
//
// Each of these values is automatically 3-way (barycentric) interpolated across the surface of
//	the triangle on a per-pixel basis during the Rasterization Stage (RS).
// "v_" stands for "Varying" meaning "barycentric-lepred" (Squirrel's personal convention)
//
// Note that the SV_Position variable is required, and expects the Vertex Shader (VS) to output
//	this variable in clip space; after the VS stage, before the Rasterization Stage (RS), this position
//	gets divided by its w value to convert from clip space to NDC (Normalized Device Coordinates).
//
// It is then 3-way (barycentric) interpolated across the surface of the triangle along with the
//	other variables here; the Pixel Shader (PS) stage then receives these interpolated values
//	which will be unique per pixel, and the SV_Position variable will be in NDC space.
//
// Semantic names other than "SV_" (System Variables) are arbitrary, and just need to match up
//	between the variable in the Vertex Shader output structure and the corresponding variable in the
//	Pixel Shader input structure.  Since we use the same structure for both, they all automatically
//	match up.
//----------------------------------------------------------------------------------------------------
struct VertexOutPixelIn
{
	float4 v_position		: SV_Position; // Required; VS output as clip-space vertex position; PS input as NDC pixel position.
	float4 v_color			: SURFACE_COLOR;
	float2 v_uvTexCoords	: SURFACE_UVTEXCOORDS;
	float3 v_worldPos		: WORLD_POSITION;
	float3 v_worldTangent	: WORLD_TANGENT;
	float3 v_worldBitangent	: WORLD_BITANGENT;
	float3 v_worldNormal	: WORLD_NORMAL;
	float3 v_modelTangent	: MODEL_TANGENT;
	float3 v_modelBitangent	: MODEL_BITANGENT;
	float3 v_modelNormal	: MODEL_NORMAL;
};

//----------------------------------------------------------------------------------------------------
// Light structure for Point and Spot lights
//----------------------------------------------------------------------------------------------------
struct Light
{
	float4  color;              // 0-15   (16 bytes)
	float3  worldPosition;      // 16-31  (16 bytes, 實際使用 12)
	float   innerRadius;        // 32-35  (4 bytes)

	float3  direction;          // 36-51  (16 bytes, 實際使用 12)
	float   outerRadius;        // 52-55  (4 bytes)

	float   innerConeAngle;     // 56-59  (4 bytes)
	float   outerConeAngle;     // 60-63  (4 bytes)
	int     lightType;          // 64-67  (4 bytes)
	float   padding;            // 68-71  (4 bytes)
};

//----------------------------------------------------------------------------------------------------
// CONSTANT BUFFERS (a.k.a. CBOs or Constant Buffer Objects, UBOs / Uniform Buffers in OpenGL)
//	"c_" stands for "Constant", Squirrel's personal naming convention.
//
// There are 14 available CBO "slots" or "registers" (b0 through b13).
//	If the C++ code binds to slot 5, we are binding to constant buffer register(b5)
// In C++ code we bind structures into CBO slots when we call:
//	m_d3dContext->VSSetConstantBuffers( slot, 1, &cbo->m_gpuBuffer ); VS... makes this CBO available in Vertex Shader
//	m_d3dContext->PSSetConstantBuffers( slot, 1, &cbo->m_gpuBuffer ); PS... makes this CBO available in Pixel Shader
//
// We might update some CBOs once per frame; others perhaps between each draw call; others only occasionally.
// CBOs have very picky alignment rules, but can otherwise be anything we want (max of 64k == 65536 bytes each).
//
// Guildhall-specific conventions we use for different CBO register slot numbers (b0 through b13):
//	register(b0) = Engine/System-Level constants (e.g. debug)	-- updated rarely
//	register(b1) = Per-Frame constants (e.g. time)				-- updated once per frame, maybe in Renderer::BeginFrame
//	register(b2) = Camera constants (e.g. view/proj matrices)	-- updated once in each Renderer::CameraBegin
//	register(b3) = Model constants (e.g. model matrix & tint)	-- updated once before each Renderer::DrawVertexBuffer call
//	b4-b7 = Other Engine-reserved slots
//	b8-b13 = Other Game-specific slots
//
// NOTE: Constant Buffers MUST be 16B-aligned (sizeof is a multiple of 16B), AND
//	also primitives may not cross 16B boundaries (unless they are 16B-aligned, like Mat44).
// So you must "pad out" any variables with dummy variables to make sure they adhere to these
//	rules, and make sure that your corresponding C++ struct has identical byte-layout to the shader struct.
// I find it easiest to think of this as the CBO having multiple rows, each row float4 (Vec4 == 16B) in size.
//----------------------------------------------------------------------------------------------------
cbuffer PerFrameConstants : register(b1)
{
	float	c_time;
	int		c_debugInt;
	float	c_debugFloat;
	float	EMPTY_PADDING_B1;
};

//----------------------------------------------------------------------------------------------------
#define MAX_LIGHTS 8

//----------------------------------------------------------------------------------------------------
cbuffer LightConstants : register(b2)
{
	// Directional Light (Sun)
	//float4 	c_sunColor;					// RGB color + intensity in alpha
	//float3 	c_sunDirection;				// Normalized direction
	//float	c_ambientIntensity;			// Global ambient light intensity

	// Point and Spot Lights
	int 	c_numLights;				// Number of active lights (0 to MAX_LIGHTS)
	float3 	EMPTY_PADDING_B2;			// 16-byte alignment padding
	Light 	c_lightArray[MAX_LIGHTS];	// Array of lights
};

//----------------------------------------------------------------------------------------------------
cbuffer CameraConstants : register(b3)
{
	float4x4	c_worldToCamera;	// a.k.a. "View" matrix; world space (+X east) to camera-relative space (+X camera-forward)
	float4x4	c_cameraToRender;	// a.k.a. "Game" matrix; axis-swaps from Game conventions (+X forward) to Render (+X right)
	float4x4	c_renderToClip;		// a.k.a. "Projection" matrix (perpective or orthographic); render space to clip space
	float3		c_cameraWorldPosition;	// Camera position in world space
	float		EMPTY_PADDING_B3;
};


//----------------------------------------------------------------------------------------------------
cbuffer ModelConstants : register(b4)
{
	float4x4	c_modelToWorld;		// a.k.a. "Model" matrix; model local space (+X model forward) to world space (+X east)
	float4		c_modelTint;		// Uniform Vec4 model tint (including alpha) to multiply against diffuse texel & vertex color
};


//----------------------------------------------------------------------------------------------------
// TEXTURE and SAMPLER constants
//
// There are 16 (on mobile) or 128 (on desktop) texture binding "slots" or "registers" (t0 through t15, or t127).
// There are 16 sampler slots (s0 through s15).
//
// In C++ code we bind textures into texture slots (t0 through t15 or t127) for use in the Pixel Shader when we call:
//	m_d3dContext->PSSetShaderResources( textureSlot, 1, &texture->m_shaderResourceView ); // e.g. (t3) if textureSlot==3
//
// In C++ code we bind texture samplers into sampler slots (s0 through s15) for use in the Pixel Shader when we call:
//	m_d3dContext->PSSetSamplers( samplerSlot, 1, &samplerState );  // e.g. (s3) if samplerSlot==3
//
// If we want to sample textures from within the Vertex Shader (VS), e.g. for displacement maps, we can also
//	use the VS versions of these C++ functions:
//	m_d3dContext->VSSetShaderResources( textureSlot, 1, &texture->m_shaderResourceView );
//	m_d3dContext->VSSetSamplers( samplerSlot, 1, &samplerState );
//----------------------------------------------------------------------------------------------------
Texture2D<float4>	t_diffuseTexture	: register(t0);			// Texture bound in texture constant slot #0 (t0)
Texture2D<float4>	t_normalTexture		: register(t1);			// Texture bound in texture constant slot #1 (t1)
Texture2D<float4>	t_specGlossEmitTexture : register(t2); 		// Texture bound in texture constant slot #2 (t2) - R=Specular, G=Gloss, B=Emissive
SamplerState		s_diffuseSampler	: register(s0);			// Sampler is bound in sampler constant slot #0 (s0)
SamplerState		s_normalSampler		: register(s1);			// Sampler is bound in sampler constant slot #1 (s1)
SamplerState		s_specGlossEmitSampler : register(s2); 		// Sampler is bound in sampler constant slot #2 (s2)

//----------------------------------------------------------------------------------------------------
// VERTEX SHADER (VS)
//
// "Main" entry point for the Vertex Shader (VS) stage; this function (and functions it calls) are
//	the vertex shader program, called once per vertex.
//
// (The name of this entry function is chosen in C++ as a D3DCompile argument.)
//
// Inputs are typically vertex attributes (PCU, PCUTBN) coming from the VBO.
// Outputs include anything we want to pass through the Rasterization Stage (RS) to the Pixel Shader (PS).
//----------------------------------------------------------------------------------------------------
VertexOutPixelIn VertexMain( VertexInput input )
{
	VertexOutPixelIn output;

	// Transform the position through the pipeline
	float4 modelPos = float4( input.a_position, 1.0 );	// VBOs provide vertexes in model space
	float4 worldPos		= mul( c_modelToWorld, modelPos );		// Model space (+X local forward) to World space (+X east)
	float4 cameraPos	= mul( c_worldToCamera, worldPos );		// World space (+X east) to Camera space (+X camera-forward)
	float4 renderPos	= mul( c_cameraToRender, cameraPos );	// Camera space (+X cam-fwd) to Render space (+X right/+Z fwd)
	float4 clipPos		= mul( c_renderToClip, renderPos );		// Render space to Clip space (range-map/FOV/aspect, and put Z in W, preparing for W-divide)

	// Transform the tangents, normals, and bitangents (using W=0 for directions)
	float4 modelTangent		= float4( input.a_tangent, 0.0 );
	float4 modelBitangent	= float4( input.a_bitangent, 0.0 );
	float4 modelNormal		= float4( input.a_normal, 0.0 );
	float4 worldTangent		= mul( c_modelToWorld, modelTangent );		// Note: here we multiply on the right (M*V) since our C++ matrices come in from our constant
	float4 worldBitangent	= mul( c_modelToWorld, modelBitangent );	//	buffers from C++ as basis-major (as opposed to component-major).  Be careful below when
	float4 worldNormal		= mul( c_modelToWorld, modelNormal );		//	we must reverse the multiplication order (V*M) when using HLSL's float3x3 constructor!

	// Set the outputs we want to pass through Rasterization Stage (RS) down to the Pixel Shader (PS)
    output.v_position		= clipPos;
    output.v_color			= input.a_color;
    output.v_uvTexCoords	= input.a_uvTexCoords;
	output.v_worldPos		= worldPos.xyz;
	output.v_worldTangent	= worldTangent.xyz;
	output.v_worldBitangent	= worldBitangent.xyz;
	output.v_worldNormal	= worldNormal.xyz;
	output.v_modelTangent	= modelTangent.xyz;
	output.v_modelBitangent	= modelBitangent.xyz;
	output.v_modelNormal	= modelNormal.xyz;

    return output; // Pass to Rasterization Stage (RS) for barycentric interpolation, then into Pixel Shader (PS)
}

//----------------------------------------------------------------------------------------------------
float RangeMap( float inValue, float inStart, float inEnd, float outStart, float outEnd )
{
	float fraction = (inValue - inStart) / (inEnd - inStart);
	float outValue = outStart + fraction * (outEnd - outStart);
	return outValue;
}


//----------------------------------------------------------------------------------------------------
float RangeMapClamped( float inValue, float inStart, float inEnd, float outStart, float outEnd )
{
	float fraction = saturate( (inValue - inStart) / (inEnd - inStart) );
	float outValue = outStart + fraction * (outEnd - outStart);
	return outValue;
}


//----------------------------------------------------------------------------------------------------
// Used standard normal color encoding, mapping xyz in [-1,1] to rgb in [0,1]
//----------------------------------------------------------------------------------------------------
float3 EncodeXYZToRGB( float3 vec )
{
	return (vec + 1.0) * 0.5;
}


//----------------------------------------------------------------------------------------------------
// Used standard normal color encoding, mapping rgb in [0,1] to xyz in [-1,1]
//----------------------------------------------------------------------------------------------------
float3 DecodeRGBToXYZ( float3 color )
{
	return (color * 2.0) - 1.0;
}

//----------------------------------------------------------------------------------------------------
// LIGHTING FUNCTIONS
//----------------------------------------------------------------------------------------------------

// Calculate Blinn-Phong lighting for a given light
void CalculateBlinnPhong(
	float3 lightDirection,		// Direction TO light (normalized)
	float3 lightColor,			// Light color * intensity
	float3 pixelNormal,			// Surface normal (normalized)
	float3 viewDirection,		// Direction TO camera (normalized)
	float3 diffuseColor,		// Material diffuse color
	float specularStrength,		// Material specular strength
	float specularPower,		// Material specular power
	inout float3 diffuseOut,	// Accumulated diffuse lighting
	inout float3 specularOut	// Accumulated specular lighting
)
{
	// Diffuse lighting (Lambert)
	float NdotL = saturate(dot(pixelNormal, lightDirection));
	diffuseOut += diffuseColor * lightColor * NdotL;

	// Specular lighting (Blinn-Phong)
	//if (NdotL > 0.0 && specularStrength > 0.0)
	if ( specularStrength > 0.0)
	{
		float3 halfwayDir = normalize(lightDirection + viewDirection);
		float NdotH = saturate(dot(pixelNormal, halfwayDir));
		float specularIntensity = pow(NdotH, specularPower);
		//specularOut += diffuseColor * lightColor * specularStrength * specularIntensity;
		specularOut += lightColor * specularStrength * specularIntensity;
	}
}

//----------------------------------------------------------------------------------------------------
// Calculate directional light contribution (Sun)
void CalculateDirectionalLight(
	float3 pixelNormal,
	float3 lightDirection,
	float4 lightColor,
	float ambientIntensity,
	float3 viewDirection,
	float3 diffuseColor,
	float specularStrength,
	float specularPower,
	inout float3 ambientOut,
	inout float3 diffuseOut,
	inout float3 specularOut,
	inout float lightStrengthOut
)
{
	float NdotL = dot(-lightDirection, pixelNormal);

	// Progressive ambience: remap part of negative result to positive
	float lightStrength = saturate(RangeMapClamped(NdotL, -1.0, 1.0, ambientIntensity, 1.0));
	lightStrengthOut = lightStrength;

	// Ambient contribution
	ambientOut += diffuseColor * lightColor.rgb * ambientIntensity;

	// Diffuse and specular contributions
	//if (NdotL > 0.0)
	//{
	//	CalculateBlinnPhong(
	//		-lightDirection,
	//		lightColor.rgb * lightColor.a,
	//		pixelNormal,
	//		viewDirection,
	//		diffuseColor,
	//		specularStrength,
	//		specularPower,
	//		diffuseOut,
	//		specularOut
	//	);
	// Diffuse and specular contributions
	// 修復：分別處理 diffuse 和 specular，不要讓 diffuse 限制 specular
	if (NdotL > 0.0)
	{
		// 只有 diffuse 需要 NdotL > 0 的限制
		diffuseOut += diffuseColor * lightColor.rgb * lightColor.a * NdotL;
	}

	// Specular 計算獨立進行，不受 NdotL 限制
	if (specularStrength > 0.0)
	{
		float3 halfwayDir = normalize(-lightDirection + viewDirection);
		float NdotH = saturate(dot(pixelNormal, halfwayDir));
		float specularIntensity = pow(NdotH, specularPower);
		specularOut += lightColor.rgb * lightColor.a * specularStrength * specularIntensity;
	}

}

//----------------------------------------------------------------------------------------------------
// Calculate point light contribution with distance falloff
void CalculatePointLight(
	Light light,
	float3 worldPos,
	float3 pixelNormal,
	float3 viewDirection,
	float3 diffuseColor,
	float specularStrength,
	float specularPower,
	inout float3 diffuseOut,
	inout float3 specularOut
)
{
	float3 lightVector = light.worldPosition - worldPos;

	float distToLight = length(lightVector);
	if (distToLight < 0.001) return; // 或其他處理方式

	float3 lightDirection = lightVector / distToLight;

	// Distance-based falloff
	float falloff = saturate(RangeMap(distToLight, light.innerRadius, light.outerRadius, 1.0, 0.0));

	if (falloff > 0.0)
	{
		float3 effectiveLightColor = light.color.rgb * light.color.a * falloff;

		CalculateBlinnPhong(
			lightDirection,
			effectiveLightColor,
			pixelNormal,
			viewDirection,
			diffuseColor,
			specularStrength,
			specularPower,
			diffuseOut,
			specularOut
		);
	}
}

//----------------------------------------------------------------------------------------------------
// Calculate spot light contribution with angular and distance falloff
void CalculateSpotLight(
	Light light,
	float3 worldPos,
	float3 pixelNormal,
	float3 viewDirection,
	float3 diffuseColor,
	float specularStrength,
	float specularPower,
	inout float3 diffuseOut,
	inout float3 specularOut
)
{
	float3 lightVector = light.worldPosition - worldPos;
	float distToLight = length(lightVector);
	float3 lightDirection = lightVector / distToLight;

	// Distance-based falloff
	float distanceFalloff = saturate(RangeMap(distToLight, light.innerRadius, light.outerRadius, 1.0, 0.0));

	// Angular falloff (spotlight cone)
	float cosAngle = dot(-lightDirection, normalize(light.direction));
	float angularFalloff = saturate(RangeMap(cosAngle, light.outerConeAngle, light.innerConeAngle, 0.0, 1.0));

	float totalFalloff = distanceFalloff * angularFalloff;

	if (totalFalloff > 0.0)
	{
		float3 effectiveLightColor = light.color.rgb * light.color.a * totalFalloff;

		CalculateBlinnPhong(
			lightDirection,
			effectiveLightColor,
			pixelNormal,
			viewDirection,
			diffuseColor,
			specularStrength,
			specularPower,
			diffuseOut,
			specularOut
		);
	}
}

//----------------------------------------------------------------------------------------------------
// PIXEL SHADER (PS)
//
// "Main" entry point for the Pixel Shader (PS) stage; this function (and functions it calls) are
//	the pixel shader program.
//
// (The name of this entry function is chosen in C++ as a D3DCompile argument.)
//
// Inputs are typically the barycentric-interpolated outputs from the Vertex Shader (VS) via Rasterization.
// Output is the color sent to the render target, to be blended via the Output Merger (OM) blend mode settings.
// If we have multiple outputs (colors to write to each of several different Render Targets), we can change
//	this function to return a structure containing multiple float4 output colors, one per target.
//----------------------------------------------------------------------------------------------------
float4 PixelMain( VertexOutPixelIn input ) : SV_Target0
{
	// Get the UV coordinates that were mapped onto this pixel
	float2 uvCoords = input.v_uvTexCoords;

	// Sample the diffuse map texture to see what this looks like at this pixel
	float4 diffuseTexel = t_diffuseTexture.Sample( s_diffuseSampler, uvCoords );
	float4 normalTexel	= t_normalTexture.Sample( s_normalSampler, uvCoords );
	float4 specGlossEmitTexel = t_specGlossEmitTexture.Sample( s_specGlossEmitSampler, uvCoords );
	float4 surfaceColor = input.v_color;
	float4 modelColor = c_modelTint;

	// Extract spec, gloss, and emissive values from the SpecGlossEmit texture
	float specularStrength = specGlossEmitTexel.r;	// Red channel = Specular strength
	float glossiness = specGlossEmitTexel.g;		// Green channel = Glossiness
	float emissiveStrength = specGlossEmitTexel.b;	// Blue channel = Emissive strength

	// Convert glossiness to specular power (higher values = sharper highlights)
	//float specularPower = glossiness * 128.0 + 1.0; // Range: 1-129
	float specularPower = max(1.0, glossiness * 64.0);
	specularStrength = max(0.1, specularStrength);
	// Decode normalTexel RGB into XYZ then renormalize; this is the per-pixel normal, in TBN space a.k.a. tangent space
	float3 pixelNormalTBNSpace = normalize( DecodeRGBToXYZ( normalTexel.rgb ) );

	// Tint diffuse color based on overall model tinting (including alpha translucency)
	float4 diffuseColor = diffuseTexel * surfaceColor * modelColor;

	// Get TBN basis vectors
	float3 surfaceTangentWorldSpace		= normalize( input.v_worldTangent );
	float3 surfaceBitangentWorldSpace	= normalize( input.v_worldBitangent );
	float3 surfaceNormalWorldSpace		= normalize( input.v_worldNormal );

	float3 surfaceTangentModelSpace		= normalize( input.v_modelTangent );
	float3 surfaceBitangentModelSpace	= normalize( input.v_modelBitangent );
	float3 surfaceNormalModelSpace		= normalize( input.v_modelNormal );

	// Create TBN matrix and transform normal to world space
	float3x3 tbnToWorld = float3x3( surfaceTangentWorldSpace, surfaceBitangentWorldSpace, surfaceNormalWorldSpace );
	float3 pixelNormalWorldSpace = mul( pixelNormalTBNSpace, tbnToWorld );

	// Choose which normal to use based on debug mode
	float3 finalNormal = pixelNormalWorldSpace;
	if( c_debugInt == 10 || c_debugInt == 12 )
	{
		finalNormal = surfaceNormalWorldSpace;
	}

	// Calculate view direction (from pixel to camera)
	float3 viewDirection = normalize(c_cameraWorldPosition - input.v_worldPos);

	// Initialize lighting accumulation
	float3 ambientLighting = float3(0, 0, 0);
	float3 diffuseLighting = float3(0, 0, 0);
	float3 specularLighting = float3(0, 0, 0);
	float lightStrength = 0.0;

	// Add directional light (Sun) contribution


	// Add point and spot lights
	for (int i = 0; i < c_numLights; i++)
	{
		Light light = c_lightArray[i];

		if(light.lightType == 0)
		{
			if (light.color.a > 0.0)
			{
				CalculateDirectionalLight(
					finalNormal,
					light.direction,
					light.color,
					light.color.a,
					viewDirection,
					diffuseColor.rgb,
					specularStrength,
					specularPower,
					ambientLighting,
					diffuseLighting,
					specularLighting,
					lightStrength
				);
			}
			else
			{
				// If no sun, still add ambient
				ambientLighting = diffuseColor.rgb * light.color.a;
			}
		}
		else if (light.lightType == 1) // Point light
		{
			CalculatePointLight(
				light,
				input.v_worldPos,
				finalNormal,
				viewDirection,
				diffuseColor.rgb,
				specularStrength,
				specularPower,
				diffuseLighting,
				specularLighting
			);
		}
		else if (light.lightType == 2) // Spot light
		{
			float radius = 5.0;
			float angle = 0.5 * c_time;

			light.worldPosition = float3(
				cos(angle) * radius+light.worldPosition.x,
				sin(angle) * radius+light.worldPosition.y,
				light.worldPosition.z
			);

			CalculateSpotLight(
				light,
				input.v_worldPos,
				finalNormal,
				viewDirection,
				diffuseColor.rgb,
				specularStrength,
				specularPower,
				diffuseLighting,
				specularLighting
			);
		}
	}

	// Add emissive contribution
	float3 emissiveLighting = diffuseColor.rgb * emissiveStrength;

	// Combine all lighting components
	float3 finalRGB = ambientLighting + diffuseLighting + specularLighting + emissiveLighting;
	float4 finalColor = float4( finalRGB, diffuseColor.a );

	if( finalColor.a <= 0.001 ) // a.k.a. "clip" in HLSL
	{
		discard;
	}

	if (c_debugInt == 1)
	{
	    finalColor.rgba = diffuseTexel.rgba;
	}
	else if(c_debugInt == 2 )
	{
		finalColor.rgba = surfaceColor.rgba;
	}
	else if(c_debugInt == 3 )
	{
	    finalColor.rgb = float3(uvCoords.x, uvCoords.y, 0.f);
	}
	else if(c_debugInt == 4 )
	{
		finalColor.rgb = EncodeXYZToRGB( surfaceTangentModelSpace );
	}
	else if(c_debugInt == 5 )
	{
		finalColor.rgb = EncodeXYZToRGB( surfaceBitangentModelSpace );
	}
	else if(c_debugInt == 6 )
	{
		finalColor.rgb = EncodeXYZToRGB( surfaceNormalModelSpace );
	}
	else if(c_debugInt == 7 )
	{
		finalColor.rgba = normalTexel.rgba;
	}
	else if(c_debugInt == 8 )
	{
		finalColor.rgb = EncodeXYZToRGB( pixelNormalTBNSpace );
	}
	else if(c_debugInt == 9 )
	{
		finalColor.rgb = EncodeXYZToRGB( pixelNormalWorldSpace );
	}
	else if(c_debugInt == 10 )
	{
		// Lit, but ignore normal maps (use surface normals only) -- see above
	}
	else if(c_debugInt == 11 || c_debugInt == 12 )
	{
		finalColor.rgb = lightStrength.xxx;
	}
	else if(c_debugInt == 13 )
	{
		finalColor.rgb = EncodeXYZToRGB( surfaceTangentWorldSpace );
	}
	else if(c_debugInt == 14 )
	{
		finalColor.rgb = EncodeXYZToRGB( surfaceBitangentWorldSpace );
	}
	else if(c_debugInt == 15 )
	{
		finalColor.rgb = EncodeXYZToRGB( surfaceNormalWorldSpace );
	}
	else if(c_debugInt == 16 )
	{
		float3 modelIBasisWorld = mul( c_modelToWorld, float4(1,0,0,0) ).xyz;
		finalColor.rgb = EncodeXYZToRGB( normalize( modelIBasisWorld.xyz ) );
	}
	else if(c_debugInt == 17 )
	{
		float3 modelJBasisWorld = mul( c_modelToWorld, float4(0,1,0,0) ).xyz;
		finalColor.rgb = EncodeXYZToRGB( normalize( modelJBasisWorld.xyz ) );
	}
	else if(c_debugInt == 18 )
	{
		float3 modelKBasisWorld = mul( c_modelToWorld, float4(0,0,1,0) ).xyz;
		finalColor.rgb = EncodeXYZToRGB( normalize( modelKBasisWorld.xyz ) );
	}
	else if(c_debugInt == 19 )
	{
		// Show SpecGlossEmit texture
		finalColor.rgba = specGlossEmitTexel.rgba;
	}
	else if(c_debugInt == 20 )
	{
		// Show specular strength (red channel)
		finalColor.rgb = specularStrength.xxx;
	}
	else if(c_debugInt == 21 )
	{
		// Show glossiness (green channel)
		finalColor.rgb = glossiness.xxx;
	}
	else if(c_debugInt == 22 )
	{
		// Show emissive strength (blue channel)
		finalColor.rgb = emissiveStrength.xxx;
	}
	else if(c_debugInt == 23 )
	{
		// Show emissive color contribution
		//finalColor.rgb = emissiveColor;
		finalColor.rgb = emissiveLighting;
	}

	return finalColor;
}

