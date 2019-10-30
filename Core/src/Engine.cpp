#include "Engine.h"

#include "platform/PlatformInterface.h"

#include "thirdparty/glad/glad.h"
#include "thirdparty/GLFW/glfw3.h"

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

}

void Engine::Init()
{
    PlatformInterface::Init();

    m_PastFrame = std::chrono::system_clock::now();


    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    GLFWwindow* window = glfwCreateWindow(500, 500, "RogueLike", NULL, NULL);
    glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    glfwSwapInterval(1);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
}

void Engine::Tick(float deltaTime)
{

    glClear(GL_COLOR_BUFFER_BIT);
    glfwPollEvents();

    std::string title = "RogueLike - " + std::to_string(static_cast<int>(1 / deltaTime)) + "fps";
    glfwSetWindowTitle(glfwGetCurrentContext(), title.c_str());
    //@TODO design tick order for engine


    glfwSwapBuffers(glfwGetCurrentContext());
}

void Engine::Deinit()
{
    PlatformInterface::Deinit();
}

bool Engine::ShouldShutdown()
{
    return glfwWindowShouldClose(glfwGetCurrentContext());
}
