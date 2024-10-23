#include "../common/headers.h"
#include "../common/function.C"
#include "../common/funUtil.h"

int    mGenMarker = 20;
int    mGenColor  = 1;
int    mGenWidth  = 2;
Double_t mGenSize   = 0.8;
int    mFont      = 42;

//------------------------------------------------------ //bremsstrahlung tail
Double_t DoubleNGaus(Double_t *x, Double_t *par);
Double_t CrystalBall(Double_t *x, Double_t *par);
Double_t CrystalBallAndGaus(Double_t *x, Double_t *par);
//TH1D *rebHist1d(TH1D *oldHisto, TString outhistname, int nBinsX, Double_t *BinX, TString NorX);

//const Double_t mFitMassLow[nDiffRapBins] = {2.3, 2.3, 2.3, 2.4, 2.4, 2.6, 2.6, 2.4, 2.4, 2.3, 2.3, 2.3};

//we are interested to calculate the particles reconstruction efficiency for given mass window and rapidity window
//note the pt window is different, it is part of acceptance, we don't need to cut on the MC, but only cut on RC
//for mass window, we can select for Jpsi or Psi
const double ptWindowLow   = 0.;
const double ptWindowHig   = mPtCut4Coh;
const double massWindowLow = mJpsiMassLow; 
const double massWindowHig = mJpsiMassHi; 
//const double massWindowLow = mPsiMassLow; 
//const double massWindowHig = mPsiMassHi; 

const int template_option  = 0; //0:OldCohJpsi; 1:NewCohJpsi (w R+1fm);
const TString outDir[2] = {"out4effAndTemp", "out4effAndTemp_NewCohJpsi"};

const TString ptname   = Form("_toCorr_onlyFor_pt_%.2f_%.2f_", ptWindowLow,   ptWindowHig   );
const TString massname = Form("_massWindow_%.2f_%.2f_",        massWindowLow, massWindowHig );

const TString TnPcases[4]	= {"", ".appliedTnP", ".appliedTnP_Low", ".appliedTnP_Hig"};
const int   RunTnPcase         = 0;	//Default 1

const double HFscaleFactor[3]	= {1,	0.97549056,	1.1137430};
const TString HFcases[4]	= {"", ".looseHF", ".tightHF", ".removeHF"};
const int   RunHFcase         = 0;	//Default 0

const int   nSpecs            = 9;
const TString specName[nSpecs]  = {"CohJpsi", "CohJpsi_0n0n", "CohJpsi_0nXn", "CohJpsi_XnXn", 
	"InCohJpsi", "CohPsi2SFeeddown", "CohPsi2S", "InCohPsi2S", "LowMassGammaGamma"};
// const TString specTitle[nSpecs] = {"Coherent J/#psi", "Coherent J/#psi (0n0n)", "Coherent J/#psi (0nXn)", "Coherent J/#psi (XnXn)", 
// 	"Incoherent J/#psi", "Coherent #psi(2S) #rightarrow J/#psi + X", "Coherent #psi(2S)", "Incoherent #psi(2S)", "#gamma#gamma#rightarrow#mu#mu"};
const TString specFileName[nSpecs]  = {"CohJpsi" + TnPcases[RunTnPcase] + HFcases[RunHFcase], "CohJpsi_0n0n" + TnPcases[RunTnPcase] + HFcases[RunHFcase], "CohJpsi_0nXn" + TnPcases[RunTnPcase] + HFcases[RunHFcase], "CohJpsi_XnXn" + TnPcases[RunTnPcase] + HFcases[RunHFcase], 
	"InCohJpsi" + TnPcases[RunTnPcase] + HFcases[RunHFcase], "CohPsi2SFeeddown" + TnPcases[RunTnPcase] + HFcases[RunHFcase], "CohPsi2S" + TnPcases[RunTnPcase] + HFcases[RunHFcase], "InCohPsi2S" + TnPcases[RunTnPcase] + HFcases[RunHFcase], "LowMassGammaGamma" + TnPcases[RunTnPcase] + HFcases[RunHFcase]};
const TString specTitle[nSpecs] = {"Coherent J/#psi" + TnPcases[RunTnPcase] + HFcases[RunHFcase], "Coherent J/#psi (0n0n)" + TnPcases[RunTnPcase] + HFcases[RunHFcase], "Coherent J/#psi (0nXn)" + TnPcases[RunTnPcase] + HFcases[RunHFcase], "Coherent J/#psi (XnXn)" + TnPcases[RunTnPcase] + HFcases[RunHFcase], 
	"Incoherent J/#psi" + TnPcases[RunTnPcase] + HFcases[RunHFcase], "Coherent #psi(2S) #rightarrow J/#psi + X" + TnPcases[RunTnPcase] + HFcases[RunHFcase], "Coherent #psi(2S)" + TnPcases[RunTnPcase] + HFcases[RunHFcase], "Incoherent #psi(2S)" + TnPcases[RunTnPcase] + HFcases[RunHFcase], "#gamma#gamma#rightarrow#mu#mu" + TnPcases[RunTnPcase] + HFcases[RunHFcase]};
const Double_t mMass[nSpecs-1]  = {3.096, 3.096, 3.096, 3.096, 3.096, 3.096, 3.686, 3.686}; //for fit to get signal shape parameters

//const int nSpecs = 2;
//TString specName[nSpecs]  = {"CohJpsi", "LowMassGammaGamma"};
//TString specTitle[nSpecs] = {"Coherent J/#psi", "#gamma#gamma#rightarrow#mu#mu"};
//const Double_t    mMass[nSpecs-1] = {3.096};

TFile*f[nSpecs];
TH3D *hMvsPtvsRap_gen[nSpecs];
TH3D *hMvsPtvsRap_woEvtSel[nSpecs];
TH3D *hMvsPtvsRap[nSpecs];
TH1D *hRap_gen[nSpecs];
TH1D *hRap_woEvtSel[nSpecs];
TH1D *hRap[nSpecs];
TH1D *hRap_gen_Symm[nSpecs];
TH1D *hRap_woEvtSel_Symm[nSpecs];
TH1D *hRap_Symm[nSpecs];

TH1D *hEffvsRap_woEvtSel[nSpecs];
TH1D *hEffvsRap[nSpecs];

TH1D *hEffvsRap_woEvtSel_Symm[nSpecs];
TH1D *hEffvsRap_Symm[nSpecs];

//Event selection effeciency
TH3D *hMvsPtvsRap_woSmear[nSpecs];
TH3D *hMvsPtvsRap_woEvtSel_woSmear[nSpecs];
TH1D *hRap_noEvtSel[nSpecs];
TH1D *hRap_aftEvtSel[nSpecs];
TH1D *hEvtSelEffvsRap[nSpecs];

TH1D *hRap_noEvtSel_Symm[nSpecs];
TH1D *hRap_aftEvtSel_Symm[nSpecs];
TH1D *hEvtSelEffvsRap_Symm[nSpecs];



TH1D *hMass[nSpecs];
TH1D *hPt[nSpecs];
TH1D *hMass_Rap[nSpecs][nDiffRapBins];
TH1D *hPt_Rap[nSpecs][nDiffRapBins];
TF1 *fTemp[nSpecs];
TF1 *fTemp_Rap[nSpecs][nDiffRapBins];

void readFiles();
void calEff();
void drawEff();
void getTemp();
void saveFiles();

void genEffAndTemp(  )
{
	gStyle->SetOptFit(1111);
	
	system(Form("mkdir -p %s", outDir[template_option].Data()));
	
	readFiles();
	
	//calEff();
	
	//drawEff();
	
	getTemp();
	
	saveFiles();

	cout << "End of program !" << endl;
}

void readFiles( )
{
	for(int is=0; is<nSpecs; is++)
	{
		if(is>3) continue;

		if(template_option==0)
		{
			f[is] = TFile::Open( Form("outFiles/ana_mc_%s.root", specFileName[is].Data()) );
		}
		else
		{
			f[is] = TFile::Open( Form("outFiles/ana_mc_%s.root", specFileName[is].Data()) );
		}

		cout<<"readin: "<<f[is]->GetName()<<endl;

		//hMvsPtvsRap_gen[is]      = (TH3D *)f[is]->Get("hRapPtMass_DiMu_gen");      //no smearing
		//hMvsPtvsRap_woEvtSel[is] = (TH3D *)f[is]->Get("hMvsPtvsRap_woEvtSel"); //with smearing, no event level cut
		hMvsPtvsRap[is]          = (TH3D *)f[is]->Get("hMvsPtvsRap");          //with smearing, with event level cut

		//for event selection efficiency
		/*
		hMvsPtvsRap_woEvtSel_woSmear[is] = (TH3D *)f[is]->Get("hMvsPtvsRap_woEvtSel_woSmear"); //no event selection but after goodMuPair selection
		hMvsPtvsRap_woSmear[is]          = (TH3D *)f[is]->Get("hMvsPtvsRap_woSmear");          //...and after event selection


		int massBinLow, massBinHi;

		massBinLow = 1;
		massBinHi  = hMvsPtvsRap_gen[is]->GetNbinsZ();

		//for event selection efficiency	
		hRap_noEvtSel[is]      = (TH1D *)hMvsPtvsRap_woEvtSel_woSmear[is] ->ProjectionX( Form("hRap_noEvtSel_%s",  specName[is].Data()), 0, -1, massBinLow, massBinHi );
		hRap_aftEvtSel[is]     = (TH1D *)hMvsPtvsRap_woSmear[is]          ->ProjectionX( Form("hRap_aftEvtSel_%s", specName[is].Data()), 0, -1, massBinLow, massBinHi );
		//--------------

		//Denominator
		hRap_gen[is]           = (TH1D *)hMvsPtvsRap_gen[is]      ->ProjectionX( Form("hRap_gen_%s",      specName[is].Data()), 0,       -1,       massBinLow, massBinHi );
		
		//Numberator, need to apply the pt cutoff as the pt acceptance factor
		const int ptBinLow    = hMvsPtvsRap[is]->GetYaxis()->FindBin( ptWindowLow + mTinyNum );
		const int ptBinHig    = hMvsPtvsRap[is]->GetYaxis()->FindBin( ptWindowHig - mTinyNum );

		hRap_woEvtSel[is]      = (TH1D*) hMvsPtvsRap_woEvtSel[is] ->ProjectionX( Form("hRap_woEvtSel_%s", specName[is].Data()), ptBinLow, ptBinHig, massBinLow, massBinHi );
		hRap[is]               = (TH1D*) hMvsPtvsRap[is]          ->ProjectionX( Form("hRap_%s",          specName[is].Data()), ptBinLow, ptBinHig, massBinLow, massBinHi );

		//Rebin rapidity bins to be same as signal rapidity bins
		hRap_gen[is]      = (TH1D*) rebHisto( hRap_gen[is],      Form("hRap_gen_ispec%d",     is), nDiffRapBins+1, mDiffRapBds, "NO");
		
		hRap[is]          = (TH1D*) rebHisto( hRap[is],          Form("hRap_ispec%d",         is), nDiffRapBins+1, mDiffRapBds, "NO");
		hRap_woEvtSel[is] = (TH1D*) rebHisto( hRap_woEvtSel[is], Form("hRap_woEvtSel_ispec%d",is), nDiffRapBins+1, mDiffRapBds, "NO");
		
		hRap_noEvtSel[is]  = (TH1D*) rebHisto( hRap_noEvtSel[is],  Form("hRap_noEvtSel_ispec%d", is), nDiffRapBins+1, mDiffRapBds, "NO");
		hRap_aftEvtSel[is] = (TH1D*) rebHisto( hRap_aftEvtSel[is], Form("hRap_aftEvtSel_ispec%d",is), nDiffRapBins+1, mDiffRapBds, "NO");

		hRap_gen_Symm[is] 	   = (TH1D *)hRap_gen[is]		->Clone( Form("hRap_gen_Symm_ispec%d",     	is) );
		hRap_Symm[is] 	  	   = (TH1D *)hRap[is]			->Clone( Form("hRap_Symm_ispec%d",     		is) );
		hRap_woEvtSel_Symm[is]  = (TH1D *)hRap_woEvtSel[is]		->Clone( Form("hRap_woEvtSel_Symm_ispec%d", is) );
		
		hRap_noEvtSel_Symm[is]  = (TH1D *)hRap_noEvtSel[is]		->Clone( Form("hRap_noEvtSel_Symm_ispec%d", is) );
		hRap_aftEvtSel_Symm[is] = (TH1D *)hRap_aftEvtSel[is]	->Clone( Form("hRap_aftEvtSel_Symm_ispec%d",is) );

		//For SymmetricRapBin, the positive rap side of the histogram is combined y 
		for (int i = nDiffRapBins/2 + 1; i < nDiffRapBins + 1; ++i)
		{
			hRap_gen_Symm[is]		->AddBinContent(i+1,	hRap_gen[is]	 ->GetBinContent(nDiffRapBins + 1 - i));

			hRap_Symm[is] 			->AddBinContent(i+1,	hRap[is]		 ->GetBinContent(nDiffRapBins + 1 - i));
			hRap_woEvtSel_Symm[is]	->AddBinContent(i+1,	hRap_woEvtSel[is] ->GetBinContent(nDiffRapBins + 1 - i));
			
			hRap_noEvtSel_Symm[is]	->AddBinContent(i+1,	hRap_noEvtSel[is] ->GetBinContent(nDiffRapBins + 1 - i));
			hRap_aftEvtSel_Symm[is]	->AddBinContent(i+1,	hRap_aftEvtSel[is]->GetBinContent(nDiffRapBins + 1 - i));
		}
		*/
	}//ispec
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------------------------------
void calEff()
{
	for(int is=0; is<nSpecs; is++)
	{
		if(is>3) continue;
		//calculate efficiencies
		//------------------------------------------------------------------------------------------------------------------------------------------
		hEffvsRap_woEvtSel[is] = (TH1D *)hRap_woEvtSel[is]->Clone( Form("hEffvsRap_woEvtSel_%s", specName[is].Data()) );
		hEffvsRap_woEvtSel[is] ->Divide( hRap_woEvtSel[is], hRap_gen[is], 1, 1, "B");
		hEffvsRap_woEvtSel[is] ->SetTitle( specTitle[is].Data() );
		hEffvsRap_woEvtSel[is] ->GetYaxis()->SetTitle("Efficiency");

		hEffvsRap[is]          = (TH1D *)hRap[is]->Clone( Form("hEffvsRap_%s", specName[is].Data()) );
		hEffvsRap[is]          ->Divide( hRap[is], hRap_gen[is], 1, 1, "B");
		hEffvsRap[is]          ->SetTitle( specTitle[is].Data() );
		hEffvsRap[is]          ->GetYaxis()->SetTitle("Efficiency");

		//when no TnP applied, these efficiency is identical to the above calculated Event selection efficiency

		hEffvsRap_woEvtSel_Symm[is] = (TH1D *)hRap_woEvtSel_Symm[is]->Clone( Form("hEffvsRap_woEvtSel_Symm_%s", specName[is].Data()) );
		hEffvsRap_woEvtSel_Symm[is] ->Divide( hRap_woEvtSel_Symm[is], hRap_gen_Symm[is], 1, 1, "B");
		hEffvsRap_woEvtSel_Symm[is] ->SetTitle( specTitle[is].Data() );
		hEffvsRap_woEvtSel_Symm[is] ->GetYaxis()->SetTitle("Efficiency");

		hEffvsRap_Symm[is]          = (TH1D *)hRap_Symm[is]->Clone( Form("hEffvsRap_Symm_%s", specName[is].Data()) );
		hEffvsRap_Symm[is]          ->Divide( hRap_Symm[is], hRap_gen_Symm[is], 1, 1, "B");
		hEffvsRap_Symm[is]          ->SetTitle( specTitle[is].Data() );
		hEffvsRap_Symm[is]          ->GetYaxis()->SetTitle("Efficiency");

		hEvtSelEffvsRap[is]         = (TH1D *)hRap_noEvtSel[is]->Clone(Form("hEvtSelEffvsRap_%s", specName[is].Data())); 
		hEvtSelEffvsRap[is]         ->Divide( hRap_aftEvtSel[is], hRap_noEvtSel[is], 1, 1, "B");
		hEvtSelEffvsRap[is]         ->SetTitle(specTitle[is].Data());
		hEvtSelEffvsRap[is]         ->GetYaxis()->SetTitle("Efficiency");

		hEvtSelEffvsRap_Symm[is]    = (TH1D *)hRap_noEvtSel_Symm[is]->Clone(Form("hEvtSelEffvsRap_Symm_%s", specName[is].Data())); 
		hEvtSelEffvsRap_Symm[is]    ->Divide( hRap_aftEvtSel_Symm[is], hRap_noEvtSel_Symm[is], 1, 1, "B");
		hEvtSelEffvsRap_Symm[is]    ->SetTitle(specTitle[is].Data());
		hEvtSelEffvsRap_Symm[is]    ->GetYaxis()->SetTitle("Efficiency");
	}//ispec

}
//----------------------------------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------------------------------
void drawEff()
{
	TCanvas* c2 = new TCanvas("c2", "c2", 0, 0, 1200, 450);
	c2->Divide(2, 1);

	Double_t xPos = 0.15;
	Double_t yPos = 0.84;

	TLegend* leg1 = new TLegend(0.36, 0.65, 0.6, 0.86);
	leg1->SetFillStyle(0);
	leg1->SetTextSize(0.04);

	TLegend* leg2 = new TLegend(0.36, 0.72, 0.6, 0.86);
	leg2->SetFillStyle(0);
	leg2->SetTextSize(0.04);

	for(int is=0; is<nSpecs; is++)
	{
		if(is>3) continue;

		c2->cd(1);
		gPad->SetLogy(0);

		setHisto(hRap_gen[is],      20, 1, 1, 1, 2);
		setHisto(hRap_woEvtSel[is], 24, 1, 2, 2, 2);
		setHisto(hRap[is],          25, 1, 4, 4, 2);

		hRap_gen[is]      ->GetYaxis()->SetTitle("Entries");
		hRap_gen[is]      ->GetXaxis()->CenterTitle();
		hRap_gen[is]      ->SetMinimum(1);
		hRap_gen[is]      ->Draw("p");
		hRap_woEvtSel[is] ->Draw("psame");
		hRap[is]          ->Draw("psame");

		if(is==0)
		{
			leg1->AddEntry( hRap_gen[is],      "GEN #times Acc.",                                                                  "pl");
			leg1->AddEntry( hRap_woEvtSel[is], "GEN #times Acc. #times #varepsilon_{reco}#times#varepsilon_{trig}",                           "pl");
			leg1->AddEntry( hRap[is],          "GEN #times Acc. #times #varepsilon_{reco}#times#varepsilon_{trig}#times#varepsilon_{evtSel}", "pl");
		}
		leg1->Draw("same");

		if(     specName[is].EqualTo("CohPsi2SFeeddown"))  drawLatex(0.27, 0.95, Form("%s", specTitle[is].Data()), mFont, 0.06, 1);
		else if(specName[is].EqualTo("LowMassGammaGamma")) drawLatex(0.43, 0.95, Form("%s", specTitle[is].Data()), mFont, 0.06, 1);
		else                                               drawLatex(0.38, 0.95, Form("%s", specTitle[is].Data()), mFont, 0.06, 1);

		c2->cd(2);
		gPad->SetLogy(0);
		setHisto(hEffvsRap_woEvtSel[is], 24, 1, 2, 2, 2);
		setHisto(hEffvsRap[is],          25, 1, 4, 4, 2);

		hEffvsRap_woEvtSel[is] ->SetMaximum(1);
		hEffvsRap_woEvtSel[is] ->GetXaxis()->CenterTitle();
		hEffvsRap_woEvtSel[is] ->Draw("p");
		hEffvsRap[is]          ->Draw("psame");

		if(is==0)
		{
			leg2->AddEntry(hEffvsRap_woEvtSel[is], "#varepsilon_{reco}#times#varepsilon_{trig}",                           "pl");
			leg2->AddEntry(hEffvsRap[is],          "#varepsilon_{reco}#times#varepsilon_{trig}#times#varepsilon_{evtSel}", "pl");
		}
		leg2->Draw("same");

		if(     specName[is].EqualTo("CohPsi2SFeeddown") ) drawLatex(0.27, 0.95, Form("%s", specTitle[is].Data()), mFont, 0.06, 1);
		else if(specName[is].EqualTo("LowMassGammaGamma")) drawLatex(0.43, 0.95, Form("%s", specTitle[is].Data()), mFont, 0.06, 1);
		else                                               drawLatex(0.38, 0.95, Form("%s", specTitle[is].Data()), mFont, 0.06, 1);

		c2->SaveAs(Form("%s/EffvsRap_%s.png", outDir[template_option].Data(), specFileName[is].Data()));
		c2->SaveAs(Form("%s/EffvsRap_%s.pdf", outDir[template_option].Data(), specFileName[is].Data()));
		//----------------------------------------------------------------------------------------------------------------------------------------------------------
		//----------------------------------------------------------------------------------------------------------------------------------------------------------
	}//is

	delete c2;
	delete leg1;
	delete leg2;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------
void getTemp( )//get pt and mass shape and parameters as templates of particles within Jpsi or Psi mass windows
{
	for(int is=0; is<nSpecs; is++)
	{
		if(is>3) continue;
		for(int irap=0; irap<nDiffRapBins; irap++)
		{
			const int rapBinLow  = hMvsPtvsRap[is]->GetXaxis()->FindBin( mDiffRapLow[irap] + mTinyNum );
			const int rapBinHi   = hMvsPtvsRap[is]->GetXaxis()->FindBin( mDiffRapHi[irap]  - mTinyNum );
			const int massBinLow = hMvsPtvsRap[is]->GetZaxis()->FindBin( massWindowLow     + mTinyNum );
			const int massBinHi  = hMvsPtvsRap[is]->GetZaxis()->FindBin( massWindowHig     - mTinyNum );

			hPt_Rap[is][irap] = (TH1D *)hMvsPtvsRap[is]->ProjectionY( Form("h%sPt_RapBin%d", specName[is].Data(), irap), rapBinLow, rapBinHi, massBinLow, massBinHi);

			if(is<nSpecs-1)
				hPt_Rap[is][irap]->SetTitle(Form("%1.1f < y < %1.1f", mDiffRapLow[irap], mDiffRapHi[irap]));
			else
				hPt_Rap[is][irap]->SetTitle(Form("%1.1f < y < %1.1f, %1.2f < mass < %1.2f", mDiffRapLow[irap], mDiffRapHi[irap], massWindowLow, massWindowHig));

			hMass_Rap[is][irap] = (TH1D *)hMvsPtvsRap[is]->ProjectionZ(Form("h%sMass_RapBin%d", specName[is].Data(), irap), rapBinLow, rapBinHi, 0, -1);
			hMass_Rap[is][irap] ->SetTitle(Form("%1.1f < y < %1.1f", mDiffRapLow[irap], mDiffRapHi[irap]));

			if(irap==0)
			{
				hPt[is] = (TH1D *)hPt_Rap[is][irap]->Clone(Form("h%sPt", specName[is].Data()));

				if(is<nSpecs-1)
					hPt[is]->SetTitle( Form("%1.1f < |y| < %1.1f", mDiffRapLow[nDiffRapBins/2], mDiffRapHi[nDiffRapBins-1]));
				else
					hPt[is]->SetTitle( Form("%1.1f < |y| < %1.1f, %1.2f < mass < %1.2f", mDiffRapLow[nDiffRapBins/2], mDiffRapHi[nDiffRapBins-1], massWindowLow, massWindowHig));

				hMass[is] = (TH1D *)hMass_Rap[is][irap]->Clone(Form("h%sMass", specName[is].Data()));
				hMass[is] ->SetTitle(Form("%1.1f < |y| < %1.1f",  mDiffRapLow[nDiffRapBins/2], mDiffRapHi[nDiffRapBins-1]));
			}
			else
			{
				hPt[is]   ->Add( hPt_Rap[is][irap]   );
				hMass[is] ->Add( hMass_Rap[is][irap] );
			}

		}//irapadity
	}//is

	//----------------------------------------------------------------------------------------------------------------------------------------------------------
	//draw and fit for templates parameters
	//----------------------------------------------------------------------------------------------------------------------------------------------------------

	TCanvas* c2 = new TCanvas("c2", "c2", 0, 0, 1200, 450);
	c2->Divide(2, 1);

	TCanvas* c3 = new TCanvas("c3", "c3", 0, 0, 800, 600);
	c3->Divide(2, 2);

	Double_t xPos = 0.15;
	Double_t yPos = 0.84;

	TLegend* leg1 = new TLegend(0.36, 0.65, 0.6, 0.86);
	leg1->SetFillStyle(0);
	leg1->SetTextSize(0.06);

	TLegend* leg2 = new TLegend(0.36, 0.72, 0.6, 0.86);
	leg1->SetFillStyle(0);
	leg1->SetTextSize(0.06);

	for(int is=0; is<nSpecs; is++)
	{
		if(is>3) continue;
		//----------------------------------------------------------------------------------------------------------------------------------------------------------
		// working for templates
		//----------------------------------------------------------------------------------------------------------------------------------------------------------
		TString tempDir = Form("%s/%sTemp", outDir[template_option].Data(), specFileName[is].Data());
		system(Form("mkdir -p %s", tempDir.Data()));

		if(is<nSpecs-1)
		{
			//fTemp[is] = new TF1(Form("f%sTemp",specName[is].Data()), "([0]*TMath::Gaus(x,[1],[2],1) + [3]*TMath::Gaus(x,[1],[2]*[4],1))*[5]", 0, 5);
			//fTemp[is]->SetParNames("N1","#mu","#sigma1","N2","#sigma2/#sigma1","binWidth");
			//fTemp[is]->SetParameters(0.8, mMass[is], 0.04, 0.2, 1.5, hMass[is]->GetBinWidth(1));
			//fTemp[is]->FixParameter(5, hMass[is]->GetBinWidth(1));

			//fTemp[is] = new TF1(Form("f%sTemp",specName[is].Data()), "[0]*ROOT::Math::crystalball_function(x,[1],[2],[3],[4])", 0, 5);
			//fTemp[is]->SetParNames("N","#alpha","n","#sigma","#mu");
			//fTemp[is]->SetParameters(0.1, 2, 5, 0.05, mMass[is]);

			fTemp[is] = new TF1(Form("f%sTemp", specName[is].Data()), "[0]*(ROOT::Math::crystalball_function(x,[1],[2],[3]*[6],[4]) + [5]*TMath::Gaus(x, [4], [6], 0))", 0, 5);
			fTemp[is]->SetParNames("N","#alpha","n","#sigma_{cb}/#sigma_{gaus}","#mu","N_{gaus}","#sigma_{gaus}");
			fTemp[is]->SetParameters(0.02, 3, 6, 1.5, mMass[is], 4, 0.04);
			//fTemp[is]->FixParameter(5, 1);
		}
		else
		{
			// try to parameterize gg->mumu mass shape
			fTemp[is] = new TF1(Form("f%sTemp", specName[is].Data()), "[0]*(x-[1])/(TMath::Exp([2]*(x-[1])+[3]/(x-[1]))+[4])", 0, 5);
			fTemp[is]->SetParameters(0.1, 2.5, 1, -1, 1);
		}

		fTemp[is]->SetTitle(Form("%1.1f < |y| < %1.1f", mDiffRapLow[nDiffRapBins/2], mDiffRapHi[nDiffRapBins-1]));
		fTemp[is]->SetNpx(1000);

		c2->cd(1);
		setHisto(hMass[is], 20, 0.6, 1, 1, 2);
		hMass[is]->Scale(1./hMass[is]->GetEntries());

		if(specName[is].Contains("Jpsi"))
		{
			hMass[is]->GetXaxis()->SetRangeUser(2.5, 4);
		}
		else if(specName[is].Contains("Psi2S"))
		{
			if(specName[is].Contains("Feeddown")) hMass[is]->GetXaxis()->SetRangeUser(2.5, 4);
			else                                  hMass[is]->GetXaxis()->SetRangeUser(3, 4.5);
		}

		hMass[is]->GetXaxis()->SetTitleOffset(0.9);
		hMass[is]->GetYaxis()->SetTitle("a.u.");
		if(is<nSpecs-1)
		{
			gPad->SetLogy(1);
			hMass[is]->Fit(fTemp[is], "RQ0", "", mMass[is]-0.4, mMass[is]+0.4);
			hMass[is]->Draw("p");
			fTemp[is]->Draw("same");
		}
		else
		{
			gPad->SetLogy(0);
			//hMass[is]->Fit(fTemp[is], "RQ0", "", 2.3, 5);
			hMass[is]->Draw("p");
			//fTemp[is]->Draw("same");
		}
		drawLatex(0.39, 0.95, Form("%1.1f < |y| < %1.1f", mDiffRapLow[nDiffRapBins/2], mDiffRapHi[nDiffRapBins-1]), mFont, 0.06, 1);

		c2->cd(2);
		gPad->SetLogy(1);
		hPt[is]->Scale(1./hPt[is]->GetEntries());
		setHisto(hPt[is], 20, 0.6, 1, 1, 2);
		if(specName[is].Contains("InCoh"))
		{
			hPt[is]->GetXaxis()->SetRangeUser(0, 2);
		}
		else if(specName[is].Contains("Feeddown"))
		{
			hPt[is]->GetXaxis()->SetRangeUser(0, 1);
		}
		else
		{
			hPt[is]->GetXaxis()->SetRangeUser(0, 0.5);
		}
		hPt[is]->GetYaxis()->SetRangeUser(0.5, hPt[is]->GetMaximum()*2);
		hPt[is]->GetXaxis()->SetTitleOffset(0.9);
		hPt[is]->GetYaxis()->SetTitle("a.u.");
		hPt[is]->Draw("p");
		drawLatex(0.39, 0.95, Form("%1.1f < |y| < %1.1f", mDiffRapLow[nDiffRapBins/2], mDiffRapHi[nDiffRapBins-1]), mFont, 0.06, 1);

		//c2->SaveAs(Form("%s/%sTemp"+massname+".pdf", tempDir.Data(), specName[is].Data()));
		c2->SaveAs(Form("%s/%sTemp"+massname+".png", tempDir.Data(), specFileName[is].Data()));

		for(int irap=0; irap<nDiffRapBins; irap++)
		{
			if(is<nSpecs-1)
			{
				//fTemp_Rap[is][irap] = new TF1(Form("f%sTemp_RapBin%d",specName[is].Data(),irap), "([0]*TMath::Gaus(x,[1],[2],1) + [3]*TMath::Gaus(x,[1],[2]*[4],1))*[5]", 0, 5);
				//fTemp_Rap[is][irap]->SetParNames("N1","#mu","#sigma1","N2","#sigma2/#sigma1","binWidth");
				//fTemp_Rap[is][irap]->SetParameters(0.8, mMass[is], 0.04, 0.2, 1.5, hMass_Rap[is][irap]->GetBinWidth(1));
				//fTemp_Rap[is][irap]->FixParameter(5, hMass_Rap[is][irap]->GetBinWidth(1));

				//fTemp_Rap[is][irap] = new TF1(Form("f%sTemp_RapBin%d",specName[is].Data(),irap), "[0]*ROOT::Math::crystalball_function(x,[1],[2],[3],[4])", 0, 5);
				//fTemp_Rap[is][irap]->SetTitle(Form("%1.1f < y < %1.1f", mDiffRapLow[irap], mDiffRapHi[irap]));
				//fTemp_Rap[is][irap]->SetParNames("N","#alpha","n","#sigma","#mu");
				//fTemp_Rap[is][irap]->SetParameters(0.1, 2, 5, 0.05, mMass[is]);
				//fTemp_Rap[is][irap]->FixParameter(2, fTemp[is]->GetParameter(2));

				fTemp_Rap[is][irap] = new TF1(Form("f%sTemp_RapBin%d", specName[is].Data(),irap), "[0]*(ROOT::Math::crystalball_function(x,[1],[2],[3]*[6],[4]) + [5]*TMath::Gaus(x, [4], [6], 0))", 0, 5);
				fTemp_Rap[is][irap]->SetTitle(Form("%1.1f < y < %1.1f", mDiffRapLow[irap], mDiffRapHi[irap]));
				fTemp_Rap[is][irap]->SetParNames("N","#alpha","n","#sigma_{cb}/#sigma_{gaus}","#mu","N_{gaus}","#sigma_{Gaus}");
				fTemp_Rap[is][irap]->SetParameters(0.02, 3, 6, 1.5, mMass[is], 4, 0.04);
				fTemp_Rap[is][irap]->FixParameter(1, fTemp[is]->GetParameter(1));
				fTemp_Rap[is][irap]->FixParameter(2, fTemp[is]->GetParameter(2));
				//fTemp_Rap[is][irap]->FixParameter(5, 1);
			}
			else
			{
				// try to parameterize gg->mumu mass shape
				fTemp_Rap[is][irap] = new TF1(Form("f%sTemp_RapBin%d", specName[is].Data(),irap), "pol4", 0, 5);
			}
			fTemp_Rap[is][irap]->SetTitle(Form("%1.1f < y < %1.1f", mDiffRapLow[irap], mDiffRapHi[irap]));
			fTemp_Rap[is][irap]->SetNpx(1000);

			c2->cd(1);
			gPad->SetLogy(1);
			setHisto(hMass_Rap[is][irap], 20, 0.6, 1, 1, 2);
			hMass_Rap[is][irap]->Scale(1./hMass_Rap[is][irap]->GetEntries());
			if(specName[is].Contains("Jpsi"))
			{
				hMass_Rap[is][irap]->GetXaxis()->SetRangeUser(2.5, 4);
			}
			else if(specName[is].Contains("Psi2S"))
			{
				if(specName[is].Contains("Feeddown")) hMass_Rap[is][irap]->GetXaxis()->SetRangeUser(2.5, 4.0);
				else                                 hMass_Rap[is][irap]->GetXaxis()->SetRangeUser(3.0, 4.5);
			}

			//hMass_Rap[is][irap]->GetYaxis()->SetRangeUser(0.5, hMass_Rap[is][irap]->GetMaximum()*2);
			hMass_Rap[is][irap]->GetXaxis()->SetTitleOffset(0.9);
			hMass_Rap[is][irap]->GetYaxis()->SetTitle("a.u.");
			if(is<nSpecs-1)
			{
				hMass_Rap[is][irap]->Fit(fTemp_Rap[is][irap], "RQ0", "", mMass[is]-0.4, mMass[is]+0.4);
				hMass_Rap[is][irap]->Draw("p");
				fTemp_Rap[is][irap]->Draw("same");
			}
			else
			{
				gPad->SetLogy(0);
				//hMass_Rap[is][irap]->Fit(fTemp_Rap[is][irap], "RQ0", "", mFitMassLow[irap], 5);
				hMass_Rap[is][irap]->Draw("p");
				//fTemp_Rap[is][irap]->Draw("same");
			}
			drawLatex(0.39, 0.95, Form("%1.1f < y < %1.1f", mDiffRapLow[irap], mDiffRapHi[irap]), mFont, 0.06, 1);

			c2->cd(2);
			gPad->SetLogy(1);
			hPt_Rap[is][irap]->Scale(1./hPt_Rap[is][irap]->GetEntries());
			setHisto(hPt_Rap[is][irap], 20, 0.6, 1, 1, 2);
			if(specName[is].Contains("InCoh"))
			{
				hPt_Rap[is][irap]->GetXaxis()->SetRangeUser(0, 2);
			}
			else if(specName[is].Contains("Feeddown"))
			{
				hPt_Rap[is][irap]->GetXaxis()->SetRangeUser(0, 1);
			}
			else
			{
				hPt_Rap[is][irap]->GetXaxis()->SetRangeUser(0, 0.5);
			}
			//hPt_Rap[is][irap]->GetYaxis()->SetRangeUser(0.5, hPt_Rap[is][irap]->GetMaximum()*2);
			hPt_Rap[is][irap]->GetXaxis()->SetTitleOffset(0.9);
			hPt_Rap[is][irap]->GetYaxis()->SetTitle("a.u.");
			hPt_Rap[is][irap]->Draw("p");
			drawLatex(0.39, 0.95, Form("%1.1f < |y| < %1.1f, %1.2f < mass < %1.2f", mDiffRapLow[nDiffRapBins/2], mDiffRapHi[nDiffRapBins-1], massWindowLow, massWindowHig), mFont, 0.06, 1);

			//c2->SaveAs(Form("%s/%sTemp_RapBin%d"+massname+".pdf", tempDir.Data(), specName[is].Data(), irap));
			c2->SaveAs(Form("%s/%sTemp_RapBin%d"+massname+".png", tempDir.Data(), specFileName[is].Data(), irap));
		}//iy
	}//ispec

}
//----------------------------------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------------------------------
void saveFiles( )
{
	//----------------------------------------------------------------------------------------------------------------------------------------------------------
	// write down efficiencies
	//----------------------------------------------------------------------------------------------------------------------------------------------------------

	/*
	TFile *fOut = new TFile(Form("%s/Efficiency_AllSpecs_%dRapBins%s%s.root", outDir[template_option].Data(), nDiffRapBins, TnPcases[RunTnPcase].Data(), HFcases[RunHFcase].Data()), "recreate");
	cout<<"save efficiencies into: "<<fOut->GetName()<<endl;
	fOut->cd();

	for(int is=0; is<nSpecs; is++)
	{
		if(is>3) continue;
		hEffvsRap_woEvtSel[is] ->Write();
		hEffvsRap[is]          ->Write();
		hEvtSelEffvsRap[is]    ->Write();
		hEffvsRap_woEvtSel_Symm[is] ->Write();
		hEffvsRap_Symm[is]          ->Write();
		hEvtSelEffvsRap_Symm[is]    ->Write();
		hRap[is]->Write();
		hRap_gen[is]->Write();
	}

	fOut->Close();
	*/

	//----------------------------------------------------------------------------------------------------------------------------------------------------------
	// write down templates
	//----------------------------------------------------------------------------------------------------------------------------------------------------------
	TFile *fOutTemp = new TFile( Form("%s/MassPtTemp_AllSpecs"+massname+"%dRapBins%s%s.root", outDir[template_option].Data(), nDiffRapBins, TnPcases[RunTnPcase].Data(), HFcases[RunHFcase].Data()), "recreate");
	cout<<"save templates into: "<<fOutTemp->GetName()<<endl;
	fOutTemp->cd();

	for(int is=0; is<nSpecs; is++)
	{
		if(is>3) continue;
		hMass[is]->Write();

		if(is<nSpecs-1) fTemp[is]->Write();

		for(int irap=0; irap<nDiffRapBins; irap++)
		{
			hMass_Rap[is][irap] ->Write();

			if(is<nSpecs-1) fTemp_Rap[is][irap]->Write();
		}
	}

	for(int is=0; is<nSpecs; is++)
	{
		if(is>3) continue;
		hPt[is] ->GetXaxis()->UnZoom();
		hPt[is] ->Write();

		for(int irap=0; irap<nDiffRapBins; irap++)
		{
			hPt_Rap[is][irap] ->GetXaxis()->UnZoom();
			hPt_Rap[is][irap] ->Write();
		}
	}

	fOutTemp->Close();

	cout << "End of program !" << endl;

}

//------------------------------------------------------ //bremsstrahlung tail
Double_t DoubleNGaus(Double_t *x, Double_t *par)
{
	Double_t N1       = par[0];
	Double_t mu       = par[1];
	Double_t s1       = par[2];
	Double_t N2       = par[3];
	Double_t s2       = par[4]*s1;
	Double_t binWidth = par[5];

	Double_t norm1    = (x[0]-mu)/s1;
	Double_t norm2    = (x[0]-mu)/s2;

	return ( N1/TMath::Sqrt(2*PI)/s1*TMath::Exp(-0.5*norm1*norm1) + N2/TMath::Sqrt(2*PI)/s2*TMath::Exp(-0.5*norm2*norm2) ) * binWidth;
}
//------------------------------------------------------ //bremsstrahlung tail
Double_t CrystalBall(Double_t *x, Double_t *par)
{
	Double_t N     = par[0];
	Double_t mu    = par[1];
	Double_t s     = par[2];
	Double_t alpha = par[3];
	Double_t n     = par[4];

	Double_t A     = TMath::Power(n/fabs(alpha), n) * TMath::Exp(-alpha*alpha/2.);
	Double_t B     = n/fabs(alpha) - fabs(alpha);
	Double_t norm  = (x[0]-mu)/s;

	if(norm > -alpha)
	{
		return N * TMath::Exp(-0.5*norm*norm);
	}
	else
	{
		return N * A * TMath::Power(B-norm, -n);
	}
}
//------------------------------------------------------ //bremsstrahlung tail
Double_t CrystalBallAndGaus(Double_t *x, Double_t *par)
{
	Double_t N     = par[0];
	Double_t mu    = par[1];
	Double_t s     = par[2];
	Double_t alpha = par[3];
	Double_t n     = par[4];

	Double_t A     = TMath::Power(n/fabs(alpha), n) * TMath::Exp(-alpha*alpha/2.);
	Double_t B     = n/fabs(alpha) - fabs(alpha);
	Double_t norm  = (x[0]-mu)/s;

	Double_t gausNorm = (x[0]-mu)/par[6];
	Double_t gausCom  = par[5] * TMath::Exp(-0.5*gausNorm*gausNorm);

	if(norm > -alpha)
	{
		return N * TMath::Exp(-0.5*norm*norm) + gausCom;
	}
	else
	{
		return N * A * TMath::Power(B-norm, -n) + gausCom;
	}
}
