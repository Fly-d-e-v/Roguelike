#pragma once

struct Component {
public:
	uint32_t entity;
};

struct TestComponentA : public Component {
public:
	int test;
};

struct TestComponentB : public Component {
public:
	float boat;
};