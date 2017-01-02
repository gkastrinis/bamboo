#include <iostream>
#include <cstdint>
#include <ctime>
#include "HashTable.h"
using namespace std;

int main(int argc, char** argv) {
	HashTable<uint64_t> ht;

	time_t t = (argc != 2) ? time(NULL) : atoi(argv[1]);
	srand(t);
	cout << "Seed: " << t << endl;
	for (uint64_t i = 0 ; i < 20 ; ++i) {
		uint64_t e = rand() % 10000;
		ht.put(e);
		if (!ht.get(e)) {
			cerr << i << ") Query for " << e << " failed!" << endl;
			return 1;
		}
	}
	//ht.debug();
	//ht.put(-121);
}
