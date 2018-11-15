#include "relation/Column.h"

using namespace std;

int main(int argc, char **argv) {
	auto column = Column<uint64_t>::mkColumn(0); // value is not important
	for (uint16_t i = 0; i < 260; i++) column.put(i);
	for (auto it = column.iterator(); it->hasData(); it->move()) cout << it->data() << endl;
}
