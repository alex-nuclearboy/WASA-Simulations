/***********************************************
* Software developement for WASA-at-COSY
* (c) 2017-2019 The WASA-at-COSY Collaboration
* This software is distributed under the terms
  of the GNU General Public Licence v3.0
***********************************************/

//Macro for simulation pd -> BS -> pdpi0 -> pd2g reaction

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
#include <TGraph.h>
#include <TROOT.h>
#include <PParticle.h>

void eventgenerator() {

    static  Double_t m_target = 1.875613; // deuteron target mass [GeV]
    static  Double_t m_beam = 0.938272;   // proton beam mass     [GeV]
    static  Double_t m_3He = 2.808950;    // 3He mass             [GeV]
    //static  Double_t m_4He = 3.7264225;   // 4He mass             [GeV]
    //static  Double_t m_n = 0.93956;       // neutron mass         [GeV]
    static  Double_t m_pi0 = 0.13497;     // pi0 mass             [GeV]
    static  Double_t m_eta = 0.547853;    // eta mass             [GeV]
    static  Double_t m_p = 0.938272;      // neutron mass         [GeV]
    static  Double_t m_d = 1.875613;      // deuteron mass        [GeV]

    //BOUND STATE PARAMETERS (peak in invariant mass spectrum)
    static  Double_t Gamma = 0.01;          //bound state width   [GeV]
    static  Double_t Bs = 0.01;             //binding energy      [GeV]
    static  Double_t s_thr = m_3He + m_eta; //s on threshold      [GeV]
    static  Double_t m_bs = s_thr - Bs;     //bound state mass    [GeV]
    static  Double_t Ns_max = 2/((TMath::Pi())*Gamma);  // peak maximum in s=m_bs
    //static  Double_t Ns_max = 1;            // peak maximum in s = m_bs

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    static Int_t nevents = 500000;

    TLorentzVector pout[4];

    TString fileout = "${PLUTO_OUTPUT}/pd-bound-pdpi0_G10_Bs10-1.root";
    //TString fout = fileout + Form("_%.3f.root", p_beam);

    TFile* newfile = new TFile((const char*) fileout,"RECREATE");

    ////General structure of the tree////
    //Declaration of leave types
    Int_t Npart;
    Float_t Impact;
    Float_t Phi;

    TClonesArray *Particles = new TClonesArray("PParticle",4);  //PParticle from Pluto - 4 particles

    TTree *baobab = new TTree("data","oak tree");
    baobab->Branch("Npart",&Npart,"Npart/I");
    baobab->Branch("Impact",&Impact,"Impact/F");
    baobab->Branch("Phi",&Phi,"Phi/F");
    baobab->Branch("Particles",&Particles);

    ////Read momentum distributions////
    //AV18
    FILE *AV18file;
    AV18file = fopen("MomentumDistr_3He.dat","r");    //File with beam momentum

    TGraph *gFermiMomDistr_AV18 = new TGraph();
    Float_t FermiMom_AV18 = 0.; //Fermi momentum: first column
    Float_t ProbMom_AV18 = 0.;  //Probability: second column

    Int_t i=-1;
    while (!feof(AV18file)) {
        i++;
        fscanf(AV18file,"%f %f\n",&FermiMom_AV18,&ProbMom_AV18);
        gFermiMomDistr_AV18->SetPoint(i,FermiMom_AV18,ProbMom_AV18);
    }

    //gFermiMomDistr_AV18->Draw("AP");  //Draw AP (A-axis, P-points)

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    TRandom3 *r = new TRandom3(0);  //Uniform probability
    r->SetSeed(0);

    //TF1 *f1 = new TF1("Uniform","1",-10.,10.);    //Uniform probability
    //gRandom->SetSeed(0);

    for(Int_t k = 0; k < nevents; ) {   //01//

        ////LAB////

        Double_t r1 = r->Rndm();
        Double_t p_beam = 1.426 + 0.209*r1;     //Ramped proton beam (from 1.426 to 1.635 [GeV/c])
        //Double_t p_beam=f1->GetRandom(1.426,1.635);

        Double_t E_beam = TMath::Sqrt(TMath::Power(p_beam,2) + TMath::Power(m_beam,2));
        //Double_t E_beam = TMath::Sqrt((p_beam*p_beam) + (m_beam*m_beam));

        Double_t s = TMath::Sqrt(TMath::Power(m_beam,2) + TMath::Power(m_target,2) + 2*m_target*TMath::Sqrt(TMath::Power(m_beam,2) + TMath::Power(p_beam,2)));
        //Double_t s = TMath::Sqrt(m_beam*m_beam+m_target*m_target+2*m_target*TMath::Sqrt(m_beam*m_beam+p_beam*p_beam));

        Double_t Beta_cm = (p_beam)/(m_target+TMath::Sqrt(TMath::Power(m_target,2) + TMath::Power(p_beam,2)));
        //Double_t Beta_cm = (p_beam)/(m_target+TMath::Sqrt(m_target*m_target+p_beam*p_beam));

        Double_t Gamma_cm = 1/(TMath::Sqrt(1 - TMath::Power(Beta_cm,2)));
        //Double_t Gamma_cm=1/(TMath::Sqrt(1-Beta_cm*Beta_cm));

        ////CM////
        Double_t p_beam_cm = p_beam + Beta_cm*Gamma_cm*((Gamma_cm*Beta_cm*p_beam/(Gamma_cm + 1)) - E_beam);     //calculated correctly (checked with boost)
        Double_t E_beam_cm = Gamma_cm*(E_beam - Beta_cm*p_beam);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        ////FOUR-VECTORS////

        //beam and target//

        TVector3 vec_beam;
        vec_beam.SetMagThetaPhi(p_beam,0,0);
        TLorentzVector P_b;     //4-vector of the beam
        P_b.SetVectM(vec_beam,m_beam);

        TVector3 vec_target;
        vec_target.SetMagThetaPhi(0,0,0);
        TLorentzVector P_t;     //4-vector of the target
        P_t.SetVectM(vec_target,m_target);

        TLorentzVector P_tot = P_b + P_t;       // total 4-vector

        ////Events with Breit-Wigner distribution of s [=s_pd]////

        TF1 *fBW_dist = new TF1("fBW",Form("%f/((2*TMath::Pi())*((x-%f)*(x-%f)+%f*%f/4))",Gamma,m_bs,m_bs,Gamma,Gamma),3.25,3.40);
        //TF1 *fBW_dist = new TF1("fBW",Form("(%f*%f/4)/((x-%f)*(x-%f)+%f*%f/4)",Gamma,Gamma,m_bs,m_bs,Gamma,Gamma),3.25,3.40);

        //Double_t z1 = f1->GetRandom(0.,1.);
        Double_t z1 = r->Rndm();
        Double_t w1 = Ns_max*z1;
        Double_t Ns = fBW_dist->Eval(s);

        //fBW_dist->Draw("C");  //Draw C-smooth line (for 1 event)

        //if(Ns > Ns_max) continue;

        if(Ns > w1) {   //02//

            Double_t Q = s - s_thr;     //Q random from -70 MeV to 30 MeV

            //Nucleon momentum distribution for "N" in 3He (AV18)
            //CM frame of 3He

            Double_t r2 = r->Rndm();
            Double_t CosTheta_N_cm = 2*r2 - 1;  //cos(theta) for resonance N* random from -1 to 1 (uniform distribution)
            //Double_t CosTheta_N_cm = f1->GetRandom(-1.0,1.0);
            Double_t Theta_N_cm = TMath::ACos(CosTheta_N_cm);     //Polar angle [rad]

            Double_t r3 = r->Rndm();
            Double_t Phi_N_cm = TMath::Pi()*(2*r3 - 1);   //Azimuthal angle random (uniform distribution from -pi to pi) [rad]
            //Double_t Phi_N_cm = TMath::Pi()*(2*r3);   //uniform distribution from 0 to 2*pi [rad]
            //Double_t Phi_N_cm = f1->GetRandom(0.,2*TMath::Pi());  //uniform distribution from 0 to 2*pi [rad]

            Double_t r4 = r->Rndm();
            Double_t pf = 0.4*r4;
            //Double_t pf = f1->GetRandom(0.,0.4);

            Double_t pfx = pf*sin(Theta_N_cm)*cos(Phi_N_cm);
            Double_t pfy = pf*sin(Theta_N_cm)*sin(Phi_N_cm);
            Double_t pfz = pf*cos(Theta_N_cm);

            Double_t Npf_max = TMath::MaxElement(gFermiMomDistr_AV18->GetN(),gFermiMomDistr_AV18->GetY());  //maximum value of function (TGraph)

            //Double_t z2 = f1->GetRandom(0.,1.);
            Double_t z2 = r->Rndm();
            Double_t w2 = Npf_max*z2;   //random N

            Double_t Npf = gFermiMomDistr_AV18->Eval(pf);

            if(Npf > w2) {  //03//

                ////CM system frame////

                //Deuteron
                Double_t px_d_cm = -pfx;
                Double_t py_d_cm = -pfy;
                Double_t pz_d_cm = -pfz;
                Double_t p_d_cm = TMath::Sqrt(TMath::Power(px_d_cm,2) + TMath::Power(py_d_cm,2) + TMath::Power(pz_d_cm,2));
                //Double_t p_d_cm = TMath::Sqrt(px_d_cm*px_d_cm + py_d_cm*py_d_cm + pz_d_cm*pz_d_cm);
                Double_t E_d_cm = TMath::Sqrt(TMath::Power(p_d_cm,2) + TMath::Power(m_d,2));
                //Double_t E_d_cm = TMath::Sqrt(p_d_cm*p_d_cm + m_d*m_d);

                Double_t Theta_d_cm = (TMath::Pi()) - Theta_N_cm;   //[rad]
                Double_t Phi_d_cm = Phi_N_cm + (TMath::Pi());       //[rad]

                TVector3 vec_pd_cm;
                vec_pd_cm.SetMagThetaPhi(p_d_cm,Theta_d_cm,Phi_d_cm);
                TLorentzVector P_d;
                P_d.SetVectM(vec_pd_cm,m_d);

                //Resonance N*
                Double_t px_N_cm = pfx;
                Double_t py_N_cm = pfy;
                Double_t pz_N_cm = pfz;

                Double_t p_N_cm = TMath::Sqrt(TMath::Power(px_N_cm,2) + TMath::Power(py_N_cm,2) + TMath::Power(pz_N_cm,2));
                //Double_t p_N_cm = TMath::Sqrt(px_N_cm*px_N_cm + py_N_cm*py_N_cm + pz_N_cm*pz_N_cm);

                //Resonance mass (m_N*) should be higher than m_p + m_pi0
                Double_t m_N = TMath::Sqrt(TMath::Power(s,2) + TMath::Power(m_d,2) - 2*s*(TMath::Sqrt(TMath::Power(m_d,2) + TMath::Power(pf,2))));
                //Double_t m_N = TMath::Sqrt(s*s + m_d*m_d - 2*s*(TMath::Sqrt(m_d*m_d + pf*pf)));

                if(m_N > (m_p + m_pi0)) {   //04//

                    Double_t E_N_cm = TMath::Sqrt(TMath::Power(p_N_cm,2) + TMath::Power(m_N,2));
                    //Double_t E_N_cm = TMath::Sqrt(p_N_cm*p_N_cm+m_N*m_N);

                    TVector3 vec_pN_cm;
                    vec_pN_cm.SetMagThetaPhi(p_N_cm,Theta_N_cm,Phi_N_cm);
                    TLorentzVector P_N;
                    P_N.SetVectM(vec_pN_cm,m_N);

                    ////N* CM frame////
                    //Proton

                    Double_t r5 = r->Rndm();
                    Double_t CosTheta_p_cmN = 2*r5 - 1;  //cos(theta) for outgoing proton random from -1 to 1 (uniform distribution)
                    //Double_t CosTheta_p_cmN = f1->GetRandom(-1.0,1.0);
                    Double_t Theta_p_cmN = TMath::ACos(CosTheta_p_cmN);     //Polar angle [rad]

                    Double_t r6 = r->Rndm();
                    Double_t Phi_p_cmN = TMath::Pi()*(2*r6 - 1);    //Azimuthal angle random (uniform distribution from -pi to pi) [rad]
                    //Double_t Phi_p_cmN = TMath::Pi()*(2*r6);        //uniform distribution from 0 to 2*pi [rad]
                    //Double_t Phi_p_cmN = f1->GetRandom(0.,2*TMath::Pi());  //uniform distribution from 0 to 2*pi [rad]

                    Double_t p_p_cmN = TMath::Sqrt(TMath::Power((TMath::Power(m_N,2) - TMath::Power(m_pi0,2) - TMath::Power(m_p,2)),2) - 4*(TMath::Power(m_pi0,2))*(TMath::Power(m_p,2)))/(2*m_N);
                    //Double_t p_p_cmN = TMath::Sqrt((m_N*m_N - m_pi0*m_pi0 - m_p*m_p)*(m_N*m_N - m_pi0*m_pi0 - m_p*m_p) - 4*(m_pi0*m_pi0)*(m_p*m_p))/(2*m_N);
                    Double_t px_p_cmN = p_p_cmN*sin(Theta_p_cmN)*cos(Phi_p_cmN);
                    Double_t py_p_cmN = p_p_cmN*sin(Theta_p_cmN)*sin(Phi_p_cmN);
                    Double_t pz_p_cmN = p_p_cmN*cos(Theta_p_cmN);
                    Double_t E_p_cmN = TMath::Sqrt(TMath::Power(p_p_cmN,2) + TMath::Power(m_p,2));
                    //Double_t E_p_cmN = TMath::Sqrt(p_p_cmN*p_p_cmN + m_p*m_p);

                    TVector3 vec_pp_cmN;
                    vec_pp_cmN.SetMagThetaPhi(p_p_cmN,Theta_p_cmN,Phi_p_cmN);
                    TLorentzVector P_p;
                    P_p.SetVectM(vec_pp_cmN,m_p);

                    //pi0
                    Double_t Theta_pi0_cmN = (TMath::Pi()) - Theta_p_cmN;   //[rad]
                    Double_t Phi_pi0_cmN = Phi_p_cmN + (TMath::Pi());       //[rad]

                    Double_t p_pi0_cmN = TMath::Sqrt(TMath::Power((TMath::Power(m_N,2) - TMath::Power(m_pi0,2) - TMath::Power(m_p,2)),2) - 4*(TMath::Power(m_pi0,2))*(TMath::Power(m_p,2)))/(2*m_N);
                    //Double_t p_pi0_cmN = TMath::Sqrt((m_N*m_N - m_pi0*m_pi0 - m_p*m_p)*(m_N*m_N - m_pi0*m_pi0 - m_p*m_p) - 4*(m_pi0*m_pi0)*(m_p*m_p))/(2*m_N);
                    Double_t px_pi0_cmN = p_pi0_cmN*sin(Theta_pi0_cmN)*cos(Phi_pi0_cmN);
                    Double_t py_pi0_cmN = p_pi0_cmN*sin(Theta_pi0_cmN)*sin(Phi_pi0_cmN);
                    Double_t pz_pi0_cmN = p_pi0_cmN*cos(Theta_pi0_cmN);
                    Double_t E_pi0_cmN = TMath::Sqrt(TMath::Power(p_pi0_cmN,2) + TMath::Power(m_pi0,2));
                    //Double_t E_pi0_cmN = TMath::Sqrt(p_pi0_cmN*p_pi0_cmN + m_pi0*m_pi0);

                    TVector3 vec_ppi0_cmN;
                    vec_ppi0_cmN.SetMagThetaPhi(p_pi0_cmN,Theta_pi0_cmN,Phi_pi0_cmN);
                    TLorentzVector P_pi0;
                    P_pi0.SetVectM(vec_ppi0_cmN,m_pi0);

                    ////pi0 CM frame////
                    //Gamma #1
                    Double_t r7 = r->Rndm();
                    Double_t CosTheta_g1_cmPi0 = 2*r7 - 1;  //cos(theta) for gamma quanta random from -1 to 1 (uniform distribution)
                    //Double_t CosTheta_g1_cmPi0 = f1->GetRandom(-1.0,1.0);
                    Double_t Theta_g1_cmPi0 = TMath::ACos(CosTheta_g1_cmPi0);     //Polar angle [rad]

                    Double_t r8 = r->Rndm();
                    Double_t Phi_g1_cmPi0 = TMath::Pi()*(2*r8 - 1);   //Azimuthal angle random (uniform distribution from -pi to pi) [rad]
                    //Double_t Phi_g1_cmPi0 = TMath::Pi()*(2*r8);     //uniform distribution from 0 to 2*pi [rad]
                    //Double_t Phi_g1_cmPi0 = f1->GetRandom(0.,2*TMath::Pi());  //uniform distribution from 0 to 2*pi [rad]

                    Double_t p_g1_cmPi0 = m_pi0/2;  //For gamma quanta #1
                    Double_t px_g1_cmPi0 = p_g1_cmPi0*sin(Theta_g1_cmPi0)*cos(Phi_g1_cmPi0);
                    Double_t py_g1_cmPi0 = p_g1_cmPi0*sin(Theta_g1_cmPi0)*sin(Phi_g1_cmPi0);
                    Double_t pz_g1_cmPi0 = p_g1_cmPi0*cos(Theta_g1_cmPi0);
                    Double_t E_g1_cmPi0 = p_g1_cmPi0;

                    TVector3 vec_pg1_cmPi0;
                    vec_pg1_cmPi0.SetMagThetaPhi(p_g1_cmPi0,Theta_g1_cmPi0,Phi_g1_cmPi0);
                    TLorentzVector P_g1;
                    P_g1.SetVectM(vec_pg1_cmPi0,0.);

                    //Gamma #2
                    Double_t Theta_g2_cmPi0 = (TMath::Pi())-Theta_g1_cmPi0; //[rad]
                    Double_t Phi_g2_cmPi0 = Phi_g1_cmPi0 + (TMath::Pi());   //[rad]

                    Double_t p_g2_cmPi0 = m_pi0/2;  //For gamma quanta #2
                    Double_t px_g2_cmPi0 = p_g2_cmPi0*sin(Theta_g2_cmPi0)*cos(Phi_g2_cmPi0);
                    Double_t py_g2_cmPi0 = p_g2_cmPi0*sin(Theta_g2_cmPi0)*sin(Phi_g2_cmPi0);
                    Double_t pz_g2_cmPi0 = p_g2_cmPi0*cos(Theta_g2_cmPi0);
                    Double_t E_g2_cmPi0 = p_g2_cmPi0;

                    TVector3 vec_pg2_cmPi0;
                    vec_pg2_cmPi0.SetMagThetaPhi(p_g2_cmPi0,Theta_g2_cmPi0,Phi_g2_cmPi0);
                    TLorentzVector P_g2;
                    P_g2.SetVectM(vec_pg2_cmPi0,0.);

//////////////////////////////////////////////////PLUTOconvention/////////////////////////////////////////////////

                    pout[0] = P_p;
                    pout[1] = P_d;
                    pout[2] = P_g1;
                    pout[3] = P_g2;

////////////////////////////////////////////////USING BOOST VECTOR////////////////////////////////////////////////

                    //boost to LAB
                    TVector3 b;
                    b = P_tot.BoostVector();  // boost to LAB
                    TVector3 b1;
                    b1 = P_N.BoostVector();   // boost to CM from N*
                    TVector3 b2;
                    b2 = P_pi0.BoostVector(); // boost to N* from Pi0

                    //Deuteron
                    pout[1].Boost(b);
                    Double_t px_d_lab = pout[1].Px();
                    Double_t py_d_lab = pout[1].Py();
                    Double_t pz_d_lab = pout[1].Pz();
                    Double_t p_d_lab = TMath::Sqrt(TMath::Power(px_d_lab,2) + TMath::Power(py_d_lab,2) + TMath::Power(pz_d_lab,2));
                    //Double_t p_d_lab = TMath::Sqrt(px_d_lab*px_d_lab + py_d_lab*py_d_lab + pz_d_lab*pz_d_lab);
                    //Double_t E_d_lab = TMath::Sqrt(p_d_lab*p_d_lab + m_d*m_d);
                    Double_t E_d_lab = TMath::Sqrt(TMath::Power(p_d_lab,2) + TMath::Power(m_d,2));
                    Double_t Ekin_d_lab = E_d_lab - m_d;
                    Double_t Theta_d_lab = pout[1].Theta();
                    Double_t CosTheta_d_lab = TMath::Cos(Theta_d_lab);
                    Double_t Phi_d_lab = pout[1].Phi();

                    //Proton
                    //boost to CM
                    pout[0].Boost(b1);
                    Double_t px_p_cm = pout[0].Px();
                    Double_t py_p_cm = pout[0].Py();
                    Double_t pz_p_cm = pout[0].Pz();
                    Double_t p_p_cm = TMath::Sqrt(TMath::Power(px_p_cm,2) + TMath::Power(py_p_cm,2) + TMath::Power(pz_p_cm,2));
                    //Double_t p_p_cm = TMath::Sqrt(px_p_cm*px_p_cm + py_p_cm*py_p_cm + pz_p_cm*pz_p_cm);
                    Double_t E_p_cm = TMath::Sqrt(TMath::Power(p_p_cm,2) + TMath::Power(m_p,2));
                    //Double_t E_p_cm = TMath::Sqrt(p_p_cm*p_p_cm + m_p*m_p);
                    Double_t Ekin_p_cm = E_p_cm - m_p;
                    Double_t Theta_p_cm = pout[0].Theta();
                    Double_t CosTheta_p_cm = TMath::Cos(Theta_p_cm);
                    Double_t Phi_p_cm = pout[0].Phi();

                    //boost to LAB
                    pout[0].Boost(b);
                    Double_t px_p_lab = pout[0].Px();
                    Double_t py_p_lab = pout[0].Py();
                    Double_t pz_p_lab = pout[0].Pz();
                    Double_t p_p_lab = TMath::Sqrt(TMath::Power(px_p_lab,2) + TMath::Power(py_p_lab,2) + TMath::Power(pz_p_lab,2));
                    //Double_t p_p_lab = TMath::Sqrt(px_p_lab*px_p_lab + py_p_lab*py_p_lab + pz_p_lab*pz_p_lab);
                    Double_t E_p_lab = TMath::Sqrt(TMath::Power(p_p_lab,2) + TMath::Power(m_p,2));
                    //Double_t E_p_lab = TMath::Sqrt(p_p_lab*p_p_lab + m_p*m_p);

                    Double_t Ekin_p_lab = E_p_lab - m_p;
                    Double_t Theta_p_lab = pout[0].Theta();
                    Double_t CosTheta_p_lab = TMath::Cos(Theta_p_lab);
                    Double_t Phi_p_lab = pout[0].Phi();

                    /*
                    //Pion
                    //boost to CM
                    P_pi0.Boost(b1);
                    Double_t px_pi0_cm = P_pi0.Px();
                    Double_t py_pi0_cm = P_pi0.Py();
                    Double_t pz_pi0_cm = P_pi0.Pz();
                    Double_t p_pi0_cm = TMath::Sqrt(TMath::Power(px_pi0_cm,2) + TMath::Power(py_pi0_cm,2) + TMath::Power(pz_pi0_cm,2));
                    //Double_t p_pi0_cm = TMath::Sqrt(px_pi0_cm*px_pi0_cm + py_pi0_cm*py_pi0_cm + pz_pi0_cm*pz_pi0_cm);
                    Double_t E_pi0_cm = TMath::Sqrt(TMath::Power(p_pi0_cm,2) + TMath::Power(m_pi0,2));
                    //Double_t E_pi0_cm = TMath::Sqrt(p_pi0_cm*p_pi0_cm + m_pi0*m_pi0);
                    Double_t Ekin_pi0_cm = E_pi0_cm - m_pi0;
                    Double_t Theta_pi0_cm = P_pi0.Theta();
                    Double_t CosTheta_pi0_cm = TMath::Cos(Theta_pi0_cm);
                    Double_t Phi_pi0_cm = P_pi0.Phi();
                    //boost to LAB
                    P_pi0.Boost(b);
                    Double_t px_pi0_lab = P_pi0.Px();
                    Double_t py_pi0_lab = P_pi0.Py();
                    Double_t pz_pi0_lab = P_pi0.Pz();
                    Double_t p_pi0_lab = TMath::Sqrt(TMath::Power(px_pi0_lab,2) + TMath::Power(py_pi0_lab,2) + TMath::Power(pz_pi0_lab,2);
                    //Double_t p_pi0_lab = TMath::Sqrt(px_pi0_lab*px_pi0_lab + py_pi0_lab*py_pi0_lab + pz_pi0_lab*pz_pi0_lab);
                    Double_t E_pi0_lab = TMath::Sqrt(TMath::Power(p_pi0_lab,2) + TMath::Power(m_pi0,2));
                    //Double_t E_pi0_lab = TMath::Sqrt(p_pi0_lab*p_pi0_lab + m_pi0*m_pi0);
                    Double_t Ekin_pi0_lab = E_pi0_lab - m_pi0;
                    Double_t Theta_pi0_lab = P_pi0.Theta();
                    Double_t CosTheta_pi0_lab = TMath::Cos(Theta_pi0_lab);
                    Double_t Phi_pi0_lab = P_pi0.Phi();
                    */

                    //Gamma #1
                    //boost to N*
                    pout[2].Boost(b2);
                    Double_t px_g1_cmN = pout[2].Px();
                    Double_t py_g1_cmN = pout[2].Py();
                    Double_t pz_g1_cmN = pout[2].Pz();
                    Double_t p_g1_cmN = TMath::Sqrt(TMath::Power(px_g1_cmN,2) + TMath::Power(py_g1_cmN,2) + TMath::Power(pz_g1_cmN,2));
                    //Double_t p_g1_cmN = TMath::Sqrt(px_g1_cmN*px_g1_cmN + py_g1_cmN*py_g1_cmN + pz_g1_cmN*pz_g1_cmN);
                    Double_t E_g1_cmN = p_g1_cmN;
                    Double_t Ekin_g1_cmN = E_g1_cmN;
                    Double_t Theta_g1_cmN = pout[2].Theta();
                    Double_t CosTheta_g1_cmN = TMath::Cos(Theta_g1_cmN);
                    Double_t Phi_g1_cmN = pout[2].Phi();

                    //boost to CM
                    pout[2].Boost(b1);
                    Double_t px_g1_cm = pout[2].Px();
                    Double_t py_g1_cm = pout[2].Py();
                    Double_t pz_g1_cm = pout[2].Pz();
                    Double_t p_g1_cm = TMath::Sqrt(TMath::Power(px_g1_cm,2) + TMath::Power(py_g1_cm,2) + TMath::Power(pz_g1_cm,2));
                    //Double_t p_g1_cm = TMath::Sqrt(px_g1_cm*px_g1_cm + py_g1_cm*py_g1_cm + pz_g1_cm*pz_g1_cm);
                    Double_t E_g1_cm = p_g1_cm;
                    Double_t Ekin_g1_cm = E_g1_cm;
                    Double_t Theta_g1_cm = pout[2].Theta();
                    Double_t CosTheta_g1_cm = TMath::Cos(Theta_g1_cm);
                    Double_t Phi_g1_cm = pout[2].Phi();

                    //boost to LAB
                    pout[2].Boost(b);
                    Double_t px_g1_lab = pout[2].Px();
                    Double_t py_g1_lab = pout[2].Py();
                    Double_t pz_g1_lab = pout[2].Pz();
                    Double_t p_g1_lab = TMath::Sqrt(TMath::Power(px_g1_lab,2) + TMath::Power(py_g1_lab,2) + TMath::Power(pz_g1_lab,2));
                    //Double_t p_g1_lab = TMath::Sqrt(px_g1_lab*px_g1_lab + py_g1_lab*py_g1_lab + pz_g1_lab*pz_g1_lab);
                    Double_t E_g1_lab = p_g1_lab;
                    Double_t Ekin_g1_lab = E_g1_lab;
                    Double_t Theta_g1_lab = pout[2].Theta();
                    Double_t CosTheta_g1_lab = TMath::Cos(Theta_g1_lab);
                    Double_t Phi_g1_lab = pout[2].Phi();

                    //Gamma #2
                    //boost to N*
                    pout[3].Boost(b2);
                    Double_t px_g2_cmN = pout[3].Px();
                    Double_t py_g2_cmN = pout[3].Py();
                    Double_t pz_g2_cmN = pout[3].Pz();
                    Double_t p_g2_cmN = TMath::Sqrt(TMath::Power(px_g2_cmN,2) + TMath::Power(py_g2_cmN,2) + TMath::Power(pz_g2_cmN,2));
                    //Double_t p_g2_cmN = TMath::Sqrt(px_g2_cmN*px_g2_cmN + py_g2_cmN*py_g2_cmN + pz_g2_cmN*pz_g2_cmN);
                    Double_t E_g2_cmN = p_g2_cmN;
                    Double_t Ekin_g2_cmN = E_g2_cmN;
                    Double_t Theta_g2_cmN = pout[3].Theta();
                    Double_t CosTheta_g2_cmN = TMath::Cos(Theta_g2_cmN);
                    Double_t Phi_g2_cmN = pout[3].Phi();

                    //boost to CM
                    pout[3].Boost(b1);
                    Double_t px_g2_cm = pout[3].Px();
                    Double_t py_g2_cm = pout[3].Py();
                    Double_t pz_g2_cm = pout[3].Pz();
                    Double_t p_g2_cm = TMath::Sqrt(TMath::Power(px_g2_cm,2) + TMath::Power(py_g2_cm,2) + TMath::Power(pz_g2_cm,2));
                    //Double_t p_g2_cm = TMath::Sqrt(px_g2_cm*px_g2_cm + py_g2_cm*py_g2_cm + pz_g2_cm*pz_g2_cm);
                    Double_t E_g2_cm = p_g2_cm;
                    Double_t Ekin_g2_cm = E_g2_cm;
                    Double_t Theta_g2_cm = pout[3].Theta();
                    Double_t CosTheta_g2_cm = TMath::Cos(Theta_g2_cm);
                    Double_t Phi_g2_cm = pout[3].Phi();

                    //boost to LAB
                    pout[3].Boost(b);
                    Double_t px_g2_lab = pout[3].Px();
                    Double_t py_g2_lab = pout[3].Py();
                    Double_t pz_g2_lab = pout[3].Pz();
                    Double_t p_g2_lab = TMath::Sqrt(TMath::Power(px_g2_lab,2) + TMath::Power(py_g2_lab,2) + TMath::Power(pz_g2_lab,2));
                    //Double_t p_g2_lab = TMath::Sqrt(px_g2_lab*px_g2_lab + py_g2_lab*py_g2_lab + pz_g2_lab*pz_g2_lab);
                    Double_t E_g2_lab = p_g2_lab;
                    Double_t Ekin_g2_lab = E_g2_lab;
                    Double_t Theta_g2_lab = pout[3].Theta();
                    Double_t CosTheta_g2_lab = TMath::Cos(Theta_g2_lab);
                    Double_t Phi_g2_lab = pout[3].Phi();

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

                    Npart = 4;
                    Impact = 0;
                    Phi = 0;
                    Particles->Clear();     //We clear all objects in TClonesArray

                    //new ((*Particles)[0])
                    //PParticle("dummy",pout[0].Px(),pout[0].Py(),pout[0].Pz(),pout[0].M());
                    //(PParticle*)((*Particles)[0]))->SetID(45045);

                    new ((*Particles)[0]) PParticle("p",pout[0].Px(),pout[0].Py(),pout[0].Pz(),pout[0].M());
                    new ((*Particles)[1]) PParticle("d",pout[1].Px(),pout[1].Py(),pout[1].Pz(),pout[1].M());
                    //new ((*Particles)[2]) PParticle("pi0",pout[2].Px(),pout[2].Py(),pout[2].Pz(),pout[2].M());
                    new ((*Particles)[2]) PParticle("g",pout[2].Px(),pout[2].Py(),pout[2].Pz(),pout[2].M());
                    new ((*Particles)[3]) PParticle("g",pout[3].Px(),pout[3].Py(),pout[3].Pz(),pout[3].M());

                    baobab->Fill();

                    k++;

                }   //04//

            }   //03//

        }   //02//

        fBW_dist->Delete();

    }   //01//

    baobab->Write();
    newfile->Close();

}
