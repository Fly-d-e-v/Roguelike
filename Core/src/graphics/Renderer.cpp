#include "Renderer.h"

#include "thirdparty/glad/glad.h"
#include "thirdparty/GLFW/glfw3.h"

#include <string>

Renderer::Renderer() : m_Window(nullptr)
{
}

Renderer::~Renderer()
{
}

bool Renderer::Init()
{

    if (!glfwInit())
    {
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    GLFWwindow* window = glfwCreateWindow(500, 500, "RogueLike", NULL, NULL);
    glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    glfwSwapInterval(1);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

    return true;
}

void Renderer::Tick(float deltaTime)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glfwPollEvents();

    std::string title = "RogueLike - " + std::to_string(static_cast<int>(1 / deltaTime)) + "fps";
    glfwSetWindowTitle(glfwGetCurrentContext(), title.c_str());
    //@TODO design tick order for engine


    glfwSwapBuffers(glfwGetCurrentContext());
}

void Renderer::Deinit()
{
    glfwTerminate();
}

bool Renderer::ShutdownRequested()
{
    return glfwWindowShouldClose(glfwGetCurrentContext());
}
