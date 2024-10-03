#ifndef PDFFACTORY_H
#define PDFFACTORY_H

/*
	PDF Factory Here
*/

class PdfFactory
{
public:

	PdfFactory(TH1D * Hist) : Hist{Hist} {}
	virtual ~PdfFactory() = default;

	// virtual void 			Fit() 		= 0;
	virtual RooGenericPdf * GetPdf()	= 0; 

protected:
	TH1D 			* 	Hist 	= nullptr;
	RooGenericPdf	*	Pdf 	= nullptr;
};


#endif