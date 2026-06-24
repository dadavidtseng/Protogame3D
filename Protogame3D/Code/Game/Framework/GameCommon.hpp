//----------------------------------------------------------------------------------------------------
// GameCommon.hpp
//----------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------
#pragma once

//-Forward-Declaration--------------------------------------------------------------------------------
class App;
class AudioSystem;
class BitmapFont;
class Game;
class Renderer;
class RandomNumberGenerator;
struct Rgba8;
struct Vec2;

// one-time declaration
extern App*                   g_app;
extern AudioSystem*           g_audio;
extern BitmapFont*            g_bitmapFont;
extern Game*                  g_game;
extern Renderer*              g_renderer;
extern RandomNumberGenerator* g_rng;

//----------------------------------------------------------------------------------------------------
template <typename T>
void GAME_SAFE_RELEASE(T*& pointer)
{
    if (pointer != nullptr)
    {
        delete pointer;
        pointer = nullptr;
    }
}
