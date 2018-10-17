#pragma once

#include <cstdint>

// NOTE: Only meant for primitive types
template<typename T>
class Index {
public:
	Index<T> *next;

	Index() : next(nullptr) {};

	virtual ~Index() = default;

	virtual void put(T v) = 0;

	virtual bool contains(T v) = 0;

	virtual uint64_t size() = 0;

	virtual void debugPrint() = 0;
};