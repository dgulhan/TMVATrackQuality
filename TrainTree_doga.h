//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Tue Jan 28 11:26:02 2014 by ROOT version 5.32/00
// from TTree TrainTree/TrainTree
// found on file: TMVA_1600_pt0_1_eta0_1_4.root
//////////////////////////////////////////////////////////

#ifndef TrainTree_doga_h
#define TrainTree_doga_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.

// Fixed size dimensions of array or collections stored in the TTree if any.
const Int_t kMaxabs_trkPtError_D_trkPt = 1;
const Int_t kMaxabs_trkDz1_D_trkDzError1 = 1;
const Int_t kMaxabs_trkDxy1_D_trkDxyError1 = 1;

class TrainTree_doga {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

   // Declaration of leaf types
   Int_t           classID;
   Char_t          className[11];
   Float_t         abs_trkPtError_D_trkPt_;
   Float_t         trkChi2;
   Float_t         abs_trkDz1_D_trkDzError1_;
   Float_t         abs_trkDxy1_D_trkDxyError1_;
   Float_t         trkNHit;
   Float_t         trkNdof;
   Float_t         trkEta;
   Float_t         trkPt;
   Float_t         trkPhi;
   Float_t         trkNlayer;
   Float_t         hiBin;
   Float_t         weight;
   Float_t         CutsGA;
   Float_t         LD;
   Float_t         prob_LD;

   // List of branches
   TBranch        *b_classID;   //!
   TBranch        *b_className;   //!
   TBranch        *b_abs_trkPtError_D_trkPt_;   //!
   TBranch        *b_trkChi2;   //!
   TBranch        *b_abs_trkDz1_D_trkDzError1_;   //!
   TBranch        *b_abs_trkDxy1_D_trkDxyError1_;   //!
   TBranch        *b_trkNHit;   //!
   TBranch        *b_trkNdof;   //!
   TBranch        *b_trkEta;   //!
   TBranch        *b_trkPt;   //!
   TBranch        *b_trkPhi;   //!
   TBranch        *b_trkNlayer;   //!
   TBranch        *b_hiBin;   //!
   TBranch        *b_weight;   //!
   TBranch        *b_CutsGA;   //!
   TBranch        *b_LD;   //!
   TBranch        *b_prob_LD;   //!

   TrainTree_doga(TTree *tree=0);
   virtual ~TrainTree_doga();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef TrainTree_doga_cxx
TrainTree_doga::TrainTree_doga(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("TMVA_1600_pt0_1_eta0_1_0_chi2_5.root");
      if (!f || !f->IsOpen()) {
         // f = new TFile("TMVA_1600_pt0_1_eta0_1_4.root");
         // f = new TFile("TMVA_1600_pt1_3_eta0_1_4.root");
         // f = new TFile("TMVA_1600_pt1_3_eta0_1_0.root");
         f = new TFile("TMVA_1600_pt0_1_eta0_1_0_chi2_5.root");
      }
      f->GetObject("TrainTree",tree);

   }
   Init(tree);
}

TrainTree_doga::~TrainTree_doga()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t TrainTree_doga::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t TrainTree_doga::LoadTree(Long64_t entry)
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

void TrainTree_doga::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("classID", &classID, &b_classID);
   fChain->SetBranchAddress("className", className, &b_className);
   fChain->SetBranchAddress("abs_trkPtError_D_trkPt_", &abs_trkPtError_D_trkPt_, &b_abs_trkPtError_D_trkPt_);
   fChain->SetBranchAddress("trkChi2", &trkChi2, &b_trkChi2);
   fChain->SetBranchAddress("abs_trkDz1_D_trkDzError1_", &abs_trkDz1_D_trkDzError1_, &b_abs_trkDz1_D_trkDzError1_);
   fChain->SetBranchAddress("abs_trkDxy1_D_trkDxyError1_", &abs_trkDxy1_D_trkDxyError1_, &b_abs_trkDxy1_D_trkDxyError1_);
   fChain->SetBranchAddress("trkNHit", &trkNHit, &b_trkNHit);
   fChain->SetBranchAddress("trkNdof", &trkNdof, &b_trkNdof);
   fChain->SetBranchAddress("trkEta", &trkEta, &b_trkEta);
   fChain->SetBranchAddress("trkPt", &trkPt, &b_trkPt);
   fChain->SetBranchAddress("trkPhi", &trkPhi, &b_trkPhi);
   fChain->SetBranchAddress("trkNlayer", &trkNlayer, &b_trkNlayer);
   fChain->SetBranchAddress("hiBin", &hiBin, &b_hiBin);
   fChain->SetBranchAddress("weight", &weight, &b_weight);
   fChain->SetBranchAddress("CutsGA", &CutsGA, &b_CutsGA);
   fChain->SetBranchAddress("LD", &LD, &b_LD);
   fChain->SetBranchAddress("prob_LD", &prob_LD, &b_prob_LD);
   Notify();
}

Bool_t TrainTree_doga::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void TrainTree_doga::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t TrainTree_doga::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef TrainTree_doga_cxx
