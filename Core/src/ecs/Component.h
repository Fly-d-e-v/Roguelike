#pragma once
#include <memory>

#include "thirdparty/glm/glm.hpp"

struct Component {
public:
	uint32_t entity = 0;
};

struct TestComponentA : public Component {
public:
	int test = 0;
};

struct TestComponentB : public Component {
public:
	float boat = 0.f;
};

struct RenderComponent : public Component {
	glm::mat4 model_matrix = glm::mat4(1.0f);
	std::shared_ptr<class Texture> texture = nullptr;
};
