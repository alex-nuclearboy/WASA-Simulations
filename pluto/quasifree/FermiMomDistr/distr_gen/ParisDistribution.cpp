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

void ParisDistribution() {

    const double sqrtpi2 = 0.7978845608;
    const double alpha = 0.23162461;

    TGraph* hParis_momentum = new TGraph();

    double p[401];
    double r[401];
    double f_p[401];

    double norm=0;
    double norm_2=0;

    int j=0;
    p[0]=0;

    while(p[j]<0.4){
        j+=1;

        p[j]=p[j]+j;
        p[j]=p[j]/1000;
        r[j] = p[j]/0.197327;

        double m[13], m2[13];

        for(int i=0;i<13;i++){
            m[i] = alpha + i;
            m2[i] = m[i]*m[i];
        }

        double c[13] = {0.88688076e0, -0.34717093e0, -0.30502380e1, 0.56207766e2,
                    -0.74957334e3,  0.53365279e4, -0.22706863e5, 0.60434469e5,
                    -0.10292058e6,  0.11223357e6, -0.75925226e5, 0.29059715e5,
                    0.};

        double d[13] = {0.23135193e-1, -0.85604572e0, 0.56068193e1, -0.69462922e2,
                    0.41631118e3,  -0.12546621e4, 0.12387830e4,  0.33739172e4,
                    -0.13041151e5,   0.19512524e5, 0., 0., 0.,};

        c[12] = 0.;
        for(int i=0;i<12;i++) c[12] -= c[i];   // normalize c[] properly

        //cout<<"c[12]= "<<c[12]<<endl;

        int n = 12, n1 = 11, n2 = 10;

        double sum1 = 0.;
        double sum2 = 0.;
        double sum3 = 0.;
        double rtemp;

        for(int i=0;i<5;i++){
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

        //cout<<"d[10]= "<<d[10]<<endl;
        //cout<<"d[11]= "<<d[11]<<endl;
        //cout<<"d[12]= "<<d[12]<<endl;

        double U = 0.;
        double W = 0.;

        for(int i=0;i<13;i++) {
            U += c[i]/(r[j]*r[j] + m2[i]);
            W += d[i]/(r[j]*r[j] + m2[i]);
        }

        U = sqrtpi2 * U;    // s wave
        W = sqrtpi2 * W;    // d wave

        f_p[j] = r[j]*r[j]*(U*U + W*W);

        norm = norm + f_p[j];
    }

    for(int i=0; i<401; i++){

        f_p[i] = f_p[i]/norm;

        norm_2 = norm_2 + f_p[i];

        hParis_momentum->SetPoint(i,p[i],f_p[i]);

        ofstream new_file;
        new_file.open("../FermiMomDistr_PARIS.dat", ios::app);
        new_file<<Form("%g %g",p[i],f_p[i])<<endl;
        new_file.close();
    }

    //cout<<"norm= "<<norm<<endl;
    //cout<<"norm_2= "<<norm_2<<endl;

    TCanvas* MyCanvas = new TCanvas;
    hParis_momentum->SetTitle("Fermi Momentum Distribution of Nucleons inside the Deuteron: PARIS Model");
    hParis_momentum->GetXaxis()->SetTitle("p_{F} GeV/c");
    hParis_momentum->GetXaxis()->SetTitleSize(0.04);
    hParis_momentum->GetXaxis()->SetTitleOffset(1.0);
    hParis_momentum->GetYaxis()->SetTitle("Probability density, c/GeV");
    hParis_momentum->GetYaxis()->SetTitleSize(0.04);
    hParis_momentum->GetYaxis()->SetTitleOffset(1.2);
    hParis_momentum->GetXaxis()->SetRangeUser(0.,0.4);
    //hParis_momentum->GetYaxis()->SetRangeUser(0.,0.012);
    //hParis_momentum->SetLineStyle(2);
    hParis_momentum->SetLineWidth(2);
    hParis_momentum->SetLineColor(46);

    hParis_momentum->Draw("");

    MyCanvas->Print("FermiMomDistr_PARIS.png","png");

}
