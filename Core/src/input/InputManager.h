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

    static std::shared_ptr<InputManager> Instance();

    std::shared_ptr<class Keyboard> GetKeyboard();

    void ButtonActionTestMethod();
    void AxisButtonTestMethod(float axis);

    template<typename BindingType>
    void BindInputAction(std::string actionName, BindingType* bindingTypeInstance, void(BindingType::* FunctionPtr)()) {
        auto keyEvent = _KeyEvents.find(actionName);
        if (keyEvent != _KeyEvents.end()) {
            keyEvent->second->Register(bindingTypeInstance, FunctionPtr);
        }
    }

    template<typename BindingType>
    void BindInputAxis(std::string actionName, BindingType* bindingTypeInstance, void(BindingType::* FunctionPtr)(float)) {
        auto axisEvent = _AxisEvents.find(actionName);
        if (axisEvent != _AxisEvents.end()) {
            axisEvent->second->Register(bindingTypeInstance, FunctionPtr);
        }
    }

private:

    void TriggerInputEvents();

    void CheckForControllerInput();
    static void key_callback(struct GLFWwindow* window, int key, int scancode, int action, int mods);
    static void cursor_position_callback(struct GLFWwindow* window, double xpos, double ypos);
    static void mouse_button_callback(struct GLFWwindow* window, int button, int action, int mods);

private:

    std::shared_ptr<class Keyboard> _Keyboard;
    std::shared_ptr<class Controller> _Controller;

    std::map<std::string, std::shared_ptr<InputEvent>> _KeyEvents;
    std::map<std::string, std::shared_ptr<AxisEvent>> _AxisEvents;

    static std::shared_ptr<InputManager> s_Instance;

};