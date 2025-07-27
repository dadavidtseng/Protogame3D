#pragma once

#include "Engine/Core/EngineCommon.hpp"
#include <string>

// 前向宣告 V8 類型
namespace v8
{
    template<class T> class FunctionCallbackInfo;
    class Value;
}

class Game;

//----------------------------------------------------------------------------------------------------
// JavaScript 管理器 - 負責遊戲特定的 JavaScript 功能
// 委託 V8 初始化和基本執行給 V8Subsystem
//----------------------------------------------------------------------------------------------------
class JavaScriptManager
{
public:
    JavaScriptManager();
    ~JavaScriptManager();

    void Initialize();
    void Shutdown();

    // 執行 JavaScript 程式碼
    bool ExecuteScript(const std::string& script);
    bool ExecuteScriptFromFile(const std::string& filename);

    // 繫結遊戲物件到 JavaScript
    void BindGameObjectsToJS(Game* game);

    // 取得 JavaScript 結果
    std::string GetLastResult() const { return m_lastResult; }

private:
    std::string m_lastResult;
    Game* m_gameReference;

    // 輔助方法
    void CreateGlobalFunctions();
    void BindPropFunctions();
    void BindPlayerFunctions();

    // JavaScript 回呼函數
    static void JSLog(const v8::FunctionCallbackInfo<v8::Value>& args);
    static void JSCreateCube(const v8::FunctionCallbackInfo<v8::Value>& args);
    static void JSMoveProp(const v8::FunctionCallbackInfo<v8::Value>& args);
    static void JSGetPlayerPosition(const v8::FunctionCallbackInfo<v8::Value>& args);
    static void JSSetPlayerPosition(const v8::FunctionCallbackInfo<v8::Value>& args);
};

// 全域 JavaScript 管理器
extern JavaScriptManager* g_theJavaScriptManager;