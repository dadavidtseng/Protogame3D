//----------------------------------------------------------------------------------------------------
// Game.cpp
//----------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------
#include "Game/Game.hpp"

#include "Engine/Core/Clock.hpp"
#include "Engine/Core/EngineCommon.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"
#include "Engine/Input/InputSystem.hpp"
#include "Engine/Platform/Window.hpp"
#include "Engine/Renderer/BitmapFont.hpp"
#include "Engine/Renderer/DebugRenderSystem.hpp"
#include "Engine/Renderer/Renderer.hpp"
#include "Engine/Resource/ResourceSubsystem.hpp"
#include "Engine/Resource/Resource/ModelResource.hpp"
#include "Engine/Resource/ResourceLoader/ObjModelLoader.hpp"
#include "Game/Framework/App.hpp"
#include "Game/Framework/GameCommon.hpp"
#include "Game/Player.hpp"
#include "Game/Prop.hpp"

//----------------------------------------------------------------------------------------------------
Game::Game()
{
    SpawnPlayer();
    SpawnProp();

    m_screenCamera = new Camera();

    Vec2 const bottomLeft = Vec2::ZERO;
    // Vec2 const screenTopRight = Vec2(SCREEN_SIZE_X, SCREEN_SIZE_Y);
    Vec2 clientDimensions = Window::s_mainWindow->GetClientDimensions();

    m_screenCamera->SetOrthoGraphicView(bottomLeft, clientDimensions);
    m_screenCamera->SetNormalizedViewport(AABB2::ZERO_TO_ONE);
    m_gameClock = new Clock(Clock::GetSystemClock());

    m_player->m_position     = Vec3(-2.f, 0.f, 1.f);
    m_firstCube->m_position  = Vec3(2.f, 2.f, 0.f);
    m_secondCube->m_position = Vec3(-2.f, -2.f, 0.f);
    m_sphere->m_position     = Vec3(10, -5, 1);
    m_grid->m_position       = Vec3::ZERO;

    DebugAddWorldBasis(Mat44(), -1.f);

    Mat44 transform;

    transform.SetIJKT3D(-Vec3::Y_BASIS, Vec3::X_BASIS, Vec3::Z_BASIS, Vec3(0.25f, 0.f, 0.25f));
    DebugAddWorldText("X-Forward", transform, 0.25f, Vec2::ONE, -1.f, Rgba8::RED);

    transform.SetIJKT3D(-Vec3::X_BASIS, -Vec3::Y_BASIS, Vec3::Z_BASIS, Vec3(0.f, 0.25f, 0.5f));
    DebugAddWorldText("Y-Left", transform, 0.25f, Vec2::ZERO, -1.f, Rgba8::GREEN);

    transform.SetIJKT3D(-Vec3::X_BASIS, Vec3::Z_BASIS, Vec3::Y_BASIS, Vec3(0.f, -0.25f, 0.25f));
    DebugAddWorldText("Z-Up", transform, 0.25f, Vec2(1.f, 0.f), -1.f, Rgba8::BLUE);
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

    g_renderer->BeginCamera(*m_player->GetCamera());

    if (m_gameState == eGameState::GAME)
    {
        RenderEntities();
        Vec2 screenDimensions = Window::s_mainWindow->GetScreenDimensions();
        Vec2 windowDimensions = Window::s_mainWindow->GetWindowDimensions();
        Vec2 clientDimensions = Window::s_mainWindow->GetClientDimensions();
        Vec2 windowPosition   = Window::s_mainWindow->GetWindowPosition();
        Vec2 clientPosition   = Window::s_mainWindow->GetClientPosition();
        DebugAddScreenText(Stringf("ScreenDimensions=(%.1f,%.1f)", screenDimensions.x, screenDimensions.y), Vec2(0, 0), 20.f, Vec2::ZERO, 0.f);
        DebugAddScreenText(Stringf("WindowDimensions=(%.1f,%.1f)", windowDimensions.x, windowDimensions.y), Vec2(0, 20), 20.f, Vec2::ZERO, 0.f);
        DebugAddScreenText(Stringf("ClientDimensions=(%.1f,%.1f)", clientDimensions.x, clientDimensions.y), Vec2(0, 40), 20.f, Vec2::ZERO, 0.f);
        DebugAddScreenText(Stringf("WindowPosition=(%.1f,%.1f)", windowPosition.x, windowPosition.y), Vec2(0, 60), 20.f, Vec2::ZERO, 0.f);
        DebugAddScreenText(Stringf("ClientPosition=(%.1f,%.1f)", clientPosition.x, clientPosition.y), Vec2(0, 80), 20.f, Vec2::ZERO, 0.f);
    }

    g_renderer->EndCamera(*m_player->GetCamera());

    //-End-of-Game-Camera-----------------------------------------------------------------------------
    //------------------------------------------------------------------------------------------------
    if (m_gameState == eGameState::GAME)
    {
        DebugRenderWorld(*m_player->GetCamera());
    }
    //------------------------------------------------------------------------------------------------
    //-Start-of-Screen-Camera-------------------------------------------------------------------------

    g_renderer->BeginCamera(*m_screenCamera);

    if (m_gameState == eGameState::ATTRACT)
    {
        RenderAttractMode();
    }

    g_renderer->EndCamera(*m_screenCamera);

    //-End-of-Screen-Camera---------------------------------------------------------------------------
    if (m_gameState == eGameState::GAME)
    {
        DebugRenderScreen(*m_screenCamera);
    }
}

//----------------------------------------------------------------------------------------------------
bool Game::IsAttractMode() const
{
    return m_gameState == eGameState::ATTRACT;
}

//----------------------------------------------------------------------------------------------------
void Game::UpdateFromKeyBoard()
{
    if (m_gameState == eGameState::ATTRACT)
    {
        if (g_input->WasKeyJustPressed(KEYCODE_ESC))
        {
            App::RequestQuit();
        }

        if (g_input->WasKeyJustPressed(KEYCODE_SPACE))
        {
            m_gameState = eGameState::GAME;
        }
    }

    if (m_gameState == eGameState::GAME)
    {
        if (g_input->WasKeyJustPressed(KEYCODE_ESC))
        {
            m_gameState = eGameState::ATTRACT;
        }

        if (g_input->WasKeyJustPressed(KEYCODE_P))
        {
            m_gameClock->TogglePause();
        }

        if (g_input->WasKeyJustPressed(KEYCODE_O))
        {
            m_gameClock->StepSingleFrame();
        }

        if (g_input->IsKeyDown(KEYCODE_T))
        {
            m_gameClock->SetTimeScale(0.1f);
        }

        if (g_input->WasKeyJustReleased(KEYCODE_T))
        {
            m_gameClock->SetTimeScale(1.f);
        }

        if (g_input->WasKeyJustPressed(NUMCODE_1))
        {
            Vec3 forward;
            Vec3 right;
            Vec3 up;
            m_player->m_orientation.GetAsVectors_IFwd_JLeft_KUp(forward, right, up);

            DebugAddWorldLine(m_player->m_position, m_player->m_position + forward * 20.f, 0.01f, 10.f, Rgba8(255, 255, 0), Rgba8(255, 255, 0), eDebugRenderMode::X_RAY);
        }

        if (g_input->IsKeyDown(NUMCODE_2))
        {
            DebugAddWorldPoint(Vec3(m_player->m_position.x, m_player->m_position.y, 0.f), 0.25f, 60.f, Rgba8(150, 75, 0), Rgba8(150, 75, 0));
        }

        if (g_input->WasKeyJustPressed(NUMCODE_3))
        {
            Vec3 forward;
            Vec3 right;
            Vec3 up;
            m_player->m_orientation.GetAsVectors_IFwd_JLeft_KUp(forward, right, up);

            DebugAddWorldWireSphere(m_player->m_position + forward * 2.f, 1.f, 5.f, Rgba8::GREEN, Rgba8::RED);
        }

        if (g_input->WasKeyJustPressed(NUMCODE_4))
        {
            DebugAddWorldBasis(m_player->GetModelToWorldTransform(), 20.f);
        }

        if (g_input->WasKeyJustReleased(NUMCODE_5))
        {
            float const  positionX    = m_player->m_position.x;
            float const  positionY    = m_player->m_position.y;
            float const  positionZ    = m_player->m_position.z;
            float const  orientationX = m_player->m_orientation.m_yawDegrees;
            float const  orientationY = m_player->m_orientation.m_pitchDegrees;
            float const  orientationZ = m_player->m_orientation.m_rollDegrees;
            String const text         = Stringf("Position: (%.2f, %.2f, %.2f)\nOrientation: (%.2f, %.2f, %.2f)", positionX, positionY, positionZ, orientationX, orientationY, orientationZ);

            Vec3 forward;
            Vec3 right;
            Vec3 up;
            m_player->m_orientation.GetAsVectors_IFwd_JLeft_KUp(forward, right, up);

            DebugAddBillboardText(text, m_player->m_position + forward, 0.1f, Vec2::HALF, 10.f, Rgba8::WHITE, Rgba8::RED);
        }

        if (g_input->WasKeyJustPressed(NUMCODE_6))
        {
            DebugAddWorldCylinder(m_player->m_position, m_player->m_position + Vec3::Z_BASIS * 2, 1.f, 10.f, true, Rgba8::WHITE, Rgba8::RED);
        }


        if (g_input->WasKeyJustReleased(NUMCODE_7))
        {
            float const orientationX = m_player->GetCamera()->GetOrientation().m_yawDegrees;
            float const orientationY = m_player->GetCamera()->GetOrientation().m_pitchDegrees;
            float const orientationZ = m_player->GetCamera()->GetOrientation().m_rollDegrees;

            DebugAddMessage(Stringf("Camera Orientation: (%.2f, %.2f, %.2f)", orientationX, orientationY, orientationZ), 5.f);
        }

        DebugAddMessage(Stringf("Player Position: (%.2f, %.2f, %.2f)", m_player->m_position.x, m_player->m_position.y, m_player->m_position.z), 0.f);
    }
}

//----------------------------------------------------------------------------------------------------
void Game::UpdateFromController()
{
    XboxController const& controller = g_input->GetController(0);

    if (m_gameState == eGameState::ATTRACT)
    {
        if (controller.WasButtonJustPressed(XBOX_BUTTON_BACK))
        {
            App::RequestQuit();
        }

        if (controller.WasButtonJustPressed(XBOX_BUTTON_START))
        {
            m_gameState = eGameState::GAME;
        }
    }

    if (m_gameState == eGameState::GAME)
    {
        if (controller.WasButtonJustPressed(XBOX_BUTTON_BACK))
        {
            m_gameState = eGameState::ATTRACT;
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

    DebugAddScreenText(Stringf("Time: %.2f\nFPS: %.2f\nScale: %.1f", m_gameClock->GetTotalSeconds(), 1.f / m_gameClock->GetDeltaSeconds(), m_gameClock->GetTimeScale()), m_screenCamera->GetOrthographicTopRight() - Vec2(250.f, 60.f), 20.f, Vec2::ZERO, 0.f, Rgba8::WHITE, Rgba8::WHITE);
}

//----------------------------------------------------------------------------------------------------
void Game::RenderAttractMode() const
{
    Vec2 clientDimensions = Window::s_mainWindow->GetClientDimensions();

    VertexList_PCU verts;
    AddVertsForDisc2D(verts, Vec2(clientDimensions.x * 0.5f, clientDimensions.y * 0.5f), 300.f, 10.f, Rgba8::YELLOW);
    g_renderer->SetModelConstants();
    g_renderer->SetBlendMode(eBlendMode::OPAQUE);
    g_renderer->SetRasterizerMode(eRasterizerMode::SOLID_CULL_BACK);
    g_renderer->SetSamplerMode(eSamplerMode::BILINEAR_CLAMP);
    g_renderer->SetDepthMode(eDepthMode::DISABLED);
    g_renderer->BindTexture(nullptr);
    g_renderer->BindShader(g_renderer->CreateOrGetShaderFromFile("Data/Shaders/Default"));
    g_renderer->DrawVertexArray(verts);
}

//----------------------------------------------------------------------------------------------------
void Game::RenderEntities() const
{
    m_firstCube->Render();
    m_secondCube->Render();
    m_sphere->Render();
    m_grid->Render();

    g_renderer->SetModelConstants(m_player->GetModelToWorldTransform());
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
    Texture const* texture = g_renderer->CreateOrGetTextureFromFile("Data/Images/TestUV.png");

    m_firstCube  = new Prop(this);
    m_secondCube = new Prop(this);
    m_sphere     = new Prop(this, texture);
    m_grid       = new Prop(this);

    m_firstCube->InitializeLocalVertsForCube();
    m_secondCube->InitializeLocalVertsForCube();
    m_sphere->InitializeLocalVertsForSphere();
    m_grid->InitializeLocalVertsForGrid();
}
