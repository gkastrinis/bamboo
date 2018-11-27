#include <fstream>
#include "relation/Relation.h"

using namespace std;

int main(int argc, char **argv) {
	Relation<int64_t> VPT(2, {{0, 1},
	                          {1, 0}});
	ifstream file("../VPT.csv"); // Value x Var
	int64_t values[2];
	auto counter = 0;
	while (true) {
		file >> values[1] >> values[0]; // Var x Value
		if (file.eof()) break;
		VPT.put(values);
		if (++counter == 100) break;
	}
	cout << "Total: " << VPT.sizeFor(0) << endl;

	Relation<int64_t> ALIAS(2);
	counter = 0;
	auto &vptVarValue = VPT.rootFor(1);
	for (auto outerIt = vptVarValue.iterator(); outerIt->hasData(); outerIt->move())
		for (auto it1 = outerIt->data().iterator(); it1->hasData(); it1->move())
			for (auto it2 = outerIt->data().iterator(); it2->hasData(); it2->move()) {
				counter++;
				values[0] = it1->data().key;
				values[1] = it2->data().key;
				ALIAS.put(values);
			}
	//cout << "--> " << counter << endl;
	cout << "Result: " << ALIAS.sizeFor(0) << endl;

	ofstream out("../Alias.txt");
	ALIAS.print(0, out);
}
