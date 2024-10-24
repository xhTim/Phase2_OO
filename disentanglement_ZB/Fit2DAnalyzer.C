#ifndef Fit2DAnalyzer_H
#define Fit2DAnalyzer_H

#include "PhotonFluxReader.C"
#include "../common_ZB/function.C"
#include "../common_ZB/MatrixIO.C"

//Fit Sigma from 2D data point of DSigmaDy and dNdy
struct Fit2DAnalyzer: Analyzer
{
    bool    print               = false;
    bool    saveCovMatrix       = false;
    double  fluxScaleFactor    = 1.0;
    class PhotonFluxReader cPhotonFluxReader;

    Fit2DAnalyzer(AnalysisData& data_, TString inFileDir_, TString subCase_) : Analyzer{data_}, cPhotonFluxReader{data_, inFileDir_, subCase_} {};

    void Fit2D()
    {
        std::vector<double> Sigma, Sigma_Err;
        std::vector<std::vector<double>> X_RapBin, Y_RapBin, Z_RapBin, Z_Err_RapBin;
        std::vector< std::pair<double, double> > fluxCoefficients_0n0n, fluxCoefficients_0nXnSum, fluxCoefficients_XnXn;

        int n_data = data.GetSize("Rap");
        for (int i = 0; i < n_data/2 ; i++)
        {
            fluxCoefficients_0n0n       .push_back( {data.Get("dNdy_0n0n"       ,i )    ,data.Get("dNdy_0n0n"       ,n_data - 1 - i )} );
            fluxCoefficients_0nXnSum    .push_back( {data.Get("dNdy_0nXnSum"    ,i )    ,data.Get("dNdy_0nXnSum"    ,n_data - 1 - i )} );
            fluxCoefficients_XnXn       .push_back( {data.Get("dNdy_XnXn"       ,i )    ,data.Get("dNdy_XnXn"       ,n_data - 1 - i )} );
        }

        //Start filling points
        for (int i = 0; i < fluxCoefficients_0n0n.size(); ++i)
        {
            std::vector<double> X, Y, Z, Z_Err;
            X.push_back(fluxCoefficients_0n0n[i].first  *fluxScaleFactor);   X.push_back(fluxCoefficients_0nXnSum[i].first   *fluxScaleFactor);   X.push_back(fluxCoefficients_XnXn[i].first  *fluxScaleFactor);
            Y.push_back(fluxCoefficients_0n0n[i].second *fluxScaleFactor);   Y.push_back(fluxCoefficients_0nXnSum[i].second  *fluxScaleFactor);   Y.push_back(fluxCoefficients_XnXn[i].second *fluxScaleFactor);
            Z.push_back(data.Get("DSigmaDy_0n0n", i));                  Z.push_back(data.Get("DSigmaDy_0nXnSum", i));                   Z.push_back(data.Get("DSigmaDy_XnXn", i));
            Z_Err.push_back(data.Get("DSigmaDy_0n0n_Err", i));          Z_Err.push_back(data.Get("DSigmaDy_0nXnSum_Err", i));           Z_Err.push_back(data.Get("DSigmaDy_XnXn_Err", i));
            X_RapBin.push_back(X);
            Y_RapBin.push_back(Y);
            Z_RapBin.push_back(Z);
            Z_Err_RapBin.push_back(Z_Err);
        }

        //Start fitting
        for (int i = 0; i < X_RapBin.size(); ++i)
        {
            cout<<"fit2D: Proceessing ------------------>Rap: "<<data.Get("Dy", i)<<" <------------------------------- "<<endl;
            auto c = new TCanvas();

            auto frame3D = new TH3D("", "TGraph2D TF2 Fit; dN_{1}/dy; dN_{2}/dy; d#sigma/dy", 10, 0, 20, 10, 0, 160,10,0,3);
            // frame3D->GetYaxis()->CenterTitle();
            // frame3D->GetXaxis()->CenterTitle();
            // frame3D->GetYaxis()->SetTitleSize(0.05);
            // frame3D->GetYaxis()->SetTitleSize(0.05);
            // frame3D->GetYaxis()->SetTitleOffset(0.99);
            // frame3D->GetYaxis()->SetLabelSize(0.04);
            // frame3D->GetXaxis()->SetTitleSize(0.05);
            // frame3D->GetXaxis()->SetTitleOffset(0.98);
            // frame3D->GetXaxis()->SetLabelSize(0.04);
            // frame3D->SetTickLength(0.04);
            frame3D->Draw("iso");

            TGraph2DErrors* gr = new TGraph2DErrors(X_RapBin[i].size(), X_RapBin[i].data(), Y_RapBin[i].data(), Z_RapBin[i].data(),0,0, Z_Err_RapBin[i].data());
            TF2 * f = new TF2("func","[0] * x + [1] * y");
            f->SetParameters(0.001,0.5);

            auto fitResults = gr->Fit(f,  "S");
            // fitResults->Print();

            if (saveCovMatrix)
            {
                auto Cov_Fit2D    = fitResults->GetCovarianceMatrix();
                MatrixIO::Write(Cov_Fit2D, Form("outFiles/CovMatrix_Fit2D_Rap%.2f.txt", data.Get("Rap",i)));
            }

            // gr->SetTitle("TGraph2D TF2 Fit; dN_{1}/dy; dN_{2}/dy; d#sigma/dy");
            gr->SetMarkerColor(kBlue);
            gr->SetMarkerSize(1.5);
            gr->SetLineWidth(3);
            gr->Draw("same p err");
            // f->SetMarkerColor(kBlue);
            f->Draw("same surf");
            drawLatex(0.1, 0.95, Form("(#sigma(y = %.2f), #sigma(y = %.2f)) = (%.4f #pm %.4f, %.4f #pm %.4f)", data.Get("Dy", i), -data.Get("Dy", i),
                                    f->GetParameter(0), f->GetParError(0), f->GetParameter(1), f->GetParError(1)),      42,       0.04,      1);
            drawLatex(0.1, 0.90, Form("#chi^{2}/ndf: %.2f/1", fitResults->Chi2()),      42,       0.04,      1);

            // c->SaveAs(Form("outFigures/fit2D_%d.pdf",i));
            cout<<endl;
            
            //insert results in the middle of the vector
            vector<double> Sigma_temp = {f->GetParameter(0),  f->GetParameter(1)};
            vector<double> Sigma_Err_temp = {f->GetParError(0),  f->GetParError(1)};
            Sigma      .insert(Sigma.begin() + Sigma.size() / 2 , Sigma_temp.begin(), Sigma_temp.end());
            Sigma_Err  .insert(Sigma_Err.begin() + Sigma_Err.size() / 2 , Sigma_Err_temp.begin(), Sigma_Err_temp.end());

            delete gr; delete c; delete f;delete frame3D;
        }

        data.Add("Sigma"        ,Sigma);
        data.Add("Sigma_Err"    ,Sigma_Err);
    }

    void Handle()
    {
        cout << endl << "+++Fit2DAnalyzer::Handling..." <<endl;
        cPhotonFluxReader.Handle();
        Fit2D();

        Analyzer::Handle();
    }
};

// void Fit2DAnalyzer()
// {
//     AnalysisData data("Test");
//     AnalysisDataObserver obs;
//     data.Subscribe(&obs);
//     data.Add("Dy",                     {-1.7000, -1.8000, -1.9000});
//     data.Add("DSigmaDy_0n0n",           {2.6450, 2.5837, 2.5214});   data.Add("DSigmaDy_0n0n_Err",           {0.0001,0.0001,0.0001});
//     data.Add("DSigmaDy_0nXnSum",           {.7501,.7296,.7069});   data.Add("DSigmaDy_0nXnSum_Err",           {0.0001,0.0001,0.0001});
//     data.Add("DSigmaDy_XnXn",           {.2722,.2682,.2634});      data.Add("DSigmaDy_XnXn_Err",           {0.0001,0.0001,0.0001});
// 	struct Fit2DAnalyzer ana(data, "../anaSimu/flux/", "_SigNN68p3R6p67a0p56");
//     ana.Handle();
//     //---------------------------------Manual Test Map------------------------------------------
// 	//WS
// 	// std::map<TString, std::vector<double>> ShadowRatio_ParamsMap = {
// 	// 	{"Raps",		{}},	{"Raps_Err",		{}},
// 	// 	{"Sigma",		{}},	{"Sigma_Err",		{}},
// 	// 	{"Sigma_IA",	{}},	{"Sigma_IA_Err",	{}},
// 	// 	{"R",			{}},	{"R_Err",			{}},
// 	// 	{"Xs",			{}},	{"Xs_Err",			{}},
// 	// 	{"Ws",			{}},	{"Ws_Err",			{}},
// 	// 	{"Rap",			{-1.7000, -1.8000, -1.9000, -2.0000, -2.1000, -2.2000, -2.3000, -2.4000}},	{"RapErr",				{0,0,0,0,0,0,0,0}},
// 	// 	{"Xsec_AnAn",	{3.6673,3.5814,3.4918,3.4005,3.2986,3.2020,3.0970,2.9921}},	{"XsecErr_AnAn",		{0.0001,0.0001,0.0001,0.0001,0.0001,0.0001,0.0001,0.0001}},
// 	// 	{"Xsec_0n0n",	{2.6450, 2.5837, 2.5214, 2.4603, 2.3931, 2.3332, 2.2687, 2.2067}},	{"XsecErr_0n0n",		{0.0001,0.0001,0.0001,0.0001,0.0001,0.0001,0.0001,0.0001}},
// 	// 	{"Xsec_0nXnSum",{.7501,.7296,.7069,.6823,.6546,.6256,.5940,.5610}},	{"XsecErr_0nXnSum",		{0.0001,0.0001,0.0001,0.0001,0.0001,0.0001,0.0001,0.0001}},
// 	// 	{"Xsec_XnXn",	{.2722,.2682,.2634,.2578,.2509,.2433,.2343,.2244}},	{"XsecErr_XnXn",		{0.0001,0.0001,0.0001,0.0001,0.0001,0.0001,0.0001,0.0001}},
// 	// };
// 	// std::map<TString, std::vector<double>> tem = {
// 	// 	{"Rap",			{1.7000, 1.8000, 1.9000, 2.0000, 2.1000, 2.2000, 2.3000, 2.4000}},	{"RapErr",				{0,0,0,0,0,0,0,0}},
// 	// 	{"Xsec_AnAn",	{3.6673,3.5814,3.4918,3.4005,3.2986,3.2020,3.0970,2.9921}},	{"XsecErr_AnAn",		{0.0001,0.0001,0.0001,0.0001,0.0001,0.0001,0.0001,0.0001}},
// 	// 	{"Xsec_0n0n",	{2.6450, 2.5837, 2.5214, 2.4603, 2.3931, 2.3332, 2.2687, 2.2067}},	{"XsecErr_0n0n",		{0.0001,0.0001,0.0001,0.0001,0.0001,0.0001,0.0001,0.0001}},
// 	// 	{"Xsec_0nXnSum",{.7501,.7296,.7069,.6823,.6546,.6256,.5940,.5610}},	{"XsecErr_0nXnSum",		{0.0001,0.0001,0.0001,0.0001,0.0001,0.0001,0.0001,0.0001}},
// 	// 	{"Xsec_XnXn",	{.2722,.2682,.2634,.2578,.2509,.2433,.2343,.2244}},	{"XsecErr_XnXn",		{0.0001,0.0001,0.0001,0.0001,0.0001,0.0001,0.0001,0.0001}},
// 	// };

// 	//SS
// 	// std::map<TString, std::vector<double>> ShadowRatio_ParamsMap = {
// 	// 	{"Raps",		{}},	{"Raps_Err",		{}},
// 	// 	{"Sigma",		{}},	{"Sigma_Err",		{}},
// 	// 	{"Sigma_IA",	{}},	{"Sigma_IA_Err",	{}},
// 	// 	{"R",			{}},	{"R_Err",			{}},
// 	// 	{"Xs",			{}},	{"Xs_Err",			{}},
// 	// 	{"Ws",			{}},	{"Ws_Err",			{}},
// 	// 	{"Rap",			{-1.7000, -1.8000, -1.9000, -2.0000, -2.1000, -2.2000, -2.3000, -2.4000  }},	{"RapErr",				{0.,0.,0,0,0,0,0,0}},
// 	// 	{"Xsec_AnAn",	{2.5587,  2.5201,  2.4798,  2.4395,  2.3908,  2.3473,  2.2968,  2.2466}},	{"XsecErr_AnAn",		{0.0001,0.0001,0.0001,0.0001,0.0001,0.0001,0.0001,0.0001}},
// 	// 	{"Xsec_0n0n",	{1.9093,	1.8860,	1.8621,	1.8395,	1.8112,	1.7887,	1.7614,	1.7356}},	{"XsecErr_0n0n",		{0.0001,0.0001,0.0001,0.0001,0.0001,0.0001,0.0001,0.0001}},
// 	// 	{"Xsec_0nXnSum",{.4788,.4662,.4528,.4386,.4224,.4059,.3879,.3692}},	{"XsecErr_0nXnSum",		{0.0001,0.0001,0.0001,0.0001,0.0001,0.0001,0.0001,0.0001}},
// 	// 	{"Xsec_XnXn",	{.1706,.1679,.1648,.1614,.1572,.1527,.1475,.1418}},	{"XsecErr_XnXn",		{0.0001,0.0001,0.0001,0.0001,0.0001,0.0001,0.0001,0.0001}},
// 	// };

// 	// std::map<TString, std::vector<double>> tem = {
// 	// 	{"Rap",			{1.7000, 1.8000, 1.9000, 2.0000, 2.1000, 2.2000, 2.3000, 2.4000  }},	{"RapErr",				{0.,0.,0,0,0,0,0,0}},
// 	// 	{"Xsec_AnAn",	{2.5587,  2.5201,  2.4798,  2.4395,  2.3908,  2.3473,  2.2968,  2.2466}},	{"XsecErr_AnAn",		{0.0001,0.0001,0.0001,0.0001,0.0001,0.0001,0.0001,0.0001}},
// 	// 	{"Xsec_0n0n",	{1.9093,	1.8860,	1.8621,	1.8395,	1.8112,	1.7887,	1.7614,	1.7356}},	{"XsecErr_0n0n",		{0.0001,0.0001,0.0001,0.0001,0.0001,0.0001,0.0001,0.0001}},
// 	// 	{"Xsec_0nXnSum",{.4788,.4662,.4528,.4386,.4224,.4059,.3879,.3692}},	{"XsecErr_0nXnSum",		{0.0001,0.0001,0.0001,0.0001,0.0001,0.0001,0.0001,0.0001}},
// 	// 	{"Xsec_XnXn",	{.1706,.1679,.1648,.1614,.1572,.1527,.1475,.1418}},	{"XsecErr_XnXn",		{0.0001,0.0001,0.0001,0.0001,0.0001,0.0001,0.0001,0.0001}},
// 	// };
// 	//-------------------------------------------------------------------------------------
// }

#endif