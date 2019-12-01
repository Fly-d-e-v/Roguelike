#pragma once

#include "Resource.h"

class Texture : public Resource {

public:

    Texture();

    void SetPath(const std::string& path);

    virtual void ImGuiDisplay();
    virtual void ImGuiNewDisplay() override;

    unsigned int _textureID = 0u;
};