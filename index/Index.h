#pragma once

#include <cstdint>
#include <iostream>

template<typename T>
struct IndexIterator {
	virtual ~IndexIterator() = default;

	virtual bool hasNext() const = 0;

	virtual void move() = 0;

	virtual const T &data() const = 0;
};

// NOTE: Only meant for primitive and pointer types
template<typename T>
struct Index {
protected:
	// Number of entries
	uint64_t size_;

public:
	Index() : size_(0) {};

	virtual ~Index() = default;

	// Pointer to the (maybe new) element, and whether it is a new element
	virtual std::pair<T *, bool> put(const T &v) = 0;

	virtual T *get(const T &v) = 0;

	virtual bool contains(const T &v) = 0;

	uint64_t size() const { return size_; }

	virtual IndexIterator<T> *iterator() = 0;
};
