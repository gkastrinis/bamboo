#include <iostream>
#include <cstdint>
#include "HashTable.h"
using namespace std;

int main(void) {
	HashTable<uint64_t> ht;

	ht.put(10);
	ht.put(23131);
	ht.put(45645);
	ht.put(46546);
	ht.put(23123);
	ht.put(213);
	ht.put(312312);
	ht.put(6565);
	ht.put(1200);
	ht.debug();
	//ht.put(-121);
}
