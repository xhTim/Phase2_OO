#ifndef FrameStrategy_H
#define FrameStrategy_H


struct FrameStrategy
{
	std::unique_ptr< TCanvas > 	c = std::make_unique< TCanvas >	();
	std::unique_ptr< TH1 > 		htemp;
	std::vector< TLegend* > 	legends{5,	nullptr};	//default 5 legends, can do more if needed

	virtual void Apply() = 0;
	virtual ~FrameStrategy() = default;

	void cd()						{	c->cd();							};
	void cd(int i)					{	c->cd(i);							};
	// virtual TLegend* GetLegend(int i)	{	return legends[i];							};

	virtual void DrawLegend()
	{
		for (auto* leg : legends)	if(leg)	leg->Draw("same");
	};
	void SaveAs(TString fileName)
	{
		c->SaveAs(fileName);
		for (auto* leg : legends)	delete leg;
		legends.clear();
	}
};

#endif