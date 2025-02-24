//----------------------------------------------------------------------------------------------------
// Prop.cpp
//----------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------
#include "Game/Prop.hpp"

#include "Engine/Core/Clock.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"
#include "Engine/Core/VertexUtils.hpp"
#include "Engine/Math/AABB3.hpp"
#include "Engine/Renderer/Renderer.hpp"
#include "Game/GameCommon.hpp"

//----------------------------------------------------------------------------------------------------
Prop::Prop(Game* owner, Texture* texture)
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
    g_theRenderer->SetModelConstants(GetModelToWorldTransform(), m_color);
    g_theRenderer->SetBlendMode(BlendMode::OPAQUE);
    // g_theRenderer->BindTexture(nullptr);
    g_theRenderer->BindTexture(m_texture);
    g_theRenderer->DrawVertexArray(static_cast<int>(m_vertexes.size()), m_vertexes.data());
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

    AddVertsForQuad3D(m_vertexes, frontBottomLeft, frontBottomRight, frontTopRight, frontTopLeft, Rgba8::RED);          // +X Red
    AddVertsForQuad3D(m_vertexes, backBottomLeft, backBottomRight, backTopRight, backTopLeft, Rgba8::CYAN);             // -X -Red (Cyan)
    AddVertsForQuad3D(m_vertexes, frontBottomRight, backBottomLeft, backTopLeft, frontTopRight, Rgba8::GREEN);          // +Y Green
    AddVertsForQuad3D(m_vertexes, backBottomRight, frontBottomLeft, frontTopLeft, backTopRight, Rgba8::MAGENTA);        // -Y -Green (Magenta)
    AddVertsForQuad3D(m_vertexes, frontTopLeft, frontTopRight, backTopLeft, backTopRight, Rgba8::BLUE);                 // +Z Blue
    AddVertsForQuad3D(m_vertexes, backBottomRight, backBottomLeft, frontBottomRight, frontBottomLeft, Rgba8::YELLOW);   // -Z -Blue (Yellow)
}

//----------------------------------------------------------------------------------------------------
void Prop::InitializeLocalVertsForSphere()
{
    float constexpr radius  = 0.5f;
    int constexpr numSlices = 32;
    int constexpr numStacks = 16;
    Rgba8 const color       = Rgba8::WHITE;
    AABB2 const UVs         = AABB2::ZERO_TO_ONE;

    AddVertsForSphere3D(m_vertexes, radius, color, UVs, numSlices, numStacks);
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
