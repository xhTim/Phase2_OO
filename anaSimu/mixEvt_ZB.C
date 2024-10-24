#include "../common_ZB/headers.h"
#include "../common_ZB/constants.h"

TH3D *hMvsPtvsRap;
TH3D *hMvsPtvsRap_NeuDir[2][2];

TH3D *hMvsPtvsRap_gg;
TH3D *hMvsPtvsRap_NeuDir_gg[2][2];

void resetError();
void writeHistos(TString fileName = "test");

void mixEvt_ZB()
{
    hMvsPtvsRap = (TH3D *)(new TFile("outFiles/ana_mc_CohJpsi_ZB.root"))->Get("hMvsPtvsRap");
    hMvsPtvsRap_gg = (TH3D *)(new TFile("outFiles/ana_mc_gg_ZB.root"))->Get("hMvsPtvsRap");

    hMvsPtvsRap_NeuDir[0][0] = (TH3D *)(new TFile("outFiles/ana_mc_CohJpsi_0n0n_ZB.root"))->Get("hMvsPtvsRap");
    hMvsPtvsRap_NeuDir_gg[0][0] = (TH3D *)(new TFile("outFiles/ana_mc_gg_0n0n_ZB.root"))->Get("hMvsPtvsRap");

    hMvsPtvsRap_NeuDir[0][1] = (TH3D *)(new TFile("outFiles/ana_mc_CohJpsi_0nXn_ZB.root"))->Get("hMvsPtvsRap");
    hMvsPtvsRap_NeuDir_gg[0][1] = (TH3D *)(new TFile("outFiles/ana_mc_gg_0nXn_ZB.root"))->Get("hMvsPtvsRap");

    hMvsPtvsRap_NeuDir[1][0] = (TH3D *)(new TFile("outFiles/ana_mc_CohJpsi_0nXn_ZB.root"))->Get("hMvsPtvsRap");
    hMvsPtvsRap_NeuDir_gg[1][0] = (TH3D *)(new TFile("outFiles/ana_mc_gg_0nXn_ZB.root"))->Get("hMvsPtvsRap");

    hMvsPtvsRap_NeuDir[1][1] = (TH3D *)(new TFile("outFiles/ana_mc_CohJpsi_XnXn_ZB.root"))->Get("hMvsPtvsRap");
    hMvsPtvsRap_NeuDir_gg[1][1] = (TH3D *)(new TFile("outFiles/ana_mc_gg_XnXn_ZB.root"))->Get("hMvsPtvsRap");

    hMvsPtvsRap->Add(hMvsPtvsRap_gg);
    for (int ip = 0; ip < 2; ip++)
	{
		for (int im = 0; im < 2; im++)
		{
			hMvsPtvsRap_NeuDir[ip][im]->Add(hMvsPtvsRap_NeuDir_gg[ip][im]);
		}
	}
    hMvsPtvsRap_NeuDir[0][1]->Scale(0.5);
    hMvsPtvsRap_NeuDir[1][0]->Scale(0.5);

    //resetError();

    hMvsPtvsRap->SetName("hMvsPtvsRap");
    for (Int_t ip = 0; ip < 2; ip++)
    {
        for (Int_t im = 0; im < 2; im++)
        {
            hMvsPtvsRap_NeuDir[ip][im]->SetName(Form("hMvsPtvsRap_NeuDir%dp%dm", ip, im));
        }
    }

    writeHistos("outFiles/rawSig");
}

void resetError(){

	for (Int_t ibinx = 1; ibinx <= hMvsPtvsRap->GetNbinsX(); ibinx++)
	{
		for (Int_t ibiny = 1; ibiny <= hMvsPtvsRap->GetNbinsY(); ibiny++)
		{
			for (Int_t ibinz = 1; ibinz <= hMvsPtvsRap->GetNbinsZ(); ibinz++)
			{
				hMvsPtvsRap->SetBinError(ibinx, ibiny, ibinz, sqrt(hMvsPtvsRap->GetBinContent(ibinx, ibiny, ibinz)));
			}
		}
	}

	for (Int_t ip = 0; ip < 2; ip++)
	{
		for (Int_t im = 0; im < 2; im++)
		{
			for (Int_t ibinx = 1; ibinx <= hMvsPtvsRap_NeuDir[ip][im]->GetNbinsX(); ibinx++)
			{
				for (Int_t ibiny = 1; ibiny <= hMvsPtvsRap_NeuDir[ip][im]->GetNbinsY(); ibiny++)
				{
					for (Int_t ibinz = 1; ibinz <= hMvsPtvsRap_NeuDir[ip][im]->GetNbinsZ(); ibinz++)
					{
						hMvsPtvsRap_NeuDir[ip][im]->SetBinError(ibinx, ibiny, ibinz, sqrt(hMvsPtvsRap_NeuDir[ip][im]->GetBinContent(ibinx, ibiny, ibinz)));
					}
				}
			}
		}
	}
}

void writeHistos(TString fileName){

	TFile *fOut = new TFile(Form("%s_ZB.root", fileName.Data()), "recreate");

	fOut->cd();

	hMvsPtvsRap->Write();

	for (Int_t ip = 0; ip < 2; ip++)
	{
		for (Int_t im = 0; im < 2; im++)
		{
			hMvsPtvsRap_NeuDir[ip][im]->Write();
		}
	}

	fOut->Close();
}
