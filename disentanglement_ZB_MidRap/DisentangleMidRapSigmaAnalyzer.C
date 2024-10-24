#ifndef DisentangleMidRapSigmaAnalyzer_H
#define DisentangleMidRapSigmaAnalyzer_H

// Use to disentangle the mid-rapidity sigma from the DSigmaDy
struct DisentangleMidRapSigmaAnalyzer: Analyzer
{
	bool    print               = false;
	double  fluxScaleFactor    	= 1.0;
	class PhotonFluxReader cPhotonFluxReader;

	DisentangleMidRapSigmaAnalyzer(AnalysisData& data_, TString inFileDir_, TString subCase_) : Analyzer{data_}, cPhotonFluxReader{data_, inFileDir_, subCase_} {};

	void PrintHandling() const
	{
		cout << endl << std::setfill('+') << std::setw(100) << Form("%s::Handle Is Handling The Business... ", typeid(this).name()) << endl;
		cout << setfill(' ');
	}

	void Calculate()
	{
		std::vector<double> Sigma, Sigma_Err, Sigma_SysErrLow, Sigma_SysErrHigh, Flux_dNdy_AnAn;
		Flux_dNdy_AnAn  = data.Get("dNdy_AnAn");

		std::cout<<"size: "<<data.GetSize("Dy")<<std::endl;
		for (int i = 0; i < data.GetSize("Dy"); ++i)
		{
			Sigma.push_back(	data.Get("DSigmaDy_AnAn", i) / data.Get("dNdy_AnAn", i) / 2.0	);
			std::cout<<"DsDy: "<<data.Get("DSigmaDy_AnAn", i)<<std::endl;
		}

		for (int i = 0; i < data.GetSize("Dy"); ++i)
		{
			Sigma_Err	.push_back(	Sigma[i] * data.Get("DSigmaDy_AnAn_Err", i) / data.Get("DSigmaDy_AnAn", i)	);
			Sigma_SysErrLow.push_back(	Sigma[i] * TMath::Hypot(	data.Get("DSigmaDy_AnAn_SysErrLow", 	i)	/ data.Get("DSigmaDy_AnAn", i),	
																	data.Get("dNdy_AnAn_Uncer", i) / 100.0	)	);	// flux err from dNdy Uncer, Converting from % to fraction
			Sigma_SysErrHigh.push_back(	Sigma[i] * TMath::Hypot(	data.Get("DSigmaDy_AnAn_SysErrHigh", 	i)	/ data.Get("DSigmaDy_AnAn", i),	
																	data.Get("dNdy_AnAn_Uncer", i) / 100.0	)	);	// flux err from dNdy Uncer, Converting from % to fraction
		}		

		std::vector<double> Sigma_ExperiSysErrLow 	= data.Get("DSigmaDy_AnAn_SysErrLow");
		std::vector<double> Sigma_ExperiSysErrHigh	= data.Get("DSigmaDy_AnAn_SysErrHigh");
		std::vector<double> Sigma_TheorySysErrLow;
		std::vector<double> Sigma_TheorySysErrHigh;
		for (int i = 0; i < Sigma_ExperiSysErrLow.size(); ++i)
		{
			Sigma_ExperiSysErrLow[i] 	*= Sigma[i]/data.Get("DSigmaDy_AnAn", i);
			Sigma_ExperiSysErrHigh[i] 	*= Sigma[i]/data.Get("DSigmaDy_AnAn", i);
			Sigma_TheorySysErrLow .push_back( TMath::Sqrt( TMath::Power(Sigma_SysErrLow[i], 2)  - TMath::Power(Sigma_ExperiSysErrLow[i], 2) ) );
			Sigma_TheorySysErrHigh.push_back( TMath::Sqrt( TMath::Power(Sigma_SysErrHigh[i], 2) - TMath::Power(Sigma_ExperiSysErrHigh[i], 2) ) );
		}

		data.Add("Sigma"        	,Sigma);
		data.Add("Sigma_Err"    	,Sigma_Err);
		data.Add("Sigma_SysErrLow" 	,Sigma_SysErrLow);
		data.Add("Sigma_SysErrHigh" ,Sigma_SysErrHigh);
		data.Add("Sigma_TheorySysErrLow" 	,Sigma_TheorySysErrLow);
		data.Add("Sigma_TheorySysErrHigh"	,Sigma_TheorySysErrHigh);
		data.Add("Sigma_ExperiSysErrLow" 	,Sigma_ExperiSysErrLow);
		data.Add("Sigma_ExperiSysErrHigh"	,Sigma_ExperiSysErrHigh);
	}

	void Handle()	override
	{
		PrintHandling();

		cPhotonFluxReader.SetFluxErr(true);
		cPhotonFluxReader.Handle();
		Calculate();

		Analyzer::Handle();
	}
};

#endif
