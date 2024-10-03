#ifndef MatrixIO_H
#define MatrixIO_H


struct MatrixIO
{
	// Method to read a matrix from a text file
	static TMatrixD Read(const TString filename)
	{
		std::ifstream infile(filename.Data());
		if (infile.is_open())
		{
			// Read the dimensions of the matrix from the first line of the file
			int nrows, ncols;
			infile >> nrows >> ncols;

			// Create a new matrix with the given dimensions
			TMatrixD result = TMatrixD(nrows, ncols);

			// Read the elements of the matrix from the file
			for (int i = 0; i < nrows; ++i)
			{
				for (int j = 0; j < ncols; ++j)
				{
					double d;
					infile >> d;
					result(i, j) = d;
				}
			}

			infile.close();
			return result;
		}
		else
		{
			throw std::runtime_error( "Error: could not open file " + filename + " for reading.");
		}
	}

	// Method to write the matrix to a text file
	static void Write(const TMatrixD matrix, const TString filename)
	{
		std::ofstream outfile(filename.Data());
		if (outfile.is_open())
		{
			outfile << matrix.GetNrows() << " " << matrix.GetNcols() << std::endl;
			for (int i = 0; i < matrix.GetNrows(); ++i)
			{
				for (int j = 0; j < matrix.GetNcols(); ++j)
				{
					outfile << matrix(i, j) << " ";
				}
				outfile << std::endl;
			}
			outfile.close();
		} 
		else
		{
			throw std::runtime_error( "Error: could not open file " + filename + " for writing.");
		}
		cout << "Matrix is written to " << filename << endl;
	}
};

#endif

// void MatrixIO()
// {
// 	TMatrixD matrix = MatrixIO::Read("../disentanglement/outFiles/CovMatrix.txt");
// 	matrix.Print();
// 	MatrixIO::Write(matrix, "matrix_copy.txt");
// }