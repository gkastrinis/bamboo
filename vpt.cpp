#include <fstream>
#include "relation/Relation.h"

using namespace std;

int main(int argc, char **argv) {
	int8_t var[] = {0, 1, 1, 0}; // 2 Variations
	Relation<int64_t> VPT(2, 2, var);
	ifstream file("../VPT.csv"); // Value x Var
	int64_t values[2];
	auto counter = 0;
	while (true) {
		file >> values[1] >> values[0]; // Var x Value
		if (file.eof()) break;
		VPT.put(values);
		if (++counter == 10000) break;
 	}
	cout << "Total: " << VPT.size() << endl;

	int8_t aliasVar[] = {0, 1};
	Relation<int64_t> ALIAS(2, 1, aliasVar);
	counter = 0;

	auto &vptVarValue = VPT.variation(1);

	for (auto outerIt = vptVarValue.iterator(); outerIt->hasData(); outerIt->move())
		for (auto it1 = outerIt->data().iterator(); it1->hasData(); it1->move())
			for (auto it2 = outerIt->data().iterator(); it2->hasData(); it2->move()) {
				//for (auto it2 = it1->cloneAndMove(); it2->hasData(); it2->move()) {
				counter++;
//				if (counter % 400000000 == 0) cout << counter << endl;
				values[0] = it1->data().key;
				values[1] = it2->data().key;
				ALIAS.put(values);
//				cout << it1->data() << " " << it2->data() << endl;
			}
	cout << "--> " << counter << endl;
	cout << ALIAS.size() << endl;
}
