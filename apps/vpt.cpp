#include <fstream>
#include <ctime>
#include "relation/Relation.h"

using namespace std;

int main() {
	double startTime, endTime;

	ifstream r2oFile("../../apps/actions.txt");
	cout << "Open R2O: " << r2oFile.is_open() << endl;
	uint32_t relCount, ruleCount;
	r2oFile >> relCount >> ruleCount;
	auto** relations = new Relation<uint64_t>*[relCount];
	auto outputs = new string[relCount];
	for (auto i = 0 ; i < relCount ; i++) {
		startTime = ((double) clock()) / CLOCKS_PER_SEC;
		uint32_t id, arity;
		string name;
		r2oFile >> id >> name >> arity;
		for (auto j = 0 ; j < arity ; j++) {
			string tmp;
			r2oFile >> tmp;
		}
		cout << name << " " << id << endl;
		relations[i] = new Relation<uint64_t>(name, arity);

		char io;
		r2oFile.ignore();
		r2oFile.read(&io, 1);
		string ioFilename;
		if (io == '<') {
			r2oFile >> ioFilename;
			ifstream ioFile(ioFilename);
			cout << "Input " << ioFilename << ": " << ioFile.is_open() << endl;
			uint64_t values[arity];
			auto counter = 0;
			while (true) {
				for (auto j = 0 ; j < arity ; j++) ioFile >> values[j];
				if (ioFile.eof()) break;
				relations[i]->put(values);
				if (++counter == 30000) break;
			}
		} else if (io == '>') {
			r2oFile >> ioFilename;
			outputs[i] = ioFilename;
		}
		endTime = ((double) clock()) / CLOCKS_PER_SEC;
		cout << name << " elapsed: " << endTime - startTime << endl;
	}

//	Relation<int64_t> VPT("VPT", 2, {{1, 0}});
//	ifstream file("../../_testIN/VPTtest.csv"); // Value x Var
//	cout << "Open VPTtest: " << file.is_open() << endl;
//	int64_t values[2];
//	auto counter = 0;
//	while (true) {
//		file >> values[0] >> values[1];
//		if (file.eof()) break;
//		VPT.put(values);
//		if (++counter == 30000) break;
//	}
//	endTime = ((double) clock()) / CLOCKS_PER_SEC;
//	cout << "VPT elapsed: " << endTime - startTime << endl;
//	//cout << "Total: " << VPT.sizeFor(0) << endl;

	startTime = ((double) clock()) / CLOCKS_PER_SEC;

	std::FILE *f = std::fopen(outputs[1].c_str(), "wb");
	//Relation<int64_t> ALIAS("Alias", 2);
	auto counter = 0;
	auto &vptVarValue = relations[0]->rootFor(0);
	for (auto outerIt = vptVarValue.iterator(); outerIt->hasData(); outerIt->move())
		for (auto it1 = outerIt->data().iterator(); it1->hasData(); it1->move())
			for (auto it2 = outerIt->data().iterator(); it2->hasData(); it2->move()) {
				counter++;
				uint64_t values[2];
				values[0] = it1->data().key;
				values[1] = it2->data().key;
				if (relations[1]->put(values)) {
					std::fwrite(values, sizeof(int64_t), 2, f);}
					//std::fprintf(f, "%d\t%d\n", values[0], values[1]);
					//std::printf("%d\t%d\n", values[0], values[1]);
			}
	//cout << "--> " << counter << endl;
	cout << "Result: " << relations[1]->sizeFor(0) << endl;
	std::fclose(f);

	endTime = ((double) clock()) / CLOCKS_PER_SEC;
	cout << "Alias elapsed: " << endTime - startTime << endl;
}
