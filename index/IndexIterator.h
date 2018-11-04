#pragma once

#include <iostream>
#include <memory>

template<typename T>
struct RawIndexIterator {
	virtual ~RawIndexIterator() = default;

	virtual RawIndexIterator *cloneNext() const = 0;

	virtual bool hasNext() const = 0;

	virtual void move() = 0;

	virtual const T &data() const = 0;
};

template<typename T>
struct IndexIterator final {
	// Keep a pointer to enable virtual dispatch
	std::shared_ptr<RawIndexIterator<T>> delegate;

	explicit IndexIterator(std::shared_ptr<RawIndexIterator<T>> delegate = nullptr) : delegate(delegate) {}

	IndexIterator cloneNext() const { return IndexIterator(delegate->cloneNext()); }

	bool hasNext() const { return delegate ? delegate->hasNext() : false; }

	void operator++() { if (delegate) delegate->move(); }

	const T &operator*() const { return delegate->data(); }
};