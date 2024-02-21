#ifndef LIBRARY_MANAGER_H
#define LIBRARY_MANAGER_H


class LibraryManager {
public:
    /**
     * @brief Initialises and loads required libraries for the simulation.
     * 
     * This function attempts to load ROOT and PLUTO libraries needed for the 
     * simulation to run. It also sets the include path for PLUTO headers 
     * to ensure that the simulation can access necessary resources.
     * The function prints out all loaded libraries to provide feedback on the 
     * initialisation process.
     * 
     * @return true if all libraries are loaded successfully, false otherwise.
     * 
     * @note The function assumes that the PLUTOSYS environment variable is
     *       correctly set to the path of the PLUTO installation.
     */
    static bool initialiseLibraries();
};

#endif // LIBRARY_MANAGER_H