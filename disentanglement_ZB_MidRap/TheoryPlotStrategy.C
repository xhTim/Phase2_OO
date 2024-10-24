#include "../common_ZB/PlotStrategy.h"

enum class TheoryPlotStrategyList
{
	DSigmaDy_AnAn_LTA_SS,		Sigma_LTA_SS,			R_LTA_SS,
	DSigmaDy_AnAn_LTA_WS,		Sigma_LTA_WS,			R_LTA_WS,
	DSigmaDy_EPS09_Central,	Sigma_EPS09_Central,	R_EPS09_Central,

	Sigma_GG_hs,		R_GG_hs,

	Sigma_bBK_GG,		R_bBK_GG,
	Sigma_bBK_A,		R_bBK_GG_A,
	
	DSigmaDy_CD_BGK,	Sigma_CD_BGK,			R_CD_BGK,
	DSigmaDy_CD_GBW,	Sigma_CD_GBW,			R_CD_GBW,
	DSigmaDy_CD_IIM,	Sigma_CD_IIM,			R_CD_IIM
};

struct DSigmaDy_AnAn_TheoryPlotStrategy : PlotStrategy
{
	DSigmaDy_AnAn_TheoryPlotStrategy(const AnalysisData& data_) :     PlotStrategy{data_} {}

	void Apply(TLegend*	leg)	override
	{
		gr  	= new TGraph		(data.GetSize("Dy"),	data.Get("Dy").data(),	data.Get("DSigmaDy_AnAn").data());
	}
};

struct DSigmaDy_0n0n_TheoryPlotStrategy : PlotStrategy
{
	DSigmaDy_0n0n_TheoryPlotStrategy(const AnalysisData& data_) :     PlotStrategy{data_} {}

	void Apply(TLegend*	leg)	override
	{
		gr  	= new TGraph		(data.GetSize("Dy"),	data.Get("Dy").data(),	data.Get("DSigmaDy_0n0n").data());
	}
};

struct DSigmaDy_0nXnSum_TheoryPlotStrategy : PlotStrategy
{
	DSigmaDy_0nXnSum_TheoryPlotStrategy(const AnalysisData& data_) :     PlotStrategy{data_} {}

	void Apply(TLegend*	leg)	override
	{
		gr  	= new TGraph		(data.GetSize("Dy"),	data.Get("Dy").data(),	data.Get("DSigmaDy_0nXnSum").data());
	}
};

struct DSigmaDy_XnXn_TheoryPlotStrategy : PlotStrategy
{
	DSigmaDy_XnXn_TheoryPlotStrategy(const AnalysisData& data_) :     PlotStrategy{data_} {}

	void Apply(TLegend*	leg)	override
	{
		gr  	= new TGraph		(data.GetSize("Dy"),	data.Get("Dy").data(),	data.Get("DSigmaDy_XnXn").data());
	}
};


//----------------------------------------------LTA SS START----------------------------------------------
struct DSigmaDy_AnAn_LTA_SS_Strategy : DSigmaDy_AnAn_TheoryPlotStrategy
{
	DSigmaDy_AnAn_LTA_SS_Strategy(const AnalysisData& data_) :     DSigmaDy_AnAn_TheoryPlotStrategy{data_} {}

	void Apply(TLegend*	leg)	override
	{
		DSigmaDy_AnAn_TheoryPlotStrategy::Apply(leg);
		leg->AddEntry(gr,	"LTA SS",		"l" );

		gr->SetLineColor(2);
		gr->SetLineStyle(1);
		gr->SetLineWidth(2);
		gr->Draw("lsame");

	}
};

struct DSigmaDy_0n0n_LTA_SS_Strategy : DSigmaDy_0n0n_TheoryPlotStrategy
{
	DSigmaDy_0n0n_LTA_SS_Strategy(const AnalysisData& data_) :     DSigmaDy_0n0n_TheoryPlotStrategy{data_} {}

	void Apply(TLegend*	leg)	override
	{
		DSigmaDy_0n0n_TheoryPlotStrategy::Apply(leg);
		leg->AddEntry(gr,	"LTA SS",		"l" );

		gr->SetLineColor(2);
		gr->SetLineStyle(1);
		gr->SetLineWidth(2);
		gr->Draw("lsame");

	}
};

struct DSigmaDy_0nXnSum_LTA_SS_Strategy : DSigmaDy_0nXnSum_TheoryPlotStrategy
{
	DSigmaDy_0nXnSum_LTA_SS_Strategy(const AnalysisData& data_) :     DSigmaDy_0nXnSum_TheoryPlotStrategy{data_} {}

	void Apply(TLegend*	leg)	override
	{
		DSigmaDy_0nXnSum_TheoryPlotStrategy::Apply(leg);
		leg->AddEntry(gr,	"LTA SS",		"l" );

		gr->SetLineColor(2);
		gr->SetLineStyle(1);
		gr->SetLineWidth(2);
		gr->Draw("lsame");

	}
};

struct DSigmaDy_XnXn_LTA_SS_Strategy : DSigmaDy_XnXn_TheoryPlotStrategy
{
	DSigmaDy_XnXn_LTA_SS_Strategy(const AnalysisData& data_) :     DSigmaDy_XnXn_TheoryPlotStrategy{data_} {}

	void Apply(TLegend*	leg)	override
	{
		DSigmaDy_XnXn_TheoryPlotStrategy::Apply(leg);
		leg->AddEntry(gr,	"LTA SS",		"l" );

		gr->SetLineColor(2);
		gr->SetLineStyle(1);
		gr->SetLineWidth(2);
		gr->Draw("lsame");

	}
};

struct Sigma_LTA_SS_Strategy : Sigma_TheoryPlotStrategy
{
	Sigma_LTA_SS_Strategy(const AnalysisData& data_) :     Sigma_TheoryPlotStrategy{data_} {}

	void Apply(TLegend*	leg)	override
	{
		Sigma_TheoryPlotStrategy::Apply(leg);
		leg->AddEntry(gr,	"LTA SS",		"l" );

		gr->SetLineColor(2);
		gr->SetLineStyle(1);
		gr->SetLineWidth(2);
		gr->Draw("lsame");

	}
};

struct R_LTA_SS_Strategy : R_TheoryPlotStrategy
{
	R_LTA_SS_Strategy(const AnalysisData& data_) :     R_TheoryPlotStrategy{data_} {}

	void Apply(TLegend*	leg)	override
	{
		R_TheoryPlotStrategy::Apply(leg);
		leg->AddEntry(gr,	"LTA SS",		"l" );

		gr->SetLineColor(2);
		gr->SetLineStyle(1);
		gr->SetLineWidth(2);
		gr->Draw("lsame");

	}
};
//----------------------------------------------LTA SS END----------------------------------------------


//----------------------------------------------LTA WS START----------------------------------------------
struct DSigmaDy_AnAn_LTA_WS_Strategy : DSigmaDy_AnAn_TheoryPlotStrategy
{
	DSigmaDy_AnAn_LTA_WS_Strategy(const AnalysisData& data_) :     DSigmaDy_AnAn_TheoryPlotStrategy{data_} {}

	void Apply(TLegend*	leg)	override
	{
		DSigmaDy_AnAn_TheoryPlotStrategy::Apply(leg);
		leg->AddEntry(gr,	"LTA WS",		"l" );

		gr->SetLineColor(2);
		gr->SetLineStyle(7);
		gr->SetLineWidth(2);
		gr->Draw("lsame");

	}
};

struct DSigmaDy_0n0n_LTA_WS_Strategy : DSigmaDy_0n0n_TheoryPlotStrategy
{
	DSigmaDy_0n0n_LTA_WS_Strategy(const AnalysisData& data_) :     DSigmaDy_0n0n_TheoryPlotStrategy{data_} {}

	void Apply(TLegend*	leg)	override
	{
		DSigmaDy_0n0n_TheoryPlotStrategy::Apply(leg);
		leg->AddEntry(gr,	"LTA WS",		"l" );

		gr->SetLineColor(2);
		gr->SetLineStyle(7);
		gr->SetLineWidth(2);
		gr->Draw("lsame");

	}
};

struct DSigmaDy_0nXnSum_LTA_WS_Strategy : DSigmaDy_0nXnSum_TheoryPlotStrategy
{
	DSigmaDy_0nXnSum_LTA_WS_Strategy(const AnalysisData& data_) :     DSigmaDy_0nXnSum_TheoryPlotStrategy{data_} {}

	void Apply(TLegend*	leg)	override
	{
		DSigmaDy_0nXnSum_TheoryPlotStrategy::Apply(leg);
		leg->AddEntry(gr,	"LTA WS",		"l" );

		gr->SetLineColor(2);
		gr->SetLineStyle(7);
		gr->SetLineWidth(2);
		gr->Draw("lsame");

	}
};

struct DSigmaDy_XnXn_LTA_WS_Strategy : DSigmaDy_XnXn_TheoryPlotStrategy
{
	DSigmaDy_XnXn_LTA_WS_Strategy(const AnalysisData& data_) :     DSigmaDy_XnXn_TheoryPlotStrategy{data_} {}

	void Apply(TLegend*	leg)	override
	{
		DSigmaDy_XnXn_TheoryPlotStrategy::Apply(leg);
		leg->AddEntry(gr,	"LTA WS",		"l" );

		gr->SetLineColor(2);
		gr->SetLineStyle(7);
		gr->SetLineWidth(2);
		gr->Draw("lsame");

	}
};

struct Sigma_LTA_WS_Strategy : Sigma_TheoryPlotStrategy
{
	Sigma_LTA_WS_Strategy(const AnalysisData& data_) :     Sigma_TheoryPlotStrategy{data_} {}

	void Apply(TLegend*	leg)	override
	{
		Sigma_TheoryPlotStrategy::Apply(leg);
		leg->AddEntry(gr,	"LTA WS",		"l" );

		gr->SetLineColor(2);
		gr->SetLineStyle(7);
		gr->SetLineWidth(2);
		gr->Draw("lsame");

	}
};

struct R_LTA_WS_Strategy : R_TheoryPlotStrategy
{
	R_LTA_WS_Strategy(const AnalysisData& data_) :     R_TheoryPlotStrategy{data_} {}

	void Apply(TLegend*	leg)	override
	{
		R_TheoryPlotStrategy::Apply(leg);
		leg->AddEntry(gr,	"LTA WS",		"l" );

		gr->SetLineColor(2);
		gr->SetLineStyle(7);
		gr->SetLineWidth(2);
		gr->Draw("lsame");

	}
};
//----------------------------------------------LTA WS END----------------------------------------------


//----------------------------------------------CD START----------------------------------------------
struct DSigmaDy_CD_BGK_Strategy : DSigmaDy_TheoryPlotStrategy
{
	DSigmaDy_CD_BGK_Strategy(const AnalysisData& data_) :     DSigmaDy_TheoryPlotStrategy{data_} {}

	void Apply(TLegend*	leg)	override
	{
		DSigmaDy_TheoryPlotStrategy::Apply(leg);
		leg->AddEntry(gr,	"CD BGK",		"l" );

		gr->SetLineColor(8);
		gr->SetLineStyle(1);
		gr->SetLineWidth(2);
		gr->Draw("lsame");

	}
};

struct DSigmaDy_CD_GBW_Strategy : DSigmaDy_TheoryPlotStrategy
{
	DSigmaDy_CD_GBW_Strategy(const AnalysisData& data_) :     DSigmaDy_TheoryPlotStrategy{data_} {}

	void Apply(TLegend*	leg)	override
	{
		DSigmaDy_TheoryPlotStrategy::Apply(leg);
		leg->AddEntry(gr,	"CD GBW",		"l" );

		gr->SetLineColor(8);
		gr->SetLineStyle(7);
		gr->SetLineWidth(2);
		gr->Draw("lsame");

	}
};

struct DSigmaDy_CD_IIM_Strategy : DSigmaDy_TheoryPlotStrategy
{
	DSigmaDy_CD_IIM_Strategy(const AnalysisData& data_) :     DSigmaDy_TheoryPlotStrategy{data_} {}

	void Apply(TLegend*	leg)	override
	{
		DSigmaDy_TheoryPlotStrategy::Apply(leg);
		leg->AddEntry(gr,	"CD IIM",		"l" );

		gr->SetLineColor(8);
		gr->SetLineStyle(3);
		gr->SetLineWidth(2);
		gr->Draw("lsame");

	}
};

struct Sigma_CD_BGK_Strategy : Sigma_TheoryPlotStrategy
{
	Sigma_CD_BGK_Strategy(const AnalysisData& data_) :     Sigma_TheoryPlotStrategy{data_} {}

	void Apply(TLegend*	leg)	override
	{
		Sigma_TheoryPlotStrategy::Apply(leg);
		leg->AddEntry(gr,	"CD BGK",		"l" );

		gr->SetLineColor(8);
		gr->SetLineStyle(1);
		gr->SetLineWidth(2);
		gr->Draw("lsame");

	}
};

struct Sigma_CD_GBW_Strategy : Sigma_TheoryPlotStrategy
{
	Sigma_CD_GBW_Strategy(const AnalysisData& data_) :     Sigma_TheoryPlotStrategy{data_} {}

	void Apply(TLegend*	leg)	override
	{
		Sigma_TheoryPlotStrategy::Apply(leg);
		leg->AddEntry(gr,	"CD GBW",		"l" );

		gr->SetLineColor(8);
		gr->SetLineStyle(7);
		gr->SetLineWidth(2);
		gr->Draw("lsame");

	}
};

struct Sigma_CD_IIM_Strategy : Sigma_TheoryPlotStrategy
{
	Sigma_CD_IIM_Strategy(const AnalysisData& data_) :     Sigma_TheoryPlotStrategy{data_} {}

	void Apply(TLegend*	leg)	override
	{
		Sigma_TheoryPlotStrategy::Apply(leg);
		leg->AddEntry(gr,	"CD IIM",		"l" );

		gr->SetLineColor(8);
		gr->SetLineStyle(3);
		gr->SetLineWidth(2);
		gr->Draw("lsame");

	}
};

struct R_CD_BGK_Strategy : R_TheoryPlotStrategy
{
	R_CD_BGK_Strategy(const AnalysisData& data_) :     R_TheoryPlotStrategy{data_} {}

	void Apply(TLegend*	leg)	override
	{
		R_TheoryPlotStrategy::Apply(leg);
		leg->AddEntry(gr,	"CD BGK",		"l" );

		gr->SetLineColor(8);
		gr->SetLineStyle(1);
		gr->SetLineWidth(2);
		gr->Draw("lsame");

	}
};

struct R_CD_GBW_Strategy : R_TheoryPlotStrategy
{
	R_CD_GBW_Strategy(const AnalysisData& data_) :     R_TheoryPlotStrategy{data_} {}

	void Apply(TLegend*	leg)	override
	{
		R_TheoryPlotStrategy::Apply(leg);
		leg->AddEntry(gr,	"CD GBW",		"l" );

		gr->SetLineColor(8);
		gr->SetLineStyle(7);
		gr->SetLineWidth(2);
		gr->Draw("lsame");

	}
};

struct R_CD_IIM_Strategy : R_TheoryPlotStrategy
{
	R_CD_IIM_Strategy(const AnalysisData& data_) :     R_TheoryPlotStrategy{data_} {}

	void Apply(TLegend*	leg)	override
	{
		R_TheoryPlotStrategy::Apply(leg);
		leg->AddEntry(gr,	"CD IIM",		"l" );

		gr->SetLineColor(8);
		gr->SetLineStyle(3);
		gr->SetLineWidth(2);
		gr->Draw("lsame");

	}
};
//----------------------------------------------CD END----------------------------------------------


