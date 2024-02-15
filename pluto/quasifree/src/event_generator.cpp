#include "event_generator.h"
#include <iostream>
#include "TCanvas.h"
#include "TH1F.h"
#include "TROOT.h"
#include "TFile.h"
#include "TF1.h"
#include "TGraph.h"
#include "TRandom.h"
#include "PParticle.h"

const double EventGenerator::PROTON_MASS = 0.938272;
const double EventGenerator::NEUTRON_MASS = 0.939565;
const double EventGenerator::DEUTERON_MASS = 1.875613;

EventGenerator::EventGenerator(const std::string& file_name) : file_name_(file_name), tree_(NULL), particles_(NULL), graph_(NULL)
{
    setupTree();
}

EventGenerator::~EventGenerator() 
{
    cleanup();
}

void EventGenerator::setGraph(TGraph* graph) 
{
    this->graph_ = graph;
}

void EventGenerator::calculate() 
{
    if (!graph_) {
        std::cerr << "Graph not set." << std::endl;
        return;
    }

    int n = graph_->GetN();
    double x, y;
    calculated_data_.clear();
    for (int i = 0; i < n; ++i) {
        graph_->GetPoint(i, x, y);
        double new_momentum = x + 0.01; // Example calculation
        double new_probability = y; // Example new probability calculation
        calculated_data_.push_back(std::make_pair(new_momentum, new_probability));
    }
}

void EventGenerator::setupTree() 
{
    TFile* file = new TFile(file_name_.c_str(), "RECREATE");
    if (!file || !file->IsOpen()) {
        std::cerr << "Failed to open file: " << file_name_ << std::endl;
        return;
    }

    Npart_ = 2;
    Impact_ = 0;
    Phi_ = 0;

    particles_ = new TClonesArray("PParticle", 2);

    tree_ = new TTree("data", "Simulation Data");
    tree_->Branch("Npart",&Npart_,"Npart/I");
    tree_->Branch("Impact",&Impact_,"Impact/F");
    tree_->Branch("Phi",&Phi_,"Phi/F");
    tree_->Branch("Particles", &particles_);
}

void EventGenerator::generateEvents(int num_events)
{
    if (!tree_ || !particles_) {
        std::cerr << "Tree or Particles array not initialized." << std::endl;
        return;
    }


    TF1 *f1=new TF1("Uniform","1",-10.,10.);  //uniform probability
    gRandom->SetSeed(0);

    for (int i = 0; i < num_events; ++i) {
        Double_t beam_momentum = f1->GetRandom(1.426,1.635);

        TVector3 vec_beam;
        vec_beam.SetMagThetaPhi(beam_momentum,0.,0.);
        TLorentzVector P_b;
        P_b.SetVectM(vec_beam,PROTON_MASS);

        TVector3 vec_target;
        vec_target.SetMagThetaPhi(0.,0.,0.);
        TLorentzVector P_t;
        P_t.SetVectM(vec_target,DEUTERON_MASS);

        finalStateParticles[0] = P_b;
        finalStateParticles[1] = P_t;

        particles_->Clear();

        new ((*particles_)[0]) PParticle("p",finalStateParticles[0].Px(),finalStateParticles[0].Py(),finalStateParticles[0].Pz(),finalStateParticles[0].M());
        new ((*particles_)[1]) PParticle("d",finalStateParticles[1].Px(),finalStateParticles[1].Py(),finalStateParticles[1].Pz(),finalStateParticles[1].M());

        tree_->Fill();

    }

    TFile* file = tree_->GetCurrentFile(); // Get the file associated with the TTree
    if (file) {
        file->Write();
        file->Close(); // This also deletes the TTree
    }
    
}

void EventGenerator::cleanup() 
{    
    if (particles_) {
        delete particles_;
        particles_ = NULL;
    }
}




std::vector<std::pair<double, double> > EventGenerator::getCalculatedData() const {
    return calculated_data_;
}
