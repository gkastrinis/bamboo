#pragma once

#include "index/Index.h"
#include "index/ArrayIndex.h"
#include "index/HashIndex.h"

template<typename T>
struct Column {
	T key;
	Index<Column<T>> *values;

	explicit Column() : values(nullptr) {}

	static Column<T> dummyColumn(const T &key) {
		Column t;
		t.key = key;
		return t;
	}

	static Column<T> mkColumn(const T &key) {
		auto t = dummyColumn(key);
		t.values = new ArrayIndex<Column<T>>();
		return t;
	}

	void rmColumn() { delete values; }

	Column<T> *put(const T &v) {
		auto columnPtr = values->get(dummyColumn(v));
		if (columnPtr != nullptr) return columnPtr;
		return values->put(mkColumn(v));
	}

	Column<T> *get(const T &v) { return values->get(dummyColumn(v)); }

	bool operator<(const Column<T> &rhs) const { return key < rhs.key; }

	bool operator==(const Column<T> &rhs) const { return key == rhs.key; }

	IndexIterator<Column<T>> *begin() { return values->begin(); }

	IndexIterator<Column<T>> *end() { return values->end(); }
};

template<typename T>
std::ostream &operator<<(std::ostream &os, const Column<T> &col) {
	os << "Column with key: " << col.key << "\n";
	if (col.values != nullptr) col.values->debugPrint();
	return os;
}