//----------------------------------------------------------------------------------------------------
// Player.cpp
//----------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------
#include "Game/Player.hpp"

#include "Engine/Core/EngineCommon.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"
#include "Engine/Input/InputSystem.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Engine/Renderer/Camera.hpp"

//----------------------------------------------------------------------------------------------------
Player::Player(Game* owner)
    : Entity(owner)
{
    m_worldCamera = new Camera();

    m_worldCamera->SetPerspectiveGraphicView(2.f, 60.f, 0.1f, 100.f);

    m_worldCamera->SetPosition(Vec3(-2, 0, 0));

    Mat44 c2r;

    c2r.m_values[Mat44::Ix] = 0.f;
    c2r.m_values[Mat44::Iz] = 1.f;
    c2r.m_values[Mat44::Jx] = -1.f;
    c2r.m_values[Mat44::Jy] = 0.f;
    c2r.m_values[Mat44::Ky] = 1.f;
    c2r.m_values[Mat44::Kz] = 0.f;

    m_worldCamera->SetCameraToRenderTransform(c2r);
}

//----------------------------------------------------------------------------------------------------
Player::~Player()
{
    delete m_worldCamera;
    m_worldCamera = nullptr;
}

//----------------------------------------------------------------------------------------------------
void Player::Update(float deltaSeconds)
{
    if (g_theInput->IsKeyDown(KEYCODE_SHIFT)) deltaSeconds *= 10.f;
    if (g_theInput->IsKeyDown(KEYCODE_Q)) m_orientation.m_rollDegrees += deltaSeconds * 90.f;
    if (g_theInput->IsKeyDown(KEYCODE_E)) m_orientation.m_rollDegrees -= deltaSeconds * 90.f;
    if (g_theInput->IsKeyDown(KEYCODE_A)) m_position.y += deltaSeconds * 2.f;
    if (g_theInput->IsKeyDown(KEYCODE_D)) m_position.y -= deltaSeconds * 2.f;
    if (g_theInput->IsKeyDown(KEYCODE_W)) m_position.x += deltaSeconds * 2.f;
    if (g_theInput->IsKeyDown(KEYCODE_S)) m_position.x -= deltaSeconds * 2.f;
    if (g_theInput->IsKeyDown(KEYCODE_Z)) m_position.z -= deltaSeconds * 2.f;
    if (g_theInput->IsKeyDown(KEYCODE_C)) m_position.z += deltaSeconds * 2.f;

    m_orientation.m_rollDegrees = GetClamped(m_orientation.m_rollDegrees, -45.f, 45.f);
    m_orientation.m_yawDegrees += g_theInput->GetCursorClientDelta().x * 0.125f;
    m_orientation.m_pitchDegrees -= g_theInput->GetCursorClientDelta().y * 0.125f;
    m_orientation.m_pitchDegrees = GetClamped(m_orientation.m_pitchDegrees, -85.f, 85.f);

    // Synchronize player and camera's transform
    m_worldCamera->SetPositionAndOrientation(m_position, m_orientation);
}

//----------------------------------------------------------------------------------------------------
void Player::Render() const
{
}
