#include <TFile.h>
#include <TGraphErrors.h>
#include <TCanvas.h>
#include <TH2D.h>
#include <TLegend.h>
#include "../common_ZB/function.h"

TGraphErrors* plot_data_w(TString filename) {
  // Open the ROOT file
  TFile* file = TFile::Open(filename);

  // Get the vectors from the file
  auto w = (std::vector<double>*) file->Get("W");
  auto sigma = (std::vector<double>*) file->Get("Sigma");
  auto sigma_err = (std::vector<double>*) file->Get("Sigma_Err");

  // Create the TGraphErrors object
  auto graph = new TGraphErrors(w->size(), w->data(), sigma->data(), nullptr, sigma_err->data());

  // Set the plot options
  graph->SetTitle("");
  graph->GetXaxis()->SetTitle("W_{#gammaN}^{Pb} (GeV)");
  graph->GetYaxis()->SetTitle("#sigma(#gamma Pb #rightarrow J/#psi Pb) (mb)");
  graph->GetXaxis()->SetTitleSize(0.04);
  graph->GetYaxis()->SetTitleSize(0.04);
  graph->GetYaxis()->SetRangeUser(0.01, 0.1);
  graph->SetMarkerStyle(20);
  graph->SetMarkerSize(1.5);

  // Clean up
  delete w;
  delete sigma;
  delete sigma_err;
  file->Close();

  return graph;
}

TGraphErrors* plot_sys_w(TString filename, TString filename_sys)
{
  TFile *file = TFile::Open(filename);
  TFile *file_err = TFile::Open(filename_sys);

  auto w = (std::vector<double>*) file->Get("W");
  auto sigma = (std::vector<double>*) file->Get("Sigma");
  auto X_AXIS_ERR		= std::vector<double>(w->size(), 3.8);
  auto sigma_sys=(std::vector<double>*) file_err->Get("Sigma_TheorySysErr");

  auto graph = new TGraphErrors(w->size(), w->data(), sigma->data(), X_AXIS_ERR.data(), sigma_sys->data());

  graph ->SetMarkerColor(0);
	graph ->SetFillColorAlpha(16, 0.5);
	graph ->SetFillStyle(1001);

  delete w;
  delete sigma;
  delete sigma_sys;
  file->Close();

  return graph;
}

TGraphErrors* plot_data_r(TString filename) {
  // Open the ROOT file
  TFile* file = TFile::Open(filename);

  // Get the vectors from the file
  auto x = (std::vector<double>*) file->Get("X");
  auto r = (std::vector<double>*) file->Get("R");
  auto r_err = (std::vector<double>*) file->Get("R_Err");

  // Create the TGraphErrors object
  auto graph = new TGraphErrors(x->size(), x->data(), r->data(), nullptr, r_err->data());

  // Set the plot options
  graph->SetTitle("");
  graph->GetXaxis()->SetTitle("Bjorken #it{x}");
  graph->GetYaxis()->SetTitle("R^{Pb}_{g}");
  graph->GetXaxis()->SetTitleSize(0.04);
  graph->GetYaxis()->SetTitleSize(0.04);
  graph->SetMarkerStyle(20);
  graph->SetMarkerSize(1.5);

  // Clean up
  delete x;
  delete r;
  delete r_err;
  file->Close();

  return graph;
}

void plot_cmp_data(){
  TGraphErrors *gPrj_w = plot_data_w("outFiles/Result_CMS.root");
  TGraphErrors *gCMS_w = plot_data_w("/home/tk/Result_CMS.root");
  TGraphErrors *gPrj_sys_w = plot_sys_w("outFiles/Result_CMS.root", "outFiles/Result_CMS_SysUncer.root");
  gPrj_w->SetMarkerColor(2);
  gPrj_w->SetLineColor(2);

  TLegend *leg1 = new TLegend(.6, .2, .8, .4, "");
  leg1->AddEntry(gPrj_w, "Phase-2 Projection");
  leg1->AddEntry(gPrj_sys_w, "Syst. #gamma flux");
  leg1->AddEntry(gCMS_w, "Run-2 Measurement");

  TCanvas *c1 = new TCanvas("c1", "", 0, 0, 800, 600);
  gPrj_w->Draw("AP");
  gPrj_sys_w->Draw("2same");
  gPrj_w->Draw("P");
  gCMS_w->Draw("P");
  c1->SetLogy();
  leg1->Draw();
  drawLatex(0.56, 0.94, "PbPb 7 nb^{-1} (5.5 TeV)",    42, 0.05, 1);
  drawLatex(0.15, 0.84, "Pb + Pb #rightarrow Pb + Pb + J/#psi",  42, 0.05, 1 );
  c1->SaveAs("Sigma_cmp.png");

  TGraphErrors *gPrj_r = plot_data_r("outFiles/Result_CMS.root");
  TGraphErrors *gCMS_r = plot_data_r("/home/tk/Result_CMS.root");
  gPrj_r->SetMarkerColor(2);
  gPrj_r->SetLineColor(2);

  TLegend *leg2 = new TLegend(.2, .6, .4, .8, "");
  leg2->AddEntry(gPrj_r, "Phase-2 Projection");
  leg2->AddEntry(gCMS_w, "Run-2 Measurement");

  TCanvas *c2 = new TCanvas("c2", "", 0, 0, 800, 600);
  c2->SetLogx();
  gPrj_r->GetYaxis()->SetRangeUser(0.4, 0.94);
  gPrj_r->Draw("AP");
  gCMS_r->Draw("P");
  leg2->Draw();
  drawLatex(0.56, 0.94, "PbPb 7 nb^{-1} (5.5 TeV)",    42, 0.05, 1);
  drawLatex(0.15, 0.84, "Pb + Pb #rightarrow Pb + Pb + J/#psi",  42, 0.05, 1 );
  c2->SaveAs("Rg_cmp.png");
}