#include "Engine.h"

#include "platform/PlatformInterface.h"
#include "graphics/Renderer.h"
#include "input/InputManager.h"
#include "audio/AudioEngine.h"
#include "core/utils/Utilities.h"
#include "resources/ResourceManager.h"
#include "resources/resources/Texture.h"
#include "ecs/EntityManager.h"

#include "core/config/ConfigLoader.h"
#include "core/config/Config.h"

#include "core/logger/Logger.h"

#include "glm/glm.hpp"
#include <glm/ext/matrix_transform.inl>

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
    Logger::WriteFirstLog();
    Logger::Log(LogVerbosity::Info, "Creating Engine Instance...");
    m_Logger = std::make_shared<Logger>();
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

    m_Renderer->RegisterTool(m_Logger);
    m_Renderer->RegisterTool(ResourceManager::Instance());

	//TODO: This probably isn't the correct place either, should be moved to the game instead of engine...
    {
		auto test_sprite_render_component = RenderComponent();
		test_sprite_render_component.model_matrix = glm::translate(test_sprite_render_component.model_matrix, glm::vec3(350.f, 350.f, 0.f));
		test_sprite_render_component.model_matrix = glm::scale(test_sprite_render_component.model_matrix, glm::vec3(150.f, 150.f, 1.f));
		test_sprite_render_component.texture = ResourceManager::Instance()->FetchResource<Texture>("Resources/Textures/Niels.jpg");

		const auto test_sprite_entity = m_EntityManager->CreateEntity();
		m_EntityManager->AddComponent<RenderComponent>(test_sprite_render_component, test_sprite_entity);
    }
}

void Engine::Tick(float deltaTime)
{
    Logger::ProcessQueue();

    InputManager::Instance()->Tick(deltaTime);

	/*TODO: Should probably be handled differently but I prefer to have it in update engine over the rendering loop for the time being
	Should in one way or another be in the game loop... */
	auto render_components = m_EntityManager->GetComponents<RenderComponent>();
	for (auto render_component : render_components)
	{
		if (render_component->texture) 
			render_component->model_matrix = glm::rotate(render_components[0]->model_matrix, glm::radians(45.f * deltaTime), glm::vec3(0, 0, 1));
	}
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

const std::shared_ptr<EntityManager> Engine::GetEntityManager()
{
	return m_EntityManager;
}
