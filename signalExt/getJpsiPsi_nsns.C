#include "../common/headers.h"
#include "../common/function.C"
#include "../common/funUtil.h"
#include "../common/LoadSignal.C"
#include "../common/PdfFactory.C"
#include "../common/HistWorker.h"

#include "RooRealVar.h"
#include "RooDataSet.h"
#include "RooDataHist.h"
#include "RooGaussian.h"
#include "RooConstVar.h"
#include "RooFormulaVar.h"
#include "RooHistPdf.h"
#include "RooGenericPdf.h"
#include "RooAddPdf.h"
#include "RooPolynomial.h"
#include "RooChi2Var.h"
#include "RooMinimizer.h"
#include "RooCategory.h"
#include "RooSimultaneous.h"
#include "RooPlot.h"
#include "RooFitResult.h"
#include "RooHist.h"
using namespace RooFit;

//------------------------------------------------------------------------------------------------------------
const Bool_t  mStorePDF = kFALSE;
const Bool_t SymmetricRapBin = kTRUE;

const double mOffSet  = 0.1;

int    mTextFont    = 42;
double mTextSize    = 0.045;
int    mTextColor   = 1;

double mMarkerStyle = 20;
double mMarkerSize  = 0.8;

double mTitleSize    = 0.06;
double mXTitleOffset = 0.95;
double mYTitleOffset = 0.95;
double mLabelSize    = 0.05;
double mTickLength   = 0.02;
int    mXNdivisions  = 210;
int    mYNdivisions  = 208;

int    mLineWidth = 2;
int    cohJpsiColor       = kBlue,      cohJpsiStyle      = 1;
int    incohJpsiColor     = kViolet-1,  incohJpsiStyle    = 2;
int    dissoJpsiColor     = kRed,       dissoJpsiStyle    = 2;
int    feeddownJpsiColor  = kAzure+10,  feeddownJpsiStyle = 1;
int    qedColor = kGreen-3, qedStyle = 1;
//------------------------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------------------------------
const int draw4Paper_flag = 1;
//------------------------------------------------------------------------------------------------------------
const int PU_option				= 0;	//Default 0
const TString PU_name[2]		=	{"_PUShuai",	"_PUATLAS"};

const int template_option  		= 0;	//Default 1 	(0:OldCohJpsi; 1:NewCohJpsi (w R+1fm));
const TString template_Name[2] 	= {"", "_NewCohJpsi"};
const TString template_Dir[2] 	= {"out4effAndTemp", "out4effAndTemp_NewCohJpsi"};

const int   RunTnPcase         	= 0;	//Default 1
const TString TnPcases[4]		= {"", ".appliedTnP", ".appliedTnP_Low", ".appliedTnP_Hig"};

const int   RunHFcase         	= 0;	//Default 0
const TString HFcases[4]		= {"", ".looseHF", ".tightHF", ".removeHF"};
// const double HFscaleFactor[3]	= {1,	0.97549056,	1.1137430};
const double HFscaleFactor[3]	= {1,	1,	1.1137430};

const int NnCases = 6;
const std::vector<int> RunCase 	= {0,1,2,3,4,5};//{0,1,2,3,4,5};

const TString nCasesName[NnCases] = {"AnAn", "0n0n", "0nXn", "Xn0n", "0nXnSum", "XnXn"};
TH1D* hCohMass_in_ny[NnCases][nDiffRapBins+1]; //last content is the sum of all y-bins, within coherent pt threshold
TH1D* hMass_in_ny[NnCases][nDiffRapBins+1];    //last content is the sum of all y-bins, for all pt 
TH1D* hPt_Jpsi_in_ny[NnCases][nDiffRapBins+1]; //last content is the sum of all y-bins
TH1D* hPt_LeftSdB_in_ny[NnCases][nDiffRapBins+1];  //last content is the sum of all y-bins
TH1D* hPt_RightSdB_in_ny[NnCases][nDiffRapBins+1]; //last content is the sum of all y-bins
TH1D* hPt_SdB_in_ny[NnCases][nDiffRapBins+1];      //last content is the sum of all y-bins
TH1D* H_EffVsY_CohJpsi;
TH1D* H_EffVsY_CohPsi;
TH1D* H_EffVsY_CohPsi2Jpsi;
TH1D* H_AccVsY_CohJpsi;


double fD_inPtCut[NnCases][nDiffRapBins+1];    		//fD within pt<0.20 GeV/c
double fDerr_inPtCut[NnCases][nDiffRapBins+1];    	//fD within pt<0.20 GeV/c
double NJpsi_inMFit[NnCases][nDiffRapBins+1]; 		//# Jpsi within pt<0.20 GeV/c from mass fitting
double NerrJpsi_inMFit[NnCases][nDiffRapBins+1]; 	//# Jpsi within pt<0.20 GeV/c from mass fitting
double Eff_CohJpsi[NnCases][nDiffRapBins+1];     	//efficiency of coherent jpsi
// const double temAcc[nDiffRapBins+1] = {0.170, 0.348, 0.348, 0.170, 0.250};//need to be updated by corrected one later
double Acc_CohJpsi[nDiffRapBins+1];     //acceptance of coherent jpsi

std::vector< std::vector<double> > xsecValue( NnCases , std::vector<double> (nDiffRapBins+1,	0));
std::vector< std::vector<double> > xsecError( NnCases , std::vector<double> (nDiffRapBins+1,	0));

//------------------------------------------------------------------------------------------------------------
void prepareData();
void loadEff();
void loadAcc();
void fitCohMass_4RNRfD( const double massLow4Fit=2.6, const double massHig4Fit=4.2);
void fitFullMassAndPt_4Decouple( const double massLow4Fit=2.6, const double massHig4Fit=4.2,  const double ptLow4Fit=0,     const double ptHig4Fit=3.5);
void calSec();
void saveFile(const TString headerTitle = "Default");
//------------------------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------------------------------
void getJpsiPsi_nsns()
{
	prepareData();
	
	loadEff();

	// loadAcc();
	
	fitCohMass_4RNRfD(2.61, 4.2);
	
	calSec();

	saveFile();
}
//------------------------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------------------------------
void prepareData()
{
	TString inFileDir 		  = Form("../anaSimu/outFiles/rawSig%s.root", HFcases[RunHFcase].Data());

	LoadMvsPtvsRap_NeuDir * hMvsPtvsRap_NeuDir = new LoadMvsPtvsRap_NeuDir(inFileDir);

	TH3D *hMvsPtvsRap_AnAn    = (TH3D*) hMvsPtvsRap_NeuDir->GetHist(); //All Sum, Inclusive
	TH3D *hMvsPtvsRap_0n0n    = (TH3D*) hMvsPtvsRap_NeuDir->GetHist(0,0)->Clone( "hMvsPtvsRap_0n0n"    );
	TH3D *hMvsPtvsRap_XnXn    = (TH3D*) hMvsPtvsRap_NeuDir->GetHist(1,1)->Clone( "hMvsPtvsRap_XnXn"    );

	TH3D *hMvsPtvsRap_0nXn    = (TH3D*) hMvsPtvsRap_NeuDir->GetHist(1,0)->Clone( "hMvsPtvsRap_0nXn"    );
	TH3D *hMvsPtvsRap_Xn0n    = (TH3D*) hMvsPtvsRap_NeuDir->GetHist(0,1)->Clone( "hMvsPtvsRap_Xn0n"    );
	
	TH3D *hMvsPtvsRap_0nXnSum = (TH3D*) hMvsPtvsRap_NeuDir->GetHist(0,1)->Clone( "hMvsPtvsRap_0nXnSum" );
	hMvsPtvsRap_0nXnSum       -> Add(hMvsPtvsRap_NeuDir->GetHist(1,0));
	
	if (RunHFcase != 0)
	{
		hMvsPtvsRap_AnAn->Scale(HFscaleFactor[RunHFcase]);
		hMvsPtvsRap_0n0n->Scale(HFscaleFactor[RunHFcase]);
		hMvsPtvsRap_XnXn->Scale(HFscaleFactor[RunHFcase]);
		hMvsPtvsRap_0nXn->Scale(HFscaleFactor[RunHFcase]);
		hMvsPtvsRap_Xn0n->Scale(HFscaleFactor[RunHFcase]);
		hMvsPtvsRap_0nXnSum->Scale(HFscaleFactor[RunHFcase]);
	}

	cout<<"hMvsPtvsRap_0nXn->GetEntries(): "<<hMvsPtvsRap_0nXn->GetEntries()<<endl;
	cout<<"hMvsPtvsRap_Xn0n->GetEntries(): "<<hMvsPtvsRap_Xn0n->GetEntries()<<endl;
	
	TH3D* hMvsPtvsRap_inWork;
	for(int i_ncase=0; i_ncase<NnCases; i_ncase++)
	{
		cout<<"i_ncase: "<<i_ncase<<endl;

		if(i_ncase==0) hMvsPtvsRap_inWork = (TH3D*) hMvsPtvsRap_AnAn    ->Clone( Form("hMvsPtvsRap_i_ncase%d",i_ncase) );
		if(i_ncase==1) hMvsPtvsRap_inWork = (TH3D*) hMvsPtvsRap_0n0n    ->Clone( Form("hMvsPtvsRap_i_ncase%d",i_ncase) );
		if(i_ncase==2) hMvsPtvsRap_inWork = (TH3D*) hMvsPtvsRap_0nXn    ->Clone( Form("hMvsPtvsRap_i_ncase%d",i_ncase) );
		if(i_ncase==3) hMvsPtvsRap_inWork = (TH3D*) hMvsPtvsRap_Xn0n    ->Clone( Form("hMvsPtvsRap_i_ncase%d",i_ncase) );
		if(i_ncase==4) hMvsPtvsRap_inWork = (TH3D*) hMvsPtvsRap_0nXnSum ->Clone( Form("hMvsPtvsRap_i_ncase%d",i_ncase) );
		if(i_ncase==5) hMvsPtvsRap_inWork = (TH3D*) hMvsPtvsRap_XnXn    ->Clone( Form("hMvsPtvsRap_i_ncase%d",i_ncase) );
		
		for(int iy=0; iy<nDiffRapBins; iy++)
		{
			cout<<"iy: "<<iy<<" "<<mDiffRapLow[iy]<<" <y< "<<mDiffRapHi[iy]<<endl;

			int rapBinLow   = HistWorker::FindXBin(hMvsPtvsRap_inWork, mDiffRapLow[iy], 0 );
			int rapBinHi    = HistWorker::FindXBin(hMvsPtvsRap_inWork, mDiffRapHi[iy] , 1 );
			
			int ptBinLow    = HistWorker::FindYBin(hMvsPtvsRap_inWork, 0.00           , 0 );
			int ptBinHi     = HistWorker::FindYBin(hMvsPtvsRap_inWork, mPtCut4Coh     , 1 ); //only look at pt<0.2 GeV/c for Coh signals, for fD
			int nptBinsMax  = hMvsPtvsRap_inWork->GetNbinsY();
			
			int mJpsiBinLow = HistWorker::FindZBin(hMvsPtvsRap_inWork, mJpsiMassLow ,   0 );
			int mJpsiBinHi  = HistWorker::FindZBin(hMvsPtvsRap_inWork, mJpsiMassHi  ,   1 );

			hCohMass_in_ny[i_ncase][iy] = (TH1D *)hMvsPtvsRap_inWork->ProjectionZ( Form("hCohMass_iNeuCase%d_iy%d", i_ncase, iy), rapBinLow, rapBinHi, ptBinLow,    ptBinHi    );
			
			hMass_in_ny[i_ncase][iy]    = (TH1D *)hMvsPtvsRap_inWork->ProjectionZ( Form("hMass_iNeuCase%d_iy%d",    i_ncase, iy), rapBinLow, rapBinHi, 1,           nptBinsMax );
			hPt_Jpsi_in_ny[i_ncase][iy] = (TH1D *)hMvsPtvsRap_inWork->ProjectionY( Form("hPt_Jpsi_iNeuCase%d_iy%d", i_ncase, iy), rapBinLow, rapBinHi, mJpsiBinLow, mJpsiBinHi );

				//---------------------------------------------------------------------------------------
				// for side band (left+right sides of Jpsi mass peak)
				//---------------------------------------------------------------------------------------
				int mLeftSdB_BinLow = HistWorker::FindZBin(hMvsPtvsRap_inWork, mLowMassBandLow, 0);
			int mLeftSdB_BinHig   = HistWorker::FindZBin(hMvsPtvsRap_inWork, mLowMassBandHi,    1 );
			int mRightSdB_BinLow  = HistWorker::FindZBin(hMvsPtvsRap_inWork, mHiMassBandLow,    0 );
			int mRightSdB_BinHig  = HistWorker::FindZBin(hMvsPtvsRap_inWork, mHiMassBandHi,     1 );
			
			hPt_LeftSdB_in_ny[i_ncase][iy]  = (TH1D*)hMvsPtvsRap_inWork->ProjectionY(Form("hPt_LeftSdB_iNeuCase%d_iy%d",  i_ncase, iy), rapBinLow,rapBinHi, mLeftSdB_BinLow, mLeftSdB_BinHig);
			hPt_RightSdB_in_ny[i_ncase][iy] = (TH1D*)hMvsPtvsRap_inWork->ProjectionY(Form("hPt_RightSdB_iNeuCase%d_iy%d", i_ncase, iy), rapBinLow,rapBinHi, mRightSdB_BinLow,mRightSdB_BinHig);
			
			//add left and right side band
			hPt_SdB_in_ny[i_ncase][iy] = (TH1D*) hPt_LeftSdB_in_ny[i_ncase][iy] -> Clone(Form("hPt_SdB_iNeuCase%d_iy%d",  i_ncase, iy));
			hPt_SdB_in_ny[i_ncase][iy] -> Add( hPt_RightSdB_in_ny[i_ncase][iy] );
			//---------------------------------------------------------------------------------------



			//			massBinLow = hMvsPtvsRap_inWork->GetZaxis()->FindBin(mLowMassBandLow + mTinyNum);
			//			massBinHi  = hMvsPtvsRap_inWork->GetZaxis()->FindBin(mLowMassBandHi  - mTinyNum);
			//			hLowMassBandPt_NeuDir[ip][im][iy] 
			//				= (TH1D *)hMvsPtvsRap_inWork->ProjectionY(Form("hLowMassBandPt_NeuDir%dp%dm_RapBin%d", ip, im, iy), rapBinLow, rapBinHi, massBinLow, massBinHi);
			//			hLowMassBandPt_NeuDir[ip][im][iy]->SetTitle(Form("%1.1f < y < %1.1f", mDiffRapLow[iy], mDiffRapHi[iy]));
			//
			//			massBinLow = hMvsPtvsRap_inWork->GetZaxis()->FindBin(mJpsiMassLow + mTinyNum);
			//			massBinHi  = hMvsPtvsRap_inWork->GetZaxis()->FindBin(mJpsiMassHi - mTinyNum);
			//			hJpsiPt_NeuDir[ip][im][iy] 
			//				= (TH1D *)hMvsPtvsRap_inWork->ProjectionY(Form("hJpsiPt_NeuDir%dp%dm_RapBin%d", ip, im, iy), rapBinLow, rapBinHi, massBinLow, massBinHi);
			//			hJpsiPt_NeuDir[ip][im][iy]->SetTitle(Form("%1.1f < y < %1.1f", mDiffRapLow[iy], mDiffRapHi[iy]));
			//
			//			massBinLow = hMvsPtvsRap_inWork->GetZaxis()->FindBin(mHiMassBandLow + mTinyNum);
			//			massBinHi  = hMvsPtvsRap_inWork->GetZaxis()->FindBin(mHiMassBandHi - mTinyNum);
			//			hHiMassBandPt_NeuDir[ip][im][iy] = (TH1D *)hMvsPtvsRap_inWork->ProjectionY(Form("hHiMassBandPt_NeuDir%dp%dm_RapBin%d", ip, im, iy), rapBinLow, rapBinHi, massBinLow, massBinHi);
			//			hHiMassBandPt_NeuDir[ip][im][iy]->SetTitle(Form("%1.1f < y < %1.1f", mDiffRapLow[iy], mDiffRapHi[iy]));
		
			if( iy==0 )
			{
				hCohMass_in_ny[i_ncase][nDiffRapBins]  = (TH1D *) hCohMass_in_ny[i_ncase][iy] ->Clone( Form("hCohMass_in_iNeuCase%d", i_ncase) );
				
				hMass_in_ny[i_ncase][nDiffRapBins]     = (TH1D *) hMass_in_ny[i_ncase][iy]    ->Clone( Form("hMass_in_iNeuCase%d",    i_ncase) );
				hPt_Jpsi_in_ny[i_ncase][nDiffRapBins]  = (TH1D *) hPt_Jpsi_in_ny[i_ncase][iy] ->Clone( Form("hPt_Jpsi_in_iNeuCase%d", i_ncase) );
				
				hPt_SdB_in_ny[i_ncase][nDiffRapBins]   = (TH1D *) hPt_SdB_in_ny[i_ncase][iy]  ->Clone( Form("hPt_SdB_in_iNeuCase%d",  i_ncase) );
				
				hCohMass_in_ny[i_ncase][nDiffRapBins]  -> SetTitle( Form("%1.1f < |y| < %1.1f", mDiffRapLow[nDiffRapBins/2], mDiffRapHi[nDiffRapBins-1]) );
				hMass_in_ny[i_ncase][nDiffRapBins]     -> SetTitle( Form("%1.1f < |y| < %1.1f", mDiffRapLow[nDiffRapBins/2], mDiffRapHi[nDiffRapBins-1]) );
				hPt_Jpsi_in_ny[i_ncase][nDiffRapBins]  -> SetTitle( Form("%1.1f < |y| < %1.1f", mDiffRapLow[nDiffRapBins/2], mDiffRapHi[nDiffRapBins-1]) );
				hPt_SdB_in_ny[i_ncase][nDiffRapBins]   -> SetTitle( Form("%1.1f < |y| < %1.1f", mDiffRapLow[nDiffRapBins/2], mDiffRapHi[nDiffRapBins-1]) );
			}
			else
			{
				hCohMass_in_ny[i_ncase][nDiffRapBins]  -> Add( hCohMass_in_ny[i_ncase][iy] );
				
				hMass_in_ny[i_ncase][nDiffRapBins]     -> Add( hMass_in_ny[i_ncase][iy] );
				hPt_Jpsi_in_ny[i_ncase][nDiffRapBins]  -> Add( hPt_Jpsi_in_ny[i_ncase][iy] );
				hPt_SdB_in_ny[i_ncase][nDiffRapBins]   -> Add( hPt_SdB_in_ny[i_ncase][iy] );
			}
		}//iy

		if(SymmetricRapBin)
		{
			for (int iy = nDiffRapBins/2; iy < nDiffRapBins; ++iy)
			{
				hCohMass_in_ny[i_ncase][iy]  -> SetTitle( Form("%1.1f < |y| < %1.1f", mDiffRapLow[iy], mDiffRapHi[iy]) );
				hMass_in_ny[i_ncase][iy]     -> SetTitle( Form("%1.1f < |y| < %1.1f", mDiffRapLow[iy], mDiffRapHi[iy]) );
				hPt_Jpsi_in_ny[i_ncase][iy]  -> SetTitle( Form("%1.1f < |y| < %1.1f", mDiffRapLow[iy], mDiffRapHi[iy]) );
				hPt_SdB_in_ny[i_ncase][iy]   -> SetTitle( Form("%1.1f < |y| < %1.1f", mDiffRapLow[iy], mDiffRapHi[iy]) );
				
				hCohMass_in_ny[i_ncase][iy]  -> Add( hCohMass_in_ny[i_ncase][nDiffRapBins - iy - 1] );
				hMass_in_ny[i_ncase][iy]     -> Add( hMass_in_ny[i_ncase][nDiffRapBins - iy - 1] );
				hPt_Jpsi_in_ny[i_ncase][iy]  -> Add( hPt_Jpsi_in_ny[i_ncase][nDiffRapBins - iy - 1] );
				hPt_SdB_in_ny[i_ncase][iy]   -> Add( hPt_SdB_in_ny[i_ncase][nDiffRapBins - iy - 1] );
			}
		}
	}//i_ncase
}
//------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------
void loadEff()
{
	LoadEfficiency Efficiency("../anaSimu/EffundTemp/Eff_CohJpsi.root", SymmetricRapBin);
	H_EffVsY_CohJpsi 		= (TH1D*)	Efficiency.GetCohJpsi()	 	->Clone();
}
//------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------
void loadAcc()
{
	LoadAcceptance Acceptance(Form("../simulation/out4AccFactors/Acceptance_AllSpecs_%dRapBins.root", nDiffRapBins), SymmetricRapBin);
	H_AccVsY_CohJpsi		= (TH1D*)	Acceptance.GetAcceptance()	->Clone();
}
//------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------
void fitCohMass_4RNRfD( const double massLow4Fit=2.6, const double massHig4Fit=4.2)
{
	cout<<"now, let's fit the pt spectras !!!"<<endl;
	
	TCanvas* c1 = new TCanvas("c1", "c1", 0, 0, 800, 600);
	setPad(0.12, 0.08, 0.07, 0.13);
	c1->cd();
	c1->SetLogy(0);
	RooRealVar  mMass("mMass", "m_{#mu#mu} (GeV)", massLow4Fit, massHig4Fit);

	TFile *inf_Temps = TFile::Open(Form("../simulation/%s/MassPtTemp_AllSpecs_massWindow_2.95_3.25_%dRapBins%s.root", template_Dir[template_option].Data(), nDiffRapBins, TnPcases[RunTnPcase].Data()));
	TF1 *fQED;
	fQED = new TF1("fQED", fReject4QED, massLow4Fit, massHig4Fit, 4);
	TF1 *fCohJpsiTemp;
	//------------------------------------------------------------------------------------------------------------
	//------------------------------------------------------------------------------------------------------------
	for(int i_ncase=0; i_ncase<NnCases; i_ncase++)
	{
		if(std::find(RunCase.begin(), RunCase.end(), i_ncase) == RunCase.end()) continue; //tem skip
		
		cout<<"i_ncase: "<<i_ncase<<endl;

		for(int iy=0; iy<nDiffRapBins+1; iy++)
		{
			if(iy==nDiffRapBins) continue; //skip all y added fitting 
			if(SymmetricRapBin && iy < nDiffRapBins/2 )          continue;
			
			cout<<"iy: "<<iy<<" "<<mDiffRapLow[iy]<<" <y< "<<mDiffRapHi[iy]<<endl;
			
			//fCohJpsiTemp = (TF1  *) inf_Temps->Get( "fCohJpsiTemp" ); //if use total temp parms to initialize Jpsi sig parms
		
			if( i_ncase ==0 ) //AnAn
			{
				//if use temp parms from different y bins to initialize
				if(     iy==nDiffRapBins)fCohJpsiTemp = (TF1  *) inf_Temps->Get( "fCohJpsi_0n0nTemp" ); 
				else if(iy<nDiffRapBins) fCohJpsiTemp = (TF1  *) inf_Temps->Get( Form("fCohJpsi_0n0nTemp_RapBin%d", iy) );
			}
			else if(i_ncase>=2||i_ncase<=4)
			{
				if(     iy==nDiffRapBins)fCohJpsiTemp = (TF1  *) inf_Temps->Get( "fCohJpsi_0nXnTemp" ); 
				else if(iy<nDiffRapBins) fCohJpsiTemp = (TF1  *) inf_Temps->Get( Form("fCohJpsi_0nXnTemp_RapBin%d", iy) );
			}
			else
			{
				if(     iy==nDiffRapBins)fCohJpsiTemp = (TF1  *) inf_Temps->Get( "fCohJpsi_"+nCasesName[i_ncase]+"Temp" ); 
				else if(iy<nDiffRapBins) fCohJpsiTemp = (TF1  *) inf_Temps->Get( Form("fCohJpsi_"+nCasesName[i_ncase]+"Temp_RapBin%d", iy) );
			}
			
			double yMean = (mDiffRapLow[iy]+mDiffRapHi[iy])/2.;

			double eff_CohJpsi      = H_EffVsY_CohJpsi     ->GetBinContent(H_EffVsY_CohJpsi    ->FindBin(yMean));
//			double eff_CohPsi       = H_EffVsY_CohPsi      ->GetBinContent(H_EffVsY_CohPsi     ->FindBin(yMean));
//			double eff_CohPsi2Jpsi  = H_EffVsY_CohPsi2Jpsi ->GetBinContent(H_EffVsY_CohPsi2Jpsi->FindBin(yMean));

			double acc_CohJpsi 		= 1.0; //H_AccVsY_CohJpsi	   ->GetBinContent(H_AccVsY_CohJpsi    ->FindBin(yMean));
			
			Eff_CohJpsi[i_ncase][iy] = eff_CohJpsi;
			Acc_CohJpsi[iy]			 = acc_CohJpsi;

			cout<<"eff_CohJpsi: "    <<eff_CohJpsi    <<endl;
//			cout<<"eff_CohPsi: "     <<eff_CohPsi     <<endl;
//			cout<<"eff_CohPsi2Jpsi: "<<eff_CohPsi2Jpsi<<endl;
			// cout<<"acc_CohJpsi: "    <<acc_CohJpsi    <<endl;
			
			TH1D* hCohMass = (TH1D*) hCohMass_in_ny[i_ncase][iy]->Clone("hMass");
			
			if( i_ncase==0               ) hCohMass->Rebin(2);
			if( i_ncase==1 				 ) hCohMass->Rebin(2);
			if( i_ncase==2 || i_ncase==3 ) hCohMass->Rebin(2);
			if( i_ncase==4 				 ) hCohMass->Rebin(2);
			//if( i_ncase==5               ) hCohMass->Rebin(2);
			if( i_ncase==5               ) hCohMass->Rebin(3);
			//------------------------------------------------------------------------------------------------------------
			//------------------------------------------------------------------------------------------------------------
			
			const double Init_cbAlpha     = fCohJpsiTemp->GetParameter(1);
			const double Init_cbN         = fCohJpsiTemp->GetParameter(2);
			const double Init_sigmaRatio  = fCohJpsiTemp->GetParameter(3);

			const double Init_cbAlphaL    = 1.52;
			const double Init_cbAlphaR    = 1.83;
			const double Init_cbNL        = 7.82;
			const double Init_cbNR        = 13.;

			//--------------TEMP! For Initializing CBAN with CB to Jpsi Peak---------------
			// auto cc1 = new TCanvas();
			// auto fJpsiPeak = new TF1("fJpsiPeak", JpsiPdf::fReject4Jpsi,massLow4Fit, massHig4Fit,6);
			// fJpsiPeak->SetParameter(1,1);
			// fJpsiPeak->SetParameter(2,1);
			// fJpsiPeak->SetParameter(3,1);
			// fJpsiPeak->SetParameter(4,3.1);
			// auto hCohMassJpsiPeak = (TH1D*)hCohMass->Clone();
			// hCohMassJpsiPeak->Rebin(2);
			// hCohMassJpsiPeak ->Fit(fJpsiPeak, "", "",  massLow4Fit, massHig4Fit);
			// fJpsiPeak->Draw("same");
			// hCohMassJpsiPeak->Draw("same");
			// cc1->SaveAs("./test.png");
			// cout<<fJpsiPeak->GetParameter(4)<<"!!!!!!!!!!!!!!!!!!!!!!"<<endl;;
			// // fJpsiPeak->GetParameter(2);
			// continue;
			//--------------TEMP! For Initializing CBAN with CB to Jpsi Peak---------------

			RooRealVar  cbAlpha 	( "cbAlpha",     	"cbAlpha",		Init_cbAlpha,	0, 10   );
			RooRealVar  cbN 		( "cbN",         	"cbN",			Init_cbN,		0, 10   );
			RooRealVar  jpsiMu		( "jpsiMu",      	"jpsiMu",		3.096,	2.90,	3.2 	);
			RooRealVar  jpsiSigma	( "jpsiSigma",   	"jpsiSigma",	0.045,	0.01,	0.1 	);

			// RooRealVar  gausN		( "gausN",       	"gausN",		3.5,	0.00,	20 		);
			// RooConstVar  sigmaRatio 	( "sigmaRatio",   "sigmaRatio",  	Init_sigmaRatio );

			// RooRealVar  jpsiN(		"jpsiN",		"jpsiN",		9.,   0.1,  1.e2  );
			// RooRealVar  psiN(		"psiN",			"psiN",			9.,   0.1,  1.e2  );
			// RooRealVar  jpsiMu(     "jpsiMu",		"jpsiMu",      3.096, 2.9,  3.3 );
			// RooRealVar  jpsiSigma(  "jpsiSigma",	"jpsiSigma",   0.050, 0.01, 0.2 );
			// RooRealVar  jpsiSigmaL( "jpsiSigmaL",	"jpsiSigmaL",  0.045, 0.01, 0.2 );
			// RooRealVar  jpsiSigmaR( "jpsiSigmaR",	"jpsiSigmaR",  0.045, 0.01, 0.2 );
			// RooRealVar  sigmaRatio( "sigmaRatio",   "sigmaRatio",  Init_sigmaRatio, 0.1, Init_sigmaRatio*20 );
			// RooRealVar  cbAlphaL(   "cbAlphaL",		"cbAlphaL",    Init_cbAlphaL,	0, 20 );
			// RooRealVar  cbNL(       "cbNL",			"cbNL",        Init_cbNL,		0, 20 );
			// RooRealVar  cbAlphaR(   "cbAlphaR",		"cbAlphaR",    Init_cbAlphaR,	0, 20 );
			// RooRealVar  cbNR(       "cbNR",			"cbNR",        Init_cbNR,		0, 20 );

			QEDPdf 	cQEDPdf(	hCohMass, mMass, massLow4Fit, massHig4Fit, 3);
			cQEDPdf.Init();
			// cQEDPdf.InitQuartic();
			// cQEDPdf.InitFreeCubic();
			RooGenericPdf *qedPdf 	= cQEDPdf.GetPdf();

			JpsiPdf cJpsiPdf(	hCohMass, mMass, massLow4Fit, massHig4Fit	);
			cJpsiPdf.Init(cbAlpha, cbN, jpsiSigma, jpsiMu);
			// cJpsiPdf.InitCrystalBallGauss(cbAlpha, cbN, jpsiSigma, sigmaRatio, jpsiMu, gausN);
			// cJpsiPdf.InitDoubleCrystalBall(jpsiN, jpsiMu, jpsiSigma, cbNL, cbAlphaL, cbNR, cbAlphaR);
			// cJpsiPdf.InitAsymDoubleCrystalBall(jpsiN, jpsiMu, jpsiSigmaL, jpsiSigmaR, cbNL, cbAlphaL, cbNR, cbAlphaR);
			RooGenericPdf *jpsiPdf 	= cJpsiPdf.GetPdf();

//			PsiPdf 	cPsiPdf(	hCohMass, mMass, massLow4Fit, massHig4Fit	);
//			cPsiPdf.Init(cbAlpha, cbN, jpsiSigma, jpsiMu);
			// cPsiPdf.InitCrystalBallGauss(cbAlpha, cbN, jpsiSigma, sigmaRatio, jpsiMu, gausN);
			// cPsiPdf.InitDoubleCrystalBall(psiN, jpsiMu, jpsiSigma, cbNL, cbAlphaL, cbNR, cbAlphaR);
			// cPsiPdf.InitAsymDoubleCrystalBall(psiN, jpsiMu, jpsiSigmaL, jpsiSigmaR, cbNL, cbAlphaL, cbNR, cbAlphaR);
//			RooGenericPdf *psiPdf 	= cPsiPdf.GetPdf();
			
			//// directly use QED template from simulation
			//int jpsiMBinLow = hQEDMassHistTemp->GetXaxis()->FindBin(massLow4Fit + mTinyNum);
			//int jpsiMBinHi  = hQEDMassHistTemp->GetXaxis()->FindBin(massHig4Fit - mTinyNum);
			//int jpsiMassBinLow = hQEDMassHistTemp->GetXaxis()->FindBin(mJpsiMassLow + mTinyNum);
			//int jpsiMassBinHi  = hQEDMassHistTemp->GetXaxis()->FindBin(mJpsiMassHi - mTinyNum);
			//double mQEDFrac = hQEDMassHistTemp->Integral(jpsiMassBinLow, jpsiMassBinHi)*1./hQEDMassHistTemp->Integral(jpsiMBinLow, jpsiMBinHi);

			//hQEDMassHistTemp->RebinX(5);
			//RooDataHist hQEDMassRooHist("hQEDMassRooHist", "hQEDMassRooHist", mMass, hQEDMassHistTemp);
			//RooHistPdf  qedPdf("qedPdf", "qedPdf", mMass, hQEDMassRooHist, 2); // RebinX and interpolation order to make the QED pdf smooth 
			//------------------------------------------------------------------------------------------------------------
			//------------------------------------------------------------------------------------------------------------
			
			const double nQED4Init      = cQEDPdf.GetInitN(3.30, 3.50);

			//const double nJpsi4Init     = hCohMass->Integral(tem_JpsiBinLow, tem_JpsiBinHig)*0.80; //- nQED4Init*(3.30-2.80)/(massHig4Fit-massLow4Fit);
			const double nJpsi4Init     = cJpsiPdf.GetInitN(2.80, 3.30, nQED4Init);
			
//			const double nPsi4Init      = nJpsi4Init*0.050;

			RooRealVar nJpsi("nJpsi", "nJpsi", nJpsi4Init*1.1,  nJpsi4Init*0.20, nJpsi4Init*10.);
//			RooRealVar nPsi( "nPsi",  "nPsi",  nPsi4Init*1.0,   nPsi4Init*0.20,  nPsi4Init*10. );
			RooRealVar nQED( "nQED",  "nQED",  nQED4Init,       nQED4Init*0.20,  nQED4Init*10. );

//			RooAddPdf  totMassPdf("totMassPdf", "totMassPdf", RooArgList(*jpsiPdf, *psiPdf, *qedPdf), RooArgList(nJpsi, nPsi, nQED));
			RooAddPdf  totMassPdf("totMassPdf", "totMassPdf", RooArgList(*jpsiPdf, *qedPdf), RooArgList(nJpsi, nQED));//Add

			RooDataHist dataMass("dataMass", "dataMass", mMass, hCohMass);
			cout << "KKLLLLLLLLL" << dataMass.weight(10);

			//------------------------------------------------------------------------------------------------------------
			//------------------------------------------------------------------------------------------------------------
			for (int ib = 1; ib <= hCohMass->GetNbinsX();ib++)
			{
				if(fabs(hCohMass->GetBinContent(ib)-pow(hCohMass->GetBinError(ib),2))>1e-3)
					cout << "TATA: " << hCohMass->GetBinContent(ib) << "\t" << pow(hCohMass->GetBinError(ib), 2) << endl;
			}
			TFile* fte = new TFile("fte.root", "recreate");
			hCohMass->Write();
			fte->Close();
			// totMassPdf.fitTo( dataMass, Range(2.70, 3.50), Extended(kTRUE), SumW2Error(kTRUE), Hesse(kTRUE), Minos(kFALSE));
			totMassPdf.fitTo( dataMass, Extended(kTRUE), SumW2Error(kTRUE), Hesse(kTRUE), Minos(kFALSE), Save());
			RooFitResult *ResFit = totMassPdf.fitTo( dataMass, Extended(kTRUE), SumW2Error(kTRUE), Hesse(kTRUE), Minos(kFALSE), Save());
			//totMassPdf.fitTo(dataMass,Range(massLow4Fit, massHig4Fit),Extended(kTRUE),SumW2Error(kTRUE),Hesse(kTRUE),Minos(kFALSE),Save());
			//------------------------------------------------------------------------------------------------------------

			//calculate ratio #Psi/#Jpsi and its uncertainty
//			const TMatrixDSym &mtrx_cov = ResFit->covarianceMatrix();   //sigma_AB = rho_AB*sigmaA*sigmaB, rho_AB is the correlationCoefficient
//			cout << "covariance matrix" << endl;
//			mtrx_cov.Print();

			const double nJpsiValue  = nJpsi.getVal();
			const double nJpsiError  = nJpsi.getError();
//			const double nPsiValue   = nPsi.getVal();
//			const double nPsiError   = nPsi.getError();
//			const double RN          = nPsiValue/nJpsiValue;
//			const double RNErr       = RN*sqrt( pow(nJpsiError/nJpsiValue, 2) + pow(nPsiError/nPsiValue, 2) - (2.*mtrx_cov[4][5])/(nPsiValue*nJpsiValue) );

//			cout<<"RN: "<<RN<<" +/- "<<RNErr<<endl;

			//------------------------------------------------------------------------------------------------------------
			//calculate R value and fD value
			//------------------------------------------------------------------------------------------------------------
//			const double aa    = br_Jpsi2uu * eff_CohJpsi;
//			const double bb    = br_Psi2uu  * eff_CohPsi;
//			const double cc    = br_Psi2Jpsi*eff_CohPsi2Jpsi*br_Jpsi2uu;

//			const double R     = ( RN * br_Jpsi2uu * eff_CohJpsi ) / ( br_Psi2uu*eff_CohPsi - RN*br_Psi2Jpsi*eff_CohPsi2Jpsi*br_Jpsi2uu  );
//			const double fD    = R * (eff_CohPsi2Jpsi/eff_CohJpsi) * br_Psi2Jpsi;
//			const double RErr  = ( aa/(bb-cc*RN) - (aa*cc*RN)/pow(bb-cc*RN,2) ) * RNErr;
//			const double fDErr = (eff_CohPsi2Jpsi/eff_CohJpsi) * br_Psi2Jpsi * RErr;

			NJpsi_inMFit[i_ncase][iy]    = nJpsiValue;
			NerrJpsi_inMFit[i_ncase][iy] = nJpsiError;
//			fD_inPtCut[i_ncase][iy]      = fD;
//			fDerr_inPtCut[i_ncase][iy]   = fDErr;
			//------------------------------------------------------------------------------------------------------------

			c1->cd();
			c1->SetLogy(0);

			int nFrameMBins  = (massHig4Fit - massLow4Fit)/hCohMass->GetBinWidth(1);
			RooPlot *frameMass = mMass.frame(Range(massLow4Fit, massHig4Fit), Title(""), Bins(nFrameMBins));
			frameMass->GetYaxis()->SetRangeUser(0.5, hCohMass->GetMaximum()*1.3);
			//frameMass ->GetYaxis()->SetTitleSize(0.10);
			frameMass ->GetYaxis()->SetTitleOffset(0.90);
			// frameMass ->GetYaxis()->SetTitleOffset(1.30);
			dataMass.plotOn(frameMass, MarkerStyle(20), MarkerSize(1), MarkerColor(2), LineColor(2), LineWidth(2), DrawOption("pez"));
			totMassPdf.plotOn(frameMass, LineColor(1), LineStyle(1), LineWidth(2));
			totMassPdf.plotOn(frameMass, Components(RooArgSet(*jpsiPdf)), LineColor(kBlue),    LineStyle(5), LineWidth(2));
//			totMassPdf.plotOn(frameMass, Components(RooArgSet(*psiPdf)),  LineColor(kBlue+2),  LineStyle(6), LineWidth(2));
			totMassPdf.plotOn(frameMass, Components(RooArgSet(*qedPdf)),  LineColor(qedColor), LineStyle(2), LineWidth(3));

			//			cout<<endl;
			//			cout<<"******** Print frame ********"<<endl;
			//			frameMass->Print();
			//			cout<<"******** End ********"<<endl;
			//			cout<<endl;


			double chi2ndf = frameMass->chiSquare("totMassPdf_Norm[mMass]", "h_dataMass", 7); // Need to change to 9 when using CBG. Jpsi+Psi fit

			frameMass->SetYTitle(Form("Events / (%.2f GeV)", hCohMass->GetBinWidth(1)));
			frameMass->Draw() ;
			
			TString yName = "";
			if(iy<nDiffRapBins)
			{	
				//different name for symmetric y bins
				if(SymmetricRapBin && iy > (nDiffRapBins/2 - 1)) yName = Form("%1.1f < |y^{#mu#mu}| < %1.1f", mDiffRapLow[iy],             mDiffRapHi[iy]            );
				else 											   yName = Form("%1.1f < y^{#mu#mu} < %1.1f",   mDiffRapLow[iy],             mDiffRapHi[iy]            );
			}
			else                								   yName = Form("%1.1f < |y^{#mu#mu}| < %1.1f", mDiffRapLow[nDiffRapBins/2], mDiffRapHi[nDiffRapBins-1]);
	
			const TString ptName = Form("%1.0f < p_{T}^{#mu#mu} < %1.1f GeV",  0.0,   mPtCut4Coh);

			drawLatex(0.15, 0.86, nCasesName[i_ncase], mTextFont, 0.06, mTextColor);
			drawLatex(.55, 0.86, yName,                mTextFont, 0.05, mTextColor);
			drawLatex(.55, 0.80, ptName,               mTextFont, 0.05, mTextColor);
			drawLatex(.15, 0.75, Form("#chi^{2}/ndf = %1.1f", chi2ndf),                                 mTextFont, mTextSize, mTextColor);
			const double textDy = 0.05;
			drawLatex(.55, 0.66+textDy, Form("N_{J/#psi} = %d #pm %d",   TMath::Nint(nJpsi.getVal()), TMath::Nint(nJpsi.getError())), mTextFont, mTextSize, mTextColor);
//			drawLatex(.55, 0.61+textDy, Form("N_{#psi(2S)} = %d #pm %d", TMath::Nint(nPsi.getVal()),  TMath::Nint(nPsi.getError()) ), mTextFont, mTextSize, mTextColor);
			drawLatex(.55, 0.55+textDy, Form("N_{QED} = %d #pm %d",      TMath::Nint(nQED.getVal()),  TMath::Nint(nQED.getError()) ), mTextFont, mTextSize, mTextColor);
//			drawLatex(.45, 0.45+textDy, Form("R_{N} = #frac{N_{#psi(2S)}}{N_{J/#psi}} = %.3f #pm %.4f", nPsi.getVal()/nJpsi.getVal(), RNErr ), mTextFont, mTextSize, mTextColor);
//			drawLatex(.45, 0.35+textDy, Form("R = #frac{#sigma_{#psi(2S)}}{#sigma_{J/#psi}} = %.3f #pm %.4f", R, RErr ), mTextFont, mTextSize, mTextColor);
//			drawLatex(.45, 0.25+textDy, Form("f_{D} = #frac{FD J/#psi}{primary J/#psi} = %.3f #pm %.4f",      fD,fDErr),  mTextFont, mTextSize, mTextColor);

			cout<<"y name: "<<yName<<endl;
			cout<<"y name: "<<yName<<" eff_CohJpsi*acc_CohJpsi/0.942: "<<eff_CohJpsi*acc_CohJpsi/0.942<<endl;


			//----------------------------------------------------------------------------------------------------------------------------------------------------
			if(SymmetricRapBin && iy > (nDiffRapBins/2 - 1) && iy < nDiffRapBins){
				c1->SaveAs( Form("outplots/massSpec_4JpsiPsi_"+nCasesName[i_ncase]+"_iy%d_Symm.png",  iy) );
				c1->SaveAs( Form("outplots/massSpec_4JpsiPsi_"+nCasesName[i_ncase]+"_iy%d_Symm.pdf",  iy) );
			}
			else{
				c1->SaveAs( Form("outplots/massSpec_4JpsiPsi_"+nCasesName[i_ncase]+"_iy%d.png",  iy) );
				c1->SaveAs( Form("outplots/massSpec_4JpsiPsi_"+nCasesName[i_ncase]+"_iy%d.pdf",  iy) );			
			}
			//----------------------------------------------------------------------------------------------------------------------------------------------------
		
			if(draw4Paper_flag==1)
			{

				frameMass->GetYaxis()->SetRangeUser(0.01, hCohMass->GetMaximum()*1.25);
				frameMass->SetYTitle(Form("Events / (%.2f GeV)", hCohMass->GetBinWidth(1)));
				frameMass->GetXaxis()->SetTitleOffset(0.95);
				frameMass->Draw() ;
				//frame_mMass_7fef6c828bd0[mMass] = (RooHist::h_dataMass,RooCurve::totMassPdf_Norm[mMass],RooCurve::totMassPdf_Norm[mMass]_Comp[jpsiCrystalBallPdf],RooCurve::totMassPdf_Norm[mMass]_Comp[psiCrystalBallPdf],RooCurve::totMassPdf_Norm[mMass]_Comp[qedPdf])

				TLegend  *leg =  new TLegend(0.50, 0.53, 0.80, 0.78);
				leg->SetFillStyle(0);
				leg->SetFillColor(0);
				leg->SetTextFont(mTextFont);
				leg->SetTextSize(0.045);
				leg->AddEntry(frameMass->findObject("h_dataMass"),            "Data",        "pe");
				leg->AddEntry(frameMass->findObject("totMassPdf_Norm[mMass]"),   Form("Fit: #chi^{2}/ndf = %1.1f", chi2ndf),   "l");
				leg->AddEntry(frameMass->findObject("totMassPdf_Norm[mMass]_Comp[jpsiCrystalBallPdf]"), Form("J/#psi (%d #pm %d)",TMath::Nint(nJpsi.getVal()), TMath::Nint(nJpsi.getError())), "l" );
//				leg->AddEntry(frameMass->findObject("totMassPdf_Norm[mMass]_Comp[psiCrystalBallPdf]"),  Form("#psi(2S) (%d #pm %d)",TMath::Nint(nPsi.getVal()), TMath::Nint(nPsi.getError())), "l" );
				leg->AddEntry(frameMass->findObject("totMassPdf_Norm[mMass]_Comp[qedPdf]"),  "#gamma#gamma #rightarrow #mu#mu", "l" );

				leg->Draw("same");

				//const TString curveName[3]  = {"totMassPdf_Norm[mMass]_Comp[jpsiCrystalBallPdf]","totMassPdf_Norm[mMass]_Comp[psiCrystalBallPdf]", "totMassPdf_Norm[mMass]_Comp[qedPdf]"};
				//const TString curveTitle[3] = {"J/#psi", "#psi(2S)", "#gamma#gamma #rightarrow #mu#mu"};
				//for(int icv=0; icv<3; icv++) 
				//{
				//	leg->AddEntry(frameMass->findObject( curveName[icv]), curveTitle[icv], "l");
				//}
				
				//drawLatex(0.69, 0.945, "#bf{CMS} #it{Preliminary}",  42,        0.045,      mTextColor );
				//drawLatex(0.16, 0.86, "Pb-Pb #sqrt{s_{NN}} = 5.02 TeV UPC ("+nCasesName[i_ncase]+")",  42,        0.055,      mTextColor );

				drawLatex(0.25, 0.950, "#bf{CMS}", 42,        0.050,      mTextColor );
				drawLatex(0.53, 0.950, "PbPb 1.52 nb^{-1} (5.02 TeV)",   42, 0.05,  1);
				drawLatex(0.17, 0.750, nCasesName[i_ncase],   42, 0.06,  1);
				//drawLatex(0.40, 0.950, "Pb-Pb #sqrt{s_{NN}} = 5.02 TeV UPC ("+nCasesName[i_ncase]+")", 42,        0.045,      mTextColor );

				//drawLatex(0.15, 0.86, nCasesName[i_ncase], mTextFont, 0.06, mTextColor);
				drawLatex(.18, 0.86, yName,                mTextFont, 0.05, mTextColor);
				drawLatex(.56, 0.86, ptName,               mTextFont, 0.05, mTextColor);
				
				//drawLatex(.55, 0.65+textDy, Form("N_{J/#psi} = %d #pm %d",   TMath::Nint(nJpsi.getVal()), TMath::Nint(nJpsi.getError())), mTextFont, mTextSize, mTextColor);
//				drawLatex(.58, 0.43+textDy, Form("R_{N} = %.3f #pm %.3f",   nPsi.getVal()/nJpsi.getVal(), RNErr ), mTextFont, mTextSize-0.005, mTextColor);
//				drawLatex(.58, 0.38+textDy, Form("R =  %.3f #pm %.3f",      R,                            RErr ),  mTextFont, mTextSize-0.005, mTextColor);
//				drawLatex(.58, 0.33+textDy, Form("f_{D}  = %.3f #pm %.3f",  fD,                           fDErr),  mTextFont, mTextSize-0.005, mTextColor);

				//----------------------------------------------------------------------------------------------------------------------------------------------------
				if(SymmetricRapBin && iy > (nDiffRapBins/2 - 1) && iy < nDiffRapBins){
					c1->SaveAs( Form("outplots/massSpec_4JpsiPsi_"+nCasesName[i_ncase]+"_iy%d_Symm_4paper.png",  iy) );
					c1->SaveAs( Form("outplots/massSpec_4JpsiPsi_"+nCasesName[i_ncase]+"_iy%d_Symm_4paper.pdf",  iy) );
				}
				else{
					c1->SaveAs( Form("outplots/massSpec_4JpsiPsi_"+nCasesName[i_ncase]+"_iy%d_4paper.png",  iy) );
					c1->SaveAs( Form("outplots/massSpec_4JpsiPsi_"+nCasesName[i_ncase]+"_iy%d_4paper.pdf",  iy) );			
				}
				//----------------------------------------------------------------------------------------------------------------------------------------------------

			}

			//------------------------------------------------------------------------------------------------------------------------------------------
			//calculate the fiting pull: (Data-FitCurve)/#Sigma_{Data}
			c1->cd();
			RooHist *hpull_mass = frameMass->pullHist("h_dataMass", "totMassPdf_Norm[mMass]"); //"totMassPdf_Norm[mMass]", "h_dataMass"
			hpull_mass ->SetMarkerStyle(24);
			hpull_mass ->SetMarkerSize(0.8);
			hpull_mass ->SetMarkerColor(1);
			hpull_mass ->SetLineColor(1);
			hpull_mass ->SetLineWidth(1);

			RooPlot *frameMassPull = mMass.frame( Range(massLow4Fit, massHig4Fit), Title(""), Bins(nFrameMBins) );
			frameMassPull ->addPlotable(hpull_mass, "pz");
			frameMassPull ->GetYaxis()->SetRangeUser(-10.0, 10.0);
			frameMassPull ->SetYTitle("(Data-Fit)/(#sigma_{Data})");
			frameMassPull ->SetXTitle("M_{#mu#mu} (GeV)");
			frameMassPull ->GetYaxis()->CenterTitle();
			//frameMassPull ->GetYaxis()->SetNdivisions(6);
			frameMassPull ->GetYaxis()->SetTitleSize(0.07);
			frameMassPull ->GetYaxis()->SetTitleOffset(0.70);
			frameMassPull ->GetYaxis()->SetLabelSize(0.05);
			//frameMassPull ->GetYaxis()->SetLabelFont(20);
			frameMassPull ->GetXaxis()->SetTitleSize(0.05);
			frameMassPull ->GetXaxis()->SetTitleOffset(1.05);
			frameMassPull ->GetXaxis()->SetLabelSize(0.05);
			//frameMassPull ->GetXaxis()->SetLabelFont(40);
			frameMassPull ->SetTickLength(0.04);
			frameMassPull ->Draw() ;
			
			drawLine(massLow4Fit, 0, massHig4Fit, 0, 1, 2, 2);
			
			drawLatex(0.15, 0.86, nCasesName[i_ncase],    mTextFont, 0.06, mTextColor);
			drawLatex(0.15, 0.80, "(Mass Fit Pull Hist)", mTextFont, 0.05, mTextColor);
			drawLatex(0.55, 0.86, yName,                  mTextFont, 0.05, mTextColor);
			drawLatex(0.55, 0.80, ptName,                 mTextFont, 0.05, mTextColor);
			//------------------------------------------------------------------------------------------------------------------------------------------
			
			//if(SymmetricRapBin && iy > (nDiffRapBins/2 - 1) && iy < nDiffRapBins){
			//c1->SaveAs( Form("outplots/massSpecFitPull_4JpsiPsi_"+nCasesName[i_ncase]+"_iy%d_Symm.png",  iy) );
			//c1->SaveAs( Form("outplots/massSpecFitPull_4JpsiPsi_"+nCasesName[i_ncase]+"_iy%d_Symm.pdf",  iy) );
			//}
			//else{
			//c1->SaveAs( Form("outplots/massSpecFitPull_4JpsiPsi_"+nCasesName[i_ncase]+"_iy%d.png",  iy) );
			//c1->SaveAs( Form("outplots/massSpecFitPull_4JpsiPsi_"+nCasesName[i_ncase]+"_iy%d.pdf",  iy) );
			//}

			delete ResFit;
			delete hCohMass;
			delete frameMass;
			delete frameMassPull;
		}//iy
	}//incase

	delete c1;
	delete inf_Temps;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------------------------
void fitFullMassAndPt_4Decouple( const double massLow4Fit=2.6, const double massHig4Fit=4.2, const double ptLow4Fit = 0.0, const double ptHig4Fit = 3.5 )
{
	const int nPtBDs4AnAn = 51+50+1-1;
	const double PtBDs4AnAn[nPtBDs4AnAn+1] = 
	{
		0, 0.02, 0.04, 0.06, 0.08, 0.1, 0.12, 0.14, 0.16, 0.18, 0.2, 0.22, 0.24, 0.26, 0.28, 0.3, 0.32, 0.34, 0.36, 0.38, 0.4, 0.42, 0.44, 0.46, 0.48, 0.5, 0.52, 0.54, 0.56, 0.58, 0.6, 0.62, 0.64, 0.66, 0.68, 0.7, 0.72, 0.74, 0.76, 0.78, 0.8, 0.82, 0.84, 0.86, 0.88, 0.9, 0.92, 0.94, 0.96, 0.98, 1.00, //51
		1.04, 1.08, 1.12, 1.16, 1.2, 1.24, 1.28, 1.32, 1.36, 1.4, 1.44, 1.48, 1.52, 1.56, 1.6, 1.64, 1.68, 1.72, 1.76, 1.8, 1.84, 1.88, 1.92, 1.96, 2, 2.04, 2.08, 2.12, 2.16, 2.2, 2.24, 2.28, 2.32, 2.36, 2.4, 2.44, 2.48, 2.52, 2.56, 2.6, 2.64, 2.68, 2.72, 2.76, 2.8, 2.84, 2.88, 2.92, 2.96, 3.00, //50
		6.0 //1
	};
	
	const int nPtBDs40n0n = 26+13+18+1-1;
	const double PtBDs40n0n[nPtBDs40n0n+1] = 
	{
		0, 0.02, 0.04, 0.06, 0.08, 0.1, 0.12, 0.14, 0.16, 0.18, 0.2, 0.22, 0.24, 0.26, 0.28, 0.3, 0.32, 0.34, 0.36, 0.38, 0.4, 0.42, 0.44, 0.46, 0.48, 0.5, //26
		0.52, 0.56, 0.6, 0.64, 0.68, 0.72, 0.76, 0.8, 0.84, 0.88, 0.92, 0.96, 1.00, //13
		1.04, 1.12, 1.2, 1.28, 1.36, 1.44, 1.52, 1.6, 1.68, 1.76, 1.84, 1.92, 2, 2.2, 2.4, 2.6, 2.8, 3.00, //18
		6.0 //1
	};

	const int nPtBDs4XnXn = 11+8+5+11+6-1;
	const double PtBDs4XnXn[nPtBDs4XnXn+1] = 
	{
		0,0.02,0.04,0.06,0.08,0.1,0.12,0.14,0.16,0.18,0.2, //11
		0.22, 0.26, 0.30, 0.34, 0.38, 0.42, 0.46, 0.50,  //8
		//0.54, 0.58, 0.62, 0.66, 0.70,0.74,0.78,0.82,0.86,0.90, //10
		0.58, 0.66, 0.74, 0.82, 0.90, //5
		1.00, 1.10, 1.20, 1.30, 1.40, 1.50, 1.60, 1.70, 1.80, 1.90, 2.0, //11
		2.20, 2.40, 2.60, 2.80, 3.0, 6.0 //6
	};

	cout<<"now, let's fit the pt spectras !!!"<<endl;
	//--------------------------------------------------
	TCanvas* c2 = new TCanvas("c2", "c2", 900, 900); //for pt fitting
	//--------------------------------------------------
	
	//--------------------------------------------------
	TCanvas* c3;
	if(draw4Paper_flag==1)
	{
		c3 = new TCanvas("c3", "c3", 0, 0, 800, 600); //for pt fitting figures for paper
		setPad(0.12, 0.08, 0.07, 0.13);
	}
	//--------------------------------------------------


	//--------------------------------------------------
	TCanvas* c1 = new TCanvas("c1", "c1", 0, 0, 800, 600);
	setPad(0.12, 0.08, 0.07, 0.13);
	c1->cd();
	c1->SetLogy(0);
	RooRealVar  mMass("mMass", "m_{#mu#mu} (GeV)", massLow4Fit, massHig4Fit);
	//--------------------------------------------------

	TFile *inf_Temps = TFile::Open(Form("../simulation/%s/MassPtTemp_AllSpecs_massWindow_2.95_3.25_%dRapBins%s.root", template_Dir[template_option].Data(), nDiffRapBins, TnPcases[RunTnPcase].Data()));
	TF1 *fQED = new TF1("fQED", fReject4QED, massLow4Fit, massHig4Fit, 4);
	TF1 *fCohJpsiTemp;


	for(int i_ncase=0; i_ncase<NnCases; i_ncase++)
	{
		cout<<"i_ncase: "<<i_ncase<<endl;
		if(std::find(RunCase.begin(), RunCase.end(), i_ncase) == RunCase.end()) continue; //tem skip

		for(int iy=0; iy<nDiffRapBins+1; iy++)
		{
			if(iy==nDiffRapBins)                           continue; //temperory to skip all y added fitting 
			if(SymmetricRapBin && iy < nDiffRapBins/2 )    continue; //tem skip
			
			double dY = mDiffRapHi[iy] - mDiffRapLow[iy]; //need to be updated if use for 1.6<|y|<2.4
			if(SymmetricRapBin && iy > (nDiffRapBins/2 -1) && iy<nDiffRapBins) dY *= 2;

			cout<<"iy: "<<iy<<" "<<mDiffRapLow[iy]<<" <y< "<<mDiffRapHi[iy]<<endl;
	
			TH1D* hMass   = (TH1D*) hMass_in_ny[i_ncase][iy]    ->Clone("hMass");
			TH1D* hPt     = (TH1D*) hPt_Jpsi_in_ny[i_ncase][iy] ->Clone("hPt");
			TH1D* hPt_SdB = (TH1D*) hPt_SdB_in_ny[i_ncase][iy]  ->Clone("hPt_SdB");

			
			////tem test
			//hPt       ->Rebin(2);
			//for(int iPT =0; iPT<hPt->GetNbinsX(); iPT++  )
			//{
			//	cout<<hPt->GetBinLowEdge(iPT)<<",";
			//}
			//cout<<endl;
			////tem test

			//"AnAn", "0n0n", "0nXn", "Xn0n", "0nXnSum", "XnXn"
			
			
			//Normalize
			//hPt ->Scale( 1./hPt->Integral() );
			

		//	//Rebin Pt
		//	if( i_ncase == 1) //0n0n
		//	{
		//		hPt = (TH1D*) hPt->Rebin(nPtBDs40n0n, Form("hPt_Rebin_incase%d_iy%d",i_ncase,iy), PtBDs40n0n);
		//	}
		//	else if( i_ncase == 5 )
		//	{
		//		hPt = (TH1D*) hPt->Rebin(nPtBDs4XnXn, Form("hPt_Rebin_incase%d_iy%d",i_ncase,iy), PtBDs4XnXn);
		//	}
		//	else 
		//	{
		//		hPt = (TH1D*) hPt->Rebin(nPtBDs4AnAn, Form("hPt_Rebin_incase%d_iy%d",i_ncase,iy), PtBDs4AnAn);
		//	}




			for(int iPT =1; iPT<hPt->GetNbinsX()+1; iPT++  )
			{
				cout<<hPt->GetBinLowEdge(iPT)<<",";
			}
			cout<<endl;
			cout<<hPt->GetBinLowEdge(hPt->GetNbinsX()+1)<<","<<endl;





			if( i_ncase==0 )
			{
				hMass     ->Rebin(2);
				hPt       ->Rebin(2);
				//hPt_SdB   ->Rebin(2);
			}
			else if ( i_ncase==2|| i_ncase==3 )
			{
				hMass     ->Rebin(2);
				hPt       ->Rebin(2);
				//hPt_SdB   ->Rebin(2);
			}
			else if(  i_ncase==5 )
			{
				hMass     ->Rebin(2);
				hPt       ->Rebin(3);
				//hPt_SdB   ->Rebin(2);
			}
			else
			{
				hMass     ->Rebin(2);
				hPt       ->Rebin(2);
				//hPt_SdB   ->Rebin(2);
			}

						//------------------------------------------------------------------------------------------------------------
			//1. Fit mass in full pt range to get the QED yield value
			//------------------------------------------------------------------------------------------------------------


			//------------------------------------------------------------------------------------------------------------
			if(      iy==nDiffRapBins ) fCohJpsiTemp = (TF1 *) inf_Temps->Get( "fCohJpsiTemp" );
			else if( iy<nDiffRapBins  ) fCohJpsiTemp = (TF1 *) inf_Temps->Get( Form("fCohJpsiTemp_RapBin%d", iy) );
			//------------------------------------------------------------------------------------------------------------

			const double Init_cbAlpha     = fCohJpsiTemp->GetParameter(1);
			const double Init_cbN         = fCohJpsiTemp->GetParameter(2);
			const double Init_sigmaRatio  = fCohJpsiTemp->GetParameter(3);

			const double Init_cbAlphaL    = 1.52;
			const double Init_cbAlphaR    = 1.83;
			const double Init_cbNL        = 7.82;
			const double Init_cbNR        = 13.;

			//------------------------------------------------------------------------------------------------------------
			RooRealVar  cbAlpha 	( "cbAlpha",     	"cbAlpha",		Init_cbAlpha,	0, 10   );
			RooRealVar  cbN 		( "cbN",         	"cbN",			Init_cbN,		0, 10   );
			RooRealVar  jpsiMu		( "jpsiMu",      	"jpsiMu",		3.096,	2.90,	3.2 	);
			RooRealVar  jpsiSigma	( "jpsiSigma",   	"jpsiSigma",	0.045,	0.01,	0.1 	);

			// RooRealVar  gausN		( "gausN",       	"gausN",		3.5,	0.00,	20 		);
			// RooConstVar  sigmaRatio 	( "sigmaRatio",   "sigmaRatio",  	Init_sigmaRatio );

			// RooRealVar  jpsiN(		"jpsiN",		"jpsiN",	   9.,     0.1,  1.e2);
			// RooRealVar  psiN(		"psiN",			"psiN",		   9.,     0.1,  1.e2);
			// RooRealVar  jpsiMu(     "jpsiMu",		"jpsiMu",      3.096,  2.9,  3.3 );
			// RooRealVar  jpsiSigma(  "jpsiSigma",	"jpsiSigma",   0.053,  0.01, 0.1 );
			// RooRealVar  jpsiSigmaL( "jpsiSigmaL",	"jpsiSigmaL",  0.045,  0.01, 0.1 );
			// RooRealVar  jpsiSigmaR( "jpsiSigmaR",	"jpsiSigmaR",  0.045,  0.01, 0.1 );
			// RooRealVar  sigmaRatio( "sigmaRatio",   "sigmaRatio",  Init_sigmaRatio, 0.1, Init_sigmaRatio*20 );
			// RooRealVar  cbAlphaL(   "cbAlphaL",		"cbAlphaL",    Init_cbAlphaL,	0, 20 );
			// RooRealVar  cbNL(       "cbNL",			"cbNL",        Init_cbNL,		0, 20 );
			// RooRealVar  cbAlphaR(   "cbAlphaR",		"cbAlphaR",    Init_cbAlphaR,	0, 20 );
			// RooRealVar  cbNR(       "cbNR",			"cbNR",        Init_cbNR,		0, 20 );

			QEDPdf 	cQEDPdf(	hMass, mMass, massLow4Fit, massHig4Fit, 3);
			cQEDPdf.Init();
			// cQEDPdf.InitQuartic();
			// cQEDPdf.InitFreeCubic();
			RooGenericPdf *qedPdf 	= cQEDPdf.GetPdf();

			JpsiPdf cJpsiPdf(	hMass, mMass, massLow4Fit, massHig4Fit	);
			cJpsiPdf.Init(cbAlpha, cbN, jpsiSigma, jpsiMu);
			// cJpsiPdf.InitCrystalBallGauss(cbAlpha, cbN, jpsiSigma, sigmaRatio, jpsiMu, gausN);
			// cJpsiPdf.InitDoubleCrystalBall(jpsiN, jpsiMu, jpsiSigma, cbNL, cbAlphaL, cbNR, cbAlphaR);
			// cJpsiPdf.InitAsymDoubleCrystalBall(jpsiN, jpsiMu, jpsiSigmaL, jpsiSigmaR, cbNL, cbAlphaL, cbNR, cbAlphaR);
			RooGenericPdf *jpsiPdf 	= cJpsiPdf.GetPdf();

			PsiPdf 	cPsiPdf(	hMass, mMass, massLow4Fit, massHig4Fit	);
			cPsiPdf.Init(cbAlpha, cbN, jpsiSigma, jpsiMu);
			// cPsiPdf.InitCrystalBallGauss(cbAlpha, cbN, jpsiSigma, sigmaRatio, jpsiMu, gausN);
			// cPsiPdf.InitDoubleCrystalBall(psiN, jpsiMu, jpsiSigma, cbNL, cbAlphaL, cbNR, cbAlphaR);
			// cPsiPdf.InitAsymDoubleCrystalBall(psiN, jpsiMu, jpsiSigmaL, jpsiSigmaR, cbNL, cbAlphaL, cbNR, cbAlphaR);
			RooGenericPdf *psiPdf 	= cPsiPdf.GetPdf();
			//------------------------------------------------------------------------------------------------------------

			//------------------------------------------------------------------------------------------------------------
			
			const double nQED4Init      = cQEDPdf.GetInitN(3.25, 3.50);
			const double nJpsi4Init     = cJpsiPdf.GetInitN(2.95, 3.25, nQED4Init);

			RooRealVar nJpsi("nJpsi", "nJpsi", nJpsi4Init*0.50,  0, nJpsi4Init*10);
			RooRealVar nPsi( "nPsi",  "nPsi",  nJpsi4Init*0.05,  0, nJpsi4Init*0.05*10);
			RooRealVar nQED( "nQED",  "nQED",  nQED4Init*0.95,   0, nQED4Init*10);
			RooAddPdf  totMassPdf("totMassPdf", "totMassPdf", RooArgList(*jpsiPdf, *psiPdf, *qedPdf), RooArgList(nJpsi, nPsi, nQED)); 

			RooDataHist dataMass("dataMass", "dataMass", mMass, hMass); 

			//------------------------------------------------------------------------------------------------------------
			//------------------------------------------------------------------------------------------------------------
			totMassPdf.fitTo( dataMass, Extended(kTRUE), SumW2Error(kTRUE), Hesse(kTRUE), Minos(kFALSE), Save());
			RooFitResult *ResFit = totMassPdf.fitTo( dataMass, Extended(kTRUE), SumW2Error(kTRUE), Hesse(kTRUE), Minos(kFALSE), Save());
			//------------------------------------------------------------------------------------------------------------
			//------------------------------------------------------------------------------------------------------------

			const double nJpsiValue  = nJpsi.getVal();
			const double nJpsiError  = nJpsi.getError();
			const double nPsiValue   = nPsi.getVal();
			const double nPsiError   = nPsi.getError();
		
			TF1* fQED4frac = new TF1("fQED4frac", "[0] + [1]*x + [2]*x*x +[3]*x*x*x", 0, 5);
			fQED4frac ->SetParameters( cQEDPdf.mP0.getVal(), cQEDPdf.mP1.getVal(), cQEDPdf.mP2.getVal(), cQEDPdf.mP3.getVal() );

			const double  fracQED  = fQED4frac->Integral(mJpsiMassLow, mJpsiMassHi) / fQED4frac->Integral(massLow4Fit, massHig4Fit);
			
			const double nQEDinJpsi     = nQED.getVal()  *fracQED;
			const double nQEDinJpsiErr  = nQED.getError()*fracQED;

			c1->cd();
			c1->SetLogy(0);

			int nFrameMBins  = (massHig4Fit - massLow4Fit)/hMass->GetBinWidth(1);
			RooPlot *frameMass = mMass.frame(Range(massLow4Fit, massHig4Fit), Title(""), Bins(nFrameMBins));
			
			//frameMass ->GetYaxis()->SetTitleSize(0.10);
			frameMass ->GetYaxis()->SetTitleOffset(0.90);
			dataMass  .plotOn(frameMass, MarkerStyle(20), MarkerSize(1), MarkerColor(1), LineColor(1), LineWidth(2), DrawOption("pz"));
			totMassPdf.plotOn(frameMass, LineColor(2), LineStyle(1), LineWidth(2));
			totMassPdf.plotOn(frameMass, Components(RooArgSet(*jpsiPdf)), LineColor(kBlue),    LineStyle(5), LineWidth(2));
			totMassPdf.plotOn(frameMass, Components(RooArgSet(*psiPdf)),  LineColor(kBlue+2),  LineStyle(6), LineWidth(2));
			totMassPdf.plotOn(frameMass, Components(RooArgSet(*qedPdf)),  LineColor(qedColor), LineStyle(2), LineWidth(3));

			//			cout<<endl;
			//			cout<<"******** Print frame ********"<<endl;
			//			frameMass->Print();
			//			cout<<"******** End ********"<<endl;
			//			cout<<endl;

			double chi2ndf = frameMass->chiSquare("totMassPdf_Norm[mMass]", "h_dataMass", 7);   // Need to change to 9 when using CBG. Jpsi+Psi fit

			frameMass->Draw() ;
			
			TString yName = "";
			if(iy<nDiffRapBins)
			{	
				//different name for symmetric y bins
				if(SymmetricRapBin && iy > (nDiffRapBins/2 - 1))   yName = Form("%1.1f < |y^{#mu#mu}| < %1.1f", mDiffRapLow[iy],             mDiffRapHi[iy]            );
				else 											   yName = Form("%1.1f < y^{#mu#mu} < %1.1f",   mDiffRapLow[iy],             mDiffRapHi[iy]            );
			}
			else                								   yName = Form("%1.1f < |y^{#mu#mu}| < %1.1f", mDiffRapLow[nDiffRapBins/2], mDiffRapHi[nDiffRapBins-1]);

			const TString ptName = Form("%1.0f < p_{T}^{#mu#mu} < %1.1f GeV",  fabs(ptLow4Fit), ptHig4Fit);

			drawLatex(0.15, 0.86, nCasesName[i_ncase], mTextFont, 0.06, mTextColor);
			drawLatex(.55, 0.86, yName,                mTextFont, 0.05, mTextColor);
			drawLatex(.55, 0.80, ptName,               mTextFont, 0.05, mTextColor);
			drawLatex(.15, 0.75, Form("#chi^{2}/ndf = %1.1f", chi2ndf),                               mTextFont, mTextSize, mTextColor);
			const double textDy = 0.05;
			drawLatex(.55, 0.66+textDy, Form("N_{J/#psi} = %d #pm %d",   TMath::Nint(nJpsi.getVal()), TMath::Nint(nJpsi.getError())), mTextFont, mTextSize, mTextColor);
			drawLatex(.55, 0.61+textDy, Form("N_{#psi(2S)} = %d #pm %d", TMath::Nint(nPsi.getVal()),  TMath::Nint(nPsi.getError()) ), mTextFont, mTextSize, mTextColor);
			drawLatex(.55, 0.55+textDy, Form("N_{QED} = %d #pm %d",      TMath::Nint(nQED.getVal()),  TMath::Nint(nQED.getError()) ), mTextFont, mTextSize, mTextColor);
			drawLatex(.55, 0.48+textDy, Form("N^{in J/#psi}_{QED} = %d #pm %d", (int)nQEDinJpsi, (int)nQEDinJpsiErr ), mTextFont, mTextSize, mTextColor);

			//----------------------------------------------------------------------------------------------------------------------------------------------------
			if(SymmetricRapBin && iy > (nDiffRapBins/2 - 1) && iy < nDiffRapBins) c1->SaveAs( Form("outplots/massSpec_4ptFitConstrain_"+nCasesName[i_ncase]+"_iy%d_Symm.pdf",  iy) );
			else c1->SaveAs( Form("outplots/massSpec_4ptFitConstrain_"+nCasesName[i_ncase]+"_iy%d.pdf",  iy) );
			//----------------------------------------------------------------------------------------------------------------------------------------------------
		
			//------------------------------------------------------------------------------------------------------------------------------------------
			//calculate the fiting pull: (Data-FitCurve)/#Sigma_{Data}
			c1->cd();
			RooHist *hpull_mass = frameMass->pullHist("h_dataMass", "totMassPdf_Norm[mMass]"); //"totMassPdf_Norm[mMass]", "h_dataMass"
			hpull_mass ->SetMarkerStyle(24);
			hpull_mass ->SetMarkerSize(0.8);
			hpull_mass ->SetMarkerColor(1);
			hpull_mass ->SetLineColor(1);
			hpull_mass ->SetLineWidth(1);

			RooPlot *frameMassPull = mMass.frame( Range(massLow4Fit, massHig4Fit), Title(""), Bins(nFrameMBins) );
			frameMassPull ->addPlotable(hpull_mass, "pz");
			frameMassPull ->GetYaxis()->SetRangeUser(-10.0, 10.0);
			frameMassPull ->SetYTitle("(Data-Fit)/(#sigma_{Data})");
			frameMassPull ->SetXTitle("M_{#mu#mu} (GeV)");
			frameMassPull ->GetYaxis()->CenterTitle();
			//frameMassPull ->GetYaxis()->SetNdivisions(6);
			frameMassPull ->GetYaxis()->SetTitleSize(0.07);
			frameMassPull ->GetYaxis()->SetTitleOffset(0.70);
			frameMassPull ->GetYaxis()->SetLabelSize(0.05);
			//frameMassPull ->GetYaxis()->SetLabelFont(20);
			frameMassPull ->GetXaxis()->SetTitleSize(0.05);
			frameMassPull ->GetXaxis()->SetTitleOffset(1.05);
			frameMassPull ->GetXaxis()->SetLabelSize(0.05);
			//frameMassPull ->GetXaxis()->SetLabelFont(40);
			frameMassPull ->SetTickLength(0.04);
			frameMassPull ->Draw() ;
			
			drawLine(massLow4Fit, 0, massHig4Fit, 0, 1, 2, 2);
			drawLatex(0.15, 0.80, "(Mass Fit Pull Hist)", mTextFont, 0.05, mTextColor);
			drawLatex(0.15, 0.86, nCasesName[i_ncase],    mTextFont, 0.06, mTextColor);
			drawLatex(0.55, 0.86, yName,                  mTextFont, 0.05, mTextColor);
			drawLatex(0.55, 0.80, ptName,                 mTextFont, 0.05, mTextColor);
			//----------------------------------------------------------------------------------------------------------------------------------------------------
			// if(SymmetricRapBin && iy > (nDiffRapBins/2 - 1) && iy < nDiffRapBins) 
			// 	c1->SaveAs( Form("outplots/massSpecFitPull_4ptFitConstrain_"+nCasesName[i_ncase]+"_iy%d_Symm.png",  iy) );
			// else 
			// 	c1->SaveAs( Form("outplots/massSpecFitPull_4ptFitConstrain_"+nCasesName[i_ncase]+"_iy%d.png",  iy) );
			//----------------------------------------------------------------------------------------------------------------------------------------------------

			delete ResFit;
			delete hMass;
			delete frameMass;
			delete frameMassPull;

			//------------------------------------------------------------------------------------------------------------
			//Fit PT Spectral to get fI factor, We only have CohJpsi for 0nXn, XnXn,
			//------------------------------------------------------------------------------------------------------------
			TH1D* hCohJpsiPtHist, *hInCohJpsiPtHist, *hFeeddownJpsiPtHist, *hQEDPtHist;
			if( i_ncase==0 || i_ncase==1)
			{
				if(iy==nDiffRapBins) hCohJpsiPtHist = (TH1D *)inf_Temps->Get( "hCohJpsiPt" );
				else                 hCohJpsiPtHist = (TH1D *)inf_Temps->Get( Form("hCohJpsiPt_RapBin%d", iy) );
			}
			else if(i_ncase==4 || i_ncase==3)
			{	
				//use 0nXn Temps for the OnXnSum case  
				if(iy==nDiffRapBins) hCohJpsiPtHist = (TH1D *)inf_Temps->Get(      "hCohJpsi_"+nCasesName[2]+"Pt");
				else                 hCohJpsiPtHist = (TH1D *)inf_Temps->Get( Form("hCohJpsi_"+nCasesName[2]+"Pt_RapBin%d", iy) );
			}
			else
			{
				if(iy==nDiffRapBins) hCohJpsiPtHist = (TH1D *)inf_Temps->Get(      "hCohJpsi_"+nCasesName[i_ncase]+"Pt");
				else                 hCohJpsiPtHist = (TH1D *)inf_Temps->Get( Form("hCohJpsi_"+nCasesName[i_ncase]+"Pt_RapBin%d", iy) );
			}


			if(iy==nDiffRapBins)
			{
				hInCohJpsiPtHist    = (TH1D *)inf_Temps->Get( "hInCohJpsiPt"         );
				hFeeddownJpsiPtHist = (TH1D *)inf_Temps->Get( "hCohPsi2SFeeddownPt"  );
				hQEDPtHist          = (TH1D *)inf_Temps->Get( "hLowMassGammaGammaPt" );
			}
			else
			{
				hInCohJpsiPtHist    = (TH1D *)inf_Temps->Get( Form("hInCohJpsiPt_RapBin%d",         iy) );
				hFeeddownJpsiPtHist = (TH1D *)inf_Temps->Get( Form("hCohPsi2SFeeddownPt_RapBin%d",  iy) );
				hQEDPtHist          = (TH1D *)inf_Temps->Get( Form("hLowMassGammaGammaPt_RapBin%d", iy) );
			}
			//--------------------------------------------------------------------------------------
			
			//--------------------------------------------------------------------------------------
			//Rebin MC templates to be same as data Pt 
			hCohJpsiPtHist       ->RebinX(hPt->GetBinWidth(1)/hCohJpsiPtHist->GetBinWidth(1));
			hInCohJpsiPtHist     ->RebinX(hPt->GetBinWidth(1)/hInCohJpsiPtHist->GetBinWidth(1));
			hFeeddownJpsiPtHist  ->RebinX(hPt->GetBinWidth(1)/hFeeddownJpsiPtHist->GetBinWidth(1));
			hQEDPtHist           ->RebinX(hPt->GetBinWidth(1)/hQEDPtHist->GetBinWidth(1));
//
//			if( i_ncase ==1 )
//			{
//				hCohJpsiPtHist       = (TH1D*) hCohJpsiPtHist      ->Rebin(nPtBDs40n0n, Form("hCohJpsiPtHist_Rebin_incase%d_iy%d",     i_ncase,iy), PtBDs40n0n);
//				hInCohJpsiPtHist     = (TH1D*) hInCohJpsiPtHist    ->Rebin(nPtBDs40n0n, Form("hInCohJpsiPtHist_Rebin_incase%d_iy%d",   i_ncase,iy), PtBDs40n0n);
//				hFeeddownJpsiPtHist  = (TH1D*) hFeeddownJpsiPtHist ->Rebin(nPtBDs40n0n, Form("hFeeddownJpsiPtHist_Rebin_incase%d_iy%d",i_ncase,iy), PtBDs40n0n);
//				hQEDPtHist           = (TH1D*) hQEDPtHist          ->Rebin(nPtBDs40n0n, Form("hQEDPtHist_Rebin_incase%d_iy%d",         i_ncase,iy), PtBDs40n0n);
//			}
//			else if( i_ncase == 5 )
//			{
//				hCohJpsiPtHist       = (TH1D*) hCohJpsiPtHist      ->Rebin(nPtBDs4XnXn, Form("hCohJpsiPtHist_Rebin_incase%d_iy%d",     i_ncase,iy), PtBDs4XnXn);
//				hInCohJpsiPtHist     = (TH1D*) hInCohJpsiPtHist    ->Rebin(nPtBDs4XnXn, Form("hInCohJpsiPtHist_Rebin_incase%d_iy%d",   i_ncase,iy), PtBDs4XnXn);
//				hFeeddownJpsiPtHist  = (TH1D*) hFeeddownJpsiPtHist ->Rebin(nPtBDs4XnXn, Form("hFeeddownJpsiPtHist_Rebin_incase%d_iy%d",i_ncase,iy), PtBDs4XnXn);
//				hQEDPtHist           = (TH1D*) hQEDPtHist          ->Rebin(nPtBDs4XnXn, Form("hQEDPtHist_Rebin_incase%d_iy%d",         i_ncase,iy), PtBDs4XnXn);
//			}
//			else
//			{
//				hCohJpsiPtHist       = (TH1D*) hCohJpsiPtHist      ->Rebin(nPtBDs4AnAn, Form("hCohJpsiPtHist_Rebin_incase%d_iy%d",     i_ncase,iy), PtBDs4AnAn);
//				hInCohJpsiPtHist     = (TH1D*) hInCohJpsiPtHist    ->Rebin(nPtBDs4AnAn, Form("hInCohJpsiPtHist_Rebin_incase%d_iy%d",   i_ncase,iy), PtBDs4AnAn);
//				hFeeddownJpsiPtHist  = (TH1D*) hFeeddownJpsiPtHist ->Rebin(nPtBDs4AnAn, Form("hFeeddownJpsiPtHist_Rebin_incase%d_iy%d",i_ncase,iy), PtBDs4AnAn);
//				hQEDPtHist           = (TH1D*) hQEDPtHist          ->Rebin(nPtBDs4AnAn, Form("hQEDPtHist_Rebin_incase%d_iy%d",         i_ncase,iy), PtBDs4AnAn);
//			}
//
			//--------------------------------------------------------------------------------------
			
			RooRealVar mPt("mPt", "p_{T} (GeV)", ptLow4Fit, ptHig4Fit);
	
			//--------------------------------------------------------------------------------------
			RooDataHist hCohJpsiPtRooHist(     "hCohJpsiPtRooHist",      "hCohJpsiPtRooHist",      mPt, hCohJpsiPtHist);
			RooHistPdf  cohJpsiPdf(            "cohJpsiPdf",             "cohJpsiPdf",             mPt, hCohJpsiPtRooHist,      0);
			//--------------------------------------------------------------------------------------

			//--------------------------------------------------------------------------------------
			RooDataHist hInCohJpsiPtRooHist(   "hInCohJpsiPtRooHist",    "hInCohJpsiPtRooHist",    mPt, hInCohJpsiPtHist);
			RooHistPdf  incohJpsiPdf(          "incohJpsiPdf",           "incohJpsiPdf",           mPt, hInCohJpsiPtRooHist,    0);
			//--------------------------------------------------------------------------------------

			//--------------------------------------------------------------------------------------
			RooDataHist hFeeddownJpsiPtRooHist("hFeeddownJpsiPtRooHist", "hFeeddownJpsiPtRooHist", mPt, hFeeddownJpsiPtHist);
			RooHistPdf  feeddownJpsiPdf(       "feeddownJpsiPdf",        "feeddownJpsiPdf",        mPt, hFeeddownJpsiPtRooHist, 0);
			//--------------------------------------------------------------------------------------

			////--------------------------------------------------------------------------------------
			////------------------------gammagamma-->mumu--------------------------------------------------------------
			////--------------------------------------------------------------------------------------
			//if use the simulated tempaltes for gammagamma-->mumu
			RooDataHist hQEDPtRooHist("hQEDPtRooHist", "hQEDPtRooHist", mPt, hQEDPtHist);
			RooHistPdf  qedPtPdf(     "qedPtPdf",      "qedPtPdf",      mPt, hQEDPtRooHist, 0);

			//if use the Side Bands as tempaltes for gammagamma-->mumu
			// RooDataHist hQEDPtRooHist("hQEDPtRooHist", "hQEDPtRooHist", mPt, hPt_SdB);
			// RooHistPdf  qedPtPdf(     "qedPtPdf",      "qedPtPdf",      mPt, hQEDPtRooHist, 0);

			//RooConstVar bpd("bpd", "bpd", 1.79);
			//RooConstVar npd("npd", "npd", 3.58);
			RooRealVar bpd("bpd", "bpd", 1.79, 0, 5.0);
			RooRealVar npd("npd", "npd", 3.58, 0, 10.);
			RooGenericPdf *dissoJpsiPdf = new RooGenericPdf("dissoJpsiPdf", "dissoJpsiPdf", "mPt*TMath::Power(1+(bpd/npd)*mPt*mPt, -npd)", RooArgSet(mPt, bpd, npd));
		
			const int    higBin_FDW = hFeeddownJpsiPtHist->FindBin( mPtCut4Coh );
			const int    higBin_Coh = hCohJpsiPtHist     ->FindBin( mPtCut4Coh );

			const double frac_FDW = hFeeddownJpsiPtHist  ->Integral(1, higBin_FDW) / hFeeddownJpsiPtHist->Integral(1, hFeeddownJpsiPtHist->GetNbinsX());
			const double frac_Coh = hCohJpsiPtHist       ->Integral(1, higBin_Coh) / hCohJpsiPtHist     ->Integral(1, hCohJpsiPtHist->GetNbinsX()     );

			const double fDValue = fD_inPtCut[i_ncase][iy]*(frac_Coh/frac_FDW); //need to rescale to full pt fD*(pdf_feeddown->Integral()/pdf_cohJpsi->Integral())

			double nInCohJpsiMax = nJpsiValue;
			double nDissoJpsiMax = nJpsiValue;

			if 		(i_ncase == 1)									{ nInCohJpsiMax *= 0.1;	nDissoJpsiMax *= 0.03; }
			else if	(i_ncase == 0)									{ nInCohJpsiMax *= 0.2;	nDissoJpsiMax *= 0.2; }
			else 													{ nInCohJpsiMax *= 0.3;	nDissoJpsiMax *= 0.3; }

			RooConstVar fracPrim(     "fracPrim",       "fracPrim",      1./(1.+fDValue));
			RooRealVar  nCohJpsi_wFDW("nCohJpsi_wFDW",  "nCohJpsi_wFDW", nJpsiValue*0.80, 0, nJpsiValue);
			RooRealVar  nInCohJpsi(   "nInCohJpsi",     "nInCohJpsi",    nJpsiValue*0.04, 0, nInCohJpsiMax);
			RooRealVar  nDissoJpsi(   "nDissoJpsi",     "nDissoJpsi",    nJpsiValue*0.06, 0, nDissoJpsiMax);
			RooConstVar nQEDBg(       "nQEDBg",         "nQEDBg",        nQEDinJpsi    );
			
			RooDataHist dataPt("dataPt", "dataPt", mPt, hPt); 
			
			RooAddPdf Pdf_CohJpsi_wFDW("Pdf_CohJpsi_wFDW", "cohJpsiPdf+feeddownJpsiPdf", RooArgList(cohJpsiPdf,feeddownJpsiPdf), fracPrim);

			RooAddPdf totPtPdf("totPtPdf", "totPtPdf", 
					RooArgList( Pdf_CohJpsi_wFDW, incohJpsiPdf, *dissoJpsiPdf, qedPtPdf ),
					RooArgList( nCohJpsi_wFDW,    nInCohJpsi,    nDissoJpsi,   nQEDBg)  );
					// RooArgList( Pdf_CohJpsi_wFDW,  *dissoJpsiPdf, qedPtPdf ),
					// RooArgList( nCohJpsi_wFDW,        nDissoJpsi,   nQEDBg)  );
			
			totPtPdf.fitTo(dataPt,Extended(kTRUE),SumW2Error(kTRUE),Hesse(kTRUE),Minos(kFALSE),Save());
			
			//calcualte numbers
			const double N_CohJpsi       = nCohJpsi_wFDW.getVal()  *fracPrim.getVal();
			const double Nerr_CohJpsi    = nCohJpsi_wFDW.getError()*fracPrim.getVal();
			const double N_InCohJpsi     = nInCohJpsi.getVal();
			const double Nerr_InCohJpsi  = nInCohJpsi.getError();
			const double N_DissoJpsi     = nDissoJpsi.getVal();
			const double Nerr_DissoJpsi  = nDissoJpsi.getError();
			
			//calcualte number of coherent Jpsi within pt<0.20 GeV/c region
			const double N_CohJpsi_inPtCut    = N_CohJpsi   *frac_Coh;
			const double Nerr_CohJpsi_inPtCut = Nerr_CohJpsi*frac_Coh;

			//calculate number of incoherent Jpsi within pt<0.20 GeV/c
			const int    higBin_InCoh         = hInCohJpsiPtHist ->FindBin( mPtCut4Coh );
			const double frac_InCoh           = hInCohJpsiPtHist ->Integral(1, higBin_InCoh) / hInCohJpsiPtHist->Integral(1, hInCohJpsiPtHist->GetNbinsX());
			const double N_InCoh_inPtCut      = N_InCohJpsi    * frac_InCoh;
			const double Nerr_InCoh_inPtCut   = Nerr_InCohJpsi * frac_InCoh;
			// const double N_InCoh_inPtCut      = 0;
			// const double Nerr_InCoh_inPtCut   = 0;
			
			//calculate number of incoherent Jpsi with n-disso within pt<0.20 GeV/c
			mPt.setRange("CohSignal", 0., mPtCut4Coh );
			RooAbsReal* frac_disso_inPtCut       = dissoJpsiPdf->createIntegral( mPt, NormSet(mPt), Range("CohSignal") );
			const double fracValue_disso_inPtCut = frac_disso_inPtCut->getVal();
			const double N_disso_inPtCut    = N_DissoJpsi   *fracValue_disso_inPtCut;
			const double Nerr_disso_inPtCut = Nerr_DissoJpsi*fracValue_disso_inPtCut;

			const double fI_Value = (N_InCoh_inPtCut + N_disso_inPtCut)/N_CohJpsi_inPtCut;
			const double fI_Error = sqrt( 
					pow(Nerr_InCoh_inPtCut + Nerr_disso_inPtCut,2) / pow(N_InCoh_inPtCut+N_disso_inPtCut,2) 
					+ pow(Nerr_CohJpsi_inPtCut/N_CohJpsi_inPtCut,2)
					)
				*fI_Value;
			

			//ALICE method, where 
			//Double_t f_I   =  (N_I     + N_diss     ) / (N_coh2);
			//Double_t ErrfI = sqrt((N_IError + N_dissError)*(N_IError + N_dissError) / ((N_I + N_diss)*(N_I + N_diss)) + (N_cohError*N_cohError)/(N_coh2*N_coh2) )*f_I;
			//const double 
			
			//calculate expected coherent Jpsi number = N Jpsi from mass fit / (1+fD+fI)
			const double NJpsi_Coh_cal    = NJpsi_inMFit[i_ncase][iy]    / (1. + fD_inPtCut[i_ncase][iy] + fI_Value);
			const double NerrJpsi_Coh_cal = NerrJpsi_inMFit[i_ncase][iy] / (1. + fD_inPtCut[i_ncase][iy] + fI_Value); //need to update with fD, fI uncertainties
			
			const double unit_ub2mb = 1000.;
			const double Lum =  mCMSLum*(unit_ub2mb); //in ub, need to use mb to compare to Alice
			
			//calculate the cross section: xsec = (NJpsiFromMfit/(1+fD+fI))*(1/eff)*(1/BR)*(1/Lum)*(1/dy)
			xsecValue[i_ncase][iy] = NJpsi_Coh_cal    * (1./Eff_CohJpsi[i_ncase][iy]) * (1./br_Jpsi2uu) * (1./Lum) * (1./dY) * (1./Acc_CohJpsi[iy]);
			xsecError[i_ncase][iy] = NerrJpsi_Coh_cal * (1./Eff_CohJpsi[i_ncase][iy]) * (1./br_Jpsi2uu) * (1./Lum) * (1./dY) * (1./Acc_CohJpsi[iy]);

			int nFramePtBins = (ptHig4Fit - ptLow4Fit)/hPt->GetBinWidth(1);
			cout<<nFramePtBins<<endl;

			//RooPlot *framePt = mPt.frame(Range(ptLow4Fit, ptHig4Fit), Title(""), Bins(nFramePtBins));
			RooPlot *framePt = mPt.frame(Range(ptLow4Fit, ptHig4Fit), Title(""));

			dataPt  .plotOn(framePt, MarkerStyle(20), MarkerSize(0.7), MarkerColor(2), LineColor(2), LineWidth(1), DrawOption("pz"));
			totPtPdf.plotOn(framePt, LineColor(1), LineStyle(1), LineWidth(mLineWidth));
			totPtPdf.plotOn(framePt, Components(RooArgSet(cohJpsiPdf)),      LineColor(cohJpsiColor),      LineStyle(cohJpsiStyle),      LineWidth(mLineWidth));
			totPtPdf.plotOn(framePt, Components(RooArgSet(feeddownJpsiPdf)), LineColor(feeddownJpsiColor), LineStyle(feeddownJpsiStyle), LineWidth(mLineWidth));
			totPtPdf.plotOn(framePt, Components(RooArgSet(incohJpsiPdf)),    LineColor(incohJpsiColor),    LineStyle(incohJpsiStyle),    LineWidth(mLineWidth));
			totPtPdf.plotOn(framePt, Components(RooArgSet(*dissoJpsiPdf)),   LineColor(dissoJpsiColor),    LineStyle(dissoJpsiStyle),    LineWidth(mLineWidth));
			totPtPdf.plotOn(framePt, Components(RooArgSet(qedPtPdf)),        LineColor(qedColor),          LineStyle(qedStyle),          LineWidth(mLineWidth));

			cout<<endl;
			cout<<"******** Print frame ********"<<endl;
			framePt->Print();
			cout<<"******** End ********"<<endl;
			cout<<endl;
			
			// chi2ndf = framePt->chiSquare("totPtPdf_Norm[mPt]", "h_dataPt", 5);
			// chi2ndf = framePt->chiSquare("totPtPdf_Norm[mPt]", "h_dataPt", 9);

			int nNonZeroBinPt = 0;
			for (int ibinPt = hPt->FindBin(ptLow4Fit); ibinPt < hPt->FindBin(ptHig4Fit)+1; ibinPt++)
			{
				if (hPt->GetBinContent(ibinPt)) nNonZeroBinPt++;
			}
			const double Pt_ndf = nNonZeroBinPt -5;
			const double Pt_chi2 = totPtPdf.createChi2(dataPt, Range(ptLow4Fit, ptHig4Fit),
                 Extended(true), DataError(RooAbsData::Poisson))->getVal();
			cout<<"NDF counted: "<< Pt_ndf<<endl;
	
			c2 -> Divide(1,2);
			c2 -> cd(1);
			gPad->SetPad(0.0,0.25,1.0,0.96);
			gPad->SetBottomMargin(0);
			gPad->SetRightMargin(0.05);
			gPad->SetTopMargin(0);
			//setPad(0.12, 0.08, 0.07, 0.13);

			//gPad->SetLogy(0);
			//framePt->GetYaxis()->SetRangeUser(0.5, hPt->GetMaximum()*1.3);
			gPad->SetLogy(1);
			gPad->SetLogx(0);
			framePt->GetYaxis()->SetRangeUser(0.5, hPt->GetMaximum()*10.);
			//framePt->SetYTitle(Form("Events / (%.2f GeV)", hPt->GetBinWidth(1)));
			framePt->SetYTitle("dN/dp_{T} (GeV^{-1})");
			framePt->Draw() ;
			drawLatex(0.15, 0.92, "CMS Pb-Pb #sqrt{s_{NN}} = 5.02 TeV UPC ("+nCasesName[i_ncase]+")",  42,        0.06,      mTextColor );
			drawLatex(0.62, 0.82, yName,                                                                 mTextFont, 0.05,      mTextColor );
			const double textDy2 = 0.05;
			drawLatex(0.25, 0.80+textDy2, Form("For p_{T}<%.2f GeV:",              mPtCut4Coh),                                   mTextFont, 0.035, mTextColor);
			drawLatex(0.25, 0.75+textDy2, Form("N^{Coh}_{J/#psi} = %d #pm %d",  (int)N_CohJpsi_inPtCut, (int)Nerr_CohJpsi_inPtCut), mTextFont, 0.035, mTextColor);
			drawLatex(0.25, 0.70+textDy2, Form("f_{I} = (N^{All}_{InCoh}/N^{Coh}_{J/#psi}) = %.3f #pm %.3f", fI_Value, fI_Error),     mTextFont, 0.035, mTextColor);
			drawLatex(0.25, 0.65+textDy2, Form("N^{in Mfit}_{J/#psi}/(1+f_{I}+f_{D}) = %d #pm %d",  (int)NJpsi_Coh_cal, (int)NerrJpsi_Coh_cal), mTextFont, 0.035, mTextColor);
			drawLatex(0.25, 0.57+textDy2, Form("#frac{d#sigma^{Coh}_{J/#psi}}{dy} = %.3f #pm %.3f (mb)", xsecValue[i_ncase][iy], xsecError[i_ncase][iy] ),                                     mTextFont, 0.035, mTextColor);

			TLegend  *leg =  new TLegend(0.60, 0.45, 0.88, 0.80);
			leg->SetFillStyle(0);
			leg->SetFillColor(0);
			leg->SetTextFont(mTextFont);
			leg->SetTextSize(0.02);
			leg->AddEntry(framePt->findObject("h_dataPt"),            "Data",        "pe");
			leg->AddEntry(framePt->findObject("totPtPdf_Norm[mPt]"),   Form("Fit: #chi^{2}/ndf = %1.1f/%1.f = %1.1f", Pt_chi2, Pt_ndf, Pt_chi2/Pt_ndf),   "l");
			//(RooHist::h_dataPt,RooCurve::totPtPdf_Norm[mPt],RooCurve::totPtPdf_Norm[mPt]_Comp[cohJpsiPdf],RooCurve::totPtPdf_Norm[mPt]_Comp[feeddownJpsiPdf],RooCurve::totPtPdf_Norm[mPt]_Comp[incohJpsiPdf],RooCurve::totPtPdf_Norm[mPt]_Comp[dissoJpsiPdf],RooCurve::totPtPdf_Norm[mPt]_Comp[qedPtPdf])
			const TString curveName[5]  = {"totPtPdf_Norm[mPt]_Comp[cohJpsiPdf]","totPtPdf_Norm[mPt]_Comp[incohJpsiPdf]", "totPtPdf_Norm[mPt]_Comp[dissoJpsiPdf]", "totPtPdf_Norm[mPt]_Comp[feeddownJpsiPdf]","totPtPdf_Norm[mPt]_Comp[qedPtPdf]"};
			const TString curveTitle[5] = {"Coherent J/#psi", "Incoherent J/#psi w/o N dissociation", "Incoherent J/#psi w. N dissociation", "Coherent #psi(2S) #rightarrow J/#psi+X", "(QED continuum) #gamma#gamma #rightarrow #mu#mu"};
			for(int icv=0; icv<5; icv++) 
			{
				//if(icv==0||icv==3) continue;
				leg->AddEntry(framePt->findObject( curveName[icv]), curveTitle[icv], "l");
			}
			//----------------------- Removing Incoherent Jpsi template -----------------------
			// const TString curveName[5]  = {"totPtPdf_Norm[mPt]_Comp[cohJpsiPdf]", "totPtPdf_Norm[mPt]_Comp[dissoJpsiPdf]", "totPtPdf_Norm[mPt]_Comp[feeddownJpsiPdf]","totPtPdf_Norm[mPt]_Comp[qedPtPdf]"};
			// const TString curveTitle[5] = {"Coherent J/#psi",  "Incoherent J/#psi with disso.", "Coherent #psi' #rightarrow J/#psi+X", "#gamma#gamma #rightarrow #mu#mu"};
			// for(int icv=0; icv<4; icv++) 
			// {
			// 	//if(icv==0||icv==3) continue;
			// 	leg->AddEntry(framePt->findObject( curveName[icv]), curveTitle[icv], "l");
			// }
			//----------------------- END ----------------------------------------------

			leg->Draw("same");

			c2	->cd(2);
			gPad->SetPad(0.0,0.0,1.0,0.25);
			gPad->SetTopMargin(0);
			gPad->SetRightMargin(0.05);
			gPad->SetBottomMargin(0.45);

			//calculate the fiting pull: (Data-FitCurve)/#Sigma_{Data}
			RooHist *hpull_pt = framePt->pullHist("h_dataPt", "totPtPdf_Norm[mPt]");
			hpull_pt ->SetMarkerStyle(24);
			hpull_pt ->SetMarkerSize(0.6);
			hpull_pt ->SetMarkerColor(1);
			hpull_pt ->SetLineColor(1);
			hpull_pt ->SetLineWidth(1);

			//RooPlot *framePtPull = mPt.frame( Range(ptLow4Fit, ptHig4Fit), Title(""), Bins(nFramePtBins) );
			RooPlot *framePtPull = mPt.frame( Range(ptLow4Fit, ptHig4Fit), Title(""));
			framePtPull ->addPlotable(hpull_pt, "pz");
			framePtPull ->GetYaxis()->SetRangeUser(-14.0, 14.0);
			framePtPull ->SetYTitle("#frac{Data-Fit}{#sigma_{Data}}");
			framePtPull ->SetXTitle("p_{T} (GeV)");
			framePtPull ->GetYaxis()->CenterTitle();
			framePtPull ->GetYaxis()->SetNdivisions(6);
			framePtPull ->GetYaxis()->SetTitleSize(0.15);
			framePtPull ->GetYaxis()->SetTitleOffset(0.33);
			framePtPull ->GetYaxis()->SetLabelSize(0.10);
			//framePtPull ->GetYaxis()->SetLabelFont(20);
			framePtPull ->GetXaxis()->SetTitleSize(0.20);
			framePtPull ->GetXaxis()->SetTitleOffset(0.95);
			framePtPull ->GetXaxis()->SetLabelSize(0.16);
			framePtPull ->GetXaxis()->SetLabelFont(40);
			framePtPull ->SetTickLength(0.08);
			framePtPull ->Draw() ;
			
			//	TH2D* htem2d_4pt = new TH2D("htem2d_4pt", "", nFramePtBins, ptLow, ptHi, 10, -0.001, 0.001);
			//	htem2d_4pt->SetYTitle("Data/Fit");
			//	htem2d_4pt->SetXTitle("p_{T} (GeV/c)");
			//	htem2d_4pt->GetYaxis()->SetNdivisions(4);
			//	htem2d_4pt->GetYaxis()->SetTitleSize(0.22);
			//	htem2d_4pt->GetYaxis()->SetTitleOffset(0.25);
			//	htem2d_4pt->GetYaxis()->SetLabelSize(0.15);
			//	htem2d_4pt->GetYaxis()->SetLabelFont(40);
			//	htem2d_4pt->GetXaxis()->SetTitleSize(0.20);
			//	htem2d_4pt->GetXaxis()->SetTitleOffset(0.95);
			//	htem2d_4pt->GetXaxis()->SetLabelSize(0.16);
			//	htem2d_4pt->GetXaxis()->SetLabelFont(40);
			//	htem2d_4pt->SetTickLength(0.08);
			//	htem2d_4pt->Draw() ;
			//
			//	histPull_pt->Draw("lsame");
			//	histPull_pt->Draw("pesame");
			//
			if(SymmetricRapBin && iy > (nDiffRapBins/2 - 1) && iy < nDiffRapBins){
				c2->SaveAs( Form("outplots/ptSpec_4decouple_"+nCasesName[i_ncase]+"_iy%d_Symm.png",  iy) );
				c2->SaveAs( Form("outplots/ptSpec_4decouple_"+nCasesName[i_ncase]+"_iy%d_Symm.pdf",  iy) );
			}
			else{
				c2->SaveAs( Form("outplots/ptSpec_4decouple_"+nCasesName[i_ncase]+"_iy%d.png",  iy) );
				c2->SaveAs( Form("outplots/ptSpec_4decouple_"+nCasesName[i_ncase]+"_iy%d.pdf",  iy) );
			}

			c2 -> Clear();

			//---------------------------------------------------------------------------------------------------------------------------------------------------------
			//---------------------------------------------------------------------------------------------------------------------------------------------------------
			
			if(draw4Paper_flag==1)
			{
				c3->cd();
				gPad->SetLogy(1);
				
				framePt->GetYaxis()->SetRangeUser(0.90, hPt->GetMaximum()*3.);
				//framePt->SetYTitle(Form("dN/dp_{T} (GeV^{-1})"));
				framePt->SetYTitle(Form("Events / (%.2f GeV)", hPt->GetBinWidth(1)));
				framePt->GetXaxis()->SetTitleOffset(0.95);
				framePt->Draw() ;
				//drawLatex(0.69, 0.945, "#bf{CMS} #it{Preliminary}",                                    42,        0.045,     mTextColor );
				//drawLatex(0.16, 0.86, "Pb-Pb #sqrt{s_{NN}} = 5.02 TeV UPC ("+nCasesName[i_ncase]+")",  42,        0.055,     mTextColor );
				drawLatex(0.12, 0.950, "#bf{CMS}" ,     42,        0.050,     mTextColor );
				
				drawLatex(0.53, 0.950, "PbPb 1.52 nb^{-1} (5.02 TeV)",   42, 0.05,  1);
				drawLatex(0.18, 0.750, nCasesName[i_ncase],   42, 0.06,  1);
				//drawLatex(0.40, 0.950, "Pb-Pb #sqrt{s_{NN}} = 5.02 TeV UPC ("+nCasesName[i_ncase]+")",     42,        0.045,     mTextColor );
				drawLatex(0.18, 0.86,  yName,                                                           mTextFont,  0.050,      mTextColor );
				drawLatex(0.53, 0.86,  Form("%.2f < m_{#mu#mu} < %.2f GeV", mJpsiMassLow, mJpsiMassHi), mTextFont,  0.050,      mTextColor );

				//drawLatex(0.18, 0.70+textDy2, Form("For p_{T} < %.2f GeV/c:",              mPtCut4Coh),                                   mTextFont, 0.035, mTextColor);
				//drawLatex(0.18, 0.65+textDy2, Form("f_{I} = (N^{All}_{InCoh}/N^{Coh}_{J/#psi}) = %.3f #pm %.3f", fI_Value, fI_Error),     mTextFont, 0.035, mTextColor);
				//drawLatex(0.18, 0.55+textDy2, Form("#frac{#sigma^{Coh}_{J/#psi}}{dy} = %.3f #pm %.3f (mb)", xsecValue[i_ncase][iy], xsecError[i_ncase][iy] ),                                     mTextFont, 0.035, mTextColor);

				//TLegend  *leg_4paper =  new TLegend(0.37, 0.49, 0.72, 0.82);
				TLegend  *leg_4paper =  new TLegend(0.42, 0.49, 0.70, 0.82);
				leg_4paper->SetFillStyle(0);
				leg_4paper->SetFillColor(0);
				leg_4paper->SetTextFont(mTextFont);
				leg_4paper->SetTextSize(0.042);
				leg_4paper->AddEntry(framePt->findObject("h_dataPt"),            "Data",        "pe");
				leg_4paper->AddEntry(framePt->findObject("totPtPdf_Norm[mPt]"),   Form("Fit: #chi^{2}/ndf = %1.1f", Pt_chi2/Pt_ndf),   "l");
				
				//const TString curveName[5]  = {"totPtPdf_Norm[mPt]_Comp[cohJpsiPdf]","totPtPdf_Norm[mPt]_Comp[incohJpsiPdf]", "totPtPdf_Norm[mPt]_Comp[dissoJpsiPdf]", "totPtPdf_Norm[mPt]_Comp[feeddownJpsiPdf]","totPtPdf_Norm[mPt]_Comp[qedPtPdf]"};
				//const TString curveTitle[5] = {"Coherent J/#psi", "Incoherent J/#psi", "Incoherent J/#psi with disso.", "Coherent #psi' #rightarrow J/#psi+X", "#gamma#gamma #rightarrow #mu#mu"};
				for(int icv=0; icv<5; icv++) 
				{
					//if(icv==0||icv==3) continue;
					leg_4paper->AddEntry(framePt->findObject( curveName[icv]), curveTitle[icv], "l");
				}

				leg_4paper->Draw("same");

				if(SymmetricRapBin && iy > (nDiffRapBins/2 - 1) && iy < nDiffRapBins)
				{
					c3->SaveAs( Form("outplots/ptSpec_4decouple_"+nCasesName[i_ncase]+"_iy%d_Symm_4paper.png",  iy) );
					c3->SaveAs( Form("outplots/ptSpec_4decouple_"+nCasesName[i_ncase]+"_iy%d_Symm_4paper.pdf",  iy) );
				}
				else
				{
					c3->SaveAs( Form("outplots/ptSpec_4decouple_"+nCasesName[i_ncase]+"_iy%d_4paper.png",  iy) );
					c3->SaveAs( Form("outplots/ptSpec_4decouple_"+nCasesName[i_ncase]+"_iy%d_4paper.pdf",  iy) );
				}

				c3 -> Clear();

			}//draw for paper

			delete hCohJpsiPtHist;
			delete hInCohJpsiPtHist;
			delete hFeeddownJpsiPtHist;
			delete hQEDPtHist;
		}//iy
	}//ixn
	delete c2;

	inf_Temps		->Close();
	cout << "End of program !" << endl;
}

void calSec(){

	for (int i_ncase = 0; i_ncase < NnCases; i_ncase++)
	{
		cout << "i_ncase: " << i_ncase << endl;

		for (int iy = 0; iy < nDiffRapBins + 1; iy++)
		{
			if(iy==nDiffRapBins)                           continue; //temperory to skip all y added fitting 
			if(SymmetricRapBin && iy < nDiffRapBins/2 )    continue; //tem skip
			
			double dY = mDiffRapHi[iy] - mDiffRapLow[iy]; //need to be updated if use for 1.6<|y|<2.4
			if(SymmetricRapBin && iy > (nDiffRapBins/2 -1) && iy<nDiffRapBins) dY *= 2;

			cout<<"iy: "<<iy<<" "<<mDiffRapLow[iy]<<" <y< "<<mDiffRapHi[iy]<<endl;

			const double unit_ub2mb = 1000.;
			const double Lum =  mCMSLum*(unit_ub2mb); //in ub, need to use mb to compare to Alice
			
			cout<<"Production: "<<NJpsi_inMFit[i_ncase][iy] * (1./Eff_CohJpsi[i_ncase][iy])* (1./Acc_CohJpsi[iy])<<endl;

			//calculate the cross section: xsec = (NJpsiFromMfit/(1+fD+fI))*(1/eff)*(1/BR)*(1/Lum)*(1/dy)
			xsecValue[i_ncase][iy] = NJpsi_inMFit[i_ncase][iy]    * (1./Eff_CohJpsi[i_ncase][iy]) * (1./br_Jpsi2uu) * (1./Lum) * (1./dY) * (1./Acc_CohJpsi[iy]);
			xsecError[i_ncase][iy] = NerrJpsi_inMFit[i_ncase][iy] * (1./Eff_CohJpsi[i_ncase][iy]) * (1./br_Jpsi2uu) * (1./Lum) * (1./dY) * (1./Acc_CohJpsi[iy]);
		}
	}
}

void saveFile(const TString headerTitle = "Default")
{
	TFile *file_JpsiXsec = new TFile(Form("JpsiXsecValues/JpsiXsec_%s_%dRapBins%s%s%s.root", headerTitle.Data(), nDiffRapBins, template_Name[template_option].Data(), TnPcases[RunTnPcase].Data(), HFcases[RunHFcase].Data()), "recreate");
	cout<<"Saving JpsiXsec into: "<<file_JpsiXsec->GetName()<<endl;

	for(int iCase=0; iCase<RunCase.size(); iCase++)
	{
		TH1D hJpsiXsec( Form("h%s",nCasesName[iCase].Data()), Form("h%s",nCasesName[iCase].Data()), nDiffRapBins+1, mDiffRapBds);
		TH1D hJpsi( Form("hJpsi%s",nCasesName[iCase].Data()), Form("h%s",nCasesName[iCase].Data()), nDiffRapBins+1, mDiffRapBds);
		hJpsiXsec.SetXTitle("y");
		hJpsiXsec.SetYTitle("d#sigma/dy / mb");

		for (int iy = 0; iy < nDiffRapBins; ++iy)
		{
			if(iy==nDiffRapBins) continue; //temperory to skip all y added fitting 
			if(SymmetricRapBin && iy < nDiffRapBins/2 )          continue; //tem skip

			double yMean = (mDiffRapLow[iy]+mDiffRapHi[iy])/2.;
			hJpsiXsec.SetBinContent(hJpsiXsec.FindBin(yMean), xsecValue[iCase][iy]);
			hJpsiXsec.SetBinError(  hJpsiXsec.FindBin(yMean), xsecError[iCase][iy]);
			
//			hJpsi.SetBinContent(hJpsiXsec.FindBin(yMean), NJpsi_inMFit[iCase][iy] * (1./Eff_CohJpsi[iCase][iy])* (1./Acc_CohJpsi[iy]));
//			hJpsi.SetBinError(  hJpsiXsec.FindBin(yMean), NerrJpsi_inMFit[iCase][iy] * (1./Eff_CohJpsi[iCase][iy])* (1./Acc_CohJpsi[iy]));
			hJpsi.SetBinContent(hJpsiXsec.FindBin(yMean), NJpsi_inMFit[iCase][iy]);
			hJpsi.SetBinError(hJpsiXsec.FindBin(yMean), NerrJpsi_inMFit[iCase][iy]);
		}
		hJpsiXsec.Write();
		hJpsi.Write();
	}
	file_JpsiXsec->Close();
}
