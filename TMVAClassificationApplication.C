/**********************************************************************************
 * Project   : TMVA - a Root-integrated toolkit for multivariate data analysis    *
 * Package   : TMVA                                                               *
 * Exectuable: TMVAClassificationApplication                                      *
 *                                                                                *
 * This macro provides a simple example on how to use the trained classifiers     *
 * within an analysis module                                                      *
 **********************************************************************************/

#include <cstdlib>
#include <vector>
#include <iostream>
#include <map>
#include <string>

#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TSystem.h"
#include "TROOT.h"
#include "TStopwatch.h"
#include "TMath.h"
#include "TH1D.h"

#include "TMVAGui.C"
#include "HiForest/hiForest.h"
#include "makeEvalPlots.h"

#if not defined(__CINT__) || defined(__MAKECINT__)
#include "TMVA/Tools.h"
#include "TMVA/Reader.h"
#include "TMVA/MethodCuts.h"
#endif

using namespace TMVA;

void TMVAClassificationApplication( TString myMethodList = "", int algo = 4 ) 
{   
#ifdef __CINT__
   gROOT->ProcessLine( ".O0" ); // turn off optimization in CINT
#endif

   //---------------------------------------------------------------

   // This loads the library
   TMVA::Tools::Instance();

   // Default MVA methods to be trained + tested
   std::map<std::string,int> Use;

   // --- Cut optimisation
   Use["Cuts"]            = 0;
   Use["CutsD"]           = 0;
   Use["CutsPCA"]         = 0;
   Use["CutsGA"]          = 0;
   Use["CutsSA"]          = 0;
   // 
   // --- 1-dimensional likelihood ("naive Bayes estimator")
   Use["Likelihood"]      = 0;
   Use["LikelihoodD"]     = 0; // the "D" extension indicates decorrelated input variables (see option strings)
   Use["LikelihoodPCA"]   = 0; // the "PCA" extension indicates PCA-transformed input variables (see option strings)
   Use["LikelihoodKDE"]   = 0;
   Use["LikelihoodMIX"]   = 0;
   //
   // --- Mutidimensional likelihood and Nearest-Neighbour methods
   Use["PDERS"]           = 0;
   Use["PDERSD"]          = 0;
   Use["PDERSPCA"]        = 0;
   Use["PDEFoam"]         = 0;
   Use["PDEFoamBoost"]    = 0; // uses generalised MVA method boosting
   Use["KNN"]             = 1; // k-nearest neighbour method
   //
   // --- Linear Discriminant Analysis
   Use["LD"]              = 0; // Linear Discriminant identical to Fisher
   Use["Fisher"]          = 0;
   Use["FisherG"]         = 0;
   Use["BoostedFisher"]   = 0; // uses generalised MVA method boosting
   Use["HMatrix"]         = 0;
   //
   // --- Function Discriminant analysis
   Use["FDA_GA"]          = 0; // minimisation of user-defined function using Genetics Algorithm
   Use["FDA_SA"]          = 0;
   Use["FDA_MC"]          = 0;
   Use["FDA_MT"]          = 0;
   Use["FDA_GAMT"]        = 0;
   Use["FDA_MCMT"]        = 0;
   //
   // --- Neural Networks (all are feed-forward Multilayer Perceptrons)
   Use["MLP"]             = 0; // Recommended ANN
   Use["MLPBFGS"]         = 0; // Recommended ANN with optional training method
   Use["MLPBNN"]          = 0; // Recommended ANN with BFGS training method and bayesian regulator
   Use["CFMlpANN"]        = 0; // Depreciated ANN from ALEPH
   Use["TMlpANN"]         = 0; // ROOT's own ANN
   //
   // --- Support Vector Machine 
   Use["SVM"]             = 0;
   // 
   // --- Boosted Decision Trees
   Use["BDT"]             = 1; // uses Adaptive Boost
   Use["BDTG"]            = 0; // uses Gradient Boost
   Use["BDTB"]            = 0; // uses Bagging
   Use["BDTD"]            = 0; // decorrelation + Adaptive Boost
   // 
   // --- Friedman's RuleFit method, ie, an optimised series of cuts ("rules")
   Use["RuleFit"]         = 0;
   // ---------------------------------------------------------------
   Use["Plugin"]          = 0;
   Use["Category"]        = 0;
   Use["SVM_Gauss"]       = 0;
   Use["SVM_Poly"]        = 0;
   Use["SVM_Lin"]         = 0;

   std::cout << std::endl;
   std::cout << "==> Start TMVAClassificationApplication" << std::endl;

   // Select methods (don't look at this code - not of interest)
   if (myMethodList != "") {
      for (std::map<std::string,int>::iterator it = Use.begin(); it != Use.end(); it++) it->second = 0;

      std::vector<TString> mlist = gTools().SplitString( myMethodList, ',' );
      for (UInt_t i=0; i<mlist.size(); i++) {
         std::string regMethod(mlist[i]);

         if (Use.find(regMethod) == Use.end()) {
            std::cout << "Method \"" << regMethod 
                      << "\" not known in TMVA under this name. Choose among the following:" << std::endl;
            for (std::map<std::string,int>::iterator it = Use.begin(); it != Use.end(); it++) {
               std::cout << it->first << " ";
            }
            std::cout << std::endl;
            return;
         }
         Use[regMethod] = 1;
      }
   }

   // --------------------------------------------------------------------------------------------------

   // --- Create the Reader object

   TMVA::Reader *reader = new TMVA::Reader( "!Color:!Silent" );    

   // Create a set of variables and declare them to the reader
   // - the variable names MUST corresponds in name and type to those given in the weight file(s) used
   Float_t tmva_ptError, tmva_dzError, tmva_dxyError, tmva_chi2;
   Float_t tmva_trkEta ,tmva_trkPhi,tmva_trkPt, tmva_trkNHit, tmva_hiBin;

   reader->AddVariable( "abs(trkPtError/trkPt)", &tmva_ptError );
   reader->AddVariable( "Chi2DOF := trkChi2/trkNdof", &tmva_chi2 );
   reader->AddVariable( "abs(trkDz1/trkDzError1)",&tmva_dzError );
   reader->AddVariable( "abs(trkDxy1/trkDxyError1)",&tmva_dxyError );
   reader->AddVariable( "trkNHit",&tmva_trkNHit );
   reader->AddVariable( "trkEta", &tmva_trkEta );
   reader->AddVariable( "trkPhi", &tmva_trkPhi );
   reader->AddVariable( "trkPt",  &tmva_trkPt );
   //reader->AddVariable( "hiBin",  &tmva_hiBin );

   // Spectator variables declared in the training have to be added to the reader, too
   Float_t tmva_highPurity, tmva_trkNlayer;
   reader->AddSpectator( "trkNlayer",   &tmva_trkNlayer  );
   reader->AddSpectator( "highPurity",  &tmva_highPurity );

   // --- Book the MVA methods

   reader->BookMVA( "BDT4", "weights/TMVAClassification_BDT4.weights.xml");
   reader->BookMVA( "BDT5", "weights/TMVAClassification_BDT5.weights.xml");
   reader->BookMVA( "BDT6", "weights/TMVAClassification_BDT6.weights.xml"); 

   HiForest * h = new HiForest("/mnt/hadoop/cms/store/user/dgulhan/PYTHIA_HYDJET_Track9_Jet30_Pyquen_DiJet_Pt80_TuneZ2_Unquenched_Hydjet1p8_2760GeV/hiForest_DijetpT370_Hydjet1p8_STARTHI53_LV1_v15_330_1_bb2.root","forest",cPbPb,1);
   //large stats sample
   //HiForest * h = new HiForest("/mnt/hadoop/cms/store/user/dgulhan/PYTHIA_HYDJET_Track9_Jet30_Pyquen_DiJet_TuneZ2_Unquenched_Hydjet1p8_2760GeV_merged/HiForest_PYTHIA_HYDJET_pthat80_Track9_Jet30_matchEqR_merged_forest_0.root","forest",cPbPb,1);

   h->LoadNoTrees();
   h->hasEvtTree = true;
   h->hasTrackTree = true;   

   TH1::SetDefaultSumw2;
 
   double upperBound = 5;
   if(algo == 5) upperBound = 2.5;

   TH1D * ehighPurityRecoPt = new TH1D("ehighPurityRecoPt","",50,0,upperBound);
   TH1D * eBDTRecoPt = new TH1D("eBDTRecoPt","",50,0,upperBound);
   TH1D * ehighPurityRecoCent = new TH1D("ehighPurityRecoCent","",50,0,200);
   TH1D * eBDTRecoCent = new TH1D("eBDTRecoCent","",50,0,200);

   TH1D * fhighPurityRecoPt = new TH1D("fhighPurityRecoPt","",50,0,upperBound);
   TH1D * fBDTRecoPt = new TH1D("fBDTRecoPt","",50,0,upperBound);
   TH1D * fhighPurityRecoCent = new TH1D("fhighPurityRecoCent","",50,0,200);
   TH1D * fBDTRecoCent = new TH1D("fBDTRecoCent","",50,0,200); 

   //event loop
   //int nEntries = h->GetEntries();
   for(int i = 0; i < 1000; i++)
   {
     h->GetEntry(i);
     if(i%1000 == 0) std::cout << i << std::endl;
     //hiBin for TMVA  
     //tmva_hiBin = h->evt.hiBin; 

     //track loop
     for(int j=0; j< h->track.nTrk; j++)     
     { 
       if(h->track.trkAlgo[j] != algo) continue;
       
       //composite variables for TMVA
       tmva_ptError  = TMath::Abs(h->track.trkPtError[j]/h->track.trkPt[j]);  
       tmva_dzError  = TMath::Abs(h->track.trkDz1[j]/h->track.trkDzError1[j]);
       tmva_dxyError = TMath::Abs(h->track.trkDxy1[j]/h->track.trkDxyError1[j]);
       tmva_chi2     = h->track.trkChi2[j]/h->track.trkNdof[j];

       //simple variables for TMVA
       tmva_trkEta = h->track.trkEta[j];
       tmva_trkPhi = h->track.trkPhi[j]; 
       tmva_trkPt  = h->track.trkPt[j];
       tmva_trkNHit = h->track.trkNHit[j];

       if(!(h->track.trkFake[j]) && h->track.highPurity[j])
       {
         ehighPurityRecoPt->Fill(h->track.trkPt[j]);
         ehighPurityRecoCent->Fill(h->evt.hiBin);
       }
       if(h->track.trkFake[j] && h->track.highPurity[j])
       {
         fhighPurityRecoPt->Fill(h->track.trkPt[j]);
         fhighPurityRecoCent->Fill(h->evt.hiBin);
       }

       double tmvaResponse =  reader->EvaluateMVA(Form("BDT%d",algo));
       if(i == 0 and j ==0) std::cout << Form("Using BDT%d", algo) << std::endl;
       
       double TMVA_Cut = 0; 
       if(algo == 4) TMVA_Cut=0;
       if(algo == 4) TMVA_Cut=-0.03;
       if(algo == 4) TMVA_Cut=0.02;

       if(!(h->track.trkFake[j]) && tmvaResponse > TMVA_Cut)
       {
         eBDTRecoPt->Fill(h->track.trkPt[j]);
         eBDTRecoCent->Fill(h->evt.hiBin);
       }
       if(h->track.trkFake[j] && tmvaResponse > TMVA_Cut)
       {
         fBDTRecoPt->Fill(h->track.trkPt[j]);
         fBDTRecoCent->Fill(h->evt.hiBin);
       } 
     }
   }
  
  eBDTRecoPt->Divide(ehighPurityRecoPt);
  eBDTRecoCent->Divide(ehighPurityRecoCent);

  fBDTRecoPt->Divide(fhighPurityRecoPt);
  fBDTRecoCent->Divide(fhighPurityRecoCent);

  makeEvalPlots(eBDTRecoPt, fBDTRecoPt, (int) algo);
} 
