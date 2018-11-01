#pragma once

#include <iostream>

template<typename T>
struct RawIndexIterator {
	virtual ~RawIndexIterator() = default;

	virtual bool hasNext() const = 0;

	virtual void move() = 0;

	virtual const T &data() const = 0;
};

template<typename T>
struct IndexIterator final {
	// Keep a pointer to enable virtual dispatch
	RawIndexIterator<T> *delegate;

	explicit IndexIterator(RawIndexIterator<T> *delegate = nullptr) : delegate(delegate) {}

	~IndexIterator() { if (delegate) delete delegate; }

	bool hasNext() const { return delegate->hasNext(); }

	void move() { delegate->move(); }

	const T &data() const { return delegate->data(); }
};