#include "EntityManager.h"
#include "Component.h"


void EntityManager::Init() {
	AddComponentChunk<TestComponentA>();
	AddComponentChunk<TestComponentB>();
}

EntityManager::~EntityManager() {
	
}

Entity EntityManager::CreateEntity() {
	Entity entity;
	entity.id = _ids;
	_entities.push_back(entity);

	_ids++;

	return entity;
}


Entity EntityManager::GetEntity(uint32_t id) {
	for (auto& i : _entities)
	{
		if (i.id == id)
			return i;
	}

	return Entity();
}