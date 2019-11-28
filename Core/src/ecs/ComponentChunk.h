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
		const uint32_t newSize = _allocatedSize + sizeof(T);
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

		const uint32_t step = sizeof(T);

		T* c = nullptr;

		for (uint32_t i = 0; i < _componentAmount; i++)
		{
			c = static_cast<T*>(_memory + (i * step));
			if (c->entity == id)
			{
				*component = c;
				return true;
			}
		}

		return false;
	}

	T* GetComponent(const uint32_t index)
	{
		if (_componentAmount == 0 || index >= _componentAmount)
			return nullptr;

		const uint32_t step = sizeof(T);

		T* c = nullptr;
		c = reinterpret_cast<T*>(_memory + (index * step));

		return c;
	}

	template <class U>
	bool IsType(U) {
		return std::is_same<T, U>::value;
	}

protected:
	uint8_t* _memory = nullptr;

	uint16_t _size = 0;
	uint32_t _allocatedSize = 0;
	uint32_t _componentAmount = 0;
};