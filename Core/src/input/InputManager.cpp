#include "InputManager.h"

#include "input/devices/Keyboard.h"

#include "GLFW/glfw3.h"

#include <iostream>

InputManager* InputManager::s_Instance = nullptr;

InputManager::InputManager() {

}

bool InputManager::Init() {


    GLFWwindow* window = glfwGetCurrentContext();
    
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);

    m_Keyboard = std::make_shared<Keyboard>();
    m_Keyboard->Init();

    return true;
}

void InputManager::Tick(float) {

}

void InputManager::Deinit() {

}

InputManager* InputManager::Instance() {

    if (s_Instance == nullptr) {
        s_Instance = new InputManager();
    }

    return s_Instance;
}

std::shared_ptr<class Keyboard> InputManager::GetKeyboard() {
    return m_Keyboard;
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
