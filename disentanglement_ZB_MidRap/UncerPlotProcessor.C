/*
For plotting the uncertainty of the results

Jan. 2023
JiaZhao Lin
*/


#include "ResultFrameStrategy.C"
#include "UncerAnalyzer.C"
#include "../common_ZB/PlotStrategy.h"

//shift the x axis by a certain amount when plotting the AnalysisData. Used to compare the results
void ShiftPlot(const AnalysisData& data_, const TString paramX, const TString paramY, const double shift,
			const int style, const int color, TLegend* leg, const TString legendName)
{
	auto x 		= data_.Get(paramX);
	auto y		= data_.Get(paramY);
	auto y_Err	= data_.Get(paramY+"_Err");

	// the shift is used to shift the x-axis. it is different for log scale and linear scale
	if(paramX == "X")
	{
		for (int i = 0; i < x.size(); ++i)
		{	
			if 	(x[i] > 1e-3)	{ x[i] += shift; 		}
			else 				{ x[i] += shift/50; 	}
		}
	}
	else
	{
		for (int i = 0; i < x.size(); ++i){	x[i] += shift; }
	}

	TGraphErrors* ge	= new TGraphErrors(x.size(),	x.data(),	y.data(),	0,	y_Err.data()	);
	ge->SetMarkerStyle(style);
	ge->SetMarkerSize(1.1);
	ge->SetMarkerColor(color);
	ge->SetLineColor(color);
	ge->SetLineWidth(2);
	ge->Draw("pezsame");

	leg->AddEntry(ge,	legendName,	"p");
}


void UncerPlot(const AnalysisData& data_default, const AnalysisData& data_, const TString paramX, const TString paramY,
				const int style, const int color, TLegend* leg, const TString legendName)
{
	auto x = data_default.Get(paramX);
	auto y = data_.Get(paramY);
	if (paramX == "Dy")
	{
		for (int i = 0; i < x.size(); ++i)
		{
			x[i] *= -1;
		}
	}

	TGraph* gr	= new TGraph(x.size(),	x.data(),	y.data());
	gr->SetMarkerStyle(style);
	gr->SetMarkerSize(1.8);
	gr->SetMarkerColor(color);
	gr->SetLineColor(color);
	gr->Draw("psame");

	leg->AddEntry(gr,	legendName,	"p");
}

struct Sigma_Log_Uncer_Strategy : FrameStrategy
{
	void Apply()
	{
		c->SetLogx();
		gPad->SetTopMargin(0.08);
		gPad->SetBottomMargin(0.12);
		gPad->SetRightMargin(0.05);

		legends[0] 	= new TLegend	(0.35, 0.55, 0.50, 0.85);
		legends[0]	->SetFillStyle(0);
		// legends[0]	->SetFillColor(0);
		legends[0]	->SetTextSize(0.035);

		htemp = std::make_unique< TH2D >	("Sigma_Uncer", ";W_{#gammaN}^{Pb} (GeV);#sigma Uncer.(%);", 10,30,520, 10,0,25);
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
	}
};

struct R_Uncer_Strategy : FrameStrategy
{
	void Apply()
	{
		c->SetLogx();
		gPad->SetTopMargin(0.08);
		gPad->SetBottomMargin(0.12);
		//gPad->SetLeftMargin(0.05);
		gPad->SetRightMargin(0.05);

		legends[0] = new TLegend	(0.35, 0.55, 0.50, 0.85);
		legends[0]	->SetFillStyle(0);
		legends[0]	->SetFillColor(0);
		legends[0]	->SetTextSize(0.04);

		//htemp = std::make_unique< TH2D >	("htemp", "", 10,3.0e-5,5.0e-2, 10, 0, 1);
		htemp = std::make_unique< TH2D >	("R_Uncer", ";x;R^{Pb}_{g} Uncer.(%);", 10,4.0e-5,1.1e-2, 10, 0, 15);
		htemp->GetYaxis()->SetTitleSize(0.06);
		htemp->GetYaxis()->SetTitleOffset(0.85);
		htemp->GetYaxis()->SetLabelSize(0.04);
		htemp->GetYaxis()->CenterTitle();
		htemp->GetXaxis()->CenterTitle();
		htemp->GetXaxis()->SetTitleSize(0.07);
		htemp->GetXaxis()->SetTitleOffset(0.69);
		htemp->GetXaxis()->SetLabelSize(0.04);
		htemp->SetTickLength(0.04);
		htemp->Draw();
	}
};

struct DSigmaDy_Sub_Uncer_Strategy : FrameStrategy
{
	TString frameName;
	DSigmaDy_Sub_Uncer_Strategy(TString frameName_):	frameName{frameName_}	{}

	virtual void Apply()	override
	{
		legends[0] = new TLegend	(0.14, 0.55, 0.55, 0.82);
		legends[0]	->	SetFillStyle(0);
		legends[0]	->	SetFillColor(0);
		legends[0]	->	SetTextSize(0.050);

		TString n = "htemp_" + frameName;
		htemp = std::make_unique< TH2D >	(n, n + ";y;d#sigma_{J/#psi}/dy Uncer.(%)", 10, -3, -1, 10, 0, 20);

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
	}
};

struct UncerPlotProcessor
{
	std::unique_ptr<FrameStrategy>		frame;
	struct UncerAnalyzer				uncerAnalyzer;

	UncerPlotProcessor() {uncerAnalyzer.Handle();}
	void SaveAs(TString fileName){	frame->SaveAs(fileName);	};

	//Take all the AnalysisData from UncerAnalyzer and plot them using ShiftPlot function using the Sigma_Log_Strategy
	void CompareSigma()
	{
		frame = std::make_unique<Sigma_Log_Strategy>();
		frame->Apply();
		delete frame->legends[0];
		frame->legends[0] = new TLegend(0.60, 0.15, 0.90, 0.45);
		frame->legends[0]->SetTextSize(0.020);

		ShiftPlot(uncerAnalyzer.CB_Poly3_AnaData,				"W",	"Sigma",	0,	47,	1,	frame->legends[0],	"Default"			);
		ShiftPlot(uncerAnalyzer.CB_Poly4_AnaData,				"W",	"Sigma",	0,	24,	1,	frame->legends[0],	"CB_Poly4"			);
		ShiftPlot(uncerAnalyzer.CB_FixCBAN_Poly3_AnaData,		"W",	"Sigma",	0,	24,	2,	frame->legends[0],	"CB_FixCBAN_Poly3"	);
		ShiftPlot(uncerAnalyzer.CBG_Poly3_AnaData,				"W",	"Sigma",	0,	24,	4,	frame->legends[0],	"CBG_Poly3"			);
		ShiftPlot(uncerAnalyzer.CB_Poly3_SdB_AnaData,			"W",	"Sigma",	-5,	24,	6,	frame->legends[0],	"QEDPtShape"		);
		ShiftPlot(uncerAnalyzer.CB_Poly3_NarrMass_AnaData,		"W",	"Sigma",	-5,	24,	8,	frame->legends[0],	"NarrMass"			);
		ShiftPlot(uncerAnalyzer.CB_Poly3_WideMass_AnaData,		"W",	"Sigma",	-5,	24,	9,	frame->legends[0],	"WideMass"			);
		ShiftPlot(uncerAnalyzer.CB_Poly3_temp_AnaData,			"W",	"Sigma",	-5,	24,	28,	frame->legends[0],	"PtFitting"			);
		ShiftPlot(uncerAnalyzer.CB_Poly3_CohPtCut_AnaData,		"W",	"Sigma",	5,	24,	30,	frame->legends[0],	"CohPtCut"			);

		ShiftPlot(uncerAnalyzer.CB_Poly3_looseHF_AnaData,		"W",	"Sigma",	5,	26,	1,	frame->legends[0],	"HFveto"			);
		ShiftPlot(uncerAnalyzer.CB_Poly3_fluxPM_AnaData,		"W",	"Sigma",	5,	26,	2,	frame->legends[0],	"PhotonFluxPM"		);
		ShiftPlot(uncerAnalyzer.CB_Poly3_fluxM_AnaData,			"W",	"Sigma",	5,	26,	4,	frame->legends[0],	"PhotonFluxM" 		);
		ShiftPlot(uncerAnalyzer.CB_Poly3_fluxEMDP_AnaData,		"W",	"Sigma",	5,	26,	6,	frame->legends[0],	"PhotonFluxEMDP" 	);
		ShiftPlot(uncerAnalyzer.CB_Poly3_fluxEMDM_AnaData,		"W",	"Sigma",	5,	26,	8,	frame->legends[0],	"PhotonFluxEMDM" 	);
		ShiftPlot(uncerAnalyzer.CB_Poly3_PU_AnaData,			"W",	"Sigma",	5,	26,	9,	frame->legends[0],	"n-PileUp"			);
		ShiftPlot(uncerAnalyzer.CB_Poly3_TnP_Low_AnaData,		"W",	"Sigma",	5,	26,	28,	frame->legends[0],	"TnP"				);
		frame->DrawLegend();

		SaveAs("outFigures/Compare_Sigma_vs_W_LogY.pdf");
	}

	//Take all the AnalysisData from UncerAnalyzer and plot them using ShiftPlot function using the R_Strategy
	void CompareR()
	{
		frame = std::make_unique<R_Strategy>();
		frame->Apply();
		delete frame->legends[0];
		frame->legends[0] = new TLegend(0.60, 0.15, 0.90, 0.45);
		frame->legends[0]->SetTextSize(0.020);

		ShiftPlot(uncerAnalyzer.CB_Poly3_AnaData,				"X",	"R",	0,		47,	1,	frame->legends[0],	"Default"			);
		ShiftPlot(uncerAnalyzer.CB_Poly4_AnaData,				"X",	"R",	0,		24,	1,	frame->legends[0],	"CB_Poly4"			);
		ShiftPlot(uncerAnalyzer.CB_FixCBAN_Poly3_AnaData,		"X",	"R",	0,		24,	2,	frame->legends[0],	"CB_FixCBAN_Poly3"	);
		ShiftPlot(uncerAnalyzer.CBG_Poly3_AnaData,				"X",	"R",	0,		24,	4,	frame->legends[0],	"CBG_Poly3"			);
		ShiftPlot(uncerAnalyzer.CB_Poly3_SdB_AnaData,			"X",	"R",	-4e-4,	24,	6,	frame->legends[0],	"QEDPtShape"		);
		ShiftPlot(uncerAnalyzer.CB_Poly3_NarrMass_AnaData,		"X",	"R",	-4e-4,	24,	8,	frame->legends[0],	"NarrMass"			);
		ShiftPlot(uncerAnalyzer.CB_Poly3_WideMass_AnaData,		"X",	"R",	-4e-4,	24,	9,	frame->legends[0],	"WideMass"			);
		ShiftPlot(uncerAnalyzer.CB_Poly3_temp_AnaData,			"X",	"R",	-4e-4,	24,	28,	frame->legends[0],	"PtFitting"			);
		ShiftPlot(uncerAnalyzer.CB_Poly3_CohPtCut_AnaData,		"X",	"R",	4e-4,	24,	30,	frame->legends[0],	"CohPtCut"			);

		ShiftPlot(uncerAnalyzer.CB_Poly3_looseHF_AnaData,		"X",	"R",	4e-4,	26,	1,	frame->legends[0],	"HFveto"			);
		ShiftPlot(uncerAnalyzer.CB_Poly3_fluxPM_AnaData,		"X",	"R",	4e-4,	26,	2,	frame->legends[0],	"PhotonFluxPM"		);
		ShiftPlot(uncerAnalyzer.CB_Poly3_fluxM_AnaData,			"X",	"R",	4e-4,	26,	4,	frame->legends[0],	"PhotonFluxM" 		);
		ShiftPlot(uncerAnalyzer.CB_Poly3_fluxEMDP_AnaData,		"X",	"R",	4e-4,	26,	6,	frame->legends[0],	"PhotonFluxEMDP" 	);
		ShiftPlot(uncerAnalyzer.CB_Poly3_fluxEMDM_AnaData,		"X",	"R",	4e-4,	26,	8,	frame->legends[0],	"PhotonFluxEMDM" 	);
		ShiftPlot(uncerAnalyzer.CB_Poly3_PU_AnaData,			"X",	"R",	4e-4,	26,	9,	frame->legends[0],	"n-PileUp"			);
		ShiftPlot(uncerAnalyzer.CB_Poly3_TnP_Low_AnaData,		"X",	"R",	4e-4,	26,	28,	frame->legends[0],	"TnP"				);
		frame->DrawLegend();

		SaveAs("outFigures/Compare_Rg_vs_x.pdf");
	}

	void Uncer_Breakdown()
	{
		//Sigma vs W Uncertainty breakdown
		//-------------------------------------------------------------------------------------------------------------------------------------------------------------
		frame = std::make_unique<Sigma_Log_Uncer_Strategy>();
		frame->Apply();

		UncerPlot(uncerAnalyzer.Default_AnaData,	uncerAnalyzer.AnaData_Breakdown.at("Signal Ext"),	"W",	"Sigma_SysUncer",	24,	1,	frame->legends[0],	"Signal Ext");
		UncerPlot(uncerAnalyzer.Default_AnaData,	uncerAnalyzer.AnaData_Breakdown.at("PhotonFlux"),	"W",	"Sigma_SysUncer",	25,	1,	frame->legends[0],	"PhotonFlux");
		UncerPlot(uncerAnalyzer.Default_AnaData,	uncerAnalyzer.AnaData_Breakdown.at("HFveto"),		"W",	"Sigma_SysUncer",	26,	1,	frame->legends[0],	"HFveto");
		UncerPlot(uncerAnalyzer.Default_AnaData,	uncerAnalyzer.AnaData_Breakdown.at("n-PileUp"),		"W",	"Sigma_SysUncer",	28,	1,	frame->legends[0],	"n-PileUp");
		UncerPlot(uncerAnalyzer.Default_AnaData,	uncerAnalyzer.AnaData_Breakdown.at("TnP"),			"W",	"Sigma_SysUncer",	32,	1,	frame->legends[0],	"TnP");
		UncerPlot(uncerAnalyzer.Default_AnaData,	uncerAnalyzer.AnaData_Breakdown.at("BR"),			"W",	"BR_SysUncer",		27,	1,	frame->legends[0],	"BR");
		UncerPlot(uncerAnalyzer.Default_AnaData,	uncerAnalyzer.AnaData_Breakdown.at("Lumi"),			"W",	"Lumi_SysUncer",	27,	2,	frame->legends[0],	"Lumi");
		UncerPlot(uncerAnalyzer.Default_AnaData,	uncerAnalyzer.AnaData_Breakdown.at("Total"),		"W",	"Sigma_SysUncer",	47,	1,	frame->legends[0],	"Total");

		frame->DrawLegend();
		SaveAs("outFigures/Uncer_Sigma_Breakdown.pdf");
		//-------------------------------------------------------------------------------------------------------------------------------------------------------------

		//R vs X Uncertainty breakdown
		//-------------------------------------------------------------------------------------------------------------------------------------------------------------
		frame = std::make_unique<R_Uncer_Strategy>();
		frame->Apply();

		UncerPlot(uncerAnalyzer.Default_AnaData,	uncerAnalyzer.AnaData_Breakdown.at("Signal Ext"),	"X",	"R_SysUncer",	24,	1,	frame->legends[0],	"Signal Ext");
		UncerPlot(uncerAnalyzer.Default_AnaData,	uncerAnalyzer.AnaData_Breakdown.at("PhotonFlux"),	"X",	"R_SysUncer",	25,	1,	frame->legends[0],	"PhotonFlux");
		UncerPlot(uncerAnalyzer.Default_AnaData,	uncerAnalyzer.AnaData_Breakdown.at("HFveto"),		"X",	"R_SysUncer",	26,	1,	frame->legends[0],	"HFveto");
		UncerPlot(uncerAnalyzer.Default_AnaData,	uncerAnalyzer.AnaData_Breakdown.at("n-PileUp"),		"X",	"R_SysUncer",	28,	1,	frame->legends[0],	"n-PileUp");
		UncerPlot(uncerAnalyzer.Default_AnaData,	uncerAnalyzer.AnaData_Breakdown.at("TnP"),			"X",	"R_SysUncer",	32,	1,	frame->legends[0],	"TnP");
		UncerPlot(uncerAnalyzer.Default_AnaData,	uncerAnalyzer.AnaData_Breakdown.at("BR"),			"X",	"R_BR_SysUncer",	27,	1,	frame->legends[0],	"BR");
		UncerPlot(uncerAnalyzer.Default_AnaData,	uncerAnalyzer.AnaData_Breakdown.at("Lumi"),			"X",	"R_Lumi_SysUncer",	27,	2,	frame->legends[0],	"Lumi");
		UncerPlot(uncerAnalyzer.Default_AnaData,	uncerAnalyzer.AnaData_Breakdown.at("IA"),			"X",	"R_SysUncer",	27,	4,	frame->legends[0],	"IA");
		UncerPlot(uncerAnalyzer.Default_AnaData,	uncerAnalyzer.AnaData_Breakdown.at("Total"),		"X",	"R_SysUncer",	47,	1,	frame->legends[0],	"Total");

		frame->DrawLegend();
		SaveAs("outFigures/Uncer_R_Breakdown.pdf");
		//-------------------------------------------------------------------------------------------------------------------------------------------------------------

		//DSigmaDy_AnAn vs y Uncertainty breakdown
		//-------------------------------------------------------------------------------------------------------------------------------------------------------------
		frame = std::make_unique<DSigmaDy_Sub_Uncer_Strategy>("AnAn");
		frame->Apply();

		UncerPlot(uncerAnalyzer.Default_AnaData,	uncerAnalyzer.AnaData_Breakdown.at("Signal Ext"),	"Dy",	"DSigmaDy_AnAn_SysUncer",	24,	1,	frame->legends[0],	"Signal Ext");
		UncerPlot(uncerAnalyzer.Default_AnaData,	uncerAnalyzer.AnaData_Breakdown.at("HFveto"),		"Dy",	"DSigmaDy_AnAn_SysUncer",	26,	1,	frame->legends[0],	"HFveto");
		UncerPlot(uncerAnalyzer.Default_AnaData,	uncerAnalyzer.AnaData_Breakdown.at("TnP"),			"Dy",	"DSigmaDy_AnAn_SysUncer",	32,	1,	frame->legends[0],	"TnP");
		UncerPlot(uncerAnalyzer.Default_AnaData,	uncerAnalyzer.AnaData_Breakdown.at("BR"),			"Dy",	"BR_SysUncer",				27,	1,	frame->legends[0],	"BR");
		UncerPlot(uncerAnalyzer.Default_AnaData,	uncerAnalyzer.AnaData_Breakdown.at("Lumi"),			"Dy",	"Lumi_SysUncer",			27,	2,	frame->legends[0],	"Lumi");
		UncerPlot(uncerAnalyzer.Default_AnaData,	uncerAnalyzer.AnaData_Breakdown.at("Total"),		"Dy",	"DSigmaDy_AnAn_SysUncer",	47,	1,	frame->legends[0],	"Total");
		
		frame->DrawLegend();
		SaveAs("outFigures/Uncer_DSigmaDy_AnAn_Breakdown.pdf");
		//-------------------------------------------------------------------------------------------------------------------------------------------------------------

		//DSigmaDy_0n0n vs y Uncertainty breakdown
		//-------------------------------------------------------------------------------------------------------------------------------------------------------------
		frame = std::make_unique<DSigmaDy_Sub_Uncer_Strategy>("0n0n");
		frame->Apply();

		UncerPlot(uncerAnalyzer.Default_AnaData,	uncerAnalyzer.AnaData_Breakdown.at("Signal Ext"),	"Dy",	"DSigmaDy_0n0n_SysUncer",	24,	1,	frame->legends[0],	"Signal Ext");
		UncerPlot(uncerAnalyzer.Default_AnaData,	uncerAnalyzer.AnaData_Breakdown.at("HFveto"),		"Dy",	"DSigmaDy_0n0n_SysUncer",	26,	1,	frame->legends[0],	"HFveto");
		UncerPlot(uncerAnalyzer.Default_AnaData,	uncerAnalyzer.AnaData_Breakdown.at("n-PileUp"),		"Dy",	"DSigmaDy_0n0n_SysUncer",	28,	1,	frame->legends[0],	"n-PileUp");
		UncerPlot(uncerAnalyzer.Default_AnaData,	uncerAnalyzer.AnaData_Breakdown.at("TnP"),			"Dy",	"DSigmaDy_0n0n_SysUncer",	32,	1,	frame->legends[0],	"TnP");
		UncerPlot(uncerAnalyzer.Default_AnaData,	uncerAnalyzer.AnaData_Breakdown.at("BR"),			"Dy",	"BR_SysUncer",				27,	1,	frame->legends[0],	"BR");
		UncerPlot(uncerAnalyzer.Default_AnaData,	uncerAnalyzer.AnaData_Breakdown.at("Lumi"),			"Dy",	"Lumi_SysUncer",			27,	2,	frame->legends[0],	"Lumi");
		UncerPlot(uncerAnalyzer.Default_AnaData,	uncerAnalyzer.AnaData_Breakdown.at("Total"),		"Dy",	"DSigmaDy_0n0n_SysUncer",	47,	1,	frame->legends[0],	"Total");

		frame->DrawLegend();
		SaveAs("outFigures/Uncer_DSigmaDy_0n0n_Breakdown.pdf");
		//-------------------------------------------------------------------------------------------------------------------------------------------------------------

		//DSigmaDy_0nXnSum vs y Uncertainty breakdown
		//-------------------------------------------------------------------------------------------------------------------------------------------------------------
		frame = std::make_unique<DSigmaDy_Sub_Uncer_Strategy>("0nXnSum");
		frame->Apply();

		UncerPlot(uncerAnalyzer.Default_AnaData,	uncerAnalyzer.AnaData_Breakdown.at("Signal Ext"),	"Dy",	"DSigmaDy_0nXnSum_SysUncer",	24,	1,	frame->legends[0],	"Signal Ext");
		UncerPlot(uncerAnalyzer.Default_AnaData,	uncerAnalyzer.AnaData_Breakdown.at("HFveto"),		"Dy",	"DSigmaDy_0nXnSum_SysUncer",	26,	1,	frame->legends[0],	"HFveto");
		UncerPlot(uncerAnalyzer.Default_AnaData,	uncerAnalyzer.AnaData_Breakdown.at("n-PileUp"),		"Dy",	"DSigmaDy_0nXnSum_SysUncer",	28,	1,	frame->legends[0],	"n-PileUp");
		UncerPlot(uncerAnalyzer.Default_AnaData,	uncerAnalyzer.AnaData_Breakdown.at("TnP"),			"Dy",	"DSigmaDy_0nXnSum_SysUncer",	32,	1,	frame->legends[0],	"TnP");
		UncerPlot(uncerAnalyzer.Default_AnaData,	uncerAnalyzer.AnaData_Breakdown.at("BR"),			"Dy",	"BR_SysUncer",					27,	1,	frame->legends[0],	"BR");
		UncerPlot(uncerAnalyzer.Default_AnaData,	uncerAnalyzer.AnaData_Breakdown.at("Lumi"),			"Dy",	"Lumi_SysUncer",				27,	2,	frame->legends[0],	"Lumi");
		UncerPlot(uncerAnalyzer.Default_AnaData,	uncerAnalyzer.AnaData_Breakdown.at("Total"),		"Dy",	"DSigmaDy_0nXnSum_SysUncer",	47,	1,	frame->legends[0],	"Total");

		frame->DrawLegend();
		SaveAs("outFigures/Uncer_DSigmaDy_0nXnSum_Breakdown.pdf");
		//-------------------------------------------------------------------------------------------------------------------------------------------------------------

		//DSigmaDy_XnXn vs y Uncertainty breakdown
		//-------------------------------------------------------------------------------------------------------------------------------------------------------------
		frame = std::make_unique<DSigmaDy_Sub_Uncer_Strategy>("XnXn");
		frame->Apply();

		UncerPlot(uncerAnalyzer.Default_AnaData,	uncerAnalyzer.AnaData_Breakdown.at("Signal Ext"),	"Dy",	"DSigmaDy_XnXn_SysUncer",		24,	1,	frame->legends[0],	"Signal Ext");
		UncerPlot(uncerAnalyzer.Default_AnaData,	uncerAnalyzer.AnaData_Breakdown.at("HFveto"),		"Dy",	"DSigmaDy_XnXn_SysUncer",		26,	1,	frame->legends[0],	"HFveto");
		UncerPlot(uncerAnalyzer.Default_AnaData,	uncerAnalyzer.AnaData_Breakdown.at("n-PileUp"),		"Dy",	"DSigmaDy_XnXn_SysUncer",		28,	1,	frame->legends[0],	"n-PileUp");
		UncerPlot(uncerAnalyzer.Default_AnaData,	uncerAnalyzer.AnaData_Breakdown.at("TnP"),			"Dy",	"DSigmaDy_XnXn_SysUncer",		32,	1,	frame->legends[0],	"TnP");
		UncerPlot(uncerAnalyzer.Default_AnaData,	uncerAnalyzer.AnaData_Breakdown.at("BR"),			"Dy",	"BR_SysUncer",					27,	1,	frame->legends[0],	"BR");
		UncerPlot(uncerAnalyzer.Default_AnaData,	uncerAnalyzer.AnaData_Breakdown.at("Lumi"),			"Dy",	"Lumi_SysUncer",				27,	2,	frame->legends[0],	"Lumi");
		UncerPlot(uncerAnalyzer.Default_AnaData,	uncerAnalyzer.AnaData_Breakdown.at("Total"),		"Dy",	"DSigmaDy_XnXn_SysUncer",		47,	1,	frame->legends[0],	"Total");

		frame->DrawLegend();
		SaveAs("outFigures/Uncer_DSigmaDy_XnXn_Breakdown.pdf");
		//-------------------------------------------------------------------------------------------------------------------------------------------------------------
	}
};

void UncerPlotProcessor()
{
	struct UncerPlotProcessor processor;
	
	processor.CompareSigma();
	processor.CompareR();
	processor.Uncer_Breakdown();
}