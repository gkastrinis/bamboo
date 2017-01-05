#pragma once

#include <cstdlib>
#include <cstdint>
#include "HashFunction.h"

template <typename T>
class HashTable {
private:
	struct Bucket;
public:
	HashTable(uint64_t bucketSize) : _size(1), _gDepth(0), _bucketSize(bucketSize) {
		_buckets = (Bucket**) malloc(sizeof(Bucket*));
		_buckets[0] = new Bucket(_bucketSize);
	}

	void put(T elem) {
		uint64_t bucketNum = getBucketNum(elem);
		Bucket* b = _buckets[bucketNum];

		if (b->get(elem)) return;

		// Double size
		if (b->isFull() and b->lDepth == _gDepth) {
			_gDepth++;
			_size *= 2;
			_buckets = (Bucket**) realloc(_buckets, sizeof(Bucket*) * _size);
			for (uint64_t i = _size ; i > 0 ; i-=2)
				_buckets[i-1] = _buckets[i-2] = _buckets[(i-1)/2];
			bucketNum *= 2;
		}

		// Split current bucket
		if (b->isFull() and b->lDepth < _gDepth) {
			Bucket* b1 = new Bucket(_bucketSize, b->lDepth + 1);
			Bucket* b2 = new Bucket(_bucketSize, b->lDepth + 1);
			// Number of blocks sharing a pointer
			uint64_t n = 2 << (_gDepth - b1->lDepth);

			uint64_t start = n*(bucketNum/n);
			uint64_t end = start + (n/2);
			for (uint64_t i = start ; i < end ; ++i)
				_buckets[i] = b1;
			start = end;
			end = start + (n/2);
			for (uint64_t i = start ; i < end ; ++i)
				_buckets[i] = b2;

			// Rehash every element in original bucket using the new global depth
			for (uint64_t i = 0 ; i < b->count ; ++i) {
				T tElem = b->buffer[i];
				_buckets[getBucketNum(tElem)]->put(tElem);
			}
			delete b;

			put(elem);
		}
		else {
			b->put(elem);
		}
	}

	bool get(T elem) {
		uint64_t bucketNum = getBucketNum(elem);
		Bucket* b = _buckets[bucketNum];
		return b->get(elem);
	}


	void debug() {
		for (uint64_t i = 0 ; i < _size ; ++i) {
			std::cout << "bucket " << i << " (" << _buckets[i] << ")" << std::endl;
			_buckets[i]->debug();
		}
	}

private:
	HashFunction<T> _hf;
	Bucket**        _buckets;
	uint64_t        _size;
	uint64_t        _gDepth;
	uint64_t        _bucketSize;

	uint64_t getBucketNum(T elem) {
		uint64_t h = _hf.hash(elem);
		// (h >> (64 - _gDepth): Check _gDepth most significant bits
		// (1 << _gDepth) - 1: Create a bit string with 1 at the first _gDepth bits
		return (h >> (64 - _gDepth)) & ((1 << _gDepth) - 1);
	}
};


template <typename T>
struct HashTable<T>::Bucket {
	Bucket(uint64_t initSize, uint64_t depth = 0) : size(initSize), count(0), lDepth(depth) {
		buffer = (T*) malloc(sizeof(T) * size);
	}

	~Bucket() {
		free(buffer);
	}

	bool isFull() {
		return count >= size;
	}

	void put(T elem) {
		if (isFull()) {
			size *= 2;
			buffer = (T*) realloc(buffer, sizeof(T) * size);
		}
		buffer[count] = elem;
		count++;
	}

	bool get(T elem) {
		for (uint64_t i = 0 ; i < count ; ++i)
			if (buffer[i] == elem) return true;
		return false;
	}

	void debug() {
		for (uint64_t i = 0 ; i < count ; ++i)
			std::cout << buffer[i] << " ";
		std::cout << std::endl;
	}

	T*       buffer;
	uint64_t size;
	uint64_t count;
	uint64_t lDepth;
};
