#pragma once

#include "thirdparty/glad/glad.h"
#include "thirdparty/GLFW/glfw3.h"
#include "Shader.h"

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
	static void GLFWErrorCallback(int error, const char* description);

	static void GLFWFramebufferSizeCallback(GLFWwindow* window, int width, int height);

	Shader _shaderProgram;
	unsigned int _vao;
	unsigned int _vbo;
	unsigned int _ebo;

};