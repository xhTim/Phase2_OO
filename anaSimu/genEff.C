#include "../common/headers.h"
#include "../common/function.C"

void genEff(TString FileName)
{
    TFile *inFile = new TFile("outFiles/ana_mc_" + FileName + ".root", "READ");

    TH1D *hRap = (TH1D *)inFile->Get("hMvsPtvsRap_px");
    TH1D *hRap_gen = (TH1D *)inFile->Get("hRapPtMass_DiMu_gen_px");

    hRap = (TH1D *)rebHisto(hRap, "hRap", nDiffRapBins + 1, mDiffRapBds, "NO");
    hRap_gen = (TH1D *)rebHisto(hRap_gen, "hRap_gen", nDiffRapBins + 1, mDiffRapBds, "NO");

    TH1D *hRap_Symm = (TH1D *)hRap->Clone("hEffvsRap_Symm");
    TH1D *hRap_gen_Symm = (TH1D *)hRap_gen->Clone("hEffvsRap_gen_Symm");

    for (int i = nDiffRapBins / 2 + 1; i < nDiffRapBins + 1; i++)
    {
        hRap_Symm->Fill(hRap->GetBinCenter(i + 1), hRap->GetBinContent(nDiffRapBins + 1 - i));
        hRap_gen_Symm->Fill(hRap_gen->GetBinCenter(i + 1), hRap_gen->GetBinContent(nDiffRapBins + 1 - i));

        hRap_Symm->SetBinError(i + 1, sqrt(hRap_Symm->GetBinContent(i + 1)));
        hRap_gen_Symm->SetBinError(i + 1, sqrt(hRap_gen_Symm->GetBinContent(i + 1)));
    }

    TH1D *hEffvsRap_Symm = (TH1D *)hRap_Symm->Clone("hEffvsRap_Symm_" + FileName);
    hEffvsRap_Symm->Divide(hRap_gen_Symm);
    hEffvsRap_Symm->SetTitle(";y;");

    TFile *fOut = new TFile("EffundTemp/Eff_" + FileName + ".root", "recreate");
    hEffvsRap_Symm->Write();
    fOut->Close();
}
