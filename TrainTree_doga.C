#define TrainTree_doga_cxx
#include "TrainTree_doga.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TGraph.h>
#include <iostream>
void TrainTree_doga::Loop()
{
//   In a ROOT session, you can do:
//      Root > .L TrainTree_doga.C
//      Root > TrainTree_doga t
//      Root > t.GetEntry(12); // Fill t data members with entry number 12
//      Root > t.Show();       // Show values of entry 12
//      Root > t.Show(16);     // Read and show values of entry 16
//      Root > t.Loop();       // Loop on all entries
//

//     This is the loop skeleton where:
//    jentry is the global entry number in the chain
//    ientry is the entry number in the current Tree
//  Note that the argument to GetEntry must be:
//    jentry for TChain::GetEntry
//    ientry for TTree::GetEntry and TBranch::GetEntry
//
//       To read only selected branches, Insert statements like:
// METHOD1:
//    fChain->SetBranchStatus("*",0);  // disable all branches
//    fChain->SetBranchStatus("branchname",1);  // activate branchname
// METHOD2: replace line
//    fChain->GetEntry(jentry);       //read all branches
//by  b_branchname->GetEntry(ientry); //read only this branch
   if (fChain == 0) return;

   Long64_t nentries = fChain->GetEntriesFast();

   Long64_t nbytes = 0, nb = 0;
   int nbin=20;
   int ncent=5;
   int centmin[]={0,2,4,10,20};
   int centmax[]={2,4,10,20,40};
   double prob_LD_cut[]={0,0.05,0.1,0.15,0.2,0.25,0.3,0.35,0.4,0.45,0.5,0.55,0.6,0.65,0.7,0.75,0.8,0.85,0.9,0.95};
   double nsignal_selected[ncent][nbin];
   double nbckg_selected[ncent][nbin];
   int nsignal[ncent];
   int nbckg[ncent];
   TH1D * hpt[ncent][2];
   TH1D * hchi2[ncent][2];
   TH1D * heta[ncent][2];
   for(int icent=0;icent<ncent;icent++){
    nsignal[icent]=0;
    nbckg[icent]=0;
	hpt[icent][0]=new TH1D(Form("hpt_%d_0",icent),"",20,0,1);
	hpt[icent][1]=new TH1D(Form("hpt_%d_1",icent),"",20,0,1);
	heta[icent][0]=new TH1D(Form("heta_%d_0",icent),"",20,-1,1);
	heta[icent][1]=new TH1D(Form("heta_%d_1",icent),"",20,-1,1);
	hchi2[icent][0]=new TH1D(Form("hchi2_%d_0",icent),"",50,0,100);
	hchi2[icent][1]=new TH1D(Form("hchi2_%d_1",icent),"",50,0,100);
    for(int ibin=0;ibin<nbin;ibin++){
     nsignal_selected[icent][ibin]=0;
     nbckg_selected[icent][ibin]=0;
    }
   }
   
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
   // for (Long64_t jentry=0; jentry<100;jentry++) {
      Long64_t ientry = LoadTree(jentry); 
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
      // if (Cut(ientry) < 0) continue;
      cout<<strcmp(className,"Signal")<<" "<<className<<endl;
      if(strcmp(className,"Signal")==0){
       for(int icent=0;icent<ncent;icent++){
        if(hiBin>centmin[icent] && hiBin<centmax[icent]){
         nsignal[icent]++;
         for(int ibin=0;ibin<nbin;ibin++){
          if(prob_LD>=prob_LD_cut[ibin])nsignal_selected[icent][ibin]++;
		  if(ibin==6){ 
		   hpt[icent][0]->Fill(trkPt);
		   hchi2[icent][0]->Fill(trkChi2);
		   heta[icent][0]->Fill(trkEta);
		  }
         }
        }
       }
      }
      else{
      for(int icent=0;icent<ncent;icent++){
       if(hiBin>centmin[icent] && hiBin<centmax[icent]){    
        nbckg[icent]++;
        for(int ibin=0;ibin<nbin;ibin++){
         if(prob_LD<prob_LD_cut[ibin]){
		  nbckg_selected[icent][ibin]++;
		  if(ibin==6){
  		   hpt[icent][1]->Fill(trkPt);
  		   heta[icent][1]->Fill(trkEta);
  		   hchi2[icent][1]->Fill(trkChi2);
		  }
		 }
        }
       }
      }
     }
   }
   TFile *outf = new TFile("centdependence_of_roc_pt1_3_chi2_5.root","recreate");
   TGraph *g[ncent]; 
   for(int icent=0;icent<ncent;icent++){
    for(int ibin=0;ibin<nbin;ibin++){
     nbckg_selected[icent][ibin]=nbckg_selected[icent][ibin]/nbckg[icent];
     nsignal_selected[icent][ibin]=nsignal_selected[icent][ibin]/nsignal[icent];
    }
   g[icent]= new TGraph(nbin,nsignal_selected[icent],nbckg_selected[icent]);
   g[icent]->SetName(Form("g_%d",icent));
   g[icent]->Write();
   hpt[icent][0]->Write();
   hpt[icent][1]->Write();
   heta[icent][0]->Write();
   heta[icent][1]->Write();
   hchi2[icent][0]->Write();
   hchi2[icent][1]->Write();
  }
  outf->Close();
}
