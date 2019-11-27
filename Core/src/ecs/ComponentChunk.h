#pragma once

#include <stdint.h>
#include <memory>

#include "Component.h"

#define DEFAULT_CHUNK_SIZE 1024

template <class T>
class ComponentChunk {
public:
	ComponentChunk() {
		_memory = (uint8_t*)malloc(DEFAULT_CHUNK_SIZE);
		_size = DEFAULT_CHUNK_SIZE;
	}

	~ComponentChunk() {
		free(_memory);
		_size = 0;
		_memory = nullptr;
	}

	bool AddComponent(T component, uint32_t entityId) {
		uint32_t newSize = _allocatedSize + sizeof(T);
		if (newSize > _size)
			return false;

		component.entity = entityId;

		memcpy(_memory + _allocatedSize, &component, sizeof(T));

		_allocatedSize = newSize;
		_componentAmount++;

		return true;
	}

	bool GetComponent(T** component, uint32_t id) {
		if (_componentAmount == 0)
			return false;

		uint32_t step = sizeof(T);

		T* c = nullptr;

		for (uint32_t i = 0; i < _componentAmount; i++)
		{
			c = (T*)(_memory + (i * step));
			if (c->entity == id)
			{
				*component = c;
				return true;
			}
		}

		return false;
	}

	template <class U>
	bool IsType(U) {
		return std::is_same<T, U>::value;
	}

protected:
	uint8_t* _memory;

	uint16_t _size;
	uint32_t _allocatedSize;
	uint32_t _componentAmount;
};