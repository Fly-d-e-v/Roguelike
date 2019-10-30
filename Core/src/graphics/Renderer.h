#pragma once

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

    struct GLFWwindow* m_Window;

};