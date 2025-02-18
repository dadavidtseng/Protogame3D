//-----------------------------------------------------------------------------------------------
// GameCommon.hpp
//

//-----------------------------------------------------------------------------------------------
#pragma once

//-----------------------------------------------------------------------------------------------
struct Rgba8;
struct Vec2;
class App;
class AudioSystem;
class BitmapFont;
class Game;
class Renderer;
class RandomNumberGenerator;

// one-time declaration
extern App*                   g_theApp;
extern AudioSystem*           g_theAudio;
extern BitmapFont*            g_theBitmapFont;
extern Game*                  g_theGame;
extern Renderer*              g_theRenderer;
extern RandomNumberGenerator* g_theRNG;

//-----------------------------------------------------------------------------------------------
// initial settings
//
constexpr float SCREEN_SIZE_X = 1600.f;
constexpr float SCREEN_SIZE_Y = 800.f;

//-----------------------------------------------------------------------------------------------
// DebugRender-related
//
void DebugDrawRing(Vec2 const& center, float radius, float thickness, Rgba8 const& color);
void DebugDrawLine(Vec2 const& start, Vec2 const& end, float thickness, Rgba8 const& color);
void DebugDrawGlowCircle(Vec2 const& center, float radius, Rgba8 const& color, float glowIntensity);
void DebugDrawGlowBox(Vec2 const& center, Vec2 const& dimensions, Rgba8 const& color, float glowIntensity);
void DebugDrawBoxRing(Vec2 const& center, float radius, float thickness, Rgba8 const& color);
