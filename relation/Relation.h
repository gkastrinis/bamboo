#pragma once

#include <functional>
#include "Column.h"

template<typename T>
class Relation {
	uint8_t arity;
	// Number of indexing variations (different orderings in the relation's columns)
	uint8_t variationsNum;
	// A 2D array with all variations, flattened to 1D
	// Zero based indexing; -1 indicates dropping remaining columns
	int8_t *variations;
	// Relation sizes for each variation (may differ due to variations dropping columns)
	uint64_t *variationsSizes;
	// Root column for each variation
	Column<T> *rootColumns;

public:
	explicit Relation(uint8_t arity, uint8_t variationsNum, int8_t *variations)
			: arity(arity), variationsNum(variationsNum), variations(variations) {
		variationsSizes = new uint64_t[variationsNum];
		rootColumns = new Column<T>[variationsNum];
		// NOTE: key value for rootColumns is not important
		for (auto i = 0; i < variationsNum; i++) {
			variationsSizes[i] = 0;
			rootColumns[i] = Column<T>::mk(0);
		}
	}

	~Relation() {
		delete[] variationsSizes;
		for (auto i = 0; i < variationsNum; i++) rootColumns[i].rm();
		delete[] rootColumns;
	}

	void put(T *values) {
		for (auto i = 0; i < variationsNum; i++) {
			auto anyNewInsertion = false;
			auto currentColumn = &rootColumns[i];
			for (auto j = 0; j < arity; j++) {
				auto index = variations[i * arity + j];
				if (index < 0) break;
				auto result = currentColumn->put(values[index]);
				currentColumn = result.first;
				anyNewInsertion |= result.second;
			}
			if (anyNewInsertion) variationsSizes[i]++;
		}
	}

	uint64_t size(uint8_t i = 0) const { return variationsSizes[i]; }

	Column<T> &variation(uint8_t i = 0) const { return rootColumns[i]; }

	void print() {
		T *buffer = new T[arity];
		uint8_t index = 0;
		std::function<void(const Column<T> &)> rec;
		rec = [&](const Column<T> &column) mutable {
			if (column.values->size() == 0) {
				for (auto i = 0; i < index; i++)
					std::cout << buffer[i] << " ";
				std::cout << std::endl;
				return;
			}
			for (auto it = column.iterator(); it->hasData(); it->move()) {
				buffer[index++] = it->data().key;
				rec(it->data());
				if (index > 0) index--;
			}
		};

		for (auto i = 0; i < variationsNum; i++) {
			std::cout << "variation " << i << ": ";
			for (auto j = 0; j < arity; j++)
				std::cout << (int) variations[i * arity + j] << " ";
			std::cout << "-- size: " << variationsSizes[i] << std::endl;
			rec(rootColumns[i]);
		}

		delete[] buffer;
	}
};
