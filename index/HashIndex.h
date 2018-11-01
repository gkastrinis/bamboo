#pragma once

#include <iostream>
#include <cstring>
#include "Index.h"
#include "IndexIterator.h"
#include "ArrayIndex.h"

template<typename T>
class HashIndex : public Index<T> {
private:
	struct Bucket : public ArrayIndex<T> {
		explicit Bucket(uint8_t localDepth = 1) : localDepth(localDepth) {}

		uint8_t localDepth;
	};

	Bucket **buckets;
	// Buckets Size
	uint32_t capacity;
	// Denotes how many bits are used for hashing
	uint8_t globalDepth;

	uint64_t getBucketNum(const T &v) {
		// (hash >> (64 - globalDepth): Check globalDepth most significant bits
		// (1 << globalDepth) - 1: Create a bit string with 1 at the first globalDepth bits
		return (v.hashCode() >> (64 - globalDepth)) & ((1 << globalDepth) - 1);
	}

	struct RawHashIndexIterator : public RawIndexIterator<T> {
		HashIndex<T> *index;
		uint32_t currentBucket;
		IndexIterator<T> bucketIterator;

		explicit RawHashIndexIterator(HashIndex<T> *index) : index(index), currentBucket(0) {
			if (currentBucket < index->capacity)
				bucketIterator = index->buckets[currentBucket]->iterator();
		}

		bool hasNext() const { return currentBucket < index->capacity; }

		void move() {
			// Current bucket has more elements
			if (bucketIterator.hasNext()) bucketIterator.move();

			// Move to next "different" bucket if the end of the current bucket is reached
			if (!bucketIterator.hasNext()) {
				// Skip bucket pointers that point to the same actual bucket
				Bucket *previousBucket;
				do {
					previousBucket = index->buckets[currentBucket++];
				} while (previousBucket == index->buckets[currentBucket]);

				// There are still more buckets to iterate over
				if (currentBucket < index->capacity)
					bucketIterator = index->buckets[currentBucket]->iterator();
			}
		}

		const T &data() const { return bucketIterator.data(); }
	};

public:
	// A global depth of 1 means that 1 bit is used and so we have two initial buckets (0,1).
	HashIndex() : capacity(2), globalDepth(1) {
		buckets = new Bucket *[capacity];
		buckets[0] = new Bucket();
		buckets[1] = new Bucket();
	}

	std::pair<T *, bool> put(const T &v) {
		while (true) {
			auto bucketNum = getBucketNum(v);
			auto bucket = buckets[bucketNum];

			// Value already in the hash table
			auto t = bucket->get(v);
			if (t != nullptr) return {t, false};

			// Double the number of buckets
			if (bucket->isFull() and bucket->localDepth == globalDepth) {
				globalDepth++;
				capacity *= 2;
				auto tmp = new Bucket *[capacity];
				auto tmpDepths = new uint8_t[capacity];
				// Each pair of bucket pointers point to the same original bucket
				for (auto i = 0; i < capacity; i += 2)
					tmp[i] = tmp[i + 1] = buckets[i / 2];
				auto old = buckets;
				buckets = tmp;
				delete[] old;
				bucketNum *= 2;
			}

			// Split current bucket
			if (bucket->isFull() and bucket->localDepth < globalDepth) {
				auto b1 = new Bucket(bucket->localDepth + (uint8_t) 1);
				auto b2 = new Bucket(bucket->localDepth + (uint8_t) 1);
				// Number of bucket sharing a pointer (they are in groups of n elements)
				auto n = 1 << (globalDepth - bucket->localDepth);
				// Not a NOP because '/' is integer division
				auto start = n * (bucketNum / n);
				auto end = start + n;
				auto middle = start + ((end - start) / 2);
				for (auto i = start; i < end; i++)
					buckets[i] = (i < middle) ? b1 : b2;

				// Re-Hash every element in original bucket using the new global depth
				for (auto i = 0; i < bucket->size(); i++) {
					auto element = bucket->rawData()[i];
					buckets[getBucketNum(element)]->put(element);
				}
				delete bucket;
			} else {
				auto result = bucket->put(v);
				this->size_++;
				return result;
			}
		}
	}

	T *get(const T &v) { return buckets[getBucketNum(v)]->get(v); }

	bool contains(const T &v) { return buckets[getBucketNum(v)]->contains(v); }

	IndexIterator<T> iterator() { return IndexIterator<T>(new RawHashIndexIterator(this)); }
};