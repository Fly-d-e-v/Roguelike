#pragma once

#include <core/config/Config.h>

#include <memory>
#include <chrono>

class Engine
{

public:

    void Main();

    [[nodiscard]] static Engine* Instance()
	{
		if (s_Instance == nullptr)
		{
			s_Instance = new Engine();
		}
		return s_Instance;
	}

    Config& GetConfig();

	[[nodiscard]] const std::shared_ptr<class EntityManager> GetEntityManager() const
	{
		return m_EntityManager;
	};
	
	[[nodiscard]] const std::shared_ptr <class Camera> GetCamera() const
	{
		return _camera;
	};
	
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

    std::shared_ptr<class Logger> m_Logger;
    std::shared_ptr<class Renderer> m_Renderer;
	std::shared_ptr<class EntityManager> m_EntityManager;
	std::shared_ptr<class Camera> _camera;
};
