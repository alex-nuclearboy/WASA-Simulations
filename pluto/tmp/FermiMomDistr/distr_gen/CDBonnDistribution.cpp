#include <TH1D.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TH3F.h>
#include <TVector3.h>
#include <TLorentzVector.h>
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
#include <TROOT.h>
#include <Riostream.h>
#include <TGraph.h>
#include <TGraphErrors.h>
#include <TFitResultPtr.h>
#include <TFitResult.h>
#include <TLegend.h>
#include <TGraphSmooth.h>
#include <iostream>
#include <math.h>
#include <cmath>

void CDBonnDistribution() {

    double sqrtpi2 = 0.7978845608;
    const double alpha = 0.2315380;

    TGraph* hCDBonn_momentum = new TGraph();

    double p[401];
    double r[401];
    double f_p[401];

    double norm=0;
    double norm_2=0;

    int j=0;
    p[0]=0;

    while(p[j]<0.4){
        j++;

        p[j] += j;
        p[j] = p[j]/1000;
        r[j] = p[j]/0.197327;

        double m[11], m2[11];

        for(int i=0;i<11;i++){
            m[i] = alpha + i*0.9;
            m2[i] = m[i]*m[i];
        }

        double c[11] = {0.88472985e0, -0.26408759e0, -0.44114404e-1, -0.14397512e2,
                    0.85591256e2,  -0.31876761e3, 0.70336701e3, -0.90049586e3,
                    0.66145441e3,  -0.25958894e3, 0.};

        double d[11] = {0.22623762e-1, -0.50471056e0, 0.56278897e0, -0.16079764e2,
                    0.11126803e3,  -0.44667490e3, 0.10985907e4,  -0.16114995e4,
                    0., 0., 0.,};

        c[10] = 0.;
        for(int i=0;i<10;i++) c[10] -= c[i];   // normalize c[] properly

        //cout<<"c[10]= "<<c[10]<<endl;

        int n = 10, n1 = 9, n2 = 8;

        double sum1 = 0.;
        double sum2 = 0.;
        double sum3 = 0.;
        double rtemp;

        for(int i=0;i<4;i++){
            rtemp = d[i*2]/m2[i*2] + d[i*2+1]/m2[i*2+1];
            sum1 = sum1 + rtemp;
            rtemp = d[i*2] + d[i*2+1];
            sum2 = sum2 + rtemp;
            rtemp = d[i*2]*m2[i*2] + d[i*2+1]*m2[i*2+1];
            sum3 = sum3 + rtemp;
        }

        for(int i=0;i<3;i++){                 // normalize d[] properly
            d[n2] = -m2[n1]*m2[n]*sum1 + (m2[n1]+m2[n])*sum2 - sum3;
            d[n2] = d[n2] * m2[n2]/(m2[n]-m2[n2])/(m2[n1]-m2[n2]);

            int cycle = n2;
            n2 = n1;
            n1 = n;
            n = cycle;
        }

        //cout<<"d[8]= "<<d[8]<<endl;
        //cout<<"d[9]= "<<d[9]<<endl;
        //cout<<"d[10]= "<<d[10]<<endl;

        double U = 0.;
        double W = 0.;

        for(int i=0;i<11;i++) {
            U += c[i]/(r[j]*r[j] + m2[i]);
            W += d[i]/(r[j]*r[j] + m2[i]);
        }

        U = sqrtpi2 * U;    // s wave
        W = sqrtpi2 * W;    // d wave

        f_p[j] = r[j]*r[j]*(U*U + W*W);

        norm +=f_p[j];
    }

    for(int i=0; i<401; i++){

        f_p[i] = f_p[i]/norm;

        norm_2 = norm_2 + f_p[i];

        hCDBonn_momentum->SetPoint(i,p[i],f_p[i]);

        ofstream new_file;
        new_file.open("../FermiMomDistr_CDBONN.dat", ios::app);
        new_file<<Form("%g %g",p[i],f_p[i])<<endl;
        new_file.close();
    }

    //cout<<"norm= "<<norm<<endl;
    //cout<<"norm_2= "<<norm_2<<endl;

    TCanvas* MyCanvas = new TCanvas;
    hCDBonn_momentum->SetTitle("Fermi Momentum Distribution of Nucleons inside the Deuteron: CDBonn Model");
    hCDBonn_momentum->GetXaxis()->SetTitle("p_{F}, GeV/c");
    hCDBonn_momentum->GetXaxis()->SetTitleSize(0.04);
    hCDBonn_momentum->GetXaxis()->SetTitleOffset(1.0);
    hCDBonn_momentum->GetYaxis()->SetTitle("Probability Density, c/GeV");
    hCDBonn_momentum->GetYaxis()->SetTitleSize(0.04);
    hCDBonn_momentum->GetYaxis()->SetTitleOffset(1.2);
    hCDBonn_momentum->GetXaxis()->SetRangeUser(0.,0.4);
    //hCDBonn_momentum->GetYaxis()->SetRangeUser(0.,0.012);
    //hCDBonn_momentum->SetLineStyle(2);
    hCDBonn_momentum->SetLineWidth(2);
    hCDBonn_momentum->SetLineColor(32);

    hCDBonn_momentum->Draw("");

    MyCanvas->Print("FermiMomDistr_CDBONN.png","png");

}
