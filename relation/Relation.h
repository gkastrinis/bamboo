#pragma once

#include "Column.h"

template<typename T>
class Relation {
	uint8_t arity;
	// Number of entries
	uint64_t size_;
	Column<T> topColumn;

	struct Printer {
		T *buffer;
		uint8_t arity;
		uint8_t index;

		int counter;

		explicit Printer(uint8_t arity) : buffer(new T[arity]), arity(arity), index(0), counter(0) {}

		~Printer() { delete[] buffer; }

		void push(const T &v) { buffer[index++] = v; }

		void pop() { if (index > 0) index--; }

		void print() {
			counter++;
			std::cout << counter << " " ;
			for (auto i = 0; i < arity; i++)
				std::cout << buffer[i] << " ";
			std::cout << std::endl;
		}
	};

	void flatPrint0(const Column<T> &column, Printer &printer) {
		if (printer.arity == printer.index) {
			printer.print();
			return;
		}
		for (auto it = column.iterator(); it->hasData(); it->move()) {
			printer.push(it->data().key);
			flatPrint0(it->data(), printer);
			printer.pop();
		}
	}

public:
	// value is not important
	explicit Relation(uint8_t arity) : arity(arity), size_(0), topColumn(Column<T>::mkColumn(0)) {}

	~Relation() { topColumn.rmColumn(); }

	void put(T *values) {
		auto anyNewInsertion = false;
		auto currentColumn = &topColumn;
		for (auto i = 0; i < arity; i++) {
			auto result = currentColumn->put(values[i]);
			currentColumn = result.first;
			anyNewInsertion |= result.second;
		}
		if (anyNewInsertion) size_++;
	}

	uint64_t size() const { return size_; }

	void print() {
		Printer printer(arity);
		flatPrint0(topColumn, printer);
	}

	void VPTtest() {
		Relation<T> result(2);
		int64_t values[2];
		uint64_t counter = 0;
		for (auto outerIt = topColumn.iterator(); outerIt->hasData(); outerIt->move())
			for (auto it1 = outerIt->data().iterator(); it1->hasData(); it1->move())
				for (auto it2 = outerIt->data().iterator(); it2->hasData(); it2->move()) {
				//for (auto it2 = it1->cloneAndMove(); it2->hasData(); it2->move()) {
					counter++;
//					if (counter % 400000000 == 0) std::cout << counter << std::endl;
					values[0] = it1->data().key;
					values[1] = it2->data().key;
					result.put(values);
//					std::cout << it1->data() << " " << it2->data() << std::endl;
				}
		std::cout << "--> " << counter << std::endl;
		std::cout << result.size() << std::endl;
	}
};
