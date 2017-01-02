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
	for (uint64_t i = 0 ; i < 20 ; ++i)
		ht.put(rand() % 10000);
	//ht.debug();
	//ht.put(-121);
}
