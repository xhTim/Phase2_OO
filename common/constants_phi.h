
//##############################################################################################
//# You will never find more perplexing constants than these.
//#		Basically, chronologically, and without saying too much, 
//#		you're screwed. Royally and totally.
//#			- You Know Who
//##############################################################################################


#ifndef CONSTANTS_H
#define CONSTANTS_H

const TString mInFileName = "HIForward_HIRun2023A_16Jan2024_240426_Masked";	// Use OLD HLT Index
const TString mInFileName_ReHLT = "HIForward_HIRun2023A_16Jan2024_240704_Masked_ReHLT";	// Use Default HLT Index
const TString mInFileName_mc = "diMu_ana_mc";
//const TString mInFileName_mc = "CohPhiToKK_mc_STARLIGHT_5p36TeV_2023Run3_240709";	// Use Default HLT Index

const TString MASS_UNIT = "GeV";
const TString ENERGY_UNIT = "GeV";

// ## CMS Constants ##############################################################################

//# Run Condition ################################################################################

const double mPtCut4Coh			= 0.20;

const int mPVFilterIdx			= 3;

const int    mHFVetoOpt			= 0;
const double mHFVetoPlus		= 9.2;
const double mHFVetoMinus		= 8.6;
const double mHFVetoPlus_tight	= 8.0;
const double mHFVetoMinus_tight	= 8.0;
const double mHFVetoPlus_loose	= 10.0;
const double mHFVetoMinus_loose	= 10.0;
std::vector<TString> mHFVetoOptName = {"" , "_HFTight", "_HFLoose"};

const int    mPIDOpt			 = 1;	// 0: no PID, 1: Gaussian Discriminator, 2: curved band cut
const double mPIDThresholdFactor = 10.0;
const std::vector<TString> mPIDOptName = {"", Form("_GausDisc%d", int(mPIDThresholdFactor)), "_CurvedBand"};

TString mInFileName_PID_ka		= Form("../PID/outFiles/dEdx_fit_ka.root");
TString mInFileName_PID_ka_mc	= Form("../PID/outFiles/dEdx_fit_ka_mc.root");
TString mInFileName_PID_pi		= Form("../PID/outFiles/dEdx_fit_pi.root");

// ## Physics Constants ##########################################################################
const double PI					= 3.141592;
const double HBARC				= 0.1973269718;	// GeV * fm

// ## PDG Constants ##############################################################################
const double MUON_MASS			= 0.105658;
const double PION_MASS			= 0.139570;
const double KAON_MASS			= 0.493677;
const double PROTON_MASS		= 0.938272;
const double ELECTRON_MASS		= 0.000511;

const double PHI_MASS_PDG		= 1.019461;
const double PHI_MASS_WIDTH_PDG	= 0.004249;
const double PHI_PDGID			= 333;
const double JPSI_PDGID			= 443;

const double KAON_PDGID			= 321;

// ## Histogram Constants ########################################################################
const Double_t mTinyNum = 1.e-6;
const Int_t    mHistRapBins		= 80;
const Double_t mHistRapLow		= -4;
const Double_t mHistRapHig		= 4;
const Int_t    mHistPtBins		= 1600;
const Double_t mHistPtLow		= 0;
const Double_t mHistPtHig		= 4;
const Int_t    mHistMassBins		= 300;
const Double_t mHistMassLow		= 2;
const Double_t mHistMassHig		= 5;
const Int_t    mHistEtaBins		= 200;
const Double_t mHistEtaLow		= -3.0;
const Double_t mHistEtaHig		= 3.0;
const Int_t    mHistPhiBins		= 100;
const Double_t mHistPhiLow		= -PI;
const Double_t mHistPhiHig		= PI;
const Int_t    mHistPBins		= 200;
const Double_t mHistPLow		= 0.0;
const Double_t mHistPHig		= 1.0;
const Int_t    mHistdEdxBins	= 200;
const Double_t mHistdEdxLow		= 0;
const Double_t mHistdEdxHig		= 15;
const Int_t    mHistLogdEdxBins	= 200;
const Double_t mHistLogdEdxLow	= -2;
const Double_t mHistLogdEdxHig	= 5;
const double mPhiMassLow		= 0.99;	//! Need to be consistent with constant.PHI_MASS_WINDOW_LOW
const double mPhiMassHig		= 1.05;	//! Need to be consistent with constant.PHI_MASS_WINDOW_HIG

// ## Analysis Constants #########################################################################
// const int    nDiffRapBins = 6;
// const double mDiffRapLow[nDiffRapBins]	= {-1.5, -0.8, -0.6, 0.2, 0.6, 0.8};
// const double mDiffRapHig[nDiffRapBins]	= {-0.8, -0.6, -0.2, 0.6, 0.8, 1.5};
// const double mDiffRapBds[nDiffRapBins + 2]	= {-1.5, -0.8, -0.6, -0.2, 0.2, 0.6, 0.8, 1.5};

// const int    nDiffRapBins = 10;
// const double mDiffRapLow[nDiffRapBins]	= {-1.0, -0.8, -0.7, -0.6, -0.5, 0.3, 0.5, 0.6, 0.7, 0.8};
// const double mDiffRapHig[nDiffRapBins]	= {-0.8, -0.7, -0.6, -0.5, -0.3, 0.5, 0.6, 0.7, 0.8, 1.0};
// const double mDiffRapBds[nDiffRapBins + 2]	= {-1.0, -0.8, -0.7, -0.6, -0.5, -0.3, 0.3, 0.5, 0.6, 0.7, 0.8, 1.0};

// const int   nDiffRapBins = 16;
// const double mDiffRapLow[nDiffRapBins]	= {-1.0, -0.95, -0.9, -0.8, -0.7, -0.6, -0.5, -0.4, 0.2, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 0.95};
// const double mDiffRapHig[nDiffRapBins]	= {-0.95, -0.9, -0.8, -0.7, -0.6, -0.5, -0.4, -0.2, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 0.95, 1.0};
// const double mDiffRapBds[nDiffRapBins + 2]	= { -1.0, -0.95, -0.9, -0.8, -0.7, -0.6, -0.5, -0.4, -0.2, 0.2, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 0.95, 1.0};

// constant 0.1 bin width
// const int    nDiffRapBins = 20;
// const double mDiffRapLow[nDiffRapBins]	= {-1.0, -0.9, -0.8, -0.7, -0.6, -0.5, -0.4, -0.3, -0.2, -0.1, 0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9};
// const double mDiffRapHig[nDiffRapBins]	= {-0.9, -0.8, -0.7, -0.6, -0.5, -0.4, -0.3, -0.2, -0.1, 0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0};
// const double mDiffRapBds[nDiffRapBins + 2]	= { -1.0, -0.9, -0.8, -0.7, -0.6, -0.5, -0.4, -0.3, -0.2, -0.1, 0.0, 0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0};

// constant 0.1 bin width
const int    nDiffRapBins = 18;
const double mDiffRapLow[nDiffRapBins]	= {-1.2, -1.1, -1.0, -0.9, -0.8, -0.7, -0.6, -0.5, -0.4, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0, 1.1 };
const double mDiffRapHig[nDiffRapBins]	= {-1.1, -1.0, -0.9, -0.8, -0.7, -0.6, -0.5, -0.4, -0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0, 1.1, 1.2 };
const double mDiffRapBds[nDiffRapBins + 2]	= { -1.2, -1.1, -1.0, -0.9, -0.8, -0.7, -0.6, -0.5, -0.4, -0.3, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0, 1.1, 1.2 };

// constant 0.05 bin width
// const int    nDiffRapBins = 30;
// const double mDiffRapLow[nDiffRapBins]	= {-1.0, -0.95, -0.9, -0.85, -0.8, -0.75, -0.7, -0.65, -0.6, -0.55, -0.5, -0.45, -0.4, -0.35, -0.3, 0.2, 0.3, 0.35, 0.4, 0.45, 0.5, 0.55, 0.6, 0.65, 0.7, 0.75, 0.8, 0.85, 0.9, 0.95};
// const double mDiffRapHig[nDiffRapBins]	= {-0.95, -0.9, -0.85, -0.8, -0.75, -0.7, -0.65, -0.6, -0.55, -0.5, -0.45, -0.4, -0.35, -0.3, -0.2, 0.3, 0.35, 0.4, 0.45, 0.5, 0.55, 0.6, 0.65, 0.7, 0.75, 0.8, 0.85, 0.9, 0.95, 1.0};
// const double mDiffRapBds[nDiffRapBins + 2]	= { -1.0, -0.95, -0.9, -0.85, -0.8, -0.75, -0.7, -0.65, -0.6, -0.55, -0.5, -0.45, -0.4, -0.35, -0.3, -0.2, 0.2, 0.3, 0.35, 0.4, 0.45, 0.5, 0.55, 0.6, 0.65, 0.7, 0.75, 0.8, 0.85, 0.9, 0.95, 1.0};

const vector<vector<double>> mTnPEtaBins = {
	{0.0, 1.2, 1.8, 2.1, 2.4},
	{0.0, 1.0, 1.6, 2.1, 2.4},
	{0.0, 1.2, 2.4},
	{0.0, 0.4, 0.8, 1.2, 1.8, 2.1, 2.4},
};
const vector<double> mTnPPtBins = {0.05, 0.09, 0.1, 0.11, 0.12, 0.13, 0.14, 0.16, 0.20};
const vector<vector<double>> mTnPHLTEtaBins = {
	{0.0, 1.2, 1.8, 2.1, 2.4},
	{0.0, 1.0, 1.6, 2.1, 2.4},
	{0.0, 1.2, 2.4},
	{0.0, 0.4, 0.8, 1.2, 1.8, 2.1, 2.4},
};
const vector<double> mTnPHLTPtBins = {0.05, 0.09, 0.1, 0.11, 0.12, 0.13, 0.14, 0.16, 0.20};
const vector<TString> mTnPOptName = {"", "_TnPEtaBins0", "_TnPEtaBins1", "_TnPEtaBins2", "_TnPEtaBins3"};

#endif
