#ifndef PHYSICS_CALCULATOR_H
#define PHYSICS_CALCULATOR_H

#include "TLorentzVector.h"

/**
 * @class PhysicsCalculator
 * @brief Provides physics calculations for particle interactions.
 */
class PhysicsCalculator {
public:
    /**
     * Calculate the energy of a particle.
     * @param p Momentum of the particle (GeV/c).
     * @param m Mass of the particle (GeV/c^2).
     * @return Energy of the particle (GeV).
     */
    static Double_t calculateEnergy(Double_t p, Double_t m);

    /**
     * Calculate the invariant mass of two particles, considering one is 
     * moving (beam) and the other is at rest (target).
     * @param m1 Mass of the moving particle (GeV/c^2).
     * @param m2 Mass of the stationary particle (GeV/c^2).
     * @param p Momentum of the moving particle (GeV/c).
     * @return Invariant mass of the system (GeV/c^2).
     */
    static Double_t calculateInvariantMass(Double_t m1, Double_t m2, Double_t p);

    /**
     * Calculate the effective mass of a proton in a proton-neutron system.
     * @param p Momentum of the target proton (GeV/c).
     * @return Effective proton mass (GeV/c^2).
     */
    static Double_t calculateEffectiveProtonMass(Double_t p);

    /**
     * Calculate the effective momentum of a proton.
     * @param e1 Energy of the beam proton (GeV).
     * @param e2 Energy of the target proton (GeV).
     * @param p1 Momentum of the beam proton (GeV/c).
     * @param p2 Momentum of the target proton (GeV/c).
     * @param angle Angle between protons (radians).
     * @param m_p_eff Effective mass of the proton (GeV/c^2).
     * @return Effective proton momentum (GeV/c).
     */
    static Double_t calculateEffectiveProtonMomentum(
        Double_t e1, Double_t e2, Double_t p1, Double_t p2, 
        Double_t angle, Double_t m_p_eff);

    /**
     * Calculate the total momentum of a particle.
     * @param px X component of momentum (GeV/c).
     * @param py Y component of momentum (GeV/c).
     * @param pz Z component of momentum (GeV/c).
     * @return Total momentum (GeV/c).
     */
    static Double_t calculateMomentum(Double_t px, Double_t py, Double_t pz);

    /**
     * Calculate the beta factor of the center-of-mass system.
     * @param p Momentum of the beam (GeV/c).
     * @param m1 Mass of the beam particle (GeV/c^2).
     * @param m2 Mass of the target particle (GeV/c^2).
     * @return Beta of the center-of-mass system.
     */
    static Double_t calculateBetaCM(Double_t p, Double_t m1, Double_t m2);

    /**
     * Calculate the gamma factor of the center-of-mass system.
     * @param beta Beta of the center-of-mass system.
     * @return Gamma of the center-of-mass system.
     */
    static Double_t calculateGammaCM(Double_t beta);

    /**
     * Create a four-vector for a particle.
     * @param m Mass of the particle (GeV/c^2).
     * @param p Momentum of the particle (GeV/c), defaults to 0.
     * @param theta Polar angle (radians), defaults to 0.
     * @param phi Azimuthal angle (radians), defaults to 0.
     * @return A TLorentzVector representing the particle.
     */
    static TLorentzVector createFourVector(
        Double_t m, Double_t p = 0, Double_t theta = 0, Double_t phi = 0);
};

#endif // PHYSICS_CALCULATOR_H
