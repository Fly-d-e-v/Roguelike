#include "Shader.h"

#include <glad/glad.h>

void Shader::SetPath(const std::string& path) {
    Path = path;
}

void Shader::Use()
{
	glUseProgram(ID);
}
