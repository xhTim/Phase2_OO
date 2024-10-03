#include "../common/constants_phi.h"

#ifndef COMMON_FUNCTIONS_H
#define COMMON_FUNCTIONS_H


void update_progress(int ientry, int total_entries, int percentage_increment)
{
	if (ientry % (total_entries / percentage_increment) == 0)
	{
		std::cout << "Processing " << ientry << "th entry... (" << (int)((double)ientry / total_entries * 100) << "%)" << std::endl;
	}
}

std::map<TString, TH1*> get_hists_map(TFile *inFile)
{
	// retrieve the histograms name from the input file and save them into a map
	TIter next(inFile->GetListOfKeys());
	TKey *key;
	TString name;
	std::map<TString, TH1*> hists;

	cout << "The histograms in the input file are: " << endl;
	while ((key = (TKey*)next()))
	{
		name = key->GetName();
		cout << name << endl;
		hists[name] = (TH1*)inFile->Get(name);
	}

	return hists;
}

bool isKaon(int pdgId)
{
	// ! require Kaon
	return abs(pdgId) == KAON_PDGID;
}

bool isPhi(int pdgId)
{
	// ! require phi
	return pdgId == PHI_PDGID;
}

bool isJpsi(int pdgId)
{
	// ! require Jpsi
	return pdgId == JPSI_PDGID;
}

void pdfAction(TCanvas *c, TPDF *ps, Bool_t isClosePDF = kFALSE)
{
	c->cd();
	c->Update();
	if(isClosePDF){
		ps->Close();
	}
	else{
		ps->NewPage();
	}
};

TLatex* drawLatex(double x, double y, TString text, int textFont, double textSize, int colorIndex, double textAngle=0)
{
	TLatex *latex = new TLatex(x,y,text.Data());
	latex->SetNDC();
	latex->SetTextFont(textFont);
	latex->SetTextSize(textSize);
	latex->SetTextColor(colorIndex);
	latex->SetTextAngle(textAngle);
	latex->Draw("same");
	return latex;
}

TLine* drawLine(double xlow,double ylow, double xup, double yup, int lineColor, int lineWidth, int lineStyle)
{
	TLine *l1 = new TLine(xlow,ylow,xup,yup);
	l1->SetLineColor(lineColor);
	l1->SetLineWidth(lineWidth);
	l1->SetLineStyle(lineStyle);
	l1->Draw("same");
	return l1;
}

std::tuple<TPad*, TPad*> drawTPads(double splitFraction = 0.2, double TopMargin1 = 0.08, double BottomMargin1 = 0.0, double TopMargin2 = 0.0, double BottomMargin2 = 0.25)
{
	TPad *pad1 = new TPad("pad1","pad1",0,splitFraction,1,1);
	TPad *pad2 = new TPad("pad2","pad2",0,0,1,splitFraction);
	pad1->SetBottomMargin(BottomMargin1);
	pad1->SetTopMargin(TopMargin1);
	pad1->Draw();

	pad2->SetTopMargin(TopMargin2);
	pad2->SetBottomMargin(BottomMargin2);
	pad2->Draw();

	return std::make_tuple(pad1,pad2);
}

void setHisto(TH1D *h,int markerStyle, double markerSize, int markerColor,int lineColor,int lineWidth=1)
{
	h->SetMarkerStyle(markerStyle);
	h->SetMarkerSize(markerSize);
	h->SetMarkerColor(markerColor);
	h->SetLineColor(lineColor);
	h->SetLineWidth(lineWidth);
};

double shift_phi(double phi)
{
	while (phi < -TMath::Pi())
	{
		phi += 2 * TMath::Pi();
	}
	while (phi > TMath::Pi())
	{
		phi -= 2 * TMath::Pi();
	}
	return phi;
}

void write_seq(TH3D *h3D, int option = 0)
{
	// print string with proper format filled with - character
	std::cout << std::setfill('-') << std::setw(80) << "-" << std::endl;
	std::cout << "Writing 3D histogram: " << h3D->GetName() << " with option: " << option << std::endl;
	std::cout << std::setfill('-') << std::setw(80) << "-" << std::endl;
	h3D->Write();

	if (option == -1)
	{
		cout << " Option -1: Only write 3D histogram" << endl;
	}
	else if (option == 0)
	{
		h3D->Project3D("yx")->Write();
		h3D->Project3D("zx")->Write();
	}
	else if (option == 1)
	{
		h3D->Project3D("yx")->Write();
		h3D->Project3D("zx")->Write();

		h3D->ProjectionX()->Write();
		h3D->ProjectionY()->Write();
		h3D->ProjectionZ()->Write();
	}
	else if (option == 2)
	{
		h3D->ProjectionX()->Write();
		h3D->ProjectionY()->Write();
		h3D->ProjectionZ()->Write();
	}
	else if (option == 3)
	{
		h3D->Project3D("yx")->Write();
		h3D->ProjectionX()->Write();
		h3D->ProjectionY()->Write();
		h3D->ProjectionZ()->Write();
	}
	else if (option == 4)
	{
		h3D->Project3D("zx")->Write();
		h3D->ProjectionX()->Write();
		h3D->ProjectionY()->Write();
		h3D->ProjectionZ()->Write();
	}
	else
	{
		std::cout << "Invalid option: " << option << std::endl;
	}
}

void write_sequence(TH3D *h3D, std::vector<TString> projects = {}, int option = 0)
{
	// print string with proper format filled with - character
	std::cout << std::setfill('-') << std::setw(80) << "-" << std::endl;
	std::cout << "  Writing 3D histogram: " << h3D->GetName() << std::endl;
	h3D->Write();

	for (auto project : projects)
	{
		std::cout << "  Projecting 3D histogram: " << project << std::endl;
		h3D->Project3D(project)->Write();
	}

	if (option == 1)
	{
		cout << "  Option 1: Projecting All 1D histograms" << endl;
		h3D->ProjectionX()->Write();
		h3D->ProjectionY()->Write();
		h3D->ProjectionZ()->Write();
	}
	std::cout << std::setfill('-') << std::setw(80) << "-" << std::endl;
}

void write_sequence(TH2D *h2D,  int option = 0)
{
	// print string with proper format filled with - character
	std::cout << std::setfill('-') << std::setw(80) << "-" << std::endl;
	std::cout << "  Writing 2D histogram: " << h2D->GetName() << std::endl;
	h2D->Write();

	if (option == 1)
	{
		cout << "  Option 1: Projecting All 1D histograms" << endl;
		h2D->ProjectionX()->Write();
		h2D->ProjectionY()->Write();
	}
	std::cout << std::setfill('-') << std::setw(80) << "-" << std::endl;
}

void write_seq(TH2D *h2D)
{
	h2D->Write();
	h2D->ProjectionX()->Write();
	h2D->ProjectionY()->Write();
}

// void save_MetaInfo()
// {
// 	//Write some text to the canvas and save it to the root file
// 	TCanvas *c = new TCanvas("MetaInfo", "MetaInfo", 800, 800);
// 	drawLatex(0.1, 0.92, "CMS Preliminary", 42, 0.04, 1);
// 	//draw 
// }
#endif
