#include <TFile.h>
#include <TGraphErrors.h>
#include <TCanvas.h>
#include <TH2D.h>
#include "../common_ZB/function.C"

void plot_data_ZB() {
  // Open the ROOT file
  TFile* file = TFile::Open("outFiles/Result_CMS_ZB.root");
  TFile *fileErr = TFile::Open("outFiles/Result_CMS_SysUncer_ZB.root");

  // Get the vectors from the file
  auto w = (std::vector<double>*) file->Get("W");
  auto sigma = (std::vector<double>*) file->Get("Sigma");
  auto sigma_err = (std::vector<double>*) file->Get("Sigma_Err");
  auto sigma_sys=(std::vector<double>*) fileErr->Get("Sigma_TheorySysErr");
  std::vector<double> X_AXIS_ERR		= std::vector<double>(w->size(), 3.8);

  auto x = (std::vector<double>*) file->Get("X");
  auto r = (std::vector<double>*) file->Get("R");
  auto r_err = (std::vector<double>*) file->Get("R_Err");

  // Create the TGraphErrors object
  auto graph = new TGraphErrors(w->size(), w->data(), sigma->data(), nullptr, sigma_err->data());
  auto graph_r = new TGraphErrors(x->size(), x->data(), r->data(), nullptr, r_err->data());

  auto gae2 = new TGraphErrors(w->size(), w->data(), sigma->data(), X_AXIS_ERR.data(), sigma_sys->data());
  gae2 ->SetMarkerColor(0);
	gae2 ->SetFillColorAlpha(16, 0.5);
	gae2 ->SetFillStyle(1001);

  // Set the plot options
  graph->SetTitle("");
  graph->GetXaxis()->SetTitle("W_{#gammaN}^{O} (GeV)");
  graph->GetYaxis()->SetTitle("#sigma(#gamma O #rightarrow J/#psi O) (mb)");
  graph->GetXaxis()->SetTitleSize(0.04);
  graph->GetYaxis()->SetTitleSize(0.04);
  //graph->GetYaxis()->SetRangeUser(0.01, 0.12);
  graph->SetMarkerColor(2);
  graph->SetLineColor(2);
  graph->SetMarkerStyle(20);
  graph->SetMarkerSize(1.5);

  graph_r->SetTitle("");
  graph_r->GetXaxis()->SetTitle("Bjorken #it{x}");
  graph_r->GetYaxis()->SetTitle("R^{O}_{g}");
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
  drawLatex(0.56, 0.94, "OO 1.5 pb^{-1} (7 TeV)", 42, 0.05, 1);
  drawLatex(0.15, 0.84, "O + O #rightarrow O + O + J/#psi",  42, 0.05, 1 );
  canvas->SaveAs("Sigma_vs_W_Log_Y.png");

  auto canvas_r = new TCanvas("canvas_r", "", 0, 0, 800, 600);
  canvas_r->SetLogx();
  graph_r->Draw("AP");
  drawLatex(0.56, 0.94, "OO 7 nb^{-1} (7 TeV)",    42, 0.05, 1);
  drawLatex(0.15, 0.84, "O + O #rightarrow O + O + J/#psi",  42, 0.05, 1 );
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
