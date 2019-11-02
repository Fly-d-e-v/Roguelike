#pragma once
#define OPENGL //TODO: Should be done by CMAKE
#include <memory>

class Renderer
{

public:

    Renderer();
    virtual ~Renderer();

    bool Init();
    void Tick(float deltaTime);
    void Deinit();

    bool ShutdownRequested();

private:

	bool LoadResources();

	std::shared_ptr<class Shader> _shaderProgram;

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

	unsigned int _textureNiels = 0u;

};