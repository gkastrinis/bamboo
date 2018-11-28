#pragma once

#include <functional>
#include "Column.h"
#include "Variations.h"

template<typename T>
class Relation {
	uint8_t arity;
	// Indexing variations (different column orderings)
	Variations variations;
	// Root column for each variation
	Column<T> *rootColumns;
	// Relation size for each variation (may differ due to variations dropping columns)
	uint64_t *variationSizes;

public:
	explicit Relation(uint8_t arity, std::initializer_list<std::initializer_list<int8_t>> variationList = {})
			: arity(arity), variations(arity, variationList) {
		auto count = variations.count();
		rootColumns = new Column<T>[count];
		variationSizes = new uint64_t[count];
		for (auto i = 0; i < count; i++) {
			rootColumns[i].alloc();
			variationSizes[i] = 0;
		}
	}

	~Relation() {
		std::function<void(const Column<T> &)> rec;
		rec = [&](Column<T> column) {
			for (auto it = column.iterator(); it->hasData(); it->move())
				rec(it->data());
			column.dealloc();
		};
		for (auto i = 0, count = (int) variations.count(); i < count; i++)
			rec(rootColumns[i]);
		delete[] rootColumns;
		delete[] variationSizes;
	}

	bool put(T *values) {
		auto insertedInDefault = false;
		for (uint8_t i = 0, variationsNum = variations.count(); i < variationsNum; i++) {
			auto anyNewInsertion = false;
			auto currentColumn = &rootColumns[i];
			for (auto j = 0; j < arity; j++) {
				auto index = variations.raw[i][j];
				if (index < 0) break;
				auto result = currentColumn->put(values[index]);
				currentColumn = result.first;
				anyNewInsertion |= result.second;
			}
			if (anyNewInsertion) variationSizes[i]++;
			if (i == 0) insertedInDefault = anyNewInsertion;
		}
		return insertedInDefault;
	}

	const Column<T> &rootFor(uint8_t i) const { return rootColumns[i]; }

	uint64_t sizeFor(uint8_t i) const { return variationSizes[i]; }

	void print(uint8_t i, std::ostream &out = std::cout) {
		T *buffer = new T[arity];
		uint8_t index = 0;
		std::function<void(const Column<T> &)> rec;
		rec = [&](const Column<T> &column) mutable {
			if (column.values->size() == 0) {
				for (auto j = 0; j < index; j++) {
					out << buffer[j];
					if (j < index - 1) out << "\t";
				}
				out << std::endl;
				return;
			}
			for (auto it = column.iterator(); it->hasData(); it->move()) {
				buffer[index++] = it->data().key;
				rec(it->data());
				if (index > 0) index--;
			}
		};
		rec(rootColumns[i]);
		delete[] buffer;
	}
};
