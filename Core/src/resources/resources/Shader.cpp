#include "Shader.h"

#include <glad/glad.h>

void Shader::Use()
{
	glUseProgram(ID);
}
