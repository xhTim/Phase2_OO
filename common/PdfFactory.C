#include "PdfFactory.h"
#include "HistWorker.h"

Double_t DoubleCrystalBall(Double_t x, Double_t N, Double_t mu, Double_t sigma, 
						Double_t cbNL, Double_t cbAlphaL, Double_t cbNR, Double_t cbAlphaR)
{

	Double_t A = TMath::Power(cbNL/fabs(cbAlphaL), cbNL) * TMath::Exp(-cbAlphaL*cbAlphaL/2.);
	Double_t B = cbNL/fabs(cbAlphaL) - fabs(cbAlphaL);

	Double_t C = TMath::Power(cbNR/fabs(cbAlphaR), cbNR) * TMath::Exp(-cbAlphaR*cbAlphaR/2.);
	Double_t D = cbNR/fabs(cbAlphaR) - fabs(cbAlphaR);

	Double_t norm = (x-mu)/sigma;

	if(norm < -cbAlphaL) 
	{
		return N * A * TMath::Power(B-norm, -cbNL);
	}
	else if(norm < cbAlphaR) 
	{
		return N * TMath::Exp(-0.5*norm*norm);
	}
	else 
	{
		return N * C * TMath::Power(D+norm, -cbNR);
	}
}

Double_t AsymDoubleCrystalBall(Double_t x, Double_t N, Double_t mu, Double_t sigmaL, Double_t sigmaR,
						Double_t cbNL, Double_t cbAlphaL, Double_t cbNR, Double_t cbAlphaR)
{

	Double_t A = TMath::Power(cbNL/fabs(cbAlphaL), cbNL) * TMath::Exp(-cbAlphaL*cbAlphaL/2.);
	Double_t B = cbNL/fabs(cbAlphaL) - fabs(cbAlphaL);

	Double_t C = TMath::Power(cbNR/fabs(cbAlphaR), cbNR) * TMath::Exp(-cbAlphaR*cbAlphaR/2.);
	Double_t D = cbNR/fabs(cbAlphaR) - fabs(cbAlphaR);

	Double_t normL = (x-mu)/sigmaL;
	Double_t normR = (x-mu)/sigmaR;

	if(normL < -cbAlphaL) 
	{
		return N * A * TMath::Power(B-normL, -cbNL);
	}
	else if (normL < 0)
	{
		return N * TMath::Exp(-0.5*normL*normL);
	}
	else if(normR < cbAlphaR) 
	{
		return N * TMath::Exp(-0.5*normR*normR);
	}
	else 
	{
		return N * C * TMath::Power(D+normR, -cbNR);
	}
}


struct QEDPdf : public PdfFactory
{
	//Priviate but not so priviate members-----------------------------------------------
	enum QEDfuction { Cubic };
	TF1 * fQED;
	const double massLow4Fit, massHig4Fit;
	const int FitN;
	RooRealVar  & mMass;
	RooConstVar mP0, mP1, mP2, mP3, mP4;
	RooRealVar  mP0_Free, mP1_Free, mP2_Free, mP3_Free;
	//-----------------------------------------------------------------------------------

	//Constructor------------------------------------------------------------------------
	QEDPdf(TH1D* Hist, RooRealVar & mMass, const double massLow4Fit, const double massHig4Fit, const int FitN) : 
			PdfFactory{Hist}, mMass{mMass}, massLow4Fit{massLow4Fit}, massHig4Fit{massHig4Fit}, FitN{FitN}	{}
	virtual ~QEDPdf() = default;
	//-----------------------------------------------------------------------------------

	//Virtual Functions------------------------------------------------------------------
	virtual RooGenericPdf * GetPdf()		{if(!Pdf)	throw std::runtime_error("QEDPdf ----> No PDF!!!");	return Pdf;	}
	//-----------------------------------------------------------------------------------

	//Free Functions---------------------------------------------------------------------
	void Init( QEDfuction f = QEDfuction::Cubic )
	{
		switch(f)
		{
			case Cubic 		:	fQED = new TF1("fQED", fReject4QED, massLow4Fit, massHig4Fit, 4);	break;
			default			:	fQED = new TF1("fQED", fReject4QED, massLow4Fit, massHig4Fit, 4);	break;
		}

		for (int i = 0; i < FitN; ++i)
		{
			Hist ->Fit(fQED, "R", "",  massLow4Fit, massHig4Fit); //use side band to initialized QED parameters
		}

		mP0	=	RooConstVar(  "mP0", "mP0",  fQED->GetParameter(0));
		mP1	=	RooConstVar(  "mP1", "mP1",  fQED->GetParameter(1));
		mP2	=	RooConstVar(  "mP2", "mP2",  fQED->GetParameter(2));
		mP3	=	RooConstVar(  "mP3", "mP3",  fQED->GetParameter(3));
		// mP0	=	RooRealVar(  "mP0", "mP0",  fQED->GetParameter(0), -1.e8,  1.e8);
		// mP1	=	RooRealVar(  "mP1", "mP1",  fQED->GetParameter(1), 0.,     1.e5);
		// mP2	=	RooRealVar(  "mP2", "mP2",  fQED->GetParameter(2), -1.e5,    0.);
		// mP3	=	RooRealVar(  "mP3", "mP3",  fQED->GetParameter(3), 0.,    1.e4);

		Pdf = new RooGenericPdf("qedPdf", "qedPdf", "mP0 + mP1*mMass + mP2*mMass*mMass + mP3*mMass*mMass*mMass", 
									RooArgSet(mP0, mP1, mP2, mP3, mMass));
	}
	void InitQuartic()
	{

		fQED = new TF1("fQED", fReject4QED_Quartic, massLow4Fit, massHig4Fit, 5);

		for (int i = 0; i < FitN; ++i)
		{
			Hist ->Fit(fQED, "R", "",  massLow4Fit, massHig4Fit); //use side band to initialized QED parameters
		}

		mP0	=	RooConstVar(  "mP0", "mP0",  fQED->GetParameter(0));
		mP1	=	RooConstVar(  "mP1", "mP1",  fQED->GetParameter(1));
		mP2	=	RooConstVar(  "mP2", "mP2",  fQED->GetParameter(2));
		mP3	=	RooConstVar(  "mP3", "mP3",  fQED->GetParameter(3));
		mP4	=	RooConstVar(  "mP4", "mP4",  fQED->GetParameter(4));

		Pdf = new RooGenericPdf("qedPdf", "qedPdf", "mP0 + mP1*mMass + mP2*mMass*mMass + mP3*mMass*mMass*mMass + mP4*mMass*mMass*mMass*mMass", 
									RooArgSet(mP0, mP1, mP2, mP3, mP4, mMass));
	}
	void InitFreeCubic()
	{

		fQED = new TF1("fQED", fReject4QED, massLow4Fit, massHig4Fit, 4);

		for (int i = 0; i < FitN; ++i)
		{
			Hist ->Fit(fQED, "R", "",  massLow4Fit, massHig4Fit); //use side band to initialized QED parameters
		}

		mP0_Free	=	RooRealVar(  "mP0_Free", "mP0_Free",  fQED->GetParameter(0), -1.e6,  1.e6);
		mP1_Free	=	RooRealVar(  "mP1_Free", "mP1_Free",  fQED->GetParameter(1), 0.,     1.e4);
		mP2_Free	=	RooRealVar(  "mP2_Free", "mP2_Free",  fQED->GetParameter(2), -1.e4,    0.);
		mP3_Free	=	RooRealVar(  "mP3_Free", "mP3_Free",  fQED->GetParameter(3), 0.,    1.e3);

		Pdf = new RooGenericPdf("qedPdf", "qedPdf", "mP0_Free + mP1_Free*mMass + mP2_Free*mMass*mMass + mP3_Free*mMass*mMass*mMass", 
									RooArgSet(mP0_Free, mP1_Free, mP2_Free, mP3_Free, mMass));
	}
	double GetInitN(const double BinLow, const double BinHigh)
	{
		const int tem_QEDBinLow = HistWorker::FindBin(Hist, BinLow, 0);
		const int tem_QEDBinHig = HistWorker::FindBin(Hist, BinHigh, 1);
		double N = Hist->Integral(tem_QEDBinLow, tem_QEDBinHig)*(massHig4Fit-massLow4Fit)/(BinHigh-BinLow);

		return N;
	}
	//-----------------------------------------------------------------------------------
	protected:
	static double fReject4QED(double *x, double *par)
	{
		if((x[0]>2.80&& x[0]<3.35) || (x[0]>3.50 && x[0]<3.85))
		{
			TF1::RejectPoint();
			return 0;
		}

		return par[0] + par[1]*x[0] + par[2]*pow(x[0],2) + par[3]*pow(x[0],3);
	}
	static double fReject4QED_Quartic(double *x, double *par)
	{
		if((x[0]>2.80&& x[0]<3.35) || (x[0]>3.50 && x[0]<3.85))
		{
			TF1::RejectPoint();
			return 0;
		}

		return par[0] + par[1]*x[0] + par[2]*pow(x[0],2) + par[3]*pow(x[0],3) + par[4]*pow(x[0],4);
	}
};

struct JpsiPdf : public PdfFactory
{	
	//Priviate but not so priviate members-----------------------------------------------
	const double massLow4Fit, massHig4Fit;
	RooRealVar  & mMass;
	//-----------------------------------------------------------------------------------

	//Constructor------------------------------------------------------------------------
	JpsiPdf(TH1D* Hist, RooRealVar & mMass, const double massLow4Fit, const double massHig4Fit) : 
			PdfFactory{Hist}, mMass{mMass}, massLow4Fit{massLow4Fit}, massHig4Fit{massHig4Fit}	{}
	//-----------------------------------------------------------------------------------

	//Virtual Functions------------------------------------------------------------------
	virtual RooGenericPdf * GetPdf()		{if(!Pdf)	throw std::runtime_error("JpsiPdf ----> No PDF!!!");	return Pdf;	}
	//-----------------------------------------------------------------------------------

	//Free Functions---------------------------------------------------------------------
	void Init(RooRealVar& cbAlpha, RooRealVar& cbN, RooRealVar& jpsiSigma, RooRealVar& jpsiMu)
	{
		Pdf = new RooGenericPdf("jpsiCrystalBallPdf", "jpsiCrystalBallPdf",
					"ROOT::Math::crystalball_function(mMass,cbAlpha,cbN,jpsiSigma,jpsiMu)", 
					RooArgSet(mMass, cbAlpha, cbN, jpsiSigma, jpsiMu));
	}

	void InitCrystalBallGauss(RooRealVar& cbAlpha, RooRealVar& cbN, RooRealVar& jpsiSigma, RooConstVar& sigmaRatio, RooRealVar& jpsiMu, RooRealVar& gausN)
	{
		Pdf = new RooGenericPdf("jpsiCrystalBallGaussPdf", "jpsiCrystalBallGaussPdf",
					"ROOT::Math::crystalball_function(mMass,cbAlpha,cbN,jpsiSigma*sigmaRatio,jpsiMu) + gausN*TMath::Gaus(mMass, jpsiMu, jpsiSigma)", 
					RooArgSet(mMass, cbAlpha, cbN, jpsiSigma, sigmaRatio, jpsiMu, gausN));
	}

	void InitDoubleCrystalBall(RooRealVar& jpsiN, RooRealVar& jpsiMu, RooRealVar& jpsiSigma, RooRealVar& cbNL, RooRealVar& cbAlphaL, RooRealVar& cbNR, RooRealVar& cbAlphaR)
	{
		Pdf = new RooGenericPdf("jpsiDoubleCrystalBallPdf", "jpsiDoubleCrystalBallPdf",
					"DoubleCrystalBall(mMass, jpsiN, jpsiMu, jpsiSigma, cbNL, cbAlphaL, cbNR, cbAlphaR)", 
					RooArgSet(mMass, jpsiN, jpsiMu, jpsiSigma, cbNL, cbAlphaL, cbNR, cbAlphaR));
	}

	void InitAsymDoubleCrystalBall(RooRealVar& jpsiN, RooRealVar& jpsiMu, RooRealVar& jpsiSigmaL, RooRealVar& jpsiSigmaR, RooRealVar& cbNL, RooRealVar& cbAlphaL, RooRealVar& cbNR, RooRealVar& cbAlphaR)
	{
		Pdf = new RooGenericPdf("jpsiAsymDoubleCrystalBallPdf", "jpsiAsymDoubleCrystalBallPdf",
					"AsymDoubleCrystalBall(mMass, jpsiN, jpsiMu, jpsiSigmaL, jpsiSigmaR, cbNL, cbAlphaL, cbNR, cbAlphaR)", 
					RooArgSet(mMass, jpsiN, jpsiMu, jpsiSigmaL, jpsiSigmaR, cbNL, cbAlphaL, cbNR, cbAlphaR));
	}

	// RooAddPdf* 		GetRooCrystalBallPdf() 			{if(!jpsiRooCrystalBallPdf)			throw std::runtime_error("JpsiPdf ----> No jpsiRooCrystalBallPdf!!!");	return jpsiRooCrystalBallPdf;}

	double GetInitN(const double BinLow, const double BinHigh, const double nQED4Init)
	{
		const int    tem_JpsiBinLow = HistWorker::FindBin(Hist, BinLow,  0);
		const int    tem_JpsiBinHig = HistWorker::FindBin(Hist, BinHigh, 1);

		//const double nJpsi4Init     = Hist->Integral(tem_JpsiBinLow, tem_JpsiBinHig)*0.80; //- nQED4Init*(3.30-2.80)/(massHig4Fit-massLow4Fit);
		double N 	= Hist->Integral(tem_JpsiBinLow, tem_JpsiBinHig) - nQED4Init*(BinHigh-BinLow)/(massHig4Fit-massLow4Fit);

		return N;
	}

	//--------------TEMP! For Initializing CBAN with CB to Jpsi Peak---------------
	// static double fReject4Jpsi(double *x, double *par)
	// {
	// 	if((x[0]<2.90) || (x[0]>3.20))
	// 	{
	// 		TF1::RejectPoint();
	// 		return 0;
	// 	}

	// 	return (par[0] * ROOT::Math::crystalball_function(x[0], par[1], par[2], par[3], par[4]) + par[5]  );
	// }
	//--------------TEMP! For Initializing CBAN with CB to Jpsi Peak---------------
	//-----------------------------------------------------------------------------------
};

struct PsiPdf : public PdfFactory
{	
	//Priviate but not so priviate members-----------------------------------------------
	const double massLow4Fit, massHig4Fit;
	RooRealVar  & mMass;
	RooConstVar massRatio = RooConstVar(  "massRatio",   "massRatio",  mPsi_PDG/mJpsi_PDG);
	//-----------------------------------------------------------------------------------

	//Constructor------------------------------------------------------------------------
	PsiPdf(TH1D* Hist, RooRealVar & mMass, const double massLow4Fit, const double massHig4Fit) : 
			PdfFactory{Hist}, mMass{mMass}, massLow4Fit{massLow4Fit}, massHig4Fit{massHig4Fit}	{}
	//-----------------------------------------------------------------------------------

	//Virtual Functions------------------------------------------------------------------
	virtual RooGenericPdf * GetPdf()		{if(!Pdf)	throw std::runtime_error("PsiPdf ----> No PDF!!!");	return Pdf;	}
	//-----------------------------------------------------------------------------------

	//Free Functions---------------------------------------------------------------------
	void Init(RooRealVar& cbAlpha, RooRealVar& cbN, RooRealVar& jpsiSigma, RooRealVar& jpsiMu)
	{
		Pdf = new RooGenericPdf("psiCrystalBallPdf",  "psiCrystalBallPdf",
					"ROOT::Math::crystalball_function(mMass,cbAlpha,cbN,jpsiSigma*massRatio,jpsiMu*massRatio)", 
					RooArgSet(mMass, cbAlpha, cbN, jpsiSigma, jpsiMu, massRatio)); // psiMu = jpsiMu * massRatio; psiSigma = jpsiSigma * massRatio
	}

	void InitCrystalBallGauss(RooRealVar& cbAlpha, RooRealVar& cbN, RooRealVar& jpsiSigma, RooConstVar& sigmaRatio, RooRealVar& jpsiMu, RooRealVar& gausN)
	{
		Pdf = new RooGenericPdf("psiCrystalBallGaussPdf",  "psiCrystalBallGaussPdf",
					"ROOT::Math::crystalball_function(mMass,cbAlpha,cbN,jpsiSigma*sigmaRatio*massRatio,jpsiMu*massRatio) + gausN*TMath::Gaus(mMass, jpsiMu*massRatio, jpsiSigma*massRatio)", 
					RooArgSet(mMass, cbAlpha, cbN, jpsiSigma, sigmaRatio, jpsiMu, massRatio, gausN)); // psiMu = jpsiMu * massRatio; psiSigma = jpsiSigma * sigmaRatio * massRatio
	}

	void InitDoubleCrystalBall(RooRealVar& psiN, RooRealVar& jpsiMu, RooRealVar& jpsiSigma, RooRealVar& cbNL, RooRealVar& cbAlphaL, RooRealVar& cbNR, RooRealVar& cbAlphaR)
	{
		Pdf = new RooGenericPdf("psiDoubleCrystalBallPdf", "psiDoubleCrystalBallPdf",
					"DoubleCrystalBall(mMass, psiN, jpsiMu*massRatio, jpsiSigma*massRatio, cbNL, cbAlphaL, cbNR, cbAlphaR)", 
					RooArgSet(mMass, psiN, jpsiMu, jpsiSigma, cbNL, cbAlphaL, cbNR, cbAlphaR, massRatio));
	}

	void InitAsymDoubleCrystalBall(RooRealVar& psiN, RooRealVar& jpsiMu, RooRealVar& jpsiSigmaL, RooRealVar& jpsiSigmaR, RooRealVar& cbNL, RooRealVar& cbAlphaL, RooRealVar& cbNR, RooRealVar& cbAlphaR)
	{
		//Pdf = new RooGenericPdf("psiAsymDoubleCrystalBallPdf", "psiAsymDoubleCrystalBallPdf",
		//			"AsymDoubleCrystalBall(mMass, psiN, jpsiMu*massRatio, jpsiSigmaL*massRatio, jpsiSigmaR*massRatio, cbNL, cbAlphaL, cbNR, cbAlphaR)", 
		//			RooArgSet(mMass, psiN, jpsiMu, jpsiSigmaL, jpsiSigmaR, cbNL, cbAlphaL, cbNR, cbAlphaR, massRatio));
	}

	// RooAddPdf* 		GetPdfRooCrystalBall()			{if(!psiRooCrystalBallPdf)			throw std::runtime_error("PsiPdf ----> No RooCrystalBallPdf!!!");			return psiRooCrystalBallPdf;}
	//-----------------------------------------------------------------------------------
};

struct TotalMassPdf: public PdfFactory
{
	//Priviate but not so priviate members-----------------------------------------------
	//-----------------------------------------------------------------------------------
	//Constructor------------------------------------------------------------------------
	//-----------------------------------------------------------------------------------
	//Virtual Functions------------------------------------------------------------------
	//-----------------------------------------------------------------------------------
	//Free Functions---------------------------------------------------------------------
	//-----------------------------------------------------------------------------------
};

struct PtPdf : public PdfFactory
{
	//Priviate but not so priviate members-----------------------------------------------
	//-----------------------------------------------------------------------------------
	//Constructor------------------------------------------------------------------------
	//-----------------------------------------------------------------------------------
	//Virtual Functions------------------------------------------------------------------
	//-----------------------------------------------------------------------------------
	//Free Functions---------------------------------------------------------------------
	//-----------------------------------------------------------------------------------
};
