/**
 * @file event_generator.h
 * @author AK <alex.nuclearboy@gmail.com>
 * @brief Declaration of the EventGenerator class for quasifree proton-deuteron 
 *        scattering reaction simulations.
 *
 * The EventGenerator class is central to the simulation of quasifree 
 * proton-deuteron scattering reactions, encapsulating the logic for generating 
 * and managing simulation events. The class is designed to work with PLUTO 
 * event generator framework, as well as ROOT classes for data storage and analysis.
 *
 * This class provides functionalities to:
 * - Set up and manage ROOT TTree structures for storing simulated event data.
 * - Generate events based on configurable parameters, including the beam 
 *   momentum range and target nucleon momentum distribution, taking into account 
 *   the kinematics of the proton-deuteron scattering process in both 
 *   the laboratory and center-of-mass frames.
 * - Calculate and store information about each event.
 * - Output simulation data for further analysis and visualization.
 *
 * @remark This class requires the ROOT and PLUTO frameworks for its operation.
 *
 * @version 2.0
 * @date 2024-02-23
 *
 * @note Distributed under the GNU General Public License version 3.0 (GPLv3).
 */

#ifndef EVENT_GENERATOR_H
#define EVENT_GENERATOR_H

#include "data_writer.h"
#include <string>
#include <vector>
#include "Rtypes.h"
#include "TGraph.h"
#include "TTree.h"
#include "TClonesArray.h"
#include "TLorentzVector.h"

/**
 * @struct ParticleData
 * @brief Encapsulates data for a single particle in the simulation.
 *
 * This structure includes the particle's name and its four-momentum vector,
 * using ROOT's TLorentzVector class for detailed representation of the
 * particle's state in terms of energy and momentum. This design facilitates
 * easy creation and manipulation of particle data within the event generator.
*/
struct ParticleData {
    std::string name;   ///< Name of the particle.
    TLorentzVector vector;  ///< Particle's four-momentum vector (momentum and energy).
    ParticleData(const std::string& n, const TLorentzVector& v)
        : name(n), vector(v) {}
};

/**
 * @class EventGenerator
 * @brief Manages the generation of simulation events for the quasi-elastic 
 *        proton-deuteron scattering reaction.
*/
class EventGenerator {
public:
    /**
     * Constructs an EventGenerator instance.
     *
     * Initialises the EventGenerator with necessary components for generating
     * and writing simulation events, integrating closely with both the ROOT
     * and PLUTO frameworks for event generation.
     *
     * @param graph Pointer to a TGraph object containing the momentum 
     *              distribution data necessary for event generation.
     * @param writer Reference to a DataWriter object used for outputting the 
     *               generated event data to files.
     * @param pluto_data_file Path to the file for storing particle data.
     * @param analysis_data_file Path to the file where various calculated 
     *                           values should be stored.
     * @param proton_data_file Path for saving proton data, including
     *        effective momentum and scattering angles.
     */
    EventGenerator(
        TGraph* graph, DataWriter& writer, 
        const std::string& pluto_data_file, 
        const std::string& analysis_data_file, 
        const std::string& proton_data_file);

    /**
     * Destructs an EventGenerator instance
     */
    ~EventGenerator();

    /**
     * Generates a specified number of simulation events for the quasi-elastic 
     * proton-deuteron scattering reaction.
     *
     * @param num_events Number of events to generate. Defaults to 1000.
     */
    void generateEvents(Int_t num_events = 1000);

    /**
    * Manages the simulation runs for a specific potential model.
    *
    * Manages execution of multiple simulation iterations, handling both the
    * generation of event data and its output to designated files, thus using 
    * the capabilities of the EventGenerator and DataWriter classes.
    *
    * @param model_name Name of the nucleon-nucleon potential model used 
    *                   in the simulation.
    * @param graph Pointer to a TGraph object containing the nucleon momentum 
    *              distribution data for the specified model.
    * @param num_events Number of events generated per each simulation iteration.
    *                   Defaults to 1000.
    * @param num_iterations Total number of iterations to run. Defaults to 1.
    */
    static void runSimulations(
        const std::string& model_name, TGraph* graph, 
        Int_t num_events = 1000, Int_t num_iterations = 1);

private:
    void setupTree();   ///< Initialises tree structures for data storage.
    void cleanup();     ///< Frees allocated resources.

    /**
     * Sets the particle data for the current simulation event.
     *
     * This method fills a TClonesArray with ParticleData instances, each 
     * representing a particle's state in the simulation.
     *
     * @param particles_array Pointer to a TClonesArray that should be filled 
     *                        with particles.
     * @param particles_data Vector of ParticleData structs, each containing 
     *        the particle's name and its four-vector.
     */
    void setParticles(
        TClonesArray* particles_array, 
        const std::vector<ParticleData>& particles_data);

    DataWriter& writer_;   ///< Manages output of simulation data.

    std::vector<std::pair<Double_t, Double_t> > proton_data;  ///< Stores proton data (momentum and scattering angle).

    std::string pluto_data_file_;
    std::string analysis_data_file_;
    std::string proton_data_file_;

    TGraph* graph_;

    TTree* particles_tree_;    ///< Stores data about the outgoing particles.
    Int_t   Npart_;    ///< Number of outgoing particles per event.
    Float_t Impact_;
    Float_t Phi_;
    TClonesArray* particles_;    ///< Array of the outgoing particles per event.

    TTree* data_tree_;    ///< Stores calculated event data.
    Double_t beam_momentum_lab, beam_energy_lab;
    Double_t beam_momentum_cm, beam_energy_cm;
    Double_t inv_mass;
    Double_t target_neutron_momentum_cm, target_proton_momentum_cm;
    Double_t target_neutron_theta_cm, target_neutron_phi_cm;
    Double_t target_proton_theta_cm, target_proton_phi_cm;
    Double_t effective_proton_mass;
    Double_t target_proton_energy_cm;
    Double_t effective_proton_momentum;
    Double_t proton_proton_angle;
    Double_t inv_mass_pp;
    Double_t beam_proton_momentum_pp, target_proton_momentum_pp;
    Double_t beam_proton_theta_scat_cm, beam_proton_phi_scat_cm;
    Double_t target_proton_theta_scat_cm, target_proton_phi_scat_cm;
    Double_t test;

};

#endif // EVENT_GENERATOR_H
