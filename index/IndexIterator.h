#pragma once

#include <iostream>

template<typename T>
struct RawIndexIterator {
	virtual ~RawIndexIterator() = default;

	virtual bool equals(const RawIndexIterator<T> &other) const = 0;

	virtual void move() = 0;

	virtual const T &data() const = 0;
};

template<typename T>
struct IndexIterator final {
	// Keep a pointer to enable virtual dispatch
	RawIndexIterator<T> *delegate;

	explicit IndexIterator(RawIndexIterator<T> *delegate = nullptr) : delegate(delegate) {}

	~IndexIterator() { if (delegate) delete delegate; }

	bool operator==(const IndexIterator &other) const { return delegate->equals(*(other.delegate)); }

	bool operator!=(const IndexIterator &other) const { return !(delegate->equals(*(other.delegate))); }

	void operator++() { delegate->move(); }

	const T &operator*() const { return delegate->data(); }
};