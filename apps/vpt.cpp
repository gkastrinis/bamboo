#include <fstream>
#include <ctime>
#include "relation/Relation.h"

using namespace std;

int main(int argc, char **argv) {
	double startTime, endTime;

	startTime = ((double) clock()) / CLOCKS_PER_SEC;
	Relation<int64_t> VPT(2, {{1, 0}});
	ifstream file("../_testIN/VPTtest.csv"); // Value x Var
	int64_t values[2];
	auto counter = 0;
	while (true) {
		file >> values[0] >> values[1];
		if (file.eof()) break;
		VPT.put(values);
		if (++counter == 30000) break;
	}
	endTime = ((double) clock()) / CLOCKS_PER_SEC;
	cout << "VPT elapsed: " << endTime - startTime << endl;
	//cout << "Total: " << VPT.sizeFor(0) << endl;

	startTime = ((double) clock()) / CLOCKS_PER_SEC;
	std::FILE *f = std::fopen("../_testOUT/Alias.dat", "wb");
	Relation<int64_t> ALIAS(2);
	counter = 0;
	auto &vptVarValue = VPT.rootFor(0);
	for (auto outerIt = vptVarValue.iterator(); outerIt->hasData(); outerIt->move())
		for (auto it1 = outerIt->data().iterator(); it1->hasData(); it1->move())
			for (auto it2 = outerIt->data().iterator(); it2->hasData(); it2->move()) {
				counter++;
				values[0] = it1->data().key;
				values[1] = it2->data().key;
				if (ALIAS.put(values))
					std::fwrite(values, sizeof(int64_t), 2, f);
					//std::fprintf(f, "%d\t%d\n", values[0], values[1]);
			}
	//cout << "--> " << counter << endl;
	//cout << "Result: " << ALIAS.sizeFor(0) << endl;
	std::fclose(f);

	endTime = ((double) clock()) / CLOCKS_PER_SEC;
	cout << "Alias elapsed: " << endTime - startTime << endl;
}
