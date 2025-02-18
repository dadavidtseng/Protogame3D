#define WIN32_LEAN_AND_MEAN		// Always #define this before #including <windows.h>
#include <cstdio>
#include <iostream>
#include <windows.h>			// #include this (massive, platform-specific) header in VERY few places (and .CPPs only)
#include "Engine/Core/EngineCommon.hpp"
#include "Game/App.hpp"
#include "Game/GameCommon.hpp"

//-----------------------------------------------------------------------------------------------
int WINAPI WinMain(HINSTANCE const applicationInstanceHandle, HINSTANCE, LPSTR const commandLineString, int)
{
    UNUSED(applicationInstanceHandle)
    UNUSED(commandLineString)

    g_theApp = new App();
    g_theApp->Startup();
    g_theApp->RunMainLoop();
    g_theApp->Shutdown();

    delete g_theApp;
    g_theApp = nullptr;

    return 0;
}
