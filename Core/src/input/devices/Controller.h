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

    std::map<int, std::shared_ptr<KeyState>> _Buttons;
    std::map<int, std::shared_ptr<AxisState>> _Axis;

    int _ControllerID;
    bool _Connected;

};