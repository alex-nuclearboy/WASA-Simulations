/***********************************************
* Software developement for WASA-at-COSY
* (c) 2016-2019 The WASA-at-COSY Collaboration
* This software is distributed under the terms
  of the GNU General Public Licence v3.0
***********************************************/

//Monte Carlo event generator for quasi-free process pd->pp n_spec, where protons interact and neutron is a spectator
//This macro generates the file in ROOT format (like PLUTO) which is used for WMC

#include<TH1F.h>
#include<TH2F.h>
#include<TH3F.h>
#include<TVector3.h>
#include<TLorentzVector.h>
#include<TF1.h>
#include<TFile.h>
#include<TTree.h>
#include<TMath.h>
#include<TCanvas.h>
#include<TClonesArray.h>
#include<PParticle.h>
#include<TRandom.h>
#include<TRandom3.h>
#include<TGraph.h>
#include <Riostream.h>
#include<TROOT.h>

void eventgenerator() {

    static Double_t m_beam = 0.938272;      //proton beam mass      [GeV/c^2]
    static Double_t m_target = 1.875613;    //deuteron target mass  [GeV/c^2]
    static Double_t m_p = 0.938272;         //proton mass           [GeV/c^2]
    static Double_t m_n = 0.939565;         //neutron mass          [GeV/c^2]
    static Double_t m_d = 1.875613;         //deuterom mass         [GeV/c^2]

    static Int_t nevents = 1000000;         //number of simulated events

    TLorentzVector pout[3];                 //number of four vectors (for outgoing particles)

    //OUTPUT file .root
    TString fileout = "${PLUTO_OUTPUT}/pd-ppn_qf-PARIS-1.root";
    //TString fileout = "${PLUTO_OUTPUT}/pd-ppn_qf-CDBONN-1.root";
    //TString fileout = "${PLUTO_OUTPUT}/pd-ppn_qf-chiral-1.root";

    TFile* newfile = new TFile((const char*) fileout,"RECREATE");

    ////General structure of the tree////
    //declaration of leave types

    Int_t       Npart;
    Float_t     Impact;
    Float_t     Phi;

    TClonesArray *Particles = new TClonesArray("PParticle",3);    //PParticle from Pluto - 3 particles

    TTree *baobab = new TTree("data","oak tree");
    baobab->Branch("Npart",&Npart,"Npart/I");
    baobab->Branch("Impact",&Impact,"Impact/F");
    baobab->Branch("Phi",&Phi,"Phi/F");
    baobab->Branch("Particles",&Particles);

    ////Read nucleon momentum distribution of nucleon in deuteron////
    //Fermi momentum distribution
    FILE *FermiFile;

    FermiFile = fopen("FermiMomDistr/FermiMomDistr_PARIS.dat","r");     //PARIS distribution
    //FermiFile = fopen("FermiMomDistr/FermiMomDistr_CDBONN_Sk.dat","r"); //CDBonn distribution
    //FermiFile = fopen("FermiMomDistr/FermiMomDistr_CDBONN.dat","r");    //CDBonn distribution
    //FermiFile = fopen("FermiMomDistr/FermiMomDistr_chiral.dat","r");    //Chiral distribution


    TGraph *gFermiMomDistr = new TGraph();

    Float_t FermiMom;   //Fermi momentum: the first column in file
    Float_t ProbMom;    //probability: the second column in file

    Int_t i = -1;

    while (! feof(FermiFile)) {
        i++;
        fscanf(FermiFile,"%f %f\n",&FermiMom,&ProbMom);
        gFermiMomDistr->SetPoint(i,FermiMom,ProbMom);
    }

    //gFermiMomDistr->Draw("AP"); //draw AP (A-axis, P-points)

    ////pseudorandom number generator////

    TRandom3 *r = new TRandom3(0);
    r->SetSeed(0);

    //TF1 *f1=new TF1("Uniform","1",-10.,10.);  //uniform probability
    //gRandom->SetSeed(0);

/////////////////////////////////////////loop over the number of events/////////////////////////////////////////

    for(Int_t j = 0; j < nevents; ) {

        Double_t r1 = r->Rndm();
        Double_t p_beam = 1.426 + 0.209*r1;	//ramped proton beam
        //Double_t p_beam = f1->GetRandom(1.426,1.635);

        Double_t E_beam = TMath::Sqrt(m_beam*m_beam + p_beam*p_beam);  //beam energy in LAB [GeV]
        Double_t s = TMath::Sqrt(m_beam*m_beam + m_target*m_target + 2*m_target*TMath::Sqrt(m_beam*m_beam + p_beam*p_beam));    //invariant mass of the colliding particles

        ////CM frame////
        Double_t Beta_cm = (p_beam)/(m_target + TMath::Sqrt(m_beam*m_beam + p_beam*p_beam));
        Double_t Gamma_cm = 1/(TMath::Sqrt(1 - Beta_cm*Beta_cm));
        Double_t p_beam_cm = p_beam + Beta_cm*Gamma_cm*((Gamma_cm*Beta_cm*p_beam/(Gamma_cm + 1)) - E_beam); //beam momentum in CM [GeV/c], calculated correctly (checked with boost)
        Double_t E_beam_cm = Gamma_cm*(E_beam - Beta_cm*p_beam);    //beam energy in CM [GeV], calculated correctly (checked with boost)

/////////////////////////////////////////////////FOUR-VECTORS/////////////////////////////////////////////////

        TVector3 vec_beam;
        vec_beam.SetMagThetaPhi(p_beam,0.,0.);
        TLorentzVector P_b;             //4-vector of the beam
        P_b.SetVectM(vec_beam,m_beam);

        TVector3 vec_target;
        vec_target.SetMagThetaPhi(0.,0.,0.);
        TLorentzVector P_t;             //4-vector of the target
        P_t.SetVectM(vec_target,m_target);

        TLorentzVector P_tot = P_b + P_t;   //total 4-vector

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

        ////nucleon momentum distribution for neutron spectator in deuteron target (Paris distribution)////
        ////CM frame of deuteron////
        ////CM frame of deuteron target = LAB frame, because target doesn't move////

        //Double_t CosTheta_n_target_cm = f1->GetRandom(-1.,1.);
        Double_t r2 = r->Rndm();
        Double_t CosTheta_n_target_cm = 2*r2-1;   //cos(theta) for nucleon inside target random from -1 to 1 (uniform distribution)

        Double_t Theta_n_target_cm = TMath::ACos(CosTheta_n_target_cm);   //nucleon polar angle [rad]

        //Double_t Phi_n_target_cm = f1->GetRandom(0.,2.*TMath::Pi());
        Double_t r3 = r->Rndm();
        Double_t Phi_n_target_cm = TMath::Pi()*(2*r3 - 1);	//nucleon azimuthal angle random from -pi to pi (uniform distribution) [rad]
        //Double_t Phi_n_target_cm = TMath::Pi()*(2*r3);	//nucleon azimuthal angle random from 0 to 2*pi (uniform distribution) [rad]

        //Double_t pf_target = f1->GetRandom(0.,0.4);
        Double_t r4 = r->Rndm();
        Double_t pf_target = 0.4*r4;

        Double_t pfx_target = pf_target*sin(Theta_n_target_cm)*cos(Phi_n_target_cm);
        Double_t pfy_target = pf_target*sin(Theta_n_target_cm)*sin(Phi_n_target_cm);
        Double_t pfz_target = pf_target*cos(Theta_n_target_cm);

        Double_t Npf_target_max = TMath::MaxElement(gFermiMomDistr->GetN(),gFermiMomDistr->GetY());   //maximum value of function (TGraph)

        //Double_t z1_target = f1->GetRandom(0.,1.);
        Double_t z1_target = r->Rndm();
        Double_t w1_target = Npf_target_max*z1_target;            //random N

        Double_t Npf_target = gFermiMomDistr->Eval(pf_target);    //N(p_f)

        if(Npf_target > w1_target) {

            ////n_sp - spectator////
            Double_t px_n_cm = pfx_target;
            Double_t py_n_cm = pfy_target;
            Double_t pz_n_cm = pfz_target;
            Double_t p_n_cm = TMath::Sqrt(px_n_cm*px_n_cm + py_n_cm*py_n_cm + pz_n_cm*pz_n_cm);   //neutron spectator momentum

            Double_t Theta_n_cm = Theta_n_target_cm;  //neutron polar angle       [rad]
            Double_t Phi_n_cm = Phi_n_target_cm;      //neutron azimuthal angle   [rad]

            Double_t E_n_cm = TMath::Sqrt(p_n_cm*p_n_cm + m_n*m_n); //neutron energy [GeV]

            TVector3 vec_pn_cm;
            vec_pn_cm.SetMagThetaPhi(p_n_cm,Theta_n_cm,Phi_n_cm);
            TLorentzVector P_n;     //4-vector of the neutron spectator
            P_n.SetVectM(vec_pn_cm,m_n);

            ////p2 - target proton////
            Double_t px_p2_cm = -pfx_target;
            Double_t py_p2_cm = -pfy_target;
            Double_t pz_p2_cm = -pfz_target;
            Double_t p_p2_cm = TMath::Sqrt(px_p2_cm*px_p2_cm + py_p2_cm*py_p2_cm + pz_p2_cm*pz_p2_cm);

            Double_t Theta_p2_cm = (TMath::Pi()) - Theta_n_target_cm;   //target proton polar angle     [rad]
            Double_t Phi_p2_cm = Phi_n_target_cm + (TMath::Pi());       //target proton azimuthal angle [rad]

            Double_t m_p2 = TMath::Sqrt(m_d*m_d + m_n*m_n - 2*m_d*(TMath::Sqrt(m_n*m_n + pf_target*pf_target)));    //target proton mass

            Double_t E_p2_cm = TMath::Sqrt(p_p2_cm*p_p2_cm + m_p2*m_p2);

            TVector3 vec_pp2_cm;
            vec_pp2_cm.SetMagThetaPhi(p_p2_cm,Theta_p2_cm,Phi_p2_cm);
            TLorentzVector P_p2;    //4-vector of the target proton
            P_p2.SetVectM(vec_pp2_cm,m_p2);

////////////////////////////////////////////proton-proton LAB frame///////////////////////////////////////////

            TLorentzVector P_p1p2_tot = P_b + P_p2;

            TVector3 vec_tot = vec_pp2_cm + vec_pn_cm + vec_beam;
            TVector3 vec_pp_int = vec_beam;
            Double_t p_pp_int = vec_pp_int.Mag();
            Double_t p_tot = vec_tot.Mag();

            //cout<<"p_tot = "<<p_tot<<endl;

            ////s_pp in pd LAB system////
            Double_t p1_p2_lab_angle = vec_beam.Angle(vec_pp2_cm);    //angle between the momenta of protons
            Double_t s_pp = (E_p2_cm + E_beam)*(E_p2_cm + E_beam) - (p_p2_cm*p_p2_cm + p_beam*p_beam + 2*p_p2_cm*p_beam*TMath::Cos(p1_p2_lab_angle));

            //cout<<"p1_p2_lab_angle= "<<p1_p2_lab_angle<<endl;
            //cout<<"s_pp= "<<s_pp<<endl;

            ////proton beam momentum in pp LAB frame////
            Double_t part1 = (s_pp - m_p*m_p - m_p2*m_p2)/(2*m_p2);
            Double_t p_p1beam = TMath::Sqrt(part1*part1 - m_p*m_p);

            TVector3 vec_p1_beam;
            vec_p1_beam.SetMagThetaPhi(p_p1beam,0,0);
            TLorentzVector P_p1_pp_lab;     //4-vector of the beam
            P_p1_pp_lab.SetVectM(vec_p1_beam,m_p);

////////////////////////////////////////////proton-proton CM frame///////////////////////////////////////////

            TVector3 b;
            b=P_p1p2_tot.BoostVector(); //BOOST to pp CM frame (using P_b and P_p2 calculated in pd LAB)

            ////beam////
            P_b.Boost(-b);
            Double_t px_p1_pp_cm = P_b.Px();
            Double_t py_p1_pp_cm = P_b.Py();
            Double_t pz_p1_pp_cm = P_b.Pz();
            Double_t p_p1_pp_cm = TMath::Sqrt(px_p1_pp_cm*px_p1_pp_cm + py_p1_pp_cm*py_p1_pp_cm + pz_p1_pp_cm*pz_p1_pp_cm);

            Double_t E_p1_pp_cm = TMath::Sqrt(p_p1_pp_cm*p_p1_pp_cm + m_p*m_p);
            Double_t Ekin_p1_pp_cm = E_p1_pp_cm - m_p;

            Double_t Theta_p1_pp_cm = P_b.Theta();
            Double_t CosTheta_p1_pp_cm = TMath::Cos(Theta_p1_pp_cm);
            Double_t Phi_p1_pp_cm = P_b.Phi();

            TVector3 vec_pp1_pp_cm = P_b.Vect();

            ////target////
            P_p2.Boost(-b);
            Double_t px_p2_pp_cm = P_p2.Px();
            Double_t py_p2_pp_cm = P_p2.Py();
            Double_t pz_p2_pp_cm = P_p2.Pz();
            Double_t p_p2_pp_cm = TMath::Sqrt(px_p2_pp_cm*px_p2_pp_cm + py_p2_pp_cm*py_p2_pp_cm + pz_p2_pp_cm*pz_p2_pp_cm);

            Double_t E_p2_pp_cm = TMath::Sqrt(p_p2_pp_cm*p_p2_pp_cm + m_p2*m_p2);
            Double_t Ekin_p2_pp_cm = E_p2_pp_cm - m_p2;

            Double_t Theta_p2_pp_cm = P_p2.Theta();
            Double_t CosTheta_p2_pp_cm = TMath::Cos(Theta_p2_pp_cm);
            Double_t Phi_p2_pp_cm = P_p2.Phi();

            TVector3 vec_pp2_pp_cm = P_p2.Vect();

///////////////////////////////////scattering between two protons in pp CM///////////////////////////////////

            ////proton 1////
            //Double_t CosTheta_p1_cm_scatt = f1->GetRandom(-1.,1.);    //cos of the scattering angle random from -1 to 1 (uniform distribution)
            Double_t r5 = r->Rndm();
            Double_t CosTheta_p1_cm_scatt = 2*r5 - 1;			//random from -1 to 1 (uniform distribution)
            Double_t Theta_p1_cm_scatt = TMath::ACos(CosTheta_p1_cm_scatt);	//scattering angle [rad]

            //Double_t Phi_p1_cm_scatt = f1->GetRandom(0.,2.*TMath::Pi()); //azimutal angle random from 0 to 2*pi [rad]
            Double_t r6 = r->Rndm();
            Double_t Phi_p1_cm_scatt = TMath::Pi()*(2*r6 - 1);	//azimutal angle random from -pi to pi (uniform distribution) [rad]
            //Double_t Phi_p1_cm_scatt = TMath::Pi()*(2*r6);	//azimutal angle random from 0 to 2*pi (uniform distribution) [rad]

            TVector3 vec_pp1_cm_scatt;
            vec_pp1_cm_scatt.SetMagThetaPhi(p_p1_pp_cm,Theta_p1_cm_scatt,Phi_p1_cm_scatt);
            TLorentzVector P_p1_scatt;				//4-vector of the 1-st proton
            P_p1_scatt.SetVectM(vec_pp1_cm_scatt,m_p);

            ////proton 2////
            Double_t Theta_p2_cm_scatt = (TMath::Pi()) - Theta_p1_cm_scatt; //scattering angle  [rad]
            Double_t Phi_p2_cm_scatt = Phi_p1_cm_scatt + (TMath::Pi());     //azimutal angle    [rad]

            TVector3 vec_pp2_cm_scatt;
            vec_pp2_cm_scatt.SetMagThetaPhi(p_p2_pp_cm,Theta_p2_cm_scatt,Phi_p2_cm_scatt);
            TLorentzVector P_p2_scatt;				//4-vector of the 2-nd proton
            P_p2_scatt.SetVectM(vec_pp2_cm_scatt,m_p2);

///////////////////////////////////////BOOST to pd LAB frame//////////////////////////////////////////////////

            TVector3 b_pd;
            b_pd = P_p1p2_tot.BoostVector();  //boost to LAB

            ////proton 1////
            P_p1_scatt.Boost(b_pd);

            Double_t px_p1_pd_lab = P_p1_scatt.Px();
            Double_t py_p1_pd_lab = P_p1_scatt.Py();
            Double_t pz_p1_pd_lab = P_p1_scatt.Pz();
            Double_t p_p1_pd_lab = TMath::Sqrt(px_p1_pd_lab*px_p1_pd_lab + py_p1_pd_lab*py_p1_pd_lab + pz_p1_pd_lab*pz_p1_pd_lab);

            Double_t E_p1_pd_lab = TMath::Sqrt(p_p1_pd_lab*p_p1_pd_lab + m_p*m_p);
            Double_t Ekin_p1_pd_lab = E_p1_pd_lab - m_p;

            Double_t Theta_p1_pd_lab = P_p1_scatt.Theta();
            Double_t CosTheta_p1_pd_lab = TMath::Cos(Theta_p1_pd_lab);
            Double_t Phi_p1_pd_lab = P_p1_scatt.Phi();

            TVector3 vec_pp1_pd_lab = P_p1_scatt.Vect();

            ////proton 2////
            P_p2_scatt.Boost(b_pd);

            Double_t px_p2_pd_lab = P_p2_scatt.Px();
            Double_t py_p2_pd_lab = P_p2_scatt.Py();
            Double_t pz_p2_pd_lab = P_p2_scatt.Pz();
            Double_t p_p2_pd_lab = TMath::Sqrt(px_p2_pd_lab*px_p2_pd_lab + py_p2_pd_lab*py_p2_pd_lab + pz_p2_pd_lab*pz_p2_pd_lab);

            Double_t E_p2_pd_lab = TMath::Sqrt(p_p2_pd_lab*p_p2_pd_lab + m_p2*m_p2);
            Double_t Ekin_p2_pd_lab = E_p2_pd_lab - m_p2;

            Double_t Theta_p2_pd_lab = P_p2_scatt.Theta();
            Double_t CosTheta_p2_pd_lab = TMath::Cos(Theta_p2_pd_lab);
            Double_t Phi_p2_pd_lab = P_p2_scatt.Phi();

            TVector3 vec_pp2_pd_lab = P_p2_scatt.Vect();

            TVector3 vec_pp_out = vec_pp1_pd_lab + vec_pp2_pd_lab + vec_pn_cm;
            Double_t p_pp_out = vec_pp_out.Mag();

            //cout<<"p_beam_int="<<p_pp_int<<endl;
            //cout<<"p_beam_out="<<p_pp_out<<endl;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

            //OUTPUT file with effective beam momentum and scattering angle .txt
            ofstream protonfile;
            protonfile.open("ProtonVariables/ProtonVariables-PARIS-1.txt", ios::app);
            //protonfile.open("ProtonVariables/ProtonVariables-CDBONN-1.txt", ios::app);
            //protonfile.open("ProtonVariables/ProtonVariables-chiral-1.txt", ios::app);
            protonfile<<Form("%g %g",p_p1beam,Theta_p1_cm_scatt)<<endl;
            protonfile.close();

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

            ////PLUTO convention - LAB2////
            pout[0] = P_n;
            pout[1] = P_p1_scatt;
            pout[2] = P_p2_scatt;

            Npart = 3;
            Impact = 0;
            Phi = 0;

            Particles->Clear();     //we clear all objects in TClonesArray

            new ((*Particles)[0]) PParticle("n",pout[0].Px(),pout[0].Py(),pout[0].Pz(),pout[0].M());
            new ((*Particles)[1]) PParticle("p",pout[1].Px(),pout[1].Py(),pout[1].Pz(),pout[1].M());
            new ((*Particles)[2]) PParticle("p",pout[2].Px(),pout[2].Py(),pout[2].Pz(),pout[2].M());

            baobab->Fill();

            j++;
        }
    }

    baobab->Write();
    newfile->Close();
}
