#include "Controller.h"

#include "GLFW/glfw3.h"

Controller::Controller(int id) : _ControllerID(id), _Connected(false) {

}

void Controller::Init() {

    for (int i = 0; i <= GLFW_GAMEPAD_BUTTON_LAST; i++) {
        std::shared_ptr<KeyState> button = std::make_shared<KeyState>();
        button->m_ID = i;
        _Buttons.insert(std::make_pair(i, button));
    }

    for (int i = 0; i <= GLFW_GAMEPAD_AXIS_LAST; i++) {
        std::shared_ptr<AxisState> axis = std::make_shared<AxisState>();
        axis->m_ID = i;
        _Axis.insert(std::make_pair(i, axis));
    }
}

void Controller::ProcessStates() {

    _Connected = static_cast<bool>(glfwJoystickPresent(_ControllerID));

    if (_Connected) {
        int count;
        const unsigned char* buttons = glfwGetJoystickButtons(_ControllerID, &count);
        for (size_t i = 0; i < _Buttons.size(); i++) {
            EPressedState previousState = _Buttons[i]->_State;
            switch (buttons[i]) {
            case GLFW_PRESS:
                if (previousState == EPressedState::Released) {
                    _Buttons[i]->_State = EPressedState::Pressed;
                    continue;
                }
                else if (previousState == EPressedState::Pressed) {
                    _Buttons[i]->_State = EPressedState::Held;
                }
                break;
            case GLFW_RELEASE:
                _Buttons[i]->_State = EPressedState::Released;
                break;
            }
        }

        const float* axisStates = glfwGetJoystickAxes(_ControllerID, &count);
        for (size_t i = 0; i < _Axis.size(); i++) {
            _Axis[i]->_Value = axisStates[i];
            if (_Axis[i]->_Value == 1.0f) {
            }
        }
    }
}

std::shared_ptr<KeyState> Controller::GetButtonState(int id) {
    return _Buttons.find(id)->second;
}

std::shared_ptr<AxisState> Controller::GetAxisState(int id) {
    return _Axis.find(id)->second;
}
