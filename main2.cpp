#include "index/Index.h"
#include "relation/Relation.h"

using namespace std;

int main(int argc, char **argv) {
	Relation<uint64_t> r(3);
	uint64_t values[3];

	values[0] = 1000; values[1] = 110; values[2] = 10;
	r.put(values);
	values[1] = 120; values[2] = 20;
	r.put(values);
	values[1] = 130; values[2] = 30;
	r.put(values);
	values[0] = 2000; values[1] = 210; values[2] = 40;
	r.put(values);
	values[1] = 220; values[2] = 50;
	r.put(values);

	r.print();
}
