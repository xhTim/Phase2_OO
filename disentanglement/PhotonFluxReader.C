#ifndef PhotonFluxReader_H
#define PhotonFluxReader_H

#include "ParamConverter.C"
#include "AnalysisData.C"
#include "../common/function.C"

class PhotonFluxReader
{
protected:
    AnalysisData& data;
    std::map<TString, std::vector<double>> PhotonFluxMap	=   //map template for loading flux
    {
        {"Energy_Table_AnAn"    ,{}}    ,{"Rap_Table_AnAn"      ,{}}    ,{"dNdk_Table_AnAn"     ,{}}    ,{"dNdy_Table_AnAn"     ,{}},
        {"Energy_Table_0n0n"    ,{}}    ,{"Rap_Table_0n0n"      ,{}}    ,{"dNdk_Table_0n0n"     ,{}}    ,{"dNdy_Table_0n0n"     ,{}},
        {"Energy_Table_0nXnSum" ,{}}    ,{"Rap_Table_0nXnSum"   ,{}}    ,{"dNdk_Table_0nXnSum"  ,{}}    ,{"dNdy_Table_0nXnSum"  ,{}},
        {"Energy_Table_XnXn"    ,{}}    ,{"Rap_Table_XnXn"      ,{}}    ,{"dNdk_Table_XnXn"     ,{}}    ,{"dNdy_Table_XnXn"     ,{}},
        {"biter_0n0n"           ,{}}    ,{"PofPhotonB_0n0n"     ,{}}    ,{"PofHadronB_0n0n"     ,{}}    ,{"PofB_0n0n"           ,{}},
        {"biter_0nXnSum"        ,{}}    ,{"PofPhotonB_0nXnSum"  ,{}}    ,{"PofHadronB_0nXnSum"  ,{}}    ,{"PofB_0nXnSum"        ,{}},
        {"biter_XnXn"           ,{}}    ,{"PofPhotonB_XnXn"     ,{}}    ,{"PofHadronB_XnXn"     ,{}}    ,{"PofB_XnXn"           ,{}}
    };
    std::map<TString, std::vector<double>> PhotonFluxMapClean = PhotonFluxMap;   //remember this template and use it to reset the map
    std::map<TString, std::vector<double>> InterPhotonFluxMap;   //use this map to remember the interpolated flux based on the rap
    bool getFluxErr = false;

public:
    TString inFileDir;
    TString subCase;
    std::vector<TString> CasesName = {"AnAn",	"0n0n", "0nXnSum", "XnXn"};

    PhotonFluxReader(AnalysisData& data_, TString inFileDir_, TString subCase_) : data{data_}, inFileDir(inFileDir_), subCase(subCase_) {};

    void LoadPhotonFlux(TString inFileDir_, TString subCase_)
    {
		//Reset the photon flux map to the clean one
        PhotonFluxMap.clear();
        InterPhotonFluxMap.clear();
        PhotonFluxMap = PhotonFluxMapClean;

        for (int i = 0; i < CasesName.size(); ++i)
        {
            TString FluxFileName  = Form("Flux_%s%s.txt", CasesName[i].Data(), subCase_.Data());
            TString PofBFileName  = Form("PofB_%s%s.txt", CasesName[i].Data(), subCase_.Data());
            cout<<"LoadPhotonFlux-------->Loading Photon Flux From " + FluxFileName + " in the Dir:"<<inFileDir_<<endl;

            ifstream myfile(Form("%s%s", inFileDir_.Data(), FluxFileName.Data()));

            if (myfile.is_open())
            {
                std::string line;
                // Read one line at a time into the variable line:
                while(std::getline(myfile, line))
                {
                    std::vector<double>   	lineData;
                    std::stringstream  		lineStream(line);

                    double value;
                    // Read an integer at a time from the line
                    while(lineStream >> value)
                    {
                        // Add the integers from a line to a 1D array (vector)
                        lineData.push_back(value);
                    }
                    // cout<<line<<endl;
                    // When all the integers have been read, add the 1D array
                    PhotonFluxMap.at("Energy_Table_" + CasesName[i]).push_back(lineData[0]);
                    PhotonFluxMap.at("dNdk_Table_" + CasesName[i]) 	.push_back(lineData[1]);
                    PhotonFluxMap.at("dNdy_Table_" + CasesName[i])	.push_back(lineData[2]);

                    PhotonFluxMap.at("Rap_Table_" + CasesName[i])	.push_back( ParamConverter::w2y( lineData[0] )	);
                }
            }
            else throw std::runtime_error( "ERROR!!! Unable to open Flux file!!!");

            if (i == 0) continue;
            ifstream myfile1(Form("%s%s", inFileDir_.Data(), PofBFileName.Data()));
            if (myfile1.is_open())
            {
                std::string line;
                // Read one line at a time into the variable line:
                while(std::getline(myfile1, line))
                {
                    std::vector<double>   	lineData;
                    std::stringstream  		lineStream(line);

                    double value;
                    // Read an integer at a time from the line
                    while(lineStream >> value)
                    {
                        // Add the integers from a line to a 1D array (vector)
                        lineData.push_back(value);
                    }
                    // cout<<line<<endl;
                    // When all the integers have been read, add the 1D array
                    PhotonFluxMap.at("biter_" + CasesName[i])		.push_back(lineData[0]);
                    PhotonFluxMap.at("PofPhotonB_" + CasesName[i]) 	.push_back(lineData[1]);
                    PhotonFluxMap.at("PofHadronB_" + CasesName[i]) 	.push_back(lineData[2]);
                    PhotonFluxMap.at("PofB_" + CasesName[i])  		.push_back(lineData[3]);
                }
            }
            else cout << "ERROR!!! Unable to open PofB file!!!";
        }
        cout<<"LoadPhotonFlux-------->DONE "<<endl<<endl;
    }

    double Interpolate(const double Egamma, TString Case)
    {
        const int nstep = 100;
        const double Emin = 1.20683e-05, Emax = 1460.59; //Emax = 2.627452e+02 for 2.76TeV

        double lnEmin=log(Emin);
        double lnEmax=log(Emax);
        double lEgamma=log(Egamma);
        double dlnE=(lnEmax-lnEmin)/nstep; 

        //       >> Egamma between Ilt and Ilt+1
        int Ilt = int((lEgamma-lnEmin)/dlnE);
        //       >> ln(Egamma) for first point 
        double lnElt = lnEmin + Ilt*dlnE; 
        //       >> Interpolate
        double flux_r = PhotonFluxMap.at("dNdy_Table_" + Case)[Ilt-1] + ((lEgamma-lnElt)/dlnE)*(PhotonFluxMap.at("dNdy_Table_" + Case)[Ilt]- PhotonFluxMap.at("dNdy_Table_" + Case)[Ilt-1]);
        flux_r = flux_r/Egamma;

        return flux_r;
    }


    // calculate the photon flux from the rap given in the data.
    // Will add the flux result to the data if it is not there already, otherwise it will do NOTHING
    // This way the function can be called multiple times without any problem and be used to calculate the flux for uncertainty
    void InterpolateFlux(TString inFileDir_, TString subCase_)
    {
        // if (PhotonFluxMap.at("Energy_Table_AnAn").size() == 0){	loadPhotonFlux(inFileDir, subCase);	}

        LoadPhotonFlux( inFileDir_, subCase_);

        //Calculate photon energy from the rap
        int n_data = data.GetSize("Rap");
        std::vector<double> w_temp;
        for (int j = 0; j < n_data; ++j)
        {
            double w = ParamConverter::y2w( data.Get("Rap", j) );
            w_temp      .push_back( w );
        }

        //Add the photon energy if it is not in the map already
        if ( !data.IsMapKeyExist("w") ) data.Add("w", w_temp);

        //Calculate photon flux from the rap
        for (int i = 0; i < CasesName.size(); ++i)
        {
            TString Case = CasesName[i];
            std::vector<double> dNdy_temp;

            for (int j = 0; j < n_data; ++j)
            {
                double w = data.Get("w", j);
                dNdy_temp   .push_back(	w * Interpolate(w, Case)	);
            }

            //Remember this interpolated flux
            InterPhotonFluxMap["dNdy_" + Case] = dNdy_temp;

            //Add the photon flux if it is not in the map already
            if ( !data.IsMapKeyExist("dNdy_" + Case) )  data.Add("dNdy_" + Case, dNdy_temp);
        }
        
        cout<<"InterpolateFlux-------->DONE"<<endl<<endl;
    }

	std::vector<double> GetFluxUncer(std::vector<double> Default, std::vector<double> Diff)
	{
		std::vector<double> Uncer{};
		for (int i = 0; i < Default.size(); ++i)
		{
			Uncer.push_back( abs( Default[i]-Diff[i] ) / Default[i] * 100.0);
		}
		return Uncer;
	}

    //Calculate the error of photon flux based on the file given by the simulation
    void CalculateFluxErr()
    {
        TString inFileDir_  = "../anaSimu/flux/";
        std::vector<TString> subCaseNucleusList	= {"_SigNN68p3R6p70a0p59", "_SigNN68p3R6p64a0p53", "_SigNN68p3R6p70a0p53", "_SigNN68p3R6p64a0p59"};
        std::vector<TString> subCaseEMDList	= {"_SigNN68p3R6p67a0p56EMD1p055", "_SigNN68p3R6p67a0p56EMD0p945"};
        std::vector<std::vector<double>> dNdy_Errs  (CasesName.size(),    std::vector<double> (data.GetSize("Rap"),   0.0)  );
        std::vector<std::vector<double>> dNdy_Uncers(CasesName.size(),    std::vector<double> (data.GetSize("Rap"),   0.0)  );
        std::vector<std::vector<double>> dNdy_EMD_Errs  (CasesName.size(),    std::vector<double> (data.GetSize("Rap"),   0.0)  );

		//First calculate the error of the flux from the nucleus parmeters
        for (int i = 0; i < subCaseNucleusList.size(); ++i)
        {
            TString subCase_    = subCaseNucleusList[i];

            InterpolateFlux(inFileDir_,  subCase_);

            for (int j = 0; j < CasesName.size(); ++j)
            {
                TString Case = CasesName[j];

                for (int k = 0; k < data.GetSize("Rap"); ++k)
                {
                    auto temp_flux = InterPhotonFluxMap.at("dNdy_" + Case)[k];
                    auto temp_flux_default = data.Get("dNdy_" + Case, k);
                    auto temp_err = abs(temp_flux - temp_flux_default);
                    dNdy_Errs[j][k] = max(dNdy_Errs[j][k],  temp_err);
                }
            }
        }

		//Then calculate the error of the flux from the EMD
		for (int i = 0; i < subCaseEMDList.size(); ++i)
		{
			TString subCase_    = subCaseEMDList[i];

			InterpolateFlux(inFileDir_,  subCase_);

			for (int j = 0; j < CasesName.size(); ++j)
			{
				TString Case = CasesName[j];

				for (int k = 0; k < data.GetSize("Rap"); ++k)
				{
					auto temp_flux = InterPhotonFluxMap.at("dNdy_" + Case)[k];
					auto temp_flux_default = data.Get("dNdy_" + Case, k);
					auto temp_err = abs(temp_flux - temp_flux_default);
					dNdy_EMD_Errs[j][k] = max(dNdy_EMD_Errs[j][k],  temp_err);
				}
			}
		}

		//Calculate the final error of the flux
		for (int i = 0; i < CasesName.size(); ++i)
		{
			TString Case = CasesName[i];
			for (int j = 0; j < dNdy_Errs.size(); ++j)
			{
				dNdy_Errs[i][j] = TMath::Hypot(dNdy_Errs[i][j], dNdy_EMD_Errs[i][j]);
				dNdy_Uncers[i][j] = dNdy_Errs[i][j] / data.Get("dNdy_" + Case, j) * 100.0;
			}
		}

        for (int i = 0; i < CasesName.size(); ++i)
        {
            data.Add("dNdy_" + CasesName[i] + "_Err", dNdy_Errs[i]);
            data.Add("dNdy_" + CasesName[i] + "_Uncer", dNdy_Uncers[i]);
        }
    }

    void SetFluxErr(const bool getFluxErr_ = false)
    {
        getFluxErr = getFluxErr_;
    }

    void Draw( TString Case)
    {	
        TCanvas *c = new TCanvas();
        c->SetLogy();

        TH2D* htem2d = new TH2D("htem2d_"+Case, ";y;dN/dy", 10,-4,4, 10, 1e-2, 1e3);

        TGraph* gr = new TGraph(PhotonFluxMap.at("Rap_Table_"+Case).size(), & PhotonFluxMap.at("Rap_Table_"+Case)[0], & PhotonFluxMap.at("dNdy_Table_"+Case)[0]);
        TGraph* points = new TGraph();

        for (int i = 0; i < data.Get("Rap").size(); ++i)
        {
            points->SetPoint(i,data.Get("Rap")[i],data.Get("dNdy_"+Case)[i]);
        }
        gr->SetLineColor(kBlack);
        gr->SetLineWidth(3);
        points->SetMarkerStyle(kFullCircle);
        points->SetMarkerColor(kRed);

        htem2d->Draw();
        gr->Draw("SAME l");
        points->Draw("SAME P");

        drawLatex(0.3, 0.85, "UPC Pb+Pb #sqrt{s_{NN}} = 5.02 TeV (" + Case +")",      42,       0.05,      1);
        // drawLatex(0.3, 0.80, Form("Emin = %.0e GeV to Emax = %.f GeV (CM frame)", Emin, Emax),      42,       0.04,      1);
        for (int i = 0; i < data.Get("Rap").size(); ++i)
        {
            drawLatex(0.15, 0.35-i*0.04, Form("y = %.2f, E = %.2f GeV, dN/dy = %.3f ",data.Get("Rap")[i],data.Get("w")[i],data.Get("dNdy_"+Case)[i]),      42,       0.04,      1);
        }

        // c->SaveAs( "outFigures/Flux_" + Case + "_dNdy.png" );
        c->SaveAs( "outFigures/Flux_" + Case + "_dNdy.pdf" );
        delete c;
        delete gr;
        delete points;
        delete htem2d;
    }

    void DrawComparison(TString Case,	std::vector<TString> subCases, std::vector<TString> legendName, TString inFileDir_ = "../anaSimu/flux/")
    {
        std::vector<int> colors{1,2,3,4};

        LoadPhotonFlux( inFileDir, subCase);
		auto PhotonFluxMap_Default = PhotonFluxMap;

        TCanvas *c = new TCanvas();
        // c->SetLogy();
        TH2D* htem2d = new TH2D("htem2d_"+Case, "_Uncer;y;dN/dy Uncer. (%)", 10,-4,4, 10, 0, 20);
        htem2d->Draw();
        auto legend = new TLegend(0.2, 0.6, 0.5, 0.8);

        for (int i = 0; i < subCases.size(); ++i)
        {
            auto subCase_ = subCases[i];
            LoadPhotonFlux(inFileDir_, subCase_);
            auto PhotonFluxMap_subCase = PhotonFluxMap;
            PhotonFluxMap = PhotonFluxMapClean;

            auto Uncer_subCase = GetFluxUncer(PhotonFluxMap_Default.at("dNdy_Table_"+Case),	PhotonFluxMap_subCase.at("dNdy_Table_"+Case));
            TGraph* gr_subCase = new TGraph(PhotonFluxMap_subCase.at("Rap_Table_"+Case).size(), PhotonFluxMap_subCase.at("Rap_Table_"+Case).data(), Uncer_subCase.data());
            
            gr_subCase->SetLineColor(colors[i]);
            gr_subCase->SetLineWidth(3);

            gr_subCase->Draw("SAME l");
            legend->AddEntry(gr_subCase, 	legendName[i],	"l");
        }

        drawLatex(0.3, 0.85, "UPC Pb+Pb #sqrt{s_{NN}} = 5.02 TeV (" + Case +")",      42,       0.05,      1);
        legend->Draw();

        // c->SaveAs( "outFigures/Flux_Comparison_" + Case + "_dNdy.png" );
        c->SaveAs( "outFigures/Flux_Comparison_" + Case + "_dNdy.pdf" );
        delete c;
        delete htem2d;
    }

    void Handle()
    {
        cout << endl << "+++PhotonFluxReader::Handling..." <<endl;
        InterpolateFlux(inFileDir, subCase);

        if (getFluxErr) CalculateFluxErr();
    }
};


void PhotonFluxReader()
{
	//------------------Standard Test-------------------------------------------
    AnalysisData data("Test");
    AnalysisDataObserver obs;
    data.Subscribe(&obs);
    data.Add("Rap",         {1.75, -1.75, 2, -2, 2.25, -2.25});
	struct PhotonFluxReader cPhotonFluxReader(data, "../anaSimu/flux/", "_SigNN68p3R6p67a0p56");
    // cPhotonFluxReader.SetFluxErr(true);
    cPhotonFluxReader.Handle();

	// cPhotonFluxReader.Draw("0n0n");
	// cPhotonFluxReader.Draw("0nXnSum");
	// cPhotonFluxReader.Draw("XnXn");
	// cPhotonFluxReader.Draw("AnAn");
	// plotPofB();
// 	//--------------------------------------------------------------------------

	// cPhotonFluxReader.DrawComparison("AnAn",		{"_SigNN68p3R6p64a0p53", "_SigNN68p3R6p70a0p59", "_SigNN68p3R6p64a0p59", "_SigNN68p3R6p70a0p53"},	{"#sigma_{NN} = 68.3, R = 6.64, a = 0.53", "#sigma_{NN} = 68.3, R = 6.70, a = 0.59", "#sigma_{NN} = 68.3, R = 6.64, a = 0.59", "#sigma_{NN} = 68.3, R = 6.70, a = 0.53"});
	// cPhotonFluxReader.DrawComparison("0n0n",		{"_SigNN68p3R6p64a0p53", "_SigNN68p3R6p70a0p59", "_SigNN68p3R6p64a0p59", "_SigNN68p3R6p70a0p53"},	{"#sigma_{NN} = 68.3, R = 6.64, a = 0.53", "#sigma_{NN} = 68.3, R = 6.70, a = 0.59", "#sigma_{NN} = 68.3, R = 6.64, a = 0.59", "#sigma_{NN} = 68.3, R = 6.70, a = 0.53"});
	// cPhotonFluxReader.DrawComparison("0nXnSum",		{"_SigNN68p3R6p64a0p53", "_SigNN68p3R6p70a0p59", "_SigNN68p3R6p64a0p59", "_SigNN68p3R6p70a0p53"},	{"#sigma_{NN} = 68.3, R = 6.64, a = 0.53", "#sigma_{NN} = 68.3, R = 6.70, a = 0.59", "#sigma_{NN} = 68.3, R = 6.64, a = 0.59", "#sigma_{NN} = 68.3, R = 6.70, a = 0.53"});
	// cPhotonFluxReader.DrawComparison("XnXn",		{"_SigNN68p3R6p64a0p53", "_SigNN68p3R6p70a0p59", "_SigNN68p3R6p64a0p59", "_SigNN68p3R6p70a0p53"},	{"#sigma_{NN} = 68.3, R = 6.64, a = 0.53", "#sigma_{NN} = 68.3, R = 6.70, a = 0.59", "#sigma_{NN} = 68.3, R = 6.64, a = 0.59", "#sigma_{NN} = 68.3, R = 6.70, a = 0.53"});

	cPhotonFluxReader.DrawComparison("AnAn",		{"_SigNN68p3R6p67a0p56EMD1p055", "_SigNN68p3R6p67a0p56EMD0p945"},	{"EMD +5.5%", "EMD -5.5%"});
	cPhotonFluxReader.DrawComparison("0n0n",		{"_SigNN68p3R6p67a0p56EMD1p055", "_SigNN68p3R6p67a0p56EMD0p945"},	{"EMD +5.5%", "EMD -5.5%"});
	cPhotonFluxReader.DrawComparison("0nXnSum",		{"_SigNN68p3R6p67a0p56EMD1p055", "_SigNN68p3R6p67a0p56EMD0p945"},	{"EMD +5.5%", "EMD -5.5%"});
	cPhotonFluxReader.DrawComparison("XnXn",		{"_SigNN68p3R6p67a0p56EMD1p055", "_SigNN68p3R6p67a0p56EMD0p945"},	{"EMD +5.5%", "EMD -5.5%"});


// 	// DrawComparison("AnAn",		{"_SigNN68p3R6p67a0p56", "_SigNN68p3R6p64a0p53", "_SigNN68p3R6p70a0p59"},	{"#sigma_{NN} = 68.3, R = 6.67, a = 0.56", "#sigma_{NN} = 68.3, R = 6.64, a = 0.53", "#sigma_{NN} = 68.3, R = 6.70, a = 0.59"});
// 	// DrawComparison("0n0n",		{"_SigNN68p3R6p67a0p56", "_SigNN68p3R6p64a0p53", "_SigNN68p3R6p70a0p59"},	{"#sigma_{NN} = 68.3, R = 6.67, a = 0.56", "#sigma_{NN} = 68.3, R = 6.64, a = 0.53", "#sigma_{NN} = 68.3, R = 6.70, a = 0.59"});
// 	// DrawComparison("0nXnSum",	{"_SigNN68p3R6p67a0p56", "_SigNN68p3R6p64a0p53", "_SigNN68p3R6p70a0p59"},	{"#sigma_{NN} = 68.3, R = 6.67, a = 0.56", "#sigma_{NN} = 68.3, R = 6.64, a = 0.53", "#sigma_{NN} = 68.3, R = 6.70, a = 0.59"});
// 	// DrawComparison("XnXn",		{"_SigNN68p3R6p67a0p56", "_SigNN68p3R6p64a0p53", "_SigNN68p3R6p70a0p59"},	{"#sigma_{NN} = 68.3, R = 6.67, a = 0.56", "#sigma_{NN} = 68.3, R = 6.64, a = 0.53", "#sigma_{NN} = 68.3, R = 6.70, a = 0.59"});
// 	//--------------------------------------------------------------------------
}

#endif
