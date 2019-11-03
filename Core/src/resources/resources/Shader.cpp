#include "Shader.h"

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
