#pragma once

#include "core/utils/Tool.h"
#include "core/serialization/SerializationHelpers.h"

class Config : Tool{

public:
    
    Config();
    virtual void ToolMethod();

    SERIALIZE_CLASS_BODY_ONE(_VSyncEnabled)

    //GraphicsConfig
    bool _VSyncEnabled = false;

};