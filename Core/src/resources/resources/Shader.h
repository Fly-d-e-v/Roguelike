#pragma once

#include <cereal/cereal.hpp>
#include <cereal/types/polymorphic.hpp>

#include "Resource.h"
class Shader : public Resource {
public:
    

    Shader();

    void SetPath(const std::string& path);

	//Program ID
	unsigned int ID = 0;

    std::string VertexPath;
    std::string FragmentPath;

	void Use();

    template<class Archive>
    void serialize(Archive& ar) {
        ar(GUID, Path, VertexPath, FragmentPath);
    }

};

#include <cereal/archives/xml.hpp>
CEREAL_REGISTER_TYPE(Shader)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Resource, Shader)