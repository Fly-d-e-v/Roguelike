#include "Shader.h"

#include <cereal/types/polymorphic.hpp>
#include <cereal/archives/xml.hpp>

#include <glad/glad.h>

Shader::Shader() {
    _Type = EResourceType::Shader;
}

void Shader::SetPath(const std::string& path) {
    Path = path;
}

void Shader::Use()
{
	glUseProgram(ID);
}

CEREAL_REGISTER_TYPE(Shader)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Resource, Shader)