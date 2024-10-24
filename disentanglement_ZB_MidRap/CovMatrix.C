#include "../common_ZB/MatrixIO.C"
#include "../common_ZB/SortByKeys.C"
#include "UncerAnalyzer.C"

class CalculateCovMatrix
{
private:
	std::vector<double> sort_keys;
public:
	TMatrixD cov_matrix;
	TMatrixD corr_matrix;

	AnalysisData *AnaData = nullptr;
	TString name, param;
	bool isSigned = false;	// Fully correlated uncertainty use unsigned covariance

	// Constructor 
	CalculateCovMatrix(const TString name_, AnalysisData *AnaData_, const TString param_, const std::vector<double> sort_keys_) : name{name_}, AnaData{AnaData_}, param{param_}, sort_keys{sort_keys_} {Init();}
	CalculateCovMatrix(const TString name_, AnalysisData *AnaData_, const TString param_, const std::vector<double> sort_keys_, bool isSigned_) : name{name_}, AnaData{AnaData_}, param{param_}, sort_keys{sort_keys_},isSigned{isSigned_} {Init();}

	void Init()
	{
		int nrows = AnaData->GetSize(param);
		int ncols = AnaData->GetSize(param);

		cov_matrix.ResizeTo(nrows, ncols);
		corr_matrix.ResizeTo(nrows, ncols);

		cov_matrix = MatrixFactory(nrows, ncols, 1);
		corr_matrix = MatrixFactory(nrows, ncols, 1);
	}

	void Print()	const
	{
		cov_matrix.Print();
		corr_matrix.Print();
	}

	void CalculateCov(std::vector<double> Norm_v)
	{
		if (AnaData == nullptr)		std::runtime_error("CalculateCovMatrix --> AnaData is not set!");
		if (sort_keys.empty())		std::runtime_error("CalculateCovMatrix --> sort_keys is not set!");

		std::vector<double> vec = AnaData->Get(param);
		if (isSigned) vec = AnaData->Get(param + "_Signed");

		//sort the vector based on the sort_keys
		vec = SortByKeys(vec, sort_keys);
		auto sorted_Norm_v = SortByKeys(Norm_v, sort_keys);

		for (int i = 0; i < cov_matrix.GetNrows(); ++i)
		{
			for (int j = 0; j < cov_matrix.GetNcols(); ++j)
			{
				double cov = (vec[i]/100.00)*sorted_Norm_v[i] * (vec[j]/100.00) * sorted_Norm_v[j];
				cov_matrix(i, j) = cov;
			}
		}
		cout << "Covariance matrix from " << name << " is calculated for " << param << endl;
		// Print();

		//update the correlation matrix
		corr_matrix = CovToCorr(cov_matrix);
	}

	static TMatrixD CombineStatCovMatrix(const int nrows, const int ncols)
	{
		TMatrixD cov_1p75 = MatrixIO::Read("outFiles/CovMatrix_Fit2D_Rap1.75.txt");
		TMatrixD cov_2p00 = MatrixIO::Read("outFiles/CovMatrix_Fit2D_Rap2.00.txt");
		TMatrixD cov_2p25 = MatrixIO::Read("outFiles/CovMatrix_Fit2D_Rap2.25.txt");

		std::vector<TMatrixD> cov_ = {cov_2p25, cov_2p00, cov_1p75};
		TMatrixD cov_combined = CalculateCovMatrix::MatrixFactory(nrows, ncols, 0.0);

		// the covariance matrix from text file is 2x2 matrix, the first row is high W and the second row is low W
		// we need to reverse the order of the rows and columns and combine them into a 6x6 matrix
		for (int i = 0; i < cov_.size(); ++i)
		{
			cov_combined(i, 		i			) = cov_[i](1,1);
			cov_combined(i, 		ncols-1-i	) = cov_[i](0,1);
			cov_combined(nrows-1-i, i			) = cov_[i](1,0);
			cov_combined(nrows-1-i, ncols-1-i	) = cov_[i](0,0);
		}
		// cov_combined.Print();

		return cov_combined;
	}

	static TMatrixD CovToCorr(const TMatrixD& cov)
	{
		int nrows = cov.GetNrows();
		int ncols = cov.GetNcols();
		TMatrixD corr = CalculateCovMatrix::MatrixFactory(nrows, ncols, 0.0);

		for (int i = 0; i < nrows; i++)
		{
			for (int j = 0; j < ncols; j++)
			{
				corr(i,j) = cov(i,j) / (TMath::Sqrt(cov(i,i)) * TMath::Sqrt(cov(j,j)));
			}
		}

		// corr.Print();

		return corr;
	}

	//create a matrix with all elements equal to value
	static TMatrixD MatrixFactory(const int nrows, const int ncols, const int value)
	{
		TMatrixD matrix(nrows, ncols);
		for (int i = 0; i < nrows; ++i)
		{
			for (int j = 0; j < ncols; ++j)
			{
				matrix(i, j) = value;
			}
		}
		return matrix;
	}


	static void DrawCov(const TMatrixD &matrix, TString outfile = "outFigures/CovMatrix.pdf")
	{
		TCanvas* c = new TCanvas("c", "c", 1000, 1000);
		c->SetRightMargin(0.17);

		auto name = outfile;
		name.Remove(0, 11);
		name.Remove(name.Length()-4, 4);
		gStyle->SetOptTitle(1);

		TH2D* hist = new TH2D("hist", name + "; W bin number; W bin number;", matrix.GetNrows(), 0, matrix.GetNrows(), matrix.GetNcols(), 0, matrix.GetNcols());
		for (int i = 0; i < matrix.GetNrows(); ++i)
		{
			for (int j = 0; j < matrix.GetNcols(); ++j)
			{
				hist->SetBinContent((i+1), (j+1), matrix[i][j]);
			}
		}
		hist->Draw("colz");

		c->SaveAs(outfile);
		
		delete c;
		delete hist;
	}	

	static void DrawCorr(const TMatrixD &matrix, TString outfile = "outFigures/CorrMatrix.pdf")
	{
		TCanvas* c = new TCanvas("c", "c", 1000, 1000);
		c->SetRightMargin(0.17);

		auto name = outfile;
		name.Remove(0, 11);
		name.Remove(name.Length()-4, 4);
		gStyle->SetOptTitle(1);

		TH2D* hist = new TH2D("hist", name + "; W bin number; W bin number;", matrix.GetNrows(), 0, matrix.GetNrows(), matrix.GetNcols(), 0, matrix.GetNcols());
		for (int i = 0; i < matrix.GetNrows(); ++i)
		{
			for (int j = 0; j < matrix.GetNcols(); ++j)
			{
				hist->SetBinContent((i+1), (j+1), matrix[i][j]);
			}
		}
		hist->GetZaxis()->SetRangeUser(-1.00001, 1.00001);
		hist->Draw("colz");
		c->SaveAs(outfile);

		delete c;
		delete hist;
	}
};

void CovMatrix()
{
	std::vector<TString> constCorrUncer = {"Lumi", "BR"};
	std::vector<TString> fullCorrUncer = {"Signal Ext", "HFveto", "TnP"};
	std::vector<TString> partCorrUncer = {"PhotonFlux", "n-PileUp"};

	std::vector<CalculateCovMatrix> fullCovMatrices;
	std::vector<CalculateCovMatrix> partCovMatrices;

	struct UncerAnalyzer UncerAna;
	UncerAna.Handle();

	const std::vector<double> sort_keys = UncerAna.Default_AnaData.Get("W");

	//calculate the covariance matrix for each uncertainty
	//constCorrUncer is in the vector of fullly correlated matrix
	for (auto& name : constCorrUncer)
	{
		class CalculateCovMatrix cov(name, &UncerAna.AnaData_Breakdown.at(name), name + "_SysUncer", sort_keys);
		cov.CalculateCov(UncerAna.Default_AnaData.Get("Sigma"));
		fullCovMatrices.push_back(cov);
	}

	for (auto& name : fullCorrUncer)
	{
		class CalculateCovMatrix cov(name, &UncerAna.AnaData_Breakdown.at(name), "Sigma_SysUncer", sort_keys);
		cov.CalculateCov(UncerAna.Default_AnaData.Get("Sigma"));
		fullCovMatrices.push_back(cov);
	}

	for (auto& name : partCorrUncer)
	{
		class CalculateCovMatrix cov(name, &UncerAna.AnaData_Breakdown.at(name), "Sigma_SysUncer", sort_keys, true);
		cov.CalculateCov(UncerAna.Default_AnaData.Get("Sigma"));
		partCovMatrices.push_back(cov);
	}

	std::vector<CalculateCovMatrix> CovMatrices = fullCovMatrices;
	CovMatrices.insert(CovMatrices.end(), partCovMatrices.begin(), partCovMatrices.end());

	//sum the covariance matrix
	TMatrixD cov_total = CalculateCovMatrix::MatrixFactory(CovMatrices[0].cov_matrix.GetNrows(), CovMatrices[0].cov_matrix.GetNcols(), 0.0);
	for (auto& cov : CovMatrices)
	{
		cov_total += cov.cov_matrix;
	}

	auto cov_stat = CalculateCovMatrix::CombineStatCovMatrix(cov_total.GetNrows(), cov_total.GetNcols());
	cov_total.Print();
	cov_total += cov_stat;
	cov_stat.Print();
	cov_total.Print();

	auto corr_total = CalculateCovMatrix::CovToCorr(cov_total);

	auto cov_lumi = fullCovMatrices[0].cov_matrix;
	auto corr_lumi = CalculateCovMatrix::CovToCorr(cov_lumi);
	auto cov_BR = fullCovMatrices[1].cov_matrix;
	auto corr_BR = CalculateCovMatrix::CovToCorr(cov_BR);

	auto cov_sigEx = fullCovMatrices[2].cov_matrix;
	auto corr_sigEx = CalculateCovMatrix::CovToCorr(cov_sigEx);
	auto cov_HFveto = fullCovMatrices[3].cov_matrix;
	auto corr_HFveto = CalculateCovMatrix::CovToCorr(cov_HFveto);
	auto cov_TnP = fullCovMatrices[4].cov_matrix;
	auto corr_TnP = CalculateCovMatrix::CovToCorr(cov_TnP);

	auto cov_flux = partCovMatrices[0].cov_matrix;
	auto corr_flux = CalculateCovMatrix::CovToCorr(cov_flux);
	auto cov_neutron = partCovMatrices[1].cov_matrix;
	auto corr_neutron = CalculateCovMatrix::CovToCorr(cov_neutron);

	auto corr_stat = CalculateCovMatrix::CovToCorr(cov_stat);

	auto cov_Experi = cov_stat + cov_lumi + cov_BR + cov_sigEx + cov_HFveto + cov_TnP + cov_neutron;
	auto corr_Experi = CalculateCovMatrix::CovToCorr(cov_Experi);

	CalculateCovMatrix::DrawCov(cov_BR,			"outFigures/CovMatrix_BR.pdf");
	CalculateCovMatrix::DrawCorr(corr_BR,		"outFigures/CorrMatrix_BR.pdf");
	CalculateCovMatrix::DrawCov(cov_lumi,		"outFigures/CovMatrix_Lumi.pdf");
	CalculateCovMatrix::DrawCorr(corr_lumi,		"outFigures/CorrMatrix_Lumi.pdf");

	CalculateCovMatrix::DrawCov(cov_sigEx,		"outFigures/CovMatrix_SigEx.pdf");
	CalculateCovMatrix::DrawCorr(corr_sigEx,	"outFigures/CorrMatrix_SigEx.pdf");
	CalculateCovMatrix::DrawCov(cov_HFveto,		"outFigures/CovMatrix_HFveto.pdf");
	CalculateCovMatrix::DrawCorr(corr_HFveto,	"outFigures/CorrMatrix_HFveto.pdf");
	CalculateCovMatrix::DrawCov(cov_TnP,		"outFigures/CovMatrix_TnP.pdf");
	CalculateCovMatrix::DrawCorr(corr_TnP,		"outFigures/CorrMatrix_TnP.pdf");

	CalculateCovMatrix::DrawCov(cov_flux,		"outFigures/CovMatrix_Flux.pdf");
	CalculateCovMatrix::DrawCorr(corr_flux,		"outFigures/CorrMatrix_Flux.pdf");
	CalculateCovMatrix::DrawCov(cov_neutron,	"outFigures/CovMatrix_Neutron.pdf");
	CalculateCovMatrix::DrawCorr(corr_neutron,	"outFigures/CorrMatrix_Neutron.pdf");

	CalculateCovMatrix::DrawCov(cov_stat,		"outFigures/CovMatrix_Stat.pdf");
	CalculateCovMatrix::DrawCorr(corr_stat,		"outFigures/CorrMatrix_Stat.pdf");

	CalculateCovMatrix::DrawCov(cov_total,		"outFigures/CovMatrix_Total.pdf");
	CalculateCovMatrix::DrawCorr(corr_total,	"outFigures/CorrMatrix_Total.pdf");


	MatrixIO::Write(cov_total, "outFiles/CovMatrix.txt");
	MatrixIO::Write(cov_Experi, "outFiles/CovMatrixExperi.txt");
	MatrixIO::Write(cov_flux, "outFiles/CovMatrixFlux.txt");
}