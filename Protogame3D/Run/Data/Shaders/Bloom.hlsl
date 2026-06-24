//------------------------------------------------------------------------------------------------
// Default (unlit) shader for Squirrel Eiserloh's C34 SD student Engine (Spring 2025)
// MODIFIED TO SUPPORT MULTIPLE RENDER TARGETS (MRT) for emissive bloom effects
//
// Requires Vertex_PCU vertex data (Position, Color, UVs).
//------------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------------------
// Input to the Vertex shader stage.
//------------------------------------------------------------------------------------------------
struct VertexInput
{
	float3	a_position		: VERTEX_POSITION;
	float4	a_color			: VERTEX_COLOR;
	float2	a_uvTexCoords	: VERTEX_UVTEXCOORDS;
	uint	a_vertexID		: SV_VertexID;
};

//------------------------------------------------------------------------------------------------
// Output passed from the Vertex shader into the Pixel/fragment shader.
//------------------------------------------------------------------------------------------------
struct VertexOutPixelIn
{
	float4 v_position		: SV_Position;
	float4 v_color			: SURFACE_COLOR;
	float2 v_uvTexCoords	: SURFACE_UVTEXCOORDS;
};

//------------------------------------------------------------------------------------------------
// MULTIPLE RENDER TARGET OUTPUT STRUCTURE
// This allows us to output to both the main color buffer and the emissive buffer simultaneously
//------------------------------------------------------------------------------------------------
struct PixelOutput
{
	float4 color : SV_Target0;		// 主要顏色輸出 (標準渲染結果)
	float4 emissive : SV_Target1;	// 發光顏色輸出 (用於 bloom 效果)
};

//------------------------------------------------------------------------------------------------
// CONSTANT BUFFERS
//------------------------------------------------------------------------------------------------
cbuffer CameraConstants : register(b3)
{
	float4x4	c_worldToCamera;
	float4x4	c_cameraToRender;
	float4x4	c_renderToClip;
};

cbuffer ModelConstants : register(b4)
{
	float4x4	c_modelToWorld;
	float4		c_modelTint;
};

//------------------------------------------------------------------------------------------------
// TEXTURE and SAMPLER constants
//------------------------------------------------------------------------------------------------
Texture2D<float4>	t_diffuseTexture : register(t0);
SamplerState		s_diffuseSampler : register(s0);

//------------------------------------------------------------------------------------------------
// VERTEX SHADER (VS)
//------------------------------------------------------------------------------------------------
VertexOutPixelIn VertexMain( VertexInput input )
{
	VertexOutPixelIn output;

	float4 modelPos = float4( input.a_position, 1.0 );
	float4 worldPos		= mul( c_modelToWorld, modelPos );
	float4 cameraPos	= mul( c_worldToCamera, worldPos );
	float4 renderPos	= mul( c_cameraToRender, cameraPos );
	float4 clipPos		= mul( c_renderToClip, renderPos );

	output.v_position		= clipPos;
	output.v_color			= input.a_color;
	output.v_uvTexCoords	= input.a_uvTexCoords;

	return output;
}

//------------------------------------------------------------------------------------------------
// PIXEL SHADER (PS) - MULTIPLE RENDER TARGET VERSION
//
// 現在輸出兩個顏色：
// 1. 正常的最終顏色到 SV_Target0 (主要渲染緩衝區)
// 2. 發光顏色到 SV_Target1 (emissive 緩衝區，用於 bloom 後處理)
//------------------------------------------------------------------------------------------------
PixelOutput PixelMain( VertexOutPixelIn input )
{
	// 取得 UV 座標
	float2 uvCoords = input.v_uvTexCoords;

	// 採樣漫反射貼圖
	float4 diffuseTexel = t_diffuseTexture.Sample( s_diffuseSampler, uvCoords );
	float4 surfaceColor = input.v_color;
	float4 modelColor = c_modelTint;

	// 計算最終漫反射顏色
	float4 diffuseColor = diffuseTexel * surfaceColor * modelColor;
	float4 finalColor = diffuseColor;

	// Alpha 測試 (丟棄透明像素)
	if( finalColor.a <= 0.001 )
	{
		discard;
	}

	// 準備輸出結構
	PixelOutput output;

	// 主要顏色輸出 (SV_Target0)
	output.color = finalColor;

	// 發光顏色輸出 (SV_Target1)
	// 方案1: 基於亮度的自動發光檢測
	float luminance = dot(finalColor.rgb, float3(0.299, 0.587, 0.114));
	if (luminance > 0.8)  // 只有很亮的像素才會發光
	{
		// 發光強度基於超過閾值的亮度
		float emissiveIntensity = (luminance - 0.8) / 0.2;  // 0.8-1.0 映射到 0.0-1.0
		output.emissive = float4(finalColor.rgb * emissiveIntensity * 2.0, 1.0);
	}
	else
	{
		output.emissive = float4(0, 0, 0, 1);  // 不發光
	}

	// 方案2: 基於顏色通道的發光檢測 (可選用)
	// 如果您想要特定顏色發光，可以替換上面的方案1：

	// 紅色物件發光
	if (finalColor.r > 0.7 && finalColor.g < 0.3 && finalColor.b < 0.3)
	{
		output.emissive = float4(finalColor.rgb * 1.5, 1.0);
	}
	// 藍色物件發光
	else if (finalColor.b > 0.7 && finalColor.r < 0.3 && finalColor.g < 0.3)
	{
		output.emissive = float4(finalColor.rgb * 1.5, 1.0);
	}
	else
	{
		output.emissive = float4(0, 0, 0, 1);
	}


	// 方案3: 基於 vertex color 的發光檢測 (可選用)
	// 如果您想通過 vertex color 來控制發光：

	// 使用 vertex color 的 alpha 通道作為發光強度控制
	float emissiveStrength = surfaceColor.a;
	if (emissiveStrength > 0.5)
	{
		output.emissive = float4(finalColor.rgb * emissiveStrength, 1.0);
	}
	else
	{
		output.emissive = float4(0, 0, 0, 1);
	}


	return output;
}