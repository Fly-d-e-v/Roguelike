#pragma once

#include <core/config/Config.h>
#include "core/Camera.h"

#include <memory>
#include <chrono>

class Engine
{

public:

    void Main();

    static Engine* Instance();

    Config& GetConfig();

	[[nodiscard]] const std::shared_ptr<class EntityManager> GetEntityManager();
	[[nodiscard]] static Camera& GetCamera()
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

	static Camera _camera; 

    std::chrono::system_clock::time_point m_PastFrame;

    Config _Config;

    std::shared_ptr<class Logger> m_Logger;
    std::shared_ptr<class Renderer> m_Renderer;
	std::shared_ptr<class EntityManager> m_EntityManager;
};
