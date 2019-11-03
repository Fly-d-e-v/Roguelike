#include "Texture.h"

#include <cereal/archives/xml.hpp>
#include <cereal/types/polymorphic.hpp>

Texture::Texture() {
    _Type = EResourceType::Texture;
}

void Texture::SetPath(const std::string& path) {
    Path = path;
}

CEREAL_REGISTER_TYPE(Texture);
CEREAL_REGISTER_POLYMORPHIC_RELATION(Resource, Texture);