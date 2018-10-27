#pragma once

#include <cstdint>
#include <iostream>

// NOTE: Only meant for primitive and pointer types
template<typename T>
struct Index {
protected:
	// Number of entries
	uint8_t size_;

public:
	Index() : size_(0) {};

	virtual ~Index() = default;

	virtual T *put(const T &v) = 0;

	virtual T *get(const T &v) = 0;

	virtual bool contains(const T &v) = 0;

	uint64_t size() { return size_; }

	virtual void debugPrint() = 0;
};