#pragma once

#include <iostream>
#include <cstring>
#include "Index.h"
#include "../ArrayIndex.h"

class StrInternHash : public HashIndex<StrIntern> {

	// If T is not UINT64/INT64 then it should provide a `hash` function
	uint64_t getBucketNum(const T &v) {
		uint64_t hash;
		if constexpr (std::is_same_v<T, uint64_t> || std::is_same_v<T, int64_t>)
			hash = hash_UINT64(v);
		else
			hash = v.hash();

		// (hash >> (64 - globalDepth): Check globalDepth most significant bits
		// (1 << globalDepth) - 1: Create a bit string with 1 at the first globalDepth bits
		return (hash >> (64 - globalDepth)) & ((1 << globalDepth) - 1);
	}

	struct HashIndexIterator : public IndexIterator<T> {
		const HashIndex <T> *index;
		uint32_t bucketNumber;
		std::unique_ptr<IndexIterator < T>> bucketIterator;

		explicit HashIndexIterator(const HashIndex <T> *index, uint32_t bucketNumber = 0)
				: index(index), bucketNumber(bucketNumber) {
			if (bucketNumber < index->capacity)
				bucketIterator = index->buckets[bucketNumber]->iterator();
		}

		bool hasData() const { return bucketNumber < index->capacity; }

		const T &data() const { return bucketIterator->data(); }

		void move() {
			// Current bucket has more elements
			if (bucketIterator->hasData()) bucketIterator->move();

			// Move to next "different" bucket if the end of the current bucket is reached
			if (!bucketIterator->hasData()) {
				// Skip bucket pointers that point to the same actual bucket
				auto tmp = index->buckets[bucketNumber];
				while (++bucketNumber < index->capacity && index->buckets[bucketNumber] == tmp);
				// There are still more buckets to iterate over
				if (bucketNumber < index->capacity)
					bucketIterator = index->buckets[bucketNumber]->iterator();
			}
		}
	};

public:
	// A global depth of 1 means that 1 bit is used and so we have two initial buckets (0,1).
	HashIndex() : capacity(2), globalDepth(1) {
		buckets = new Bucket *[capacity];
		buckets[0] = new Bucket();
		buckets[1] = new Bucket();
	}

	~HashIndex() {
		for (auto i = 0; i < capacity;) {
			delete buckets[i];
			auto tmp = buckets[i];
			// Skip pointers pointing to same (deleted) bucket
			while (buckets[++i] == tmp);
		}
		delete[] buckets;
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
				for (auto it = bucket->iterator(); it->hasData(); it->move()) {
					auto element = it->data();
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

	std::unique_ptr<IndexIterator < T>> iterator() const { return std::make_unique<HashIndexIterator>(this); }


	static uint64_t hash_UINT64(uint64_t key) {
		uint64_t hash;
		hash = (key ^ (key >> 30)) * UINT64_C(0xbf58476d1ce4e5b9);
		hash = (hash ^ (hash >> 27)) * UINT64_C(0x94d049bb133111eb);
		hash = hash ^ (hash >> 31);
		return hash;
	}
};
