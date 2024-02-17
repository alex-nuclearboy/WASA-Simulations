
#include "TLorentzVector.h"
class PhysicsCalculator {
public:
    Double_t calculateEnergy(Double_t momentum, Double_t mass);
    Double_t calculateInvariantMass(Double_t beam_mass, Double_t target_mass, Double_t beam_energy);
    Double_t calculateEffectiveProtonMass(Double_t momentum);
    Double_t calculateEffectiveProtonMomentum(Double_t beam_proton_energy, Double_t target_proton_energy, Double_t beam_proton_momentum, Double_t target_proton_momentum, Double_t angle);
    Double_t calculateMomentum(Double_t px, Double_t py, Double_t pz);
    Double_t calculateBetaCM(Double_t beam_momentum, Double_t beam_mass, Double_t target_mass);
    Double_t calculateGammaCM(Double_t beta_cm);
    TLorentzVector createFourVector(Double_t mass, Double_t momentum = 0, Double_t theta = 0, Double_t phi = 0);
};