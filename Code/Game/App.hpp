//----------------------------------------------------------------------------------------------------
// App.hpp
//----------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------
#pragma once
#include "Engine/Core/EventSystem.hpp"
#include "Engine/Core/Timer.hpp"

//-Forward-Declaration--------------------------------------------------------------------------------
class Camera;
class Game;

//----------------------------------------------------------------------------------------------------
class App
{
public:
    App()  = default;
    ~App() = default;
    void Startup();
    void Shutdown();
    void RunFrame();

    void RunMainLoop();

    static bool OnWindowClose(EventArgs& args);
    static bool Event_KeyPressed(EventArgs& args);
    static void RequestQuit();
    static bool m_isQuitting;

private:
    void BeginFrame() const;
    void Update();
    void Render() const;
    void EndFrame() const;

    void DeleteAndCreateNewGame();

    Camera* m_devConsoleCamera = nullptr;
};
