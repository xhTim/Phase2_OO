//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Mon Sep 30 20:34:40 2024 by ROOT version 6.24/09
// from TTree ParticleTree/ParticleTree
// found on file: /home/tk/Phase2_OO/anaSimu/inFiles/CohJpsi_0n0n/diMu_ana_mc_1.root
//////////////////////////////////////////////////////////

#ifndef ParticleTree_h
#define ParticleTree_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.
#include "vector"
#include "vector"
#include "vector"
#include "vector"
#include "vector"
#include "vector"
#include "vector"

class ParticleTree {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

// Fixed size dimensions of array or collections stored in the TTree if any.

   // Declaration of leaf types
   Bool_t          isValid;
   UChar_t         nPV;
   UShort_t        BXNb;
   UShort_t        NtrkHP;
   UShort_t        Ntrkoffline;
   UInt_t          EventNb;
   UInt_t          LSNb;
   UInt_t          RunNb;
   Int_t           Ntrkgen;
   Float_t         HFsumETMinus;
   Float_t         HFsumETPlus;
   Float_t         Npixel;
   Float_t         NpixelTracks;
   Float_t         PFHFmaxEMinus;
   Float_t         PFHFmaxEPlus;
   Float_t         PFHFsumETMinus;
   Float_t         PFHFsumETPlus;
   Float_t         ZDCMinus;
   Float_t         ZDCPlus;
   Float_t         bestvtxX;
   Float_t         bestvtxY;
   Float_t         bestvtxZ;
   Float_t         genWeight;
   Float_t         vtxXerr;
   Float_t         vtxYerr;
   Float_t         vtxZerr;
   vector<bool>    *evtSel;
   vector<bool>    *passHLT;
   vector<bool>    *passL1;
   vector<bool>    *cand_matchGEN;
   vector<bool>    *cand_momMatchGEN;
   vector<char>    *cand_charge;
   vector<int>     *cand_genPdgId;
   vector<int>     *cand_isSwap;
   vector<int>     *cand_pdgId;
   vector<unsigned char> *cand_sourceId;
   vector<unsigned char> *cand_status;
   vector<unsigned short> *cand_genIdx;
   vector<unsigned short> *cand_srcIdx;
   vector<unsigned short> *cand_trkIdx;
   vector<unsigned int> *cand_momMatchIdx;
   vector<float>   *cand_angle2D;
   vector<float>   *cand_angle3D;
   vector<float>   *cand_dca;
   vector<float>   *cand_decayLength2D;
   vector<float>   *cand_decayLength3D;
   vector<float>   *cand_decayLengthError2D;
   vector<float>   *cand_decayLengthError3D;
   vector<float>   *cand_eta;
   vector<float>   *cand_mass;
   vector<float>   *cand_p;
   vector<float>   *cand_pT;
   vector<float>   *cand_phi;
   vector<float>   *cand_pseudoDecayLengthError2D;
   vector<float>   *cand_pseudoDecayLengthError3D;
   vector<float>   *cand_vtxChi2;
   vector<float>   *cand_vtxProb;
   vector<float>   *cand_y;
   vector<vector<unsigned int> > *cand_dauIdx;
   vector<vector<unsigned int> > *cand_momIdx;
   vector<vector<float> > *cand_etaDau;
   vector<vector<float> > *cand_massDau;
   vector<vector<float> > *cand_pTDau;
   vector<vector<float> > *cand_phiDau;
   vector<bool>    *trk_isHP;
   vector<int>     *trk_nHitPixel;
   vector<int>     *trk_nHitPixelBarrel;
   vector<int>     *trk_nHitPixelEndcap;
   vector<int>     *trk_nHitStrip;
   vector<int>     *trk_nLayer;
   vector<int>     *trk_nLayerPixel;
   vector<int>     *trk_nLayerPixelBarrel;
   vector<int>     *trk_nLayerPixelEndcap;
   vector<int>     *trk_nLayerStrip;
   vector<unsigned short> *trk_algo;
   vector<unsigned short> *trk_nHit;
   vector<float>   *trk_nChi2;
   vector<float>   *trk_pTErr;
   vector<float>   *trk_xyDCASignificance;
   vector<float>   *trk_zDCASignificance;
   vector<vector<unsigned int> > *trk_candIdx;
   vector<vector<float> > *trk_trackCovariance;
   vector<vector<float> > *trk_trackParameters;
   vector<bool>    *muon_hybridSoftID;
   vector<bool>    *muon_isGlobal;
   vector<bool>    *muon_isOneStTight;
   vector<bool>    *muon_isPF;
   vector<bool>    *muon_isTracker;
   vector<bool>    *muon_softID;
   vector<bool>    *muon_tightID;
   vector<char>    *muon_nMuonHit;
   vector<char>    *muon_nPixelHit;
   vector<char>    *muon_nPixelLayer;
   vector<char>    *muon_nTrackerLayer;
   vector<unsigned char> *muon_nMatchedStation;
   vector<unsigned char> *muon_nMatches;
   vector<unsigned int> *muon_candIdx;
   vector<unsigned int> *muon_selectionType;
   vector<unsigned int> *muon_selector;
   vector<float>   *muon_bestTrkdXY;
   vector<float>   *muon_bestTrkdZ;
   vector<float>   *muon_dXSig_seg;
   vector<float>   *muon_dXY;
   vector<float>   *muon_dX_seg;
   vector<float>   *muon_dYSig_seg;
   vector<float>   *muon_dY_seg;
   vector<float>   *muon_dZ;
   vector<float>   *muon_ddXdZSig_seg;
   vector<float>   *muon_ddXdZ_seg;
   vector<float>   *muon_ddYdZSig_seg;
   vector<float>   *muon_ddYdZ_seg;
   vector<float>   *muon_glbTrkNChi2;
   vector<float>   *muon_hadMax;
   vector<char>    *gen_charge;
   vector<int>     *gen_pdgId;
   vector<unsigned char> *gen_status;
   vector<unsigned short> *gen_statusBit;
   vector<float>   *gen_angle2D;
   vector<float>   *gen_angle3D;
   vector<float>   *gen_decayLength2D;
   vector<float>   *gen_decayLength3D;
   vector<float>   *gen_eta;
   vector<float>   *gen_mass;
   vector<float>   *gen_p;
   vector<float>   *gen_pT;
   vector<float>   *gen_phi;
   vector<float>   *gen_y;
   vector<vector<unsigned short> > *gen_dauIdx;
   vector<vector<unsigned short> > *gen_momIdx;
   vector<vector<unsigned int> > *gen_candIdx;
   vector<vector<float> > *gen_trackParameters;

   // List of branches
   TBranch        *b_isValid;   //!
   TBranch        *b_nPV;   //!
   TBranch        *b_BXNb;   //!
   TBranch        *b_NtrkHP;   //!
   TBranch        *b_Ntrkoffline;   //!
   TBranch        *b_EventNb;   //!
   TBranch        *b_LSNb;   //!
   TBranch        *b_RunNb;   //!
   TBranch        *b_Ntrkgen;   //!
   TBranch        *b_HFsumETMinus;   //!
   TBranch        *b_HFsumETPlus;   //!
   TBranch        *b_Npixel;   //!
   TBranch        *b_NpixelTracks;   //!
   TBranch        *b_PFHFmaxEMinus;   //!
   TBranch        *b_PFHFmaxEPlus;   //!
   TBranch        *b_PFHFsumETMinus;   //!
   TBranch        *b_PFHFsumETPlus;   //!
   TBranch        *b_ZDCMinus;   //!
   TBranch        *b_ZDCPlus;   //!
   TBranch        *b_bestvtxX;   //!
   TBranch        *b_bestvtxY;   //!
   TBranch        *b_bestvtxZ;   //!
   TBranch        *b_genWeight;   //!
   TBranch        *b_vtxXerr;   //!
   TBranch        *b_vtxYerr;   //!
   TBranch        *b_vtxZerr;   //!
   TBranch        *b_evtSel;   //!
   TBranch        *b_passHLT;   //!
   TBranch        *b_passL1;   //!
   TBranch        *b_cand_matchGEN;   //!
   TBranch        *b_cand_momMatchGEN;   //!
   TBranch        *b_cand_charge;   //!
   TBranch        *b_cand_genPdgId;   //!
   TBranch        *b_cand_isSwap;   //!
   TBranch        *b_cand_pdgId;   //!
   TBranch        *b_cand_sourceId;   //!
   TBranch        *b_cand_status;   //!
   TBranch        *b_cand_genIdx;   //!
   TBranch        *b_cand_srcIdx;   //!
   TBranch        *b_cand_trkIdx;   //!
   TBranch        *b_cand_momMatchIdx;   //!
   TBranch        *b_cand_angle2D;   //!
   TBranch        *b_cand_angle3D;   //!
   TBranch        *b_cand_dca;   //!
   TBranch        *b_cand_decayLength2D;   //!
   TBranch        *b_cand_decayLength3D;   //!
   TBranch        *b_cand_decayLengthError2D;   //!
   TBranch        *b_cand_decayLengthError3D;   //!
   TBranch        *b_cand_eta;   //!
   TBranch        *b_cand_mass;   //!
   TBranch        *b_cand_p;   //!
   TBranch        *b_cand_pT;   //!
   TBranch        *b_cand_phi;   //!
   TBranch        *b_cand_pseudoDecayLengthError2D;   //!
   TBranch        *b_cand_pseudoDecayLengthError3D;   //!
   TBranch        *b_cand_vtxChi2;   //!
   TBranch        *b_cand_vtxProb;   //!
   TBranch        *b_cand_y;   //!
   TBranch        *b_cand_dauIdx;   //!
   TBranch        *b_cand_momIdx;   //!
   TBranch        *b_cand_etaDau;   //!
   TBranch        *b_cand_massDau;   //!
   TBranch        *b_cand_pTDau;   //!
   TBranch        *b_cand_phiDau;   //!
   TBranch        *b_trk_isHP;   //!
   TBranch        *b_trk_nHitPixel;   //!
   TBranch        *b_trk_nHitPixelBarrel;   //!
   TBranch        *b_trk_nHitPixelEndcap;   //!
   TBranch        *b_trk_nHitStrip;   //!
   TBranch        *b_trk_nLayer;   //!
   TBranch        *b_trk_nLayerPixel;   //!
   TBranch        *b_trk_nLayerPixelBarrel;   //!
   TBranch        *b_trk_nLayerPixelEndcap;   //!
   TBranch        *b_trk_nLayerStrip;   //!
   TBranch        *b_trk_algo;   //!
   TBranch        *b_trk_nHit;   //!
   TBranch        *b_trk_nChi2;   //!
   TBranch        *b_trk_pTErr;   //!
   TBranch        *b_trk_xyDCASignificance;   //!
   TBranch        *b_trk_zDCASignificance;   //!
   TBranch        *b_trk_candIdx;   //!
   TBranch        *b_trk_trackCovariance;   //!
   TBranch        *b_trk_trackParameters;   //!
   TBranch        *b_muon_hybridSoftID;   //!
   TBranch        *b_muon_isGlobal;   //!
   TBranch        *b_muon_isOneStTight;   //!
   TBranch        *b_muon_isPF;   //!
   TBranch        *b_muon_isTracker;   //!
   TBranch        *b_muon_softID;   //!
   TBranch        *b_muon_tightID;   //!
   TBranch        *b_muon_nMuonHit;   //!
   TBranch        *b_muon_nPixelHit;   //!
   TBranch        *b_muon_nPixelLayer;   //!
   TBranch        *b_muon_nTrackerLayer;   //!
   TBranch        *b_muon_nMatchedStation;   //!
   TBranch        *b_muon_nMatches;   //!
   TBranch        *b_muon_candIdx;   //!
   TBranch        *b_muon_selectionType;   //!
   TBranch        *b_muon_selector;   //!
   TBranch        *b_muon_bestTrkdXY;   //!
   TBranch        *b_muon_bestTrkdZ;   //!
   TBranch        *b_muon_dXSig_seg;   //!
   TBranch        *b_muon_dXY;   //!
   TBranch        *b_muon_dX_seg;   //!
   TBranch        *b_muon_dYSig_seg;   //!
   TBranch        *b_muon_dY_seg;   //!
   TBranch        *b_muon_dZ;   //!
   TBranch        *b_muon_ddXdZSig_seg;   //!
   TBranch        *b_muon_ddXdZ_seg;   //!
   TBranch        *b_muon_ddYdZSig_seg;   //!
   TBranch        *b_muon_ddYdZ_seg;   //!
   TBranch        *b_muon_glbTrkNChi2;   //!
   TBranch        *b_muon_hadMax;   //!
   TBranch        *b_gen_charge;   //!
   TBranch        *b_gen_pdgId;   //!
   TBranch        *b_gen_status;   //!
   TBranch        *b_gen_statusBit;   //!
   TBranch        *b_gen_angle2D;   //!
   TBranch        *b_gen_angle3D;   //!
   TBranch        *b_gen_decayLength2D;   //!
   TBranch        *b_gen_decayLength3D;   //!
   TBranch        *b_gen_eta;   //!
   TBranch        *b_gen_mass;   //!
   TBranch        *b_gen_p;   //!
   TBranch        *b_gen_pT;   //!
   TBranch        *b_gen_phi;   //!
   TBranch        *b_gen_y;   //!
   TBranch        *b_gen_dauIdx;   //!
   TBranch        *b_gen_momIdx;   //!
   TBranch        *b_gen_candIdx;   //!
   TBranch        *b_gen_trackParameters;   //!

   ParticleTree(TTree *tree=0);
   virtual ~ParticleTree();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef ParticleTree_cxx
ParticleTree::ParticleTree(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("/home/tk/Phase2_OO/anaSimu/inFiles/CohJpsi_0n0n/diMu_ana_mc_1.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("/home/tk/Phase2_OO/anaSimu/inFiles/CohJpsi_0n0n/diMu_ana_mc_1.root");
      }
      TDirectory * dir = (TDirectory*)f->Get("/home/tk/Phase2_OO/anaSimu/inFiles/CohJpsi_0n0n/diMu_ana_mc_1.root:/diMuAna");
      dir->GetObject("ParticleTree",tree);

   }
   Init(tree);
}

ParticleTree::~ParticleTree()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t ParticleTree::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t ParticleTree::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void ParticleTree::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set object pointer
   evtSel = 0;
   passHLT = 0;
   passL1 = 0;
   cand_matchGEN = 0;
   cand_momMatchGEN = 0;
   cand_charge = 0;
   cand_genPdgId = 0;
   cand_isSwap = 0;
   cand_pdgId = 0;
   cand_sourceId = 0;
   cand_status = 0;
   cand_genIdx = 0;
   cand_srcIdx = 0;
   cand_trkIdx = 0;
   cand_momMatchIdx = 0;
   cand_angle2D = 0;
   cand_angle3D = 0;
   cand_dca = 0;
   cand_decayLength2D = 0;
   cand_decayLength3D = 0;
   cand_decayLengthError2D = 0;
   cand_decayLengthError3D = 0;
   cand_eta = 0;
   cand_mass = 0;
   cand_p = 0;
   cand_pT = 0;
   cand_phi = 0;
   cand_pseudoDecayLengthError2D = 0;
   cand_pseudoDecayLengthError3D = 0;
   cand_vtxChi2 = 0;
   cand_vtxProb = 0;
   cand_y = 0;
   cand_dauIdx = 0;
   cand_momIdx = 0;
   cand_etaDau = 0;
   cand_massDau = 0;
   cand_pTDau = 0;
   cand_phiDau = 0;
   trk_isHP = 0;
   trk_nHitPixel = 0;
   trk_nHitPixelBarrel = 0;
   trk_nHitPixelEndcap = 0;
   trk_nHitStrip = 0;
   trk_nLayer = 0;
   trk_nLayerPixel = 0;
   trk_nLayerPixelBarrel = 0;
   trk_nLayerPixelEndcap = 0;
   trk_nLayerStrip = 0;
   trk_algo = 0;
   trk_nHit = 0;
   trk_nChi2 = 0;
   trk_pTErr = 0;
   trk_xyDCASignificance = 0;
   trk_zDCASignificance = 0;
   trk_candIdx = 0;
   trk_trackCovariance = 0;
   trk_trackParameters = 0;
   muon_hybridSoftID = 0;
   muon_isGlobal = 0;
   muon_isOneStTight = 0;
   muon_isPF = 0;
   muon_isTracker = 0;
   muon_softID = 0;
   muon_tightID = 0;
   muon_nMuonHit = 0;
   muon_nPixelHit = 0;
   muon_nPixelLayer = 0;
   muon_nTrackerLayer = 0;
   muon_nMatchedStation = 0;
   muon_nMatches = 0;
   muon_candIdx = 0;
   muon_selectionType = 0;
   muon_selector = 0;
   muon_bestTrkdXY = 0;
   muon_bestTrkdZ = 0;
   muon_dXSig_seg = 0;
   muon_dXY = 0;
   muon_dX_seg = 0;
   muon_dYSig_seg = 0;
   muon_dY_seg = 0;
   muon_dZ = 0;
   muon_ddXdZSig_seg = 0;
   muon_ddXdZ_seg = 0;
   muon_ddYdZSig_seg = 0;
   muon_ddYdZ_seg = 0;
   muon_glbTrkNChi2 = 0;
   muon_hadMax = 0;
   gen_charge = 0;
   gen_pdgId = 0;
   gen_status = 0;
   gen_statusBit = 0;
   gen_angle2D = 0;
   gen_angle3D = 0;
   gen_decayLength2D = 0;
   gen_decayLength3D = 0;
   gen_eta = 0;
   gen_mass = 0;
   gen_p = 0;
   gen_pT = 0;
   gen_phi = 0;
   gen_y = 0;
   gen_dauIdx = 0;
   gen_momIdx = 0;
   gen_candIdx = 0;
   gen_trackParameters = 0;
   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("isValid", &isValid, &b_isValid);
   fChain->SetBranchAddress("nPV", &nPV, &b_nPV);
   fChain->SetBranchAddress("BXNb", &BXNb, &b_BXNb);
   fChain->SetBranchAddress("NtrkHP", &NtrkHP, &b_NtrkHP);
   fChain->SetBranchAddress("Ntrkoffline", &Ntrkoffline, &b_Ntrkoffline);
   fChain->SetBranchAddress("EventNb", &EventNb, &b_EventNb);
   fChain->SetBranchAddress("LSNb", &LSNb, &b_LSNb);
   fChain->SetBranchAddress("RunNb", &RunNb, &b_RunNb);
   fChain->SetBranchAddress("Ntrkgen", &Ntrkgen, &b_Ntrkgen);
   fChain->SetBranchAddress("HFsumETMinus", &HFsumETMinus, &b_HFsumETMinus);
   fChain->SetBranchAddress("HFsumETPlus", &HFsumETPlus, &b_HFsumETPlus);
   fChain->SetBranchAddress("Npixel", &Npixel, &b_Npixel);
   fChain->SetBranchAddress("NpixelTracks", &NpixelTracks, &b_NpixelTracks);
   fChain->SetBranchAddress("PFHFmaxEMinus", &PFHFmaxEMinus, &b_PFHFmaxEMinus);
   fChain->SetBranchAddress("PFHFmaxEPlus", &PFHFmaxEPlus, &b_PFHFmaxEPlus);
   fChain->SetBranchAddress("PFHFsumETMinus", &PFHFsumETMinus, &b_PFHFsumETMinus);
   fChain->SetBranchAddress("PFHFsumETPlus", &PFHFsumETPlus, &b_PFHFsumETPlus);
   fChain->SetBranchAddress("ZDCMinus", &ZDCMinus, &b_ZDCMinus);
   fChain->SetBranchAddress("ZDCPlus", &ZDCPlus, &b_ZDCPlus);
   fChain->SetBranchAddress("bestvtxX", &bestvtxX, &b_bestvtxX);
   fChain->SetBranchAddress("bestvtxY", &bestvtxY, &b_bestvtxY);
   fChain->SetBranchAddress("bestvtxZ", &bestvtxZ, &b_bestvtxZ);
   fChain->SetBranchAddress("genWeight", &genWeight, &b_genWeight);
   fChain->SetBranchAddress("vtxXerr", &vtxXerr, &b_vtxXerr);
   fChain->SetBranchAddress("vtxYerr", &vtxYerr, &b_vtxYerr);
   fChain->SetBranchAddress("vtxZerr", &vtxZerr, &b_vtxZerr);
   fChain->SetBranchAddress("evtSel", &evtSel, &b_evtSel);
   fChain->SetBranchAddress("passHLT", &passHLT, &b_passHLT);
   fChain->SetBranchAddress("passL1", &passL1, &b_passL1);
   fChain->SetBranchAddress("cand_matchGEN", &cand_matchGEN, &b_cand_matchGEN);
   fChain->SetBranchAddress("cand_momMatchGEN", &cand_momMatchGEN, &b_cand_momMatchGEN);
   fChain->SetBranchAddress("cand_charge", &cand_charge, &b_cand_charge);
   fChain->SetBranchAddress("cand_genPdgId", &cand_genPdgId, &b_cand_genPdgId);
   fChain->SetBranchAddress("cand_isSwap", &cand_isSwap, &b_cand_isSwap);
   fChain->SetBranchAddress("cand_pdgId", &cand_pdgId, &b_cand_pdgId);
   fChain->SetBranchAddress("cand_sourceId", &cand_sourceId, &b_cand_sourceId);
   fChain->SetBranchAddress("cand_status", &cand_status, &b_cand_status);
   fChain->SetBranchAddress("cand_genIdx", &cand_genIdx, &b_cand_genIdx);
   fChain->SetBranchAddress("cand_srcIdx", &cand_srcIdx, &b_cand_srcIdx);
   fChain->SetBranchAddress("cand_trkIdx", &cand_trkIdx, &b_cand_trkIdx);
   fChain->SetBranchAddress("cand_momMatchIdx", &cand_momMatchIdx, &b_cand_momMatchIdx);
   fChain->SetBranchAddress("cand_angle2D", &cand_angle2D, &b_cand_angle2D);
   fChain->SetBranchAddress("cand_angle3D", &cand_angle3D, &b_cand_angle3D);
   fChain->SetBranchAddress("cand_dca", &cand_dca, &b_cand_dca);
   fChain->SetBranchAddress("cand_decayLength2D", &cand_decayLength2D, &b_cand_decayLength2D);
   fChain->SetBranchAddress("cand_decayLength3D", &cand_decayLength3D, &b_cand_decayLength3D);
   fChain->SetBranchAddress("cand_decayLengthError2D", &cand_decayLengthError2D, &b_cand_decayLengthError2D);
   fChain->SetBranchAddress("cand_decayLengthError3D", &cand_decayLengthError3D, &b_cand_decayLengthError3D);
   fChain->SetBranchAddress("cand_eta", &cand_eta, &b_cand_eta);
   fChain->SetBranchAddress("cand_mass", &cand_mass, &b_cand_mass);
   fChain->SetBranchAddress("cand_p", &cand_p, &b_cand_p);
   fChain->SetBranchAddress("cand_pT", &cand_pT, &b_cand_pT);
   fChain->SetBranchAddress("cand_phi", &cand_phi, &b_cand_phi);
   fChain->SetBranchAddress("cand_pseudoDecayLengthError2D", &cand_pseudoDecayLengthError2D, &b_cand_pseudoDecayLengthError2D);
   fChain->SetBranchAddress("cand_pseudoDecayLengthError3D", &cand_pseudoDecayLengthError3D, &b_cand_pseudoDecayLengthError3D);
   fChain->SetBranchAddress("cand_vtxChi2", &cand_vtxChi2, &b_cand_vtxChi2);
   fChain->SetBranchAddress("cand_vtxProb", &cand_vtxProb, &b_cand_vtxProb);
   fChain->SetBranchAddress("cand_y", &cand_y, &b_cand_y);
   fChain->SetBranchAddress("cand_dauIdx", &cand_dauIdx, &b_cand_dauIdx);
   fChain->SetBranchAddress("cand_momIdx", &cand_momIdx, &b_cand_momIdx);
   fChain->SetBranchAddress("cand_etaDau", &cand_etaDau, &b_cand_etaDau);
   fChain->SetBranchAddress("cand_massDau", &cand_massDau, &b_cand_massDau);
   fChain->SetBranchAddress("cand_pTDau", &cand_pTDau, &b_cand_pTDau);
   fChain->SetBranchAddress("cand_phiDau", &cand_phiDau, &b_cand_phiDau);
   fChain->SetBranchAddress("trk_isHP", &trk_isHP, &b_trk_isHP);
   fChain->SetBranchAddress("trk_nHitPixel", &trk_nHitPixel, &b_trk_nHitPixel);
   fChain->SetBranchAddress("trk_nHitPixelBarrel", &trk_nHitPixelBarrel, &b_trk_nHitPixelBarrel);
   fChain->SetBranchAddress("trk_nHitPixelEndcap", &trk_nHitPixelEndcap, &b_trk_nHitPixelEndcap);
   fChain->SetBranchAddress("trk_nHitStrip", &trk_nHitStrip, &b_trk_nHitStrip);
   fChain->SetBranchAddress("trk_nLayer", &trk_nLayer, &b_trk_nLayer);
   fChain->SetBranchAddress("trk_nLayerPixel", &trk_nLayerPixel, &b_trk_nLayerPixel);
   fChain->SetBranchAddress("trk_nLayerPixelBarrel", &trk_nLayerPixelBarrel, &b_trk_nLayerPixelBarrel);
   fChain->SetBranchAddress("trk_nLayerPixelEndcap", &trk_nLayerPixelEndcap, &b_trk_nLayerPixelEndcap);
   fChain->SetBranchAddress("trk_nLayerStrip", &trk_nLayerStrip, &b_trk_nLayerStrip);
   fChain->SetBranchAddress("trk_algo", &trk_algo, &b_trk_algo);
   fChain->SetBranchAddress("trk_nHit", &trk_nHit, &b_trk_nHit);
   fChain->SetBranchAddress("trk_nChi2", &trk_nChi2, &b_trk_nChi2);
   fChain->SetBranchAddress("trk_pTErr", &trk_pTErr, &b_trk_pTErr);
   fChain->SetBranchAddress("trk_xyDCASignificance", &trk_xyDCASignificance, &b_trk_xyDCASignificance);
   fChain->SetBranchAddress("trk_zDCASignificance", &trk_zDCASignificance, &b_trk_zDCASignificance);
   fChain->SetBranchAddress("trk_candIdx", &trk_candIdx, &b_trk_candIdx);
   fChain->SetBranchAddress("trk_trackCovariance", &trk_trackCovariance, &b_trk_trackCovariance);
   fChain->SetBranchAddress("trk_trackParameters", &trk_trackParameters, &b_trk_trackParameters);
   fChain->SetBranchAddress("muon_hybridSoftID", &muon_hybridSoftID, &b_muon_hybridSoftID);
   fChain->SetBranchAddress("muon_isGlobal", &muon_isGlobal, &b_muon_isGlobal);
   fChain->SetBranchAddress("muon_isOneStTight", &muon_isOneStTight, &b_muon_isOneStTight);
   fChain->SetBranchAddress("muon_isPF", &muon_isPF, &b_muon_isPF);
   fChain->SetBranchAddress("muon_isTracker", &muon_isTracker, &b_muon_isTracker);
   fChain->SetBranchAddress("muon_softID", &muon_softID, &b_muon_softID);
   fChain->SetBranchAddress("muon_tightID", &muon_tightID, &b_muon_tightID);
   fChain->SetBranchAddress("muon_nMuonHit", &muon_nMuonHit, &b_muon_nMuonHit);
   fChain->SetBranchAddress("muon_nPixelHit", &muon_nPixelHit, &b_muon_nPixelHit);
   fChain->SetBranchAddress("muon_nPixelLayer", &muon_nPixelLayer, &b_muon_nPixelLayer);
   fChain->SetBranchAddress("muon_nTrackerLayer", &muon_nTrackerLayer, &b_muon_nTrackerLayer);
   fChain->SetBranchAddress("muon_nMatchedStation", &muon_nMatchedStation, &b_muon_nMatchedStation);
   fChain->SetBranchAddress("muon_nMatches", &muon_nMatches, &b_muon_nMatches);
   fChain->SetBranchAddress("muon_candIdx", &muon_candIdx, &b_muon_candIdx);
   fChain->SetBranchAddress("muon_selectionType", &muon_selectionType, &b_muon_selectionType);
   fChain->SetBranchAddress("muon_selector", &muon_selector, &b_muon_selector);
   fChain->SetBranchAddress("muon_bestTrkdXY", &muon_bestTrkdXY, &b_muon_bestTrkdXY);
   fChain->SetBranchAddress("muon_bestTrkdZ", &muon_bestTrkdZ, &b_muon_bestTrkdZ);
   fChain->SetBranchAddress("muon_dXSig_seg", &muon_dXSig_seg, &b_muon_dXSig_seg);
   fChain->SetBranchAddress("muon_dXY", &muon_dXY, &b_muon_dXY);
   fChain->SetBranchAddress("muon_dX_seg", &muon_dX_seg, &b_muon_dX_seg);
   fChain->SetBranchAddress("muon_dYSig_seg", &muon_dYSig_seg, &b_muon_dYSig_seg);
   fChain->SetBranchAddress("muon_dY_seg", &muon_dY_seg, &b_muon_dY_seg);
   fChain->SetBranchAddress("muon_dZ", &muon_dZ, &b_muon_dZ);
   fChain->SetBranchAddress("muon_ddXdZSig_seg", &muon_ddXdZSig_seg, &b_muon_ddXdZSig_seg);
   fChain->SetBranchAddress("muon_ddXdZ_seg", &muon_ddXdZ_seg, &b_muon_ddXdZ_seg);
   fChain->SetBranchAddress("muon_ddYdZSig_seg", &muon_ddYdZSig_seg, &b_muon_ddYdZSig_seg);
   fChain->SetBranchAddress("muon_ddYdZ_seg", &muon_ddYdZ_seg, &b_muon_ddYdZ_seg);
   fChain->SetBranchAddress("muon_glbTrkNChi2", &muon_glbTrkNChi2, &b_muon_glbTrkNChi2);
   fChain->SetBranchAddress("muon_hadMax", &muon_hadMax, &b_muon_hadMax);
   fChain->SetBranchAddress("gen_charge", &gen_charge, &b_gen_charge);
   fChain->SetBranchAddress("gen_pdgId", &gen_pdgId, &b_gen_pdgId);
   fChain->SetBranchAddress("gen_status", &gen_status, &b_gen_status);
   fChain->SetBranchAddress("gen_statusBit", &gen_statusBit, &b_gen_statusBit);
   fChain->SetBranchAddress("gen_angle2D", &gen_angle2D, &b_gen_angle2D);
   fChain->SetBranchAddress("gen_angle3D", &gen_angle3D, &b_gen_angle3D);
   fChain->SetBranchAddress("gen_decayLength2D", &gen_decayLength2D, &b_gen_decayLength2D);
   fChain->SetBranchAddress("gen_decayLength3D", &gen_decayLength3D, &b_gen_decayLength3D);
   fChain->SetBranchAddress("gen_eta", &gen_eta, &b_gen_eta);
   fChain->SetBranchAddress("gen_mass", &gen_mass, &b_gen_mass);
   fChain->SetBranchAddress("gen_p", &gen_p, &b_gen_p);
   fChain->SetBranchAddress("gen_pT", &gen_pT, &b_gen_pT);
   fChain->SetBranchAddress("gen_phi", &gen_phi, &b_gen_phi);
   fChain->SetBranchAddress("gen_y", &gen_y, &b_gen_y);
   fChain->SetBranchAddress("gen_dauIdx", &gen_dauIdx, &b_gen_dauIdx);
   fChain->SetBranchAddress("gen_momIdx", &gen_momIdx, &b_gen_momIdx);
   fChain->SetBranchAddress("gen_candIdx", &gen_candIdx, &b_gen_candIdx);
   fChain->SetBranchAddress("gen_trackParameters", &gen_trackParameters, &b_gen_trackParameters);
   Notify();
}

Bool_t ParticleTree::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void ParticleTree::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t ParticleTree::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef ParticleTree_cxx
