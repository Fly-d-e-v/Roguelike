#version 460 core

out vec4 FragColor;

in vec3 vertexColor;
in vec2 UV;

uniform sampler2D albedo;

void main()
{	
	FragColor = vec4(vertexColor, 1.f) * texture(albedo, UV);
}