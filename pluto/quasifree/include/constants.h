/**
 * @file constants.h
 * @author AK <alex.nuclearboy@gmail.com>
 * @brief Provides physical constants for nuclear physics simulations.
 * 
 * @date 2024-02-17
 * 
 * @note All constants are given in natural units, where c = 1.
 */

#ifndef CONSTANTS_H
#define CONSTANTS_H

#include "Rtypes.h"

// Namespace to encapsulate physical constants used in simulations
namespace Constants {
    static const Double_t PROTON_MASS = 0.938272;       ///< Proton mass in GeV/c^2.
    static const Double_t NEUTRON_MASS = 0.939565;      ///< Neutron mass in GeV/c^2.
    static const Double_t DEUTERON_MASS = 1.875613;     ///< Deuteron mass in GeV/c^2.
    static const Double_t HELIUM_3_MASS = 2.808391;     ///< Mass of Helium-3 in GeV/c^2.
    static const Double_t HELIUM_4_MASS = 3.7264225;    ///< Mass of Helium-4 in GeV/c^2.
    static const Double_t PI_0_MASS = 0.134977;         ///< Neutral pion (pi0) mass in GeV/c^2.
    static const Double_t ETA_MASS = 0.547862;          ///< Eta meson mass in GeV/c^2.
    static const Double_t BEAM_MOMENTUM_MIN = 1.426;    ///< Minimum beam momentum in GeV/c.
    static const Double_t BEAM_MOMENTUM_MAX = 1.635;    ///< Maximum beam momentum in GeV/c.
}

#endif // CONSTANTS_H
