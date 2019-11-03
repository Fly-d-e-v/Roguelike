#pragma once

#include "core/utils/Tool.h"
#include "core/serialization/SerializationHelpers.h"

class Config : Tool{

public:
    
    Config();
    virtual void ToolMethod();

    SERIALIZE_CLASS_BODY_FOUR(_VSyncEnabled,_FullScreenEnabled, _ScreenWidth, _ScreenHeight)

    //GraphicsConfig
    bool _VSyncEnabled = false;
    bool _FullScreenEnabled = false;
    int _ScreenWidth = 1000;
    int _ScreenHeight = 1000;

};