/*
Classes to read histogram from root file
Contains pointers for accessing the histograms

Dec. 2022
JiaZhao Lin
*/


#ifndef LOADSIGNAL_C
#define LOADSIGNAL_C

#include "LoadSignal.h"
#include "constants.h"

struct LoadMvsPtvsRap_NeuDir : public LoadSignal
{
	//Priviate but not so priviate members-----------------------------------------------
	TH3D * hMvsPtvsRap;	//For AnAn
	TH3D * hMvsPtvsRap_NeuDir[nNeus][nNeus];
	//-----------------------------------------------------------------------------------

	//Constructor------------------------------------------------------------------------
	LoadMvsPtvsRap_NeuDir(TString infileDir) : LoadSignal{infileDir} {	Read();	}
	virtual ~LoadMvsPtvsRap_NeuDir() = default;
	//-----------------------------------------------------------------------------------

	//Virtual Functions------------------------------------------------------------------
	virtual Bool_t Read() override
	{	

		if( nNeus !=2 ) 
		{
			throw std::runtime_error("LoadMvsPtvsRap_NeuDir ----> Not running the correct 0nXn!!!");
			return kFALSE;
		}

		cout<<"------>START Reading LoadMvsPtvsRap_NeuDir: "<<infile->GetName()<<endl;

		//Getting AnAn
		hMvsPtvsRap = (TH3D*) infile->Get("hMvsPtvsRap");
		//Getting 0n0n, 0nXn, Xn0n, and XnXn
		for (int ip = 0; ip < nNeus; ip++) 
		{
			for (int im = 0; im < nNeus; im++) 
			{
				hMvsPtvsRap_NeuDir[ip][im]     = (TH3D*)infile->Get( Form("hMvsPtvsRap_NeuDir%dp%dm",     ip, im) );
				
				cout<<"Reading: "<<hMvsPtvsRap_NeuDir[ip][im]->GetName()     <<endl;
				cout<<"Entries: "<<hMvsPtvsRap_NeuDir[ip][im]->GetEntries() <<endl;
			}
		}

		cout<<"------>DONE  Reading LoadMvsPtvsRap_NeuDir: "<<infile->GetName()<<endl;
		return kTRUE;
	}
	//-----------------------------------------------------------------------------------

	//Free Functions---------------------------------------------------------------------
	TH3D * GetHist(const int idx_1, const int idx_2) const {	return hMvsPtvsRap_NeuDir[idx_1][idx_2];	}
	TH3D * GetHist() 								 const {	return hMvsPtvsRap;	}
};


struct LoadEfficiency : public LoadSignal
{
	//Priviate but not so priviate members-----------------------------------------------
	TH1D* H_EffVsY_CohJpsi;
	TH1D* H_EffVsY_CohPsi;
	TH1D* H_EffVsY_CohPsi2Jpsi;
	const Bool_t SymmetricRapBin;
	//-----------------------------------------------------------------------------------

	//Constructor------------------------------------------------------------------------
	LoadEfficiency(TString infileDir, const Bool_t SymmetricRapBin) : LoadSignal{infileDir}, 
																	SymmetricRapBin{SymmetricRapBin} {	Read();	}
	virtual ~LoadEfficiency() = default;
	//-----------------------------------------------------------------------------------

	//Virtual Functions------------------------------------------------------------------
	virtual Bool_t Read() override
	{	
		cout<<"------>START Reading LoadEfficiency: "<<infile->GetName()<<endl;

		if(SymmetricRapBin){
			H_EffVsY_CohJpsi     = (TH1D*) infile->Get("hEffvsRap_Symm_CohJpsi");        //hEffvsRap_CohJpsi_0n0n, 0nXn, XnXn
			H_EffVsY_CohPsi      = (TH1D*) infile->Get("hEffvsRap_Symm_CohPsi2S");
			H_EffVsY_CohPsi2Jpsi = (TH1D*) infile->Get("hEffvsRap_Symm_CohPsi2SFeeddown");
		}
		else
		{
			H_EffVsY_CohJpsi     = (TH1D*) infile->Get("hEffvsRap_CohJpsi");        //hEffvsRap_CohJpsi_0n0n, 0nXn, XnXn
			H_EffVsY_CohPsi      = (TH1D*) infile->Get("hEffvsRap_CohPsi2S");
			H_EffVsY_CohPsi2Jpsi = (TH1D*) infile->Get("hEffvsRap_CohPsi2SFeeddown");
		}

		cout<<"------>DONE  Reading LoadEfficiency: "<<infile->GetName()<<endl;
		return kTRUE;
	}
	//-----------------------------------------------------------------------------------

	//Free Functions---------------------------------------------------------------------
	TH1D* GetCohJpsi()		{if(!H_EffVsY_CohJpsi) 		std::runtime_error("LoadEfficiency: Empty!!!"); return H_EffVsY_CohJpsi;}
	TH1D* GetCohPsi()		{if(!H_EffVsY_CohPsi) 		std::runtime_error("LoadEfficiency: Empty!!!"); return H_EffVsY_CohPsi; }
	TH1D* GetCohPsi2Jpsi()	{if(!H_EffVsY_CohPsi2Jpsi) 	std::runtime_error("LoadEfficiency: Empty!!!"); return H_EffVsY_CohPsi2Jpsi;}
};


struct LoadAcceptance : public LoadSignal
{
	//Priviate but not so priviate members-----------------------------------------------
	TH1D* H_AccVsY_CohJpsi;

	const Bool_t SymmetricRapBin;
	//-----------------------------------------------------------------------------------

	//Constructor------------------------------------------------------------------------
	LoadAcceptance(TString infileDir, const Bool_t SymmetricRapBin) : LoadSignal{infileDir}, 
																	SymmetricRapBin{SymmetricRapBin} {	Read();	}
	virtual ~LoadAcceptance() = default;
	//-----------------------------------------------------------------------------------

	//Virtual Functions------------------------------------------------------------------
	virtual Bool_t Read() override
	{	
		cout<<"------>START Reading LoadAcceptance: "<<infile->GetName()<<endl;

		if(SymmetricRapBin)		H_AccVsY_CohJpsi     = (TH1D*) infile->Get("hAccvsRap_Symm_CohJpsi");
		else 					H_AccVsY_CohJpsi     = (TH1D*) infile->Get("hAccvsRap_CohJpsi");

		cout<<"------>DONE  Reading LoadAcceptance: "<<infile->GetName()<<endl;
		return kTRUE;
	}
	//-----------------------------------------------------------------------------------

	//Free Functions---------------------------------------------------------------------
	TH1D* GetAcceptance()	{if(!H_AccVsY_CohJpsi)	std::runtime_error("LoadAcceptance: Empty!!!"); return H_AccVsY_CohJpsi;}
};


struct LoadDSigmaDy : public LoadSignal
{
	//Priviate but not so priviate members-----------------------------------------------
	TH1D* V_JpsiDSigmaDy[6];
	std::map<TString, std::vector<double>> Map;
	//-----------------------------------------------------------------------------------

	//Constructor------------------------------------------------------------------------
	LoadDSigmaDy(TString infileDir) : LoadSignal{infileDir}	{	Read();	}
	virtual ~LoadDSigmaDy() = default;
	//-----------------------------------------------------------------------------------

	//Virtual Functions------------------------------------------------------------------
	virtual Bool_t Read() override
	{	
		cout<<"------>START Loading DSigmaDy: "<<infile->GetName()<<endl;

		V_JpsiDSigmaDy[0] = (TH1D*) infile->Get("hAnAn") ;
		V_JpsiDSigmaDy[1] = (TH1D*) infile->Get("h0n0n") ;
		V_JpsiDSigmaDy[2] = (TH1D*) infile->Get("h0nXn") ;
		V_JpsiDSigmaDy[3] = (TH1D*) infile->Get("hXn0n") ;
		V_JpsiDSigmaDy[4] = (TH1D*) infile->Get("h0nXnSum") ;
		V_JpsiDSigmaDy[5] = (TH1D*) infile->Get("hXnXn") ;

		std::vector<double> DSigmaDy_0n0n, DSigmaDy_0nXnSum, DSigmaDy_XnXn, DSigmaDy_AnAn, Rap;
		std::vector<double> DSigmaDy_0n0n_Err, DSigmaDy_0nXnSum_Err, DSigmaDy_XnXn_Err, DSigmaDy_AnAn_Err, Dy_Err;
		for (int iy = nDiffRapBins/2 + 2 ; iy < nDiffRapBins + 2 ; ++iy)
		{
			Rap.push_back(	V_JpsiDSigmaDy[5]->GetBinCenter(iy)	); 				Dy_Err.push_back(		V_JpsiDSigmaDy[5]->GetBinWidth(iy)/2	);
			DSigmaDy_AnAn.push_back(	V_JpsiDSigmaDy[0]->GetBinContent(iy)	);	DSigmaDy_AnAn_Err.push_back(		V_JpsiDSigmaDy[0]->GetBinError(iy)	);
			DSigmaDy_0n0n.push_back(	V_JpsiDSigmaDy[1]->GetBinContent(iy)	);	DSigmaDy_0n0n_Err.push_back(		V_JpsiDSigmaDy[1]->GetBinError(iy)	);
			DSigmaDy_0nXnSum.push_back(	V_JpsiDSigmaDy[4]->GetBinContent(iy)	);	DSigmaDy_0nXnSum_Err.push_back(		V_JpsiDSigmaDy[4]->GetBinError(iy)	);
			DSigmaDy_XnXn.push_back(	V_JpsiDSigmaDy[5]->GetBinContent(iy)	);	DSigmaDy_XnXn_Err.push_back(		V_JpsiDSigmaDy[5]->GetBinError(iy)	);
		}
		Map["Dy"] = Rap; 								Map["Dy_Err"] = Dy_Err;
		Map["DSigmaDy_0n0n"] = DSigmaDy_0n0n; 			Map["DSigmaDy_0n0n_Err"] = DSigmaDy_0n0n_Err;
		Map["DSigmaDy_0nXnSum"] = DSigmaDy_0nXnSum; 	Map["DSigmaDy_0nXnSum_Err"] = DSigmaDy_0nXnSum_Err;
		Map["DSigmaDy_XnXn"] = DSigmaDy_XnXn; 			Map["DSigmaDy_XnXn_Err"] = DSigmaDy_XnXn_Err;
		Map["DSigmaDy_AnAn"] = DSigmaDy_AnAn; 			Map["DSigmaDy_AnAn_Err"] = DSigmaDy_AnAn_Err;

		cout<<"------>DONE  Reading DSigmaDy: "<<infile->GetName()<<endl;
		return kTRUE;
	}
	//-----------------------------------------------------------------------------------

	//Free Functions---------------------------------------------------------------------
	TH1D* GetHist(const int i)	{if(!V_JpsiDSigmaDy[i])	std::runtime_error("LoadDSigmaDy: Empty!!!"); return V_JpsiDSigmaDy[i];}
	std::map<TString, std::vector<double>> GetMap()	const {if(!Map.size())	std::runtime_error("LoadDSigmaDy: Empty!!!"); 	return Map;}
	std::vector<double> GetMapElement(const TString n)	{if(!Map.size())	std::runtime_error("LoadDSigmaDy: Empty!!!");	return Map[n];}
	double GetMapElementVal(const TString n, const int i)	{if(!Map.size())	std::runtime_error("LoadDSigmaDy: Empty!!!");	return Map[n][i];}
};

#endif
