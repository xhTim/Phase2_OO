/*
Turning the one you have into the one you want
scenarios: theory data input comes in with variaty of format. we need to turn it into a 
the analysis data class.
*/

#include "ShadowingRatioAnalyzer.C"


struct TheoryAdapter
{
	AnalysisData data;
	TString name;
	TString inFileDir;

	TheoryAdapter(TString name_,	TString inFileDir_) : data{name_},	name{name_},	inFileDir{inFileDir_} {}
	virtual ~TheoryAdapter() = default;
	virtual void Process() = 0;
};

struct DSigmaDy_LTA_SS : TheoryAdapter
{
	DSigmaDy_LTA_SS(TString name_ = "LTA_SS",	TString inFileDir_ = "./inFiles/LTA_Jpsi_strong_shadowing.dat") : TheoryAdapter{name_, inFileDir_} {	Process();	}

	virtual void Process()	override	{	data.LoadTextFile(inFileDir,	{"Dy", "DSigmaDy_AnAn", "DSigmaDy_0n0n", "DSigmaDy_0nXnSum", "DSigmaDy_XnXn"}	);	}
};

struct Sigma_LTA_SS : TheoryAdapter
{
	Sigma_LTA_SS(TString name_ = "LTA_SS",	TString inFileDir_ = "./inFiles/LTA_Jpsi_strong_shadowing.dat") : TheoryAdapter{name_, inFileDir_} {	Process();	}

	virtual void Process()	override	{	data.LoadTextFile(inFileDir,	{"Dy", "Sigma_AnAn", "Sigma_0n0n", "Sigma_0nXnSum", "Sigma_XnXn"}	);	}
};

struct DSigmaDy_LTA_WS : TheoryAdapter
{
	DSigmaDy_LTA_WS(TString name_ = "LTA_WS",	TString inFileDir_ = "./inFiles/LTA_Jpsi_weak_shadowing.dat") : TheoryAdapter{name_, inFileDir_} {	Process();	}

	virtual void Process()	override	{	data.LoadTextFile(inFileDir,	{"Dy", "DSigmaDy_AnAn", "DSigmaDy_0n0n", "DSigmaDy_0nXnSum", "DSigmaDy_XnXn"}	);	}
}; 

struct DSigmaDy_CD_BGK : TheoryAdapter
{
	DSigmaDy_CD_BGK(TString name_ = "CD_BGK",	TString inFileDir_ = "./inFiles/CD_BGK_Jpsi_Xsec.dat") : TheoryAdapter{name_, inFileDir_} {	Process();	}

	virtual void Process() override		{	data.LoadTextFile(inFileDir,	{"Dy", "DSigmaDy_AnAn"}	);	}
};

struct DSigmaDy_CD_GBW : TheoryAdapter
{
	DSigmaDy_CD_GBW(TString name_ = "CD_GBW",	TString inFileDir_ = "./inFiles/CD_GBW_Jpsi_Xsec.dat") : TheoryAdapter{name_, inFileDir_} {	Process();	}

	virtual void Process() override		{	data.LoadTextFile(inFileDir,	{"Dy", "DSigmaDy_AnAn"}	);	}
};

struct DSigmaDy_CD_IIM : TheoryAdapter
{
	DSigmaDy_CD_IIM(TString name_ = "CD_IIM",	TString inFileDir_ = "./inFiles/CD_IIM_Jpsi_Xsec.dat") : TheoryAdapter{name_, inFileDir_} {	Process();	}

	virtual void Process() override		{	data.LoadTextFile(inFileDir,	{"Dy", "DSigmaDy_AnAn"}	);	}
};

// void TheoryAdapter()
// {
// 	DSigmaDy_LTA_SS adapter;
// 	adapter.Process();
// }