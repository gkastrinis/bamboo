#pragma once

#include "Column.h"

template<typename T>
class Relation {
	// Number of entries
	uint64_t size_;
	uint8_t arity;
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
//			for (auto i = 0; i < arity; i++)
//				std::cout << buffer[i] << " ";
//			std::cout << std::endl;
		}
	};

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

public:
	// value is not important
	explicit Relation(uint8_t arity) : size_(0), arity(arity), topColumn(Column<T>::mkColumn(0)) {}

	~Relation() { topColumn.rmColumn(); }

	uint64_t size() const { return size_; }

	void put(T* values) {
		// TODO fix this is not correct if values are already present
		size_++;

		auto currentColumn = &topColumn;
		for (auto i = 0 ; i < arity ; i++)
			currentColumn = currentColumn->put(values[i]);
	}

	void print() {
		Printer printer(arity);
		flatPrint0(topColumn, printer);
		std::cout << printer.counter << std::endl;
	}
};