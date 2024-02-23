/**
 * @file library_manager.h
 * @author AK <alex.nuclearboy@gmail.com>
 * @brief Declaration of LibraryManager class for initialising simulation libraries.
 *
 * This class provides functionality to initialise and load the ROOT and PLUTO 
 * libraries required for running simulations. It ensures that all necessary 
 * libraries are loaded and accessible to the simulation environment.
 *
 * @version 2.0
 * @date 2024-02-23
 *
 * @note Distributed under the GNU General Public License version 3.0 (GPLv3).
 */

#ifndef LIBRARY_MANAGER_H
#define LIBRARY_MANAGER_H

#include "Rtypes.h"

/**
 * @class LibraryManager
 * @brief Manages loading of ROOT and PLUTO libraries for the simulation.
*/
class LibraryManager {
public:
    /**
     * Initialises and loads required libraries.
     *
     * Attempts to load ROOT and PLUTO libraries essential for the simulation's 
     * execution. It sets the include path for PLUTO headers, ensuring the 
     * simulation can utilise necessary resources. Outputs the status of library 
     * loading to facilitate troubleshooting.
     *
     * @return true if all libraries are successfully loaded, false otherwise.
     *
     * @note Assumes the PLUTOSYS environment variable is correctly set 
     *       to the PLUTO installation path.
     */
    static Bool_t initialiseLibraries();
};

#endif // LIBRARY_MANAGER_H
