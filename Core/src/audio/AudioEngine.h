#pragma once

#include <memory>

class AudioEngine {
    
public:

    static std::shared_ptr<AudioEngine> Instance();

    bool Init();

private:

    static std::shared_ptr<AudioEngine> s_Instance;

};