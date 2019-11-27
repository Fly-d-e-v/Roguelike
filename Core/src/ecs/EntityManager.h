#pragma once

#include <vector>
#include <memory>

#include "Entity.h"
#include "ComponentChunk.h"

#include <any>

class EntityManager {
public:
	void Init();
	~EntityManager();

	Entity CreateEntity();
	Entity GetEntity(uint32_t id);

	template<class T>
	void AddComponentChunk();

	template<class T>
	bool AddComponent(T component, Entity entity) {

		for (int i = 0; i < _chunks.size(); i++)
		{
			ComponentChunk<T>* chunk;

			try {
				chunk = std::any_cast<ComponentChunk<T>*>(_chunks[i]);
			}
			catch (const std::bad_any_cast) {
				continue;
			}

			chunk->AddComponent(component, entity.id);
			return true;
		}

		return false;
	}

	template<class T>
	T* GetComponent(Entity entity) {
		for (int i = 0; i < _chunks.size(); i++)
		{
			ComponentChunk<T>* chunk;

			try {
				chunk = std::any_cast<ComponentChunk<T>*>(_chunks[i]);
			}
			catch (const std::bad_any_cast) {
				continue;
			}

			T* component = nullptr;
			if (chunk->GetComponent(&component, entity.id))
				return component;

			return nullptr;
		}

		return nullptr;
	}

private:
	// Just a vector for now
	std::vector<Entity> _entities;

	std::vector<std::any> _chunks;

	uint32_t _ids;
};