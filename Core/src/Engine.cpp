#include "Engine.h"

#include "platform/PlatformInterface.h"

Engine* Engine::s_Instance = nullptr;

void Engine::Main()
{
    Init();

    while (!ShouldShutdown())
    {
        std::chrono::duration<float> deltaTime = std::chrono::system_clock::now() - m_PastFrame;
        m_PastFrame = std::chrono::system_clock::now();

        Tick(deltaTime.count());
    }

    Deinit();
}

Engine* Engine::Instance()
{

    if (s_Instance != nullptr)
    {
        s_Instance = new Engine();
    }

    return s_Instance;
}

Engine::Engine()
{

}

void Engine::Init()
{
    PlatformInterface::Init();
}

void Engine::Tick(float)
{
}

void Engine::Deinit()
{
    PlatformInterface::Deinit();
}

bool Engine::ShouldShutdown()
{
    return false;
}
