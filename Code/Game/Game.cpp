//----------------------------------------------------------------------------------------------------
// Game.cpp
//----------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------
#include "Game/Game.hpp"

#include <complex>

#include "Engine/Core/Clock.hpp"
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

    SpawnPlayer();
    SpawnProp();

    Vec2 const bottomLeft     = Vec2::ZERO;
    Vec2 const screenTopRight = Vec2(SCREEN_SIZE_X, SCREEN_SIZE_Y);

    m_screenCamera->SetOrthoGraphicView(bottomLeft, screenTopRight);
    // m_worldCamera->SetOrthoView(Vec2(-1, -1), Vec2(1, 1));
    // m_worldCamera->SetOrthoGraphicView(Vec2(-1, -1), Vec2(1, 1));

    m_gameClock = new Clock(Clock::GetSystemClock());

    m_firstCube->m_position  = Vec3(2.f, 2.f, 0.f);
    m_secondCube->m_position = Vec3(-2.f, -2.f, 0.f);
}

//----------------------------------------------------------------------------------------------------
Game::~Game()
{
    delete m_firstCube;
    m_firstCube = nullptr;

    delete m_player;
    m_player = nullptr;

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

    m_player->Update((float)m_gameClock->GetDeltaSeconds());
    m_firstCube->Update((float)m_gameClock->GetDeltaSeconds());
    m_secondCube->Update((float)m_gameClock->GetDeltaSeconds());

    float time              = static_cast<float>(m_gameClock->GetTotalSeconds()*10.0);
    float colorValue        = (std::sin(time) + 1.0f) * 0.5f * 255.0f;

    m_secondCube->m_color.r = (unsigned char)colorValue;
    m_secondCube->m_color.g = (unsigned char)colorValue;
    m_secondCube->m_color.b = (unsigned char)colorValue;
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

    g_theRenderer->BeginCamera(*m_player->GetCamera());

    if (!m_isAttractMode)
    {
        RenderUI();
    }

    g_theRenderer->EndCamera(*m_player->GetCamera());
}

//----------------------------------------------------------------------------------------------------
bool Game::IsAttractMode() const
{
    return m_isAttractMode;
}

//----------------------------------------------------------------------------------------------------
Clock* Game::GetGameClock() const
{
    return m_gameClock;
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
    DebugDrawRing(Vec2(800.f, 400.f), 300.f, 10.f, Rgba8::YELLOW);
}

//----------------------------------------------------------------------------------------------------
void Game::RenderUI() const
{
    // DebugDrawLine(Vec2(100.f, 100.f), Vec2(1500.f, 700.f), 10.f, Rgba8(100, 200, 100));
    // DebugDrawLine(Vec2(1500.f, 100.f), Vec2(100.f, 700.f), 10.f, Rgba8(100, 200, 100));

    g_theRenderer->SetModelConstants(m_firstCube->GetModelToWorldTransform(), m_firstCube->m_color);
    m_player->Render();
    m_firstCube->Render();
    g_theRenderer->SetModelConstants(m_secondCube->GetModelToWorldTransform(), m_secondCube->m_color);
    m_secondCube->Render();
}

//----------------------------------------------------------------------------------------------------
void Game::SpawnPlayer()
{
    m_player = new Player(this);
}

//----------------------------------------------------------------------------------------------------
void Game::SpawnProp()
{
    m_firstCube  = new Prop(this);
    m_secondCube = new Prop(this);
}
