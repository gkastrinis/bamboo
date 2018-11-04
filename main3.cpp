#include <fstream>
#include "index/Index.h"
#include "relation/Relation.h"

using namespace std;

int main(int argc, char **argv) {
	Relation<int64_t> r(2);
	ifstream file("../VPT.csv");
	auto counter = 0;
	while (true) {
		int64_t values[2];
		file >> values[0] >> values[1];
		if (file.eof()) break;
		r.put(values);
		if (++counter == 49) break;
	}
	cout << "Total: " << r.size() << endl;
	r.print();
//	r.VPTtest();
}
