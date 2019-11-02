#include "InputManager.h"

#include "input/devices/Controller.h"
#include "input/devices/Keyboard.h"

#include "GLFW/glfw3.h"

#include <iostream>

std::shared_ptr<InputManager> InputManager::s_Instance = nullptr;

InputManager::InputManager() {

}

bool InputManager::Init() {

    GLFWwindow* window = glfwGetCurrentContext();
    
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);

    m_Keyboard = std::make_shared<Keyboard>();
    m_Keyboard->Init();

    m_Controller = std::make_shared<Controller>(0);
    m_Controller->Init();

    m_KeyEvents.insert(std::make_pair("OKAY", std::make_shared<InputEvent>()));
    m_KeyEvents.find("OKAY")->second->SetEventStateTrigger(EPressedState::Pressed);
    m_KeyEvents.find("OKAY")->second->AddKeyState(m_Keyboard->GetKeyState(GLFW_KEY_ENTER));
    m_KeyEvents.find("OKAY")->second->AddKeyState(m_Controller->GetButtonState(GLFW_GAMEPAD_BUTTON_A));
    m_KeyEvents.find("OKAY")->second->Register(this, &InputManager::ButtonActionTestMethod);

    return true;
}

void InputManager::Tick(float) {

    m_Controller->ProcessStates();

    for (auto keyEvents : m_KeyEvents) {
        keyEvents.second->AttemptTrigger();
    }

}

void InputManager::Deinit() {

}

std::shared_ptr<InputManager> InputManager::Instance() {

    if (s_Instance == nullptr) {
        s_Instance = std::make_shared<InputManager>();
    }

    return s_Instance;
}

std::shared_ptr<class Keyboard> InputManager::GetKeyboard() {
    return m_Keyboard;
}

void InputManager::ButtonActionTestMethod() {
    printf("OK Action is triggered!");
}

void InputManager::AxisButtonTestMethod(float) {

}

void InputManager::TriggerInputEvents() {

}

void InputManager::CheckForControllerInput() {

}

void InputManager::key_callback(GLFWwindow*, int key, int, int action, int) {
    s_Instance->GetKeyboard()->ProcessKeyEvent(key, action);
}

void InputManager::cursor_position_callback(GLFWwindow*, double, double) {

}

void InputManager::mouse_button_callback(GLFWwindow*, int, int, int) {

}
