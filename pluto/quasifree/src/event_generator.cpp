#include "event_generator.h"
#include "random_generator.h"
#include <iostream>
#include "TCanvas.h"
#include "TH1F.h"
#include "TROOT.h"
#include "TFile.h"
#include "TF1.h"
#include "TGraph.h"
#include "TRandom3.h"
#include "PParticle.h"

const double EventGenerator::PROTON_MASS = 0.938272;    // [GeV/c^2]
const double EventGenerator::NEUTRON_MASS = 0.939565;   // [GeV/c^2]
const double EventGenerator::DEUTERON_MASS = 1.875613;  // [GeV/c^2]
const double EventGenerator::BEAM_MOMENTUM_MIN = 1.426; // [GeV/c]
const double EventGenerator::BEAM_MOMENTUM_MAX = 1.635; // [GeV/c]

RandomGenerator rand_gen;

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

    Npart_ = 3;
    Impact_ = 0;
    Phi_ = 0;

    particles_ = new TClonesArray("PParticle", 3);

    tree_ = new TTree("data", "Simulation Data");
    tree_->Branch("Npart",&Npart_,"Npart/I");
    tree_->Branch("Impact",&Impact_,"Impact/F");
    tree_->Branch("Phi",&Phi_,"Phi/F");
    tree_->Branch("Particles", &particles_);
}

Double_t EventGenerator::calculateEnergy(Double_t momentum, Double_t mass) 
{
    return sqrt(pow(momentum, 2) + pow(mass, 2));
}

Double_t EventGenerator::calculateEffectiveProtonMass(Double_t momentum) 
{
    return sqrt(pow(DEUTERON_MASS, 2) + pow(NEUTRON_MASS, 2) - 2 * DEUTERON_MASS * sqrt(pow(NEUTRON_MASS, 2) + pow(momentum, 2)));
}

Double_t EventGenerator::calculateEffectiveProtonMomentum(
    Double_t beam_energy, Double_t target_proton_energy, Double_t beam_momentum, Double_t target_proton_momentum, Double_t angle) 
{
    Double_t protons_inv_mass = pow(beam_energy + target_proton_energy, 2) - pow(beam_momentum, 2) - pow(target_proton_momentum, 2) - 2 * beam_momentum * target_proton_momentum * TMath::Cos(angle);
    Double_t part = (protons_inv_mass - pow(PROTON_MASS, 2) - pow(calculateEffectiveProtonMass(target_proton_momentum), 2)) / (2 * calculateEffectiveProtonMass(target_proton_momentum));
    return sqrt(pow(part, 2) - pow(PROTON_MASS, 2));
}

Double_t EventGenerator::calculateMomentum(Double_t px, Double_t py, Double_t pz) 
{
    return sqrt(pow(px, 2) + pow(py, 2) + pow(pz, 2));
}

Double_t EventGenerator::calculateInvariantMass(Double_t beam_mass, Double_t target_mass, Double_t beam_energy) 
{
    return sqrt(pow(beam_mass, 2) + pow(target_mass, 2) + 2 * target_mass * beam_energy);
}

Double_t EventGenerator::calculateBetaCM(Double_t beam_momentum, Double_t beam_mass, Double_t target_mass) 
{
    return beam_momentum / (target_mass + sqrt(pow(beam_mass, 2) + pow(beam_momentum, 2)));
}

Double_t EventGenerator::calculateGammaCM(Double_t betaCM) {
    return 1 / (sqrt(1 - betaCM * betaCM));
}

TLorentzVector EventGenerator::createFourVector(double mass, double momentum, double theta, double phi) 
{
    TVector3 vec;
    vec.SetMagThetaPhi(momentum, theta, phi);
    TLorentzVector vec4;
    vec4.SetVectM(vec, mass);
    return vec4;
}

void EventGenerator::setParticles(TClonesArray* particlesArray, const std::vector<ParticleData>& particlesData) {
    particlesArray->Clear();

    for (size_t i = 0; i < particlesData.size(); ++i) {
        const ParticleData& pd = particlesData[i];
        new ((*particlesArray)[i]) PParticle(pd.name.c_str(), pd.vector.Px(), pd.vector.Py(), pd.vector.Pz(), pd.vector.M());
    }
}

void EventGenerator::generateEvents(int num_events)
{
    if (!tree_ || !particles_) {
        std::cerr << "Tree or Particles array not initialized." << std::endl;
        return;
    }

    proton_data.clear();

    for (int i = 0; i < num_events; ) {
        std::vector<ParticleData> event_particles;

        /* LAB FRAME */
        Double_t beam_momentum_lab = rand_gen.generate(BEAM_MOMENTUM_MIN, BEAM_MOMENTUM_MAX);
        Double_t beam_energy_lab = calculateEnergy(beam_momentum_lab, PROTON_MASS);

        TVector3 beam_vector;
        beam_vector.SetMagThetaPhi(beam_momentum_lab, 0., 0.);
        
        TLorentzVector beam_4vector = createFourVector(PROTON_MASS, beam_momentum_lab, 0, 0);
        TLorentzVector target_4vector = createFourVector(DEUTERON_MASS, 0, 0, 0);

        //TLorentzVector total_4vector = beam_4vector + target_4vector;

        TLorentzVector beam_proton_4vector = createFourVector(PROTON_MASS, beam_momentum_lab, 0, 0);

        //Double_t inv_mass = calculateInvariantMass(PROTON_MASS, DEUTERON_MASS, beam_energy_lab); // Invariant mass of the colliding particles

        /* CM FRAME */
        //Double_t beta_cm = calculateBetaCM(beam_momentum_lab, PROTON_MASS, DEUTERON_MASS);
        //Double_t gamma_cm = calculateGammaCM(beta_cm);

        //Double_t beam_momentum_cm = beam_momentum_lab + beta_cm * gamma_cm * ((gamma_cm * beta_cm * beam_momentum_lab / (gamma_cm + 1)) - beam_energy_lab);
        //Double_t beam_energy_cm = gamma_cm * (beam_energy_lab - beta_cm * beam_momentum_lab);

        /* Deuteron CM frame */
        Double_t target_nucleon_cos_theta_cm = rand_gen.generate(-1, 1);        // Random cos(theta) for nucleon inside target
        Double_t target_nucleon_theta_cm = TMath::ACos(target_nucleon_cos_theta_cm);   // [rad] - polar angle of nucleon inside target
        Double_t target_nucleon_phi_cm = rand_gen.generate(-TMath::Pi(), TMath::Pi()); // [rad] - random azimuthal angle for nucleon inside target

        Double_t target_nucleon_momentum_cm = rand_gen.generate(0, 0.4);
        
        Double_t target_nucleon_px_cm = target_nucleon_momentum_cm * sin(target_nucleon_theta_cm) * cos(target_nucleon_phi_cm);
        Double_t target_nucleon_py_cm = target_nucleon_momentum_cm * sin(target_nucleon_theta_cm) * sin(target_nucleon_phi_cm);
        Double_t target_nucleon_pz_cm = target_nucleon_momentum_cm * cos(target_nucleon_theta_cm);

        if (!graph_) {
            std::cerr << "Graph not set." << std::endl;
            return;
        }

        Double_t target_nucleon_mom_distr_max = TMath::MaxElement(graph_->GetN(), graph_->GetY());

        Double_t target_nucleon_mom_random = rand_gen.generate(0, 1) * target_nucleon_mom_distr_max;

        Double_t target_nucleon_mom_distr = graph_->Eval(target_nucleon_momentum_cm);

        if (target_nucleon_mom_distr > target_nucleon_mom_random) {

            /* Neutron spectator */            
            Double_t target_neutron_px_cm = target_nucleon_px_cm;
            Double_t target_neutron_py_cm = target_nucleon_py_cm;
            Double_t target_neutron_pz_cm = target_nucleon_pz_cm;
            Double_t target_neutron_theta_cm = target_nucleon_theta_cm;
            Double_t target_neutron_phi_cm = target_nucleon_phi_cm;

            Double_t target_neutron_momentum_cm = calculateMomentum(target_neutron_px_cm, target_neutron_py_cm, target_neutron_pz_cm);

            TLorentzVector target_neutron_4vector = createFourVector(NEUTRON_MASS, target_neutron_momentum_cm, target_neutron_theta_cm, target_neutron_phi_cm);

            /* Target proton */
            Double_t target_proton_px_cm = -target_nucleon_px_cm;
            Double_t target_proton_py_cm = -target_nucleon_py_cm;
            Double_t target_proton_pz_cm = -target_nucleon_pz_cm;
            Double_t target_proton_theta_cm = TMath::Pi() - target_nucleon_theta_cm;
            Double_t target_proton_phi_cm = TMath::Pi() + target_nucleon_phi_cm;

            Double_t target_proton_momentum_cm = calculateMomentum(target_proton_px_cm, target_proton_py_cm, target_proton_pz_cm);

            Double_t effective_proton_mass = calculateEffectiveProtonMass(target_proton_momentum_cm);

            Double_t target_proton_energy_cm = calculateEnergy(target_proton_momentum_cm, effective_proton_mass);

            TLorentzVector target_proton_4vector = createFourVector(effective_proton_mass, target_proton_momentum_cm, target_proton_theta_cm, target_proton_phi_cm);

            TVector3 target_proton_vector_cm;
            target_proton_vector_cm.SetMagThetaPhi(target_proton_momentum_cm, target_proton_theta_cm, target_proton_phi_cm);

            /* Proton-proton LAB frame */
            
            TLorentzVector proton_proton_4vector = beam_proton_4vector + target_proton_4vector;
            
            Double_t proton_proton_angle = beam_vector.Angle(target_proton_vector_cm);  // [rad] - angle between the momenta of protons
            Double_t effective_proton_momentum = calculateEffectiveProtonMomentum(beam_energy_lab, target_proton_energy_cm, beam_momentum_lab, target_proton_momentum_cm, proton_proton_angle);
            
            //TLorentzVector beam_proton_4vector_lab = createFourVector(PROTON_MASS, effective_proton_momentum, 0, 0);

            /* Proton-proton CM frame */
            TVector3 b = proton_proton_4vector.BoostVector();    // Transform to proton-proton CM frame (using 4-vectors calculated in proton-deuteron LAB frame)

            beam_proton_4vector.Boost(-b);

            Double_t beam_proton_px_pp = beam_proton_4vector.Px();
            Double_t beam_proton_py_pp = beam_proton_4vector.Py();
            Double_t beam_proton_pz_pp = beam_proton_4vector.Pz();
            Double_t beam_proton_momentum_pp = calculateMomentum(beam_proton_px_pp, beam_proton_py_pp, beam_proton_pz_pp);

            target_proton_4vector.Boost(-b);

            Double_t target_proton_px_pp = target_proton_4vector.Px();
            Double_t target_proton_py_pp = target_proton_4vector.Py();
            Double_t target_proton_pz_pp = target_proton_4vector.Pz();
            Double_t target_proton_momentum_pp = calculateMomentum(target_proton_px_pp, target_proton_py_pp, target_proton_pz_pp);

            /* Scattering between two protons in the proton-proton CM frame */
            Double_t beam_proton_cos_theta_scat_cm = rand_gen.generate(-1, 1);
            Double_t beam_proton_theta_scat_cm = TMath::ACos(beam_proton_cos_theta_scat_cm);    // [rad]
            Double_t beam_proton_phi_scat_cm = rand_gen.generate(-TMath::Pi(), TMath::Pi());    // [rad]

            TLorentzVector beam_proton_scat_4vector = createFourVector(PROTON_MASS, beam_proton_momentum_pp, beam_proton_theta_scat_cm, beam_proton_phi_scat_cm);

            Double_t target_proton_theta_scat_cm = TMath::Pi() - beam_proton_theta_scat_cm;
            Double_t target_proton_phi_scat_cm = TMath::Pi() + beam_proton_phi_scat_cm;

            TLorentzVector target_proton_scat_4vector = createFourVector(effective_proton_mass, target_proton_momentum_pp, target_proton_theta_scat_cm, target_proton_phi_scat_cm);

            proton_data.push_back(std::make_pair(effective_proton_momentum, beam_proton_theta_scat_cm));

            /* Proton-deuteron LAB frame */
            TVector3 b_pd;
            b_pd = proton_proton_4vector.BoostVector();

            beam_proton_scat_4vector.Boost(b_pd);
            target_proton_scat_4vector.Boost(b_pd);

            event_particles.push_back(ParticleData("n", target_neutron_4vector));
            event_particles.push_back(ParticleData("p", beam_proton_scat_4vector));
            event_particles.push_back(ParticleData("p", target_proton_scat_4vector));

            particles_->Clear();

            setParticles(particles_, event_particles);

            tree_->Fill();

            i++;
        }        
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
