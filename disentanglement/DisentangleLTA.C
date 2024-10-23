#include "DisentanglementAnalyzer.C"
#include "ResultPlotProcessor.C"

// Use to disentangle the sigma from the DSigmaDy from LTA.
// The disentanglement is done by the DisentanglementAnalyzer class.
void DisentangleLTA()
{
	TString flux_subCase = "_SigNN68p3R6p67a0p56";

	auto Data_LTA_WS = DSigmaDy_LTA::Read("LTA_WS");

	AnalysisDataObserver obs;
	Data_LTA_WS.Subscribe(&obs);
	
	auto map = Data_LTA_WS.GetMap();
	std::map<TString, std::vector<double>> map_cut;
	for(auto& [key, value] : map)
	{
		cout << key << endl;
		int half_size = value.size() / 2;
		std::vector<double> temp(value.begin()+36, value.begin() + half_size-15);
		map_cut[key] = temp;
	}
	Data_LTA_WS.LoadMap(map_cut);
	// Data_LTA_WS.Print();
	// throw;

	DisentanglementAnalyzer AnaRoot_LTA_WS					(Data_LTA_WS,					"../anaSimu/flux/",      flux_subCase);
	AnaRoot_LTA_WS.Handle();

	Data_LTA_WS.Print();

	struct ResultPlotProcessor p;

	p.SetFigureFrame(ResultFrameStrategyList::Sigma_Log);
	p.AddPlot		(Data_LTA_WS,			ResultPlotStrategyList::Sigma_LTA);
	p.AddTheory		(TheoryList::Sigma_R_LTA, 				"Sigma");
	p.Process		();
	p.SaveAs		("./outFigures/Sigma_vs_W_LogY_LTA.pdf");
}