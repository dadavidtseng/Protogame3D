//----------------------------------------------------------------------------------------------------
// Game.hpp
//----------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------
#pragma once
#include "Engine/Core/VertexUtils.hpp"

//----------------------------------------------------------------------------------------------------
class Camera;
class Clock;
class Player;
class Prop;

//----------------------------------------------------------------------------------------------------
class Game
{
public:
    Game();
    ~Game();

    void Update();
    void Render() const;
    bool IsAttractMode() const;
    Clock* GetGameClock() const;

private:
    void UpdateFromKeyBoard();
    void UpdateFromController();
    void AdjustForPauseAndTimeDistortion();
    void RenderAttractMode() const;
    void RenderUI() const;

    void SpawnPlayer();
    void SpawnProp();

    Camera* m_screenCamera  = nullptr;
    bool m_isAttractMode    = true;
    bool m_isDevConsoleMode = false;
    Player* m_player        = nullptr;
    Prop* m_firstCube       = nullptr;
    Prop* m_secondCube      = nullptr;
    Prop* m_sphere          = nullptr;
    Prop* m_grid            = nullptr;
    Clock* m_gameClock      = nullptr;
};
