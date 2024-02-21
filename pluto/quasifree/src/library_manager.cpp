#include "library_manager.h"
#include <iostream>
#include <string>
#include "TSystem.h"


bool LibraryManager::initialiseLibraries() {
    std::cout << "Initialising simulation environment..." << std::endl;

    // Attempt to load standard ROOT libraries.
    const char* common_libraries[] = {
        "libMatrix.so", "libHist.so", "libPhysics.so", "libRIO.so",
        "libTree.so", "libTreeViewer.so"
    };

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

    // Construct the path for libPluto.so using the PLUTOSYS environment variable.
    std::string lib_pluto_path = std::string(getenv("PLUTOSYS")) + 
                                 "/lib/libPluto.so";

    // Attempt to load libPluto.so using the constructed path.
    if (gSystem->Load(lib_pluto_path.c_str()) == -1) {
        std::cerr << "Unable to load the PLUTO library" << std::endl;
        return false;
    }

    // Set the include path for PLUTO headers using the PLUTOSYS path.
    std::string include_path = std::string("-I") + getenv("PLUTOSYS") + "/src";
    gSystem->SetIncludePath(include_path.c_str());
    std::cout << "Include path set to: " << include_path << std::endl;

    std::cout << "All libraries loaded successfully." << std::endl;

    // Print the list of loaded libraries for confirmation.
    TString loaded_libraries = gSystem->GetLibraries();
    std::cout << "Loaded libraries:" << std::endl << loaded_libraries.Data() 
              << std::endl << std::endl;

    return true;
}