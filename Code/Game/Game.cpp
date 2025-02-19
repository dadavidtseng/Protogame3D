//----------------------------------------------------------------------------------------------------
// Game.cpp
//----------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------
#include "Game/Game.hpp"

#include "Engine/Core/EngineCommon.hpp"
#include "Engine/Input/InputSystem.hpp"
#include "Engine/Renderer/Renderer.hpp"
#include "Game/GameCommon.hpp"
#include "Game/Player.hpp"
#include "Game/Prop.hpp"

//----------------------------------------------------------------------------------------------------
Game::Game()
{
    g_theEventSystem->FireEvent("help");

    m_screenCamera = new Camera();
    m_worldCamera  = new Camera();

    SpawnPlayer();
    SpawnProp();

    Vec2 const bottomLeft     = Vec2::ZERO;
    Vec2 const screenTopRight = Vec2(SCREEN_SIZE_X, SCREEN_SIZE_Y);

    m_screenCamera->SetOrthoGraphicView(bottomLeft, screenTopRight);
    // m_worldCamera->SetOrthoView(Vec2(-1, -1), Vec2(1, 1));
    m_worldCamera->SetOrthoGraphicView(Vec2(-1, -1), Vec2(1, 1));

    m_worldCamera->SetPerspectiveGraphicView(2.f, 60.f, 0.1f, 100.f);

    // m_worldCamera->SetPosition(Vec3(-2,0,0));

    Mat44 c2r;

    c2r.m_values[Mat44::Ix] = 0.f;
    c2r.m_values[Mat44::Iz] = 1.f;
    c2r.m_values[Mat44::Jx] = -1.f;
    c2r.m_values[Mat44::Jy] = 0.f;
    c2r.m_values[Mat44::Ky] = 1.f;
    c2r.m_values[Mat44::Kz] = 0.f;
    m_worldCamera->SetCameraToRenderTransform(c2r);


}

Game::~Game()
{
    delete m_prop;
    m_prop = nullptr;

    delete m_player;
    m_player = nullptr;

    delete m_worldCamera;
    m_worldCamera = nullptr;

    delete m_screenCamera;
    m_screenCamera = nullptr;
}

//----------------------------------------------------------------------------------------------------
void Game::Update()
{
    // #TODO: Select keyboard or controller
    UpdateFromKeyBoard();
    UpdateFromController();
    AdjustForPauseAndTimeDistortion();
}

//----------------------------------------------------------------------------------------------------
void Game::Render() const
{
    g_theRenderer->BeginCamera(*m_screenCamera);

    if (m_isAttractMode)
    {
        RenderAttractMode();
    }

    g_theRenderer->EndCamera(*m_screenCamera);

    g_theRenderer->BeginCamera(*m_worldCamera);

    if (!m_isAttractMode)
    {
        RenderUI();
    }

    g_theRenderer->EndCamera(*m_worldCamera);
}

//----------------------------------------------------------------------------------------------------
void Game::UpdateFromKeyBoard()
{
    if (m_isDevConsoleMode == true)
    {
        return;
    }

    if (g_theInput->WasKeyJustPressed(KEYCODE_SPACE))
    {
        m_isAttractMode    = false;
        m_isDevConsoleMode = true;
        // SoundID const clickSound = g_theAudio->CreateOrGetSound("Data/Audio/TestSound.mp3");
        // g_theAudio->StartSound(clickSound, false, 1.f, 0.f, 0.5f);
    }

    if (g_theInput->WasKeyJustPressed(KEYCODE_ESC))
    {
        m_isAttractMode = true;
        // SoundID const clickSound = g_theAudio->CreateOrGetSound("Data/Audio/TestSound.mp3");
        // g_theAudio->StartSound(clickSound);
    }
}

//----------------------------------------------------------------------------------------------------
void Game::UpdateFromController()
{
    XboxController const& controller = g_theInput->GetController(0);

    UNUSED(controller)
}

//----------------------------------------------------------------------------------------------------
void Game::AdjustForPauseAndTimeDistortion()
{
}

//----------------------------------------------------------------------------------------------------
void Game::RenderAttractMode() const
{
    DebugDrawRing(Vec2(800.f, 400.f), 300.f, 10.f, Rgba8(255, 127, 0));
}

//----------------------------------------------------------------------------------------------------
void Game::RenderUI() const
{
    // DebugDrawLine(Vec2(100.f, 100.f), Vec2(1500.f, 700.f), 10.f, Rgba8(100, 200, 100));
    // DebugDrawLine(Vec2(1500.f, 100.f), Vec2(100.f, 700.f), 10.f, Rgba8(100, 200, 100));

    m_prop->Render();
}

//----------------------------------------------------------------------------------------------------
void Game::SpawnPlayer()
{
    m_player = new Player(this);
}

//----------------------------------------------------------------------------------------------------
void Game::SpawnProp()
{
    m_prop = new Prop(this);
}
