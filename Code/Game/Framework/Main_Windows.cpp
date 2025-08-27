#define WIN32_LEAN_AND_MEAN		// Always #define this before #including <windows.h>
#include <cstdio>
#include <iostream>
#include <windows.h>			// #include this (massive, platform-specific) header in VERY few places (and .CPPs only)
#include "Engine/Core/EngineCommon.hpp"
#include "Game/Framework/App.hpp"
#include "Game/Framework/GameCommon.hpp"

//-----------------------------------------------------------------------------------------------
int WINAPI WinMain(HINSTANCE const applicationInstanceHandle, HINSTANCE, LPSTR const commandLineString, int)
{
    UNUSED(applicationInstanceHandle)
    UNUSED(commandLineString)

    g_app = new App();
    g_app->Startup();
    g_app->RunMainLoop();
    g_app->Shutdown();

    delete g_app;
    g_app = nullptr;

    return 0;
}
