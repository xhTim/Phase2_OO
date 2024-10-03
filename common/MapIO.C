#ifndef MAPIO_H
#define MAPIO_H

struct MapIO
{
	static void WriteToRoot(const std::map<TString, std::vector<double>> map, TString fileName)
	{
		cout << Form("WriteToRoot-------->Saving Map To The RootFile------->%s",	fileName.Data()) << endl;
		TFile *file = TFile::Open(fileName.Data(), "RECREATE");
		std::vector<TString> keys;

		for (auto it = map.begin(); it != map.end(); ++it)
		{
			TString key 	=	it->first;
			std::cout << "Saving the key:	" << key << endl;
			keys.push_back(key);
			std::vector<double> 	value	=	it->second;
			file->WriteObject(&value, key.Data()); // I store the vector in the TFile
		}
		file->WriteObject(&keys, "keys");
		file->Close();
		cout << Form("WriteToRoot-------->DONE SAVING The RootFile------->%s",	fileName.Data()) << endl << endl;
	}

	static std::map<TString, std::vector<double>> ReadFromRoot(TString fileName)
	{
		cout << Form("ReadFromRoot-------->Reading Map From The RootFile------->%s",	fileName.Data()) << endl;
		TFile *file = TFile::Open(fileName, "READ");
		std::map<TString, std::vector<double>> map;

		std::vector<TString> *keys;
		file->GetObject("keys", keys); // I try to retrieve the vector
		for(auto it = keys->begin(); it != keys->end(); ++it) {
			std::vector<Double_t> *temp;

			file->GetObject(*it, temp);
			std::cout << "Retrieving the key:	" << *it << endl;
			map[*it] = *temp;
		}
		cout << Form("ReadFromRoot-------->DONE READING The RootFile------->%s",	fileName.Data()) << endl << endl;
		return map;
	}

	// static void WriteToText(std::map<TString, std::vector<double>> map, TString fileName)
	// {
	// 	cout << Form("WriteToText-------->Saving Map To The TextFile------->%s",	fileName.Data()) << endl;
	// 	TFile *file = TFile::Open(fileName.Data(), "RECREATE");
	// 	std::vector<TString> keys;

	// 	for (auto it = map.begin(); it != map.end(); ++it)
	// 	{
	// 		TString key 	=	it->first;
	// 		std::cout << "Saving the key:	" << key << endl;
	// 		keys.push_back(key);
	// 		std::vector<double> 	value	=	it->second;
	// 		file->WriteObject(&value, key.Data()); // I store the vector in the TFile
	// 	}
	// 	file->WriteObject(&keys, "keys");
	// 	file->Close();
	// 	cout << Form("WriteToRoot-------->DONE SAVING The RootFile------->%s",	fileName.Data()) << endl << endl;
	// }
};

// void MapIO()
// {
// 	cout<< MapIO::FormatVector( {1,2} ) <<endl;
// }

#endif