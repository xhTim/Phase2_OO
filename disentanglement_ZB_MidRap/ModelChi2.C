#include "CovMatrix.C"
#include "../common_ZB/DataReader.C"
#include "ResultFrameStrategy.C"
#include "../common_ZB/SortByKeys.C"
#include "./ParamConverter.C"

struct CalculateChi2
{
	//Given data and model, calculate chi2 by using the covariance matrix
	std::vector<double> x_data, y_data;
	TMatrixD cov_matrix;

	CalculateChi2(std::vector<double> x_data_, std::vector<double> y_data_, TMatrixD cov_matrix_) : x_data{x_data_}, y_data{y_data_}, cov_matrix{cov_matrix_}
	{
		//assuming the covariance matrix is sorted. from small to large x. now sort the data
		y_data = SortByKeys(y_data, x_data);
		std::sort(x_data.begin(), x_data.end());
	}

	TF1 CurveFit(const vector<double> &x, const vector<double> &y)
	{
		TF1 f("f", "pol8");
		f.SetParameters(1, 1, 1, 1, 1, 1, 1	, 1, 1);

		TGraphErrors gr(x.size(), x.data(), y.data(), 0, 0);
		gr.Fit(&f, "Q");

		//throw if the fit is not good
		if (f.GetChisquare() / f.GetNDF() > 1) throw std::runtime_error("CurveFit --> Fit is not good!");

		std::unique_ptr<FrameStrategy>	frame = std::make_unique<Sigma_Log_Strategy>	();
		frame->Apply();

		std::vector<double> x_fit,y_fit;
		for (int i = 1; i < 400; i++)
		{
			x_fit.push_back(i);
			y_fit.push_back(f.Eval(i));
		}

		auto gr_fit = TGraphErrors(x_fit.size(), x_fit.data(), y_fit.data(),0, 0);
		
		f.SetLineStyle(2);

		gr.SetMarkerColor(kRed);
		gr.SetLineWidth(2);
		gr.Draw("same pez");

		gr_fit.SetMarkerColor(kBlue);
		gr_fit.SetLineWidth(2);
		gr_fit.Draw("same pez");
		cout<<endl;

		frame->SaveAs("outFigures/Test.pdf");

		return f;
	}

	std::map< TString, TMatrixD > GetModelDiff()
	{
		std::map< TString, TMatrixD >	map_model_diffs = 
		{
			{"LTA_WS",		TMatrixD(x_data.size(), 1)},
			{"LTA_SS",		TMatrixD(x_data.size(), 1)},
			{"CD_BGK",		TMatrixD(x_data.size(), 1)},
			{"CD_GBW",		TMatrixD(x_data.size(), 1)},
			{"CD_IIM",		TMatrixD(x_data.size(), 1)},
			{"bBK_GG",		TMatrixD(x_data.size(), 1)},
			{"bBK_A",		TMatrixD(x_data.size(), 1)},
			{"GG_hs",		TMatrixD(x_data.size(), 1)},
			{"CGC",			TMatrixD(x_data.size(), 1)},
			{"IA",			TMatrixD(x_data.size(), 1)}
		};

		SingleDataReader 	SDR_LTA("./inFiles/LTA_Jpsi_Sigma_R_2.dat",
								{"Dy", "W_Left", "Sigma_LTA_SS_Left", "Sigma_LTA_WS_Left", "W_Right", "Sigma_LTA_SS_Right", "Sigma_LTA_WS_Right"});

		MultiDataReader 	MDR_CD({"./inFiles/CD_BGK_Jpsi_Sigma.dat", "./inFiles/CD_GBW_Jpsi_Sigma.dat", "./inFiles/CD_IIM_Jpsi_Sigma.dat"},
								{"W", "Sigma"});
		MultiDataReader 	MDR_bBK({"./inFiles/bBK_GG_Jpsi_Sigma.txt", "./inFiles/bBK_A_Jpsi_Sigma.txt"},
								{"X", "W", "Sigma"});
		SingleDataReader 	SDR_GG_hs("./inFiles/GG-hs_Jpsi_Sigma.txt", 
								{"X", "Sigma"});
		SingleDataReader 	SDR_CGC("./inFiles/CGC_Jpsi_Sigma_NoFluct.txt", 
								{"W", "Sigma", "Sigma_InCoh"});

		std::map<TString, std::vector<double>> Map ={
			{"W_LTA",			{}},
			{"Sigma_LTA_WS",	{}},
			{"Sigma_LTA_SS",	{}},

			{"W_CD_BGK",		{}},
			{"W_CD_GBW",		{}},
			{"W_CD_IIM",		{}},
			{"Sigma_CD_BGK",	{}},
			{"Sigma_CD_GBW",	{}},
			{"Sigma_CD_IIM",	{}},

			{"W_bBK_GG",		{}},
			{"W_bBK_A",			{}},
			{"Sigma_bBK_GG",	{}},
			{"Sigma_bBK_A",		{}},

			{"X_GG_hs", 		{}},
			{"W_GG_hs", 		{}},
			{"Sigma_GG_hs",		{}},

			{"W_CGC", 			{}},
			{"Sigma_CGC",		{}},

			{"W_IA",			{}},
			{"Sigma_IA",		{}}
		};
		
		//Formating LTA------------------------------------------------------------
		Map.at("W_LTA")			= SDR_LTA.GetVec("W_Right");
		Map.at("Sigma_LTA_WS")	= SDR_LTA.GetVec("Sigma_LTA_WS_Right");
		Map.at("Sigma_LTA_SS")	= SDR_LTA.GetVec("Sigma_LTA_SS_Right");

		auto Ws_temp 			= SDR_LTA.GetVec("W_Left");				std::reverse(Ws_temp.begin(),Ws_temp.end());
		auto Sigma_LTA_WS_temp 	= SDR_LTA.GetVec("Sigma_LTA_WS_Left");		std::reverse(Sigma_LTA_WS_temp.begin(),Sigma_LTA_WS_temp.end());
		auto Sigma_LTA_SS_temp 	= SDR_LTA.GetVec("Sigma_LTA_SS_Left");		std::reverse(Sigma_LTA_SS_temp.begin(),Sigma_LTA_SS_temp.end());

		Ws_temp.pop_back();
		Sigma_LTA_WS_temp.pop_back();
		Sigma_LTA_SS_temp.pop_back();

		Map.at("W_LTA")			.insert(Map.at("W_LTA").begin(),				Ws_temp.begin(),				Ws_temp.end());
		Map.at("Sigma_LTA_WS")	.insert(Map.at("Sigma_LTA_WS").begin(),	Sigma_LTA_WS_temp.begin(),	Sigma_LTA_WS_temp.end());
		Map.at("Sigma_LTA_SS")	.insert(Map.at("Sigma_LTA_SS").begin(),	Sigma_LTA_SS_temp.begin(),	Sigma_LTA_SS_temp.end());
		//End of LTA---------------------------------------------------------------

		// Some sigma's are in mb, some in nb. Convert all to mb.
		Map.at("W_CD_BGK")		= MDR_CD.GetVec(0,	"W");
		Map.at("W_CD_GBW")		= MDR_CD.GetVec(1,	"W");
		Map.at("W_CD_IIM")		= MDR_CD.GetVec(2,	"W");
		Map.at("Sigma_CD_BGK")	= MDR_CD.GetVec(0,	"Sigma");	for (int i = 0; i < Map.at("Sigma_CD_BGK").size(); i++) Map.at("Sigma_CD_BGK")[i]	*= 0.001;
		Map.at("Sigma_CD_GBW")	= MDR_CD.GetVec(1,	"Sigma"); 	for (int i = 0; i < Map.at("Sigma_CD_GBW").size(); i++) Map.at("Sigma_CD_GBW")[i] 	*= 0.001;
		Map.at("Sigma_CD_IIM")	= MDR_CD.GetVec(2,	"Sigma"); 	for (int i = 0; i < Map.at("Sigma_CD_IIM").size(); i++) Map.at("Sigma_CD_IIM")[i] 	*= 0.001;

		Map.at("W_bBK_GG")		= MDR_bBK.GetVec(0,	"W");
		Map.at("W_bBK_A")		= MDR_bBK.GetVec(1,	"W");
		Map.at("Sigma_bBK_GG")	= MDR_bBK.GetVec(0,	"Sigma");	for (int i = 0; i < Map.at("Sigma_bBK_GG").size(); i++) Map.at("Sigma_bBK_GG")[i]	*= 0.001;
		Map.at("Sigma_bBK_A")	= MDR_bBK.GetVec(1,	"Sigma"); 	for (int i = 0; i < Map.at("Sigma_bBK_A").size(); i++) Map.at("Sigma_bBK_A")[i] 	*= 0.001;

		Map.at("X_GG_hs")		= SDR_GG_hs.GetVec("X");		for (int i = 0; i < Map.at("X_GG_hs").size(); i++) Map.at("W_GG_hs").push_back(ParamConverter::x2W(Map.at("X_GG_hs")[i]));
		Map.at("Sigma_GG_hs")	= SDR_GG_hs.GetVec("Sigma"); 	for (int i = 0; i < Map.at("Sigma_GG_hs").size(); i++) Map.at("Sigma_GG_hs")[i]		*= 0.001;

		Map.at("W_CGC")			= SDR_CGC.GetVec("W");
		Map.at("Sigma_CGC")		= SDR_CGC.GetVec("Sigma");

		for (int i = 5; i < 500; ++i){	Map.at("W_IA").push_back(i);	}
		auto IA_pair = ImpulseApprox::GetImpulseApprox(Map.at("W_IA"));
		Map.at("Sigma_IA") = IA_pair.first;

		//interpolate the theoretical curve to evaluate the model at the same points as the data
		TGraph gr_LTA_WS(Map.at("W_LTA").size(), 	Map.at("W_LTA").data(),		Map.at("Sigma_LTA_WS").data());
		TGraph gr_LTA_SS(Map.at("W_LTA").size(),	Map.at("W_LTA").data(), 	Map.at("Sigma_LTA_SS").data());
		TGraph gr_CD_BGK(Map.at("W_CD_BGK").size(), Map.at("W_CD_BGK").data(), 	Map.at("Sigma_CD_BGK").data());
		TGraph gr_CD_GBW(Map.at("W_CD_GBW").size(), Map.at("W_CD_GBW").data(), 	Map.at("Sigma_CD_GBW").data());
		TGraph gr_CD_IIM(Map.at("W_CD_IIM").size(), Map.at("W_CD_IIM").data(),	Map.at("Sigma_CD_IIM").data());
		TGraph gr_bBK_GG(Map.at("W_bBK_GG").size(), Map.at("W_bBK_GG").data(),	Map.at("Sigma_bBK_GG").data());
		TGraph gr_bBK_A	(Map.at("W_bBK_A").size(), 	Map.at("W_bBK_A").data(),	Map.at("Sigma_bBK_A").data());
		TGraph gr_GG_hs	(Map.at("W_GG_hs").size(),	Map.at("W_GG_hs").data(),	Map.at("Sigma_GG_hs").data());
		TGraph gr_CGC	(Map.at("W_CGC").size(),	Map.at("W_CGC").data(),		Map.at("Sigma_CGC").data());
		TGraph gr_IA	(Map.at("W_IA").size(),		Map.at("W_IA").data(),		Map.at("Sigma_IA").data());

		for (int i = 0; i < x_data.size(); i++)
		{
			// cout<<x_data[i]<<" "<<gr.Eval(x_data[i],	0,	"s")<<endl;
			map_model_diffs.at("LTA_WS")(i, 0)	= y_data[i] - gr_LTA_WS	.Eval(x_data[i]);
			map_model_diffs.at("LTA_SS")(i, 0)	= y_data[i] - gr_LTA_SS	.Eval(x_data[i]);
			map_model_diffs.at("CD_BGK")(i, 0)	= y_data[i] - gr_CD_BGK	.Eval(x_data[i]);
			map_model_diffs.at("CD_GBW")(i, 0)	= y_data[i] - gr_CD_GBW	.Eval(x_data[i]);
			map_model_diffs.at("CD_IIM")(i, 0)	= y_data[i] - gr_CD_IIM	.Eval(x_data[i]);
			map_model_diffs.at("bBK_GG")(i, 0)	= y_data[i] - gr_bBK_GG	.Eval(x_data[i]);
			map_model_diffs.at("bBK_A")	(i, 0) 	= y_data[i] - gr_bBK_A	.Eval(x_data[i]);
			map_model_diffs.at("GG_hs")	(i, 0) 	= y_data[i] - gr_GG_hs	.Eval(x_data[i]);
			map_model_diffs.at("CGC")	(i, 0) 	= y_data[i] - gr_CGC	.Eval(x_data[i]);
			map_model_diffs.at("IA")	(i, 0) 	= y_data[i] - gr_IA		.Eval(x_data[i]);
		}

		return map_model_diffs;
	}

	std::map<TString, double> GetChi2()
	{
		auto model_diffs = GetModelDiff();
		std::map<TString, double> map_results;

		auto cov_matrix_inv = cov_matrix.Invert();

		for (auto& it : model_diffs)
		{
			auto temp_model_diff = it.second;
			auto temp = cov_matrix_inv * temp_model_diff;
			auto chi2 = temp_model_diff.T() * temp;
			map_results[it.first] =	chi2(0, 0);
		}

		//print the chi2 values into a table
		cout << std::setfill('-') << std::setw(15) << std::left << "Model" << std::setw(15) << std::left << "Chi2" << std::setw(15) << "Prob" << endl;
		for (auto& it : map_results)
		{
			cout << std::setfill(' ') << std::setw(15) << std::left << it.first << std::setw(15) << std::left << it.second << std::setw(15) << TMath::Prob(it.second, x_data.size()) << endl;
		}

		return map_results;
	}

};

void ModelChi2()
{
	AnalysisData Data_CMS("CMS");
	Data_CMS	.LoadMapFile("outFiles/Result_CMS.root");
	auto data_W 		= Data_CMS.Get("W");
	auto data_Sigma 	= Data_CMS.Get("Sigma");

	//Get the covariance matrix
	TMatrixD cov_matrix_sys 	= MatrixIO::Read("outFiles/SysCovMatrix.txt");
	TMatrixD cov_matrix_stat 	= MatrixIO::Read("outFiles/StatCovMatrix.txt");
	TMatrixD cov_matrix_flux 	= MatrixIO::Read("outFiles/CovMatrixFlux.txt");
	TMatrixD cov_matrix 		= MatrixIO::Read("outFiles/CovMatrix.txt");
	cov_matrix_sys.Print();
	cov_matrix_stat.Print();
	cov_matrix_flux.Print();
	cov_matrix.Print();
	
	//Calculate chi2
	CalculateChi2 CC(data_W, data_Sigma, cov_matrix);

	auto chi2 = CC.GetChi2();
}