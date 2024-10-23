void rootlogon()
{
	//gROOT->Reset();
	//gROOT->ProcessLine(".x $ROOTSYS/etc/rootlogon.C");

	SysInfo_t sInfo;
	if (gSystem->GetSysInfo(&sInfo) >= 0) 
	{
		cout << "SYSINFO: "
			<< sInfo.fModel << ", "
			<< sInfo.fCpus << " cores @ "
			<< sInfo.fCpuSpeed / 1000.0 << "GHz, "
			<< TMath::Nint(sInfo.fPhysRam / 1024.) << "GB RAM"<< endl;
	}

	printf("******************************************\n");
	printf(" Welcome to ROOT v. %s\n", gROOT->GetVersion());

	printf("******************************************\n");


	gROOT->Macro("/home/tk/ResultAna/myrootlogon.C");
	//gROOT->ProcessLine(".x $HOME/Tools/Macro/myrootlogon.C"); // is equivalent to gROOT->Macro("$HOME/Tools/Macro/myrootlogon.C");

	//// load macros, but will cause contridiction if one explicitly includes /Users/syang/Tools/Macro/function.C in the analysis macro
	//gROOT->ProcessLine(".L /Users/syang/Tools/Macro/function.C");
	//OR
	//gInterpreter->ProcessLine(".L /Users/syang/Tools/Macro/function.C");

	//// just in case root pythia tutorial cannot find related head file
	//gSystem->AddIncludePath("-I/Users/syang/Tools/PYTHIA/pythia8303/include");

	//how to use the color defined below --->  SetLineColor(TColor::GetColor(SkyBlue)) or SetLineColor(TColor::GetColor(SkyBlue.Data()))
	TString Red1 =  "#FFCC00";
	TString Red2 =  "#FF9900";
	TString Red3 =  "#FF6600";
	TString Red4 =  "#FF3300";
	TString Red5 =  "#FF0000";

	TString Blue1 =  "#3300FF";
	TString Blue2 =  "#0000FF";
	TString Blue3 =  "#0033FF";
	TString Blue4 =  "#0066FF";
	TString Blue5 =  "#0099FF";
	TString Blue6 =  "#00CCFF";
	TString Blue7 =  "#00FFFF";
	TString Blue8 =  "#00FFCC";

	TString Green1 =  "#006633";
	TString Green2 =  "#006600";
	TString Green3 =  "#009933";
	TString Green4 =  "#009900";
	TString Green5 =  "#339900";
	TString Green6 =  "#00CC33";
	TString Green7 =  "#00CC00";
	TString Green8 =  "#33CC00";
	TString Green9 =  "#00FF00";

	TString Oran1 = "#FF3300";
	TString Oran2 = "#FF6600";
	TString Oran3 = "#FF6633";
	TString Oran4 = "#FF9900";
	TString Oran5 = "#FF9933";
	TString Oran6 = "#FF9966";  

	TString SkyBlue = "#00CCFF";
	TString SeaBlue = "#0099FF";
	TString SadBlue = "#009999";
	TString LakeBlue = "#0099CC";
	TString DarkBlue = "#000099";

	TString Purp1 = "#CC33CC";
	TString Purp2 = "#9900FF";
	TString Purp3 = "#CC00FF";
	TString Purp4 = "#FF00FF";
	TString Purp5 = "#FF33FF";
	TString Purp6 = "#FF33CC";   
	TString Purp7 = "#FF66FF";
}
