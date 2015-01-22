#include "TH1D.h"
#include "TCanvas.h"
#include "TAttMarker.h"
#include "TColor.h"
#include "TAttLine.h"
#include "TLegend.h"
#include "TPad.h"
#include "TLine.h"

void makeEvalPlots(TH1D* eff, TH1D* fake, int algo)
{
  TCanvas * c = new TCanvas("c","",800,600);

  TLine * l = new TLine(0,1, eff->GetBinLowEdge(51),1);
  l->SetLineWidth(1);   
  l->SetLineStyle(2);   
  l->SetLineColor(1);

  c->cd(0)->SetFillColor(0);
  eff->SetMinimum(0);
  eff->SetMaximum(1.5);
  if(algo==6) eff->SetMaximum(5);

  eff->SetMarkerStyle(20);
  eff->SetMarkerSize(0.8);
  eff->SetMarkerColor(kBlack);
  eff->SetLineColor(kBlack);
  eff->SetLineWidth(1);
  eff->GetXaxis()->SetTitle("p_{t}");
  eff->GetYaxis()->SetTitle("BDT/highPurity");

  fake->SetMarkerStyle(21);
  fake->SetMarkerSize(0.8);
  fake->SetMarkerColor(kRed+2);
  fake->SetLineColor(kRed+2);
  fake->SetLineWidth(1);

  eff->Draw("p e");
  fake->Draw("p e same");

  TLegend * leg = new TLegend(0.15,0.7,0.4,0.8);
  leg->AddEntry((TObject*)0, Form("trkAlgo=%d",algo),"");
  leg->AddEntry(eff,"Efficiency","pe");
  leg->AddEntry(fake,"Fake","pe");
  leg->Draw("same");
  l->Draw();

  c->SaveAs(Form("plots/EffFake_Algo%d.png",algo));

  return;
}
