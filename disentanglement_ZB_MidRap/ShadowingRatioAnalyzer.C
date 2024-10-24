#ifndef SHADOWINGRATIOANALYZER_H
#define SHADOWINGRATIOANALYZER_H

#include "Analyzer.h"
#include "ImpulseApprox.C"

// Use to Calculate Shadowing Ratio R
struct ShadowingRatioAnalyzer : Analyzer
{
    bool Simple_R_SysErr = false;

    ShadowingRatioAnalyzer(AnalysisData& data_) : Analyzer{data_} {};

    void Set_Simple_R_SysErr(bool val) { Simple_R_SysErr = val; }

    void PrintHandling() const
    {
        cout << endl << std::setfill('+') << std::setw(100) << Form("%s::Handle Is Handling The Business... ", typeid(this).name()) << endl;
        cout << setfill(' ');
    }

    void Calculate_R()
    {
        int n_data = data.GetSize("Sigma");
        std::vector<double> temp;
        for (int i = 0; i < n_data; ++i)
        {
            temp.push_back( sqrt( data.Get("Sigma", i)/data.Get("Sigma_IA", i) ) );
        }
        data.Add("R", temp);
    }

    void Calculate_R_Err()
    {
        int n_data = data.GetSize("Sigma");
        std::vector<double> temp;
        for (int i = 0; i < n_data; ++i)
        {
            temp.push_back( 0.5 * data.Get("R", i) * data.Get("Sigma_Err", i)/data.Get("Sigma", i) );
        }
        data.Add("R_Err", temp);
    }

    void Calculate_Simple_R_SysErr()
    {
        //Use for Fwd and Mid Rapidity only. It calculate the R systematic error by combining the Sigma systematic error and IA systematic error.
        int n_data = data.GetSize("Sigma");

        //Check if the systematic error is Aymmetric or Symmetric.
        if(data.IsMapKeyExist("Sigma_SysErrLow") && data.IsMapKeyExist("Sigma_SysErrHigh"))
        {
            std::vector<double> tempLow,   tempHigh;
            std::vector<double> TheoryLow, TheoryHigh,   ExperiLow,    ExperiHigh;
            for (int i = 0; i < n_data; ++i)
            {
                tempLow .push_back( 0.5 * data.Get("R", i) * TMath::Hypot( data.Get("Sigma_SysErrLow", i)/data.Get("Sigma", i),
                                                                data.Get("Sigma_IA_Err", i)/data.Get("Sigma_IA", i) ) );
                tempHigh.push_back( 0.5 * data.Get("R", i) * TMath::Hypot( data.Get("Sigma_SysErrHigh", i)/data.Get("Sigma", i),
                                                                data.Get("Sigma_IA_Err", i)/data.Get("Sigma_IA", i) ) );

                ExperiLow.push_back(    0.5 * data.Get("R", i) * data.Get("Sigma_ExperiSysErrLow", i)/data.Get("Sigma", i)    );
                ExperiHigh.push_back(   0.5 * data.Get("R", i) * data.Get("Sigma_ExperiSysErrHigh", i)/data.Get("Sigma", i)   );
                TheoryLow.push_back(    TMath::Sqrt( TMath::Power(tempLow[i], 2)  - TMath::Power(ExperiLow[i], 2) )      );
                TheoryHigh.push_back(   TMath::Sqrt( TMath::Power(tempHigh[i], 2) - TMath::Power(ExperiHigh[i], 2) )    );
            }
            data.Add("R_SysErrLow", tempLow);
            data.Add("R_SysErrHigh", tempHigh);
            data.Add("R_TheorySysErrLow", TheoryLow);
            data.Add("R_TheorySysErrHigh", TheoryHigh);
            data.Add("R_ExperiSysErrLow", ExperiLow);
            data.Add("R_ExperiSysErrHigh", ExperiHigh);
        }
        else
        {
            std::vector<double> temp;
            for (int i = 0; i < n_data; ++i)
            {
                temp.push_back( 0.5 * data.Get("R", i) * TMath::Hypot( data.Get("Sigma_SysErr", i)/data.Get("Sigma", i),
                                                                data.Get("Sigma_IA_Err", i)/data.Get("Sigma_IA", i) ) );
            }
            data.Add("R_SysErr", temp);
        }
    }
    
    void Handle() override
    {
        cout << endl << "+++ShadowingRatioAnalyzer::Handling..." <<endl;

        //Calculate Sigma_IA
        auto IA_pair = ImpulseApprox::GetImpulseApprox(data.Get("W"));
        data.Add("Sigma_IA",    IA_pair.first);
        data.Add("Sigma_IA_Err",IA_pair.second);

        //Calculate R must be first!
        Calculate_R();
        Calculate_R_Err();
        if (Simple_R_SysErr) Calculate_Simple_R_SysErr();

        //Moving to the next Analyzer
        Analyzer::Handle();
    }
};


// void ShadowingRatioAnalyzer()
// {
//     AnalysisData data("Test");
//     AnalysisDataObserver obs;
//     data.Subscribe(&obs);
//     data.Add("Sigma",       {1,2,3});
//     data.Add("Rap",         {1,2,3});
//     data.Add("W",           {10,22,33});
//     data.Add("Sigma_Err",   {0.1,0.2,0.3});
//     struct ShadowingRatioAnalyzer R_Ana(data);
//     R_Ana.Handle();
// }

#endif
