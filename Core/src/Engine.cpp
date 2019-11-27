#include "Engine.h"

#include "platform/PlatformInterface.h"
#include "graphics/Renderer.h"
#include "input/InputManager.h"
#include "audio/AudioEngine.h"
#include "core/utils/Utilities.h"
#include "resources/ResourceManager.h"
#include "ecs/EntityManager.h"

#include "core/config/ConfigLoader.h"
#include "core/config/Config.h"

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
	m_EntityManager = std::make_shared<EntityManager>();
}

void Engine::Init()
{
    PlatformInterface::Init();
    ResourceManager::Instance()->LoadResourceList();

    m_PastFrame = std::chrono::system_clock::now();

    _Config = Config();
    ConfigLoader::LoadConfig(_Config);
    ConfigLoader::SaveConfig(_Config);

    m_Renderer->Init();
    InputManager::Instance()->Init();
    AudioEngine::Instance()->Init();
	m_EntityManager->Init();

    m_Renderer->RegisterTool(ResourceManager::Instance());
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
    ResourceManager::Instance()->SaveResourceList();
    PlatformInterface::Deinit();
}

bool Engine::ShouldShutdown()
{
    return m_Renderer->ShutdownRequested();
}

Config& Engine::GetConfig() {
    return _Config;
}
