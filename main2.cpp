#include "index/Index.h"
#include "Column.h"

using namespace std;

int main(int argc, char **argv) {
	auto column1 = Column<uint64_t>::mkColumn(0); // value is not important
	Column<uint64_t> *column2Ptr, *column3Ptr;

	column2Ptr = column1.put(1000);
	column3Ptr = column2Ptr->put(110);
	column3Ptr->put(10);
	column3Ptr = column2Ptr->put(120);
	column3Ptr->put(20);
	column3Ptr = column2Ptr->put(130);
	column3Ptr->put(30);

	column2Ptr = column1.put(2000);
	column3Ptr = column2Ptr->put(210);
	column3Ptr->put(40);
	column3Ptr = column2Ptr->put(220);
	column3Ptr->put(50);

	auto it = column1.iterator();
	for (; it->hasNext(); it->move())
		cout << it->data() << endl;
	delete it;

	column1.rmColumn();
}
