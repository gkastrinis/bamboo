#pragma once

#include <cstdint>
#include <iostream>

// NOTE: Only meant for primitive types
template<typename T>
struct Index {
	Index<T> *next;

	Index() : next(nullptr) {};

	virtual ~Index() = default;

	virtual void put(T v) = 0;

	virtual bool contains(T v) = 0;

	virtual uint64_t size() = 0;

	virtual void debugPrint() = 0;
};

template<typename T>
struct KV {
	T key;
	Index<T> *value;

	explicit KV() {}

	KV(const T &key, Index<T> *const value) : key(key), value(value) {}

	static KV<T> foo(const T &key, Index<T> *const value) { return KV<T>(key, value); }

	bool operator<(const KV<T> &rhs) const { return key < rhs.key; }

	bool operator==(const KV<T> &rhs) const { return key == rhs.key; }
};

template<typename T>
std::ostream &operator<<(std::ostream &os, const KV<T> &kv) {
	os << kv.key;
	return os;
}