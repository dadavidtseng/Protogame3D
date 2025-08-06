//----------------------------------------------------------------------------------------------------
// Game.hpp
//----------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------
#pragma once
#include "Engine/Resource/ResourceHandle.hpp"

struct Vertex_PCUTBN;
class ModelResource;
//----------------------------------------------------------------------------------------------------
class Camera;
class Clock;
class Player;
class Prop;

//----------------------------------------------------------------------------------------------------
enum class eGameState : uint8_t
{
    ATTRACT,
    GAME
};

//----------------------------------------------------------------------------------------------------
class Game
{
public:
    Game();
    ~Game();

    void Update();
    void Render() const;
    bool IsAttractMode() const;

private:
    void UpdateFromKeyBoard();
    void UpdateFromController();
    void UpdateEntities(float gameDeltaSeconds, float systemDeltaSeconds) const;
    void RenderAttractMode() const;
    void RenderEntities() const;

    void SpawnPlayer();
    void SpawnProp();

    Camera*    m_screenCamera = nullptr;
    Player*    m_player       = nullptr;
    Prop*      m_firstCube    = nullptr;
    Prop*      m_secondCube   = nullptr;
    Prop*      m_sphere       = nullptr;
    Prop*      m_grid         = nullptr;
    Clock*     m_gameClock    = nullptr;
    eGameState m_gameState    = eGameState::ATTRACT;
};
