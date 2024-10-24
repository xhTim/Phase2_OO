/*
Core Data analysis chain abstraction
Contain a AnalysisData for convenient manipulation of the data 

Design pattern used: Chain of Resposibility

Dec. 2022
JiaZhao Lin
*/

#ifndef Analyzer_H
#define Analyzer_H

#include "AnalysisData.C"

class Analyzer
{
protected:
	Analyzer*		next{nullptr};
	AnalysisData& 	data;

public:
	Analyzer(AnalysisData& data_)	:	data(data_) {};
	virtual ~Analyzer() = default;
	
	AnalysisData GetAnaData() const { return data; }
	void Add(Analyzer* ana)
	{
		if	(next)	{	next->Add(ana);     }
		else		{	next = ana;         }
	}
	virtual void Handle()
	{
		if	(next)	{	next->Handle();		}
		else		{	cout << endl << "------> Analyzer END << ------" << endl << endl;}
	}
};

#endif