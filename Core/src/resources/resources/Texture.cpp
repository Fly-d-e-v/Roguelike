#include "Texture.h"

Texture::Texture() {
    _Type = EResourceType::Texture;
}

void Texture::SetPath(const std::string& path) {
    Path = path;
}
