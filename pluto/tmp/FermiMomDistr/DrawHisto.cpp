#include <TH1F.h>
#include <TH2F.h>
#include <TH1D.h>
#include <TF1.h>
#include <TFile.h>
#include <TTree.h>
#include <TMath.h>
#include <TCanvas.h>
#include <TClonesArray.h>
#include <TPaveLabel.h>
#include <TFrame.h>
#include <TSystem.h>
#include <TNtuple.h>
#include <TPaveText.h>
#include <TInterpreter.h>
#include <TStyle.h>
#include <TAxis.h>
#include <Riostream.h>
#include <TLegend.h>
#include <TLine.h>
#include <TROOT.h>
#include <TGraph.h>
#include <TGraphErrors.h>
#include <stdio.h>

void DrawHisto() {

    TGraph *MomentumDistribition[3];
    FILE *MomDistFile[3];

    Float_t Momentum[3];
    Float_t Probability[3];

    MomDistFile[0] = fopen("FermiMomDistr_PARIS.dat","r");
    MomDistFile[1] = fopen("FermiMomDistr_CDBONN_Sk.dat","r");
    MomDistFile[2] = fopen("FermiMomDistr_chiral.dat","r");

    MomentumDistribition[0] = new TGraph();
    Int_t i=-1;

    while (!feof(MomDistFile[0])) {
        i++;
        fscanf(MomDistFile[0],"%f %f\n",&Momentum[0],&Probability[0]);
        MomentumDistribition[0]->SetPoint(i,Momentum[0],Probability[0]);
    }

    MomentumDistribition[1] = new TGraph();
    Int_t j=-1;
    while (!feof(MomDistFile[1])) {
        j++;
        fscanf(MomDistFile[1],"%f %f\n",&Momentum[1],&Probability[1]);
        MomentumDistribition[1]->SetPoint(j,Momentum[1],Probability[1]);
    }

    MomentumDistribition[2] = new TGraph();
    Int_t k=-1;
    while (!feof(MomDistFile[2])) {
        k++;
        fscanf(MomDistFile[2],"%f %f\n",&Momentum[2],&Probability[2]);
        MomentumDistribition[2]->SetPoint(k,Momentum[2],Probability[2]);
    }


    gStyle->SetOptStat(kFALSE);
    gStyle->SetPalette(1,0);
    gStyle->SetOptStat(kFALSE);
    gStyle->SetPadLeftMargin(0.15);
    gStyle->SetPadRightMargin(0.12);
    gStyle->SetPadBottomMargin(0.15);

    TCanvas *MyCanvas[4];
    TLegend *MyLegend[4];

    MyCanvas[0] = new TCanvas;
    gPad->SetTickx(0);
    gPad->SetTicky(0);

    //MomentumDistribition[0]->SetTitle("Fermi Momentum Distribition of Nucleons inside the Deuteron");
    MomentumDistribition[0]->GetXaxis()->SetTitle("p_{F}, GeV/c");
    MomentumDistribition[0]->GetXaxis()->SetTitleSize(0.05);
    MomentumDistribition[0]->GetXaxis()->SetTitleOffset(1.);
    MomentumDistribition[0]->GetXaxis()->SetLabelSize(0.04);
    MomentumDistribition[0]->GetXaxis()->SetRangeUser(0.,0.4);
    //MomentumDistribition[0]->GetXaxis()->SetNdivisions(609);
    MomentumDistribition[0]->GetYaxis()->SetTitle("Probability density, c/GeV");
    MomentumDistribition[0]->GetYaxis()->SetTitleSize(0.05);
    MomentumDistribition[0]->GetYaxis()->SetTitleOffset(1.1);
    MomentumDistribition[0]->GetYaxis()->SetLabelSize(0.04);
    //MomentumDistribition[0]->GetYaxis()->SetNdivisions(609);
    //MomentumDistribition[0]->GetYaxis()->SetRangeUser(0.,0.012);

    MomentumDistribition[0]->SetLineStyle(1);
    MomentumDistribition[0]->SetLineWidth(2);
    MomentumDistribition[0]->SetLineColor(46);
    MomentumDistribition[0]->Draw("");

    MomentumDistribition[1]->SetLineStyle(1);
    MomentumDistribition[1]->SetLineWidth(2);
    MomentumDistribition[1]->SetLineColor(12);
    MomentumDistribition[1]->Draw("same");

    MomentumDistribition[2]->SetLineStyle(2);
    MomentumDistribition[2]->SetLineWidth(2);
    MomentumDistribition[2]->SetLineColor(38);
    //MomentumDistribition[2]->Draw("same");

    MyLegend[0] = new TLegend(0.596, 0.734, 0.867, 0.863);
    MyLegend[0]->SetFillStyle(1); MyLegend[0]->SetFillColor(0); MyLegend[0]->SetLineColor(0); MyLegend[0]->SetTextSize(0.04);
    MyLegend[0]->AddEntry(MomentumDistribition[0], "PARIS model" , "l");
    MyLegend[0]->AddEntry(MomentumDistribition[1], "CDBonn model" , "l");
    //MyLegend[0]->AddEntry(MomentumDistribition[2], "Chiral" , "l");
    MyLegend[0]->Draw("same");

    MyCanvas[0]->Print("FermiMomDistr_all.png","png");

    /*
    MyCanvas[1] = new TCanvas("MyCanvas[1]","Fermi Momentum Distribition of Nucleons inside the Deuteron");

    MyCanvas[1] ->cd();
    MyCanvas[1]->Divide(2,1,0.01, 0.01);

    MyCanvas[1] ->cd(1);
    gPad->SetTickx(1);
    gPad->SetTicky(1);
    MomentumDistribition[0]->SetTitle("PARIS model");
    MomentumDistribition[0]->GetXaxis()->SetRangeUser(0.,0.4);
    MomentumDistribition[0]->Draw("");

    MyCanvas[1] ->cd(2);
    gPad->SetTickx(1);
    gPad->SetTicky(1);
    MomentumDistribition[1]->SetTitle("CDBonn model");
    MomentumDistribition[1]->GetXaxis()->SetTitle("Momentum [GeV/c]");
    MomentumDistribition[1]->GetXaxis()->SetTitleSize(0.05);
    MomentumDistribition[1]->GetXaxis()->SetTitleOffset(1.);
    MomentumDistribition[1]->GetXaxis()->SetLabelSize(0.04);
    MomentumDistribition[1]->GetYaxis()->SetTitle("Probability density [c/MeV]");
    MomentumDistribition[1]->GetYaxis()->SetTitleSize(0.05);
    MomentumDistribition[1]->GetYaxis()->SetTitleOffset(1.2);
    MomentumDistribition[1]->GetYaxis()->SetLabelSize(0.04);
    MomentumDistribition[1]->GetXaxis()->SetRangeUser(0.,0.4);
    MomentumDistribition[1]->SetLineStyle(1);
    MomentumDistribition[1]->SetLineWidth(1);
    MomentumDistribition[1]->SetLineColor(1);
    MomentumDistribition[1]->Draw("");

    MyCanvas[1] ->cd(3);
    gPad->SetTickx(1);
    gPad->SetTicky(1);
    MomentumDistribition[2]->SetTitle("Chiral");
    MomentumDistribition[2]->GetXaxis()->SetTitle("Momentum [GeV/c]");
    MomentumDistribition[2]->GetXaxis()->SetTitleSize(0.05);
    MomentumDistribition[2]->GetXaxis()->SetTitleOffset(1.);
    MomentumDistribition[2]->GetXaxis()->SetLabelSize(0.04);
    MomentumDistribition[2]->GetYaxis()->SetTitle("Probability density [c/MeV]");
    MomentumDistribition[2]->GetYaxis()->SetTitleSize(0.05);
    MomentumDistribition[2]->GetYaxis()->SetTitleOffset(1.2);
    MomentumDistribition[2]->GetYaxis()->SetLabelSize(0.04);
    MomentumDistribition[2]->GetXaxis()->SetRangeUser(0.,0.4);
    MomentumDistribition[2]->SetLineStyle(1);
    MomentumDistribition[2]->SetLineWidth(1);
    MomentumDistribition[2]->SetLineColor(1);
    MomentumDistribition[2]->Draw("");
*/

}
