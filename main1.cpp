#include <ctime>
#include "index/Index.h"
#include "index/ArrayIndex.h"
#include "index/HashIndex.h"

using namespace std;

int main(int argc, char **argv) {
	time_t t = (argc != 2) ? std::time(nullptr) : atoi(argv[1]);
	srand(t);
	cout << "Seed: " << t << endl;

	Index<uint64_t> *index = new ArrayIndex<uint64_t>();

	uint64_t values[] = {100, 42, 80, 4, 10};
	//int S = 100000;
	//uint64_t values[S];
	//for (auto i = 0; i < S; i++) values[i] = rand() % S;

	for (auto i = 0; i < sizeof(values)/sizeof(*values);) {
		try {
			index->put(values[i]);
		} catch (int e) {
			cout << "---- new index ---- " << i << endl;
			auto old = index;
			index = new HashIndex<uint64_t>();
			delete old;
			for (auto j = 0; j < i; j++) index->put(values[j]);
			continue;
		}
		i++;
	}
	index->debugPrint();
}
