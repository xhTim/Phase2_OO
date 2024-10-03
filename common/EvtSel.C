
//################################################################################
//# Don't ask me to explain this code, I myself don't understand it.
//#		And my mom told me not to talk to strangers...
//################################################################################


#ifndef EVTSEL_H
#define EVTSEL_H

bool check_dau_basic_cand(ParticleTree *tree, int icand)
{
	//? check the daughter basic information

	//* The candidate should have 2 daughters
	auto daughters = tree->cand_dauIdx->at(icand);
	if (daughters.size() != 2)
	{
		cout << "daughters.size() != 2,	sondern " << daughters.size() << endl;
		return false;
	}

	int dauIdx_1 = daughters[0];
	int dauIdx_2 = daughters[1];

	//*check the charge of the daughters are correct
	if (tree->cand_charge->at(dauIdx_1) == 0 || tree->cand_charge->at(dauIdx_2) == 0)	return false;
	if (tree->cand_charge->at(dauIdx_1) == tree->cand_charge->at(dauIdx_2))				return false;

	//*check status of both daughters
	if (tree->cand_status->at(dauIdx_1) != 1 || tree->cand_status->at(dauIdx_2) != 1)	return false;

	return true;
}

bool check_dau_isHP_cand(ParticleTree *tree, int icand)
{
	//? check the daughter

	auto daughters = tree->cand_dauIdx->at(icand);
	if (daughters.size() != 2)
	{
		cout << "daughters.size() != 2" << endl;
		return false;
	}

	int dauIdx_1 = daughters[0];
	int dauIdx_2 = daughters[1];

	// check the track index of both daughters is HP
	if (tree->trk_isHP->at(tree->cand_trkIdx->at(dauIdx_1)) != 1 || tree->trk_isHP->at(tree->cand_trkIdx->at(dauIdx_2)) != 1) return false;

	return true;
}

bool check_dau_cand(ParticleTree *tree, int icand)
{
	//? check the daughter

	//*check the daughter basic information
	if ( !check_dau_basic_cand(tree, icand) ) return false;

	//*check the track index of both daughters is HP
	if ( !check_dau_isHP_cand(tree, icand) ) return false;

	return true;
}

bool check_soft_muons(ParticleTree *tree, int icand)
{
	auto daughters = tree->cand_dauIdx->at(icand);
	if (daughters.size() != 2)
	{
		cout << "daughters.size() != 2" << endl;
		return false;
	}

	int dauIdx_1 = daughters[0];
	int dauIdx_2 = daughters[1];

	bool pass = false;

	for (int imuon = 0; imuon < tree->muon_candIdx->size(); imuon++)
	{
		if ((tree->muon_candIdx->at(imuon) == dauIdx_1 || tree->muon_candIdx->at(imuon) == dauIdx_2) && tree->muon_softID->at(imuon))
			pass = true;
	}

	return pass;
}

const vector<int> mHLT_Idx = {0};
const vector<int> mHLT_Idx_OLD = {3};	//? for the old data, the HLT trigger is at index 3

bool passHLT(ParticleTree *tree, std::vector<int> HLT_Idx = mHLT_Idx)
{
	//return true if pass any of the HLT trigger in the HLT_Idx
	for (auto iHLT : HLT_Idx)
	{
		if (tree->passHLT->at(iHLT)) return true;
	}
	return false;
}

bool pass_PixelTrkQuality(ParticleTree *tree, int idau)
{
	int iTrk = tree->cand_trkIdx->at(idau);
	int nLayerPixel = tree->trk_nLayer->at(iTrk);
	int nHitPixel = tree->trk_nHit->at(iTrk);

	bool pass_nLayerPixel = nLayerPixel > 0;
	bool pass_nHitPixel = nHitPixel > 0;

	return pass_nLayerPixel && pass_nHitPixel;
}

bool pass_PixelTrkQuality_cand(ParticleTree *tree, int icand)
{
	auto daughters = tree->cand_dauIdx->at(icand);
	bool pass_dau1 = pass_PixelTrkQuality(tree, daughters[0]);
	bool pass_dau2 = pass_PixelTrkQuality(tree, daughters[1]);

	return pass_dau1 && pass_dau2;
}

bool pass_TrkQuality(ParticleTree *tree, int idau)
{
	double xyDCASig_cut = 3.0;
	double zDCASig_cut = 3.0;

	int iTrk = tree->cand_trkIdx->at(idau);

	double xyDCASig = tree->trk_xyDCASignificance->at(iTrk);
	double zDCASig = tree->trk_zDCASignificance->at(iTrk);

	bool pass_xyDCASig = fabs(xyDCASig) < xyDCASig_cut;
	bool pass_zDCASig = fabs(zDCASig) < zDCASig_cut;

	return pass_xyDCASig && pass_zDCASig;
}

bool pass_TrkQuality_cand(ParticleTree *tree, int icand)
{
	auto daughters = tree->cand_dauIdx->at(icand);
	bool pass_dau1 = pass_TrkQuality(tree, daughters[0]);
	bool pass_dau2 = pass_TrkQuality(tree, daughters[1]);

	return pass_dau1 && pass_dau2;
}

bool pass_acc_gen(const double pt, const double eta)
{
	// check if the gen trk is within acceptance 
	const Double_t mPtTh = 0.05,  mEtaThLow = 0, mEtaThHi = 2.4;

	if ( pt < mPtTh ) return false;
	// if ( fabs(eta) > mEtaThHi || fabs(eta) < mEtaThLow ) return false;
	if ( fabs(eta) > mEtaThHi ) return false;

	return true;
}

bool pass_acc_gen_cand(ParticleTree *tree, int icand)
{
	auto daughters = tree->cand_dauIdx->at(icand);
	int dau_Idx_1 = daughters[0];
	int dau_Idx_2 = daughters[1];

	double pt_dau_1 = tree->cand_pT->at(dau_Idx_1);
	double pt_dau_2 = tree->cand_pT->at(dau_Idx_2);
	double eta_dau_1 = tree->cand_eta->at(dau_Idx_1);
	double eta_dau_2 = tree->cand_eta->at(dau_Idx_2);

	bool pass_dau1 = pass_acc_gen(pt_dau_1, eta_dau_1);
	bool pass_dau2 = pass_acc_gen(pt_dau_2, eta_dau_2);

	return pass_dau1 && pass_dau2;
}

bool pass_EvtSel_standard(ParticleTree *tree, TH1D *hnEvts = nullptr, int PVFilter_Idx = mPVFilterIdx, int HFVeto_option = mHFVetoOpt, std::vector<int> HLT_Idx = mHLT_Idx)
{
	//= Event Selection ======================================================================
	// evtSel->at(3) = Flag_primaryVertexFilterRecoveryForUPC

	bool pass_NtrkHP = tree->NtrkHP == 2;
	//bool pass_HLTtrig = passHLT(tree, HLT_Idx);
	//bool pass_HFveto = false;
	//if (HFVeto_option == 0) pass_HFveto = tree->PFHFmaxEPlus < mHFVetoPlus && tree->PFHFmaxEMinus < mHFVetoMinus;
	//else if (HFVeto_option == 1) pass_HFveto = tree->PFHFmaxEPlus < mHFVetoPlus_tight && tree->PFHFmaxEMinus < mHFVetoMinus_tight;
	//else if (HFVeto_option == 2) pass_HFveto = tree->PFHFmaxEPlus < mHFVetoPlus_loose && tree->PFHFmaxEMinus < mHFVetoMinus_loose;
	//else cout << "HFVeto_option not defined" << endl;
	//bool pass_PVFilter = tree->evtSel->at(PVFilter_Idx);

	//if (hnEvts)
	//{
	//	hnEvts->Fill(0);
	//	if (pass_NtrkHP) hnEvts->Fill(1);
	//	if (pass_NtrkHP && pass_HLTtrig) hnEvts->Fill(2);
	//	if (pass_NtrkHP && pass_HLTtrig && pass_HFveto) hnEvts->Fill(3);
	//	if (pass_NtrkHP && pass_HLTtrig && pass_HFveto && pass_PVFilter) hnEvts->Fill(4);
	//}

	//return pass_NtrkHP && pass_HLTtrig && pass_HFveto && pass_PVFilter;
	return pass_NtrkHP;
}

bool pass_EvtSel_noHLT(ParticleTree *tree, TH1D *hnEvts = nullptr, int PVFilter_Idx = mPVFilterIdx, int HFVeto_option = mHFVetoOpt)
{
	//= Event Selection ======================================================================
	// evtSel->at(3) = Flag_primaryVertexFilterRecoveryForUPC

	bool pass_NtrkHP = tree->NtrkHP == 2;
	bool pass_HFveto = false;
	if (HFVeto_option == 0) pass_HFveto = tree->PFHFmaxEPlus < mHFVetoPlus && tree->PFHFmaxEMinus < mHFVetoMinus;
	else if (HFVeto_option == 1) pass_HFveto = tree->PFHFmaxEPlus < mHFVetoPlus_tight && tree->PFHFmaxEMinus < mHFVetoMinus_tight;
	else if (HFVeto_option == 2) pass_HFveto = tree->PFHFmaxEPlus < mHFVetoPlus_loose && tree->PFHFmaxEMinus < mHFVetoMinus_loose;
	else cout << "HFVeto_option not defined" << endl;
	bool pass_PVFilter = tree->evtSel->at(PVFilter_Idx);

	if (hnEvts)
	{
		hnEvts->Fill(0);
		if (pass_NtrkHP) hnEvts->Fill(1);
		if (pass_NtrkHP) hnEvts->Fill(2);
		if (pass_NtrkHP && pass_HFveto) hnEvts->Fill(3);
		if (pass_NtrkHP && pass_HFveto && pass_PVFilter) hnEvts->Fill(4);
	}

	return pass_NtrkHP && pass_HFveto && pass_PVFilter;
}

bool pass_EvtSel_noNtrkHP(ParticleTree *tree, TH1D *hnEvts = nullptr, int PVFilter_Idx = mPVFilterIdx, int HFVeto_option = mHFVetoOpt, std::vector<int> HLT_Idx = mHLT_Idx)
{
	//= Event Selection ======================================================================
	// evtSel->at(3) = Flag_primaryVertexFilterRecoveryForUPC

	bool pass_HLTtrig = passHLT(tree, HLT_Idx);
	bool pass_HFveto = false;
	if (HFVeto_option == 0) pass_HFveto = tree->PFHFmaxEPlus < mHFVetoPlus && tree->PFHFmaxEMinus < mHFVetoMinus;
	else if (HFVeto_option == 1) pass_HFveto = tree->PFHFmaxEPlus < mHFVetoPlus_tight && tree->PFHFmaxEMinus < mHFVetoMinus_tight;
	else if (HFVeto_option == 2) pass_HFveto = tree->PFHFmaxEPlus < mHFVetoPlus_loose && tree->PFHFmaxEMinus < mHFVetoMinus_loose;
	else cout << "HFVeto_option not defined" << endl;
	bool pass_PVFilter = tree->evtSel->at(PVFilter_Idx);

	if (hnEvts)
	{
		hnEvts->Fill(0);
		if (pass_HLTtrig) hnEvts->Fill(1);
		if (pass_HLTtrig) hnEvts->Fill(2);
		if (pass_HLTtrig && pass_HFveto) hnEvts->Fill(3);
		if (pass_HLTtrig && pass_HFveto && pass_PVFilter) hnEvts->Fill(4);
	}

	return pass_HLTtrig && pass_HFveto && pass_PVFilter;
}

#endif
