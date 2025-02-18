//----------------------------------------------------------------------------------------------------
// Game.hpp
//----------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------
#pragma once

//----------------------------------------------------------------------------------------------------
class Camera;

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

    Camera* m_screenCamera     = nullptr;
    bool    m_isAttractMode    = true;
    bool    m_isDevConsoleMode = false;
};
