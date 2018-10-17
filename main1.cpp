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
	//uint64_t values[100000];
	//for (auto i = 0; i < 100000; i++) values[i] = rand() % 100000;

	for (auto i = 0; i < 100000;) {
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
