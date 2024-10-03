#ifndef HISTWORKER_H
#define HISTWORKER_H

/*
	Worker Here
*/

static const Double_t mTinyNum = 1.e-6;

struct HistWorker
{

	static int FindBin(TH1D* Hist1D, const double num, const int high)
	{
		return (high) ? Hist1D->FindBin(num - mTinyNum) : Hist1D->FindBin(num + mTinyNum);
	}
	static int FindXBin(TH3D* Hist3D, const double num, const int high)
	{
		return (high) ? Hist3D->GetXaxis()->FindBin(num - mTinyNum) : Hist3D->GetXaxis()->FindBin(num + mTinyNum);
	}
	static int FindYBin(TH3D* Hist3D, const double num, const int high)
	{
		return (high) ? Hist3D->GetYaxis()->FindBin(num - mTinyNum) : Hist3D->GetYaxis()->FindBin(num + mTinyNum);
	}
	static int FindZBin(TH3D* Hist3D, const double num, const int high)
	{
		return (high) ? Hist3D->GetZaxis()->FindBin(num - mTinyNum) : Hist3D->GetZaxis()->FindBin(num + mTinyNum);
	}

};

#endif