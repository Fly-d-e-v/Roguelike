#pragma once

#include "InputEnums.h"

struct KeyState {
    int m_ID;
    EPressedState _State;
};

struct AxisState {
    int m_ID;
    float _Value;
};