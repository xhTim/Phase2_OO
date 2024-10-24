/*
Core Data structure used in the analysis
Contain a map for convenient manipulation of the data 
Use AnalysisDataObserver to notify each data change

Example Usage:
-----------------------------------------
struct AnalysisData a("CMS");
AnalysisDataObserver obs;
a.Subscribe(&obs);
a.LoadMap(map); // a.File(".root");
a.ClearMap();
-----------------------------------------

Design pattern used: Observer

Dec. 2022
JiaZhao Lin
*/


#ifndef AnalysisData_H
#define AnalysisData_H

#include "Observer.h"
#include "../common_ZB/LoadSignal.C"
#include "../common_ZB/DataReader.C"
#include "../common_ZB/MapIO.C"

class AnalysisData: public Observable<AnalysisData>
{
private:
	std::map< TString, std::vector<double> > data_map;

public:
	const TString data_name;
	AnalysisData(TString name): data_name(name) {}
	AnalysisData(TString name, TString inFileDir): data_name{name} { LoadHistFile(inFileDir); }	// Load from DSigmaDy file !!!
	AnalysisData( const AnalysisData& other ): data_name{other.data_name},	data_map{other.data_map} {}	// Copy constructor only copies the map and the name but not the observers

	bool IsMapEmpty()					const	{ return !data_map.size(); }
	bool IsMapKeyExist(TString param)	const	{ return data_map.find(param) != data_map.end(); }
	void CheckParam(TString param)		const	{ if ( !IsMapKeyExist(param) ) throw std::runtime_error( Form( "AnalysisData --> Param %s does not Exist!", param.Data() ) ); }
	void CheckMap()						const	{ if ( data_map.size() == 0 ) throw std::runtime_error("AnalysisData --> Empty Map!"); }
	void ClearMap()								{ data_map.clear(); Notify(*this, "Cleared"); }
	TString GetDataName()				const	{ return data_name;}
	
	void LoadMap(std::map< TString, std::vector<double> > map)
	{
		//Load a map into the data
		if (!IsMapEmpty()) ClearMap();
		data_map = map;
		Notify(*this, "Loaded");
	}

	void LoadMapFile(TString inFileDir)
	{
		//Read a map from a file containing a map
		auto temp = MapIO::ReadFromRoot(inFileDir);
		LoadMap( temp );
	}

	void WriteMapFile(TString outFileDir)
	{
		//Write the map to a file
		if( IsMapEmpty() ) throw std::runtime_error("AnalysisData::WriteMapFile --> Writing a Empty Map!");
		MapIO::WriteToRoot(data_map, outFileDir);
	}

	void LoadHistFile(TString inFileDir)
	{
		//Load a map from a file containing histograms
		LoadDSigmaDy cLoadDSigmaDy(inFileDir);
		LoadMap(	cLoadDSigmaDy.GetMap()	);
	}

	void LoadTextFile(TString inFileDir)
	{
		//Load a collumnlized file into a map
		SingleDataReader dataReader( inFileDir );
		LoadMap(	dataReader.GetMap()	);
	}

	void LoadTextFile(TString inFileDir,	std::vector<TString> ColumnNames_)
	{
		//Load a collumnlized file into a map without column header
		SingleDataReader dataReader( inFileDir, ColumnNames_ );
		LoadMap(	dataReader.GetMap()	);
	}

	void Init()
	{
		if (!IsMapEmpty()) ClearMap();
		data_map = {
			{"Rap",		    {}},    {"Rap_Err", 		{}},
			{"Sigma",		{}},    {"Sigma_Err",		{}},
			{"Sigma_IA",	{}},    {"Sigma_IA_Err",	{}},
			{"R",			{}},    {"R_Err",			{}},
			{"X",			{}},    {"X_Err",			{}},
			{"W",			{}},    {"W_Err",			{}},

			{"Dy",			    {}},	{"Dy_Err",   				{}},
			{"DSigmaDy_AnAn",	{}},	{"DSigmaDy_AnAn_Err",		{}},
			{"DSigmaDy_0n0n",	{}},	{"DSigmaDy_0n0n_Err",		{}},
			{"DSigmaDy_0nXnSum",{}},	{"DSigmaDy_0nXnSum_Err",	{}},
			{"DSigmaDy_XnXn",	{}},	{"DSigmaDy_XnXn_Err",		{}}
		};
	}

	void Add(TString param, std::vector<double> values)
	{
		if ( IsMapKeyExist(param) ) throw std::runtime_error( Form( "AnalysisData --> Param %s Already Exist!", param.Data() ) );
		data_map[param] = values;
		Notify(*this, param);
	}

	void Update(TString param, std::vector<double> values)
	{
		CheckParam(param);
		data_map.at(param) = values;
		Notify(*this, param);
	}

	void Modify(TString param, int i, double value)
	{
		CheckParam(param);
		data_map[param][i] = value;
		Notify(*this, param);
	}

	void Combine(AnalysisData& second_data)
	{
		//Combine the second data into the data
		auto temp = second_data.GetMap();
		for (auto& it: temp)
		{
			Add(it.first, it.second);
		}
	}

	double Get(TString param, int i)	const
	{
		CheckParam(param);
		return data_map.at(param)[i];
	}

	std::vector<double> Get(TString param)	const
	{
		CheckParam(param);
		return data_map.at(param);
	}

	int GetSize(TString param)	const
	{
		CheckParam(param);
		return data_map.at(param).size();
	}

	std::map< TString, std::vector<double> > GetMap()	const
	{
		CheckMap();
		return data_map;
	}

	void Print()	const
	{
		Notify(*this, "Print");
	}
};


struct AnalysisDataObserver: Observer<AnalysisData>
{
	void PrintKeyValues(TString key, std::vector<double> values) const
	{
		cout << std::left << std::setw(28) << key;
		for (int i = 0; i < values.size(); i++) { cout << std::setw(15) << values[i]; }
		cout << endl;
	}

	void FieldChanged(const AnalysisData& source, TString name) const
	{
		if (name == "Loaded" || name == "Print")
		{
			auto temp_map = source.GetMap();
			
			if (name == "Loaded")	cout << Form("AnalysisDataObserver------> %s Map has been updated. Printing the Map! <------",	source.GetDataName().Data()) << endl;
			else					cout << Form("AnalysisDataObserver------> Printing the Map: %s <------", 						source.GetDataName().Data()) << endl;

			for (auto it = temp_map.begin(); it != temp_map.end(); it++)
			{
				auto key    = it->first;
				auto values  = it->second;

				PrintKeyValues(key, values);
			}
			cout << "AnalysisDataObserver----------> Print Ended! <----------" << endl << endl;
		}
		else if (name == "Cleared")
		{
			cout << Form("AnalysisDataObserver------> %s Map Cleared <------", source.GetDataName().Data()) << endl;
			cout << "------------------------------------------" << endl << endl;
		}
		else
		{
			cout << Form("AnalysisDataObserver------> %s Param updated <------", source.GetDataName().Data()) << endl;
			PrintKeyValues(name, source.Get(name));
		}
	}
};

#endif