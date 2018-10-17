#pragma once

#include <iostream>
#include <cstring>
#include "Index.h"

template<typename T>
class ArrayIndex : public Index<T> {
private:
	static const uint8_t MAX_CAPACITY = 64;

	// Should be kept ordered (ASC)
	T *buffer;
	// Buffer Size
	uint8_t capacity;
	// Number of entries
	uint8_t size_;

	// Binary Search
	// @return the index where the value was found
	// or where it should have been (due to sorting), if it is absent.
	uint8_t find0(T v, bool &found) {
		found = false;
		if (size_ == 0) return 0;

		auto start = 0, end = size_ - 1, middle = end;
		while (start <= end) {
			middle = (start + end) / 2;
			if (buffer[middle] == v) {
				found = true;
				return (uint8_t) middle;
			} else if (buffer[middle] < v) start = middle + 1;
			else end = middle - 1;
		}
		return (uint8_t) (buffer[middle] < v ? middle + 1 : middle);
	}

public:
	ArrayIndex() : capacity(1), size_(0) { buffer = new T[capacity]; }

	~ArrayIndex() { delete[] buffer; }

	void put(T v) {
		bool found;
		auto i = find0(v, found);
		if (found) return;

		if (size_ >= capacity) {
			capacity *= 2;
			if (capacity > MAX_CAPACITY) throw -1;

			T *tmp = new T[capacity];
			std::memcpy(tmp, buffer, sizeof(T) * i);
			tmp[i] = v;
			std::memcpy(tmp + i + 1, buffer + i, sizeof(T) * (size_ - i));
			size_++;
			auto old = buffer;
			buffer = tmp;
			delete[] old;
		} else {
			std::memmove(buffer + i + 1, buffer + i, sizeof(T) * (size_ - i));
			buffer[i] = v;
			size_++;
		}
	}

	bool contains(T v) {
		bool found;
		find0(v, found);
		return found;
	}

	uint64_t size() { return size_; }

	bool isFull() { return size_ >= MAX_CAPACITY; }

	void debugPrint() {
		for (auto i = 0; i < size_; i++)
			std::cout << buffer[i] << " ";
		std::cout << std::endl;
	}

	const T* rawData() { return buffer; }
};