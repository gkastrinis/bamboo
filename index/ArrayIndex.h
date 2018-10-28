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

	// Binary Search
	// Return a pointer to the element (if found)
	// and the position where it should have been (due to sorting)
	std::pair<T *, uint8_t> find(const T &v) {
		if (this->size_ == 0) return {nullptr, 0};

		auto start = 0, end = this->size_ - 1, middle = end;
		while (start <= end) {
			middle = (start + end) / 2;
			if (buffer[middle] == v) return {&buffer[middle], middle};
			else if (buffer[middle] < v) start = middle + 1;
			else end = middle - 1;
		}
		return {nullptr, (uint8_t) (buffer[middle] < v ? middle + 1 : middle)};
	}

	struct ArrayIndexIterator : public IndexIterator<T> {
		ArrayIndex<T> *index;
		uint8_t pos;

		ArrayIndexIterator(ArrayIndex<T> *index, const uint8_t pos) : index(index), pos(pos) {}

		IndexIterator<T> *operator++() {
			++pos;
			return this;
		}

		bool operator!=(const IndexIterator<T> *other) const { return pos != ((ArrayIndexIterator *) other)->pos; }

		const T &operator*() const { return index->buffer[pos]; };
	};

public:
	ArrayIndex() : capacity(1) { buffer = new T[capacity]; }

	~ArrayIndex() { delete[] buffer; }

	T *put(const T &v) {
		auto searchResult = find(v);
		if (searchResult.first != nullptr) return searchResult.first;
		auto i = searchResult.second;

		if (this->size_ >= capacity) {
			capacity *= 2;
			if (capacity > MAX_CAPACITY) throw -1;

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
		return &buffer[i];
	}

	T *get(const T &v) { return find(v).first; }

	bool contains(const T &v) { return find(v).first != nullptr; }

	bool isFull() { return this->size_ >= MAX_CAPACITY; }

	IndexIterator<T> *begin() { return new ArrayIndexIterator(this, 0); }

	IndexIterator<T> *end() { return new ArrayIndexIterator(this, this->size_); }


	void debugPrint() {
		std::cout << "Total: " << (int) this->size_ << std::endl;
		for (auto i = 0; i < this->size_; i++)
			std::cout << buffer[i] << " ";
		std::cout << std::endl;
	}

	const T *rawData() { return buffer; }
};