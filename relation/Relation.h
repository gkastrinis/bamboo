#pragma once

#include <functional>
#include "Column.h"

template<typename T>
struct Printer {
	T *buffer;
	uint8_t index;

	explicit Printer(uint8_t arity) : buffer(new T[arity]), index(0) {}

	~Printer() { delete[] buffer; }

	void push(const T &v) { buffer[index++] = v; }

	void pop() { if (index > 0) index--; }

	void print() const {
		for (auto i = 0; i < index; i++)
			std::cout << buffer[i] << " ";
		std::cout << std::endl;
	}
};

template<typename T>
class Relation {
	uint8_t arity;
	uint64_t size_;
	Column<T> rootColumn;

public:
	// NOTE: key value for rootColumn is not important
	explicit Relation(uint8_t arity, bool *indexesToIgnore = nullptr)
			: arity(arity), size_(0), rootColumn(Column<T>::mk(0)) {}

	~Relation() { rootColumn.rm(); }

	void put(T *values) {
		auto anyNewInsertion = false;
		auto currentColumn = &rootColumn;
		for (auto i = 0; i < arity; i++) {
			auto result = currentColumn->put(values[i]);
			currentColumn = result.first;
			anyNewInsertion |= result.second;
		}
		if (anyNewInsertion) size_++;
	}

	uint64_t size() const { return size_; }

	void print() {
		Printer<T> printer(arity);
		std::function<void(const Column<T> &)> rec;
		rec = [&](const Column<T> &column) mutable {
			if (column.values->size() == 0) {
				printer.print();
				return;
			}
			for (auto it = column.iterator(); it->hasData(); it->move()) {
				printer.push(it->data().key);
				rec(it->data());
				printer.pop();
			}
		};
		rec(rootColumn);
	}


	void VPTtest() {
		Relation<T> result(2);
		int64_t values[2];
		uint64_t counter = 0;
		for (auto outerIt = rootColumn.iterator(); outerIt->hasData(); outerIt->move())
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
