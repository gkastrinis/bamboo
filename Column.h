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
		// Value is already in the index
		auto columnPtr = values->get(Column<T>(v));
		if (columnPtr != nullptr) return columnPtr;

		Column<T> *ret;
		try {
			ret = values->put(mkColumn(v));
		} catch (int e) {
			std::cout << "---- new index ---- " << std::endl;
			auto old = values;
			values = new HashIndex<Column<T>>();
			auto it = old->iterator();
			for (; it->hasNext(); it->move()) values->put(it->data());
			delete it;
			delete old;
			ret = values->put(mkColumn(v));
		}
		return ret;
	}

	Column<T> *get(const T &v) { return values->get(Column<T>(v)); }

	bool operator<(const Column<T> &rhs) const { return key < rhs.key; }

	bool operator==(const Column<T> &rhs) const { return key == rhs.key; }

	IndexIterator<Column<T>> *iterator() const { return values->iterator(); }


	struct Printer {
		uint8_t arity;
		T *buffer;
		uint64_t index;

		explicit Printer(uint8_t arity) : arity(arity), buffer(new T[arity]), index(0) {}

		~Printer() { delete[] buffer; }

		void push(const T &v) { buffer[index++] = v; }

		void pop() { if (index > 0) index--; }

		void print() {
			for (auto i = 0; i < arity; i++)
				std::cout << buffer[i] << " ";
			std::cout << std::endl;
		}
	};

	void flatPrint(uint8_t arity) {
		Printer printer(arity);
		flatPrint0(*this, printer);
	}

	void flatPrint0(const Column<T> &column, Printer &printer) {
		if (printer.arity == printer.index) {
			printer.print();
			return;
		}
		auto it = column.iterator();
		for (; it->hasNext(); it->move()) {
			printer.push(it->data().key);
			flatPrint0(it->data(), printer);
			printer.pop();
		}
		delete it;
	}

	friend std::ostream &operator<<(std::ostream &os, const Column<T> &col) {
		os << "Column with key: " << col.key << "\n";
		if (col.values != nullptr) col.values->debugPrint();
		return os;
	}
};