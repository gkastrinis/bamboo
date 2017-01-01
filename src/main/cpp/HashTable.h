#pragma once

#include <cstdlib>
#include <cstdint>
#include "Bucket.h"

template <typename T>
class HashFunction {
public:
	uint64_t hash(T) { throw 0; }
};

template <>
class HashFunction<uint64_t> {
public:
	uint64_t hash(uint64_t key) {
		key = (key ^ (key >> 30)) * UINT64_C(0xbf58476d1ce4e5b9);
		key = (key ^ (key >> 27)) * UINT64_C(0x94d049bb133111eb);
		key =  key ^ (key >> 31);
		return key;
	}
};



static const size_t HASH_INIT_SIZE = 4;

template <typename T>
class HashTable {
public:
	HashTable() : _size(HASH_INIT_SIZE) {
		_buckets = (Bucket<T>**) malloc(sizeof(Bucket<T>*) * _size);
		for (size_t i = 0 ; i < _size ; i++)
			_buckets[i] = NULL;
	}

	void put(T elem) {
		size_t index = _hf.hash(elem) % _size;
		if (_buckets[index] == NULL) {
			_buckets[index] = new Bucket<T>;
		}
		_buckets[index]->put(elem);
	}


	void debug() {
		for (size_t i = 0 ; i < _size ; ++i) {
			std::cout << "bucket " << i << std::endl;
			if (_buckets[i] != NULL)
				_buckets[i]->debug();
		}
	}

private:
	Bucket<T>**     _buckets;
	size_t          _size;
	HashFunction<T> _hf;
};
