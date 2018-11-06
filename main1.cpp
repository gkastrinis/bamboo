#include "relation/Column.h"

using namespace std;

int main(int argc, char **argv) {
	auto column = Column<uint64_t>::mkColumn(0); // value is not important
	uint64_t values[] = {100, 42, 80, 4, 10};
	for (auto v : values) column.put(v);
	for (auto it = column.iterator(); it->hasData(); it->move()) cout << it->data() << endl;
}
