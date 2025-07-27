//----------------------------------------------------------------------------------------------------
// Game.hpp
//----------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------
#pragma once
#include "Engine/Core/StringUtils.hpp"
#include "Engine/Core/VertexUtils.hpp"
#include "Engine/Core/Vertex_PCUTBN.hpp"
#include "Engine/Resource/ResourceHandle.hpp"

struct Vertex_PCUTBN;
class ModelResource;
class JavaScriptManager;
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


    // 新增 JavaScript 相關公用方法
    void ExecuteJavaScript(const String& script);
    void RunJavaScriptTests();

    // 讓 JavaScript 可以存取這些成員
    Player* GetPlayer() const { return m_player; }
    Prop* GetProp(int index) const;
    void MoveProp(int index, const Vec3& newPosition);

private:
    void UpdateFromKeyBoard();
    void UpdateFromController();
    void UpdateEntities(float gameDeltaSeconds, float systemDeltaSeconds) const;
    void RenderAttractMode() const;
    void RenderEntities() const;

    void SpawnPlayer();
    void SpawnProp();

    // 新增 JavaScript 相關私有方法
    void InitializeJavaScript();
    void ShutdownJavaScript();

    Camera*    m_screenCamera = nullptr;
    Player*    m_player       = nullptr;
    Prop*      m_firstCube    = nullptr;
    Prop*      m_secondCube   = nullptr;
    Prop*      m_sphere       = nullptr;
    Prop*      m_grid         = nullptr;
    Clock*     m_gameClock    = nullptr;
    eGameState m_gameState    = eGameState::ATTRACT;
    // 新增 JavaScript 管理器
    JavaScriptManager* m_jsManager = nullptr;
};
