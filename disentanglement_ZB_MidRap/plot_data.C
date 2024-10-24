#include <TFile.h>
#include <TGraphErrors.h>
#include <TCanvas.h>
#include <TH2D.h>
#include "../common_ZB/function.h"
#include "ParamConverter.C"
#include "TGraphAsymmErrors.h"

void plot_data() {
  // Open the ROOT file
  TFile* file = TFile::Open("outFiles/Result_CMS.root");
  TFile *fileErr = TFile::Open("outFiles/Result_CMS_SysUncer.root");
  TFile *file_midrap = TFile::Open("outFiles/Result_MidRap.root");

  // Get the vectors from the file
  auto w = (std::vector<double>*) file->Get("W");
  auto sigma = (std::vector<double> *)file->Get("Sigma");
  auto sigma_err = (std::vector<double>*) file->Get("Sigma_Err");
  auto sigma_sys=(std::vector<double>*) fileErr->Get("Sigma_TheorySysErr");
  auto dy_midrap = (std::vector<double> *)file_midrap->Get("Dy");
  auto sigma_midrap = (std::vector<double> *)file_midrap->Get("Sigma");
  auto sigma_err_midrap = (std::vector<double> *)file_midrap->Get("Sigma_Err");
  auto sigma_syshig_midrap = (std::vector<double> *)file_midrap->Get("Sigma_TheorySysErrHigh");
  auto sigma_syslow_midrap = (std::vector<double> *)file_midrap->Get("Sigma_TheorySysErrLow");
  std::vector<double> X_AXIS_ERR = std::vector<double>(w->size(), 3.8);

  std::vector<double>* w_new = new std::vector<double>();
  std::vector<double> *sigma_new = new std::vector<double>();
  std::vector<double> *sigma_err_new = new std::vector<double>();
  std::vector<double> *sigma_syshig_new = new std::vector<double>();
  std::vector<double> *sigma_syslow_new = new std::vector<double>();
  for (int i = 0; i < w->size(); i++)
  {
    if (fabs(ParamConverter::W2y((*w)[i])) < 0.5) continue;
    w_new->push_back((*w)[i]);
    sigma_new->push_back((*sigma)[i]);
    sigma_err_new->push_back((*sigma_err)[i] * sqrt(50));
    sigma_syshig_new->push_back((*sigma_sys)[i]);
    sigma_syslow_new->push_back((*sigma_sys)[i]);
  }
  for (int i = 0; i < dy_midrap->size(); i++)
  {
    w_new->push_back(ParamConverter::y2W((*dy_midrap)[i]));
    sigma_new->push_back((*sigma_midrap)[i]);
    sigma_err_new->push_back((*sigma_err_midrap)[i] * sqrt(50));
    sigma_syshig_new->push_back((*sigma_syshig_midrap)[i]);
    sigma_syslow_new->push_back((*sigma_syslow_midrap)[i]);
  }

  for (int i = 0; i < w_new->size(); i++)
  {
    std::cout << (*w_new)[i] << "\t" << (*sigma_new)[i] << "\t" << (*sigma_err_new)[i] << "\t" << (*sigma_syshig_new)[i] << std::endl;
  }

  auto x = (std::vector<double>*) file->Get("X");
  auto r = (std::vector<double>*) file->Get("R");
  auto r_err = (std::vector<double>*) file->Get("R_Err");

  // Create the TGraphErrors object with scaled y-axis error
  auto graph = new TGraphErrors(w_new->size(), w_new->data(), sigma_new->data(), nullptr, sigma_err_new->data());

  // Create the TGraphErrors object
//  auto graph = new TGraphErrors(w->size(), w->data(), sigma->data(), nullptr, sigma_err->data());
  auto graph_r = new TGraphErrors(x->size(), x->data(), r->data(), nullptr, r_err->data());

//  auto gae2 = new TGraphErrors(w->size(), w->data(), sigma->data(), X_AXIS_ERR.data(), sigma_sys->data());
  auto gae2 = new TGraphAsymmErrors(w_new->size(), w_new->data(), sigma_new->data(), X_AXIS_ERR.data(), X_AXIS_ERR.data(), sigma_syslow_new->data(), sigma_syshig_new->data());
  gae2->SetMarkerColor(0);
  gae2 ->SetFillColorAlpha(16, 0.5);
	gae2 ->SetFillStyle(1001);

  // Set the plot options
  graph->SetTitle("");
  graph->GetXaxis()->SetTitle("W_{#gammaN}^{Pb} (GeV)");
  graph->GetYaxis()->SetTitle("#sigma(#gamma Pb #rightarrow J/#psi Pb) (mb)");
  graph->GetXaxis()->SetTitleSize(0.04);
  graph->GetYaxis()->SetTitleSize(0.04);
  graph->SetMarkerColor(2);
  graph->SetLineColor(2);
  graph->SetMarkerStyle(20);
  graph->SetMarkerSize(1.5);

  graph_r->SetTitle("");
  graph_r->GetXaxis()->SetTitle("Bjorken #it{x}");
  graph_r->GetYaxis()->SetTitle("R^{Pb}_{g}");
  graph_r->GetXaxis()->SetTitleSize(0.04);
  graph_r->GetYaxis()->SetTitleSize(0.04);
  graph_r->GetXaxis()->SetTimeOffset(0.98);
  graph_r->SetMarkerColor(2);
  graph_r->SetLineColor(2);
  graph_r->SetMarkerStyle(20);
  graph_r->SetMarkerSize(1.5);

  // Draw the graph
  auto canvas = new TCanvas("canvas", "", 0, 0, 800, 600);
  canvas->SetLogy();
  graph->Draw("AP");
  gae2->Draw("2same");
  graph->Draw("P");
  drawLatex(0.56, 0.94, "PbPb 7 nb^{-1} (5.5 TeV)", 42, 0.05, 1);
  drawLatex(0.15, 0.84, "Pb + Pb #rightarrow Pb + Pb + J/#psi",  42, 0.05, 1 );
  canvas->SaveAs("Sigma_vs_W_Log_Y.png");

  auto canvas_r = new TCanvas("canvas_r", "", 0, 0, 800, 600);
  canvas_r->SetLogx();
  graph_r->Draw("AP");
  drawLatex(0.56, 0.94, "PbPb 7 nb^{-1} (5.5 TeV)",    42, 0.05, 1);
  drawLatex(0.15, 0.84, "Pb + Pb #rightarrow Pb + Pb + J/#psi",  42, 0.05, 1 );
  canvas_r->SaveAs("Rg_vs_x.png");

  // Clean up
  delete w;
  delete sigma;
  delete sigma_err;
  delete x;
  delete r;
  delete r_err;
  file->Close();
}