#include "ResultFrameStrategy.C"
#include "ResultPlotStrategy.C"
// #include "TheoryPlotStrategy.C"
#include "TheoryReader.C"

struct ResultPlotProcessor
{
	std::unique_ptr<FrameStrategy> 					frame;
	ResultFrameStrategyList							currentFrameStrategy;
	std::vector< std::unique_ptr<PlotStrategy> > 	plots;
	std::vector< std::unique_ptr<Theory		> > 	theories;


	void SaveAs(TString fileName){	frame->SaveAs(fileName);	};

	void Process()
	{
		
		frame->Apply();

		// Switching between different way for drawing legends. this is a bad design, but I don't know how to do it better.
		switch (currentFrameStrategy)
		{
		case ResultFrameStrategyList::DSigmaDy:
			for(int i = 0; i < theories.size(); i++)
			{
				if		( i<2 )	theories[i]->Draw(frame->legends[1]); //"Draw" means "Draw this theory on canvas", "frame->legends[1]" means "add it into 1st legend "
				else			theories[i]->Draw(frame->legends[2]);
			}
			for(int i = 0; i < plots.size(); i++)
			{
				plots[i]->Apply(frame->legends[0]);
			}
			break;
		case ResultFrameStrategyList::DSigmaDy_00_0X_XX:

			for(int i = 0; i < theories.size(); i++)
			{
				theories[i]->DrawBox(frame->legends);
			}

			for(int i = 0; i < plots.size(); i++)
			{
				plots[i]->Apply(frame->legends[0]);
			}
			
			break;
		case ResultFrameStrategyList::DSigmaDy_NeuConfig:
			for(int i = 0; i < theories.size(); i++)
			{
				auto temp_frame = dynamic_cast<DSigmaDy_NeuConfig_Strategy*>(frame.get());
				int n = temp_frame->FindFrame( theories[i]->Case );
				temp_frame->cd(n);
				//plots and theories must be in the same order
				if		( i<3 )	theories[i]->Draw(temp_frame->GetLegend(n-1,	0));
				else if	( i<4 )	theories[i]->Draw(temp_frame->GetLegend(n-1,	1));
				else			theories[i]->Draw(temp_frame->GetLegend(n-1,	2));
			}
			for(int i = 0; i < plots.size(); i++)
			{
				auto temp_frame = dynamic_cast<DSigmaDy_NeuConfig_Strategy*>(frame.get());
				int n = plots[i]->GetIndex();
				temp_frame->cd(n);
				//plots and theories must be in the same order
				plots[i]->Apply(temp_frame->GetLegend(n-1,	0));
			}
			break;
		case ResultFrameStrategyList::Sigma_Log:
			for(int i = 0; i < theories.size(); i++)
			{
				if		( i<2 )	theories[i]->Draw(frame->legends[1]);
				else			theories[i]->Draw(frame->legends[2]);
			}
			for(auto &plot : plots) 	{	plot->Apply(frame->legends[0]	);	}
//			frame->legends[0]->AddEntry(plots[1]->gae, "Syst. exp.", "f");
//			frame->legends[0]->AddEntry(plots[1]->gae2, "Syst. #gamma flux", "f");
			break;
		case ResultFrameStrategyList::R:
			for(int i = 0; i < theories.size(); i++)
			{
				if		( i<2 )	theories[i]->Draw(frame->legends[1]);
				else			theories[i]->Draw(frame->legends[2]);
			}
			for(auto &plot : plots) 	{	plot->Apply(frame->legends[0]	);	}
			frame->legends[0]->AddEntry(plots[0]->gae, "Syst. exp.", "f");
			frame->legends[0]->AddEntry(plots[0]->gae2, "Syst. #gamma flux", "f");
			break;
		default:
			for(auto &theory : theories) {	theory->Draw(frame->legends[0]	);	}
			for(auto &plot : plots) 	{	plot->Apply(frame->legends[0]	);	}
			break;
		}
		frame->DrawLegend();
	}

	void SetFigureFrame(ResultFrameStrategyList frame_)
	{
		//clear old plots when setting new frame
		plots	.clear();
		theories.clear();
		currentFrameStrategy = frame_;

		switch (frame_)
		{
		case ResultFrameStrategyList::Sigma_Log:
			frame = std::make_unique<	Sigma_Log_Strategy				>();
			break;
		case ResultFrameStrategyList::Sigma_LogLog:
			frame = std::make_unique<	Sigma_LogLog_Strategy			>();
			break;
		case ResultFrameStrategyList::R:
			frame = std::make_unique<	R_Strategy						>();
			break;
		case ResultFrameStrategyList::DSigmaDy:
			frame = std::make_unique<	DSigmaDy_Strategy				>();
			break;
		case ResultFrameStrategyList::DSigmaDy_00_0X_XX:
			frame = std::make_unique<	DSigmaDy_00_0X_XX_Strategy		>();
			break;
		case ResultFrameStrategyList::DSigmaDy_NeuConfig:
			frame = std::make_unique<	DSigmaDy_NeuConfig_Strategy		>();
			break;
		
		default:
			std::runtime_error("ResultFrameStrategy Not Found!");
			break;
		}
	}

	void AddPlot(const AnalysisData& data_,	ResultPlotStrategyList plot_, const TString NeuConfig_ = "AnAn",	const int index_ = 0, 
				const TString legName = "", const int color = 0, const int mstyle = 1 )
	{
		switch (plot_)
		{
		case ResultPlotStrategyList::Empty:
			plots.push_back( std::make_unique<	Empty_PlotStrategy					>(data_) );
			break;
		
		case ResultPlotStrategyList::DSigmaDy_CMS:
			plots.push_back( std::make_unique<	DSigmaDy_CMS_Strategy			>(data_,NeuConfig_,index_) );
			break;
		case ResultPlotStrategyList::DSigmaDy_00_0X_XX_CMS:
			plots.push_back( std::make_unique<	DSigmaDy_00_0X_XX_CMS_Strategy	>(data_,NeuConfig_,index_,legName,color,mstyle) );
			break;
		case ResultPlotStrategyList::DSigmaDy_ALICE_2019:
			plots.push_back( std::make_unique<	DSigmaDy_ALICE_2019_Strategy	>(data_,NeuConfig_,index_) );
			break;
		case ResultPlotStrategyList::DSigmaDy_ALICE_2021:
			plots.push_back( std::make_unique<	DSigmaDy_ALICE_2021_Strategy	>(data_,NeuConfig_,index_) );
			break;
		case ResultPlotStrategyList::DSigmaDy_LHCb_2022:
			plots.push_back( std::make_unique<	DSigmaDy_LHCb_2022_Strategy		>(data_,NeuConfig_,index_) );
			break;
			
		case ResultPlotStrategyList::Sigma_STARLight:
			plots.push_back( std::make_unique<	Sigma_STARLight_PlotStrategy	>(data_) );
			break;
		case ResultPlotStrategyList::Sigma_LTA:
			plots.push_back( std::make_unique<	Sigma_LTA_PlotStrategy			>(data_) );
			break;
		case ResultPlotStrategyList::Sigma_CMS:
			plots.push_back( std::make_unique<	Sigma_CMS_PlotStrategy			>(data_) );
			break;
		case ResultPlotStrategyList::Sigma_ALICE_2019:
			plots.push_back( std::make_unique<	Sigma_ALICE_2019_Strategy		>(data_) );
			break;
		case ResultPlotStrategyList::Sigma_ALICE_2021:
			plots.push_back( std::make_unique<	Sigma_ALICE_2021_Strategy		>(data_) );
			break;
		case ResultPlotStrategyList::Sigma_LHCb_2022:
			plots.push_back( std::make_unique<	Sigma_LHCb_2022_Strategy		>(data_) );
			break;

		case ResultPlotStrategyList::R_CMS:
			plots.push_back( std::make_unique<	R_CMS_Strategy					>(data_) );
			break;
		case ResultPlotStrategyList::R_ALICE_2019:
			plots.push_back( std::make_unique<	R_ALICE_2019_Strategy			>(data_) );
			break;
		case ResultPlotStrategyList::R_ALICE_2021:
			plots.push_back( std::make_unique<	R_ALICE_2021_Strategy			>(data_) );
			break;
		case ResultPlotStrategyList::R_LHCb_2022:
			plots.push_back( std::make_unique<	R_LHCb_2022_Strategy			>(data_) );
			break;

		default:
			throw std::runtime_error("ResultPlotStrategyList Not Found!");
			break;
		}
	}

	void AddTheory(TheoryList theory_, const TString Case)
	{
		switch(theory_)
		{
		case TheoryList::Sigma_IA:
			theories.push_back( std::make_unique<	Sigma_IA				>(Case) );
			break;
		case TheoryList::Sigma_R_CGC:
			theories.push_back( std::make_unique<	Sigma_R_CGC				>(Case) );
			break;
		case TheoryList::DSigmaDy_STARLight:
			theories.push_back( std::make_unique<	DSigmaDy_STARLight		>(Case) );
			break;
		case TheoryList::DSigmaDy_LTA:
			theories.push_back( std::make_unique<	DSigmaDy_LTA			>(Case) );
			break;
		case TheoryList::Sigma_R_LTA:
			theories.push_back( std::make_unique<	Sigma_R_LTA				>(Case) );
			break;
		case TheoryList::Sigma_R_GG:
			theories.push_back( std::make_unique<	Sigma_R_GG				>(Case) );
			break;
		case TheoryList::DSigmaDy_Sigma_R_bBK:
			theories.push_back( std::make_unique<	DSigmaDy_Sigma_R_bBK	>(Case) );
			break;
		case TheoryList::DSigmaDy_Sigma_R_CD:
			theories.push_back( std::make_unique<	DSigmaDy_Sigma_R_CD		>(Case) );
			break;
		}
	}
};

// void ResultPlotProcessor()
// {
// 	AnalysisData Data_CMS("CMS");
// 	AnalysisDataObserver obs;
// 	Data_CMS.Subscribe(&obs);

// 	Data_CMS.LoadHistFile("../signalExt/JpsiXsecValues/JpsiXsec_CB_Poly3_PUShuai_6RapBins_NewCohJpsi.appliedTnP.root");

// 	struct ResultPlotProcessor p(Data_CMS);
// 	p.SetFigureFrame(ResultFrameStrategyList::Sigma_Log);
// 	p.AddPlot(ResultPlotStrategyList::Sigma_CMS);
// 	p.Process();
// }
