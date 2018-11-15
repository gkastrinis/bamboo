#pragma once

#include "../index/Index.h"
#include "../index/ArrayIndex.h"
#include "../index/HashIndex.h"

template<typename T>
class Column {
	explicit Column(const T &key, Index<Column<T>> *values = nullptr) : key(key), values(values) {}

public:
	T key;
	Index<Column<T>> *values;

	explicit Column() : values(nullptr) {}

	static Column<T> mkColumn(const T &key) { return Column<T>(key, new ArrayIndex<Column<T>>()); }

	// TODO check usage
	void rmColumn() { delete values; }

	uint64_t hashCode() const {
		uint64_t hash;
		hash = (key ^ (key >> 30)) * UINT64_C(0xbf58476d1ce4e5b9);
		hash = (hash ^ (hash >> 27)) * UINT64_C(0x94d049bb133111eb);
		hash = hash ^ (hash >> 31);
		return hash;
	}

	// Pointer to the (maybe new) element, and whether it is a new element
	std::pair<Column<T> *, bool> put(const T &v) {
		// Value is already in the index
		auto columnPtr = values->get(Column<T>(v));
		if (columnPtr != nullptr) return {columnPtr, false};

		std::pair<Column<T> *, bool> result;
		try {
			result = values->put(mkColumn(v));
		} catch (int e) {
//			std::cout << "NEW\n";
			auto old = values;
			values = new HashIndex<Column<T>>();
			for (auto it = old->iterator(); it->hasData(); it->move())
				values->put(it->data());
			delete old;
			result = values->put(mkColumn(v));
		}
		return result;
	}

	bool operator<(const Column<T> &rhs) const { return key < rhs.key; }

	bool operator==(const Column<T> &rhs) const { return key == rhs.key; }

	std::unique_ptr<IndexIterator<Column<T>>> iterator() const { return values->iterator(); }

	friend std::ostream &operator<<(std::ostream &os, const Column<T> &col) {
		os << "Column with key: " << col.key;
		return os;
	}
};
