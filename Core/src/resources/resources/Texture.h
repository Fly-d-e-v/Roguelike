#pragma once

#include "Resource.h"

class Texture : public Resource {

public:

    Texture();

    void SetPath(const std::string& path);
    virtual void ImGuiDisplay();

    unsigned int _textureID = 0u;
};