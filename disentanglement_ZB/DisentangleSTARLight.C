#include "DisentanglementAnalyzer.C"
#include "ResultPlotProcessor.C"

// Use to disentangle the sigma from the DSigmaDy from STARLight.
// The disentanglement is done by the DisentanglementAnalyzer class.
void DisentangleSTARLight()
{
	TString flux_subCase = "_SigNN68p3R6p67a0p56";

	auto Data_STARLight = DSigmaDy_STARLight::Read();

	AnalysisDataObserver obs;
	Data_STARLight.Subscribe(&obs);

	auto map = Data_STARLight.GetMap();
	std::map<TString, std::vector<double>> map_cut;
	for(auto& [key, value] : map)
	{
		cout << key << endl;
		int half_size = value.size() / 2;
		std::vector<double> temp(value.begin()+12, value.begin() + half_size-6);
		map_cut[key] = temp;
	}
	Data_STARLight.LoadMap(map_cut);


	DisentanglementAnalyzer AnaRoot_STARLight					(Data_STARLight,					"../anaSimu/flux/",      flux_subCase);
	AnaRoot_STARLight.Handle();

	Data_STARLight.Print();

	struct ResultPlotProcessor p;

	p.SetFigureFrame(ResultFrameStrategyList::Sigma_Log);
	p.AddPlot		(Data_STARLight,			ResultPlotStrategyList::Sigma_STARLight);
	p.AddTheory		(TheoryList::Sigma_IA, 				"Sigma");
	p.Process		();
	p.SaveAs		("./outFigures/Sigma_vs_W_LogY_TEST.pdf");
}