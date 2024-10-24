#ifndef FwdRapAnalyzer_H
#define FwdRapAnalyzer_H

#include "ConversionAnalyzer.C"
#include "DisentangleFwdRapSigmaAnalyzer.C"
#include "ShadowingRatioAnalyzer.C"

struct FwdRapAnalyzer : Analyzer
{
	ConversionAnalyzer				cConversionAnalyzer;
	DisentangleFwdRapSigmaAnalyzer	cDisentangleFwdRapSigmaAnalyzer;
	ShadowingRatioAnalyzer			cShadowingRatioAnalyzer;

    FwdRapAnalyzer(AnalysisData& data_,	TString inFileDir_,	TString subCase_) : Analyzer{data_},	cConversionAnalyzer{data_},	
																					cDisentangleFwdRapSigmaAnalyzer{data_, inFileDir_,	subCase_},	cShadowingRatioAnalyzer{data_}	{};

	void PrintHandling() const
	{
		cout << endl << std::setfill('+') << std::setw(100) << Form("%s::Handle Is Handling The Business... ", typeid(this).name()) << endl;
		cout << setfill(' ');
	}

    void Handle() override
    {
        PrintHandling();
        
		this->Add(&cConversionAnalyzer);
		this->Add(&cDisentangleFwdRapSigmaAnalyzer);
		this->Add(&cShadowingRatioAnalyzer);
		cShadowingRatioAnalyzer.Set_Simple_R_SysErr(true);

        Analyzer::Handle();
    }
};


#endif