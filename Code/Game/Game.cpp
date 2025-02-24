//----------------------------------------------------------------------------------------------------
// Game.cpp
//----------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------
#include "Game/Game.hpp"

#include <complex>

#include "Engine/Core/Clock.hpp"
#include "Engine/Core/EngineCommon.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"
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

    m_player->m_position     = Vec3(-2.f, 0.f, 1.f);
    m_firstCube->m_position  = Vec3(2.f, 2.f, 0.f);
    m_secondCube->m_position = Vec3(-2.f, -2.f, 0.f);
    m_sphere->m_position     = Vec3(10, -5, 1);
    m_grid->m_position       = Vec3(0, 0, 0);
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

    float deltaSeconds = (float)m_gameClock->GetDeltaSeconds();

    m_player->Update(deltaSeconds);
    m_firstCube->Update(deltaSeconds);
    m_secondCube->Update(deltaSeconds);
    m_sphere->Update(deltaSeconds);
    m_grid->Update(deltaSeconds);

    m_firstCube->m_orientation.m_pitchDegrees += 30.f * deltaSeconds;
    m_firstCube->m_orientation.m_rollDegrees += 30.f * deltaSeconds;

    float time       = static_cast<float>(m_gameClock->GetTotalSeconds() * 10.0);
    float colorValue = (std::sin(time) + 1.0f) * 0.5f * 255.0f;

    m_secondCube->m_color.r = (unsigned char)colorValue;
    m_secondCube->m_color.g = (unsigned char)colorValue;
    m_secondCube->m_color.b = (unsigned char)colorValue;

    m_sphere->m_orientation.m_yawDegrees += 45.f * deltaSeconds;
    //
    // DebuggerPrintf("%f\n", m_sphere->m_orientation.m_pitchDegrees);
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


    // m_firstCube->Render();
    // m_secondCube->Render();
    m_sphere->Render();
    m_grid->Render();

    g_theRenderer->SetModelConstants(m_player->GetModelToWorldTransform(), m_player->m_color);
    m_player->Render();
}

//----------------------------------------------------------------------------------------------------
void Game::SpawnPlayer()
{
    m_player = new Player(this);
}

//----------------------------------------------------------------------------------------------------
void Game::SpawnProp()
{
    Texture* texture = g_theRenderer->CreateOrGetTextureFromFile("Data/Images/TestUV.png");

    m_firstCube  = new Prop(this);
    m_secondCube = new Prop(this);
    m_sphere     = new Prop(this, texture);
    m_grid       = new Prop(this);

    m_firstCube->InitializeLocalVertsForCube();
    m_secondCube->InitializeLocalVertsForCube();
    m_sphere->InitializeLocalVertsForSphere();
    m_grid->InitializeLocalVertsForGrid();
}
