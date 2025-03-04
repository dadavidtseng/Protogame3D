//----------------------------------------------------------------------------------------------------
// Game.hpp
//----------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------
#pragma once

//----------------------------------------------------------------------------------------------------
class Camera;
class Clock;
class Player;
class Prop;

//----------------------------------------------------------------------------------------------------
enum class eGameState
{
    Attract,
    Game
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
    Prop*      m_cylinder     = nullptr;
    Prop*      m_text         = nullptr;
    Prop*      m_arrow        = nullptr;
    Clock*     m_gameClock    = nullptr;
    eGameState m_gameState    = eGameState::Attract;
};
