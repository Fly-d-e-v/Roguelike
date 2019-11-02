#version 460 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aCol;
layout(location = 2) in vec2 aUV;

out vec3 vertexColor;
out vec2 UV;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
	UV = aUV;
	vertexColor = aCol;
	gl_Position =  projection * view * model * vec4(aPos, 1.0f);
}