#pragma once

#include <cstdlib>
#include <cstdint>
#include "HashFunction.h"

template <typename T>
class HashTable {
private:
	struct Bucket;
public:
	HashTable(uint64_t bucketSize) : _capacity(1), _size(0), _gDepth(0), _bucketCap(bucketSize) {
		_buckets = (Bucket**) malloc(sizeof(Bucket*));
		_buckets[0] = new Bucket(_bucketCap);
	}

	void put(T elem) {
		while (true) {
			uint64_t bucketNum = getBucketNum(elem);
			Bucket* b = _buckets[bucketNum];

			// Element already in the hash table
			if (b->get(elem)) return;

			// Double size
			if (b->isFull() and b->lDepth == _gDepth) {
				_gDepth++;
				_capacity *= 2;
				_buckets = (Bucket**) realloc(_buckets, sizeof(Bucket*) * _capacity);
				for (uint64_t i = _capacity ; i > 0 ; i-=2)
					_buckets[i-1] = _buckets[i-2] = _buckets[(i-1)/2];
				bucketNum *= 2;
			}

			// Split current bucket
			if (b->isFull() and b->lDepth < _gDepth) {
				Bucket* b1 = new Bucket(_bucketCap, b->lDepth + 1);
				Bucket* b2 = new Bucket(_bucketCap, b->lDepth + 1);
				// Number of blocks sharing a pointer
				uint64_t n = 2 << (_gDepth - b1->lDepth);

				uint64_t start  = n * (bucketNum/n);
				uint64_t end    = start + n;
				uint64_t middle = start + ((end - start) / 2);
				for (uint64_t i = start ; i < end ; ++i)
					_buckets[i] = (i < middle) ? b1 : b2;

				// Rehash every element in original bucket using the new global depth
				for (uint64_t i = 0 ; i < b->size ; ++i) {
					T tElem = b->buffer[i];
					_buckets[getBucketNum(tElem)]->put(tElem);
				}
				delete b;
			}
			else {
				b->put(elem);
				_size++;
				return;
			}
		}
	}

	bool get(T elem) {
		uint64_t bucketNum = getBucketNum(elem);
		Bucket* b = _buckets[bucketNum];
		return b->get(elem);
	}

	uint64_t size() { return _size; }


	void debug() {
		for (uint64_t i = 0 ; i < _capacity ; ++i) {
			std::cout << "bucket " << i << " (" << _buckets[i] << ")" << std::endl;
			_buckets[i]->debug();
		}
	}

private:
	HashFunction<T> _hf;
	Bucket**        _buckets;
	uint64_t        _capacity;
	uint64_t        _size;
	uint64_t        _gDepth;
	uint64_t        _bucketCap;

	uint64_t getBucketNum(T elem) {
		uint64_t h = _hf.hash(elem);
		// (h >> (64 - _gDepth): Check _gDepth most significant bits
		// (1 << _gDepth) - 1: Create a bit string with 1 at the first _gDepth bits
		return (h >> (64 - _gDepth)) & ((1 << _gDepth) - 1);
	}
};


template <typename T>
struct HashTable<T>::Bucket {
	Bucket(uint64_t initCap, uint64_t depth = 0) : capacity(initCap), size(0), lDepth(depth) {
		buffer = (T*) malloc(sizeof(T) * capacity);
	}

	~Bucket() {
		free(buffer);
	}

	bool isFull() {
		return size >= capacity;
	}

	void put(T elem) {
		if (isFull()) {
			capacity *= 2;
			buffer = (T*) realloc(buffer, sizeof(T) * capacity);
		}
		buffer[size] = elem;
		size++;
	}

	bool get(T elem) {
		for (uint64_t i = 0 ; i < size ; ++i)
			if (buffer[i] == elem) return true;
		return false;
	}

	void debug() {
		for (uint64_t i = 0 ; i < size ; ++i)
			std::cout << buffer[i] << " ";
		std::cout << std::endl;
	}

	T*       buffer;
	uint64_t capacity;
	uint64_t size;
	uint64_t lDepth;
};
