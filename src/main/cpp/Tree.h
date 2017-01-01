#pragma once

#include <cstdlib>

static const size_t TREE_INIT_SIZE = 3;

template <typename T>
class Tree {
public:
	Tree(T nullvalue) : _size(TREE_INIT_SIZE), _NULLVALUE(nullvalue) {
		_array = (T*) malloc(sizeof(T) * _size);
		for (size_t i = 0 ; i < _size ; i++)
			_array[i] = _NULLVALUE;
	}

	void put(T elem) {
		size_t i = 0;
		while (true) {
			if (_array[i] == _NULLVALUE) {
				_array[i] = elem;
				return;
			}
			else if (elem < _array[i]) {
				i = 2*i + 1;
			}
			else {
				i = 2*i + 2;
			}

			if (i >= _size) {
				size_t oldSize = _size;
				_size = (_size * 2) + 1;
				_array = (T*) realloc(_array, sizeof(T) * _size);
				for (size_t j = oldSize ; j < _size ; ++j)
					_array[j] = _NULLVALUE;
			}
		}
	}


	void debug() {
		for (size_t i = 0 ; i < _size ; ++i) {
			if (_array[i] == _NULLVALUE)
				std::cout << "-" << std::endl;
			else
				std::cout << _array[i] << std::endl;
		}
	}

private:
	T*      _array;
	size_t  _size;
	const T _NULLVALUE;
};
