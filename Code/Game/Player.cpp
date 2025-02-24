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
    // --- 更新角度 ---
    // 1. 用滑鼠增量直接更新 yaw 與 pitch，然後夾制 pitch 的範圍
    m_orientation.m_yawDegrees   += g_theInput->GetCursorClientDelta().x * 0.125f;
    m_orientation.m_pitchDegrees -= g_theInput->GetCursorClientDelta().y * 0.125f;
    m_orientation.m_pitchDegrees  = GetClamped(m_orientation.m_pitchDegrees, -85.f, 85.f);

    // 2. 使用鍵盤控制 roll 的角速度，然後積分更新 roll
    m_angularVelocity.m_rollDegrees = 0.f;
    if (g_theInput->IsKeyDown(KEYCODE_Q))
        m_angularVelocity.m_rollDegrees = 90.f;   // 90°/秒向正方向旋轉
    if (g_theInput->IsKeyDown(KEYCODE_E))
        m_angularVelocity.m_rollDegrees = -90.f;  // 90°/秒向反方向旋轉

    // 積分更新 roll
    m_orientation.m_rollDegrees += m_angularVelocity.m_rollDegrees * deltaSeconds;
    m_orientation.m_rollDegrees = GetClamped(m_orientation.m_rollDegrees, -45.f, 45.f);

    // --- 根據攝影機方向計算移動向量 ---

    Vec3 forward ;  // 取得 "前進方向" 的單位向量
    Vec3 right   ;    // 取得 "右方向" 的單位向量
    Vec3 up      ;                 // Z 軸永遠向上
    m_orientation.GetAsVectors_IFwd_JLeft_KUp(forward, right, up);

    // --- 設定速度 ---
    m_velocity = Vec3::ZERO;
    float moveSpeed = 2.f;

    if (g_theInput->IsKeyDown(KEYCODE_W)) m_velocity += forward * moveSpeed;  // 向前
    if (g_theInput->IsKeyDown(KEYCODE_S)) m_velocity -= forward * moveSpeed;  // 向後
    if (g_theInput->IsKeyDown(KEYCODE_A)) m_velocity += right * moveSpeed;    // 向左
    if (g_theInput->IsKeyDown(KEYCODE_D)) m_velocity -= right * moveSpeed;    // 向右
    if (g_theInput->IsKeyDown(KEYCODE_C)) m_velocity += up * moveSpeed;       // 向上
    if (g_theInput->IsKeyDown(KEYCODE_Z)) m_velocity -= up * moveSpeed;       // 向下

    // 若按下 Shift，加速移動
    if (g_theInput->IsKeyDown(KEYCODE_SHIFT))
        deltaSeconds *= 10.f;

    // 積分更新位置
    m_position += m_velocity * deltaSeconds;

    // --- 同步攝影機 ---
    m_worldCamera->SetPositionAndOrientation(m_position, m_orientation);
}

//----------------------------------------------------------------------------------------------------
void Player::Render() const
{
}
