#pragma once

#include <cstdlib>
#include <cstdint>

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
static const size_t BUCKET_INIT_SIZE = 2;

template <typename T>
class HashTable {
private:
	struct Bucket;
public:
	HashTable() : _size(HASH_INIT_SIZE) {
		_buckets = (Bucket**) malloc(sizeof(Bucket*) * _size);
		for (size_t i = 0 ; i < _size ; i++)
			_buckets[i] = NULL;
	}

	void put(T elem) {
		size_t index = _hf.hash(elem) % _size;
		if (_buckets[index] == NULL) {
			_buckets[index] = new Bucket;
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
	Bucket**     _buckets;
	size_t          _size;
	HashFunction<T> _hf;
};


template <typename T>
struct HashTable<T>::Bucket {
	Bucket() : size(BUCKET_INIT_SIZE), count(0) {
		buffer = (T*) malloc(sizeof(T) * size);
	}

	void put(T elem) {
		if (count == size) {
			size *= 2;
			buffer = (T*) realloc(buffer, sizeof(T) * size);
		}
		buffer[count] = elem;
		count++;
	}

	void debug() {
		for (size_t i = 0 ; i < count ; ++i)
			std::cout << buffer[i] << " ";
		std::cout << std::endl;
	}

	T*     buffer;
	size_t size;
	size_t count;
};
