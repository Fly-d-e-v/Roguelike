#pragma once

#include <memory>

class InputManager
{

public:

    InputManager();

    bool Init();
    void Tick(float deltaTime);
    void Deinit();

    static InputManager* Instance();

    std::shared_ptr<class Keyboard> GetKeyboard();

private:

    void TriggerInputEvents();

    void CheckForControllerInput();
    static void key_callback(struct GLFWwindow* window, int key, int scancode, int action, int mods);
    static void cursor_position_callback(struct GLFWwindow* window, double xpos, double ypos);
    static void mouse_button_callback(struct GLFWwindow* window, int button, int action, int mods);

private:

    std::shared_ptr<class Keyboard> m_Keyboard;

    static InputManager* s_Instance;

};