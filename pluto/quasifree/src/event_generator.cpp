/**
 * @file event_generator.cpp
 * @author AK <alex.nuclearboy@gmail.com>
 * @brief Implementation of the EventGenerator class for quasifree proton-deuteron 
 *        scattering reaction simulations.
 *
 * @version 2.1
 * @date 2024-02-24
 *
 * @note Distributed under the GNU General Public License version 3.0 (GPLv3).
 */

#include "event_generator.h"
#include "random_generator.h"
#include "physics_calculator.h"
#include "constants.h"
#include <iostream>
#include "TROOT.h"
#include "TGraph.h"
#include "PParticle.h"

const Double_t proton_mass = Constants::PROTON_MASS;
const Double_t neutron_mass = Constants::NEUTRON_MASS;
const Double_t deuteron_mass = Constants::DEUTERON_MASS;

RandomGenerator rand_gen;

EventGenerator::EventGenerator(
    TGraph* graph, DataWriter& writer, 
    const std::string& pluto_data_file, 
    const std::string& analysis_data_file, 
    const std::string& proton_data_file)
    : graph_(graph), writer_(writer), 
    pluto_data_file_(pluto_data_file), 
    analysis_data_file_(analysis_data_file), 
    proton_data_file_(proton_data_file) {}

EventGenerator::~EventGenerator() {}

void EventGenerator::setupTree() 
{
    // Set up a tree structure for storing data on outgoing particles
    Impact_ = 0;
    Phi_ = 0;
    particles_ = new TClonesArray("PParticle", Npart_);

    particles_tree_ = new TTree("data", "Particles Tree");
    particles_tree_->Branch("Npart", &Npart_, "Npart/I");
    particles_tree_->Branch("Impact", &Impact_, "Impact/F");
    particles_tree_->Branch("Phi", &Phi_, "Phi/F");
    particles_tree_->Branch("Particles", &particles_);

    // Set up a tree structure with calculated values
    data_tree_ = new TTree("values", "Simulation Data");
    data_tree_->Branch("beam_momentum_lab", &beam_momentum_lab, 
                       "beam_momentum_lab/D");
    data_tree_->Branch("beam_momentum_cm", &beam_momentum_cm, 
                       "beam_momentum_cm/D");
    data_tree_->Branch("beam_energy_lab", &beam_energy_lab, 
                       "beam_energy_lab/D");
    data_tree_->Branch("beam_energy_cm", &beam_energy_cm, "beam_energy_cm/D");
    data_tree_->Branch("inv_mass_pd", &inv_mass_pd, "inv_mass_pd/D");
    data_tree_->Branch("target_neutron_momentum_cm", &target_neutron_momentum_cm, 
                       "target_neutron_momentum_cm/D");
    data_tree_->Branch("target_neutron_theta_cm", &target_neutron_theta_cm, 
                       "target_neutron_theta_cm/D");
    data_tree_->Branch("target_neutron_phi_cm", &target_neutron_phi_cm, 
                       "target_neutron_phi_cm/D");
    data_tree_->Branch("target_proton_momentum_cm", &target_proton_momentum_cm, 
                       "target_proton_momentum_cm/D");
    data_tree_->Branch("target_proton_theta_cm", &target_proton_theta_cm, 
                       "target_proton_theta_cm/D");
    data_tree_->Branch("target_proton_phi_cm", &target_proton_phi_cm, 
                       "target_proton_phi_cm/D");
    data_tree_->Branch("proton_proton_angle", &proton_proton_angle, 
                       "proton_proton_angle/D");
    data_tree_->Branch("inv_mass_pp", &inv_mass_pp, "inv_mass_pp/D");
    data_tree_->Branch("effective_proton_mass", &effective_proton_mass, 
                       "effective_proton_mass/D");
    data_tree_->Branch("effective_proton_momentum", &effective_proton_momentum, 
                       "effective_proton_momentum/D");
    data_tree_->Branch("beam_proton_momentum_pp", &beam_proton_momentum_pp, 
                       "beam_proton_momentum_pp/D");
    data_tree_->Branch("beam_proton_theta_scat_cm", &beam_proton_theta_scat_cm, 
                       "beam_proton_theta_scat_cm/D");
    data_tree_->Branch("beam_proton_phi_scat_cm", &beam_proton_phi_scat_cm, 
                       "beam_proton_phi_scat_cm/D");
    data_tree_->Branch("target_proton_momentum_pp", &target_proton_momentum_pp, 
                       "target_proton_momentum_pp/D");
    data_tree_->Branch("target_proton_theta_scat_cm", &target_proton_theta_scat_cm, 
                       "target_proton_theta_scat_cm/D");
    data_tree_->Branch("target_proton_phi_scat_cm", &target_proton_phi_scat_cm, 
                       "target_proton_phi_scat_cm/D");
    data_tree_->Branch("target_proton_energy_cm", &target_proton_energy_cm, 
                       "target_proton_energy_cm/D");
}

void EventGenerator::setParticles(
    TClonesArray* particles_array, 
    const std::vector<ParticleData>& particles_data) 
{
    particles_array->Clear();

    // Fill particles array with ParticleData instances
    for (size_t i = 0; i < particles_data.size(); ++i) {
        const ParticleData& pd = particles_data[i];
        new ((*particles_array)[i]) PParticle(
            pd.name.c_str(),
            pd.vector.Px(),
            pd.vector.Py(),
            pd.vector.Pz(),
            pd.vector.M()
        );
    }
}

void EventGenerator::generateEvents(Int_t num_events)
{
    setupTree();

    if (!particles_tree_ || !data_tree_ || !particles_) {
        std::cerr << "Tree or Particles array not initialized." << std::endl;
        return;
    }

    proton_data.clear();

    for (Int_t i = 0; i < num_events; ) {
        std::vector<ParticleData> event_particles;

        /* LAB frame */
        beam_momentum_lab = rand_gen.generate(
            Constants::BEAM_MOMENTUM_MIN, Constants::BEAM_MOMENTUM_MAX);
        beam_energy_lab = PhysicsCalculator::calculateEnergy(
            beam_momentum_lab, proton_mass);

        TVector3 beam_vector;
        beam_vector.SetMagThetaPhi(beam_momentum_lab, 0., 0.);

        TLorentzVector beam_4vector = PhysicsCalculator::createFourVector(
            proton_mass, beam_momentum_lab, 0, 0);
        TLorentzVector target_4vector = PhysicsCalculator::createFourVector(
            deuteron_mass, 0, 0, 0);

        TLorentzVector total_4vector = beam_4vector + target_4vector;

        TLorentzVector beam_proton_4vector = PhysicsCalculator::createFourVector(
            proton_mass, beam_momentum_lab, 0, 0);

        inv_mass_pd = PhysicsCalculator::calculateInvariantMass(  ///< Invariant mass of the colliding particles
            proton_mass, deuteron_mass, beam_momentum_lab);

        /* CM frame */
        Double_t beta_cm = PhysicsCalculator::calculateBetaCM(
            beam_momentum_lab, proton_mass, deuteron_mass);
        Double_t gamma_cm = PhysicsCalculator::calculateGammaCM(beta_cm);

        beam_momentum_cm = beam_momentum_lab + beta_cm * gamma_cm *
                                    ((gamma_cm * beta_cm * beam_momentum_lab / 
                                    (gamma_cm + 1)) - beam_energy_lab);
        beam_energy_cm = gamma_cm * (beam_energy_lab - beta_cm * 
                                  beam_momentum_lab);

        /* Deuteron CM frame */
        Double_t target_nucleon_cos_theta_cm = rand_gen.generate(-1, 1);    ///< Random cos(theta) for nucleon inside target
        Double_t target_nucleon_theta_cm = TMath::ACos(
            target_nucleon_cos_theta_cm);   ///< [rad] - polar angle of nucleon inside target
        Double_t target_nucleon_phi_cm = rand_gen.generate(0, TMath::TwoPi()); ///< [rad] - random azimuthal angle for nucleon inside target

        Double_t target_nucleon_momentum_cm = rand_gen.generate(0, 0.4);

        Double_t target_nucleon_px_cm = target_nucleon_momentum_cm * 
                                        sin(target_nucleon_theta_cm) * 
                                        cos(target_nucleon_phi_cm);
        Double_t target_nucleon_py_cm = target_nucleon_momentum_cm * 
                                        sin(target_nucleon_theta_cm) * 
                                        sin(target_nucleon_phi_cm);
        Double_t target_nucleon_pz_cm = target_nucleon_momentum_cm * 
                                        cos(target_nucleon_theta_cm);

        if (!graph_) {
            std::cerr << "Graph not set." << std::endl;
            return;
        }

        Double_t target_nucleon_mom_distr_max = TMath::MaxElement(
            graph_->GetN(), graph_->GetY());

        Double_t target_nucleon_mom_random = rand_gen.generate(0, 1) * 
                                             target_nucleon_mom_distr_max;

        Double_t target_nucleon_mom_distr = graph_->Eval(
            target_nucleon_momentum_cm);

        if (target_nucleon_mom_distr > target_nucleon_mom_random) {

            /* Neutron spectator */
            Double_t target_neutron_px_cm = target_nucleon_px_cm;
            Double_t target_neutron_py_cm = target_nucleon_py_cm;
            Double_t target_neutron_pz_cm = target_nucleon_pz_cm;
            target_neutron_theta_cm = target_nucleon_theta_cm;
            target_neutron_phi_cm = target_nucleon_phi_cm;

            target_neutron_momentum_cm = 
                PhysicsCalculator::calculateMomentum(
                    target_neutron_px_cm, 
                    target_neutron_py_cm, 
                    target_neutron_pz_cm);

            TLorentzVector target_neutron_4vector = 
                PhysicsCalculator::createFourVector(
                    neutron_mass, 
                    target_neutron_momentum_cm, 
                    target_neutron_theta_cm, 
                    target_neutron_phi_cm);

            /* Target proton */
            Double_t target_proton_px_cm = -target_nucleon_px_cm;
            Double_t target_proton_py_cm = -target_nucleon_py_cm;
            Double_t target_proton_pz_cm = -target_nucleon_pz_cm;
            target_proton_theta_cm = TMath::Pi() - 
                                              target_nucleon_theta_cm;
            target_proton_phi_cm = TMath::Pi() + target_nucleon_phi_cm;

            if (target_proton_phi_cm >= 2 * TMath::Pi()) {
                target_proton_phi_cm -= 2 * TMath::Pi();
            }

            target_proton_momentum_cm = 
                PhysicsCalculator::calculateMomentum(
                    target_proton_px_cm, 
                    target_proton_py_cm, 
                    target_proton_pz_cm);

            effective_proton_mass = 
                PhysicsCalculator::calculateEffectiveProtonMass(
                    target_proton_momentum_cm);

            target_proton_energy_cm = 
                PhysicsCalculator::calculateEnergy(
                    target_proton_momentum_cm, effective_proton_mass);

            TLorentzVector target_proton_4vector = 
                PhysicsCalculator::createFourVector(
                    effective_proton_mass, target_proton_momentum_cm, 
                    target_proton_theta_cm, target_proton_phi_cm);

            TVector3 target_proton_vector_cm;
            target_proton_vector_cm.SetMagThetaPhi(
                target_proton_momentum_cm, 
                target_proton_theta_cm, 
                target_proton_phi_cm);

            /* Proton-proton LAB frame */
            TLorentzVector proton_proton_4vector = beam_proton_4vector + 
                                                   target_proton_4vector;

            proton_proton_angle = 
                beam_vector.Angle(target_proton_vector_cm);  ///< [rad] - angle between the momenta of protons

            inv_mass_pp = PhysicsCalculator::calculateInvariantMass(
                    beam_energy_lab, target_proton_energy_cm, 
                    beam_momentum_lab, target_proton_momentum_cm, 
                    proton_proton_angle);

            effective_proton_momentum = 
                PhysicsCalculator::calculateEffectiveProtonMomentum(
                    beam_energy_lab, target_proton_energy_cm, 
                    beam_momentum_lab, target_proton_momentum_cm, 
                    proton_proton_angle, effective_proton_mass);

            TLorentzVector beam_proton_4vector_lab = 
                PhysicsCalculator::createFourVector(
                    proton_mass, effective_proton_momentum, 0, 0);

            /* Proton-proton CM frame */

            // Transform to proton-proton CM frame 
            // (using 4-vectors calculated in proton-deuteron LAB frame)
            TVector3 b = proton_proton_4vector.BoostVector();

            beam_proton_4vector.Boost(-b);

            Double_t beam_proton_px_pp = beam_proton_4vector.Px();
            Double_t beam_proton_py_pp = beam_proton_4vector.Py();
            Double_t beam_proton_pz_pp = beam_proton_4vector.Pz();
            beam_proton_momentum_pp = 
                PhysicsCalculator::calculateMomentum(
                    beam_proton_px_pp, 
                    beam_proton_py_pp, 
                    beam_proton_pz_pp);

            target_proton_4vector.Boost(-b);

            Double_t target_proton_px_pp = target_proton_4vector.Px();
            Double_t target_proton_py_pp = target_proton_4vector.Py();
            Double_t target_proton_pz_pp = target_proton_4vector.Pz();
            target_proton_momentum_pp = 
                PhysicsCalculator::calculateMomentum(
                    target_proton_px_pp, 
                    target_proton_py_pp, 
                    target_proton_pz_pp);

            /* Scattering between two protons in the proton-proton CM frame */
            Double_t beam_proton_cos_theta_scat_cm = rand_gen.generate(-1, 1);
            beam_proton_theta_scat_cm = 
                TMath::ACos(beam_proton_cos_theta_scat_cm);
            beam_proton_phi_scat_cm = rand_gen.generate(0, TMath::TwoPi());

            TLorentzVector beam_proton_scat_4vector = 
                PhysicsCalculator::createFourVector(
                    proton_mass, beam_proton_momentum_pp, 
                    beam_proton_theta_scat_cm, beam_proton_phi_scat_cm);

            target_proton_theta_scat_cm = TMath::Pi() - 
                                                   beam_proton_theta_scat_cm;
            target_proton_phi_scat_cm = TMath::Pi() + 
                                                 beam_proton_phi_scat_cm;
            if (target_proton_phi_scat_cm >= 2 * TMath::Pi()) {
                target_proton_phi_scat_cm -= 2 * TMath::Pi();
            }

            TLorentzVector target_proton_scat_4vector = 
                PhysicsCalculator::createFourVector(
                    effective_proton_mass, target_proton_momentum_pp, 
                    target_proton_theta_scat_cm, target_proton_phi_scat_cm);

            proton_data.push_back(std::make_pair(
                effective_proton_momentum, beam_proton_theta_scat_cm));

            /* Proton-deuteron LAB frame */
            TVector3 b_pd;
            b_pd = proton_proton_4vector.BoostVector();

            beam_proton_scat_4vector.Boost(b_pd);
            target_proton_scat_4vector.Boost(b_pd);

            event_particles.push_back(ParticleData(
                "n", target_neutron_4vector));
            event_particles.push_back(ParticleData(
                "p", beam_proton_scat_4vector));
            event_particles.push_back(ParticleData(
                "p", target_proton_scat_4vector));

            particles_->Clear();

            setParticles(particles_, event_particles);

            Npart_ = event_particles.size();

            particles_tree_->Fill();
            data_tree_->Fill();

            i++;
        }
    }

    writer_.writeTreeToFile(particles_tree_, pluto_data_file_);
    writer_.writeTreeToFile(data_tree_, analysis_data_file_);
    writer_.writeProtonData(proton_data, proton_data_file_);

    cleanup();
}

void EventGenerator::runSimulations(
    const std::string& model_name, TGraph* graph, 
        Int_t num_events, Int_t num_iterations)
{
    DataWriter dataWriter;

    for (Int_t iteration = 0; iteration < num_iterations; ++iteration) {
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
    if (particles_tree_ != NULL) {
        delete particles_tree_;
        particles_tree_ = NULL;
    }

    if (data_tree_ != NULL) {
        delete data_tree_;
        data_tree_ = NULL;
    }

    if (particles_ != NULL) {
        delete particles_;
        particles_ = NULL;
    }
}
