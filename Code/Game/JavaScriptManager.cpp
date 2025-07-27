// JavaScriptManager.cpp - 修正 HandleScope 問題的版本
#include "Game/JavaScriptManager.hpp"
#include "Game/Game.hpp"
#include "Game/Prop.hpp"
#include "Game/Player.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"
#include "Engine/Core/StringUtils.hpp"
#include "Engine/Math/Vec3.hpp"
#include "Engine/Scripting/V8Subsystem.hpp"
#include <fstream>

// V8 標頭檔 - 只在需要時包含
#pragma warning(push)
#pragma warning(disable: 4100 4324 4127)
// #include <v8.h>
#include "ThirdParty/packages/v8-v143-x64.13.0.245.25/include/v8.h"
#pragma warning(pop)

// 全域變數
JavaScriptManager* g_theJavaScriptManager = nullptr;

//----------------------------------------------------------------------------------------------------
JavaScriptManager::JavaScriptManager()
    : m_gameReference(nullptr)
{
}

//----------------------------------------------------------------------------------------------------
JavaScriptManager::~JavaScriptManager()
{
    Shutdown();
}

//----------------------------------------------------------------------------------------------------
void JavaScriptManager::Initialize()
{
    DebuggerPrintf("JavaScript Manager - 委託給 V8Subsystem\n");

    if (!g_theV8Subsystem)
    {
        DebuggerPrintf("錯誤：V8Subsystem 尚未初始化！\n");
        DebuggerPrintf("請確保 V8Subsystem 在 JavaScriptManager 之前初始化。\n");
        return;
    }

    if (!g_theV8Subsystem->IsInitialized())
    {
        DebuggerPrintf("錯誤：V8Subsystem 初始化失敗！\n");
        return;
    }

    DebuggerPrintf("JavaScript Manager 初始化成功（使用 V8Subsystem）！\n");
}

//----------------------------------------------------------------------------------------------------
void JavaScriptManager::Shutdown()
{
    // JavaScriptManager 不需要關閉 V8，這由 V8Subsystem 負責
    m_gameReference = nullptr;
    m_lastResult.clear();
    DebuggerPrintf("JavaScript Manager 關閉（V8Subsystem 將處理 V8 清理）。\n");
}

//----------------------------------------------------------------------------------------------------
bool JavaScriptManager::ExecuteScript(const std::string& script)
{
    if (!g_theV8Subsystem || !g_theV8Subsystem->IsInitialized())
    {
        DebuggerPrintf("FALLBACK: 執行 JavaScript: %s\n", script.c_str());

        // 您原來的 fallback 邏輯
        if (script.find("console.log") != std::string::npos)
        {
            size_t start = script.find("console.log(");
            if (start != std::string::npos)
            {
                start += 12;
                size_t end = script.find(")", start);
                if (end != std::string::npos)
                {
                    std::string logContent = script.substr(start, end - start);
                    if (!logContent.empty())
                    {
                        if ((logContent.front() == '\'' && logContent.back() == '\'') ||
                            (logContent.front() == '"' && logContent.back() == '"'))
                        {
                            logContent = logContent.substr(1, logContent.size() - 2);
                        }
                    }
                    DebuggerPrintf("JS FALLBACK LOG: %s\n", logContent.c_str());
                }
            }
        }

        if (script.find("Game.createCube") != std::string::npos)
        {
            DebuggerPrintf("JS FALLBACK: Would create cube\n");
        }

        if (script.find("Game.getPlayerPos") != std::string::npos)
        {
            DebuggerPrintf("JS FALLBACK: Would get player position\n");
            m_lastResult = "{x: -2, y: 0, z: 1}";
        }

        if (script.find("Game.moveProp") != std::string::npos)
        {
            DebuggerPrintf("JS FALLBACK: Would move prop\n");
        }

        if (script.find("Game.setPlayerPos") != std::string::npos)
        {
            DebuggerPrintf("JS FALLBACK: Would set player position\n");
        }

        // 數學運算模擬
        if (script.find("+") != std::string::npos || script.find("*") != std::string::npos)
        {
            DebuggerPrintf("JS FALLBACK: Would perform mathematical calculation\n");
            m_lastResult = "42"; // 假結果
        }

        // 字串連接模擬
        if (script.find("'Hello '") != std::string::npos && script.find("'JavaScript!'") != std::string::npos)
        {
            DebuggerPrintf("JS FALLBACK: String concatenation result\n");
            m_lastResult = "Hello JavaScript!";
        }

        return true; // Fallback 總是返回成功
    }

    // 委託給 V8Subsystem 執行
    bool result = g_theV8Subsystem->ExecuteScript(script);

    // 如果 V8Subsystem 有錯誤，記錄下來
    if (!result && g_theV8Subsystem->HasError())
    {
        m_lastResult = "Error: " + g_theV8Subsystem->GetLastError();
    }

    return result;
}

//----------------------------------------------------------------------------------------------------
bool JavaScriptManager::ExecuteScriptFromFile(const std::string& filename)
{
    if (!g_theV8Subsystem || !g_theV8Subsystem->IsInitialized())
    {
        DebuggerPrintf("FALLBACK: 無法執行腳本檔案: %s (V8Subsystem 不可用)\n", filename.c_str());

        // Fallback 檔案讀取模擬
        std::ifstream file(filename);
        if (!file.is_open())
        {
            DebuggerPrintf("FALLBACK: Failed to open JavaScript file: %s\n", filename.c_str());
            return false;
        }

        std::string script;
        std::string line;
        while (std::getline(file, line))
        {
            script += line + "\n";
        }
        file.close();

        DebuggerPrintf("FALLBACK: Loaded JavaScript file: %s (%zu characters)\n", filename.c_str(), script.length());

        return ExecuteScript(script);
    }

    return g_theV8Subsystem->ExecuteScriptFile(filename);
}

//----------------------------------------------------------------------------------------------------
void JavaScriptManager::BindGameObjectsToJS(Game* game)
{
    m_gameReference = game;

    if (!g_theV8Subsystem || !g_theV8Subsystem->IsInitialized())
    {
        DebuggerPrintf("FALLBACK: 遊戲物件綁定（V8Subsystem 不可用）\n");
        return;
    }

    // 使用安全的上下文執行方法
    bool success = g_theV8Subsystem->ExecuteInContext([&](v8::Local<v8::Context> context) -> bool {
        try
        {
            v8::Isolate* isolate = g_theV8Subsystem->GetIsolate();

            // 建立遊戲物件
            v8::Local<v8::Object> gameObj = v8::Object::New(isolate);

            // 綁定遊戲函數的安全方法
            auto tryBindFunction = [&](const char* name, v8::FunctionCallback callback) -> bool {
                try {
                    v8::MaybeLocal<v8::Function> func = v8::Function::New(context, callback);
                    if (!func.IsEmpty()) {
                        gameObj->Set(context,
                            v8::String::NewFromUtf8(isolate, name).ToLocalChecked(),
                            func.ToLocalChecked()
                        );
                        return true;
                    }
                } catch (...) {
                    DebuggerPrintf("綁定函數失敗: %s\n", name);
                }
                return false;
            };

            // 綁定所有遊戲相關函數
            tryBindFunction("createCube", JSCreateCube);
            tryBindFunction("moveProp", JSMoveProp);
            tryBindFunction("getPlayerPos", JSGetPlayerPosition);
            tryBindFunction("setPlayerPos", JSSetPlayerPosition);

            // 將遊戲物件設為全域變數
            context->Global()->Set(context,
                v8::String::NewFromUtf8(isolate, "Game").ToLocalChecked(),
                gameObj
            );

            DebuggerPrintf("遊戲物件成功綁定到 JavaScript！\n");
            DebuggerPrintf("可用的函數：Game.createCube(), Game.moveProp(), Game.getPlayerPos(), Game.setPlayerPos()\n");
            return true;
        }
        catch (const std::exception& e)
        {
            DebuggerPrintf("綁定遊戲物件時發生例外: %s\n", e.what());
            return false;
        }
        catch (...)
        {
            DebuggerPrintf("綁定遊戲物件時發生未知例外！\n");
            return false;
        }
    });

    if (!success)
    {
        DebuggerPrintf("遊戲物件綁定失敗！\n");
    }
}

//----------------------------------------------------------------------------------------------------
void JavaScriptManager::CreateGlobalFunctions()
{
    // 這個方法不再需要，因為 V8Subsystem 會處理全域函數
    DebuggerPrintf("全域函數建立委託給 V8Subsystem\n");
}

//----------------------------------------------------------------------------------------------------
void JavaScriptManager::BindPropFunctions()
{
    // 空實作 - 由 BindGameObjectsToJS 處理
    DebuggerPrintf("道具函數綁定由 BindGameObjectsToJS 處理\n");
}

//----------------------------------------------------------------------------------------------------
void JavaScriptManager::BindPlayerFunctions()
{
    // 空實作 - 由 BindGameObjectsToJS 處理
    DebuggerPrintf("玩家函數綁定由 BindGameObjectsToJS 處理\n");
}

//----------------------------------------------------------------------------------------------------
void JavaScriptManager::JSLog(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    std::string output = "JS: ";

    for (int i = 0; i < args.Length(); i++)
    {
        if (i > 0) output += " ";

        v8::String::Utf8Value str(args.GetIsolate(), args[i]);
        output += *str;
    }

    DebuggerPrintf("%s\n", output.c_str());
}

//----------------------------------------------------------------------------------------------------
void JavaScriptManager::JSCreateCube(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    if (!g_theJavaScriptManager || !g_theJavaScriptManager->m_gameReference)
    {
        DebuggerPrintf("JSCreateCube: 遊戲參考無效\n");
        return;
    }

    float x = 0.0f, y = 0.0f, z = 0.0f;

    if (args.Length() >= 3)
    {
        x = (float)args[0]->NumberValue(args.GetIsolate()->GetCurrentContext()).FromMaybe(0.0);
        y = (float)args[1]->NumberValue(args.GetIsolate()->GetCurrentContext()).FromMaybe(0.0);
        z = (float)args[2]->NumberValue(args.GetIsolate()->GetCurrentContext()).FromMaybe(0.0);
    }

    DebuggerPrintf("JS: 在位置 (%.2f, %.2f, %.2f) 建立方塊\n", x, y, z);

    // 這裡可以呼叫實際的遊戲邏輯
    // 例如：g_theJavaScriptManager->m_gameReference->CreateCube(Vec3(x, y, z));

    // 暫時只是記錄訊息
    DebuggerPrintf("JS: 方塊建立請求已接收\n");
}

//----------------------------------------------------------------------------------------------------
void JavaScriptManager::JSMoveProp(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    if (!g_theJavaScriptManager || !g_theJavaScriptManager->m_gameReference)
    {
        DebuggerPrintf("JSMoveProp: 遊戲參考無效\n");
        return;
    }

    if (args.Length() >= 4)
    {
        int propIndex = (int)args[0]->Int32Value(args.GetIsolate()->GetCurrentContext()).FromMaybe(0);
        float x = (float)args[1]->NumberValue(args.GetIsolate()->GetCurrentContext()).FromMaybe(0.0);
        float y = (float)args[2]->NumberValue(args.GetIsolate()->GetCurrentContext()).FromMaybe(0.0);
        float z = (float)args[3]->NumberValue(args.GetIsolate()->GetCurrentContext()).FromMaybe(0.0);

        DebuggerPrintf("JS: 移動物件 %d 到位置 (%.2f, %.2f, %.2f)\n", propIndex, x, y, z);

        // 呼叫實際的遊戲邏輯
        g_theJavaScriptManager->m_gameReference->MoveProp(propIndex, Vec3(x, y, z));
    }
    else
    {
        DebuggerPrintf("JSMoveProp: 參數不足，需要 (propIndex, x, y, z)\n");
    }
}

//----------------------------------------------------------------------------------------------------
void JavaScriptManager::JSGetPlayerPosition(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    if (!g_theJavaScriptManager || !g_theJavaScriptManager->m_gameReference)
    {
        DebuggerPrintf("JSGetPlayerPosition: 遊戲參考無效\n");

        // 返回預設位置
        v8::Isolate* isolate = args.GetIsolate();
        v8::Local<v8::Context> context = isolate->GetCurrentContext();
        v8::Local<v8::Object> posObj = v8::Object::New(isolate);

        posObj->Set(context, v8::String::NewFromUtf8(isolate, "x").ToLocalChecked(), v8::Number::New(isolate, -2.0));
        posObj->Set(context, v8::String::NewFromUtf8(isolate, "y").ToLocalChecked(), v8::Number::New(isolate, 0.0));
        posObj->Set(context, v8::String::NewFromUtf8(isolate, "z").ToLocalChecked(), v8::Number::New(isolate, 1.0));

        args.GetReturnValue().Set(posObj);
        return;
    }

    Player* player = g_theJavaScriptManager->m_gameReference->GetPlayer();
    Vec3 pos = player ? player->m_position : Vec3(-2.0f, 0.0f, 1.0f);

    v8::Isolate* isolate = args.GetIsolate();
    v8::Local<v8::Context> context = isolate->GetCurrentContext();
    v8::Local<v8::Object> posObj = v8::Object::New(isolate);

    posObj->Set(context, v8::String::NewFromUtf8(isolate, "x").ToLocalChecked(), v8::Number::New(isolate, pos.x));
    posObj->Set(context, v8::String::NewFromUtf8(isolate, "y").ToLocalChecked(), v8::Number::New(isolate, pos.y));
    posObj->Set(context, v8::String::NewFromUtf8(isolate, "z").ToLocalChecked(), v8::Number::New(isolate, pos.z));

    args.GetReturnValue().Set(posObj);

    DebuggerPrintf("JS: 取得玩家位置 (%.2f, %.2f, %.2f)\n", pos.x, pos.y, pos.z);
}

//----------------------------------------------------------------------------------------------------
void JavaScriptManager::JSSetPlayerPosition(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    if (!g_theJavaScriptManager || !g_theJavaScriptManager->m_gameReference)
    {
        DebuggerPrintf("JSSetPlayerPosition: 遊戲參考無效\n");
        return;
    }

    if (args.Length() >= 3)
    {
        float x = (float)args[0]->NumberValue(args.GetIsolate()->GetCurrentContext()).FromMaybe(0.0);
        float y = (float)args[1]->NumberValue(args.GetIsolate()->GetCurrentContext()).FromMaybe(0.0);
        float z = (float)args[2]->NumberValue(args.GetIsolate()->GetCurrentContext()).FromMaybe(0.0);

        DebuggerPrintf("JS: 設定玩家位置為 (%.2f, %.2f, %.2f)\n", x, y, z);

        Player* player = g_theJavaScriptManager->m_gameReference->GetPlayer();
        if (player)
        {
            player->m_position = Vec3(x, y, z);
            DebuggerPrintf("JS: 玩家位置設定成功\n");
        }
        else
        {
            DebuggerPrintf("JS: 警告 - 找不到玩家物件\n");
        }
    }
    else
    {
        DebuggerPrintf("JSSetPlayerPosition: 參數不足，需要 (x, y, z)\n");
    }
}