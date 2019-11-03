#pragma once

#include <cereal/cereal.hpp>

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