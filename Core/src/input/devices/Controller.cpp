#include "Controller.h"

#include "GLFW/glfw3.h"

Controller::Controller(int id) : m_ControllerID(id), m_Connected(false) {

}

void Controller::Init() {

    for (int i = 0; i <= GLFW_GAMEPAD_BUTTON_LAST; i++) {
        std::shared_ptr<KeyState> button = std::make_shared<KeyState>();
        button->m_ID = i;
        m_Buttons.insert(std::make_pair(i, button));
    }

    for (int i = 0; i <= GLFW_GAMEPAD_AXIS_LAST; i++) {
        std::shared_ptr<AxisState> axis = std::make_shared<AxisState>();
        axis->m_ID = i;
        m_Axis.insert(std::make_pair(i, axis));
    }
}

void Controller::ProcessStates() {

    m_Connected = static_cast<bool>(glfwJoystickPresent(m_ControllerID));

    if (m_Connected) {
        int count;
        const unsigned char* buttons = glfwGetJoystickButtons(m_ControllerID, &count);
        for (int i = 0; i < m_Buttons.size(); i++) {
            EPressedState previousState = m_Buttons[i]->m_State;
            switch (buttons[i]) {
            case GLFW_PRESS:
                if (previousState == EPressedState::Released) {
                    m_Buttons[i]->m_State = EPressedState::Pressed;
                    continue;
                }
                else if (previousState == EPressedState::Pressed) {
                    m_Buttons[i]->m_State = EPressedState::Held;
                }
                break;
            case GLFW_RELEASE:
                m_Buttons[i]->m_State = EPressedState::Released;
                break;
            }
        }

        const float* axisStates = glfwGetJoystickAxes(m_ControllerID, &count);
        for (int i = 0; i < m_Axis.size(); i++) {
            m_Axis[i]->m_Value = axisStates[i];
            if (m_Axis[i]->m_Value == 1.0f) {
            }
        }
    }
}

std::shared_ptr<KeyState> Controller::GetButtonState(int id) {
    return m_Buttons.find(id)->second;
}

std::shared_ptr<AxisState> Controller::GetAxisState(int id) {
    return m_Axis.find(id)->second;
}
