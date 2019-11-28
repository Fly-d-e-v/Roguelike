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
	ComponentChunk<T>* AddComponentChunk(){
		auto new_chunk = new ComponentChunk<T>();
		_chunks.push_back(new_chunk);
		return new_chunk;
	}

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

		auto chunk = AddComponentChunk<T>();
		chunk->AddComponent(component, entity.id);

		return true;
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

	template<class T>
	std::vector<T*> GetComponents()
	{
		std::vector<T*> component_list;

		for (auto i = 0; i < _chunks.size(); i++)
		{
			ComponentChunk<T>* chunk;

			try {
				chunk = std::any_cast<ComponentChunk<T>*>(_chunks[i]);
			}
			catch (const std::bad_any_cast) {
				continue;
			}

			auto run = true;
			uint32_t index = 0;
			while (run)
			{
				T* component = chunk->GetComponent(index);
				if (component)
				{
					component_list.push_back(component);
					index++;
				}					
				else
					run = false;
			}				
		}

		return component_list;
	}

private:
	// Just a vector for now
	std::vector<Entity> _entities;

	std::vector<std::any> _chunks;

	uint32_t _ids;
};