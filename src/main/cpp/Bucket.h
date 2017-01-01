#pragma once

#include <cstdlib>

static const size_t BUCKET_INIT_SIZE = 2;

template <typename T>
class Bucket {
public:
	Bucket() : _size(BUCKET_INIT_SIZE), _count(0) {
		_buffer = (T*) malloc(sizeof(T) * _size);
	}

	void put(T elem) {
		if (_count == _size) {
			_size *= 2;
			_buffer = (T*) realloc(_buffer, sizeof(T) * _size);
		}
		_buffer[_count] = elem;
		_count++;
	}

	void debug() {
		for (size_t i = 0 ; i < _count ; ++i)
			std::cout << _buffer[i] << " ";
		std::cout << std::endl;
	}

private:
	T*     _buffer;
	size_t _size;
	size_t _count;
};
