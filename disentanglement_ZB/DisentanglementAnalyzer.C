/*
Main analysis chain for disentangling cross sections
Use Analyzer for connecting each analysis step 
Use AnalysisData for convenient manipulation of the data 
Use AnalysisDataObserver to notify each data change

Example Usage:
-----------------------------------------
AnalysisData Data_CMS("CMS");
AnalysisDataObserver obs;
Data_CMS.Subscribe(&obs);

Data_CMS.LoadHistFile(".root");

DisentanglementAnalyzer 	AnaRoot_CMS     (Data_CMS);
-----------------------------------------

Design pattern used: Chain of Responsibility, Observer

Dec. 2022
JiaZhao Lin
*/

#ifndef DisentanglementAnalyzer_H
#define DisentanglementAnalyzer_H

#include "ConversionAnalyzer.C"
#include "Fit2DAnalyzer.C"
#include "ShadowingRatioAnalyzer.C"


// This is the analyzer that handles the disentanglement analysis
struct DisentanglementAnalyzer : Analyzer
{
	friend struct PairUncerAnalyzer;
	friend struct CombineUncerAnalyzer;

	ConversionAnalyzer		cConversionAnalyzer;
	Fit2DAnalyzer			cFit2DAnalyzer;
	ShadowingRatioAnalyzer	cShadowingRatioAnalyzer;

    DisentanglementAnalyzer(AnalysisData& data_,	TString inFileDir_ = "../anaSimu/flux/",	TString subCase_ = "_SigNN68p3R6p67a0p56") :
							Analyzer{data_},	cConversionAnalyzer{data_},	cFit2DAnalyzer{data_, inFileDir_,	subCase_},	cShadowingRatioAnalyzer{data_}	{};

	void PrintHandling() const
	{
		cout << endl << std::setfill('+') << std::setw(100) << Form("%s::Handle Is Handling The Business... ", typeid(this).name()) << endl;
		cout << setfill(' ');
	}

	void SetSaveCovMatrix(const bool save)	{cFit2DAnalyzer.saveCovMatrix = save;}

    void Handle() override
    {
        PrintHandling();
        
		this->Add(&cConversionAnalyzer);
		this->Add(&cFit2DAnalyzer);
		this->Add(&cShadowingRatioAnalyzer);

        Analyzer::Handle();
    }
};


#endif