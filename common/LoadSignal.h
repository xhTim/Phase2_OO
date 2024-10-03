#ifndef LOADSIGNAL_H
#define LOADSIGNAL_H

/*
	Interface for signal extraction
*/


struct LoadSignal
{
	TString infileDir;
	TFile* infile = nullptr;

	LoadSignal(TString infileDir) : infileDir{infileDir}
	{
		infile = new TFile(infileDir, "read");
		if(!infile) {throw std::runtime_error("LoadSignal ----> Can Not Find The File!!!");}
	}
	virtual ~LoadSignal() = default;

	virtual Bool_t Read() = 0;
};

#endif