#pragma once

#include "../index/Index.h"
#include "../index/ArrayIndex.h"
#include "../index/HashIndex.h"

template<typename T>
struct Column {
	T key;
	Index<Column<T>> *values;

	explicit Column(const T &key = T(), Index<Column<T>> *values = nullptr) : key(key), values(values) {}

	void alloc() { values = new ArrayIndex<Column<T>>(); }

	void dealloc() {
		if (values) {
			delete values;
			values = nullptr;
		}
	}

	// Pointer to the (maybe new) element, and whether it is a new element
	std::pair<Column<T> *, bool> put(const T &v) {
		// Value is already in the index
		auto columnPtr = values->get(Column<T>(v));
		if (columnPtr != nullptr) return {columnPtr, false};

		std::pair<Column<T> *, bool> result;
		Column<T> column(v, new ArrayIndex<Column<T>>());
		try {
			result = values->put(column);
		} catch (int e) {
			auto old = values;
			values = new HashIndex<Column<T>>();
			for (auto it = old->iterator(); it->hasData(); it->move())
				values->put(it->data());
			delete old;
			result = values->put(column);
		}
		return result;
	}

	Column<T> *get(const T &v) { return values->get(Column<T>(v)); }

	bool operator<(const Column<T> &rhs) const { return key < rhs.key; }

	bool operator==(const Column<T> &rhs) const { return key == rhs.key; }

	std::unique_ptr<IndexIterator<Column<T>>> iterator() const { return values->iterator(); }

	friend std::ostream &operator<<(std::ostream &os, const Column<T> &col) {
		os << "Column with key: " << col.key;
		return os;
	}

	uint64_t hash() const {
		if constexpr (std::is_same_v<T, uint64_t> || std::is_same_v<T, int64_t>)
			return HashIndex<T>::hash_UINT64(key);
		else
			return key.hash();
	}
};
