#ifndef PlotStrategy_H
#define PlotStrategy_H

#include "../disentanglement/AnalysisData.C"

struct PlotStrategy
{
	const AnalysisData& 	data;
	TGraph*		 			gr	{nullptr};
	TGraphErrors* 			ge	{nullptr};
	TGraphAsymmErrors*		gae	{nullptr};
	TGraphAsymmErrors*		gae2{nullptr};
	int 					index{0};	// when the figure has sub figure, it indicates which subfigure to plot on

	PlotStrategy(const AnalysisData& data_) : data{data_} {};

	int GetIndex() const {return index;}
	virtual void Apply(TLegend*	leg) = 0;
	virtual ~PlotStrategy() = default;
};



#endif