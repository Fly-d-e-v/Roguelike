#pragma once

#include "input/utils/InputEnums.h"
#include "input/utils/InputTypes.h"

#include <map>
#include <memory>

class Controller {

public:

    Controller(int id);

    void Init();
    void ProcessStates();

    std::shared_ptr<KeyState> GetButtonState(int id);
    std::shared_ptr<AxisState> GetAxisState(int id);

private:

    std::map<int, std::shared_ptr<KeyState>> m_Buttons;
    std::map<int, std::shared_ptr<AxisState>> m_Axis;

    int m_ControllerID;
    bool m_Connected;

};