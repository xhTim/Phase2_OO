
#ifndef MATCHING_H
#define MATCHING_H

const double mPtResThreshold = 1.0;
const double mDeltaRThreshold = 1.0;

double get_DeltaR(ParticleTree *tree, const int igen, const int icand)
{
	auto pt_gen = tree->gen_pT->at(igen);
	auto eta_gen = tree->gen_eta->at(igen);
	auto phi_gen = tree->gen_phi->at(igen);

	auto pt_reco = tree->cand_pT->at(icand);
	auto eta_reco = tree->cand_eta->at(icand);
	auto phi_reco = tree->cand_phi->at(icand);

	TVector3 P3_gen;
	P3_gen.SetPtEtaPhi(pt_gen, eta_gen, phi_gen);
	TVector3 P3_reco;
	P3_reco.SetPtEtaPhi(pt_reco, eta_reco, phi_reco);

	return P3_gen.DeltaR(P3_reco);
}

double get_PtRes(ParticleTree *tree, const int igen, const int icand)
{
	auto pt_gen = tree->gen_pT->at(igen);
	auto pt_reco = tree->cand_pT->at(icand);

	return (pt_reco - pt_gen) / pt_gen;
}

bool reco_match_gen(ParticleTree *tree, const int icand, TH2D *hPtResDeltaR = nullptr, double PtRes_threshold = mPtResThreshold, double DeltaR_threshold = mDeltaRThreshold)
{
	// check if the reconstructed particle is matched to the generated one
	// DeltaPt/genPt < 0.15 ensures to keep 99.9842% events
	// DeltaR < 0.05 ensures to keep 99.9763% events
	// DeltaR < BestDeltaR ensures to keep only the best match
	
	//loop over all gen particles to find the best match
	double BestDeltaR = 99999;
	double BestPtRes = 99999;
	int BestIdx = -1;
	int charge_reco = tree->cand_charge->at(icand);

	for (int igen = 0; igen < tree->gen_pdgId->size(); igen++)
	{
		int pdgId = tree->gen_pdgId->at(igen);
		int status_gen = tree->gen_status->at(igen);
		int charge_gen = tree->gen_charge->at(igen);
		if (isJpsi(pdgId) || status_gen != 1) continue;
		if (charge_reco != charge_gen) continue;

		double dR = get_DeltaR(tree, igen, icand);
		double PtRes = get_PtRes(tree, igen, icand);

		if (dR < BestDeltaR)
		{
			BestDeltaR = dR;
			BestPtRes = PtRes;
			BestIdx = igen;
		}
	}

	if (BestIdx == -1) return false;

	if (hPtResDeltaR)	hPtResDeltaR->Fill(BestPtRes, BestDeltaR);

	return (BestDeltaR < DeltaR_threshold) && (abs(BestPtRes) < PtRes_threshold);
}

bool pass_reco_match_gen_cand(ParticleTree *tree, const int icand, TH2D *hPtResDeltaR = nullptr, double PtRes_threshold = mPtResThreshold, double DeltaR_threshold = mDeltaRThreshold)
{
	auto daughters = tree->cand_dauIdx->at(icand);
	
	if (daughters.size() != 2)
	{
		cout << "Error: cand_dauIdx->size() != 2" << endl;
		return false;
	}

	bool pass_dau1 = reco_match_gen(tree, daughters[0], hPtResDeltaR, PtRes_threshold, DeltaR_threshold);
	bool pass_dau2 = reco_match_gen(tree, daughters[1], hPtResDeltaR, PtRes_threshold, DeltaR_threshold);

	return pass_dau1 && pass_dau2;
}

void reco_match_gen_fillMass(ParticleTree *tree, const int icand, TH3D *hAbsPtResDeltaRMass = nullptr, TH2D *hPtResDeltaR = nullptr)
{	
	//loop over all gen particles to find the best match
	double BestDeltaR = 99999;
	double BestPtRes = 99999;
	int BestIdx = -1;
	int charge_reco = tree->cand_charge->at(icand);

	for (int igen = 0; igen < tree->gen_pdgId->size(); igen++)
	{
		int pdgId = tree->gen_pdgId->at(igen);
		int status_gen = tree->gen_status->at(igen);
		int charge_gen = tree->gen_charge->at(igen);
		if (isJpsi(pdgId) || status_gen != 1) continue;
		if (charge_reco != charge_gen) continue;

		double dR = get_DeltaR(tree, igen, icand);
		double PtRes = get_PtRes(tree, igen, icand);

		if (dR < BestDeltaR)
		{
			BestDeltaR = dR;
			BestPtRes = PtRes;
			BestIdx = igen;
		}
	}

	if (BestIdx == -1) return;

	auto mothers = tree->cand_momIdx->at(icand);
	for (auto imom : mothers)
	{
		if (abs(tree->cand_pdgId->at(imom)) == 13)
		{
			if (! check_dau_cand(tree, imom) ) continue;
			double mass = tree->cand_mass->at(imom);
			if (hAbsPtResDeltaRMass)	hAbsPtResDeltaRMass->Fill(abs(BestPtRes), BestDeltaR, mass);
			if (hPtResDeltaR)	hPtResDeltaR->Fill(BestPtRes, BestDeltaR);
		}
	}
}

void reco_match_gen_fillMass_cand(ParticleTree *tree, const int icand, TH3D *hAbsPtResDeltaRMass_pos = nullptr, TH3D *hAbsPtResDeltaRMass_neg = nullptr, TH2D *hPtResDeltaR = nullptr)
{
	auto daughters = tree->cand_dauIdx->at(icand);
	
	if (daughters.size() != 2)
	{
		cout << "Error: cand_dauIdx->size() != 2" << endl;
		return;
	}
	int dauIdx_pos = (tree->cand_charge->at(daughters[0]) > 0) ? daughters[0] : daughters[1];
	int dauIdx_neg = (tree->cand_charge->at(daughters[0]) < 0) ? daughters[0] : daughters[1];

	reco_match_gen_fillMass(tree, dauIdx_pos, hAbsPtResDeltaRMass_pos, hPtResDeltaR);
	reco_match_gen_fillMass(tree, dauIdx_neg, hAbsPtResDeltaRMass_neg, hPtResDeltaR);
}


bool gen_match_reco(ParticleTree *tree, const int igen, TH2D *hPtResDeltaR = nullptr, double PtRes_threshold = mPtResThreshold, double DeltaR_threshold = mDeltaRThreshold)
{
	// check if the generated particle is matched to the reconstructed one
	// DeltaPt/genPt < 0.15 ensures to keep 99.9842% events
	// DeltaR < 0.05 ensures to keep 99.9763% events
	// DeltaR < BestDeltaR ensures to keep only the best match
	
	//loop over all reco particles to find the best match
	double BestDeltaR = 99999;
	double BestPtRes = 99999;
	int BestIdx = -1;
	int charge_gen = tree->gen_charge->at(igen);
	bool print = false;

	for (int icand = 0; icand < tree->cand_pdgId->size(); icand++)
	{
		int pdgId = tree->cand_pdgId->at(icand);
		int status_reco = tree->cand_status->at(icand);
		int charge_reco = tree->cand_charge->at(icand);
		if (isJpsi(pdgId) || status_reco != 1) continue;
		if (charge_gen != charge_reco) continue;

		double dR = get_DeltaR(tree, igen, icand);
		double PtRes = get_PtRes(tree, igen, icand);

		if (dR < BestDeltaR)
		{
			BestDeltaR = dR;
			BestPtRes = PtRes;
			BestIdx = icand;
		}
	}

	if (BestIdx == -1) return false;

	if (hPtResDeltaR)	hPtResDeltaR->Fill(BestPtRes, BestDeltaR);

	return (BestDeltaR < DeltaR_threshold) && (abs(BestPtRes) < PtRes_threshold);
}

bool pass_gen_match_reco_cand(ParticleTree *tree, const int igen, TH2D *hPtResDeltaR = nullptr, double PtRes_threshold = mPtResThreshold, double DeltaR_threshold = mDeltaRThreshold)
{
	auto daughters = tree->gen_dauIdx->at(igen);
	
	if (daughters.size() != 2)
	{
		cout << "Error: cand_dauIdx->size() != 2" << endl;
		return false;
	}

	bool pass_dau1 = gen_match_reco(tree, daughters[0], hPtResDeltaR, PtRes_threshold, DeltaR_threshold);
	bool pass_dau2 = gen_match_reco(tree, daughters[1], hPtResDeltaR, PtRes_threshold, DeltaR_threshold);

	return pass_dau1 && pass_dau2;
}

#endif
