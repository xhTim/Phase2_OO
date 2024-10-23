void globleStyle()
{
	//============================================================
	// 
	//          Make graphs pretty
	// 
	//============================================================
	cout<<"gStyle mode requested !"<<endl;

	TGaxis::SetMaxDigits(3);
	//TGaxis::SetExponentOffset(0.06,-0.015,"X"); // move the scientific term around axis
	//TGaxis::SetExponentOffset(-0.06,0.015,"Y"); // move the scientific term around axis

	//int font = 22;
	int font = 42;

	gStyle->SetOptFit(1111);
	gStyle->SetOptTitle(0);
	gStyle->SetOptDate(0);
	gStyle->SetOptStat(0);
	gStyle->SetStatFont(font);
	gStyle->SetStatColor(10);
	gStyle->SetStatH(0.18);
	gStyle->SetStatW(0.18);
	gStyle->SetPalette(1,0);
	gStyle->SetTextFont(font);
	gStyle->SetTextSize(0.05);
	gStyle->SetDrawBorder(0);

	// set of X error bars and bar caps
	//gStyle->SetErrorX(0);
	gStyle->SetEndErrorSize(4);

	gStyle->SetCanvasDefH(600);
	gStyle->SetCanvasDefW(800);
	gStyle->SetCanvasColor(10);
	gStyle->SetCanvasBorderMode(0);
	gStyle->SetCanvasBorderSize(2);
	gStyle->SetPadColor(10);
	gStyle->SetPadBorderMode(0);
	gStyle->SetPadBorderSize(0);
	gStyle->SetPadLeftMargin(0.12);
	gStyle->SetPadRightMargin(0.1);
	gStyle->SetPadTopMargin(0.08);
	gStyle->SetPadBottomMargin(0.12);
	gStyle->SetPadTickX(1);
	gStyle->SetPadTickY(1);
	gStyle->SetPadGridX(0);
	gStyle->SetPadGridY(0);
	gStyle->SetGridColor(18);
	gStyle->SetFrameFillStyle(4000);
	gStyle->SetFrameFillColor(10);
	gStyle->SetFrameBorderSize(2);
	gStyle->SetFrameBorderMode(0);
	gStyle->SetFrameLineWidth(2);
	//gStyle->SetFrameLineStyle(1);

	gStyle->SetLegendBorderSize(0);
	gStyle->SetLegendFillColor(10);
	gStyle->SetLegendFont(font);	
	gStyle->SetLegendTextSize(0.05);

	gStyle->SetLineColor(1);
	gStyle->SetLineWidth(1); // affect almost everything, be careful to set it to be 2
	gStyle->SetHistLineColor(1);
	gStyle->SetHistLineWidth(2);
	gStyle->SetMarkerStyle(20);
	gStyle->SetMarkerSize(1.);

	gStyle->SetNdivisions(510,"xyz");
	gStyle->SetTitleStyle(0);
	gStyle->SetTitleBorderSize(0);
	gStyle->SetTitleAlign(23); // adjust title position of histogram / graph etc 
	gStyle->SetTitleColor(1);
	gStyle->SetTitleFont(font,"xyz");
	//gStyle->SetTitleSize(0.056,"xyz");
	gStyle->SetTitleSize(0.06,"xyz");
	gStyle->SetTitleOffset(0.9,"x");
	gStyle->SetTitleOffset(0.95,"yz");
	gStyle->SetTickLength(0.02,"xyz");
	//gStyle->SetLabelSize(0.045,"xyz");
	gStyle->SetLabelSize(0.05,"xyz");
	gStyle->SetLabelFont(font,"xyz");
	gStyle->SetLabelOffset(0.008,"xyz");
}

// set color display, raibow, grayscale...
// see more in https://root.cern.ch/doc/master/classTColor.html
void set_color_env()
{
	cout<<"Build new color environment !"<<endl;

	const Int_t NRGBs = 5;
	const Int_t NCont = 255;

	Int_t fcol;

	Double_t stops[NRGBs] = { 0.00, 0.34, 0.61, 0.84, 1.00 };
	Double_t red[NRGBs]   = { 0.00, 0.00, 0.87, 1.00, 0.51 };
	Double_t green[NRGBs] = { 0.00, 0.81, 1.00, 0.20, 0.00 };
	Double_t blue[NRGBs]  = { 0.51, 1.00, 0.12, 0.00, 0.00 };
	fcol = TColor::CreateGradientColorTable(NRGBs, stops, red, green, blue, NCont);
	gStyle->SetNumberContours(NCont);
	//SetPalette has been called in the above function, color style will
	//be set according to your own rgb definition if called

	//grayscale
	/*
	   double dcol = 1/double(NRGBs);
	   double grey = 1;
	   for(int j = 0; j < NRGBs; j++){  
	// ...... Define color with RGB equal to : gray, gray, gray .......
	stops[j]=double(j)/double(NRGBs-1);
	red[j]=grey;
	blue[j]=grey;
	green[j]=grey;
	}
	TColor::CreateGradientColorTable(NRGBs, stops, red, green, blue, NCont);
	*/
}

void loadFunctionMacro()
{
	gROOT->ProcessLine(".L /Users/syang/Tools/Macro/function.C");
	//OR
	//gInterpreter->ProcessLine(".L /Users/syang/Tools/Macro/function.C");

	cout<<"Self-defined functions loaded !"<<endl;
}

void myrootlogon()
{
	//load your system wise	root settings
	globleStyle();

	//set_color_env();
	//loadFunctionMacro();
}
