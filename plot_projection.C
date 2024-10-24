#include "common/headers.h"
#include "common/function.C"
#include "disentanglement/ParamConverter.C"

TCanvas *c;

TLegend *leg;

TGraphErrors *g_ditrk_jpsi;
TGraphErrors *g_mt_jpsi;
TGraphErrors *g_midrap_jpsi;
TGraphErrors *g_syserr_ditrk_jpsi;
TGraphErrors *g_syserr_mt_jpsi;
TGraphAsymmErrors *g_syserr_midrap_jpsi;

TGraphErrors *g_mt_psi;
TGraphErrors *g_ditrk_psi;
TGraphErrors *g_syserr_mt_psi;
TGraphErrors *g_syserr_ditrk_psi;

TGraphErrors *g_upsilon;
TGraphErrors *g_syserr_upsilon;

double mMarkerSize = 1.2;

void plot_jpsi();
void plot_psi2s();
void plot_upsilon1s();

void plot_data();

void plot_projection()
{
    c = new TCanvas("c", "", 0, 0, 800, 600);
    c->SetLogy();

    leg = new TLegend(.3, .4, .5, .5, "");
    leg->SetTextSize(0.03);

    // plot_data();
    plot_jpsi();
    // plot_psi2s();
    // plot_upsilon1s();

    drawLatex(0.56, 0.92, "PbPb 7 nb^{-1} (5.5 TeV)", 42, 0.05, 1);
    drawLatex(0.1, 0.92, "#bf{CMS} #it{Simulation}", 42, 0.045, 1);
    drawLatex(0.15, 0.84, "Projected J/#psi in PbPb UPC", 42, 0.045, 1);
    drawLatex(0.6, 0.84, "STARlight + CMS Phase-2", 42, 0.040, 1);

    // leg->AddEntry(g_mt_psi, "#psi(2S) with Muon + Track");
    // leg->AddEntry(g_ditrk_psi, "#psi(2S) with Di-track (zero bias trigger)");
    // leg->AddEntry(g_upsilon, "#varUpsilon(1S) with Di-track (zero bias trigger)");
    // leg->AddEntry(g_syserr_mt_jpsi, "Syst. #gamma flux");
    // leg->SetTextSize(0.030);

    c->cd();
    leg->Draw();
    c->SaveAs("projection_jpsi.pdf");
}

void plot_jpsi()
{
    TFile *file_mt = TFile::Open("disentanglement/outFiles/Result_CMS.root");
    TFile *file_sysuncer_mt = TFile::Open("disentanglement/outFiles/Result_CMS_SysUncer.root");
    TFile *file_ditrk = TFile::Open("disentanglement_ZB/outFiles/Result_CMS_ZB.root");
    TFile *file_sysuncer_ditrk = TFile::Open("disentanglement_ZB/outFiles/Result_CMS_SysUncer_ZB.root");
    TFile *file_midrap_ditrk = TFile::Open("disentanglement_ZB_MidRap/outFiles/Result_MidRap.root");

    auto W_mt = (std::vector<double> *)file_mt->Get("W");
    auto W_ditrk = (std::vector<double> *)file_ditrk->Get("W");
    auto Dy_midrap = (std::vector<double> *)file_midrap_ditrk->Get("Dy");
    std::vector<double>* W_midrap = new std::vector<double>();
    for (int i = 0; i < Dy_midrap->size(); i++)
    {
        W_midrap->push_back(ParamConverter::y2W((*Dy_midrap)[i]));
    }

    auto sigma_mt = (std::vector<double> *)file_mt->Get("Sigma");
    auto sigma_ditrk = (std::vector<double> *)file_ditrk->Get("Sigma");
    auto sigma_midrap = (std::vector<double> *)file_midrap_ditrk->Get("Sigma");

    auto sigma_err_mt = (std::vector<double> *)file_mt->Get("Sigma_Err");
    auto sigma_err_ditrk = (std::vector<double> *)file_ditrk->Get("Sigma_Err");
    auto sigma_err_midrap = (std::vector<double> *)file_midrap_ditrk->Get("Sigma_Err");
    std::vector<double>* sigma_err_midrap_new = new std::vector<double>();
    for (int i = 0; i < sigma_err_midrap->size(); i++)
    {
        sigma_err_midrap_new->push_back((*sigma_err_midrap)[i] * sqrt(50));
    }

    auto sigma_syserr_mt = (std::vector<double> *)file_sysuncer_mt->Get("Sigma_TheorySysErr");
    auto sigma_syserr_ditrk = (std::vector<double> *)file_sysuncer_ditrk->Get("Sigma_TheorySysErr");
    auto sigma_syserrhig_midrap = (std::vector<double> *)file_midrap_ditrk->Get("Sigma_TheorySysErrHigh");
    auto sigma_syserrlow_midrap = (std::vector<double> *)file_midrap_ditrk->Get("Sigma_TheorySysErrLow");

    std::vector<double>* W_ditrk_new = new std::vector<double>();
    std::vector<double>* sigma_ditrk_new = new std::vector<double>();
    std::vector<double>* sigma_err_ditrk_new = new std::vector<double>();
    std::vector<double>* sigma_syserr_ditrk_new = new std::vector<double>();
    for (int i = 0; i < W_ditrk->size(); i++)
    {
        double theRap = fabs(ParamConverter::W2y((*W_ditrk)[i]));
        if ((theRap < 3.3 && theRap > 0.7) || theRap < 0.4) continue;
        W_ditrk_new->push_back((*W_ditrk)[i]);
        sigma_ditrk_new->push_back((*sigma_ditrk)[i]);
        sigma_err_ditrk_new->push_back((*sigma_err_ditrk)[i] * sqrt(50));
        sigma_syserr_ditrk_new->push_back((*sigma_syserr_ditrk)[i]);
    }

    g_mt_jpsi = new TGraphErrors(W_mt->size(), W_mt->data(), sigma_mt->data(), nullptr, sigma_err_mt->data());
    cout<<"CHECK: "<<W_mt->size()<<"\t"<<sigma_mt->size()<<endl;
    g_ditrk_jpsi = new TGraphErrors(W_ditrk_new->size(), W_ditrk_new->data(), sigma_ditrk_new->data(), nullptr, sigma_err_ditrk_new->data());
    g_midrap_jpsi = new TGraphErrors(W_midrap->size(), W_midrap->data(), sigma_midrap->data(), nullptr, sigma_err_midrap_new->data());

    g_mt_jpsi->SetMarkerColor(2);
    g_mt_jpsi->SetLineColor(2);
    g_mt_jpsi->SetMarkerSize(mMarkerSize);
    g_mt_jpsi->SetMarkerStyle(20);

    g_ditrk_jpsi->SetTitle("");
    g_ditrk_jpsi->GetXaxis()->SetTitle("W_{#gammaN}^{Pb} (GeV)");
    g_ditrk_jpsi->GetYaxis()->SetTitle("#sigma(#gamma+Pb #rightarrow VM+Pb) (mb)");
    g_ditrk_jpsi->GetYaxis()->SetTitleOffset(1.1);
    g_ditrk_jpsi->GetXaxis()->SetTitleSize(0.04);
    g_ditrk_jpsi->GetYaxis()->SetTitleSize(0.04);
    g_ditrk_jpsi->SetMarkerColor(2);
    g_ditrk_jpsi->SetLineColor(2);
    g_ditrk_jpsi->SetMarkerSize(mMarkerSize);
    g_ditrk_jpsi->SetMarkerStyle(22);

    g_midrap_jpsi->SetMarkerColor(2);
    g_midrap_jpsi->SetLineColor(2);
    g_midrap_jpsi->SetMarkerSize(mMarkerSize);
    g_midrap_jpsi->SetMarkerStyle(22);

    std::vector<double> X_AXIS_ERR_mt = std::vector<double>(W_mt->size(), 5.3);
    std::vector<double> X_AXIS_ERR_ditrk = std::vector<double>(W_ditrk_new->size(), 5.3);
    std::vector<double> X_AXIS_ERR_midrap = std::vector<double>(W_midrap->size(), 5.3);

    g_syserr_mt_jpsi = new TGraphErrors(W_mt->size(), W_mt->data(), sigma_mt->data(), X_AXIS_ERR_mt.data(), sigma_syserr_mt->data());
    g_syserr_ditrk_jpsi = new TGraphErrors(W_ditrk_new->size(), W_ditrk_new->data(), sigma_ditrk_new->data(), X_AXIS_ERR_ditrk.data(), sigma_syserr_ditrk_new->data());
    g_syserr_midrap_jpsi = new TGraphAsymmErrors(W_midrap->size(), W_midrap->data(), sigma_midrap->data(), X_AXIS_ERR_midrap.data(), X_AXIS_ERR_midrap.data(), sigma_syserrlow_midrap->data(), sigma_syserrhig_midrap->data());

    g_syserr_mt_jpsi->SetMarkerColor(0);
    g_syserr_mt_jpsi->SetFillColorAlpha(1, 0);
    g_syserr_mt_jpsi->SetFillStyle(2);

    g_syserr_ditrk_jpsi->SetMarkerColor(0);
    g_syserr_ditrk_jpsi->SetFillColorAlpha(1, 0);
    g_syserr_ditrk_jpsi->SetFillStyle(2);
    //g_ditrk_jpsi->GetYaxis()->SetRangeUser(0.000001, 1);

    g_syserr_midrap_jpsi->SetMarkerColor(0);
    g_syserr_midrap_jpsi->SetFillColorAlpha(1, 0);
    g_syserr_midrap_jpsi->SetFillStyle(2);

    leg->AddEntry(g_mt_jpsi, "Projected J/#psi with Muon + Track", "p");
    leg->AddEntry(g_ditrk_jpsi, "Projected J/#psi with Di-track (zero bias trigger)", "p");

    c->cd();
    g_ditrk_jpsi->Draw("apez");
    //g_syserr_mt_jpsi->Draw("2same");
    //g_syserr_ditrk_jpsi->Draw("2same");
    //g_syserr_midrap_jpsi->Draw("2same");
    //g_mt_jpsi->Draw("Pez");
    //g_ditrk_jpsi->Draw("Pez");
    //g_midrap_jpsi->Draw("Pez");
}

void plot_data()
{
    // CMS
    TGraphErrors *gCMS = new TGraphErrors("Data_and_Theor/CMS_stat.txt", "%lg %lg %lg");
    gCMS->SetMarkerColor(kBlue);
    gCMS->SetMarkerSize(mMarkerSize);
    gCMS->SetMarkerStyle(20);
    TGraphErrors *gCMS_exsyst = new TGraphErrors("Data_and_Theor/CMS_exsyst.txt");
//    gCMS_exsyst->SetMarkerColor(0);
    gCMS_exsyst->SetLineWidth(0);
    gCMS_exsyst->SetFillColorAlpha(16, 0.7);
    gCMS_exsyst->SetFillStyle(1001);
    TGraphErrors *gCMS_thsyst = new TGraphErrors("Data_and_Theor/CMS_thsyst.txt");
    gCMS_thsyst->SetMarkerColor(0);
    gCMS_thsyst->SetFillColorAlpha(1, 0);
    gCMS_thsyst->SetFillStyle(2);

    // ALICE
    TGraphErrors *gALICE = new TGraphErrors("Data_and_Theor/ALICE_stat.txt", "%lg %lg %lg");
    gALICE->SetMarkerColor(kBlue);
    gALICE->SetLineColor(kBlue);
    gALICE->SetMarkerSize(mMarkerSize);
    gALICE->SetMarkerStyle(24);
    TGraphAsymmErrors *gALICE_exsyst = new TGraphAsymmErrors("Data_and_Theor/ALICE_exsyst.txt");
    gALICE_exsyst->SetMarkerColor(0);
    gALICE_exsyst->SetFillColorAlpha(16, 0.7);
    gALICE_exsyst->SetFillStyle(1001);
    TGraphErrors *gALICE_thsyst = new TGraphErrors("Data_and_Theor/ALICE_thsyst.txt");
    gALICE_thsyst->SetMarkerColor(0);
    gALICE_thsyst->SetFillColorAlpha(1, 0);
    gALICE_thsyst->SetFillStyle(2);

    // LHCb
    TGraph *gLHCb = new TGraph("Data_and_Theor/LHCb_stat.txt");
    gLHCb->SetMarkerColor(kBlue);
    gLHCb->SetMarkerSize(mMarkerSize);
    gLHCb->SetMarkerStyle(26);
    TGraphErrors *gLHCb_exsyst = new TGraphErrors("Data_and_Theor/LHCb_exsyst.txt");
    gLHCb_exsyst->SetMarkerColor(0);
    gLHCb_exsyst->SetFillColorAlpha(16, 0.7);
    gLHCb_exsyst->SetFillStyle(1001);
    TGraphErrors *gLHCb_thsyst = new TGraphErrors("Data_and_Theor/LHCb_thsyst.txt");
    gLHCb_thsyst->SetMarkerColor(0);
    gLHCb_thsyst->SetFillColorAlpha(1, 0);
    gLHCb_thsyst->SetFillStyle(2);

    // Theories
    TGraph *gLTA_SS = new TGraph("Data_and_Theor/LTA_SS.txt");
    gLTA_SS->SetLineWidth(2);
    gLTA_SS->SetLineColor(2);
    TGraph *gLTA_WS = new TGraph("Data_and_Theor/LTA_WS.txt");
    gLTA_WS->SetLineWidth(2);
    gLTA_WS->SetLineColor(2);
    gLTA_WS->SetLineStyle(7);
    TGraph *gCD_BGK = new TGraph("Data_and_Theor/CD_BGK.txt");
    gCD_BGK->SetLineWidth(2);
    gCD_BGK->SetLineColor(kGreen);
    TGraph *gCD_GBW = new TGraph("Data_and_Theor/CD_GBW.txt");
    gCD_GBW->SetLineWidth(2);
    gCD_GBW->SetLineColor(kGreen);
    gCD_GBW->SetLineStyle(7);
    TGraph *gCD_IIM = new TGraph("Data_and_Theor/CD_IIM.txt");
    gCD_IIM->SetLineWidth(2);
    gCD_IIM->SetLineColor(kGreen);
    gCD_IIM->SetLineStyle(2);
    TGraph *gbBK_GG = new TGraph("Data_and_Theor/bBK_GG.txt");
    gbBK_GG->SetLineWidth(2);
    gbBK_GG->SetLineColor(kBlue);
    TGraph *gbBK_A = new TGraph("Data_and_Theor/bBK_A.txt");
    gbBK_A->SetLineWidth(2);
    gbBK_A->SetLineColor(kBlue);
    gbBK_A->SetLineStyle(7);
    TGraph *gCGC = new TGraph("Data_and_Theor/CGC_IPsat.txt");
    gCGC->SetLineWidth(2);
    gCGC->SetLineColor(kGray);
    gCGC->SetLineStyle(4);
    TGraph *gGG = new TGraph("Data_and_Theor/GG-hs.txt");
    gGG->SetLineWidth(2);
    gGG->SetLineColor(kPink);
    gGG->SetLineStyle(4);
    TGraph *gIA = new TGraph("Data_and_Theor/IA.txt");
    gIA->SetLineWidth(2);
    gIA->SetLineStyle(4);

    TH2D *hFrame = new TH2D("hFrame", ";W_{#gammaN}^{Pb} (GeV);#sigma(#gamma+Pb #rightarrow J/#psi+Pb) (mb)", 50, 0, 930, 50, 2*pow(10, -3), 0.25);
    hFrame->GetXaxis()->CenterTitle();
    hFrame->GetYaxis()->CenterTitle();
    hFrame->GetXaxis()->SetTitleSize(0.054);
    hFrame->GetXaxis()->SetTitleOffset(1);

    TLegend* leg_data = new TLegend(.3, .15, .5, .4, "");
    leg_data->AddEntry(gCMS, "CMS", "p");
    leg_data->AddEntry(gALICE, "ALICE", "p");
    leg_data->AddEntry(gLHCb, "LHCb*(-4.5<y<-3.5)", "p");
    leg_data->AddEntry(gCMS_exsyst, "Syst. exp.", "f");
    leg_data->AddEntry(gCMS_thsyst, "Syst. #gamma flux", "f");
    leg_data->SetTextSize(0.03);

    TLegend *leg_th = new TLegend(.6, .15, .9, .4, "");
    leg_th->SetNColumns(2);
    leg_th->AddEntry(gLTA_SS, "LTA_SS", "l");
    leg_th->AddEntry(gbBK_GG, "bBK_GG", "l");
    leg_th->AddEntry(gLTA_WS, "LTA_WS", "l");
    leg_th->AddEntry(gbBK_A, "bBK_A", "l");
    leg_th->AddEntry(gCD_BGK, "CD_BGK", "l");
    leg_th->AddEntry(gCGC, "CGC IPsat", "l");
    leg_th->AddEntry(gCD_GBW, "CD_GBW", "l");
    leg_th->AddEntry(gGG, "GG-hs", "l");
    leg_th->AddEntry(gCD_IIM, "CD_IIM", "l");
    leg_th->AddEntry(gIA, "Impulse approx.", "l");
    leg_th->SetTextSize(0.03);
    c->cd();
    c->SetTopMargin(0.11);
    c->SetRightMargin(0.04);
    hFrame->Draw();
    gLTA_SS->Draw("l");
    gLTA_WS->Draw("l");
    gCD_BGK->Draw("l");
    gCD_GBW->Draw("l");
    gCD_IIM->Draw("l");
    gbBK_GG->Draw("l");
    gbBK_A->Draw("l");
    gCGC->Draw("l");
    gIA->Draw("l");
    gCMS_exsyst->Draw("2same");
    gCMS_thsyst->Draw("2same");
    gALICE_exsyst->Draw("2same");
    gALICE_thsyst->Draw("2same");
    gLHCb_exsyst->Draw("2same");
    gLHCb_thsyst->Draw("2same");
    gCMS->Draw("pez");
    gALICE->Draw("pez");
    gLHCb->Draw("p");
    leg_th->Draw();
    leg_data->Draw();
}

void plot_psi2s()
{
    TFile *file_mt = TFile::Open("CohPsi2S_Prj_Dir/CohPsi2S_MT/disentanglement/outFiles/Result_CMS.root");
    TFile *file_sysuncer_mt = TFile::Open("CohPsi2S_Prj_Dir/CohPsi2S_MT/disentanglement/outFiles/Result_CMS_SysUncer.root");
    TFile *file_ditrk = TFile::Open("CohPsi2S_Prj_Dir/CohPsi2S_DiTrack/disentanglement/outFiles/Result_CMS.root");
    TFile *file_sysuncer_ditrk = TFile::Open("CohPsi2S_Prj_Dir/CohPsi2S_DiTrack/disentanglement/outFiles/Result_CMS_SysUncer.root");

    auto W_mt = (std::vector<double> *)file_mt->Get("W");
    auto W_ditrk = (std::vector<double> *)file_ditrk->Get("W");

    auto sigma_mt = (std::vector<double> *)file_mt->Get("Sigma");
    auto sigma_ditrk = (std::vector<double> *)file_ditrk->Get("Sigma");

    auto sigma_err_mt = (std::vector<double> *)file_mt->Get("Sigma_Err");
    auto sigma_err_ditrk = (std::vector<double> *)file_ditrk->Get("Sigma_Err");

    auto sigma_syserr_mt = (std::vector<double> *)file_sysuncer_mt->Get("Sigma_TheorySysErr");
    auto sigma_syserr_ditrk = (std::vector<double> *)file_sysuncer_ditrk->Get("Sigma_TheorySysErr");

    std::vector<double>* W_ditrk_new = new std::vector<double>();
    std::vector<double>* sigma_ditrk_new = new std::vector<double>();
    std::vector<double>* sigma_err_ditrk_new = new std::vector<double>();
    std::vector<double>* sigma_syserr_ditrk_new = new std::vector<double>();
    for (int i = 0; i < W_ditrk->size(); i++)
    {
        if (fabs(ParamConverter::W2y((*W_ditrk)[i])) < 1 || fabs(ParamConverter::W2y((*W_ditrk)[i])) > 3)
        {
            W_ditrk_new->push_back((*W_ditrk)[i]);
            sigma_ditrk_new->push_back((*sigma_ditrk)[i]);
            sigma_err_ditrk_new->push_back((*sigma_err_ditrk)[i] * sqrt(50));
            sigma_syserr_ditrk_new->push_back((*sigma_syserr_ditrk)[i]);
        }
    }

    g_mt_psi = new TGraphErrors(W_mt->size(), W_mt->data(), sigma_mt->data(), nullptr, sigma_err_mt->data());
    g_ditrk_psi = new TGraphErrors(W_ditrk_new->size(), W_ditrk_new->data(), sigma_ditrk_new->data(), nullptr, sigma_err_ditrk_new->data());

    g_mt_psi->SetMarkerColor(4);
    g_mt_psi->SetLineColor(4);
    g_mt_psi->SetMarkerSize(mMarkerSize);
    g_mt_psi->SetMarkerStyle(20);

    g_ditrk_psi->SetMarkerColor(4);
    g_ditrk_psi->SetLineColor(4);
    g_ditrk_psi->SetMarkerSize(mMarkerSize);
    g_ditrk_psi->SetMarkerStyle(22);

    std::vector<double> X_AXIS_ERR_mt = std::vector<double>(W_mt->size(), 3.8);
    std::vector<double> X_AXIS_ERR_ditrk = std::vector<double>(W_ditrk_new->size(), 3.8);

    g_syserr_mt_psi = new TGraphErrors(W_mt->size(), W_mt->data(), sigma_mt->data(), X_AXIS_ERR_mt.data(), sigma_syserr_mt->data());
    g_syserr_ditrk_psi = new TGraphErrors(W_ditrk_new->size(), W_ditrk_new->data(), sigma_ditrk_new->data(), X_AXIS_ERR_ditrk.data(), sigma_syserr_ditrk_new->data());

    g_syserr_mt_psi->SetMarkerColor(0);
    g_syserr_mt_psi->SetFillColorAlpha(1, 0);
    g_syserr_mt_psi->SetFillStyle(2);

    g_syserr_ditrk_psi->SetMarkerColor(0);
    g_syserr_ditrk_psi->SetFillColorAlpha(1, 0);
    g_syserr_ditrk_psi->SetFillStyle(2);

    c->cd();
    g_syserr_mt_psi->Draw("2same");
    g_syserr_ditrk_psi->Draw("2same");
    g_mt_psi->Draw("Pez");
    g_ditrk_psi->Draw("Pez");
}

void plot_upsilon1s()
{
    TFile *file = TFile::Open("CohUp_Prj_Dir/CohUp_DiTrack/disentanglement/outFiles/Result_CMS.root");
    TFile *file_sysuncer = TFile::Open("CohUp_Prj_Dir/CohUp_DiTrack/disentanglement/outFiles/Result_CMS_SysUncer.root");

    auto W = (std::vector<double> *)file->Get("W");
    auto sigma = (std::vector<double> *)file->Get("Sigma");
    auto sigma_err = (std::vector<double> *)file->Get("Sigma_Err");
    auto sigma_syserr = (std::vector<double> *)file_sysuncer->Get("Sigma_TheorySysErr");

    g_upsilon = new TGraphErrors(W->size(), W->data(), sigma->data(), nullptr, sigma_err->data());

    g_upsilon->SetMarkerSize(mMarkerSize);
    g_upsilon->SetMarkerStyle(22);

    std::vector<double> X_AXIS_ERR = std::vector<double>(W->size(), 3.8);

    g_syserr_upsilon = new TGraphErrors(W->size(), W->data(), sigma->data(), X_AXIS_ERR.data(), sigma_syserr->data());

    g_syserr_upsilon->SetMarkerColor(0);
    g_syserr_upsilon->SetFillColorAlpha(1, 0);
    g_syserr_upsilon->SetFillStyle(2);

    c->cd();
    g_syserr_upsilon->Draw("2same");
    g_upsilon->Draw("Pez");
}
