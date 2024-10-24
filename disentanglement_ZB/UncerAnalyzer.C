/*
Calculate relative uncertainty of Multiple AnalysisData

Run this macro in ROOT: To Save The Uncertainty of the AnalysisData

Dec. 2022
JiaZhao Lin
*/

#ifndef UncerAnalyzer_H
#define UncerAnalyzer_H

#include "DisentanglementAnalyzer.C"

struct PairUncer
{
	//calculate relative uncertainty of two AnalysisData
	//and add the result to the second AnalysisData

	static void RelativeUncer(const AnalysisData& DefaultAnaData, AnalysisData& UncerAnaData, const TString param)
	{
		std::vector<double> Relative_Uncer;
		std::vector<double> Relative_Uncer_Signed;

		//Calculate Relative Uncertainty
		auto Default = DefaultAnaData.Get(param);
		auto Uncer = UncerAnaData.Get(param);

		for ( auto i = 0; i < Default.size(); i++ )
		{
			Relative_Uncer			.push_back(	abs(	(Default[i] - Uncer[i]) / Default[i]	) * 100.0	);
			Relative_Uncer_Signed	.push_back(	   (	(Default[i] - Uncer[i]) / Default[i]	) * 100.0	);
		}

		UncerAnaData	.Add( Form("%s_SysUncer", param.Data())			, Relative_Uncer );
		UncerAnaData	.Add( Form("%s_SysUncer_Signed", param.Data())	, Relative_Uncer_Signed );
	}
	static void SqrtUncer(const AnalysisData& DefaultAnaData, AnalysisData& UncerAnaData, const TString param)
	{
		std::vector<double> Sqrt_Uncer;

		//Calculate Relative Uncertainty
		auto Default = DefaultAnaData.Get(param);
		auto Uncer = UncerAnaData.Get(param);
		auto Sq   = UncerAnaData.Get("Sigma_SysUncer");	//Important	!! use the uncertainty of Sigma to calculate uncertainty of R

		for ( auto i = 0; i < Default.size(); i++ )
		{
			Sqrt_Uncer.	push_back(	0.5 * Sq[i]	);
		}

		UncerAnaData.Add( Form("%s_SysUncer", param.Data()), Sqrt_Uncer );
	}
	static void Calculate(const AnalysisData& DefaultAnaData, AnalysisData& UncerAnaData)
	{
		RelativeUncer(	DefaultAnaData,	UncerAnaData,	"Sigma"				);
		SqrtUncer	 (	DefaultAnaData,	UncerAnaData,	"R"					);	//Important	!! use the uncertainty of Sigma to calculate uncertainty of R

		RelativeUncer(	DefaultAnaData,	UncerAnaData,	"DSigmaDy_AnAn"		);
		RelativeUncer(	DefaultAnaData,	UncerAnaData,	"DSigmaDy_0n0n"		);
		RelativeUncer(	DefaultAnaData,	UncerAnaData,	"DSigmaDy_0nXnSum"	);
		RelativeUncer(	DefaultAnaData,	UncerAnaData,	"DSigmaDy_XnXn"		);
	}
};

struct CombineUncer
{
	//combine uncertainties of multiple AnalysisData
	//and add the result to a new AnalysisData

	static std::vector<double> Combine(const std::vector< AnalysisData >& AnaDataList,	TString param, const bool isSigned = false)
	{
		//initialize Combined Uncertainty with 0
		const int n_data = AnaDataList[0].Get(param).size();
		std::vector<double> Combined_Uncer( n_data, 0.0 );

		for ( auto i = 0; i < AnaDataList.size(); i++ )
		{
			auto temp_param = AnaDataList[i].Get(param);
			assert( n_data == temp_param.size() );

			for ( auto j = 0; j < temp_param.size(); j++ )
			{
				double sign = 1.0;
				if ( isSigned ) 
				{
					//getting the sign of the uncertainty from their sum
					if (std::signbit( Combined_Uncer[j]+temp_param[j] ))	sign *= -1.0;
				}
				Combined_Uncer[j] = TMath::Hypot(	Combined_Uncer[j],	 temp_param[j] );
				Combined_Uncer[j] *= sign;
			}
		}
		return Combined_Uncer;
	}

	static AnalysisData Calculate(const std::vector< AnalysisData >& AnaDataList, const std::vector<TString> paramList)
	{
		AnalysisData AnaData("CombineUncer");

		assert( AnaDataList.size() > 0 );
		
		for(int i = 0; i < paramList.size(); i++)
		{
			std::vector<double> Combined_Uncer = Combine( AnaDataList,	paramList[i] );
			AnaData.Add( paramList[i], Combined_Uncer );
			if( paramList[i] != "R_SysUncer")
			{
				std::vector<double> Combined_Uncer_Signed = Combine( AnaDataList,	paramList[i]+"_Signed" , true);
				AnaData.Add( paramList[i]+"_Signed", Combined_Uncer_Signed );
			}
		}
		return AnaData;
	}
};

struct LargestUncer
{
	//find the largest uncertainty of multiple AnalysisData
	//and add the result to a new AnalysisData

	static std::vector<double> Largest(const std::vector< AnalysisData >& AnaDataList,	TString param, const bool isSigned = false)
	{
		//initialize Largest Uncertainty with 0
		const int n_data = AnaDataList[0].Get(param).size();
		std::vector<double> Largest_Uncer( n_data, 0.0 );

		for ( auto i = 0; i < AnaDataList.size(); i++ )
		{
			auto temp_param = AnaDataList[i].Get(param);
			assert( n_data == temp_param.size() );

			for ( auto j = 0; j < temp_param.size(); j++ )
			{
				if ( isSigned ) 
				{
					if (	abs(Largest_Uncer[j]) <	abs(temp_param[j]) ) Largest_Uncer[j] = temp_param[j];
				}
				else			Largest_Uncer[j] = TMath::Max(	Largest_Uncer[j],	 temp_param[j] );
			}
		}
		return Largest_Uncer;
	}

	static AnalysisData Calculate(const std::vector< AnalysisData >& AnaDataList,	const std::vector<TString> paramList)
	{
		AnalysisData AnaData("LargestUncer");

		for(int i = 0; i < paramList.size(); i++)
		{
			std::vector<double> Largest_Uncer 			= Largest( AnaDataList,	paramList[i] );
			AnaData.Add( paramList[i], Largest_Uncer );

			if (paramList[i] != "R_SysUncer") 
			{
				std::vector<double> Largest_Uncer_Signed 	= Largest( AnaDataList,	paramList[i]+"_Signed", true );
				AnaData.Add( paramList[i]+ "_Signed", Largest_Uncer_Signed );
			}
		}
		return AnaData;
	}
};

struct UncerAnalyzer : Analyzer
{
	//analyze uncertainties of the AnalysisData.
	//the result will be added to the new AnalysisData: AnaData, containing only the sys uncertainties
	AnalysisDataObserver obs;
	AnalysisData AnaData{"SysUncer"};

	//use this to record the breakdown of uncertainties
	std::map<TString, AnalysisData> AnaData_Breakdown;

	// those parameters must be in the same order
	std::vector<TString> paramList 			= {"Sigma", "R", "DSigmaDy_AnAn", "DSigmaDy_0n0n", "DSigmaDy_0nXnSum", "DSigmaDy_XnXn"};
	std::vector<TString> SysUncer_paramList;
	std::vector<TString> SysUncer_Signed_paramList;
	std::vector<TString> SysErr_paramList;
	
	void Init()
	{
		//initialize the paramList
		for ( auto i = 0; i < paramList.size(); i++ )
		{
			SysUncer_paramList.push_back( paramList[i] + "_SysUncer" );
			SysUncer_Signed_paramList.push_back( paramList[i] + "_SysUncer_Signed" );
			SysErr_paramList.push_back( paramList[i] + "_SysErr" );
		}
	}

	const int template_option  = 1; //0:OldCohJpsi; 1:NewCohJpsi (w R+1fm);
	const TString template_Name[2] = {"", "_NewCohJpsi"};

	AnalysisData CB_Poly3_AnaData 			{	"CB_Poly3_AnaData"			,	"../signalExt/JpsiXsecValues/JpsiXsec_Default_18RapBins.root"				};
//	AnalysisData CB_Poly4_AnaData 			{	"CB_Poly4_AnaData"			,	Form("../signalExt/JpsiXsecValues/JpsiXsec_CB_Poly4_PUShuai_6RapBins%s.appliedTnP.root", template_Name[template_option].Data())				};
//	AnalysisData CB_FixCBAN_Poly3_AnaData 	{	"CB_FixCBAN_Poly3_AnaData"	,	Form("../signalExt/JpsiXsecValues/JpsiXsec_CB_FixCBAN_Poly3_PUShuai_6RapBins%s.appliedTnP.root", template_Name[template_option].Data())		};
//	AnalysisData CBG_Poly3_AnaData 			{	"CBG_Poly3_AnaData"			,	Form("../signalExt/JpsiXsecValues/JpsiXsec_CBG_Poly3_PUShuai_6RapBins%s.appliedTnP.root", template_Name[template_option].Data())			};
//	AnalysisData CB_Poly3_SdB_AnaData 		{	"CB_Poly3_SdB_AnaData"		,	Form("../signalExt/JpsiXsecValues/JpsiXsec_CB_Poly3_PUShuai_SdB_6RapBins%s.appliedTnP.root", template_Name[template_option].Data())			};
//	//Mass Range uncertainty is using old Jpsi template because the uncertainty is too large in the new one. may need to be updated
//	AnalysisData CB_Poly3_NarrMass_AnaData	{	"CB_Poly3_NarrMass_AnaData"	,	Form("../signalExt/JpsiXsecValues/JpsiXsec_CB_Poly3_PUShuai_NarrowMass_6RapBins%s.appliedTnP.root", template_Name[0].Data())				};
//	AnalysisData CB_Poly3_WideMass_AnaData	{	"CB_Poly3_WideMass_AnaData"	,	Form("../signalExt/JpsiXsecValues/JpsiXsec_CB_Poly3_PUShuai_WideMass_6RapBins%s.appliedTnP.root", template_Name[0].Data())					};
//	AnalysisData CB_Poly3_looseHF_AnaData 	{	"CB_Poly3_looseHF_AnaData"	,	Form("../signalExt/JpsiXsecValues/JpsiXsec_CB_Poly3_PUShuai_6RapBins%s.appliedTnP.looseHF.root", template_Name[template_option].Data())		};
	// AnalysisData CB_Poly3_tightHF_AnaData 	{	"CB_Poly3_tightHF_AnaData"	,	Form("../signalExt/JpsiXsecValues/JpsiXsec_CB_Poly3_PUShuai_6RapBins%s.tightHF.root", template_Name[template_option].Data())				};
	AnalysisData CB_Poly3_fluxP_AnaData 	{	"CB_Poly3_fluxP_AnaData"	,	"../signalExt/JpsiXsecValues/JpsiXsec_Default_18RapBins.root"				};
	AnalysisData CB_Poly3_fluxM_AnaData 	{	"CB_Poly3_fluxM_AnaData"	,	"../signalExt/JpsiXsecValues/JpsiXsec_Default_18RapBins.root"				};
	AnalysisData CB_Poly3_fluxPM_AnaData 	{	"CB_Poly3_fluxPM_AnaData"	,	"../signalExt/JpsiXsecValues/JpsiXsec_Default_18RapBins.root"				};
	AnalysisData CB_Poly3_fluxMP_AnaData 	{	"CB_Poly3_fluxMP_AnaData"	,	"../signalExt/JpsiXsecValues/JpsiXsec_Default_18RapBins.root"				};
	AnalysisData CB_Poly3_fluxEMDP_AnaData 	{	"CB_Poly3_fluxEMDP_AnaData"	,	"../signalExt/JpsiXsecValues/JpsiXsec_Default_18RapBins.root"				};
	AnalysisData CB_Poly3_fluxEMDM_AnaData 	{	"CB_Poly3_fluxEMDM_AnaData"	,	"../signalExt/JpsiXsecValues/JpsiXsec_Default_18RapBins.root"				};
//	AnalysisData CB_Poly3_TnP_Low_AnaData 	{	"CB_Poly3_TnP_Low_AnaData"	,	Form("../signalExt/JpsiXsecValues/JpsiXsec_CB_Poly3_PUShuai_6RapBins%s.appliedTnP_Low.root", template_Name[template_option].Data())			};
//	AnalysisData CB_Poly3_TnP_Hig_AnaData	{	"CB_Poly3_TnP_Hig_AnaData"	,	Form("../signalExt/JpsiXsecValues/JpsiXsec_CB_Poly3_PUShuai_6RapBins%s.appliedTnP_Hig.root", template_Name[template_option].Data())			};
//	AnalysisData CB_Poly3_PU_AnaData 		{	"CB_Poly3_PU_AnaData"		,	Form("../signalExt/JpsiXsecValues/JpsiXsec_CB_Poly3_PUATLAS_6RapBins%s.appliedTnP.root", template_Name[template_option].Data())				};
//	AnalysisData CB_Poly3_temp_AnaData 		{	"CB_Poly3_temp_AnaData"		,	Form("../signalExt/JpsiXsecValues/JpsiXsec_CB_Poly3_PUShuai_6RapBins.appliedTnP.root")														};
//	AnalysisData CB_Poly3_CohPtCut_AnaData 	{	"CB_Poly3_CohPtCut_AnaData"	,	Form("../signalExt/JpsiXsecValues/JpsiXsec_CB_Poly3_PUShuai_CohPtCut0p3_6RapBins%s.appliedTnP.root", template_Name[template_option].Data())	};

	const AnalysisData& Default_AnaData = CB_Poly3_AnaData;
	
	UncerAnalyzer() : Analyzer{AnaData} {	Init();	AnaData.Subscribe(&obs);	};
	
	//Uses analysis data, calculate apply dientaglementanlayzer and calculate the uncertainty
	//The uncertainty is calculated by the difference between the analysis data and the default analysis data
	//The uncertainty is added to the analysis data 
	void GetPairUncer()
	{
		DisentanglementAnalyzer CB_Poly3_Ana 			{CB_Poly3_AnaData			};	CB_Poly3_Ana.SetSaveCovMatrix(true);	CB_Poly3_Ana			.Handle();
//		DisentanglementAnalyzer CB_Poly4_Ana 			{CB_Poly4_AnaData			};	CB_Poly4_Ana			.Handle();
//		DisentanglementAnalyzer CB_FixCBAN_Poly3_Ana 	{CB_FixCBAN_Poly3_AnaData	};	CB_FixCBAN_Poly3_Ana 	.Handle();
//		DisentanglementAnalyzer CBG_Poly3_Ana 			{CBG_Poly3_AnaData			};	CBG_Poly3_Ana 			.Handle();
//		DisentanglementAnalyzer CB_Poly3_SdB_Ana 		{CB_Poly3_SdB_AnaData		};	CB_Poly3_SdB_Ana 		.Handle();
//		DisentanglementAnalyzer CB_Poly3_NarrMass_Ana	{CB_Poly3_NarrMass_AnaData	};	CB_Poly3_NarrMass_Ana	.Handle();
//		DisentanglementAnalyzer CB_Poly3_WideMass_Ana	{CB_Poly3_WideMass_AnaData	};	CB_Poly3_WideMass_Ana	.Handle();
//		DisentanglementAnalyzer CB_Poly3_looseHF_Ana 	{CB_Poly3_looseHF_AnaData	};	CB_Poly3_looseHF_Ana 	.Handle();
		// DisentanglementAnalyzer CB_Poly3_tightHF_Ana 	{CB_Poly3_tightHF_AnaData	};	CB_Poly3_tightHF_Ana 	.Handle();
		DisentanglementAnalyzer CB_Poly3_fluxP_Ana 		{CB_Poly3_fluxP_AnaData, 	"../anaSimu/flux/", "_P"};	CB_Poly3_fluxP_Ana 		.Handle();
		DisentanglementAnalyzer CB_Poly3_fluxM_Ana 		{CB_Poly3_fluxM_AnaData, 	"../anaSimu/flux/", "_M"};	CB_Poly3_fluxM_Ana 		.Handle();
		DisentanglementAnalyzer CB_Poly3_fluxPM_Ana 	{CB_Poly3_fluxPM_AnaData,	"../anaSimu/flux/", "_PM"};	CB_Poly3_fluxPM_Ana 	.Handle();
		DisentanglementAnalyzer CB_Poly3_fluxMP_Ana 	{CB_Poly3_fluxMP_AnaData,	"../anaSimu/flux/", "_MP"};	CB_Poly3_fluxMP_Ana 	.Handle();
		DisentanglementAnalyzer CB_Poly3_fluxEMDP_Ana 	{CB_Poly3_fluxEMDP_AnaData,	"../anaSimu/flux/", "_EMDP"};	CB_Poly3_fluxEMDP_Ana 	.Handle();
		DisentanglementAnalyzer CB_Poly3_fluxEMDM_Ana 	{CB_Poly3_fluxEMDM_AnaData,	"../anaSimu/flux/", "_EMDM"};	CB_Poly3_fluxEMDM_Ana 	.Handle();
//		DisentanglementAnalyzer CB_Poly3_TnP_Low_Ana 	{CB_Poly3_TnP_Low_AnaData	};	CB_Poly3_TnP_Low_Ana 	.Handle();
//		DisentanglementAnalyzer CB_Poly3_TnP_Hig_Ana	{CB_Poly3_TnP_Hig_AnaData	};	CB_Poly3_TnP_Hig_Ana 	.Handle();
//		DisentanglementAnalyzer CB_Poly3_PU_Ana 		{CB_Poly3_PU_AnaData		};	CB_Poly3_PU_Ana 		.Handle();
//		DisentanglementAnalyzer CB_Poly3_temp_Ana 		{CB_Poly3_temp_AnaData		};	CB_Poly3_temp_Ana 		.Handle();
//		DisentanglementAnalyzer CB_Poly3_CohPtCut_Ana 	{CB_Poly3_CohPtCut_AnaData	};	CB_Poly3_CohPtCut_Ana 	.Handle();

		// PairUncer::Calculate(	Default_AnaData,	CB_Poly3_AnaData			);
//		PairUncer::Calculate(	Default_AnaData,	CB_Poly4_AnaData			);
//		PairUncer::Calculate(	Default_AnaData,	CB_FixCBAN_Poly3_AnaData	);
//		PairUncer::Calculate(	Default_AnaData,	CBG_Poly3_AnaData			);
//		PairUncer::Calculate(	Default_AnaData,	CB_Poly3_SdB_AnaData		);
		//Mass Range uncertainty is comparing to the old Jpsi template because the uncertainty is too large in the new one. may need to be updated!!!!!!!!!!!!
//		PairUncer::Calculate(	CB_Poly3_temp_AnaData,	CB_Poly3_NarrMass_AnaData	);
//		PairUncer::Calculate(	CB_Poly3_temp_AnaData,	CB_Poly3_WideMass_AnaData	);
//		PairUncer::Calculate(	Default_AnaData,	CB_Poly3_looseHF_AnaData	);
		// PairUncer::Calculate(	Default_AnaData,	CB_Poly3_tightHF_AnaData	);
		PairUncer::Calculate(	Default_AnaData,	CB_Poly3_fluxP_AnaData		);
		PairUncer::Calculate(	Default_AnaData,	CB_Poly3_fluxM_AnaData		);
		PairUncer::Calculate(	Default_AnaData,	CB_Poly3_fluxPM_AnaData		);
		PairUncer::Calculate(	Default_AnaData,	CB_Poly3_fluxMP_AnaData		);
		PairUncer::Calculate(	Default_AnaData,	CB_Poly3_fluxEMDP_AnaData	);
		PairUncer::Calculate(	Default_AnaData,	CB_Poly3_fluxEMDM_AnaData	);
//		PairUncer::Calculate(	Default_AnaData,	CB_Poly3_TnP_Low_AnaData	);
//		PairUncer::Calculate(	Default_AnaData,	CB_Poly3_TnP_Hig_AnaData	);
//		PairUncer::Calculate(	Default_AnaData,	CB_Poly3_PU_AnaData			);
//		PairUncer::Calculate(	Default_AnaData,	CB_Poly3_temp_AnaData		);
//		PairUncer::Calculate(	Default_AnaData,	CB_Poly3_CohPtCut_AnaData	);
	}

	void PrintHandling() const
    {
        cout << endl << std::setfill('+') << std::setw(100) << Form("%s::Handle Is Handling The Business... ", typeid(this).name()) << endl;
        cout << setfill(' ');
    }

	//Given the systematic uncertainty, add a vector of uncertainty to it
	void AddToSysUncer(std::vector<double> v,	TString param)
	{
		std::vector<double> SysUncer = AnaData.Get( param );

		if (v.size() != SysUncer.size()) throw std::runtime_error("AddToSysUncer: vector size not match!");

		for (int i = 0; i < SysUncer.size(); ++i)
		{
			SysUncer[i] = TMath::Hypot(SysUncer[i], v[i]);
		}
		AnaData.Update( param, SysUncer );
	}

	//Given the systematic uncertainty saved in AnaData, add a const uncertainty to it
	//only do this once
	void AddConsSysUncer()
	{
		//check if it is called the first time
		static bool called = false;
		if (called) throw std::runtime_error("AddConsSysUncer: called twice!");
		called = true;

		std::vector<double> v_Lumi_Uncer;
		std::vector<double> v_BR_Uncer;

		//Add Lumi and BR uncertainty to each of the systematic uncertainty
		for (int i = 0; i < SysUncer_paramList.size(); ++i)
		{
			if (SysUncer_paramList[i] == "R_SysUncer")
			{
				v_Lumi_Uncer	=	std::vector<double>	(AnaData.GetSize(SysUncer_paramList[i]),	0.5*Lumi_Uncer);
				v_BR_Uncer		=	std::vector<double> (AnaData.GetSize(SysUncer_paramList[i]),	0.5*br_Jpsi2uu_Uncer);
			}
			else{
				v_Lumi_Uncer	=	std::vector<double>	(AnaData.GetSize(SysUncer_paramList[i]),	Lumi_Uncer);
				v_BR_Uncer		=	std::vector<double> (AnaData.GetSize(SysUncer_paramList[i]),	br_Jpsi2uu_Uncer);
			}
			AddToSysUncer(v_Lumi_Uncer, SysUncer_paramList.at(i));
			AddToSysUncer(v_BR_Uncer, SysUncer_paramList.at(i));
		}

		//Add Lumi and BR uncertainty to the breakdown uncertainty
		AnalysisData AnaData_Lumi {"Lumi"};
		AnalysisData AnaData_BR   {"BR"};
		AnalysisData AnaData_IA   {"IA_Uncer"};
		AnaData_Lumi	.Add("Lumi_SysUncer", 	std::vector<double>	(AnaData.GetSize("Sigma_SysUncer"),	Lumi_Uncer));
		AnaData_BR		.Add("BR_SysUncer", 	std::vector<double>	(AnaData.GetSize("Sigma_SysUncer"),	br_Jpsi2uu_Uncer));
		AnaData_Lumi	.Add("R_Lumi_SysUncer", std::vector<double>	(AnaData.GetSize("R_SysUncer"),		0.5*Lumi_Uncer));
		AnaData_BR		.Add("R_BR_SysUncer", 	std::vector<double>	(AnaData.GetSize("R_SysUncer"),		0.5*br_Jpsi2uu_Uncer));

		//now add the IA uncertainty to the R systematic uncertainty
		std::vector<double> v_IA_Uncer;
		for (int i = 0; i < Default_AnaData.GetSize("Sigma_IA"); ++i)
		{
			v_IA_Uncer.push_back( 0.5 * 100.0 * Default_AnaData.Get("Sigma_IA_Err", i) / Default_AnaData.Get("Sigma_IA", i) );
		}
		AddToSysUncer(v_IA_Uncer, "R_SysUncer");

		//take out the experimental uncertainty
		auto v_Sigma_ExperiUncer = AnaData.Get("Sigma_SysUncer");
		auto v_Sigma_TheoryUncer = AnaData.Get("Sigma_TheorySysUncer");
		auto v_R_ExpriUncer = AnaData.Get("R_SysUncer");
		auto v_R_TheoryUncer = AnaData.Get("R_TheorySysUncer");
		for (int i = 0; i < v_Sigma_ExperiUncer.size(); ++i)
		{
			v_Sigma_ExperiUncer[i] 	= TMath::Sqrt( TMath::Power(v_Sigma_ExperiUncer[i], 2) 	- TMath::Power(v_Sigma_TheoryUncer[i], 2) );

			//include the IA uncertainty to the R theoretical uncertainty first
			v_R_TheoryUncer[i]		= TMath::Hypot(v_R_TheoryUncer[i], v_IA_Uncer[i]);
			v_R_ExpriUncer[i]		= TMath::Sqrt( TMath::Power(v_R_ExpriUncer[i], 2) 		- TMath::Power(v_R_TheoryUncer[i], 2) );
		}
		AnaData.Add("Sigma_ExperiSysUncer"	, v_Sigma_ExperiUncer);
		AnaData.Update("R_TheorySysUncer",v_R_TheoryUncer);
		AnaData.Add("R_ExperiSysUncer"		, v_R_ExpriUncer);

		//now add the IA uncertainty to the breakdown uncertainty
		AnaData_IA.Add("R_SysUncer", v_IA_Uncer);
		AnaData_Breakdown.insert({"IA", AnaData_IA});

		//Add AnaData_Const to AnaData_Breakdown
		AnaData_Breakdown.insert({"Lumi", AnaData_Lumi});
		AnaData_Breakdown.insert({"BR", AnaData_BR});

		AnalysisData AnaData_Total{AnaData};
		AnaData_Breakdown.insert({"Total", AnaData_Total});
		
	}


	void CalculateSysUncer()
	{
			
//		auto MassFitRangeUncer	= LargestUncer::Calculate({	CB_Poly3_NarrMass_AnaData,	CB_Poly3_WideMass_AnaData	},	SysUncer_paramList);

//		auto FitUncer 			= CombineUncer::Calculate({	CB_Poly4_AnaData,			CB_FixCBAN_Poly3_AnaData,	
//															CBG_Poly3_AnaData,			CB_Poly3_temp_AnaData,
//															CB_Poly3_CohPtCut_AnaData,	CB_Poly3_SdB_AnaData,		
//															MassFitRangeUncer										},	SysUncer_paramList);
		auto FluxNucleusUncer	= LargestUncer::Calculate({	CB_Poly3_fluxM_AnaData,		CB_Poly3_fluxP_AnaData,
															CB_Poly3_fluxPM_AnaData,	CB_Poly3_fluxMP_AnaData		},	SysUncer_paramList);
		
		auto FluxEMDUncer		= LargestUncer::Calculate({	CB_Poly3_fluxEMDP_AnaData,	CB_Poly3_fluxEMDM_AnaData	},	SysUncer_paramList);

		auto FluxUncer			= CombineUncer::Calculate({	FluxNucleusUncer,			FluxEMDUncer				},	SysUncer_paramList);
//		auto TnPUncer			= LargestUncer::Calculate({	CB_Poly3_TnP_Low_AnaData,	CB_Poly3_TnP_Hig_AnaData	},	SysUncer_paramList);

//		auto TotalUncer			= CombineUncer::Calculate({	FitUncer,					FluxUncer,			
//															TnPUncer,					CB_Poly3_PU_AnaData,		
//															CB_Poly3_looseHF_AnaData								},	SysUncer_paramList);

//		AnaData.LoadMap(TotalUncer.GetMap());
		AnaData.LoadMap(FluxUncer.GetMap());

		//Add Theoritical uncertainty to sperately
		AnaData.Add("Sigma_TheorySysUncer",	FluxUncer.Get("Sigma_SysUncer"));
		AnaData.Add("R_TheorySysUncer",		FluxUncer.Get("R_SysUncer"));
		AddConsSysUncer();	//Add the constant systematic uncertainty

		//load the breakdown of systematic uncertainties to AnaData_Breakdown
//		AnaData_Breakdown.insert(	{"Signal Ext",		FitUncer}	);
//		AnaData_Breakdown.insert(	{"PhotonFlux",		FluxUncer}		);
//		AnaData_Breakdown.insert(	{"PhotonFluxNucleus",	FluxNucleusUncer}	);
//		AnaData_Breakdown.insert(	{"PhotonFluxEMD",		FluxEMDUncer}		);
//		AnaData_Breakdown.insert(	{"TnP",				TnPUncer}		);
//		AnaData_Breakdown.insert(	{"HFveto",			CB_Poly3_looseHF_AnaData}	); // HF veto used loose HF cut for now, may update later
//		AnaData_Breakdown.insert(	{"n-PileUp",		CB_Poly3_PU_AnaData}		);

	}

	//caculate the total systematic error, must be done after all the systematic uncertainty is calculated
	void CalculateSysErr()
	{
		for (int i = 0; i < SysErr_paramList.size(); ++i)
		{
			std::vector<double> v_SysErr;
			std::vector<double> v_SysUncer = AnaData.Get(SysUncer_paramList[i]);
			std::vector<double> v_value = Default_AnaData.Get(paramList[i]);

			for (int j = 0; j < v_SysUncer.size(); ++j)
			{
				v_SysErr.push_back( v_SysUncer[j] / 100.0 * v_value[j] );
			}

			AnaData.Add(SysErr_paramList[i], v_SysErr);
		}

		std::vector<TString> List = {"Sigma_TheorySys", "Sigma_ExperiSys", "R_TheorySys", "R_ExperiSys"};
		std::vector<TString> List2 = {"Sigma", "Sigma", "R", "R"};
		//calculate the theoretical error and experimental error
		for (int i = 0; i < List.size(); ++i)
		{
			std::vector<double> v_SysErr;
			std::vector<double> v_SysUncer = AnaData.Get(List[i]+"Uncer");

			std::vector<double> v_value = Default_AnaData.Get(List2[i]);

			for (int j = 0; j < v_SysUncer.size(); ++j)
			{
				v_SysErr.push_back( v_SysUncer[j] / 100.0 * v_value[j] );
			}

			AnaData.Add(List[i]+"Err", v_SysErr);
		}
	}

	void SaveResults(TString outFileDir)
	{
		AnaData.WriteMapFile(outFileDir);
	}

	void Handle() override
	{
		PrintHandling();

		GetPairUncer();
		CalculateSysUncer();
		CalculateSysErr();

		Analyzer::Handle();		
	}


};

void UncerAnalyzer()
{
	using clock = std::chrono::system_clock;
	using sec = std::chrono::duration<double>;
	const auto before = clock::now();
//-------------------------------------TIMER-------------------------------------------------------


	struct UncerAnalyzer UncerAna;
	UncerAna.Handle();
	UncerAna.SaveResults("outFiles/Result_CMS_SysUncer.root");


//-------------------------------------TIMER END---------------------------------------------------
	const sec duration = clock::now() - before;
	std::cout << "UncerAnalyzer:: It took " << duration.count() << "s to run!!!" << std::endl;
}

#endif
