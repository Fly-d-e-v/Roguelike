#pragma once

#include "InputEnums.h"

struct KeyState {
    int m_ID;
    EPressedState m_State;
};

struct AxisState {
    int m_ID;
    float m_Value;
};