//----------------------------------------------------------------------------------------------------
// Prop.cpp
//----------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------
#include "Game/Prop.hpp"

#include "GameCommon.hpp"
#include "Engine/Core/VertexUtils.hpp"
#include "Engine/Renderer/Renderer.hpp"

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

    g_theRenderer->SetBlendMode(BlendMode::OPAQUE);
    g_theRenderer->BindTexture(nullptr);
    g_theRenderer->DrawVertexArray(static_cast<int>(m_vertexesClone.size()), m_vertexesClone.data());
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

    Vec3 const w000(1.5f, -0.5f, -0.5f);
    Vec3 const w001(1.5f, -0.5f, 0.5f);
    Vec3 const w010(1.5f, 0.5f, -0.5f);
    Vec3 const w011(1.5f, 0.5f, 0.5f);
    Vec3 const w100(2.f, -0.5f, -0.5f);
    Vec3 const w101(2.f, -0.5f, 0.5f);
    Vec3 const w110(2.f, 0.5f, -0.5f);
    Vec3 const w111(2.f, 0.5f, 0.5f);

    AddVertsForQuad3D(m_vertexesClone, w100, w101, w111, w110, Rgba8::CYAN);      // +X Red
    AddVertsForQuad3D(m_vertexesClone, w000, w010, w011, w001, Rgba8::CYAN);     // -X -Red (Cyan)
    AddVertsForQuad3D(m_vertexesClone, w010, w110, w111, w011, Rgba8::CYAN);    // +Y Green
    AddVertsForQuad3D(m_vertexesClone, w000, w001, w101, w100, Rgba8::CYAN);  // -Y -Green (Magenta)
    AddVertsForQuad3D(m_vertexesClone, w001, w011, w111, w101, Rgba8::CYAN);     // +Z Blue
    AddVertsForQuad3D(m_vertexesClone, w000, w100, w110, w010, Rgba8::CYAN);   // -Z -Blue (Yellow)
}
