#version 460 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aCol;
layout(location = 2) in vec2 aUV;
layout(location = 3) in float aTile;

out vec3 vertexColor;
out vec2 UV;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform uint spriteSheetSize;
uniform uvec2 mapSize;

void main() {
	float row = floor(aTile / spriteSheetSize);
	uint collumn = uint(aTile) % spriteSheetSize - 1;
	UV = vec2((collumn + aUV.x) / spriteSheetSize, (spriteSheetSize - 1 - row + aUV.y) /spriteSheetSize);

	vertexColor = aCol;
	gl_Position =  projection * view * model * vec4(aPos.x + gl_InstanceID % mapSize.x , aPos.y - gl_InstanceID / mapSize.x , aPos.z, 1.0f);
}