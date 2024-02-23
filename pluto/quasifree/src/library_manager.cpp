/**
 * @file library_manager.cpp
 * @author AK <alex.nuclearboy@gmail.com>
 * @brief Implementation of LibraryManager class methods.
 *
 * Implements methods for initialising the simulation environment by loading
 * necessary ROOT and PLUTO libraries. It sets the include path for PLUTO
 * headers, ensuring that the simulation can access required resources.
 *
 * @version 2.0
 * @date 2024-02-23
 *
 * @note Distributed under the GNU General Public License version 3.0 (GPLv3).
 */

#include "library_manager.h"
#include <iostream>
#include <string>
#include <cstdlib>  // For getenv()
#include "TSystem.h"

Bool_t LibraryManager::initialiseLibraries() 
{
    std::cout << "Initialising simulation environment..." << std::endl;

    // Array of standard ROOT library names required for the simulation.
    // These libraries provide fundamental functionalities.
    const char* common_libraries[] = {
        "libMatrix.so", "libHist.so", "libPhysics.so",
        "libRIO.so", "libTree.so", "libTreeViewer.so"
    };

    // Attempt to load each of the standard ROOT libraries.
    for (size_t i = 0; 
        i < sizeof(common_libraries) / sizeof(common_libraries[0]); 
        ++i) 
    {
        if (gSystem->Load(common_libraries[i]) == -1) {
            std::cerr << "Unable to load library: " << common_libraries[i] 
                      << std::endl;
            return false;
        }
    }

    // The PLUTO library, essential for specialised simulation functionalities,
    // is specified using the PLUTOSYS environment variable.
    std::string lib_pluto_path = std::string(getenv("PLUTOSYS")) + 
                                 "/lib/libPluto.so";
    if (gSystem->Load(lib_pluto_path.c_str()) == -1) {
        std::cerr << "Unable to load the PLUTO library. Ensure the PLUTOSYS "
                  << "environment variable is set to the correct path of the "
                  << "PLUTO installation." << std::endl;
        return false;
    }

    // Configures the include path for PLUTO headers to enable the use of 
    // PLUTO's advanced features, such as particle generation and decay models.
    std::string include_path = std::string("-I") + getenv("PLUTOSYS") + "/src";
    gSystem->SetIncludePath(include_path.c_str());
    std::cout << "Include path for PLUTO headers set to: " << include_path 
              << std::endl;

    std::cout << "All libraries loaded successfully. "
              << "Simulation environment is ready." << std::endl;

    // Print the list of loaded libraries.
    TString loaded_libraries = gSystem->GetLibraries();
    std::cout << "Loaded libraries include:" << std::endl << loaded_libraries.Data() 
              << std::endl << std::endl;

    return true;
}
