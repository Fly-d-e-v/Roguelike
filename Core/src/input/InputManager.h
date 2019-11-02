#pragma once

#include "input/InputEvent.h"

#include <memory>
#include <map>

class InputManager
{

public:

    InputManager();

    bool Init();
    void Tick(float deltaTime);
    void Deinit();

    static InputManager* Instance();

    std::shared_ptr<class Keyboard> GetKeyboard();

    void ButtonActionTestMethod();
    void AxisButtonTestMethod(float axis);

private:

    void TriggerInputEvents();

    void CheckForControllerInput();
    static void key_callback(struct GLFWwindow* window, int key, int scancode, int action, int mods);
    static void cursor_position_callback(struct GLFWwindow* window, double xpos, double ypos);
    static void mouse_button_callback(struct GLFWwindow* window, int button, int action, int mods);

private:

    std::shared_ptr<class Keyboard> m_Keyboard;
    std::shared_ptr<class Controller> m_Controller;

    std::map<std::string, std::shared_ptr<InputEvent>> m_KeyEvents;
    std::map<std::string, std::shared_ptr<AxisEvent>> m_AxisEvents;

    static InputManager* s_Instance;

};