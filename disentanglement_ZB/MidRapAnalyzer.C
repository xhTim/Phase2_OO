#ifndef MidRapAnalyzer_H
#define MidRapAnalyzer_H

#include "ConversionAnalyzer.C"
#include "DisentangleMidRapSigmaAnalyzer.C"
#include "ShadowingRatioAnalyzer.C"

struct MidRapAnalyzer : Analyzer
{
	ConversionAnalyzer				cConversionAnalyzer;
	DisentangleMidRapSigmaAnalyzer	cDisentangleMidRapSigmaAnalyzer;
	ShadowingRatioAnalyzer			cShadowingRatioAnalyzer;

    MidRapAnalyzer(AnalysisData& data_,	TString inFileDir_,	TString subCase_) : Analyzer{data_},	cConversionAnalyzer{data_},	
																					cDisentangleMidRapSigmaAnalyzer{data_, inFileDir_,	subCase_},	cShadowingRatioAnalyzer{data_}	{};

	void PrintHandling() const
	{
		cout << endl << std::setfill('+') << std::setw(100) << Form("%s::Handle Is Handling The Business... ", typeid(this).name()) << endl;
		cout << setfill(' ');
	}

    void Handle() override
    {
        PrintHandling();
        
		this->Add(&cConversionAnalyzer);
		this->Add(&cDisentangleMidRapSigmaAnalyzer);
		this->Add(&cShadowingRatioAnalyzer);
		cShadowingRatioAnalyzer.Set_Simple_R_SysErr(true);

        Analyzer::Handle();
    }
};


#endif