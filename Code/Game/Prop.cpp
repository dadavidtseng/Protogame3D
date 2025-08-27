//----------------------------------------------------------------------------------------------------
// Prop.cpp
//----------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------
#include "Game/Prop.hpp"

#include "Engine/Core/Clock.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"
#include "Engine/Renderer/VertexUtils.hpp"
#include "Engine/Math/AABB3.hpp"
#include "Engine/Renderer/BitmapFont.hpp"
#include "Engine/Renderer/Renderer.hpp"
#include "Game/Framework/GameCommon.hpp"
#include "ThirdParty/stb/stb_image.h"

//----------------------------------------------------------------------------------------------------
Prop::Prop(Game* owner, Texture const* texture)
    : Entity(owner),
      m_texture(texture)
{
}

//----------------------------------------------------------------------------------------------------
void Prop::Update(float const deltaSeconds)
{
    m_orientation.m_yawDegrees += m_angularVelocity.m_yawDegrees * deltaSeconds;
    m_orientation.m_pitchDegrees += m_angularVelocity.m_pitchDegrees * deltaSeconds;
    m_orientation.m_rollDegrees += m_angularVelocity.m_rollDegrees * deltaSeconds;
}

//----------------------------------------------------------------------------------------------------
void Prop::Render() const
{
    g_renderer->SetModelConstants(GetModelToWorldTransform(), m_color);
    g_renderer->SetBlendMode(eBlendMode::OPAQUE); //AL
    g_renderer->SetRasterizerMode(eRasterizerMode::SOLID_CULL_BACK);  //SOLID_CULL_NONE
    g_renderer->SetSamplerMode(eSamplerMode::POINT_CLAMP);
    g_renderer->SetDepthMode(eDepthMode::READ_WRITE_LESS_EQUAL);  //DISABLE
    g_renderer->BindTexture(m_texture);
    g_renderer->BindShader(g_renderer->CreateOrGetShaderFromFile("Data/Shaders/Bloom",eVertexType::VERTEX_PCU));
    g_renderer->DrawVertexArray(static_cast<int>(m_vertexes.size()), m_vertexes.data());
}

//----------------------------------------------------------------------------------------------------
void Prop::InitializeLocalVertsForCube()
{
    Vec3 const frontBottomLeft(0.5f, -0.5f, -0.5f);
    Vec3 const frontBottomRight(0.5f, 0.5f, -0.5f);
    Vec3 const frontTopLeft(0.5f, -0.5f, 0.5f);
    Vec3 const frontTopRight(0.5f, 0.5f, 0.5f);
    Vec3 const backBottomLeft(-0.5f, 0.5f, -0.5f);
    Vec3 const backBottomRight(-0.5f, -0.5f, -0.5f);
    Vec3 const backTopLeft(-0.5f, 0.5f, 0.5f);
    Vec3 const backTopRight(-0.5f, -0.5f, 0.5f);

    AddVertsForQuad3D(m_vertexes, frontBottomLeft, frontBottomRight, frontTopLeft, frontTopRight, Rgba8::RED);          // +X Red
    AddVertsForQuad3D(m_vertexes, backBottomLeft, backBottomRight, backTopLeft, backTopRight, Rgba8::CYAN);             // -X -Red (Cyan)
    AddVertsForQuad3D(m_vertexes, frontBottomRight, backBottomLeft, frontTopRight, backTopLeft, Rgba8::GREEN);          // -Y -Green (Magenta)
    AddVertsForQuad3D(m_vertexes, backBottomRight, frontBottomLeft, backTopRight, frontTopLeft, Rgba8::MAGENTA);        // +Y Green
    AddVertsForQuad3D(m_vertexes, frontTopLeft, frontTopRight, backTopRight, backTopLeft, Rgba8::BLUE);                 // +Z Blue
    AddVertsForQuad3D(m_vertexes, backBottomRight, backBottomLeft, frontBottomLeft, frontBottomRight, Rgba8::YELLOW);   // -Z -Blue (Yellow)
}

//----------------------------------------------------------------------------------------------------
void Prop::InitializeLocalVertsForSphere()
{
    float constexpr radius    = 0.5f;
    int constexpr   numSlices = 32;
    int constexpr   numStacks = 16;
    Rgba8 const     color     = Rgba8::WHITE;
    AABB2 const     UVs       = AABB2::ZERO_TO_ONE;

    AddVertsForSphere3D(m_vertexes, m_position, radius, color, UVs, numSlices, numStacks);
}

//----------------------------------------------------------------------------------------------------
void Prop::InitializeLocalVertsForGrid()
{
    float gridLineLength = 100.f;

    for (int i = -(int)gridLineLength / 2; i < (int)gridLineLength / 2; i++)
    {
        float lineWidth = 0.05f;
        if (i == 0) lineWidth = 0.3f;

        AABB3 boundsX = AABB3(Vec3(-gridLineLength / 2.f, -lineWidth / 2.f + (float)i, -lineWidth / 2.f), Vec3(gridLineLength / 2.f, lineWidth / 2.f + (float)i, lineWidth / 2.f));
        AABB3 boundsY = AABB3(Vec3(-lineWidth / 2.f + (float)i, -gridLineLength / 2.f, -lineWidth / 2.f), Vec3(lineWidth / 2.f + (float)i, gridLineLength / 2.f, lineWidth / 2.f));

        Rgba8 colorX = Rgba8::DARK_GREY;
        Rgba8 colorY = Rgba8::DARK_GREY;

        if (i % 5 == 0)
        {
            colorX = Rgba8::RED;
            colorY = Rgba8::GREEN;
        }

        AddVertsForAABB3D(m_vertexes, boundsX, colorX);
        AddVertsForAABB3D(m_vertexes, boundsY, colorY);
    }
}

//----------------------------------------------------------------------------------------------------
void Prop::InitializeLocalVertsForWorldCoordinateArrows()
{
    AddVertsForArrow3D(m_vertexes, m_position, m_position + Vec3::X_BASIS * 2.f, 0.6f, 0.25f, 0.4f, Rgba8::RED);
    AddVertsForArrow3D(m_vertexes, m_position, m_position + Vec3::Y_BASIS * 2.f, 0.6f, 0.25f, 0.4f, Rgba8::GREEN);
    AddVertsForArrow3D(m_vertexes, m_position, m_position + Vec3::Z_BASIS * 2.f, 0.6f, 0.25f, 0.4f, Rgba8::BLUE);
}

//----------------------------------------------------------------------------------------------------
void Prop::InitializeLocalVertsForText2D()
{
    // g_theBitmapFont->AddVertsForTextInBox2D(m_vertexes, "XXX", AABB2::ZERO_TO_ONE, 10.f);
    g_bitmapFont->AddVertsForText3DAtOriginXForward(m_vertexes, "ABCDEFGHIJKL", 1.f);
}
