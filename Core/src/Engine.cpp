#include "Engine.h"

#include "platform/PlatformInterface.h"
#include "graphics/Renderer.h"
#include "input/InputManager.h"

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

    if (s_Instance == nullptr)
    {
        s_Instance = new Engine();
    }

    return s_Instance;
}

Engine::Engine() : m_PastFrame()
{
    m_Renderer = std::make_shared<Renderer>();
}

void Engine::Init()
{
    PlatformInterface::Init();

    m_PastFrame = std::chrono::system_clock::now();

    m_Renderer->Init();
    InputManager::Instance()->Init();

}

void Engine::Tick(float deltaTime)
{
    InputManager::Instance()->Tick(deltaTime);
    m_Renderer->Tick(deltaTime);
}

void Engine::Deinit()
{
    InputManager::Instance()->Deinit();
    m_Renderer->Deinit();
    PlatformInterface::Deinit();
}

bool Engine::ShouldShutdown()
{
    return m_Renderer->ShutdownRequested();
}
