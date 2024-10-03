/*
	functions for the tree analysis
	Must include the tree header file before importing this file
*/
#include "../common/constants.h"

#ifndef COMMON_FUNCTIONS_TREE_H
#define COMMON_FUNCTIONS_TREE_H


double BetheBloch(double *x, double *par)
{
	// Bethe-Bloch formula

	// double p = ;
	double m = par[0];
	double K = par[1];
	double C = par[2];

	double dEdx = K * m * m / (x[0] * x[0]) + C;

	return dEdx;
}

double LogBetheBloch_scale(double *x, double *par)
{
	// Log Bethe-Bloch formula

	return ROOT::Math::log(BetheBloch(x, par) * par[3]);
}

double LogBetheBloch(double *x, double *par)
{
	// Log Bethe-Bloch formula

	return ROOT::Math::log(BetheBloch(x, par));
}

double prob_Gauss(double p, double logdEdx, TF1* fMean, double Sigma)
{
	// * calculate the gaussian probability of the dE/dx
	double mean = fMean->Eval(p);
	// double sigma = gSigma->Eval(p);
	double prob = TMath::Gaus(logdEdx, mean, Sigma, kTRUE);	// kTRUE for normalisation (default is kFALSE) the result is divided by sqrt(2*Pi)*sigma.
	
	// double prob = ROOT::Math::gaussian_pdf( logdEdx, Sigma, mean);

	return prob;
}
double prob_Gauss(double p, double logdEdx, TF1* fMean, TF1* fSigma)
{
	// * calculate the gaussian probability of the dE/dx
	double mean = fMean->Eval(p);
	double sigma = fSigma->Eval(p);
	//* Handling the upper and lower limit of the sigma
	if (p > 0.75) sigma = fSigma->Eval(0.75);
	
	double prob = TMath::Gaus(logdEdx, mean, sigma, kTRUE);	// kTRUE for normalisation (default is kFALSE) the result is divided by sqrt(2*Pi)*sigma.
	
	return prob;
}

TF1 *fPID_ka = nullptr, *fPID_ka_pos = nullptr, *fPID_ka_neg = nullptr;
TF1 *fPID_ka_sigma = nullptr, *fPID_ka_sigma_pos = nullptr, *fPID_ka_sigma_neg = nullptr;
TF1 *fPID_ka_mc = nullptr, *fPID_ka_pos_mc = nullptr, *fPID_ka_neg_mc = nullptr;
TH1D *hPID_ka_sigma_mc = nullptr, *hPID_ka_sigma_pos_mc = nullptr, *hPID_ka_sigma_neg_mc = nullptr;

TF1 *fPID_pi = nullptr, *fPID_pi_pos = nullptr, *fPID_pi_neg = nullptr;
TF1 *fPID_pi_sigma = nullptr, *fPID_pi_sigma_pos = nullptr, *fPID_pi_sigma_neg = nullptr;

TH1D *hPID_shift = nullptr, *hPID_shift_pos = nullptr, *hPID_shift_neg = nullptr;
TF1 *fPID_shift = nullptr, *fPID_shift_pos = nullptr, *fPID_shift_neg = nullptr;
const double mPID_ka_sigma = 0.14;

void init_fPID_BB(TString inFileDir_PID)
{
	// * initialize the PID functions with the Bethe-Bloch fit
	TFile *inFile_PID = new TFile(inFileDir_PID, "READ");
	if (!inFile_PID)
	{
		throw std::runtime_error("PID file not found");
	}
	TF1 *temp_ka		= (TF1 *)inFile_PID->Get("fPID_ka");
	TF1 *temp_ka_pos	= (TF1 *)inFile_PID->Get("fPID_ka_pos");
	TF1 *temp_ka_neg	= (TF1 *)inFile_PID->Get("fPID_ka_neg");

	double K_ka		= temp_ka->GetParameter(1);
	double C_ka		= temp_ka->GetParameter(2);
	double K_ka_pos	= temp_ka_pos->GetParameter(1);
	double C_ka_pos	= temp_ka_pos->GetParameter(2);
	double K_ka_neg	= temp_ka_neg->GetParameter(1);
	double C_ka_neg	= temp_ka_neg->GetParameter(2);

	fPID_ka = new TF1("fMean_ka", LogBetheBloch, 0.0, 2.0, 3);
	fPID_ka->SetParNames("m", "K", "C");
	fPID_ka->SetParameters(KAON_MASS, K_ka, C_ka);
	fPID_ka_pos = new TF1("fMean_ka_pos", LogBetheBloch, 0.0, 2.0, 3);
	fPID_ka_pos->SetParNames("m", "K", "C");
	fPID_ka_pos->SetParameters(KAON_MASS, K_ka_pos, C_ka_pos);
	fPID_ka_neg = new TF1("fMean_ka_neg", LogBetheBloch, 0.0, 2.0, 3);
	fPID_ka_neg->SetParNames("m", "K", "C");
	fPID_ka_neg->SetParameters(KAON_MASS, K_ka_neg, C_ka_neg);

	fPID_pi = new TF1("fMean_pi", LogBetheBloch, 0.0, 2.0, 3);
	fPID_pi->SetParNames("m", "K", "C");
	fPID_pi->SetParameters(PION_MASS, K_ka, C_ka);
	fPID_pi_pos = new TF1("fMean_pi_pos", LogBetheBloch, 0.0, 2.0, 3);
	fPID_pi_pos->SetParNames("m", "K", "C");
	fPID_pi_pos->SetParameters(PION_MASS, K_ka_pos, C_ka_pos);
	fPID_pi_neg = new TF1("fMean_pi_neg", LogBetheBloch, 0.0, 2.0, 3);
	fPID_pi_neg->SetParNames("m", "K", "C");
	fPID_pi_neg->SetParameters(PION_MASS, K_ka_neg, C_ka_neg);

	cout << "===============================================================" << endl;
	cout << "PID file: " << inFileDir_PID << endl;
	cout << "PID functions initialized with -------> Bethe-Bloch <-------" << endl;
	cout << "PID_ka: K = " << K_ka << ", C = " << C_ka << endl;
	cout << "PID_ka_pos: K = " << K_ka_pos << ", C = " << C_ka_pos << endl;
	cout << "PID_ka_neg: K = " << K_ka_neg << ", C = " << C_ka_neg << endl;
	cout << "===============================================================" << endl;
	return;
}
void init_fPID(TString inFileDir_PID = mInFileName_PID_ka, TString inFileDir_PID_mc = mInFileName_PID_ka_mc, TString inFileDir_PID_pi = mInFileName_PID_pi)
{
	// * initialize the PID functions with the dE/dx fit
	TFile *inFile_PID = new TFile(inFileDir_PID, "READ");
	TFile *inFile_PID_mc = new TFile(inFileDir_PID_mc, "READ");
	TFile *inFile_PID_pi = new TFile(inFileDir_PID_pi, "READ");

	if (!inFile_PID || !inFile_PID_mc || !inFile_PID_pi)
	{
		throw std::runtime_error("PID file not found");
	}

	fPID_ka = (TF1 *)inFile_PID->Get("fMean");
	fPID_ka_pos = (TF1 *)inFile_PID->Get("fMean_pos");
	fPID_ka_neg = (TF1 *)inFile_PID->Get("fMean_neg");
	fPID_ka_sigma = (TF1 *)inFile_PID->Get("fSigma");
	fPID_ka_sigma_pos = (TF1 *)inFile_PID->Get("fSigma_pos");
	fPID_ka_sigma_neg = (TF1 *)inFile_PID->Get("fSigma_neg");

	fPID_ka_mc = (TF1 *)inFile_PID_mc->Get("fMean");
	fPID_ka_pos_mc = (TF1 *)inFile_PID_mc->Get("fMean_pos");
	fPID_ka_neg_mc = (TF1 *)inFile_PID_mc->Get("fMean_neg");

	fPID_pi = (TF1 *)inFile_PID_pi->Get("fMean");
	fPID_pi_pos = (TF1 *)inFile_PID_pi->Get("fMean_pos");
	fPID_pi_neg = (TF1 *)inFile_PID_pi->Get("fMean_neg");
	fPID_pi_sigma = (TF1 *)inFile_PID_pi->Get("fSigma");
	fPID_pi_sigma_pos = (TF1 *)inFile_PID_pi->Get("fSigma_pos");
	fPID_pi_sigma_neg = (TF1 *)inFile_PID_pi->Get("fSigma_neg");


	cout << "===============================================================" << endl;
	cout << "PID file: " << inFileDir_PID << endl;
	cout << "PID functions initialized" << endl;
	cout << "PID Kaon Function (data): " << fPID_ka->GetExpFormula() << ", Sigma: " << fPID_ka_sigma->GetExpFormula() << endl;
	cout << "PID Kaon Function (mc): " << fPID_ka_mc->GetExpFormula() << endl;
	cout << "PID Pion Function (data): " << fPID_pi->GetExpFormula() << ", Sigma: " << fPID_pi_sigma->GetExpFormula() << endl;
	// cout << "PID Pion Function (mc): " << fPID_pi_mc->GetExpFormula() << endl;
	cout << "===============================================================" << endl;
	return;
}

double get_shift(double p, int charge)
{
	// if (!hPID_shift || !hPID_shift_pos || !hPID_shift_neg)
	// {
	// 	throw std::runtime_error("PID shift not initialized");
	// }
	if (charge == 0)
	{
		throw std::runtime_error("get_shift: charge = 0");
	}


	// int bin = hPID_shift->FindBin(p);
	// //handle the overflow
	// if (bin > hPID_shift->GetNbinsX()) bin = hPID_shift->GetNbinsX();
	// if (bin < 1) bin = 1;
	// return charge > 0 ? hPID_shift_pos->GetBinContent(bin) : hPID_shift_neg->GetBinContent(bin);

	// double p_min = 0.16;
	// double p_max = 0.70;
	// if (p < p_min) return charge > 0 ? fPID_shift_pos->Eval(p_min) : fPID_shift_neg->Eval(p_min);
	// if (p > p_max) return charge > 0 ? fPID_shift_pos->Eval(p_max) : fPID_shift_neg->Eval(p_max);
	// return charge > 0 ? fPID_shift_pos->Eval(p) : fPID_shift_neg->Eval(p);

	return charge > 0 ? (fPID_ka_pos->Eval(p) - fPID_ka_pos_mc->Eval(p)) : (fPID_ka_neg->Eval(p) - fPID_ka_neg_mc->Eval(p));
}

bool pass_discriminator(ParticleTree *tree, int icand, int isMC = 0)
{
	if ( !fPID_ka || !fPID_ka_pos || !fPID_ka_neg )
	{
		throw std::runtime_error("PID functions not initialized");
	}

	auto daughters = tree->cand_dauIdx->at(icand);
	int dauIdx_1 = daughters[0];
	int dauIdx_2 = daughters[1];

	int trkIdx_1 = tree->cand_trkIdx->at(dauIdx_1);
	int trkIdx_2 = tree->cand_trkIdx->at(dauIdx_2);

	double p_1 = tree->cand_p->at(dauIdx_1);
	double p_2 = tree->cand_p->at(dauIdx_2);

	double dEdx_1 = tree->trk_dEdx_dedxPixelHarmonic2->at(trkIdx_1);
	double dEdx_2 = tree->trk_dEdx_dedxPixelHarmonic2->at(trkIdx_2);
	// double dEdx_1 = tree->trk_dEdx_energyLossProducer_energyLossAllHits->at(trkIdx_1);
	// double dEdx_2 = tree->trk_dEdx_energyLossProducer_energyLossAllHits->at(trkIdx_2);

	double logdEdx_1 = ROOT::Math::log(dEdx_1);
	double logdEdx_2 = ROOT::Math::log(dEdx_2);

	int charge_1 = tree->cand_charge->at(dauIdx_1);
	int charge_2 = tree->cand_charge->at(dauIdx_2);

	if (isMC == 1)	//Only shift mc
	{
		// * use the PID shift to shift the LogdEdx
		double shift_1 = get_shift(p_1, charge_1);
		double shift_2 = get_shift(p_2, charge_2);

		logdEdx_1 += shift_1;
		logdEdx_2 += shift_2;
	}

	//= Using constant sigma ========================================================
	// double prob_ka_1 = prob_Gauss(p_1, logdEdx_1, charge_1 > 0 ? fPID_ka_pos : fPID_ka_neg, mPID_ka_sigma);
	// double prob_ka_2 = prob_Gauss(p_2, logdEdx_2, charge_2 > 0 ? fPID_ka_pos : fPID_ka_neg, mPID_ka_sigma);
	// double prob_pi_1 = prob_Gauss(p_1, logdEdx_1, charge_1 > 0 ? fPID_pi_pos : fPID_pi_neg, mPID_ka_sigma);
	// double prob_pi_2 = prob_Gauss(p_2, logdEdx_2, charge_2 > 0 ? fPID_pi_pos : fPID_pi_neg, mPID_ka_sigma);

	//= Using Bin by Bin sigma ========================================================
	double prob_ka_1 = prob_Gauss(p_1, logdEdx_1, charge_1 > 0 ? fPID_ka_pos : fPID_ka_neg, charge_1 > 0 ? fPID_ka_sigma_pos : fPID_ka_sigma_neg);
	double prob_ka_2 = prob_Gauss(p_2, logdEdx_2, charge_2 > 0 ? fPID_ka_pos : fPID_ka_neg, charge_2 > 0 ? fPID_ka_sigma_pos : fPID_ka_sigma_neg);
	double prob_pi_1 = prob_Gauss(p_1, logdEdx_1, charge_1 > 0 ? fPID_pi_pos : fPID_pi_neg, charge_1 > 0 ? fPID_pi_sigma_pos : fPID_pi_sigma_neg);
	double prob_pi_2 = prob_Gauss(p_2, logdEdx_2, charge_2 > 0 ? fPID_pi_pos : fPID_pi_neg, charge_2 > 0 ? fPID_pi_sigma_pos : fPID_pi_sigma_neg);

	//* Paired probability
	// bool isKaon = (prob_ka_1 * prob_ka_2) > (mPIDThresholdFactor * prob_pi_1 * prob_pi_2);
	//* Singular probability
	bool isKaon_1 = prob_ka_1 > mPIDThresholdFactor * prob_pi_1;
	bool isKaon_2 = prob_ka_2 > mPIDThresholdFactor * prob_pi_2;
	bool isKaon = isKaon_1 && isKaon_2;

	return isKaon;

}

bool pass_discriminator_single(ParticleTree *tree, int idau, int isMC = 0)
{
	if ( !fPID_ka || !fPID_ka_pos || !fPID_ka_neg )
	{
		throw std::runtime_error("PID functions not initialized");
	}

	int trkIdx = tree->cand_trkIdx->at(idau);

	double p = tree->cand_p->at(idau);
	double dEdx = tree->trk_dEdx_dedxPixelHarmonic2->at(trkIdx);
	// double dEdx = tree->trk_dEdx_energyLossProducer_energyLossAllHits->at(trkIdx);
	double logdEdx = ROOT::Math::log(dEdx);
	int charge = tree->cand_charge->at(idau);

	if (isMC == 1)	//Only shift mc
	{
		// * use the PID shift to shift the LogdEdx
		double shift = get_shift(p, charge);
		logdEdx += shift;
	}

	//= Using Bin by Bin sigma ========================================================
	double prob_ka = prob_Gauss(p, logdEdx, charge > 0 ? fPID_ka_pos : fPID_ka_neg, charge > 0 ? fPID_ka_sigma_pos : fPID_ka_sigma_neg);
	double prob_pi = prob_Gauss(p, logdEdx, charge > 0 ? fPID_pi_pos : fPID_pi_neg, charge > 0 ? fPID_pi_sigma_pos : fPID_pi_sigma_neg);

	bool isKaon = prob_ka > mPIDThresholdFactor * prob_pi;

	return isKaon;
}

bool pass_LogdEdx_curve_single(ParticleTree *tree, int idau, int isMC, double nSig = 3.0)
{
	// * check if the dE/dx is within the curve
	if ( !fPID_ka || !fPID_ka_pos || !fPID_ka_neg )
	{
		throw std::runtime_error("PID functions not initialized");
	}

	double p = tree->cand_p->at(idau);
	int trkIdx = tree->cand_trkIdx->at(idau);
	double dEdx = tree->trk_dEdx_dedxPixelHarmonic2->at(trkIdx);
	// double dEdx = tree->trk_dEdx_energyLossProducer_energyLossAllHits->at(trkIdx);
	double logdEdx = ROOT::Math::log(dEdx);
	int charge = tree->cand_charge->at(idau);

	if (isMC == 1)	//Only shift mc
	{
		// * use the PID shift to shift the LogdEdx
		double shift = get_shift(p, charge);
		logdEdx += shift;
	}

	// * check if the dE/dx is within the curve
	bool pass_low_band = charge > 0 ? (logdEdx > fPID_ka_pos->Eval(p) - nSig * mPID_ka_sigma) : (logdEdx > fPID_ka_neg->Eval(p) - nSig * mPID_ka_sigma);
	bool pass_high_band = charge > 0 ? (logdEdx < fPID_ka_pos->Eval(p) + nSig * mPID_ka_sigma) : (logdEdx < fPID_ka_neg->Eval(p) + nSig * mPID_ka_sigma);

	// * dEdx lower limit
	bool pass_low_dEdx = dEdx > 2.1;

	return pass_low_band && pass_high_band && pass_low_dEdx;
}

bool pass_LogdEdx_curve(ParticleTree *tree, int icand, int isMC)
{

	auto daughters = tree->cand_dauIdx->at(icand);
	int dauIdx_1 = daughters[0];
	int dauIdx_2 = daughters[1];

	bool pass_1 = pass_LogdEdx_curve_single(tree, dauIdx_1, isMC);
	bool pass_2 = pass_LogdEdx_curve_single(tree, dauIdx_2, isMC);

	return pass_1 && pass_2;

}

TF1 *fPID_tag_low, *fPID_tag_high;
TF1 *fPID_tag_pi_low, *fPID_tag_pi_high;
void init_fPID_tag()
{
	//= Kaon ======================================================================
	fPID_tag_low = new TF1("fPID_tag_low", "[A] + exp([B]+[C]*x)", 0, 1);
	fPID_tag_low->SetParameters(0.8, 0.7, -4);
	fPID_tag_high = new TF1("fPID_tag_high", "[A] + exp([B]+[C]*x)", 0, 1);
	fPID_tag_high->SetParameters(1.4, 1.1, -4);
	//* For new dEdx
	// fPID_tag_low = new TF1("fPID_tag_low", "[A] + exp([B]+[C]*x)", 0, 2);
	// fPID_tag_low->SetParameters(1.0, 0.6, -3);
	// fPID_tag_high = new TF1("fPID_tag_high", "[A] + exp([B]+[C]*x)", 0, 2);
	// fPID_tag_high->SetParameters(1.2, 0.9, -2);

	fPID_tag_low->SetLineColor(6);
	fPID_tag_high->SetLineColor(6);

	//= Pion ======================================================================
	fPID_tag_pi_low = new TF1("fPID_tag_pi_low", "[A] + exp( [B] + [C] * x)", 0.0, 2);
	fPID_tag_pi_low->SetParameters(0.5, 1.2, -15);
	fPID_tag_pi_high = new TF1("fPID_tag_pi_high", "[A] + exp( [B] + [C] * x)", 0.0, 2);
	fPID_tag_pi_high->SetParameters(0.8, 1.6, -15);
	//* For new dEdx
	// fPID_tag_pi_low = new TF1("fPID_tag_pi_low", "[A] + exp([B]+[C]*x)", 0, 2);
	// fPID_tag_pi_low->SetParameters(0.85, 1.2, -15);
	// fPID_tag_pi_high = new TF1("fPID_tag_pi_high", "[A] + exp([B]+[C]*x)", 0, 2);
	// fPID_tag_pi_high->SetParameters(1.2, 1.6, -15);

	fPID_tag_pi_low->SetLineColor(6);
	fPID_tag_pi_high->SetLineColor(6);

	cout << "===============================================================" << endl;
	cout << "Tagging functions initialized" << endl;
	cout << "fPID_tag_low: " << fPID_tag_low->GetExpFormula() << endl;
	cout << "fPID_tag_high: " << fPID_tag_high->GetExpFormula() << endl;
	cout << "===============================================================" << endl;
}
bool pass_tag_kaon_tight(ParticleTree *tree, int dauIdx, int isMC)
{
	double cut_dEdx = 4.0;
	double cut_LogdEdx = ROOT::Math::log(cut_dEdx);
	double cut_p = 0.15;

	double p = tree->cand_p->at(dauIdx);
	int trkIdx = tree->cand_trkIdx->at(dauIdx);
	double dEdx = tree->trk_dEdx_dedxPixelHarmonic2->at(trkIdx);
	// double dEdx = tree->trk_dEdx_energyLossProducer_energyLossAllHits->at(trkIdx);
	double logdEdx = ROOT::Math::log(dEdx);

	bool pass_curve_single = pass_LogdEdx_curve_single(tree, dauIdx, isMC);
	bool pass_tight = logdEdx > cut_LogdEdx;

	return pass_curve_single && pass_tight;
}
bool pass_tag_kaon(ParticleTree *tree, int dauIdx, int isMC)
{
	if ( !fPID_tag_low || !fPID_tag_high )
	{
		throw std::runtime_error("Tagging functions not initialized");
	}

	int trkIdx = tree->cand_trkIdx->at(dauIdx);

	double p = tree->cand_p->at(dauIdx);
	double dEdx = tree->trk_dEdx_dedxPixelHarmonic2->at(trkIdx);
	// double dEdx = tree->trk_dEdx_energyLossProducer_energyLossAllHits->at(trkIdx);
	double logdEdx = ROOT::Math::log(dEdx);

	double band_low = fPID_tag_low->Eval(p);
	double band_high = fPID_tag_high->Eval(p);

	if ( isMC == 1 )
	{
		double shift = get_shift(p, tree->cand_charge->at(dauIdx));
		logdEdx += shift;
	}

	if ( (band_low < logdEdx) && (logdEdx < band_high) ) return true;

	return false;
}
bool pass_tag_kaon_cand(ParticleTree *tree, int icand, int isMC)
{
	// check if the dE/dx is within strict kaon band
	// the band is manually set

	auto daughters = tree->cand_dauIdx->at(icand);
	if (daughters.size() != 2) cout << "WARNING! pass_tag_kaon_cand: daughters.size() != 2" << endl;

	for (int idau = 0; idau < daughters.size(); idau++)
	{
		// * only one daughter needs to pass the cut
		//! Don't shift the dEdx for the tagging
		if ( pass_tag_kaon(tree, daughters[idau], 0) ) return true;
	}

	return false;
}

bool pass_tag_pion(ParticleTree *tree, int icand)
{
	// check if the dE/dx is within strict pion band
	// the band is manually set
	if ( !fPID_tag_pi_low || !fPID_tag_pi_high )
	{
		throw std::runtime_error("Tagging functions not initialized");
	}

	auto daughters = tree->cand_dauIdx->at(icand);
	for (int idau = 0; idau < daughters.size(); idau++)
	{
		int dauIdx = daughters[idau];
		int trkIdx = tree->cand_trkIdx->at(dauIdx);

		double p = tree->cand_p->at(dauIdx);
		double dEdx = tree->trk_dEdx_dedxPixelHarmonic2->at(trkIdx);
		// double dEdx = tree->trk_dEdx_energyLossProducer_energyLossAllHits->at(trkIdx);
		double logdEdx = ROOT::Math::log(dEdx);

		double band_low = fPID_tag_pi_low->Eval(p);
		double band_high = fPID_tag_pi_high->Eval(p);

		if ( (band_low < logdEdx) && (logdEdx < band_high) ) return true;
	}

	return false;
}

bool pass_PID(ParticleTree *tree, const int icand, int isMC)
{
	//PID Cut
	if (mPIDOpt == 0)
	{
		cout << "WARNING! goodDiKa: mPIDOpt = 0" << endl;
	}
	else if (mPIDOpt == 1)
	{
		if ( !pass_discriminator(tree, icand, isMC) ) return false;
	}
	else if (mPIDOpt == 2)
	{
		if ( !pass_LogdEdx_curve(tree, icand, isMC) ) return false;
	}
	else
	{
		throw std::runtime_error("goodDiKa: mPIDOpt not defined");
	}

	return true;
}

bool goodDiKa(ParticleTree *tree, const int icand, int isMC = 0, TH1D *hnEvts = nullptr)
{
	// pass acceptance
	bool pass_acc = pass_acc_gen_cand(tree, icand);
	
	// pass TrkQuality
	bool pass_trk = pass_TrkQuality_cand(tree, icand);

	// pass PID
	bool pass_pid = pass_PID(tree, icand, isMC);

	if(hnEvts)
	{
		if (pass_acc) hnEvts->Fill(5);
		if (pass_acc && pass_trk) hnEvts->Fill(6);
		if (pass_acc && pass_trk && pass_pid) hnEvts->Fill(7);
	}

	return pass_acc && pass_trk && pass_pid;
}

void fill_dEdx_dau(ParticleTree *tree, TH3D *h3D, int icand)
{
	auto daughters = tree->cand_dauIdx->at(icand);
	
	for (int idau = 0; idau < daughters.size(); idau++)
	{
		// if (tree->cand_status->at(idau) != 1) continue;

		int dauIdx = daughters[idau];

		int trkIdx = tree->cand_trkIdx->at(dauIdx);

		double p = tree->cand_p->at(dauIdx);
		double eta = tree->cand_eta->at(dauIdx);

		double dEdx = tree->trk_dEdx_dedxPixelHarmonic2->at(trkIdx);
		// double dEdx = tree->trk_dEdx_energyLossProducer_energyLossAllHits->at(trkIdx);
		double logdEdx = ROOT::Math::log(dEdx);

		h3D->Fill(p, dEdx, logdEdx);
	}
}

void fill_dEdx_dau_charged(ParticleTree *tree, TH3D *h3D_pos, TH3D *h3D_neg,  int icand)
{
	auto daughters = tree->cand_dauIdx->at(icand);

	for (int idau = 0; idau < daughters.size(); idau++)
	{
		int dauIdx = daughters[idau];

		int trkIdx = tree->cand_trkIdx->at(dauIdx);

		double p = tree->cand_p->at(dauIdx);
		double eta = tree->cand_eta->at(dauIdx);
		int charge = tree->cand_charge->at(dauIdx);

		double dEdx = tree->trk_dEdx_dedxPixelHarmonic2->at(trkIdx);
		// double dEdx = tree->trk_dEdx_energyLossProducer_energyLossAllHits->at(trkIdx);
		double logdEdx = ROOT::Math::log(dEdx);

		if (charge > 0)
		{
			h3D_pos->Fill(p, dEdx, logdEdx);
		}
		else if (charge < 0)
		{
			h3D_neg->Fill(p, dEdx, logdEdx);
		}
		else
		{
			cout << "ERROR! fill_dEdx_dau_charged: charge = 0" << endl;
		}
	}
}

void fill_PtEtaPhi_dau_charge(ParticleTree *tree, TH3D *h3D_pos, TH3D *h3D_neg,  int icand)
{
	auto daughters = tree->cand_dauIdx->at(icand);

	for (int idau = 0; idau < daughters.size(); idau++)
	{
		int dauIdx = daughters[idau];

		double pt = tree->cand_pT->at(dauIdx);
		double eta = tree->cand_eta->at(dauIdx);
		double phi = tree->cand_phi->at(dauIdx);
		int charge = tree->cand_charge->at(dauIdx);

		if (charge > 0)
		{
			h3D_pos->Fill(pt, eta, phi);
		}
		else if (charge < 0)
		{
			h3D_neg->Fill(pt, eta, phi);
		}
		else
		{
			cout << "ERROR! fill_PtEtaPhi_dau_charge: charge = 0" << endl;
		}
	}
}

void fill_Pixel_dau(ParticleTree *tree, TH3D *hnHit, TH3D *hnLayer, int icand)
{
	auto daughters = tree->cand_dauIdx->at(icand);

	for (int idau = 0; idau < daughters.size(); idau++)
	{
		int dauIdx = daughters[idau];

		int trkIdx = tree->cand_trkIdx->at(dauIdx);

		int nHitPixel = tree->trk_nHitPixel->at(trkIdx);
		int nHitPixelBarrel = tree->trk_nHitPixelBarrel->at(trkIdx);
		int nHitPixelEndcap = tree->trk_nHitPixelEndcap->at(trkIdx);

		int nLayerPixel = tree->trk_nLayerPixel->at(trkIdx);
		int nLayerPixelBarrel = tree->trk_nLayerPixelBarrel->at(trkIdx);
		int nLayerPixelEndcap = tree->trk_nLayerPixelEndcap->at(trkIdx);

		hnHit->Fill(nHitPixel, nHitPixelBarrel, nHitPixelEndcap);
		hnLayer->Fill(nLayerPixel, nLayerPixelBarrel, nLayerPixelEndcap);
	}
}


#endif