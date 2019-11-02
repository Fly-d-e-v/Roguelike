#pragma once

class Shader {
public:
	//Program ID
	unsigned int ID = 0;

	void Load(const char* vertexPath, const char* fragmentPath);

	void Use();

};
