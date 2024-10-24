// #include "uncertainties/uncertainties/impl.hpp"
// #include "uncertainties/uncertainties/io.hpp"
// #include "uncertainties/uncertainties/ureal.hpp"
// #include "uncertainties/uncertainties/math.hpp"

// template<typename T> 
// struct Observer
// {
// 	virtual void    FieldChanged(const T& source, TString field_name)   const = 0;
// 	virtual         ~Observer	()		= default;
// };

// template <typename T> 
// struct Observable
// {
// 	void Notify	(const T& source, TString name)    const
// 	{
// 		for (auto obs : observers)
// 			obs->FieldChanged(source, name);
// 	}
// 	void Subscribe	(Observer< T >* obs) { observers.push_back(obs); }
// 	void Unsubscribe(Observer< T >* obs)
// 	{
// 		auto remove_status = remove(observers.begin(), observers.end(), obs);
// 		cout<< "The Observer Unsubscribed..." <<endl;
// 	}
// // private:
// 	std::vector< Observer< T > * > observers;
// };

class AnalysisData
{
public:
    std::string data_name;
    AnalysisData(std::string name): data_name{name} {}
    // AnalysisData(const AnalysisData &other) : data_name(other.data_name) {}
    // AnalysisData& operator=(const AnalysisData &other)
    // {
    //     if (this != &other)
    //     {
    //         data_name = other.data_name;
    //     }
    //     return *this;
    // }
};

void Test()
{

    std::map<std::string, AnalysisData> data_map;

    AnalysisData data("data1");
    AnalysisData data2("data2");
    data2 = data;
    data_map["data1"] = nullptr;

}