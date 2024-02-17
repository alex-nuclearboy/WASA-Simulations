/**
 * @file constants.h
 * @author AK <alex.nuclearboy@gmail.com>
 * @brief Provides physical constants for nuclear physics simulations.
 * 
 * @date 2024-02-17
 */

#ifndef CONSTANTS_H
#define CONSTANTS_H

#include "Rtypes.h"

// Namespace to encapsulate physical constants used in simulations
namespace Constants {
    static const Double_t PROTON_MASS = 0.938272;    ///< Proton mass in GeV/c^2.
    static const Double_t NEUTRON_MASS = 0.939565;   ///< Neutron mass in GeV/c^2.
    static const Double_t DEUTERON_MASS = 1.875613;  ///< Deuteron mass in GeV/c^2.
    static const Double_t BEAM_MOMENTUM_MIN = 1.426; ///< Minimum beam momentum in GeV/c.
    static const Double_t BEAM_MOMENTUM_MAX = 1.635; ///< Maximum beam momentum in GeV/c.
}

#endif // CONSTANTS_H
