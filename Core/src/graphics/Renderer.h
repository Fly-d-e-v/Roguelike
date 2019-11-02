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
	unsigned int _vao;
	unsigned int _vbo;
	unsigned int _ebo;

#endif // OPENGL


	unsigned int _textureNiels;

};