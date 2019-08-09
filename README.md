# Monte Carlo Simulations

There are sources of my software for simulation of the proton-deteron fusion reactions. 

All files are distributed under the terms of the GNU General Public Licence Version 3.

## Simulation Software

### ROOT 
ROOT is an object-oriented framework for large scale data analyses based on C++.
It was developed for experiments at CERN, in which an impressive amount of data has to be processed. It was originally designed for particle physics data analysis.
ROOT has an extensive library with modules in physics, mathematics, statistics, histograms and graphics. It contains statistics tools, which can be used for data analysis.
 
### PLUTO
PLUTO is a simulation framework for heavy ions and hadronic physics based on ROOT environment. It was initially developed by the HADES collaboration in GSI. 
It adds a library of C++ classes providing an easy way to simulate dierent reactions in particle physics. 
The package includes models for resonance and Dalitz decays, resonance spectral functions with mass-dependent widths, and anisotropic angular distributions for selected channels.
The output file of a PLUTO simulation has a ROOT format. It contains the four-vectors of all final state particles as well as vertices where the decays take place. 
The PLUTO simulation doesn't take into account any detector effects.
The output of a PLUTO simulation is used as an input for the WASA Monte Carlo simulation program.

### WASA Monte Carlo (WMC)
The WMC software is based on GEANT3 package developed in CERN. This program is used to determine detector response to particles generated from PLUTO. 
Based on physical models, it is posible to  obtain the signals from different detector elements.

## Needed environment variables:

path where ROOT is installed

    ROOTSYS

path where PLUTO is instaled

    PLUTOSYS

path where WMC is installed

    WASA_ROOT

path where resulting files after running simulations are stored

    PLUTO_OUTPUT

path where the resulting data after running WMC are stored

    WMC_DATA


## Running simulation

First open ROOT environment:

    root rootlogon.C

Then load SimulationMacro.C and execute it with the generated root file:

    .L eventgenerator.C+
    .x eventgenerator.C+

Then run WMC:

    ./runWMC.sh <reaction>

