/**
 * @file momentum_data_loader.h
 * @author AK <alex.nuclearboy@gmail.com>
 * @brief Declaration of MomentumDataLoader class for loading momentum 
 *        distribution data.
 *
 * This class is designed to read and process data from files that contain 
 * momentum and probability values, constructing a TGraph object from this data. 
 * It is essential for simulations requiring momentum distributions, such as 
 * quasi-elastic proton-deuteron scattering and the formation and decay of the
 * bound ^3He-eta state. The expected file format is a text file where each
 * line contains a pair of numerical values: the first representing momentum and
 * the second its corresponding probability.
 *
 * @version 2.0
 * @date 2024-02-23
 *
 * @note Distributed under the GNU General Public License version 3.0 (GPLv3).
 */

#ifndef MOMENTUM_DATA_LOADER_H
#define MOMENTUM_DATA_LOADER_H

#include <string>
#include "TGraph.h"

/**
 * @class MomentumDataLoader
 * @brief Manages the loading of momentum distribution data from files.
 *
 * The MomentumDataLoader class encapsulates the process of opening specified 
 * files, reading momentum and probability values, and generating a TGraph
 * representation of this data.
 */
class MomentumDataLoader {
public:
    /**
     * Constructs a MomentumDataLoader with a specified file path.
     *
     * @param file_path Path to the file containing momentum distribution data.
     */
    explicit MomentumDataLoader(const std::string& file_path);

    /**
     * Loads the nucleon momentum distribution data for a specified potential model.
     *
     * Maps the model names to the corresponding data files and constructs
     * a TGraph object representing the momentum distribution. 
     * Currently supports 'paris' and 'cdbonn' models.
     *
     * @param model_name Name of the potential model for which to load momentum 
     *                   distribution data.
     * @return Pointer to a TGraph object containing momentum distribution data, 
     *         or NULL if an error occurs.
     * 
     * @note Data files are expected to be located in the
     *       "../momentum_distributions/" directory.
     */
    static TGraph* loadDeuteronNMD(const std::string& model_name);

    /**
     * Loads momentum distribution for a proton or an N*(1535) resonance 
     * within the ^3He nucleus.
     *
     * This method provides access to the momentum distribution data crucial for 
     * simulations of the eta-mesic ^3He nucleus.
     * It supports loading data for two specific scenarios:
     * - The momentum distribution of a proton within the ^3He nucleus.
     * - The theoretically calculated momentum distribution of the N*(1535) 
     *   resonance within the ^3He nucleus, where the ^3He is considered 
     *   as a bound resonance-deuteron system, for specific binding energies 
     *   of 0.33, 0.53, and 0.74 MeV.
     *
     * @param particle_type Specifies the type of particle ("proton" or "resonance") 
     *                      for which the momentum distribution is to be loaded.
     * @param energy Specifies the binding energy in the resonance-deuteron system 
     *               within the ^3He nucleus, relevant for resonance distributions. 
     *               The default value (5.5 MeV) is primarily set for proton 
     *               momentum distribution.
     * @return Pointer to a TGraph object containing the momentum distribution data, 
     *         or NULL if an error occurs.
     *
     * @note Data files are expected to be located in the 
     *       "../momentum_distributions/" directory.
     */

    /**
     * Loads the momentum distribution for resonance or proton within the ^3He nucleus.
     *
     * This method facilitates the loading of momentum distribution data for specific
     * particle types and energies, constructing a TGraph object with the data. It
     * supports distributions for proton momentum and resonances at specific binding
     * energies, enhancing the simulation's accuracy for studies involving the ^3He nucleus.
     *
     * @param particle_type Type of a particle (proton or resonance) for which 
     *                      to load momentum distribution.
     * @param energy Energy of the binding energy in the resonance-deuteron system
     *               within the ^3He nucleus, in MeV. Defaults to 5.5 MeV for protons.
     * @return Pointer to a TGraph object containing the momentum distribution data,
     *         or NULL if an error occurs.
     *
     * @note Data files are expected to be located in the 
     *       "../momentum_distributions/" directory.
     */
    static TGraph* loadHeliumNMD(
        const std::string& particle_type, const Double_t energy = 5.5);

private:
    std::string file_path;  ///< Path to the data file.

    /**
     * Reads data from the file and constructs a TGraph object.
     * @return Pointer to a TGraph object containing the data from the file.
     * @throws std::runtime_error if the file cannot be opened or read.
     */
    TGraph* readData();
};

#endif // MOMENTUM_DATA_LOADER_H
