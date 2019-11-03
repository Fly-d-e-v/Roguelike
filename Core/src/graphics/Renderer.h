#pragma once
#define OPENGL //TODO: Should be done by CMAKE
#include <memory>

#include "core/event/Event.h"

class Renderer
{

public:

    Renderer();
    virtual ~Renderer();

    bool Init();
    void Tick(float deltaTime);
    void Deinit();

    void RegisterTool(std::shared_ptr<class Tool> tool);

    bool ShutdownRequested();

private:

	bool LoadResources();

    void TickImgui(float deltaTime);

    void ImGuiDemo();

	std::shared_ptr<class Shader> _shaderProgram;

    std::shared_ptr<class Event<>> _ImguiToolRenderEvent;
    std::shared_ptr<class Event<>> _ImguiMenuItemEvent;

#ifdef OPENGL

	static void GLFWErrorCallback(int error, const char* description);

	static void GLFWFramebufferSizeCallback(struct GLFWwindow* window, int width, int height);
	unsigned int _vao = 0u;
	unsigned int _vbo = 0u;
	unsigned int _ebo = 0u;

	unsigned int _model_mat_uniform = 0u;
	unsigned int _proj_mat_uniform = 0u;
	unsigned int _view_mat_uniform = 0u;

#endif // OPENGL

	static bool _didResize;

    std::shared_ptr<class Texture> _NielsTexture;

};