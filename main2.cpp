#include <ctime>
#include "index/Index.h"
#include "index/ArrayIndex.h"
#include "index/HashIndex.h"

using namespace std;

int main(int argc, char **argv) {
	time_t t = (argc != 2) ? std::time(nullptr) : atoi(argv[1]);
	srand(t);
	cout << "Main 2 Seed: " << t << endl;

	Index<uint64_t> *index1 = new ArrayIndex<uint64_t>();
	uint64_t values1[] = {100, 42, 80, 4, 10};
	for (auto i = 0; i < sizeof(values1)/sizeof(*values1);i++) index1->put(values1[i]);

	Index<uint64_t> *index2 = new ArrayIndex<uint64_t>();
	uint64_t values2[] = {8, 5, 35};
	for (auto i = 0; i < sizeof(values2)/sizeof(*values2);i++) index2->put(values2[i]);

	ArrayIndex<KV<uint64_t>> top;
	top.put(KV<uint64_t>::foo(1000, index1));
	top.put(KV<uint64_t>::foo(2000, index2));
	top.debugPrint();
	cout << top.contains(KV<uint64_t>::foo(1000, nullptr)) << endl;
	cout << top.contains(KV<uint64_t>::foo(1010, nullptr)) << endl;
}
