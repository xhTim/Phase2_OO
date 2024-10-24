/*
	A data structure for reading Columnized data from files
	Do:
		SingleDataReader(FileDir, ColumnNames)
	There are two Read strategy to choose from IO and Tree

	JiaZhao Lin
	Dec. 2022
*/

#ifndef DATAREADER_H
#define DATAREADER_H


enum class ReadColumnFileStrategy
{
	ReadViaIO,
	ReadViaTree
};

struct ColumnPrototype
{
	std::vector<TString> ColumnNames;

	ColumnPrototype( std::vector<TString> ColumnNames_	) : ColumnNames{ColumnNames_} {}
	ColumnPrototype( TString fileName					)
	{
		//Reading the column names from the first line of the file
		ifstream inFile(	fileName.Data()	);
		if (inFile.is_open())
		{
			std::string line;
			// Read one line at a time into the variable line:
			std::getline(inFile, line);
			std::stringstream  		lineStream(line);

			std::string value;
			// Read an integer at a time from the line
			while(lineStream >> value)
			{
				// Add the integers from a line to a 1D array (vector)
				ColumnNames.push_back(value);
			}
		}
	}
	
	TString getHeader()
	{
		TString header{""};
		for(auto name: ColumnNames){	header += name + ":";	}
		return header.Remove(header.Length()-1);
	}
	int size()	{return ColumnNames.size();}
};

struct DataReader
{
	virtual void Read() 	= 0;
	virtual ~DataReader() 	= default;
};

struct SingleDataReader	:	DataReader
{
	/*
		Input the directory of the file and the column name (as a vector of TString)
	*/
	TString FileDir;
	ColumnPrototype cp;
	bool hasHeader = false;
	ReadColumnFileStrategy ReadStrategy = ReadColumnFileStrategy::ReadViaIO;

	std::map<TString, std::vector<double>> Map;

	SingleDataReader(TString FileDir_)										:	FileDir{FileDir_},	cp{FileDir_},	hasHeader{true}		{ 	Read(); }
	SingleDataReader(TString FileDir_,	ColumnPrototype cp_)				:	FileDir{FileDir_},	cp{cp_} 			{ 	Read(); }
	SingleDataReader(TString FileDir_,	std::vector<TString> ColumnNames_)	:	FileDir{FileDir_},	cp{ColumnNames_} 	{	Read(); }

	void CheckMap()						const	{ if ( Map.size() == 0 ) throw std::runtime_error("SingleDataReader --> Empty Map!"); }

	void Read() override
	{
		Init_Map();

		switch (ReadStrategy)
		{
		case ReadColumnFileStrategy::ReadViaIO:
			Map = ReadViaIO(FileDir,	cp.ColumnNames,	hasHeader);
			break;
		case ReadColumnFileStrategy::ReadViaTree:
			Map = ReadViaTree(FileDir,	cp);
			break;
		default:
			Map = ReadViaIO(FileDir,	cp.ColumnNames,	hasHeader);
			break;
		}
	}

	static std::map<TString, std::vector<double>> ReadViaTree( TString FileDir_,	ColumnPrototype cp_ )
	{
		std::map<TString, std::vector<double>> outMap;

		TTree *tree = new TTree();
		tree->ReadFile(	FileDir_, cp_.getHeader().Data() );
		int N = tree->GetEntries();
		std::vector<float> temp(cp_.size());
		for (int i = 0; i < cp_.size(); ++i){ tree->SetBranchAddress(cp_.ColumnNames[i],	&temp[i]); }
		for (int i = 0; i < N; ++i)
		{
			tree->GetEntry(i);
			for (int j = 0; j < cp_.size(); ++j)
			{
				outMap[cp_.ColumnNames[j]].push_back(double(temp[j]));
				cout<<temp[j]<<endl;
			}
		}

		return outMap;
	}
	
	static std::map<TString, std::vector<double>> ReadViaIO(TString FileDir_,	std::vector<TString> ColumnNames_,	bool skipFirstLine = false)
	{
		std::map<TString, std::vector<double>> outMap;
		ifstream inFile(	FileDir_.Data()	);

		if (inFile.is_open())
		{
			std::string line;

			if (skipFirstLine)	std::getline(inFile, line);

			// Read one line at a time into the variable line:
			while(std::getline(inFile, line))
			{
				//skip empty lines
				if(line.empty()) 	continue;
				
				std::vector<double>   	lineData;
				std::stringstream  		lineStream(line);


				double value;
				// Read an integer at a time from the line
				while(lineStream >> value)
				{
					// Add the integers from a line to a 1D array (vector)
					lineData.push_back(value);
				}
				if( lineData.size() != ColumnNames_.size() ) throw std::runtime_error(Form("ReadViaIO: Incorrect number of column: LineSize: %d vs ColumnSize: %d",(int)lineData.size(), (int)ColumnNames_.size()));

				for (int j = 0; j < ColumnNames_.size(); ++j)
				{
					outMap[ColumnNames_[j]].push_back(lineData[j]);
				}
				// cout<<line<<endl;
			}
		}
		else throw std::runtime_error( "ReadViaIO: ERROR!!! Unable to open file: " + FileDir_);

		return outMap;
	}

	std::vector<double> GetVec(TString s) const
	{
		if (Map.size() == 0)	std::runtime_error("SingleDataReader: No MAP!?");
		return Map.at(s);
	}

	std::map< TString, std::vector<double> > GetMap()	const
	{
		CheckMap();
		return Map;
	}
	
private:
	void Init_Map(){ if(Map.size()!=0) cout << Form( "WARNING: Overwriting: %s", FileDir.Data() ) << endl; for (auto name : cp.ColumnNames) { Map[name] = {}; } }
};

struct MultiDataReader : DataReader
{
	std::vector<TString> FileDirs;
	std::vector<TString> ColumnNames;
	std::vector<SingleDataReader> SDRs;

	MultiDataReader(std::vector<TString> FileDirs_, std::vector<TString> ColumnNames_) : FileDirs{FileDirs_}, ColumnNames{ColumnNames_} { Read(); }

	void Read() override { for (auto FileDir : FileDirs) {	SDRs.push_back( {FileDir, ColumnNames} ); }	}
	std::vector<double> GetVec(int i, TString s) const { return SDRs[i].GetVec(s); }
};


// void DataReader(){
// 	//Testing
// 	// ColumnPrototype("../disentanglement/inFiles/DSigmaDy_ALICE_2019.txt");
// 	// SingleDataReader a("../disentanglement/inFiles/DSigmaDy_ALICE_2019.txt");
// 	// cout<<a.cp.getHeader().Data()<<endl;
// 	// MultiDataReader b({"../physicsFigures/inputfiles/LTA_Jpsi_weak_shadowing.dat", "../physicsFigures/inputfiles/LTA_Jpsi_strong_shadowing.dat"}, {"y", "AnAn", "0n0n", "0nXnSum", "XnXn"});
// 	// cout<<a.GetVec("y")[2]<<endl;
// 	// cout<<b.GetVec(0,"y")[7]<<endl;
// }


#endif