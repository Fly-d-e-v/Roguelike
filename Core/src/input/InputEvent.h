#pragma once

#include "core/event/Event.h"
#include "input/utils/InputTypes.h"
#include "input/utils/InputEnums.h"


class InputEvent : public Event<> {

public:

    void AddKeyState(std::shared_ptr<KeyState> keyState);

    void SetEventStateTrigger(EPressedState pressedState);

    void AttemptTrigger();

private:

    std::vector<std::shared_ptr<KeyState>> _KeyAction;
    EPressedState _EventStateTrigger;

};

class AxisEvent : public Event<float> {

public:

    void AddKeyState(std::shared_ptr<AxisState> keyState);

    void AttemptTrigger();

private:

    std::vector<std::shared_ptr<AxisState>> _AxisBinding;

};