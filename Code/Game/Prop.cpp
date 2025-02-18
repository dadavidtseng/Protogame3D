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

    VertexList vertices;
    // 定義立方體的 8 個頂點 (中心在原點, 邊長 1)
    Vec3 v000(-0.5f, -0.5f, -0.5f); // 左下後
    Vec3 v001(-0.5f, -0.5f, 0.5f); // 左下前
    Vec3 v010(-0.5f, 0.5f, -0.5f); // 左上後
    Vec3 v011(-0.5f, 0.5f, 0.5f); // 左上前
    Vec3 v100(0.5f, -0.5f, -0.5f); // 右下後
    Vec3 v101(0.5f, -0.5f, 0.5f); // 右下前
    Vec3 v110(0.5f, 0.5f, -0.5f); // 右上後
    Vec3 v111(0.5f, 0.5f, 0.5f); // 右上前

    // UV 區域 (假設整張貼圖對應到整個面)
    AABB2 uv(Vec2(0.f, 0.f), Vec2(1.f, 1.f));

    // 添加 6 個面，每個面由 2 個三角形組成
    AddVertsForQuad3D(vertices, v100, v101, v111, v110, Rgba8(255, 0, 0), uv);     // +X (紅)
    AddVertsForQuad3D(vertices, v000, v010, v011, v001, Rgba8(0, 255, 255), uv);   // -X (青)
    AddVertsForQuad3D(vertices, v010, v110, v111, v011, Rgba8(0, 255, 0), uv);     // +Y (綠)
    AddVertsForQuad3D(vertices, v000, v001, v101, v100, Rgba8(255, 0, 255), uv);   // -Y (洋紅)
    AddVertsForQuad3D(vertices, v001, v011, v111, v101, Rgba8(0, 0, 255), uv);     // +Z (藍)
    AddVertsForQuad3D(vertices, v000, v100, v110, v010, Rgba8(255, 255, 0), uv);   // -Z (黃)

    g_theRenderer->BindTexture(nullptr);
    g_theRenderer->DrawVertexArray(static_cast<int>(m_vertexes.size()), m_vertexes.data());
}
