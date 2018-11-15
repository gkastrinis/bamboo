#pragma once

#include <iostream>
#include <cstring>
#include "Index.h"

template<typename T>
class ArrayIndex : public Index<T> {
	// Should be kept ordered (ASC)
	T *buffer;
	// Buffer Size
	uint16_t capacity;
	// Max capacity for this specific index
	uint16_t maxCapacity;

	// Binary Search
	// Return a pointer to the element (if found)
	// and the position where it should have been (due to sorting)
	std::pair<T *, uint16_t> search(const T &v) {
		if (this->size_ == 0) return {nullptr, 0};

		// "Bigger" type than uint16 and signed to support negative index values
		int32_t start = 0, end = ((uint16_t) this->size_) - 1, middle = end;
		while (start <= end) {
			middle = (start + end) / 2;
			if (buffer[middle] == v) return {&buffer[middle], middle};
			else if (buffer[middle] < v) start = middle + 1;
			else end = middle - 1;
		}
		return {nullptr, (uint16_t) (buffer[middle] < v ? middle + 1 : middle)};
	}

	struct ArrayIndexIterator : public IndexIterator<T> {
		const ArrayIndex<T> *index;
		uint16_t pos;

		explicit ArrayIndexIterator(const ArrayIndex<T> *index, uint16_t pos = 0) : index(index), pos(pos) {}

		bool hasData() const { return pos < index->size_; }

		const T &data() const { return index->buffer[pos]; }

		void move() { pos++; }

		std::unique_ptr<IndexIterator<T>> clone() const {
			return std::make_unique<ArrayIndexIterator>(index, (uint16_t) pos);
		}

		std::unique_ptr<IndexIterator<T>> cloneAndMove() const {
			return std::make_unique<ArrayIndexIterator>(index, (uint16_t) (pos + 1));
		}
	};

public:
	static const uint16_t DEFAULT_MAX_CAPACITY = 128;

	explicit ArrayIndex(uint16_t maxCapacity = DEFAULT_MAX_CAPACITY) : capacity(1), maxCapacity(maxCapacity) {
		buffer = new T[capacity];
	}

	~ArrayIndex() { delete[] buffer; }

	std::pair<T *, bool> put(const T &v) {
		auto searchResult = search(v);
		// Element found
		if (searchResult.first != nullptr) return {searchResult.first, false};
		auto i = searchResult.second;

		if (this->size_ >= capacity) {
			if (capacity >= maxCapacity) throw -1;
			capacity *= 2;
			// Hack: Overflow to max value for the given int size
			if (capacity == 0) capacity = -1;

			T *tmp = new T[capacity];
			std::memcpy(tmp, buffer, sizeof(T) * i);
			tmp[i] = v;
			std::memcpy(tmp + i + 1, buffer + i, sizeof(T) * (this->size_ - i));
			this->size_++;
			auto old = buffer;
			buffer = tmp;
			delete[] old;
		} else {
			std::memmove(buffer + i + 1, buffer + i, sizeof(T) * (this->size_ - i));
			buffer[i] = v;
			this->size_++;
		}
		return {&buffer[i], true};
	}

	T *get(const T &v) { return search(v).first; }

	bool contains(const T &v) { return get(v) != nullptr; }

	bool isFull() { return this->size_ >= maxCapacity; }

	std::unique_ptr<IndexIterator<T>> iterator() const { return std::make_unique<ArrayIndexIterator>(this); }
};
