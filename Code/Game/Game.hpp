//----------------------------------------------------------------------------------------------------
// Game.hpp
//----------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------
#pragma once

//----------------------------------------------------------------------------------------------------
class Camera;
class Player;
class Prop;

//----------------------------------------------------------------------------------------------------
class Game
{
public:
    Game();
    ~Game() = default;

    void Update();
    void Render() const;
    bool IsAttractMode() const { return m_isAttractMode; }

private:
    void UpdateFromKeyBoard();
    void UpdateFromController();
    void AdjustForPauseAndTimeDistortion();
    void RenderAttractMode() const;
    void RenderUI() const;

    void SpawnPlayer();
    void SpawnProp();

    Camera* m_screenCamera     = nullptr;
    Camera* m_worldCamera      = nullptr;
    bool    m_isAttractMode    = true;
    bool    m_isDevConsoleMode = false;
    Player* m_player           = nullptr;
    Prop*   m_prop             = nullptr;
};
