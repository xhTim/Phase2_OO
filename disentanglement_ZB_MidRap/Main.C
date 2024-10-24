/*
Use this work space to calculate everything you need for the disentanglement analysis.
Draw the figures and save them to the output directory.
*/

#include "UncerAnalyzer.C"
#include "FwdRapAnalyzer.C"
#include "MidRapAnalyzer.C"
#include "ResultPlotProcessor.C"

void DoAnalysis()
{
	TString flux_subCase = "";

//	AnalysisData Data_CMS("CMS");
//	AnalysisData Data_ALICE_2019_Selected("ALICE_2019_Selected");
	AnalysisData Data_ALICE_2021_Selected("ALICE_2021_Selected");
//	AnalysisData Data_LHCb_2022_Selected("LHCb_2022_Selected");

	AnalysisDataObserver obs;
//	Data_CMS.Subscribe(&obs);
//	Data_ALICE_2019_Selected.Subscribe(&obs);
	Data_ALICE_2021_Selected.Subscribe(&obs);
//	Data_LHCb_2022_Selected.Subscribe(&obs);

//	Data_CMS					.LoadHistFile("../signalExt/JpsiXsecValues/JpsiXsec_Default_26RapBins_ZB.root");
//	Data_ALICE_2019_Selected	.LoadTextFile("inFiles/DSigmaDy_ALICE_2019_Selected.txt");
	Data_ALICE_2021_Selected	.LoadTextFile("inFiles/MidRap.txt");
//	Data_LHCb_2022_Selected		.LoadTextFile("inFiles/DSigmaDy_LHCb_2022_Selected.txt");

//	DisentanglementAnalyzer AnaRoot_CMS					(Data_CMS,					"../anaSimu/flux/",      flux_subCase);
//	FwdRapAnalyzer 			AnaRoot_ALICE_2019_Selected	(Data_ALICE_2019_Selected,	"../anaSimu/flux/",      flux_subCase);
	MidRapAnalyzer 			AnaRoot_ALICE_2021_Selected	(Data_ALICE_2021_Selected,	"../anaSimu/flux/",      flux_subCase);
//	FwdRapAnalyzer 			AnaRoot_LHCb_2022_Selected	(Data_LHCb_2022_Selected,	"../anaSimu/flux/",      flux_subCase);

//	AnaRoot_CMS.Handle();
//	AnaRoot_ALICE_2019_Selected.Handle();
	AnaRoot_ALICE_2021_Selected.Handle();
//	AnaRoot_LHCb_2022_Selected.Handle();


	// Write the results to the output files.
//	Data_CMS					.WriteMapFile("outFiles/Result_CMS.root");
//	Data_ALICE_2019_Selected	.WriteMapFile("outFiles/Result_ALICE_2019_Selected.root");
	Data_ALICE_2021_Selected	.WriteMapFile("outFiles/Result_MidRap.root");
//	Data_LHCb_2022_Selected		.WriteMapFile("outFiles/Result_LHCb_2022_Selected.root");
}

void Main()
{
//	AnalysisData Data_CMS("CMS");
//	AnalysisData Data_ALICE_2019_Selected("ALICE_2019_Selected");
//	AnalysisData Data_ALICE_2021_Selected("ALICE_2021_Selected");
//	AnalysisData Data_LHCb_2022_Selected("LHCb_2022_Selected");
//	AnalysisData Data_ALICE_2019("ALICE_2019");
//	AnalysisData Data_ALICE_2021("ALICE_2021");
//	AnalysisData Data_LHCb_2022("LHCb_2022");

//------------------------------------REDO Analysis------------------------------------------------
//-------------------------------------------------------------------------------------------------
	 DoAnalysis();
	 UncerAnalyzer();
//-------------------------------------------------------------------------------------------------


//------------------------------------Load From Root-----------------------------------------------
//-------------------------------------------------------------------------------------------------
//	Data_CMS					.LoadMapFile("outFiles/Result_CMS.root");
//	Data_ALICE_2019_Selected	.LoadMapFile("outFiles/Result_ALICE_2019_Selected.root");
//	Data_ALICE_2021_Selected	.LoadMapFile("outFiles/Result_ALICE_2021_Selected.root");
//	Data_LHCb_2022_Selected		.LoadMapFile("outFiles/Result_LHCb_2022_Selected.root");
//	Data_ALICE_2019				.LoadTextFile("inFiles/DSigmaDy_ALICE_2019.txt");
//	Data_ALICE_2021				.LoadTextFile("inFiles/DSigmaDy_ALICE_2021.txt");
//	Data_LHCb_2022				.LoadTextFile("inFiles/DSigmaDy_LHCb_2022.txt");
//-------------------------------------------------------------------------------------------------

//	AnalysisData Data_CMS_SysUncer("CMS_SysUncer");	Data_CMS_SysUncer.LoadMapFile("outFiles/Result_CMS_SysUncer.root");
//	Data_CMS.Combine(Data_CMS_SysUncer);

//	AnalysisDataObserver obs;
//	Data_CMS.Subscribe(&obs);
//	Data_ALICE_2019_Selected.Subscribe(&obs);
//	Data_ALICE_2021_Selected.Subscribe(&obs);
//	Data_LHCb_2022_Selected.Subscribe(&obs);
	// Data_CMS.Print();
	// Data_ALICE_2019_Selected.Print();
	// Data_ALICE_2021_Selected.Print();
	// Data_LHCb_2022.Print();
	// throw "stop";

//------------------------------------Plotting------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//	struct ResultPlotProcessor p;
//------------------------------------DSigmaDy------------------------------------------------------
//	p.SetFigureFrame(ResultFrameStrategyList::DSigmaDy);
//	p.AddPlot		(Data_CMS,			ResultPlotStrategyList::DSigmaDy_CMS,		"AnAn");
	// p.AddPlot		(Data_CMS,	ResultPlotStrategyList::Empty,	"AnAn");
//	p.AddPlot		(Data_ALICE_2019,	ResultPlotStrategyList::DSigmaDy_ALICE_2019,	"AnAn");
//	p.AddPlot		(Data_ALICE_2021,	ResultPlotStrategyList::DSigmaDy_ALICE_2021,	"AnAn");
//	p.AddPlot		(Data_LHCb_2022,	ResultPlotStrategyList::DSigmaDy_LHCb_2022,		"AnAn");
//	p.AddTheory		(TheoryList::DSigmaDy_LTA, "AnAn");
//	p.AddTheory		(TheoryList::DSigmaDy_STARLight, "AnAn");
//	p.AddTheory		(TheoryList::DSigmaDy_Sigma_R_CD, "AnAn");
//	p.Process		();
//	p.SaveAs		("./outFigures/DSigmaDy_AnAn.pdf");

//	p.SetFigureFrame(ResultFrameStrategyList::DSigmaDy_NeuConfig);
//	p.AddPlot		(Data_CMS,			ResultPlotStrategyList::DSigmaDy_CMS,		"0n0n",		1);
//	p.AddPlot		(Data_CMS,			ResultPlotStrategyList::DSigmaDy_CMS,		"0nXnSum",	2);
//	p.AddPlot		(Data_CMS,			ResultPlotStrategyList::DSigmaDy_CMS,		"XnXn",		3);
//	p.AddPlot		(Data_CMS,			ResultPlotStrategyList::DSigmaDy_CMS,		"AnAn",		4);
//	p.AddPlot		(Data_ALICE_2019,	ResultPlotStrategyList::DSigmaDy_ALICE_2019,	"AnAn",		4);
//	p.AddPlot		(Data_ALICE_2021,	ResultPlotStrategyList::DSigmaDy_ALICE_2021,	"AnAn",		4);
//	p.AddPlot		(Data_LHCb_2022,	ResultPlotStrategyList::DSigmaDy_LHCb_2022,		"AnAn",		4);
//	p.AddTheory		(TheoryList::DSigmaDy_LTA, "0n0n");
//	p.AddTheory		(TheoryList::DSigmaDy_LTA, "0nXnSum");
//	p.AddTheory		(TheoryList::DSigmaDy_LTA, "XnXn");
//	p.AddTheory		(TheoryList::DSigmaDy_LTA, "AnAn");
//	p.AddTheory		(TheoryList::DSigmaDy_Sigma_R_CD, "AnAn");
//	p.Process		();
//	p.SaveAs		("./outFigures/DSigmaDy.pdf");

//------------------------------------DSigmaDy--0n0n, 0nXn, XnXn----------------------------------------------------
//	p.SetFigureFrame(ResultFrameStrategyList::DSigmaDy_00_0X_XX);
//	p.AddPlot		(Data_CMS, 			ResultPlotStrategyList::DSigmaDy_00_0X_XX_CMS,		"0n0n",    0, " ", 1, 20);
//	p.AddPlot		(Data_CMS, 			ResultPlotStrategyList::DSigmaDy_00_0X_XX_CMS,		"0nXnSum", 0, " ", 2, 24);
//	p.AddPlot		(Data_CMS, 			ResultPlotStrategyList::DSigmaDy_00_0X_XX_CMS,		"XnXn",    0, "  Data ", 4, 21);

//	p.AddTheory		(TheoryList::DSigmaDy_LTA, "0n0n");
//	p.AddTheory		(TheoryList::DSigmaDy_LTA, "0nXnSum");
//	p.AddTheory		(TheoryList::DSigmaDy_LTA, "XnXn");
//	p.AddTheory		(TheoryList::DSigmaDy_STARLight, "0n0n");
//	p.AddTheory		(TheoryList::DSigmaDy_STARLight, "0nXnSum");
//	p.AddTheory		(TheoryList::DSigmaDy_STARLight, "XnXn");

//	p.Process		();
//	p.SaveAs		("./outFigures/DSigmaDy_00_0X_XX.pdf");

//------------------------------------Sigma----------------------------------------------------------
//	p.SetFigureFrame(ResultFrameStrategyList::Sigma_Log);
//	p.AddPlot		(Data_ALICE_2019_Selected,	ResultPlotStrategyList::Sigma_ALICE_2019);
//	p.AddPlot		(Data_ALICE_2021_Selected,	ResultPlotStrategyList::Sigma_ALICE_2021);
//	p.AddPlot		(Data_LHCb_2022_Selected,	ResultPlotStrategyList::Sigma_LHCb_2022);
//	p.AddTheory		(TheoryList::Sigma_IA, 				"Sigma");
//	p.Process		();
//	p.SaveAs		("./outFigures/Sigma_vs_W_LogY_0.pdf");

//	p.SetFigureFrame(ResultFrameStrategyList::Sigma_Log);
//	p.AddPlot		(Data_CMS,	ResultPlotStrategyList::Empty,	"AnAn");
//	p.AddPlot		(Data_ALICE_2019_Selected,	ResultPlotStrategyList::Sigma_ALICE_2019);
//	p.AddPlot		(Data_ALICE_2021_Selected,	ResultPlotStrategyList::Sigma_ALICE_2021);
//	p.AddPlot		(Data_LHCb_2022_Selected,	ResultPlotStrategyList::Sigma_LHCb_2022);
//	p.AddTheory		(TheoryList::Sigma_R_LTA, 			"Sigma");
//	p.AddTheory		(TheoryList::DSigmaDy_Sigma_R_CD, 	"Sigma");
//	p.AddTheory		(TheoryList::DSigmaDy_Sigma_R_bBK,	"Sigma");
//	p.AddTheory		(TheoryList::Sigma_R_CGC, 			"Sigma");
//	p.AddTheory		(TheoryList::Sigma_R_GG, 			"Sigma");
//	p.AddTheory		(TheoryList::Sigma_IA, 				"Sigma");
//	p.Process		();
//	p.SaveAs		("./outFigures/Sigma_vs_W_LogY_1.pdf");
	
//	p.SetFigureFrame(ResultFrameStrategyList::Sigma_Log);
//	p.AddPlot		(Data_CMS,			ResultPlotStrategyList::Sigma_CMS);
//	p.AddPlot		(Data_ALICE_2019_Selected,	ResultPlotStrategyList::Sigma_ALICE_2019);
//	p.AddPlot		(Data_ALICE_2021_Selected,	ResultPlotStrategyList::Sigma_ALICE_2021);
//	p.AddPlot		(Data_LHCb_2022_Selected,	ResultPlotStrategyList::Sigma_LHCb_2022);
//	p.AddTheory		(TheoryList::Sigma_R_LTA, 			"Sigma");
//	p.AddTheory		(TheoryList::DSigmaDy_Sigma_R_CD, 	"Sigma");
//	p.AddTheory		(TheoryList::DSigmaDy_Sigma_R_bBK,	"Sigma");
//	p.AddTheory		(TheoryList::Sigma_R_CGC, 			"Sigma");
//	p.AddTheory		(TheoryList::Sigma_R_GG, 			"Sigma");
//	p.AddTheory		(TheoryList::Sigma_IA, 				"Sigma");
//	p.Process		();
//	p.SaveAs		("./outFigures/Sigma_vs_W_LogY_2.pdf");

//	 p.SetFigureFrame(ResultFrameStrategyList::Sigma_LogLog);
//	 p.AddPlot		(Data_CMS,			ResultPlotStrategyList::Sigma_CMS);
//	 p.AddPlot		(Data_ALICE_2019_Selected,	ResultPlotStrategyList::Sigma_ALICE_2019);
//	 p.AddPlot		(Data_ALICE_2021_Selected,	ResultPlotStrategyList::Sigma_ALICE_2021);
//	 p.AddPlot		(Data_LHCb_2022_Selected,	ResultPlotStrategyList::Sigma_LHCb_2022);
//	 p.AddTheory		(TheoryList::Sigma_IA, 				"Sigma");
//	 p.AddTheory		(TheoryList::Sigma_R_CGC, 			"Sigma");
//	 p.AddTheory		(TheoryList::Sigma_R_GG, 			"Sigma");
//	 p.AddTheory		(TheoryList::Sigma_R_LTA, 			"Sigma");
//	 p.AddTheory		(TheoryList::DSigmaDy_Sigma_R_bBK,	"Sigma");
//	 p.AddTheory		(TheoryList::DSigmaDy_Sigma_R_CD, 	"Sigma");
//	 p.Process		();
//	 p.SaveAs		("./outFigures/Sigma_vs_W_LogXY.pdf");
//------------------------------------R--------------------------------------------------------------
//	p.SetFigureFrame(ResultFrameStrategyList::R);
//	p.AddPlot		(Data_CMS,			ResultPlotStrategyList::R_CMS);
//	p.AddPlot		(Data_ALICE_2019_Selected,	ResultPlotStrategyList::R_ALICE_2019);
//	p.AddPlot		(Data_ALICE_2021_Selected,	ResultPlotStrategyList::R_ALICE_2021);
//	p.AddPlot		(Data_LHCb_2022_Selected,	ResultPlotStrategyList::R_LHCb_2022);
	// p.AddTheory		(TheoryList::Sigma_R_CGC, 			"R");	//too short
//	p.AddTheory		(TheoryList::Sigma_R_LTA, 			"R");
//	p.AddTheory		(TheoryList::DSigmaDy_Sigma_R_CD, 	"R");
//	p.AddTheory		(TheoryList::DSigmaDy_Sigma_R_bBK,	"R");
//	p.AddTheory		(TheoryList::Sigma_R_GG, 			"R");
//	p.Process		();
//	p.SaveAs		("./outFigures/Rg_vs_x.pdf");
//-------------------------------------------------------------------------------------------------
}
