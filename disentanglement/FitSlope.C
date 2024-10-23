#include "AnalysisData.C"
#include "../common/function.C"
#include "ResultFrameStrategy.C"

// Given sigma, fit the slope at high W.
struct SigmaSlopeFitter
{
	static double fPieceWise4Fit(double *x, double *par)
	{
		if((x[0]>42))
		{
			return exp( par[2]*x[0] + par[3]);
		}
		return exp( par[0]*x[0] + par[1]);
	}

	TF1 * FitSigmas(const int data_option = 0, const int func_option = 9)
	{
		AnalysisData Data_CMS("CMS");
		AnalysisData Data_ALICE_2019_Selected("ALICE_2019_Selected");
		AnalysisData Data_ALICE_2021_Selected("ALICE_2021_Selected");
		AnalysisData Data_LHCb_2022_Selected("LHCb_2022_Selected");

		//------------------------------------Load From Root-----------------------------------------------
		//-------------------------------------------------------------------------------------------------
		Data_CMS					.LoadMapFile("outFiles/Result_CMS.root");
		Data_ALICE_2019_Selected	.LoadMapFile("outFiles/Result_ALICE_2019_Selected.root");
		Data_ALICE_2021_Selected	.LoadMapFile("outFiles/Result_ALICE_2021_Selected.root");
		Data_LHCb_2022_Selected		.LoadMapFile("outFiles/Result_LHCb_2022_Selected.root");
		//-------------------------------------------------------------------------------------------------

		AnalysisData Data_CMS_SysUncer("CMS_SysUncer");	Data_CMS_SysUncer.LoadMapFile("outFiles/Result_CMS_SysUncer.root");
		Data_CMS.Combine(Data_CMS_SysUncer);

		auto W_ToFit 			= Data_CMS.Get("W");
		auto Sigma_ToFit 		= Data_CMS.Get("Sigma");
		auto Sigma_Err_ToFit 	= Data_CMS.Get("Sigma_Err");

		auto W_ToFit_ALICE_2019 			= Data_ALICE_2019_Selected.Get("W");
		auto Sigma_ToFit_ALICE_2019 		= Data_ALICE_2019_Selected.Get("Sigma");
		auto Sigma_Err_ToFit_ALICE_2019 	= Data_ALICE_2019_Selected.Get("Sigma_Err");

		auto W_ToFit_ALICE_2021 			= Data_ALICE_2021_Selected.Get("W");
		auto Sigma_ToFit_ALICE_2021 		= Data_ALICE_2021_Selected.Get("Sigma");
		auto Sigma_Err_ToFit_ALICE_2021 	= Data_ALICE_2021_Selected.Get("Sigma_Err");

		auto W_ToFit_LHCb_2022 			= Data_LHCb_2022_Selected.Get("W");
		auto Sigma_ToFit_LHCb_2022 		= Data_LHCb_2022_Selected.Get("Sigma");
		auto Sigma_Err_ToFit_LHCb_2022 	= Data_LHCb_2022_Selected.Get("Sigma_Err");

		// --------------------------For moving points up and down-----------------------------------
		auto total_SysErr = Data_CMS.Get("Sigma_SysErr");
		auto thoery_SysErr = Data_CMS.Get("Sigma_TheorySysErr");
		auto experi_SysErr = Data_CMS.Get("Sigma_ExperiSysErr");
		// AnalysisDataObserver obs;
		// Data_CMS.Subscribe(&obs);
		// Data_CMS.Print();
		// throw;
		// for (int i = 0; i < W_ToFit.size(); i++)
		// {
		// 	cout<<W_ToFit[i]<<endl;
		// 	// if (i<W_ToFit.size()/2)
		// 	// {
		// 	// 	Sigma_ToFit[i] -= total_SysErr[i];
		// 	// }
		// 	// else
		// 	// {
		// 	// 	Sigma_ToFit[i] += total_SysErr[i];
		// 	// }
		// 	Sigma_ToFit[i] += thoery_SysErr[i];
		// }
		// ------------------------------------------------------------------------------------------


		TF1* funcSigmas;

		switch(func_option) {
			case 1:
				funcSigmas = new TF1("SigmasPrediction","[0] * x/([1]+[2]*x)");
				funcSigmas->SetParameters(0.001, 10, 1);
				break;
			case 2:
				funcSigmas = new TF1("SigmasPrediction","[0] * pow(x,[1]) / (pow(x,[1]) + [2])");
				funcSigmas->SetParameters(0.01, 1, 10);
				break;
			case 3: //Asymptotic regression
				funcSigmas = new TF1("SigmasPrediction","[0] - ( [0]-[1] ) * exp(-[2]*x)");
				funcSigmas->SetParameters(0.01, 1, 0.01);
				break;
			case 4:	//Logistic curve
				funcSigmas = new TF1("SigmasPrediction","[0] + ([1]-[0])/(1+exp([2]*(x-[3])))");
				funcSigmas->SetParameters(-0.1, 0.01,-0.01,100);
				break;
			case 5:	//Gompertz Curve
				funcSigmas = new TF1("SigmasPrediction","[0] + ([1]-[0])*(1-exp(-exp([2]*(x-[3]))))");
				funcSigmas->SetParameters(-0.1, 0.01,-0.01,100);
				break;
			case 6:	//Alt. Gompertz Curve
				funcSigmas = new TF1("SigmasPrediction","[0] + ([1]-[0])*exp(-exp([2]*(x-[3])))");
				funcSigmas->SetParameters(-0.1, 0.01,-0.01,100);
				break;
			case 7:	//Log-logistic curve
				funcSigmas = new TF1("SigmasPrediction","[0] + ([1]-[0])/( 1+exp( [2]*(log(x)-log([3])) ) )");
				funcSigmas->SetParameters(0.01, 0.01,-1,10);
				break;
			case 8:	//piecewise
				funcSigmas = new TF1("SigmasPrediction", SigmaSlopeFitter::fPieceWise4Fit, 0, 400,4);
				funcSigmas->SetParameters(0.01, 0.01,0.01,0.01);
				funcSigmas->SetParNames("a1", "b1", "a2", "b2");
				// fJpsiPeak->SetParameter(1,1);
				// fJpsiPeak->SetParameter(2,1);
				// fJpsiPeak->SetParameter(3,1);
				// fJpsiPeak->SetParameter(4,3.1);
				// funcSigmas = new TF1("SigmasPrediction","[0] + ([1]-[0])/( 1+exp( [2]*(log(x)-log([3])) ) )");
				// funcSigmas->SetParameters(0.01, 0.01,-1,10);
				break;
			case 9:	//Linear
				funcSigmas = new TF1("SigmasPrediction", "[0] * x + [1]", 42,400);
				funcSigmas->SetParameters(0.001, 1);
				funcSigmas->SetParNames("a", "b");
				break;
			case 10: //Log
				funcSigmas = new TF1("SigmasPrediction", "exp( [0]*x + [1])", 42,400);
				funcSigmas->SetParameters(0.001, 0.1);
				funcSigmas->SetParNames("a", "b");
				break;
			default: // Err Funcion
				funcSigmas = new TF1("SigmasPrediction","[0]*TMath::Erf((x-[1])/[2])");
				funcSigmas->SetParameters(0.1,10,10);
		}
		cout<<endl;
		funcSigmas->Print();
		cout<<endl;

		switch(data_option) {
			case 1:	// use ALICE LHCb Fwd and CMS Data points 
				W_ToFit.insert(W_ToFit.end(),	W_ToFit_ALICE_2019.begin(),	W_ToFit_ALICE_2019.end());
				W_ToFit.insert(W_ToFit.end(),	W_ToFit_LHCb_2022.begin(),	W_ToFit_LHCb_2022.end());
				Sigma_ToFit.insert(Sigma_ToFit.end(),	Sigma_ToFit_ALICE_2019.begin(),	Sigma_ToFit_ALICE_2019.end());
				Sigma_ToFit.insert(Sigma_ToFit.end(),	Sigma_ToFit_LHCb_2022.begin(),	Sigma_ToFit_LHCb_2022.end());
				Sigma_Err_ToFit.insert(Sigma_Err_ToFit.end(),	Sigma_Err_ToFit_ALICE_2019.begin(),	Sigma_Err_ToFit_ALICE_2019.end());
				Sigma_Err_ToFit.insert(Sigma_Err_ToFit.end(),	Sigma_Err_ToFit_LHCb_2022.begin(),	Sigma_Err_ToFit_LHCb_2022.end());
				break;
			case 2:	// use ALICE Mid and CMS Data points
				W_ToFit.insert(W_ToFit.end(),	W_ToFit_ALICE_2021.begin(),	W_ToFit_ALICE_2021.end());
				Sigma_ToFit.insert(Sigma_ToFit.end(),	Sigma_ToFit_ALICE_2021.begin(),	Sigma_ToFit_ALICE_2021.end());
				Sigma_Err_ToFit.insert(Sigma_Err_ToFit.end(),	Sigma_Err_ToFit_ALICE_2021.begin(),	Sigma_Err_ToFit_ALICE_2021.end());
				break;
			case 3:	// use ALICE Fwd, Mid and CMS Data points
				W_ToFit.insert(W_ToFit.end(),	W_ToFit_ALICE_2019.begin(),	W_ToFit_ALICE_2019.end());
				W_ToFit.insert(W_ToFit.end(),	W_ToFit_ALICE_2021.begin(),	W_ToFit_ALICE_2021.end());
				Sigma_ToFit.insert(Sigma_ToFit.end(),	Sigma_ToFit_ALICE_2019.begin(),	Sigma_ToFit_ALICE_2019.end());
				Sigma_ToFit.insert(Sigma_ToFit.end(),	Sigma_ToFit_ALICE_2021.begin(),	Sigma_ToFit_ALICE_2021.end());
				Sigma_Err_ToFit.insert(Sigma_Err_ToFit.end(),	Sigma_Err_ToFit_ALICE_2019.begin(),	Sigma_Err_ToFit_ALICE_2019.end());
				Sigma_Err_ToFit.insert(Sigma_Err_ToFit.end(),	Sigma_Err_ToFit_ALICE_2021.begin(),	Sigma_Err_ToFit_ALICE_2021.end());
				break;
			default: // use CMS Data points ONLY
				break;
		}

		std::unique_ptr<FrameStrategy>	frame = std::make_unique<Sigma_Log_Strategy>	();
		frame->Apply();

		cout<<"fitSigmas----------------->Proceessing<----------------"<<endl;
		TGraphErrors* gr = new TGraphErrors(W_ToFit.size(), W_ToFit.data(), Sigma_ToFit.data(),0, Sigma_Err_ToFit.data());
		
		funcSigmas->SetLineStyle(2);
		gr->Fit(funcSigmas, "R");
		gr->SetTitle("TGraph1D TF1 Fit; dN_{1}/dy; dN_{2}/dy; d#sigma/dy");
		gr->SetMarkerColor(kBlue);
		// gr->SetMarkerSize(0.8);
		gr->SetLineWidth(2);
		gr->Draw("same pez");
		cout<<endl;

		// TPaveStats *st = (TPaveStats*)gr->FindObject("stats");
		// st->SetX1NDC(0.8); //new x start position
		// st->SetX2NDC(0.9); //new x end position

		frame->SaveAs("outFigures/Sigmas_vs_W_Fit.pdf");
		cout<<"fitSigmas----------------->DONE<----------------"<<endl;

		return funcSigmas;
	}
};


void FitSlope()
{
	SigmaSlopeFitter fitter = SigmaSlopeFitter();
	fitter.FitSigmas();
}