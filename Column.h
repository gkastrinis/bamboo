#pragma once

#include "index/Index.h"
#include "index/ArrayIndex.h"
#include "index/HashIndex.h"

template<typename T>
class Column {
	T key;
	Index<Column<T>> *values;

	explicit Column(const T &key, Index<Column<T>> *values = nullptr) : key(key), values(values) {}

public:
	explicit Column() : values(nullptr) {}

	static Column<T> mkColumn(const T &key) { return Column<T>(key, new ArrayIndex<Column<T>>()); }

	void rmColumn() { delete values; }

	uint64_t hashCode() const {
		uint64_t hash;
		hash = (key ^ (key >> 30)) * UINT64_C(0xbf58476d1ce4e5b9);
		hash = (hash ^ (hash >> 27)) * UINT64_C(0x94d049bb133111eb);
		hash = hash ^ (hash >> 31);
		return hash;
	}

	Column<T> *put(const T &v) {
		auto columnPtr = values->get(Column<T>(v));
		if (columnPtr != nullptr) return columnPtr;
		return values->put(mkColumn(v));
	}

	Column<T> *get(const T &v) { return values->get(Column<T>(v)); }

	bool operator<(const Column<T> &rhs) const { return key < rhs.key; }

	bool operator==(const Column<T> &rhs) const { return key == rhs.key; }

	IndexIterator<Column<T>> *begin() { return values->begin(); }

	IndexIterator<Column<T>> *end() { return values->end(); }

	friend std::ostream &operator<<(std::ostream &os, const Column<T> &col) {
		os << "Column with key: " << col.key << "\n";
		if (col.values != nullptr) col.values->debugPrint();
		return os;
	}
};