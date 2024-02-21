#include "event_generator.h"
#include "random_generator.h"
#include "physics_calculator.h"
#include "constants.h"
#include <iostream>
#include "TCanvas.h"
#include "TH1F.h"
#include "TROOT.h"
#include "TFile.h"
#include "TF1.h"
#include "TGraph.h"
#include "TRandom3.h"
#include "PParticle.h"

const double proton_mass = Constants::PROTON_MASS;
const double neutron_mass = Constants::NEUTRON_MASS;
const double deuteron_mass = Constants::DEUTERON_MASS;

RandomGenerator rand_gen;

EventGenerator::EventGenerator(TGraph* graph, DataWriter& writer, const std::string& particlesFile, const std::string& calculationsFile, const std::string& protonDataFile)
: graph_(graph), writer_(writer), particlesFileName_(particlesFile), calculationsFileName_(calculationsFile), protonDataFileName_(protonDataFile) {
   //setupTree();
}

EventGenerator::~EventGenerator() 
{
   // cleanup();
}

void EventGenerator::setGraph(TGraph* graph) 
{
    this->graph_ = graph;
}

void EventGenerator::setupTree() 
{ 
    Npart_ = 3;
    Impact_ = 0;
    Phi_ = 0;

    particles_ = new TClonesArray("PParticle", Npart_);

    // Setup for particlesTree_
    particlesTree_ = new TTree("data", "Particles Tree");

    // Define branches for `particlesTree_`
    particlesTree_->Branch("Npart", &Npart_, "Npart/I");
    particlesTree_->Branch("Impact", &Impact_, "Impact/F");
    particlesTree_->Branch("Phi", &Phi_, "Phi/F");
    particlesTree_->Branch("Particles", &particles_);

    // Setup for calculationsTree_
    calculationsTree_ = new TTree("values", "Simulation Data");
    // Define branches for `calculationsTree_`
    calculationsTree_->Branch("Momentum", &momentum_, "Momentum/D");

    
}

void EventGenerator::setParticles(TClonesArray* particlesArray, const std::vector<ParticleData>& particlesData) 
{
    particlesArray->Clear();

    for (size_t i = 0; i < particlesData.size(); ++i) {
        const ParticleData& pd = particlesData[i];
        new ((*particlesArray)[i]) PParticle(pd.name.c_str(), pd.vector.Px(), pd.vector.Py(), pd.vector.Pz(), pd.vector.M());
    }
}

void EventGenerator::generateEvents(int num_events)
{ 
    setupTree();
    

    if (!particlesTree_ || !calculationsTree_ || !particles_) {
        std::cerr << "Tree or Particles array not initialized." << std::endl;
        return;
    }

    proton_data.clear();

    for (int i = 0; i < num_events; ) {
        std::vector<ParticleData> event_particles;

        /* LAB FRAME */
        Double_t beam_momentum_lab = rand_gen.generate(Constants::BEAM_MOMENTUM_MIN, Constants::BEAM_MOMENTUM_MAX);
        Double_t beam_energy_lab = PhysicsCalculator::calculateEnergy(beam_momentum_lab, proton_mass);

        

        TVector3 beam_vector;
        beam_vector.SetMagThetaPhi(beam_momentum_lab, 0., 0.);
        
        TLorentzVector beam_4vector = PhysicsCalculator::createFourVector(proton_mass, beam_momentum_lab, 0, 0);
        TLorentzVector target_4vector = PhysicsCalculator::createFourVector(deuteron_mass, 0, 0, 0);

        TLorentzVector total_4vector = beam_4vector + target_4vector;

        TLorentzVector beam_proton_4vector = PhysicsCalculator::createFourVector(proton_mass, beam_momentum_lab, 0, 0);

        Double_t inv_mass = PhysicsCalculator::calculateInvariantMass(proton_mass, deuteron_mass, beam_energy_lab); // Invariant mass of the colliding particles

        /* CM FRAME */
        Double_t beta_cm = PhysicsCalculator::calculateBetaCM(beam_momentum_lab, proton_mass, deuteron_mass);
        Double_t gamma_cm = PhysicsCalculator::calculateGammaCM(beta_cm);

        Double_t beam_momentum_cm = beam_momentum_lab + beta_cm * gamma_cm * ((gamma_cm * beta_cm * beam_momentum_lab / (gamma_cm + 1)) - beam_energy_lab);
        Double_t beam_energy_cm = gamma_cm * (beam_energy_lab - beta_cm * beam_momentum_lab);

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

            momentum_ = beam_momentum_lab;

            calculationsTree_->Fill();


            /* Neutron spectator */            
            Double_t target_neutron_px_cm = target_nucleon_px_cm;
            Double_t target_neutron_py_cm = target_nucleon_py_cm;
            Double_t target_neutron_pz_cm = target_nucleon_pz_cm;
            Double_t target_neutron_theta_cm = target_nucleon_theta_cm;
            Double_t target_neutron_phi_cm = target_nucleon_phi_cm;

            Double_t target_neutron_momentum_cm = PhysicsCalculator::calculateMomentum(target_neutron_px_cm, target_neutron_py_cm, target_neutron_pz_cm);

            TLorentzVector target_neutron_4vector = PhysicsCalculator::createFourVector(neutron_mass, target_neutron_momentum_cm, target_neutron_theta_cm, target_neutron_phi_cm);

            /* Target proton */
            Double_t target_proton_px_cm = -target_nucleon_px_cm;
            Double_t target_proton_py_cm = -target_nucleon_py_cm;
            Double_t target_proton_pz_cm = -target_nucleon_pz_cm;
            Double_t target_proton_theta_cm = TMath::Pi() - target_nucleon_theta_cm;
            Double_t target_proton_phi_cm = TMath::Pi() + target_nucleon_phi_cm;

            Double_t target_proton_momentum_cm = PhysicsCalculator::calculateMomentum(target_proton_px_cm, target_proton_py_cm, target_proton_pz_cm);

            Double_t effective_proton_mass = PhysicsCalculator::calculateEffectiveProtonMass(target_proton_momentum_cm);

            Double_t target_proton_energy_cm = PhysicsCalculator::calculateEnergy(target_proton_momentum_cm, effective_proton_mass);

            TLorentzVector target_proton_4vector = PhysicsCalculator::createFourVector(effective_proton_mass, target_proton_momentum_cm, target_proton_theta_cm, target_proton_phi_cm);

            TVector3 target_proton_vector_cm;
            target_proton_vector_cm.SetMagThetaPhi(target_proton_momentum_cm, target_proton_theta_cm, target_proton_phi_cm);

            /* Proton-proton LAB frame */
            
            TLorentzVector proton_proton_4vector = beam_proton_4vector + target_proton_4vector;
            
            Double_t proton_proton_angle = beam_vector.Angle(target_proton_vector_cm);  // [rad] - angle between the momenta of protons
            Double_t effective_proton_momentum = PhysicsCalculator::calculateEffectiveProtonMomentum(beam_energy_lab, target_proton_energy_cm, beam_momentum_lab, target_proton_momentum_cm, proton_proton_angle, effective_proton_mass);
            
            TLorentzVector beam_proton_4vector_lab = PhysicsCalculator::createFourVector(proton_mass, effective_proton_momentum, 0, 0);

            /* Proton-proton CM frame */
            TVector3 b = proton_proton_4vector.BoostVector();    // Transform to proton-proton CM frame (using 4-vectors calculated in proton-deuteron LAB frame)

            beam_proton_4vector.Boost(-b);

            Double_t beam_proton_px_pp = beam_proton_4vector.Px();
            Double_t beam_proton_py_pp = beam_proton_4vector.Py();
            Double_t beam_proton_pz_pp = beam_proton_4vector.Pz();
            Double_t beam_proton_momentum_pp = PhysicsCalculator::calculateMomentum(beam_proton_px_pp, beam_proton_py_pp, beam_proton_pz_pp);

            target_proton_4vector.Boost(-b);

            Double_t target_proton_px_pp = target_proton_4vector.Px();
            Double_t target_proton_py_pp = target_proton_4vector.Py();
            Double_t target_proton_pz_pp = target_proton_4vector.Pz();
            Double_t target_proton_momentum_pp = PhysicsCalculator::calculateMomentum(target_proton_px_pp, target_proton_py_pp, target_proton_pz_pp);

            /* Scattering between two protons in the proton-proton CM frame */
            Double_t beam_proton_cos_theta_scat_cm = rand_gen.generate(-1, 1);
            Double_t beam_proton_theta_scat_cm = TMath::ACos(beam_proton_cos_theta_scat_cm);    // [rad]
            Double_t beam_proton_phi_scat_cm = rand_gen.generate(-TMath::Pi(), TMath::Pi());    // [rad]

            TLorentzVector beam_proton_scat_4vector = PhysicsCalculator::createFourVector(proton_mass, beam_proton_momentum_pp, beam_proton_theta_scat_cm, beam_proton_phi_scat_cm);

            Double_t target_proton_theta_scat_cm = TMath::Pi() - beam_proton_theta_scat_cm;
            Double_t target_proton_phi_scat_cm = TMath::Pi() + beam_proton_phi_scat_cm;

            TLorentzVector target_proton_scat_4vector = PhysicsCalculator::createFourVector(effective_proton_mass, target_proton_momentum_pp, target_proton_theta_scat_cm, target_proton_phi_scat_cm);

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

            particlesTree_->Fill();

            i++;
        }        
    }

    writer_.writeTreeToFile(particlesTree_, particlesFileName_);
    writer_.writeTreeToFile(calculationsTree_, calculationsFileName_);
    writer_.writeProtonData(proton_data, protonDataFileName_);

    cleanup();
}

void EventGenerator::runSimulations(
    TGraph* graph, const int num_iterations, const int num_events, 
    const std::string& model_name) 
{
    DataWriter dataWriter;

    for (int iteration = 0; iteration < num_iterations; ++iteration) {
        std::cout << "Processing simulation run " << (iteration + 1) << "..." 
                  << std::endl;

        std::string pluto_file_path = 
            DataWriter::getPlutoFilePath(model_name, iteration);
        std::string data_file_path = 
            DataWriter::getDataFilePath(model_name, iteration);
        std::string proton_file_path = 
            DataWriter::getProtonFilePath(model_name, iteration);
        
        // Initialise EventGenerator with the current model's graph and file names
        EventGenerator eventGenerator(graph, dataWriter, pluto_file_path,
                                      data_file_path, proton_file_path);
        
        // Generate and process events
        eventGenerator.generateEvents(num_events);

        std::cout << "Simulation run " << (iteration + 1) << " completed." 
                  << std::endl;
        std::cout << "PLUTO file: " << pluto_file_path << std::endl;
        std::cout << "Calculated data file: " << data_file_path << std::endl;
        std::cout << "Proton data file: " << proton_file_path 
                  << std::endl << std::endl;

    }
    std::cout << "Simulation completed successfully." << std::endl;
}


void EventGenerator::cleanup() 
{    
    // Clean up particlesTree_
    if (particlesTree_ != NULL) {
        delete particlesTree_; // This also frees memory allocated for branches
        particlesTree_ = NULL;
    }

    // Clean up calculationsTree_
    if (calculationsTree_ != NULL) {
        delete calculationsTree_;
        calculationsTree_ = NULL;
    }

    // If using dynamic memory for particles_, ensure it is properly deleted
    if (particles_ != NULL) {
        delete particles_;
        particles_ = NULL;
    }
}

