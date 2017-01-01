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


template <typename T>
class HashTable {
private:
	struct Bucket;
public:
	HashTable() : _size(1), _gDepth(0) {
		_buckets = (Bucket**) malloc(sizeof(Bucket*));
		_buckets[0] = new Bucket;
	}

	void put(T elem) {
		uint64_t bucketNum = getBucketNum(elem);
		Bucket* b = _buckets[bucketNum];

		// Double size
		if (b->isFull() and b->lDepth == _gDepth) {
			_gDepth++;
			_size *= 2;
			_buckets = (Bucket**) realloc(_buckets, sizeof(Bucket*) * _size);
			for (uint64_t i = _size ; i > 0 ; i-=2)
				_buckets[i-1] = _buckets[i-2] = _buckets[(i-1)/2];
		}

		// Split current bucket
		if (b->isFull() and b->lDepth < _gDepth) {
			Bucket* b1 = new Bucket;
			Bucket* b2 = new Bucket;
			// Rehash every element in original bucket using the new global depth
			for (uint64_t i = 0 ; i < b->count ; ++i) {
				T tElem = b->buffer[i];
				uint64_t tBucketNum = getBucketNum(tElem);
				// Last bit is 0
				if (tBucketNum % 2 == 1)
					b1->put(tElem);
				else
					b2->put(tElem);
			}
			// Repeat for current element (need to rehash)
			uint64_t tBucketNum = getBucketNum(elem);
			// Last bit is 0
			if (tBucketNum % 2 == 0)
				b1->put(elem);
			else
				b2->put(elem);

			// Last bit is 0
			if (bucketNum % 2 == 0) {
				_buckets[bucketNum]   = b1;
				_buckets[bucketNum+1] = b2;
			}
			else {
				_buckets[bucketNum-1] = b1;
				_buckets[bucketNum]   = b2;
			}
			b1->lDepth = b2->lDepth = b->lDepth + 1;
			delete b;
		}
		else {
			b->put(elem);
		}
	}


	void debug() {
		for (uint64_t i = 0 ; i < _size ; ++i) {
			std::cout << "bucket " << i << std::endl;
			_buckets[i]->debug();
		}
	}

private:
	HashFunction<T> _hf;
	Bucket**        _buckets;
	uint64_t        _size;
	uint64_t        _gDepth;

	uint64_t getBucketNum(T elem) {
		uint64_t h = _hf.hash(elem);
		// Create a bit string with 1 at the first _gDepth bits
		return h & ((1 << _gDepth) - 1);
	}
};


#define BUCKET_INIT_SIZE 1

template <typename T>
struct HashTable<T>::Bucket {
	Bucket() : size(BUCKET_INIT_SIZE), count(0), lDepth(0) {
		buffer = (T*) malloc(sizeof(T) * size);
	}

	~Bucket() {
		free(buffer);
	}

	bool isFull() {
		return count == size;
	}

	void put(T elem) {
		if (isFull()) {
			size *= 2;
			buffer = (T*) realloc(buffer, sizeof(T) * size);
		}
		buffer[count] = elem;
		count++;
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
