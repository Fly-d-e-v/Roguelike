#version 460 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aCol;

out vec3 vertexColor;

void main() {
	vertexColor = aCol;
	gl_Position = vec4(aPos, 1.0f);
}