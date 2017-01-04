#pragma once

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