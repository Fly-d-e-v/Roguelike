#pragma once

#include <cereal/cereal.hpp>

class Config {

public:
    
    Config();

    template<class Archive>
    void serialize(Archive& ar) {
        ar(
            CEREAL_NVP(_VSyncEnabled), 
            CEREAL_NVP(_FullScreenEnabled),
            CEREAL_NVP(_ScreenWidth),
            CEREAL_NVP(_ScreenHeight));
    }

    //GraphicsConfig
    bool _VSyncEnabled = false;
    bool _FullScreenEnabled = false;
    int _ScreenWidth = 1000;
    int _ScreenHeight = 1000;

};