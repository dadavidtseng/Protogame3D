//----------------------------------------------------------------------------------------------------
// Prop.cpp
//----------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------
#include "Game/Prop.hpp"

#include "Engine/Core/Clock.hpp"
#include "Engine/Core/VertexUtils.hpp"
#include "Engine/Renderer/Renderer.hpp"
#include "Game/GameCommon.hpp"

//----------------------------------------------------------------------------------------------------
Prop::Prop(Game* owner)
    : Entity(owner)
{
    InitializeLocalVerts();
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
    g_theRenderer->SetBlendMode(BlendMode::OPAQUE);
    g_theRenderer->BindTexture(nullptr);
    g_theRenderer->DrawVertexArray(static_cast<int>(m_vertexes.size()), m_vertexes.data());
}

//----------------------------------------------------------------------------------------------------
void Prop::InitializeLocalVerts()
{
    Vec3 const v000(-0.5f, -0.5f, -0.5f);
    Vec3 const v001(-0.5f, -0.5f, 0.5f);
    Vec3 const v010(-0.5f, 0.5f, -0.5f);
    Vec3 const v011(-0.5f, 0.5f, 0.5f);
    Vec3 const v100(0.5f, -0.5f, -0.5f);
    Vec3 const v101(0.5f, -0.5f, 0.5f);
    Vec3 const v110(0.5f, 0.5f, -0.5f);
    Vec3 const v111(0.5f, 0.5f, 0.5f);

    AddVertsForQuad3D(m_vertexes, v100, v101, v111, v110, Rgba8::RED);      // +X Red
    AddVertsForQuad3D(m_vertexes, v000, v010, v011, v001, Rgba8::CYAN);     // -X -Red (Cyan)
    AddVertsForQuad3D(m_vertexes, v010, v110, v111, v011, Rgba8::GREEN);    // +Y Green
    AddVertsForQuad3D(m_vertexes, v000, v001, v101, v100, Rgba8::MAGENTA);  // -Y -Green (Magenta)
    AddVertsForQuad3D(m_vertexes, v001, v011, v111, v101, Rgba8::BLUE);     // +Z Blue
    AddVertsForQuad3D(m_vertexes, v000, v100, v110, v010, Rgba8::YELLOW);   // -Z -Blue (Yellow)
}
