//----------------------------------------------------------------------------------------------------
// LightSubsystem.hpp
//----------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------
#pragma once
#include <vector>

//-Forward-Declaration--------------------------------------------------------------------------------
struct Light;

//----------------------------------------------------------------------------------------------------
struct sLightConfig
{
};

//----------------------------------------------------------------------------------------------------
class LightSubsystem
{
public:
    LightSubsystem();
    explicit LightSubsystem(sLightConfig config);
    ~LightSubsystem() = default;

    void StartUp();
    void BeginFrame();
    void Update();
    void Render();
    void EndFrame();
    void ShutDown();

    // Light management
    void   AddLight(Light* light);
    void   RemoveLight(int index);
    void   ClearLights();
    Light* GetLight(int index);
    int    GetLightCount() const;


    // Update and bind
    void UpdateLightConstants();
    void BindLightConstants();

private:
    sLightConfig        m_config;
    std::vector<Light*> m_lights;

    // LightConstants* m_lightConstants = nullptr;
    // ConstantBuffer* m_lightCBO = nullptr;
};
