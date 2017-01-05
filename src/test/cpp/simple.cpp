#include <iostream>
#include <cstdint>
#include <ctime>
#include "HashTable.h"
using namespace std;

int main(int argc, char** argv) {
	HashTable<uint64_t> ht(24);

	time_t t = (argc != 2) ? time(NULL) : atoi(argv[1]);
	srand(t);
	cout << "Seed: " << t << endl;
	for (uint64_t i = 0 ; i < 1000000 ; ++i) {
		uint64_t e = rand() % 100000;
		//cout << "Put(" << e << ")" << endl;
		ht.put(e);
		if (!ht.get(e)) {
			cerr << i << ") Query for " << e << " failed!" << endl;
			return 1;
		}
	}
	//ht.debug();
}
