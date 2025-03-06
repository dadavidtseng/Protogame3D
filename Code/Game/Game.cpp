//----------------------------------------------------------------------------------------------------
// Game.cpp
//----------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------
#include "Game/Game.hpp"

#include "Engine/Core/Clock.hpp"
#include "Engine/Core/EngineCommon.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"
#include "Engine/Input/InputSystem.hpp"
#include "Engine/Renderer/BitmapFont.hpp"
#include "Engine/Renderer/DebugRenderSystem.hpp"
#include "Engine/Renderer/Renderer.hpp"
#include "Game/App.hpp"
#include "Game/GameCommon.hpp"
#include "Game/Player.hpp"
#include "Game/Prop.hpp"

//----------------------------------------------------------------------------------------------------
Game::Game()
{
    g_theEventSystem->FireEvent("help");

    SpawnPlayer();
    SpawnProp();

    m_screenCamera = new Camera();

    Vec2 const bottomLeft     = Vec2::ZERO;
    Vec2 const screenTopRight = Vec2(SCREEN_SIZE_X, SCREEN_SIZE_Y);

    m_screenCamera->SetOrthoGraphicView(bottomLeft, screenTopRight);

    m_gameClock = new Clock(Clock::GetSystemClock());

    m_player->m_position     = Vec3(-2.f, 0.f, 1.f);
    m_firstCube->m_position  = Vec3(2.f, 2.f, 0.f);
    m_secondCube->m_position = Vec3(-2.f, -2.f, 0.f);
    m_sphere->m_position     = Vec3(10, -5, 1);
    m_grid->m_position       = Vec3::ZERO;
    m_cylinder->m_position   = Vec3(1, 5, 3);
    m_text->m_position       = Vec3(0, 0, 10);

    DebugAddWorldBasis(Mat44(), -1);
}

//----------------------------------------------------------------------------------------------------
Game::~Game()
{
    delete m_gameClock;
    m_gameClock = nullptr;

    delete m_grid;
    m_grid = nullptr;

    delete m_sphere;
    m_sphere = nullptr;

    delete m_secondCube;
    m_secondCube = nullptr;

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
    float const gameDeltaSeconds   = static_cast<float>(m_gameClock->GetDeltaSeconds());
    float const systemDeltaSeconds = static_cast<float>(Clock::GetSystemClock().GetDeltaSeconds());

    // #TODO: Select keyboard or controller

    UpdateEntities(gameDeltaSeconds, systemDeltaSeconds);
    UpdateFromKeyBoard();
    UpdateFromController();
}

//----------------------------------------------------------------------------------------------------
void Game::Render() const
{
    //-Start-of-Game-Camera---------------------------------------------------------------------------

    g_theRenderer->BeginCamera(*m_player->GetCamera());

    if (m_gameState == eGameState::Game)
    {
        RenderEntities();
    }

    g_theRenderer->EndCamera(*m_player->GetCamera());

    //-End-of-Game-Camera-----------------------------------------------------------------------------
    //------------------------------------------------------------------------------------------------
    DebugRenderWorld(*m_player->GetCamera());
    //------------------------------------------------------------------------------------------------
    //-Start-of-Screen-Camera-------------------------------------------------------------------------

    g_theRenderer->BeginCamera(*m_screenCamera);

    if (m_gameState == eGameState::Attract)
    {
        RenderAttractMode();
    }

    g_theRenderer->EndCamera(*m_screenCamera);

    //-End-of-Screen-Camera---------------------------------------------------------------------------

    DebugRenderScreen(*m_screenCamera);
}

//----------------------------------------------------------------------------------------------------
bool Game::IsAttractMode() const
{
    return m_gameState == eGameState::Attract;
}

//----------------------------------------------------------------------------------------------------
void Game::UpdateFromKeyBoard()
{
    if (m_gameState == eGameState::Attract)
    {
        if (g_theInput->WasKeyJustPressed(KEYCODE_ESC))
        {
            App::RequestQuit();
        }

        if (g_theInput->WasKeyJustPressed(KEYCODE_SPACE))
        {
            m_gameState = eGameState::Game;
        }
    }

    if (m_gameState == eGameState::Game)
    {
        if (g_theInput->WasKeyJustPressed(KEYCODE_ESC))
        {
            m_gameState = eGameState::Attract;
            FireEvent("Command_DebugRenderClear");
        }

        if (g_theInput->WasKeyJustPressed(KEYCODE_P))
        {
            m_gameClock->TogglePause();
        }

        if (g_theInput->WasKeyJustPressed(KEYCODE_O))
        {
            m_gameClock->StepSingleFrame();
        }

        if (g_theInput->IsKeyDown(KEYCODE_T))
        {
            m_gameClock->SetTimeScale(0.1f);
        }

        if (g_theInput->WasKeyJustReleased(KEYCODE_T))
        {
            m_gameClock->SetTimeScale(1.f);
        }

        if (g_theInput->WasKeyJustReleased(NUMCODE_1))
        {
            DebugAddWorldLine(m_player->m_position + Vec3::X_BASIS * 0.5f, m_player->m_position + Vec3::X_BASIS * 20.5f, 0.01f, 10.f, Rgba8(255, 255, 0), Rgba8(255, 255, 0), DebugRenderMode::X_RAY);
        }

        if (g_theInput->WasKeyJustReleased(NUMCODE_2))
        {
            DebugAddWorldArrow(m_player->m_position, m_player->m_position + Vec3::X_BASIS, 1.f, 5);
        }

        if (g_theInput->WasKeyJustReleased(NUMCODE_3))
        {
            DebugAddWorldWireSphere(m_player->m_position, 1.f, 5.f, Rgba8::YELLOW, Rgba8::RED);
        }

        if (g_theInput->WasKeyJustReleased(NUMCODE_4))
        {
            DebugAddBillboardText("ABCDEFG\n", m_player->m_position, 10.f, Vec2(0.5f, 0.5f), 10.f);
        }

        if (g_theInput->WasKeyJustReleased(NUMCODE_5))
        {
            DebugAddScreenText("ABCDEFG\n", Vec2(SCREEN_SIZE_X / 2, SCREEN_SIZE_Y / 2), 10.f, Vec2(0.5f, 0.5f), 10.f);
        }

        if (g_theInput->WasKeyJustReleased(NUMCODE_6))
        {
            DebugAddMessage("ABCDEFG\n", -1.f);
        }

        if (g_theInput->WasKeyJustReleased(NUMCODE_7))
        {
            DebugAddWorldWireCylinder(m_player->m_position, m_player->m_position + Vec3::Z_BASIS * 3, 1.f, 10.f, Rgba8::WHITE, Rgba8::RED);
        }

        if (g_theInput->WasKeyJustReleased(NUMCODE_8))
        {
            DebugAddWorldPoint(Vec3(m_player->m_position.x, m_player->m_position.y, 0.f), 0.5f, 60.f, Rgba8(150, 75, 0));
        }
    }
}

//----------------------------------------------------------------------------------------------------
void Game::UpdateFromController()
{
    XboxController const& controller = g_theInput->GetController(0);

    if (m_gameState == eGameState::Attract)
    {
        if (controller.WasButtonJustPressed(XBOX_BUTTON_BACK))
        {
            App::RequestQuit();
        }

        if (controller.WasButtonJustPressed(XBOX_BUTTON_START))
        {
            m_gameState = eGameState::Game;
        }
    }

    if (m_gameState == eGameState::Game)
    {
        if (controller.WasButtonJustPressed(XBOX_BUTTON_BACK))
        {
            m_gameState = eGameState::Attract;
        }

        if (controller.WasButtonJustPressed(XBOX_BUTTON_B))
        {
            m_gameClock->TogglePause();
        }

        if (controller.WasButtonJustPressed(XBOX_BUTTON_Y))
        {
            m_gameClock->StepSingleFrame();
        }

        if (controller.WasButtonJustPressed(XBOX_BUTTON_X))
        {
            m_gameClock->SetTimeScale(0.1f);
        }

        if (controller.WasButtonJustReleased(XBOX_BUTTON_X))
        {
            m_gameClock->SetTimeScale(1.f);
        }
    }
}

//----------------------------------------------------------------------------------------------------
void Game::UpdateEntities(float const gameDeltaSeconds, float const systemDeltaSeconds) const
{
    m_player->Update(systemDeltaSeconds);
    m_firstCube->Update(gameDeltaSeconds);
    m_secondCube->Update(gameDeltaSeconds);
    m_sphere->Update(gameDeltaSeconds);
    m_grid->Update(gameDeltaSeconds);

    m_firstCube->m_orientation.m_pitchDegrees += 30.f * gameDeltaSeconds;
    m_firstCube->m_orientation.m_rollDegrees += 30.f * gameDeltaSeconds;

    float const time       = static_cast<float>(m_gameClock->GetTotalSeconds());
    float const colorValue = (sinf(time) + 1.0f) * 0.5f * 255.0f;

    m_secondCube->m_color.r = static_cast<unsigned char>(colorValue);
    m_secondCube->m_color.g = static_cast<unsigned char>(colorValue);
    m_secondCube->m_color.b = static_cast<unsigned char>(colorValue);

    m_sphere->m_orientation.m_yawDegrees += 45.f * gameDeltaSeconds;
}

//----------------------------------------------------------------------------------------------------
void Game::RenderAttractMode() const
{
    DebugDrawRing(Vec2(800.f, 400.f), 300.f, 10.f, Rgba8::YELLOW);
}

//----------------------------------------------------------------------------------------------------
void Game::RenderEntities() const
{
    m_firstCube->Render();
    m_secondCube->Render();
    m_sphere->Render();
    m_grid->Render();
    m_cylinder->Render();
    // g_theRenderer->SetModelConstants(GetBillboardMatrix(eBillboardType::FULL_OPPOSING, m_player->GetCamera()->GetCameraToWorldTransform(), m_text->m_position));
    m_text->Render();
    // m_arrow->Render();

    g_theRenderer->SetModelConstants(m_player->GetModelToWorldTransform());
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
    m_cylinder   = new Prop(this);
    m_text       = new Prop(this, &g_theBitmapFont->GetTexture());
    m_arrow      = new Prop(this);

    m_firstCube->InitializeLocalVertsForCube();
    m_secondCube->InitializeLocalVertsForCube();
    m_sphere->InitializeLocalVertsForSphere();
    m_grid->InitializeLocalVertsForGrid();
    m_cylinder->InitializeLocalVertsForCylinder();
    m_text->InitializeLocalVertsForText2D();
    m_arrow->InitializeLocalVertsForWorldCoordinateArrows();
}
