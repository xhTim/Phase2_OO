#ifndef ConversionAnalyzer_H
#define ConversionAnalyzer_H

#include "Analyzer.h"
#include "ParamConverter.C"

struct ConversionAnalyzer : Analyzer
{
    ConversionAnalyzer(AnalysisData& data_) : Analyzer{data_} {};

    void PrintHandling() const
	{
		cout << endl << std::setfill('+') << std::setw(100) << Form("%s::Handle Is Handling The Business... ", typeid(this).name()) << endl;
		cout << setfill(' ');
	}

    void AddRap()
    {
        std::vector<double> temp    = data.Get("Dy");
        int n_Dy = data.GetSize("Dy");
        for (int i = 0; i < n_Dy; i++)
        {
            temp.push_back( -data.Get("Dy", n_Dy - 1 - i) );
        }
        data.Add("Rap", temp);
    }

    void Conversions()
    {
        std::vector<double> W_temp;
        std::vector<double> X_temp;
        std::vector<double> rap_temp = data.Get("Rap");
        for (int i = 0; i < rap_temp.size(); i++)
        {
            W_temp.push_back( ParamConverter::y2W( rap_temp[i]) );
            X_temp.push_back( ParamConverter::y2x( rap_temp[i]) );
        }

        data.Add("W", W_temp);
        data.Add("X", X_temp);
    }

    void Handle() override
    {
        PrintHandling();
        
        AddRap();
        Conversions();

        Analyzer::Handle();
    }
};



#endif