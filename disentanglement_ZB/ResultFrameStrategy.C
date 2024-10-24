#include "../common_ZB/function.C"
#include "../common_ZB/FrameStrategy.h"
// #include "ParamConverter.C"

enum class ResultFrameStrategyList
{
	Sigma_Log,
	Sigma_LogLog,
	R,
	DSigmaDy,
	DSigmaDy_00_0X_XX,
	DSigmaDy_NeuConfig
};


struct Sigma_Log_Strategy : FrameStrategy
{

	void Apply()
	{
		c->SetLogy();
		gPad->SetTopMargin(0.08);
		gPad->SetBottomMargin(0.12);
		gPad->SetRightMargin(0.05);

		legends[0] 	= new TLegend	(0.21, 0.16, 0.45, 0.46);
		legends[0]	->SetFillStyle(0);
		legends[0]	->SetFillColor(0);
		legends[0]	->SetTextSize(0.035);

		legends[1]	= new TLegend(0.58, 0.20, 0.78, 0.48);
		legends[1]	->SetFillStyle(0);
		legends[1]	->SetFillColor(0);
		legends[1]	->SetTextSize(0.030);
		// legends[1]	->AddEntry(ge_IA,                "Impulse Approximation", "l");
		// legends[1]	->AddEntry(ge_CGCnoFluct,        "CGC IPsat",             "l");

		legends[2]	= new TLegend(0.74, 0.20, 0.94, 0.48);
		// legends[2]	->SetNColumns(2);
		legends[2]	->SetFillStyle(0);
		legends[2]	->SetFillColor(0);
		legends[2]	->SetTextSize(0.030);
	
		legends[3]	= new TLegend(0.58, 0.21, 0.91, 0.26);
		legends[3]	->SetNColumns(2);
		legends[3]	->SetFillStyle(0);
		legends[3]	->SetFillColor(0);
		legends[3]	->SetTextSize(0.030);
		
		legends[4]	= new TLegend(0.58, 0.16, 0.94, 0.21);
		legends[4]	->SetNColumns(3);
		legends[4]	->SetFillStyle(0);
		legends[4]	->SetFillColor(0);
		legends[4]	->SetTextSize(0.030);

		htemp = std::make_unique< TH2D >	("SigmaVsW", ";W_{#gammaN}^{Pb} (GeV);#sigma(#gamma Pb #rightarrow J/#psi Pb) (mb);", 10,0,430, 10, 0.002, 0.16);
		//htemp = std::make_unique< TH2D >	("SigmaVsW", ";W_{#gammaN}^{Pb} (GeV);#sigma(#gamma Pb #rightarrow J/#psi Pb) (mb);", 10,0,430, 10, 0.004, 0.19);
		htemp->GetYaxis()->CenterTitle();
		htemp->GetXaxis()->CenterTitle();
		htemp->GetYaxis()->SetTitleSize(0.05);
		htemp->GetYaxis()->SetTitleSize(0.05);
		htemp->GetYaxis()->SetTitleOffset(0.99);
		htemp->GetYaxis()->SetLabelSize(0.04);
		htemp->GetXaxis()->SetTitleSize(0.05);
		htemp->GetXaxis()->SetTitleOffset(1.05);
		htemp->GetXaxis()->SetLabelSize(0.04);
		htemp->SetTickLength(0.04);
		htemp->Draw("");

		// TAxis *A1 = (TAxis*) htemp->GetXaxis()->Clone();
		// TF1 *f1		=	new TF1("W2x","2*x",50,430);
		// TGaxis *A1 	= 	new TGaxis(0,0.16,430,0.16,1,10000,50510,"G");
		// A1->SetTitle("axis with decreasing values");
  		// A1->SetLabelOffset(0.02);
		// A1->SetLabelSize(0.03);

		// set the labels of the top axis bins to the values of your desired variable
		// for (int i = 1; i <= A1->GetNbins(); i++) {
		// 	double value = ParamConverter::W2x( A1->GetBinCenter(i) );
		// 	A1->SetBinLabel(i, "12");
		// }
  		// A1->Draw();

		// htemp->GetListOfFunctions()->Add(A1);


		// drawLatex(0.14,0.85,"#bf{#it{x}}:  3.8#times10^{-3}",42, 0.035, 1);
		// drawLatex(0.29,0.85,"9.6#times10^{-4}",   42, 0.035, 1);
		// drawLatex(0.47,0.85,"2.4#times10^{-4}",   42, 0.035, 1);
		// drawLatex(0.67,0.85,"1.1#times10^{-4}",   42, 0.035, 1);
		// drawLatex(0.86,0.85,"6#times10^{-5}",    42, 0.035, 1);

		//drawLatex(0.15, 0.80, "Pb + Pb #rightarrow Pb + Pb + J/#psi",  42, 0.05, 1 );
		drawLatex(0.56, 0.94, "PbPb 1.52 nb^{-1} (5.02 TeV)",    42, 0.05, 1);
		drawLatex(0.15, 0.94, "#bf{CMS}",                        42, 0.05, 1);

	}
};

struct Sigma_LogLog_Strategy : FrameStrategy
{
	void Apply()
	{
		c->SetLogx();
		c->SetLogy();
		gPad->SetTopMargin(0.08);
		gPad->SetBottomMargin(0.12);
		gPad->SetRightMargin(0.05);

		legends[0] = new TLegend	(0.14, 0.66, 0.37, 0.82);
		legends[0]	->SetFillStyle(0);
		legends[0]	->SetFillColor(0);
		legends[0]	->SetTextSize(0.04);

		htemp = std::make_unique< TH2D >	("SigmaVsW", ";W_{#gammaN}^{Pb} (GeV);#sigma(#gamma Pb #rightarrow J/#psi Pb) (mb);", 10,14,520, 10, 0.004, 0.13);
		htemp->GetYaxis()->CenterTitle();
		htemp->GetXaxis()->CenterTitle();
		htemp->GetYaxis()->SetTitleSize(0.05);
		htemp->GetYaxis()->SetTitleSize(0.05);
		htemp->GetYaxis()->SetTitleOffset(0.99);
		htemp->GetYaxis()->SetLabelSize(0.04);
		htemp->GetXaxis()->SetTitleSize(0.05);
		htemp->GetXaxis()->SetTitleOffset(0.98);
		htemp->GetXaxis()->SetLabelSize(0.04);
		htemp->SetTickLength(0.04);
		htemp->Draw("");

		drawLatex(0.18,0.86,"x~3.8#times10^{-3}",42, 0.025, 1);
		drawLatex(0.29,0.86,"x~9.6#times10^{-4}",42, 0.025, 1);
		drawLatex(0.48,0.86,"x~2.4#times10^{-4}",42, 0.025, 1);
		drawLatex(0.67,0.86,"x~1.1#times10^{-4}",42, 0.025, 1);
		drawLatex(0.87,0.86,"x~6.0#times10^{-5}",42, 0.025, 1);

		drawLatex(0.15, 0.80, "Pb + Pb #rightarrow Pb + Pb + J/#psi",  42,        0.05,      1 );
		drawLatex(0.56,0.94,"PbPb 1.52 nb^{-1} (5.02 TeV)",42, 0.05, 1);
		drawLatex(0.15,0.94,"#bf{CMS}",42, 0.05, 1);

	}
};

struct R_Strategy : FrameStrategy
{
	void Apply()
	{
		c->SetLogx();
		gPad->SetTopMargin(0.08);
		gPad->SetBottomMargin(0.12);
		//gPad->SetLeftMargin(0.05);
		gPad->SetRightMargin(0.05);
	
		//CMS, ALICE, LHCb
		legends[0] = new TLegend	(0.15, 0.50, 0.40, 0.82);
		legends[0]	->SetFillStyle(0);
		legends[0]	->SetFillColor(0);
		legends[0]	->SetTextSize(0.035);

		//For LTA, CD
		legends[1] = new TLegend(0.62, 0.18, 0.82, 0.40);
		//legends[1]	->SetNColumns(2);
		legends[1]	->SetFillStyle(0);
		legends[1]	->SetFillColor(0);
		legends[1]	->SetTextSize(0.03);

		//For CGC GG-hs
		legends[2]	= new TLegend(0.78, 0.27, 0.98, 0.40);
		// legends[2]	->SetNColumns(2);
		legends[2]	->SetFillStyle(0);
		legends[2]	->SetFillColor(0);
		legends[2]	->SetTextSize(0.03);

		//For BK 
		legends[3]	= new TLegend(0.53, 0.21, 0.91, 0.32);
		legends[3]	->SetNColumns(2);
		legends[3]	->SetFillStyle(0);
		legends[3]	->SetFillColor(0);
		legends[3]	->SetTextSize(0.03);

		//For CD
		legends[4]	= new TLegend(0.53, 0.17, 0.94, 0.28);
		legends[4]	->SetNColumns(3);
		legends[4]	->SetFillStyle(0);
		legends[4]	->SetFillColor(0);
		legends[4]	->SetTextSize(0.03);

		//htemp = std::make_unique< TH2D >	("htemp", "", 10,3.0e-5,5.0e-2, 10, 0, 1);
		htemp = std::make_unique< TH2D >	("RvsX", "; Bjorken #it{x};R^{Pb}_{g}(x, #mu^{2} = 2.4 GeV^{2});", 10,1.0e-5,0.07, 10, 0.35, 1.05);
		//htemp = std::make_unique< TH2D >	("RvsX", "; Bjorken #it{x};R^{Pb}_{g}(x, #mu^{2} = 2.4 GeV^{2});", 10,1.0e-5,5e-2, 10, 0.2, 1.05);
		htemp->GetYaxis()->SetTitleSize(0.06);
		htemp->GetYaxis()->SetTitleOffset(0.85);
		htemp->GetYaxis()->SetLabelSize(0.04);
		htemp->GetYaxis()->CenterTitle();
		htemp->GetXaxis()->CenterTitle();
		htemp->GetXaxis()->SetTitleSize(0.055);
		htemp->GetXaxis()->SetTitleOffset(0.95);
		htemp->GetXaxis()->SetLabelSize(0.04);
		htemp->SetTickLength(0.04);
		htemp->Draw();

		drawLatex(0.15, 0.84, "Pb + Pb #rightarrow Pb + Pb + J/#psi",  42, 0.05, 1 );
		drawLatex(0.56, 0.94, "PbPb 1.52 nb^{-1} (5.02 TeV)",    42, 0.05, 1);
		drawLatex(0.15, 0.94, "#bf{CMS}",       42, 0.05, 1);
	}
};

struct DSigmaDy_Strategy : FrameStrategy
{
	void Apply()
	{
		setPad(0.12, 0.08, 0.07, 0.13);
		c->SetCanvasSize(800, 500);
		// c->SetLogy();

		legends[0] = new TLegend	(0.14, 0.55, 0.55, 0.82);
		legends[0]	->	SetFillStyle(0);
		legends[0]	->	SetFillColor(0);
		legends[0]	->	SetTextSize(0.045);

		//For LTA
		legends[1] = new TLegend	(0.50, 0.17, 0.70, 0.37);
		legends[1]	->	SetFillStyle(0);
		legends[1]	->	SetFillColor(0);
		legends[1]	->	SetTextSize(0.040);
	
		//For CD
		legends[2] =  new TLegend	(0.70, 0.17, 0.90, 0.37);
		legends[2]	->	SetFillStyle(0);
		legends[2]	->	SetFillColor(0);
		legends[2]	->	SetTextSize(0.040);

		//For others
		legends[3] =  new TLegend	(0.28, 0.19, 0.50, 0.35);
		legends[3]	->	SetFillStyle(0);
		legends[3]	->	SetFillColor(0);
		legends[3]	->	SetTextSize(0.040);

		//htemp = std::make_unique< TH2D >	("htemp", "", 10, -4.1, 1.0, 10, 0, 14.0);
		// htemp = std::make_unique< TH2D >	("AnAn", "AnAn;#it{y};d#sigma_{J/#psi} / d#it{y} (mb);", 10, -4.6, 0, 10, 1.0, 11.0);
		//default
		htemp = std::make_unique< TH2D >	("AnAn", "AnAn;#it{y};d#sigma_{J/#psi} / d#it{y} (mb);", 10, -4.5, 0, 10, 0, 7);

		htemp ->GetYaxis()->CenterTitle();
		htemp ->GetYaxis()->SetNdivisions(6);
		htemp ->GetXaxis()->SetNdivisions(6);
		htemp ->GetYaxis()->SetTitleSize(0.065);
		htemp ->GetYaxis()->SetTitleOffset(0.8);
		htemp ->GetYaxis()->SetLabelSize(0.05);
		htemp ->GetXaxis()->CenterTitle();
		htemp ->GetXaxis()->SetTitleSize(0.065);
		htemp ->GetXaxis()->SetTitleOffset(0.75);
		htemp ->GetXaxis()->SetLabelSize(0.05);
		//htemp ->SetTickLength(0.08);
		htemp->Draw();

		drawLatex(0.15, 0.85, "Pb + Pb #rightarrow Pb + Pb + J/#psi",	42,	0.053, 1);
		drawLatex(0.7,  0.85,  "#bf{AnAn}",                      42, 0.065, 1);
		drawLatex(0.58, 0.94, "PbPb 1.52 nb^{-1} (5.02 TeV)",   42, 0.05,  1);
		drawLatex(0.13, 0.94, "#bf{CMS}",                       42, 0.05,  1);

	}
};

struct DSigmaDy_00_0X_XX_Strategy : FrameStrategy
{
	void Apply()
	{
		// setPad(0.12, 0.08, 0.07, 0.13);

		c->SetLogy();
		c->SetCanvasSize(800, 500);
		// c->SetPad(0.12, 0.08, 0.07, 0.13);

		legends[0] = new TLegend	(0.55, 0.30, 1.09, 0.35);
		legends[0]	->	SetNColumns(3);
		legends[0]	->	SetFillStyle(0);
		// legends[0]	->	SetBorderSize(0);
		legends[0]	->	SetFillColor(0);
		legends[0]	->	SetTextSize(0.035);

		legends[1] = new TLegend	(0.55, 0.25, 1.15, 0.30);
		legends[1]	->	SetNColumns(3);
		legends[1]	->	SetFillStyle(0);
		legends[1]	->	SetFillColor(0);
		legends[1]	->	SetTextSize(0.035);
	
		legends[2] =  new TLegend	(0.55, 0.20, 1.16, 0.25);
		legends[2]	->	SetNColumns(3);
		legends[2]	->	SetFillStyle(0);
		legends[2]	->	SetFillColor(0);
		legends[2]	->	SetTextSize(0.035);

		legends[3] =  new TLegend	(0.55, 0.15, 1.19, 0.20);
		legends[3]	->	SetNColumns(3);
		legends[3]	->	SetFillStyle(0);
		legends[3]	->	SetFillColor(0);
		legends[3]	->	SetTextSize(0.035);

		//htemp = std::make_unique< TH2D >	("injn", "injn;#it{y};d#sigma_{J/#psi} / d#it{y} (mb);", 10, -2.8, -1.5, 10, 0.0, 3.9);
		htemp = std::make_unique< TH2D >	("injn", "injn;#it{y};d#sigma_{J/#psi} / d#it{y} (mb);", 10, -4.6, -0, 10, 0.02, 7);
		htemp ->SetName("injn");

		htemp ->GetYaxis()->CenterTitle();
		htemp ->GetYaxis()->SetNdivisions(6);
		htemp ->GetXaxis()->SetNdivisions(6);
		htemp ->GetYaxis()->SetTitleSize(0.065);
		htemp ->GetYaxis()->SetTitleOffset(0.8);
		htemp ->GetYaxis()->SetLabelSize(0.05);
		htemp ->GetXaxis()->CenterTitle();
		htemp ->GetXaxis()->SetTitleSize(0.065);
		htemp ->GetXaxis()->SetTitleOffset(0.75);
		htemp ->GetXaxis()->SetLabelSize(0.05);
		//htemp ->SetTickLength(0.08);
		htemp->Draw();

		drawLatex(0.15, 0.85, "Pb + Pb #rightarrow Pb + Pb + J/#psi",	42,	0.053, 1);
		//drawLatex(0.7,  0.8,  "#bf{AnAn}",                      42, 0.075, 1);
		drawLatex(0.58, 0.94, "PbPb 1.52 nb^{-1} (5.02 TeV)",   42, 0.05,  1);
		drawLatex(0.13, 0.94, "#bf{CMS}",                       42, 0.05,  1);
		
		drawLatex(0.55, 0.36, "0n0n    0nXn    XnXn",	42,	0.04, 1);
		// drawLatex(0.80, 0.78, "LTA WS",	42,	0.035, 1);
		// drawLatex(0.80, 0.74, "LTA SS",	42,	0.035, 1);
	}
};


struct DSigmaDy_Sub_Strategy : FrameStrategy
{
	TString frameName;
	DSigmaDy_Sub_Strategy(TString frameName_):	frameName{frameName_}	{};

	virtual void Apply()	override
	{
		legends[0] = new TLegend	(0.14, 0.55, 0.55, 0.82);
		legends[0]	->	SetFillStyle(0);
		legends[0]	->	SetFillColor(0);
		legends[0]	->	SetTextSize(0.050);

		//For LTA
		legends[1] = new TLegend	(0.48, 0.18, 0.70, 0.34);
		legends[1]	->	SetFillStyle(0);
		legends[1]	->	SetFillColor(0);
		legends[1]	->	SetTextSize(0.050);
	
		//For CD
		legends[2] =  new TLegend	(0.71, 0.15, 0.97, 0.35);
		legends[2]	->	SetFillStyle(0);
		legends[2]	->	SetFillColor(0);
		legends[2]	->	SetTextSize(0.050);


		TString n = "htemp_" + frameName;
		if 		(frameName == "0n0n") 		htemp = std::make_unique< TH2D >	(n, n + ";#it{y};d#sigma_{J/#psi} / dy (mb)", 10, -4.1, 0, 10, 0.0, 7.0);
		else if (frameName == "0nXn")		htemp = std::make_unique< TH2D >	(n, n + ";#it{y};d#sigma_{J/#psi} / dy (mb)", 10, -4.1, 0, 10, 0.0, 2.0);
		else if (frameName == "XnXn")		htemp = std::make_unique< TH2D >	(n, n + ";#it{y};d#sigma_{J/#psi} / dy (mb)", 10, -4.1, 0, 10, 0.0, 0.5);
		else								htemp = std::make_unique< TH2D >	(n, n + ";#it{y};d#sigma_{J/#psi} / dy (mb)", 10, -4.1, 0, 10, 0.0, 8.0);

		htemp ->GetYaxis()->CenterTitle();
		htemp ->GetYaxis()->SetNdivisions(6);
		htemp ->GetXaxis()->SetNdivisions(6);
		htemp ->GetYaxis()->SetTitleSize(0.065);
		htemp ->GetYaxis()->SetTitleOffset(0.9);
		htemp ->GetYaxis()->SetLabelSize(0.05);
		htemp ->GetXaxis()->CenterTitle();
		htemp ->GetXaxis()->SetTitleSize(0.065);
		htemp ->GetXaxis()->SetTitleOffset(0.75);
		htemp ->GetXaxis()->SetLabelSize(0.05);
		//htemp ->SetTickLength(0.08);
		htemp->Draw();

		if 		(frameName == "0n0n") 		drawLatex(0.15, 0.85, "Pb + Pb #rightarrow Pb + Pb + J/#psi",		42,		0.053,		1);
		else if	(frameName == "0nXn") 		drawLatex(0.15, 0.85, "Pb + Pb #rightarrow Pb + Pb + J/#psi",	42,		0.053,		1);
		else								drawLatex(0.15, 0.85, "Pb + Pb #rightarrow Pb + Pb + J/#psi",	42,		0.053,		1);
		
		drawLatex(0.8, 0.8, Form("#bf{%s}", frameName.Data()),      42,       0.075,      1);
		drawLatex(0.58,0.94,"PbPb 1.52 nb^{-1} (5.02 TeV)",42, 0.05, 1);
		drawLatex(0.15,0.94,"#bf{CMS}",42, 0.05, 1);
	}
};

struct DSigmaDy_NeuConfig_Strategy : FrameStrategy
{
	std::vector< std::unique_ptr< DSigmaDy_Sub_Strategy > > SubFrames;
	std::vector< TString > frameNames		=	{"0n0n",	"0nXn",		"XnXn",	"AnAn"};
	std::vector< TString > NeuConfigNames	=	{"0n0n",	"0nXnSum",	"XnXn",	"AnAn"};
	DSigmaDy_NeuConfig_Strategy()
	{
		for (int i = 0; i < frameNames.size(); ++i)
		{
			SubFrames.push_back( std::make_unique<DSigmaDy_Sub_Strategy> (	frameNames[i]	) );
		}
	}

	virtual void Apply() override
	{
		c->Divide(2,2);
		setPad(0.12, 0.08, 0.07, 0.13);

		for (int i = 0; i < SubFrames.size(); ++i)
		{
			c->cd(i+1);
			c->GetPad(i+1)->SetRightMargin(0.01);

			SubFrames[i]->Apply();
		}
	}

	virtual void DrawLegend() override
	{
		for (int i = 0; i < SubFrames.size(); ++i)
		{
			c->cd(i+1);
			for (int j = 0; j < SubFrames[i]->legends.size(); ++j)
			{
				if ( SubFrames[i]->legends[j] )	SubFrames[i]->legends[j]->Draw("same");
			}
		}
	}

	TLegend* GetLegend(int i, int j)
	{
		return SubFrames[i]->legends[j];
	}

	int FindFrame(TString frameName)
	{
		auto itr = std::find(NeuConfigNames.begin(), NeuConfigNames.end(), frameName);
		if(itr == NeuConfigNames.end()) std::runtime_error("Frame not found");

		int index = std::distance(NeuConfigNames.begin(), itr) + 1;

		return index;
	}
};