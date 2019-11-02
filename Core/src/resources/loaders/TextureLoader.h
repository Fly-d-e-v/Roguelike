#pragma once

#include <memory>
#include <string>

class TextureLoader {

public:

    static std::shared_ptr<class Texture> LoadTexture(const std::string& path);
    static void LoadTexture(std::shared_ptr<Texture> textureHandle);
};