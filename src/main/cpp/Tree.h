#pragma once

#include <cstdlib>

static const size_t TREE_INIT_SIZE = 3;

template <typename T>
class Tree {
public:
	Tree(T nullvalue) : _capacity(TREE_INIT_SIZE), _NULLVALUE(nullvalue) {
		_array = (T*) malloc(sizeof(T) * _size);
		for (size_t i = 0 ; i < _size ; i++)
			_array[i] = _NULLVALUE;
	}

private:
	T*      _array;
	size_t  _size;
	const T _NULLVALUE;
};
