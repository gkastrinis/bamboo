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
	// Relation sizes for each variation (may differ due to variations dropping columns)
	uint64_t *variationsSizes;

	void init() {
		auto variationsNum = variations.count();
		rootColumns = new Column<T>[variationsNum];
		variationsSizes = new uint64_t[variationsNum];
		// NOTE: key value for rootColumns is not important
		for (auto i = 0; i < variationsNum; i++) {
			rootColumns[i] = Column<T>::mk(0);
			variationsSizes[i] = 0;
		}
	}

public:
	explicit Relation(uint8_t arity) : arity(arity), variations(arity) { init(); }

	Relation(uint8_t arity, Variations &variations) : arity(arity), variations(variations) { init(); }

	~Relation() {
		for (uint8_t i = 0, variationsNum = variations.count(); i < variationsNum; i++)
			rootColumns[i].rm();
		delete[] rootColumns;
		delete[] variationsSizes;
	}

	void put(T *values) {
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

		for (uint8_t i = 0, variationsNum = variations.count(); i < variationsNum; i++) {
			std::cout << "variation " << i << ": ";
			for (auto j = 0; j < arity; j++)
				std::cout << (int) variations.raw[i][j] << " ";
			std::cout << "-- size: " << variationsSizes[i] << std::endl;
			rec(rootColumns[i]);
		}

		delete[] buffer;
	}
};
