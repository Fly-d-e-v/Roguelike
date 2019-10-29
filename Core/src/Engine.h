#pragma once


#include <chrono>

class Engine
{

public:

    void Main();

    static Engine* Instance();

private:

    Engine();

    void Init();
    void Tick(float deltaTime);
    void Deinit();

    bool ShouldShutdown();

private:

    static Engine* s_Instance;

    std::chrono::system_clock::time_point m_PastFrame;

};