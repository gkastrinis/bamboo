#include <ctime>
#include "relation/Column.h"

using namespace std;

int main(int argc, char **argv) {
	time_t t = (argc != 2) ? std::time(nullptr) : atoi(argv[1]);
	srand(t);
	cout << "Seed: " << t << endl;

	auto column = Column<uint64_t>::mkColumn(0); // value is not important

	uint64_t values[] = {100, 42, 80, 4, 10};
	//int S = 100000;
	//uint64_t values[S];
	//for (auto i = 0; i < S; i++) values[i] = rand() % S;

	for (auto v : values) column.put(v);

	for (auto it = column.iterator(); it.hasNext(); it.move())
		cout << it.data() << endl;
}
