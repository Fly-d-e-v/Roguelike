#pragma once

#include <core/config/Config.h>

#include <memory>
#include <chrono>

class Engine
{

public:

    void Main();

    static Engine* Instance();

    Config& GetConfig();

private:

    Engine();

    void Init();
    void Tick(float deltaTime);
    void Deinit();

    bool ShouldShutdown();

private:

    static Engine* s_Instance;

    std::chrono::system_clock::time_point m_PastFrame;

    Config _Config;

    std::shared_ptr<class Renderer> m_Renderer;

};