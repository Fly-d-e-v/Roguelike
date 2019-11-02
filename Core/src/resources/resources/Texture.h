#pragma once

#include <cereal/types/polymorphic.hpp>
#include "Resource.h"

class Texture : public Resource {

public:

    void SetPath(const std::string& path);

    unsigned int _textureID = 0u;
};

CEREAL_REGISTER_TYPE(Texture);
CEREAL_REGISTER_POLYMORPHIC_RELATION(Resource, Texture);