#ifndef _GINFile_hh
#define _GINFile_hh 1
#include "TLorentzVector.h"
#include <stdio.h>

//   Class for writting input file required by WMC
//   $Id: GINFile.hh 246 2007-10-26 13:41:12Z hejny $
class GINFile {
private:
    float fPx[30], fPy[30], fPz[30];  ///< Arrays to store the x, y, and z momentum components of particles
    int fId[30];    ///< Array to store the particle ID
    FILE *fout;     ///< File pointer for the output file
    int fPart;      ///< Number of defined particles in the current event
    int fMaxPart;   ///< Maximum number of particles allowed
    int fEvent;     ///< Event counter
    int fReac;      ///< Reaction ID
    float fBeamPz;  ///< Beam momentum in the z direction
public:
    GINFile(char *conf = 0);    ///< Constructor with optional configuration file
    int AddParticle(int type, const TLorentzVector *);
    int WriteHeader(const char *, int reac, float pz, int nev);
    int WriteEvent(float );
    int Close();    ///< Close output file
    void Reset() {fPart = 0;};  ///< Reset the particle counter for the next event
};

#endif /* _GINFile_hh */
