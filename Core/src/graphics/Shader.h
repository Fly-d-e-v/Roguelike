#pragma once

#include <glad/glad.h>

class Shader {
public:
	//Program ID
	unsigned int ID = 0;

	void Load(const GLchar* vertexPath, const GLchar* fragmentPath);

	void Use();

};
