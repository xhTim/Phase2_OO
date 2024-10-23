#include "./ImpulseApprox.C"
#include "../common/DataReader.C"
#include "./ParamConverter.C"
#include "./AnalysisData.C"

enum class TheoryList
{
	Sigma_R_CGC,
	Sigma_IA,
	DSigmaDy_LTA,
	Sigma_R_LTA,
	Sigma_R_GG,
	DSigmaDy_Sigma_R_bBK,
	DSigmaDy_Sigma_R_CD,
	DSigmaDy_STARLight
};

struct Theory
{
	TString Case;
	Theory(TString Case_) : Case{Case_} {}
	virtual void Draw(TLegend * leg) = 0;
	virtual void DrawBox(std::vector<TLegend *> legs) {throw std::runtime_error("DrawBox not implemented!");};
	virtual ~Theory() = default;
};

struct Sigma_IA : Theory
{
	TGraph* gr_Sigma_IA	{nullptr};

	Sigma_IA(TString Case_ ) : Theory{Case_} {}
	virtual ~Sigma_IA() override
	{
		delete gr_Sigma_IA;
	}

	virtual void Draw(TLegend * leg) override
	{
		std::vector<double> temp_W;	// For plotting IA
		for (int i = 5; i < 500; ++i){	temp_W.push_back(i);	}
		auto IA_pair = ImpulseApprox::GetImpulseApprox(temp_W);

		gr_Sigma_IA	= new TGraphErrors(temp_W.size(),	temp_W.data(),	IA_pair.first.data());
		gr_Sigma_IA	->SetMarkerColor(2);
		gr_Sigma_IA	->SetLineStyle(5);
		gr_Sigma_IA	->SetLineColor(1);
		gr_Sigma_IA	->SetLineWidth(2);
		gr_Sigma_IA	->Draw("lsame");

		leg->AddEntry(gr_Sigma_IA,	"Impulse approx.",	"l");
	}
};

struct Sigma_R_CGC : Theory
{
	TGraph * gr_SigmaVsW	{nullptr};
	TGraph * gr_RvsX		{nullptr};
	
	Sigma_R_CGC(TString Case_ ) : Theory(Case_) {}
	virtual ~Sigma_R_CGC()	override
	{
		delete gr_SigmaVsW;
		delete gr_RvsX;
	}

	virtual void Draw(TLegend * leg) override
	{
		std::vector<TString> Name = {"CGC IPsat"};
		std::vector<int> colors = {11};
		std::vector<int> styles = {5};

		std::map<TString, std::vector<double>> Map ={
			{"X",			{}},
			{"W",			{}},
			{"Sigma",		{}},
			{"R",			{}},
		};

		SingleDataReader SDR1("./inFiles/CGC_Jpsi_Sigma_NoFluct.txt", 
							{"W", "Sigma", "Sigma_InCoh"});

		Map.at("W")	 = SDR1.GetVec("W");	
		Map.at("Sigma") = SDR1.GetVec("Sigma");
		
		for(int j=0; j<Map.at("W").size(); j++ )
		{
			Map.at("X")			.push_back(ParamConverter::W2x(Map.at("W")[j]));
		}

		auto IA_pair = ImpulseApprox::GetImpulseApprox(Map.at("W"));
		for (int i = 0; i < Map.at("Sigma").size(); ++i)
		{
			Map.at("R")		.push_back(sqrt(Map.at("Sigma")[i]/IA_pair.first[i]));
		}

		auto gr_SigmaVsW 	= new TGraph(Map.at("W").size(),	
									Map.at("W").data(),			Map.at("Sigma").data());
		auto gr_RvsW 		= new TGraph(Map.at("X").size(),	
									Map.at("X").data(),			Map.at("R").data());

		if (Case == "Sigma")
		{
			// gr_SigmaVsW->SetFillColor(colors[0]);
			// gr_SigmaVsW->SetFillStyle(3005);
			
			gr_SigmaVsW->SetLineColor(colors[0]);
			gr_SigmaVsW->SetLineStyle(styles[0]);
			gr_SigmaVsW->SetLineWidth(2);
			gr_SigmaVsW->Draw("lsame");
			leg->AddEntry(gr_SigmaVsW,	Name[0].Data(),		"l" );

		}
		else if (Case == "R")
		{
			// gr_RvsW->SetFillColor(colors[0]);
			// gr_RvsW->SetFillStyle(3005);
			gr_RvsW->SetLineColor(colors[0]);
			gr_RvsW->SetLineStyle(styles[0]);
			gr_RvsW->SetLineWidth(2);
			gr_RvsW->Draw("lsame");
			leg->AddEntry(gr_RvsW,	Name[0].Data(),		"l" );
		}
	}
};

struct DSigmaDy_STARLight :	Theory
{
	TGraph* gr_0n0n		{nullptr};
	TGraph* gr_0nXnSum	{nullptr};
	TGraph* gr_XnXn		{nullptr};
	TGraph* gr_AnAn		{nullptr};

	DSigmaDy_STARLight(TString Case_ ) : Theory{Case_} {}
	virtual ~DSigmaDy_STARLight() override
	{
		delete gr_0n0n;
		delete gr_0nXnSum;
		delete gr_XnXn;
	}

	static AnalysisData Read(const TString infileDir = "inFiles/DSigmaDy_STARLight.root")
	{
		TFile *infile = new TFile(infileDir, "read");
		if(!infile) {throw std::runtime_error("LoadSignal ----> Can Not Find The File!!!");}

		auto hAnAn 		= (TH1D*) infile->Get("hAnAn") ;
		auto h0n0n 		= (TH1D*) infile->Get("h0n0n") ;
		auto h0nXnSum 	= (TH1D*) infile->Get("h0nXnSum") ;
		auto hXnXn 		= (TH1D*) infile->Get("hXnXn") ;

		AnalysisData anaData{"STARLight"};

		std::vector<double> DSigmaDy_0n0n, DSigmaDy_0nXnSum, DSigmaDy_XnXn, DSigmaDy_AnAn, Dy;
		std::vector<double> DSigmaDy_0n0n_Err, DSigmaDy_0nXnSum_Err, DSigmaDy_XnXn_Err, DSigmaDy_AnAn_Err, Dy_Err;
		for (int iy = 1 ; iy < hAnAn->GetNbinsX() ; ++iy)
		{
			Dy				.push_back(	hAnAn	->GetBinCenter(iy)	);
			DSigmaDy_AnAn	.push_back(	hAnAn	->GetBinContent(iy)	);
			DSigmaDy_0n0n	.push_back(	h0n0n	->GetBinContent(iy)	);
			DSigmaDy_0nXnSum.push_back(	h0nXnSum->GetBinContent(iy)	);
			DSigmaDy_XnXn	.push_back(	hXnXn	->GetBinContent(iy)	);

			Dy_Err				.push_back(	hAnAn	->GetBinWidth(iy)/2	);
			DSigmaDy_AnAn_Err	.push_back(	hAnAn	->GetBinError(iy)	);
			DSigmaDy_0n0n_Err	.push_back(	h0n0n	->GetBinError(iy)	);
			DSigmaDy_0nXnSum_Err.push_back(	h0nXnSum->GetBinError(iy)	);
			DSigmaDy_XnXn_Err	.push_back(	hXnXn	->GetBinError(iy)	);
		}

		anaData.Add("Dy",			Dy);
		anaData.Add("DSigmaDy_AnAn",	DSigmaDy_AnAn);
		anaData.Add("DSigmaDy_0n0n",	DSigmaDy_0n0n);
		anaData.Add("DSigmaDy_0nXnSum",	DSigmaDy_0nXnSum);
		anaData.Add("DSigmaDy_XnXn",	DSigmaDy_XnXn);

		anaData.Add("Dy_Err",			Dy_Err);
		anaData.Add("DSigmaDy_AnAn_Err",	DSigmaDy_AnAn_Err);
		anaData.Add("DSigmaDy_0n0n_Err",	DSigmaDy_0n0n_Err);
		anaData.Add("DSigmaDy_0nXnSum_Err",	DSigmaDy_0nXnSum_Err);
		anaData.Add("DSigmaDy_XnXn_Err",	DSigmaDy_XnXn_Err);

		return anaData;
	}

	virtual void Draw(TLegend * leg) override
	{
		std::vector<TString> Name = {"STARLight"};
		std::vector<int> colors = {1};
		std::vector<int> styles = {3};

		auto AnaData = Read("inFiles/DSigmaDy_STARLight.root");
		gr_0n0n		= new TGraph(AnaData.GetSize("Dy"),	AnaData.Get("Dy").data(),	AnaData.Get("DSigmaDy_0n0n").data());
		gr_0nXnSum	= new TGraph(AnaData.GetSize("Dy"),	AnaData.Get("Dy").data(),	AnaData.Get("DSigmaDy_0nXnSum").data());
		gr_XnXn		= new TGraph(AnaData.GetSize("Dy"),	AnaData.Get("Dy").data(),	AnaData.Get("DSigmaDy_XnXn").data());
		gr_AnAn		= new TGraph(AnaData.GetSize("Dy"),	AnaData.Get("Dy").data(),	AnaData.Get("DSigmaDy_AnAn").data());

		if (Case == "AnAn")
		{
			gr_AnAn->SetLineColor(colors[0]);
			gr_AnAn->SetLineStyle(styles[0]);
			gr_AnAn->SetLineWidth(2);
			gr_AnAn->Draw("csame");
			leg->AddEntry(gr_AnAn,	Name[0].Data(),		"l" );
		}
		else if (Case == "0n0n")
		{
			gr_0n0n->SetLineColor(1);
			gr_0n0n->SetLineStyle(styles[0]);
			gr_0n0n->SetLineWidth(2);
			gr_0n0n->Draw("lsame");
			leg->AddEntry(gr_0n0n,	" ",		"l" );
		}
		else if (Case == "0nXnSum")
		{
			gr_0nXnSum->SetLineColor(2);
			gr_0nXnSum->SetLineStyle(styles[0]);
			gr_0nXnSum->SetLineWidth(2);
			gr_0nXnSum->Draw("lsame");
			leg->AddEntry(gr_0nXnSum,	" ",	"l" );
		}
		else if (Case == "XnXn")
		{
			gr_XnXn->SetLineColor(4);
			gr_XnXn->SetLineStyle(styles[0]);
			gr_XnXn->SetLineWidth(2);
			gr_XnXn->Draw("lsame");
			leg->AddEntry(gr_XnXn,	Name[0].Data(),		"l" );
		}
	}

	virtual void DrawBox(std::vector<TLegend *> legs) override
	{
		std::vector<TString> Name = {" STARLight"};
		std::vector<int> colors = {1};
		std::vector<int> styles = {3};

		auto AnaData = Read("inFiles/DSigmaDy_STARLight.root");
		gr_0n0n		= new TGraph(AnaData.GetSize("Dy"),	AnaData.Get("Dy").data(),	AnaData.Get("DSigmaDy_0n0n").data());
		gr_0nXnSum	= new TGraph(AnaData.GetSize("Dy"),	AnaData.Get("Dy").data(),	AnaData.Get("DSigmaDy_0nXnSum").data());
		gr_XnXn		= new TGraph(AnaData.GetSize("Dy"),	AnaData.Get("Dy").data(),	AnaData.Get("DSigmaDy_XnXn").data());
		gr_AnAn		= new TGraph(AnaData.GetSize("Dy"),	AnaData.Get("Dy").data(),	AnaData.Get("DSigmaDy_AnAn").data());

		if (Case == "AnAn")
		{
			gr_AnAn->SetLineColor(colors[0]);
			gr_AnAn->SetLineStyle(styles[0]);
			gr_AnAn->SetLineWidth(2);
			gr_AnAn->Draw("lsame");
			legs[3]->AddEntry(gr_AnAn,	" ",		"l" );
		}
		else if (Case == "0n0n")
		{
			gr_0n0n->SetLineColor(1);
			gr_0n0n->SetLineStyle(styles[0]);
			gr_0n0n->SetLineWidth(2);
			gr_0n0n->Draw("lsame");
			legs[3]->AddEntry(gr_0n0n,	" ",		"l" );
		}
		else if (Case == "0nXnSum")
		{
			gr_0nXnSum->SetLineColor(2);
			gr_0nXnSum->SetLineStyle(styles[0]);
			gr_0nXnSum->SetLineWidth(2);
			gr_0nXnSum->Draw("lsame");
			legs[3]->AddEntry(gr_0nXnSum,	" ",	"l" );
		}
		else if (Case == "XnXn")
		{
			gr_XnXn->SetLineColor(4);
			gr_XnXn->SetLineStyle(styles[0]);
			gr_XnXn->SetLineWidth(2);
			gr_XnXn->Draw("lsame");
			legs[3]->AddEntry(gr_XnXn,	Name[0].Data(),		"l" );
		}
	}
};

struct DSigmaDy_LTA : Theory
{
	TGraph* gr_AnAn		{nullptr};
	TGraph* gr_0n0n		{nullptr};
	TGraph* gr_0nXnSum	{nullptr};
	TGraph* gr_XnXn		{nullptr};

	DSigmaDy_LTA(TString Case_ ) : Theory{Case_} {}
	virtual ~DSigmaDy_LTA() override
	{
		delete gr_AnAn;
		delete gr_0n0n;
		delete gr_0nXnSum;
		delete gr_XnXn;
	}

	static AnalysisData Read(const TString name)
	{
		MultiDataReader MDR({"./inFiles/LTA_Jpsi_strong_shadowing.dat", "./inFiles/LTA_Jpsi_weak_shadowing.dat", "./inFiles/EPS09_central_Jpsi.dat"},
							{"Dy", "AnAn", "0n0n", "0nXnSum", "XnXn"});
		AnalysisData AnaData{name};
		if (name == "LTA_SS")
		{
			AnaData.Add("Dy",				MDR.GetVec(0, "Dy"));
			AnaData.Add("DSigmaDy_AnAn",	MDR.GetVec(0, "AnAn"));
			AnaData.Add("DSigmaDy_0n0n",	MDR.GetVec(0, "0n0n"));
			AnaData.Add("DSigmaDy_0nXnSum",	MDR.GetVec(0, "0nXnSum"));
			AnaData.Add("DSigmaDy_XnXn",	MDR.GetVec(0, "XnXn"));
		}
		else if (name == "LTA_WS")
		{
			AnaData.Add("Dy",				MDR.GetVec(1, "Dy"));
			AnaData.Add("DSigmaDy_AnAn",	MDR.GetVec(1, "AnAn"));
			AnaData.Add("DSigmaDy_0n0n",	MDR.GetVec(1, "0n0n"));
			AnaData.Add("DSigmaDy_0nXnSum",	MDR.GetVec(1, "0nXnSum"));
			AnaData.Add("DSigmaDy_XnXn",	MDR.GetVec(1, "XnXn"));
		}
		else if (name == "EPS09_central")
		{
			AnaData.Add("Dy",				MDR.GetVec(2, "Dy"));
			AnaData.Add("DSigmaDy_AnAn",	MDR.GetVec(2, "AnAn"));
			AnaData.Add("DSigmaDy_0n0n",	MDR.GetVec(2, "0n0n"));
			AnaData.Add("DSigmaDy_0nXnSum",	MDR.GetVec(2, "0nXnSum"));
			AnaData.Add("DSigmaDy_XnXn",	MDR.GetVec(2, "XnXn"));
		}
		else
		{
			throw std::runtime_error("DSigmaDy_LTA::Read() : wrong name");
		}
		//set errors as square root of the values
		// std::vector<double> DSigmaDy_0n0n_Err, DSigmaDy_0nXnSum_Err, DSigmaDy_XnXn_Err, DSigmaDy_AnAn_Err;
		// for (int i = 0; i < AnaData.Get("DSigmaDy_0n0n").size(); ++i)
		// {
		// 	DSigmaDy_0n0n_Err.push_back( sqrt(AnaData.Get("DSigmaDy_0n0n")[i]) );
		// 	DSigmaDy_0nXnSum_Err.push_back( sqrt(AnaData.Get("DSigmaDy_0nXnSum")[i]) );
		// 	DSigmaDy_XnXn_Err.push_back( sqrt(AnaData.Get("DSigmaDy_XnXn")[i]) );
		// 	DSigmaDy_AnAn_Err.push_back( sqrt(AnaData.Get("DSigmaDy_AnAn")[i]) );
		// }

		// AnaData.Add("DSigmaDy_0n0n_Err",		DSigmaDy_0n0n_Err);
		// AnaData.Add("DSigmaDy_0nXnSum_Err",		DSigmaDy_0nXnSum_Err);
		// AnaData.Add("DSigmaDy_XnXn_Err",		DSigmaDy_XnXn_Err);
		// AnaData.Add("DSigmaDy_AnAn_Err",		DSigmaDy_AnAn_Err);

		std::vector<double> error = vector<double>(MDR.GetVec(0, "Dy").size(), 0.0000001);
		AnaData.Add("Dy_Err",				error);
		AnaData.Add("DSigmaDy_AnAn_Err",	error);
		AnaData.Add("DSigmaDy_0n0n_Err",	error);
		AnaData.Add("DSigmaDy_0nXnSum_Err",	error);
		AnaData.Add("DSigmaDy_XnXn_Err",	error);
		return AnaData;
	}

	virtual void Draw(TLegend * leg) override
	{
		std::vector<TString> Name = {"LTA_SS", "LTA_WS", "EPS09 central"};
		std::vector<int> colors = {2,2,1};
		std::vector<int> styles = {2,5,3};

		MultiDataReader MDR({"./inFiles/LTA_Jpsi_strong_shadowing.dat", "./inFiles/LTA_Jpsi_weak_shadowing.dat", "./inFiles/EPS09_central_Jpsi.dat"},
							{"Dy", "AnAn", "0n0n", "0nXnSum", "XnXn"});

		for (int i = 0; i < MDR.SDRs.size()-1; ++i)
		{
			gr_AnAn 	= new TGraph(MDR.GetVec(i, "Dy").size(),	MDR.GetVec(i, "Dy").data(),	MDR.GetVec(i, "AnAn").data());
			gr_0n0n 	= new TGraph(MDR.GetVec(i, "Dy").size(),	MDR.GetVec(i, "Dy").data(),	MDR.GetVec(i, "0n0n").data());
			gr_0nXnSum 	= new TGraph(MDR.GetVec(i, "Dy").size(),	MDR.GetVec(i, "Dy").data(),	MDR.GetVec(i, "0nXnSum").data());
			gr_XnXn 	= new TGraph(MDR.GetVec(i, "Dy").size(),	MDR.GetVec(i, "Dy").data(),	MDR.GetVec(i, "XnXn").data());
		
			if (Case == "AnAn")
			{
				gr_AnAn->SetLineColor(colors[i]);
				gr_AnAn->SetLineStyle(styles[i]);
				gr_AnAn->SetLineWidth(2);
				gr_AnAn->Draw("lsame");
				leg->AddEntry(gr_AnAn,	Name[i].Data(),		"l" );
			}
			else if (Case == "0n0n")
			{
				gr_0n0n->SetLineColor(colors[i]);
				gr_0n0n->SetLineStyle(styles[i]);
				gr_0n0n->SetLineWidth(2);
				gr_0n0n->Draw("lsame");
				leg->AddEntry(gr_0n0n,	Name[i].Data(),		"l" );
			}
			else if (Case == "0nXnSum")
			{
				gr_0nXnSum->SetLineColor(colors[i]);
				gr_0nXnSum->SetLineStyle(styles[i]);
				gr_0nXnSum->SetLineWidth(2);
				gr_0nXnSum->Draw("lsame");
				leg->AddEntry(gr_0nXnSum,	Name[i].Data(),	"l" );
			}
			else if (Case == "XnXn")
			{
				gr_XnXn->SetLineColor(colors[i]);
				gr_XnXn->SetLineStyle(styles[i]);
				gr_XnXn->SetLineWidth(2);
				gr_XnXn->Draw("lsame");
				leg->AddEntry(gr_XnXn,	Name[i].Data(),		"l" );
			}
		}
	}

	virtual void DrawBox(std::vector<TLegend *> legs) override
	{
		std::vector<TString> Name = {" LTA_SS", " LTA_WS", "EPS09 central"};
		std::vector<int> colors = {1,2,1};
		std::vector<int> styles = {2,5,3};

		MultiDataReader MDR({"./inFiles/LTA_Jpsi_strong_shadowing.dat", "./inFiles/LTA_Jpsi_weak_shadowing.dat", "./inFiles/EPS09_central_Jpsi.dat"},
							{"Dy", "AnAn", "0n0n", "0nXnSum", "XnXn"});

		for (int i = 0; i < MDR.SDRs.size()-1; ++i)
		{
			gr_AnAn 	= new TGraph(MDR.GetVec(i, "Dy").size(),	MDR.GetVec(i, "Dy").data(),	MDR.GetVec(i, "AnAn").data());
			gr_0n0n 	= new TGraph(MDR.GetVec(i, "Dy").size(),	MDR.GetVec(i, "Dy").data(),	MDR.GetVec(i, "0n0n").data());
			gr_0nXnSum 	= new TGraph(MDR.GetVec(i, "Dy").size(),	MDR.GetVec(i, "Dy").data(),	MDR.GetVec(i, "0nXnSum").data());
			gr_XnXn 	= new TGraph(MDR.GetVec(i, "Dy").size(),	MDR.GetVec(i, "Dy").data(),	MDR.GetVec(i, "XnXn").data());
		
			if (Case == "AnAn")
			{
				gr_AnAn->SetLineColor(colors[i]);
				gr_AnAn->SetLineStyle(styles[i]);
				gr_AnAn->SetLineWidth(2);
				gr_AnAn->Draw("lsame");
				legs[i+1]->AddEntry(gr_AnAn,	" ",		"l" );
			}
			else if (Case == "0n0n")
			{
				gr_0n0n->SetLineColor(1);
				gr_0n0n->SetLineStyle(styles[i]);
				gr_0n0n->SetLineWidth(2);
				gr_0n0n->Draw("lsame");
				legs[i+1]->AddEntry(gr_0n0n,	" ",		"l" );
			}
			else if (Case == "0nXnSum")
			{
				gr_0nXnSum->SetLineColor(2);
				gr_0nXnSum->SetLineStyle(styles[i]);
				gr_0nXnSum->SetLineWidth(2);
				gr_0nXnSum->Draw("lsame");
				legs[i+1]->AddEntry(gr_0nXnSum,	" ",	"l" );
			}
			else if (Case == "XnXn")
			{
				gr_XnXn->SetLineColor(4);
				gr_XnXn->SetLineStyle(styles[i]);
				gr_XnXn->SetLineWidth(2);
				gr_XnXn->Draw("lsame");
				legs[i+1]->AddEntry(gr_XnXn,	Name[i].Data(),		"l" );
			}
		}
	}

};

struct Sigma_R_LTA : Theory
{
	TGraph* gr_SigmaVsW_StrSuppr	{nullptr};
	TGraph* gr_SigmaVsW_WeakSuppr	{nullptr};
	TGraph* gr_RvsX_StrSuppr		{nullptr};
	TGraph* gr_RvsX_WeakSuppr		{nullptr};

	Sigma_R_LTA(TString Case_ ) : Theory{Case_} {}
	virtual ~Sigma_R_LTA() override
	{
		delete gr_SigmaVsW_StrSuppr;
		delete gr_SigmaVsW_WeakSuppr;
		delete gr_RvsX_StrSuppr;
		delete gr_RvsX_WeakSuppr;
	}

	static AnalysisData Read(const TString name)
	{
		SingleDataReader SDR1("./inFiles/LTA_Jpsi_Sigma_R_1.dat", 
							{"Dy", "X_Left", "R_LTA_SS_Left", "R_LTA_WS_Left", "W_Left", "Sigma_LTA_SS_Left", "Sigma_LTA_WS_Left", "X_Right", "R_LTA_SS_Right", "R_LTA_WS_Right", "W_Right", "Sigma_LTA_SS_Right", "Sigma_LTA_WS_Right"});
		SingleDataReader SDR2("./inFiles/LTA_Jpsi_Sigma_R_2.dat",
							{"Dy", "W_Left", "Sigma_LTA_SS_Left", "Sigma_LTA_WS_Left", "W_Right", "Sigma_LTA_SS_Right", "Sigma_LTA_WS_Right"});
		
		AnalysisData anaData{name};

		std::map<TString, std::vector<double>> Map ={
			{"X",					{}},
			{"R_LTA_WS",			{}},
			{"R_LTA_SS",			{}},
			{"W",					{}},
			{"Sigma_LTA_WS",		{}},
			{"Sigma_LTA_SS",		{}}
		};

		/*Using R with IA calculated from our code for now. There is 5-10% difference in calculated form factor value compares to Vadim*/
		Map.at("X")				=	SDR1.GetVec("X_Right");
		// Map.at("R_LTA_WS")		=	SDR1.GetVec("R_LTA_WS_Right");
		// Map.at("R_LTA_SS")		=	SDR1.GetVec("R_LTA_SS_Right");
		Map.at("W")				=	SDR2.GetVec("W_Right");
		Map.at("Sigma_LTA_WS")	=	SDR2.GetVec("Sigma_LTA_WS_Right");
		Map.at("Sigma_LTA_SS")	=	SDR2.GetVec("Sigma_LTA_SS_Right");
		auto X_temp 				= SDR1.GetVec("X_Left");			std::reverse(X_temp.begin(),X_temp.end());
		// auto R_LTA_WS_temp 	= SDR1.GetVec("R_LTA_WS_Left");	std::reverse(R_LTA_WS_temp.begin(),R_LTA_WS_temp.end());
		// auto R_LTA_SS_temp 	= SDR1.GetVec("R_LTA_SS_Left");	std::reverse(R_LTA_SS_temp.begin(),R_LTA_SS_temp.end());
		auto Ws_temp 				= SDR2.GetVec("W_Left");			std::reverse(Ws_temp.begin(),Ws_temp.end());
		auto Sigma_LTA_WS_temp 	= SDR2.GetVec("Sigma_LTA_WS_Left");	std::reverse(Sigma_LTA_WS_temp.begin(),Sigma_LTA_WS_temp.end());
		auto Sigma_LTA_SS_temp 	= SDR2.GetVec("Sigma_LTA_SS_Left");	std::reverse(Sigma_LTA_SS_temp.begin(),Sigma_LTA_SS_temp.end());

		Map.at("X")			.insert(Map.at("X").begin(),			X_temp.begin(),	X_temp.end());
		// Map.at("R_LTA_WS")	.insert(Map.at("R_LTA_WS").begin(),	R_LTA_WS_temp.begin(),	R_LTA_WS_temp.end());
		// Map.at("R_LTA_SS")	.insert(Map.at("R_LTA_SS").begin(),	R_LTA_SS_temp.begin(),	R_LTA_SS_temp.end());
		Map.at("W")			.insert(Map.at("W").begin(),			Ws_temp.begin(),	Ws_temp.end());
		Map.at("Sigma_LTA_WS")	.insert(Map.at("Sigma_LTA_WS").begin(),	Sigma_LTA_WS_temp.begin(),	Sigma_LTA_WS_temp.end());
		Map.at("Sigma_LTA_SS")	.insert(Map.at("Sigma_LTA_SS").begin(),	Sigma_LTA_SS_temp.begin(),	Sigma_LTA_SS_temp.end());
		
		auto IA_pair = ImpulseApprox::GetImpulseApprox(Map.at("W"));
		for (int i = 0; i < Map.at("W").size(); ++i)
		{
			Map.at("R_LTA_WS")		.push_back(sqrt(Map.at("Sigma_LTA_WS")[i]/IA_pair.first[i]));
			Map.at("R_LTA_SS")		.push_back(sqrt(Map.at("Sigma_LTA_SS")[i]/IA_pair.first[i]));
		}

		if (name == "LTA_WS")
		{
			anaData.Add("X", 		Map.at("X"));
			anaData.Add("R", 		Map.at("R_LTA_WS"));
			anaData.Add("W", 		Map.at("W"));
			anaData.Add("Sigma", 	Map.at("Sigma_LTA_WS"));
		}
		else if (name == "LTA_SS")
		{
			anaData.Add("X", 		Map.at("X"));
			anaData.Add("R", 		Map.at("R_LTA_SS"));
			anaData.Add("W", 		Map.at("W"));
			anaData.Add("Sigma", 	Map.at("Sigma_LTA_SS"));
		}
		else
		{
			throw std::runtime_error("Wrong name in LTA_Jpsi_Sigma_R::GetAnalysisData");
		}

		return anaData;
	}

	virtual void Draw(TLegend * leg)	override
	{
		std::vector<TString> Name = {"LTA_SS","LTA_WS"};
		std::vector<int> colors = {2,2,2};
		std::vector<int> styles = {1,7,3};

		auto anaData_SS = Sigma_R_LTA::Read(Name[0]);
		auto anaData_WS = Sigma_R_LTA::Read(Name[1]);
		
		auto W = anaData_SS.Get("W");
		auto X = anaData_SS.Get("X");

		gr_SigmaVsW_StrSuppr 	= new TGraph(W.size(),	
													W.data(),			anaData_SS.Get("Sigma").data());
		gr_SigmaVsW_WeakSuppr 	= new TGraph(W.size(),	
													W.data(),			anaData_WS.Get("Sigma").data());
		gr_RvsX_StrSuppr	 	= new TGraph(X.size(),	
													X.data(),			anaData_SS.Get("R").data());
		gr_RvsX_WeakSuppr	 	= new TGraph(X.size(),	
													X.data(),			anaData_WS.Get("R").data());

		if (Case == "Sigma")
		{
			// gr_SigmaVsW_StrSuppr->SetFillColor(colors[0]);
			// gr_SigmaVsW_StrSuppr->SetFillStyle(3005);
			gr_SigmaVsW_StrSuppr->SetLineColor(colors[0]);
			gr_SigmaVsW_StrSuppr->SetLineStyle(styles[0]);
			gr_SigmaVsW_StrSuppr->SetLineWidth(2);
			gr_SigmaVsW_StrSuppr->Draw("lsame");
			leg->AddEntry(gr_SigmaVsW_StrSuppr,	Name[0].Data(),		"l" );

			// gr_SigmaVsW_WeakSuppr->SetFillColor(colors[1]);
			// gr_SigmaVsW_WeakSuppr->SetFillStyle(3005);
			gr_SigmaVsW_WeakSuppr->SetLineColor(colors[1]);
			gr_SigmaVsW_WeakSuppr->SetLineStyle(styles[1]);
			gr_SigmaVsW_WeakSuppr->SetLineWidth(2);
			gr_SigmaVsW_WeakSuppr->Draw("lsame");
			leg->AddEntry(gr_SigmaVsW_WeakSuppr,	Name[1].Data(),		"l" );
		}
		else if (Case == "R")
		{
			// gr_RvsX_StrSuppr->SetFillColor(colors[0]);
			// gr_RvsX_StrSuppr->SetFillStyle(3005);
			gr_RvsX_StrSuppr->SetLineColor(colors[0]);
			gr_RvsX_StrSuppr->SetLineStyle(styles[0]);
			gr_RvsX_StrSuppr->SetLineWidth(2);
			gr_RvsX_StrSuppr->Draw("lsame");
			leg->AddEntry(gr_RvsX_StrSuppr,	Name[0].Data(),		"l" );

			// gr_RvsX_WeakSuppr->SetFillColor(colors[1]);
			// gr_RvsX_WeakSuppr->SetFillStyle(3005);
			gr_RvsX_WeakSuppr->SetLineColor(colors[1]);
			gr_RvsX_WeakSuppr->SetLineStyle(styles[1]);
			gr_RvsX_WeakSuppr->SetLineWidth(2);
			gr_RvsX_WeakSuppr->Draw("lsame");
			leg->AddEntry(gr_RvsX_WeakSuppr,	Name[1].Data(),		"l" );
		}
	}
};

struct Sigma_R_GG : Theory
{
	TGraph * gr_SigmaVsW	{nullptr};
	TGraph * gr_RvsX		{nullptr};
	
	Sigma_R_GG(TString Case_ ) : Theory(Case_) {}
	virtual ~Sigma_R_GG()	override
	{
		delete gr_SigmaVsW;
		delete gr_RvsX;
	}

	virtual void Draw(TLegend * leg) override
	{
		std::vector<TString> Name = {"GG-hs"};
		std::vector<int> colors = {6};
		std::vector<int> styles = {5};

		std::map<TString, std::vector<double>> Map ={
			{"X",			{}},
			{"W",			{}},
			{"Sigma",		{}},
			{"R",			{}},
		};

		SingleDataReader SDR1("./inFiles/GG-hs_Jpsi_Sigma.txt", 
							{"X", "Sigma"});

		Map.at("X")	 = SDR1.GetVec("X");	
		Map.at("Sigma") = SDR1.GetVec("Sigma");
		int N = Map.at("X").size()-4;	
		Map.at("X").resize(N);	Map.at("Sigma").resize(N);
		
		for(int j=0; j<Map.at("X").size(); j++ )
		{
			Map.at("W")			.push_back(ParamConverter::x2W(Map.at("X")[j]));
			Map.at("Sigma")[j] *=	0.001;
		}

		auto IA_pair = ImpulseApprox::GetImpulseApprox(Map.at("W"));
		for (int i = 0; i < N; ++i)
		{
			Map.at("R")		.push_back(sqrt(Map.at("Sigma")[i]/IA_pair.first[i]));
		}

		auto gr_SigmaVsW 	= new TGraph(Map.at("W").size(),	
									Map.at("W").data(),			Map.at("Sigma").data());
		auto gr_RvsW 		= new TGraph(Map.at("X").size(),	
									Map.at("X").data(),			Map.at("R").data());

		if (Case == "Sigma")
		{
			// gr_SigmaVsW->SetFillColor(colors[0]);
			// gr_SigmaVsW->SetFillStyle(3005);
			gr_SigmaVsW->SetLineColor(colors[0]);
			gr_SigmaVsW->SetLineStyle(styles[0]);
			gr_SigmaVsW->SetLineWidth(2);
			gr_SigmaVsW->Draw("lsame");
			leg->AddEntry(gr_SigmaVsW,	Name[0].Data(),		"l" );

		}
		else if (Case == "R")
		{
			// gr_RvsW->SetFillColor(colors[0]);
			// gr_RvsW->SetFillStyle(3005);
			gr_RvsW->SetLineColor(colors[0]);
			gr_RvsW->SetLineStyle(styles[0]);
			gr_RvsW->SetLineWidth(2);
			gr_RvsW->Draw("lsame");
			leg->AddEntry(gr_RvsW,	Name[0].Data(),		"l" );
		}
	}
};

struct DSigmaDy_Sigma_R_bBK : Theory
{
	TGraph*	gr_AnAn		{nullptr};
	TGraph*	gr_Simgas	{nullptr};
	TGraph*	gr_R		{nullptr};

	DSigmaDy_Sigma_R_bBK(TString Case_ ) : Theory(Case_) {}
	virtual ~DSigmaDy_Sigma_R_bBK()	override
	{
		delete gr_AnAn;
		delete gr_Simgas;
		delete gr_R;
	}

	virtual void Draw(TLegend * leg) override
	{
		std::vector<TString> Name	 = {"bBK_GG",	"bBK_A"};

		std::vector<int> colors = {4, 4};
		std::vector<int> styles = {1, 7};

		MultiDataReader MDR1({"./inFiles/bBK_GG_Jpsi_Sigma.txt", "./inFiles/bBK_A_Jpsi_Sigma.txt"},
							{"X", "W", "Sigma"});
		MultiDataReader MDR2({"./inFiles/bBK_GG_Jpsi_Xsec.txt", "./inFiles/bBK_A_Jpsi_Xsec.txt"},
							{"Dy", "DSigmaDy_AnAn"});

		for (int i = 0; i < MDR1.SDRs.size(); ++i)
		{
			std::map<TString, std::vector<double>> Map ={
				{"W",			{MDR1.GetVec(i, "W")}},
				{"Sigma",		{MDR1.GetVec(i, "Sigma")}},
				{"Dy",			{MDR2.GetVec(i, "Dy")}},
				{"DSigmaDy_AnAn",{MDR2.GetVec(i, "DSigmaDy_AnAn")}},
				{"X",			{MDR1.GetVec(i, "X")}},
				{"R",			{}}
			};

			auto IA_pair = ImpulseApprox::GetImpulseApprox(Map.at("W"));
			for (int j = 0; j < Map.at("W").size(); ++j)
			{
				Map.at("Sigma")[j] *= 0.001;
				Map.at("R")		.push_back(sqrt(Map.at("Sigma")[j]/IA_pair.first[j]));
			}

			gr_AnAn 	= new TGraph(Map.at("Dy").size(),	Map.at("Dy").data(),	Map.at("DSigmaDy_AnAn").data());
			gr_Simgas 	= new TGraph(Map.at("W").size(),	Map.at("W").data(),	Map.at("Sigma").data());
			gr_R 		= new TGraph(Map.at("X").size(),	Map.at("X").data(),	Map.at("R").data());
		
			if (Case == "DSigmaDy_AnAn"){
				gr_AnAn->SetLineColor(colors[i]);
				gr_AnAn->SetLineStyle(styles[i]);
				gr_AnAn->SetLineWidth(2);
				gr_AnAn->Draw("lsame");
				leg->AddEntry(gr_AnAn,	Name[i].Data(),		"l" );
			}
			else if (Case == "Sigma"){
				gr_Simgas->SetLineColor(colors[i]);
				gr_Simgas->SetLineStyle(styles[i]);
				gr_Simgas->SetLineWidth(2);
				gr_Simgas->Draw("lsame");
				leg->AddEntry(gr_Simgas,	Name[i].Data(),		"l" );
			}
			else if (Case == "R"){
				gr_R->SetLineColor(colors[i]);
				gr_R->SetLineStyle(styles[i]);
				gr_R->SetLineWidth(2);
				gr_R->Draw("lsame");
				leg->AddEntry(gr_R,	Name[i].Data(),		"l" );
			}
		}
	}
};

struct DSigmaDy_Sigma_R_CD : Theory
{
	TGraph* gr_AnAn		{nullptr};
	TGraph* gr_Simgas	{nullptr};
	TGraph* gr_R		{nullptr};

	DSigmaDy_Sigma_R_CD(TString Case_ ) : Theory(Case_) {}
	virtual ~DSigmaDy_Sigma_R_CD()	override
	{
		delete gr_AnAn;
		delete gr_Simgas;
		delete gr_R;
	}

	virtual void Draw(TLegend * leg)	override
	{
		std::vector<TString> Name	 = {"CD_BGK",	"CD_GBW",	"CD_IIM"};
		std::vector<int> colors = {8, 8, 8};
		std::vector<int> styles = {1, 7, 3};

		MultiDataReader MDR1({"./inFiles/CD_BGK_Jpsi_Sigma.dat", "./inFiles/CD_GBW_Jpsi_Sigma.dat", "./inFiles/CD_IIM_Jpsi_Sigma.dat"},
							{"W", "Sigma"});
		MultiDataReader MDR2({"./inFiles/CD_BGK_Jpsi_Xsec.dat", "./inFiles/CD_GBW_Jpsi_Xsec.dat", "./inFiles/CD_IIM_Jpsi_Xsec.dat"},
							{"Dy", "DSigmaDy"});

		for (int i = 0; i < MDR1.SDRs.size(); ++i)
		{
			std::map<TString, std::vector<double>> Map ={
				{"W",			{MDR1.GetVec(i, "W")}},
				{"Sigma",		{MDR1.GetVec(i, "Sigma")}},
				{"Dy",		{MDR2.GetVec(i, "Dy")}},
				{"DSigmaDy",		{MDR2.GetVec(i, "DSigmaDy")}},
				{"X",			{}},
				{"R",			{}}
			};
			auto IA_pair = ImpulseApprox::GetImpulseApprox(Map.at("W"));
			for (int j = 0; j < Map.at("W").size(); ++j)
			{
				Map.at("Sigma")[j] *= 0.001;
				Map.at("X")	.push_back(ParamConverter::W2x( Map.at("W")[j] ) );
				Map.at("R")		.push_back(sqrt(Map.at("Sigma")[j]/IA_pair.first[j]));
			}
			for (int j = 0; j < Map.at("Dy").size(); ++j)
			{
				Map.at("DSigmaDy")[j] *= 0.001;
			}

			auto gr_AnAn	= new TGraph(Map.at("Dy").size(),	Map.at("Dy").data(),	Map.at("DSigmaDy").data());
			auto gr_Simgas 	= new TGraph(Map.at("W").size(),	Map.at("W").data(),	Map.at("Sigma").data());
			auto gr_R 		= new TGraph(Map.at("X").size(),	Map.at("X").data(),	Map.at("R").data());
		
			if (Case == "AnAn"){
				gr_AnAn->SetLineColor(colors[i]);
				gr_AnAn->SetLineStyle(styles[i]);
				gr_AnAn->SetLineWidth(2);
				gr_AnAn->Draw("lsame");
				leg->AddEntry(gr_AnAn,	Name[i].Data(),		"l" );
			}
			else if (Case == "Sigma"){
				gr_Simgas->SetLineColor(colors[i]);
				gr_Simgas->SetLineStyle(styles[i]);
				gr_Simgas->SetLineWidth(2);
				gr_Simgas->Draw("lsame");
				leg->AddEntry(gr_Simgas,	Name[i].Data(),		"l" );
			}
			else if (Case == "R"){
				gr_R->SetLineColor(colors[i]);
				gr_R->SetLineStyle(styles[i]);
				gr_R->SetLineWidth(2);
				gr_R->Draw("lsame");
				leg->AddEntry(gr_R,	Name[i].Data(),		"l" );
			}
		}
	}
};