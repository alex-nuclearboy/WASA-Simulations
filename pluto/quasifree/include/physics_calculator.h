/**
 * @file physics_calculator.h
 * @author AK <alex.nuclearboy@gmail.com>
 * @brief Declaration of PhysicsCalculator class for performing essential 
 *        physics calculations for simulations.
 *
 * This class provides static methods for various physics calculations relevant 
 * to nuclear reaction simulations, including energy, momentum, and invariant 
 * mass computations, as well as constructing four-vectors. 
 * Uses ROOT's mathematical libraries for precise and efficient calculations.
 *
 * @version 2.1
 * @date 2024-02-24
 *
 * @note Distributed under the GNU General Public License version 3.0 (GPLv3).
 *
 * @remark Within the framework of this class, the convention c = 1 is applied, 
 *         simplifying expressions through the use of natural units.
 */

#ifndef PHYSICS_CALCULATOR_H
#define PHYSICS_CALCULATOR_H

#include "Rtypes.h"
#include "TLorentzVector.h"
#include "TF1.h"

/**
 * @class PhysicsCalculator
 * @brief Provides physics calculations for particle interactions within simulations.
 */
class PhysicsCalculator {
public:
    /**
     * Calculates the total energy of a particle given its momentum and mass 
     * using the relativistic energy-momentum relation: E = sqrt(p^2 + m^2).
     *
     * @param p Momentum of particle in GeV/c.
     * @param m Mass of particle in GeV/c^2.
     * @return Total energy of particle in GeV.
     */
    static Double_t calculateEnergy(Double_t p, Double_t m);

    /**
     * Calculates the invariant mass of two-particle system, 
     * with one particle moving with momentum p and the other at rest.
     *
     * @param m1 Mass of moving particle in GeV/c^2.
     * @param m2 Mass of stationary particle in GeV/c^2.
     * @param p Momentum of moving particle in GeV/c.
     * @return Invariant mass of system in GeV/c^2.
     */
    static Double_t calculateInvariantMass(Double_t m1, Double_t m2, Double_t p);

    /**
     * Calculates the invariant mass of a two-particle system where both particles 
     * are moving. This method takes into account the energies and momenta of 
     * both particles, as well as the angle between their momentum vectors.
     *
     * @param e1 Energy of the first particle in GeV.
     * @param e2 Energy of the second particle in GeV.
     * @param p1 Momentum of the first particle in GeV/c.
     * @param p2 Momentum of the second particle in GeV/c.
     * @param angle The angle in radians between the momentum vectors of the 
     *              two particles.
     * @return Invariant mass of the two-particle system in GeV/c^2.
     */
    static Double_t calculateInvariantMass(
        Double_t e1, Double_t e2, Double_t p1, Double_t p2, Double_t angle);

    /**
     * Calculates the effective mass of proton in neutron-proton system, 
     * considering internal dynamics within deuteron.
     *
     * @param p Relative momentum between neutron and proton in GeV/c.
     * @return Effective mass of proton in GeV/c^2.
     */
    static Double_t calculateEffectiveProtonMass(Double_t p);

    /**
     * Calculates effective momentum of proton in proton-proton system for
     * quasi-elastic scattering simulations.
     *
     * @param e1 Energy of the beam proton in GeV
     * @param e2 Energy of the target proton in GeV.
     * @param p1 Momentum of the beam proton in GeV/c.
     * @param p2 Momentum of the target proton in GeV/c.
     * @param angle Relative angle between two protons in radians.
     * @param m_p_eff Effective mass of the target proton in the p-n system in GeV/c^2.
     * @return Effective momentum of proton in GeV/c.
     */
    static Double_t calculateEffectiveProtonMomentum(
        Double_t e1, Double_t e2, Double_t p1, Double_t p2, 
        Double_t angle, Double_t m_p_eff);

    /**
     * Calculates the total momentum of particle from its component vectors.
     * @param px X-component of momentum in GeV/c.
     * @param py Y-component of momentum in GeV/c.
     * @param pz Z-component of momentum in GeV/c.
     * @return Total momentum of particle in GeV/c.
     */
    static Double_t calculateMomentum(Double_t px, Double_t py, Double_t pz);

    /**
     * Calculates the beta factor of center-of-mass system for two-particle collisions.
     * @param p Momentum of the beam in GeV/c.
     * @param m1 Mass of the beam particle in GeV/c^2.
     * @param m2 Mass of the target particle in GeV/c^2.
     * @return Beta of the center-of-mass system.
     */
    static Double_t calculateBetaCM(Double_t p, Double_t m1, Double_t m2);

    /**
     * Calculates the gamma factor of the center-of-mass system.
     * @param beta Beta of the center-of-mass system.
     * @return Gamma of the center-of-mass system.
     */
    static Double_t calculateGammaCM(Double_t beta);

    /**
     * Creates a four-vector for a particle.
     * @param m Mass of the particle in GeV/c^2.
     * @param p Momentum of the particle in GeV/c, defaults to 0.
     * @param theta Polar angle in radians, defaults to 0.
     * @param phi Azimuthal angle in radians, defaults to 0.
     * @return TLorentzVector representing the particle state.
     */
    static TLorentzVector createFourVector(
        Double_t m, Double_t p = 0, Double_t theta = 0, Double_t phi = 0);

    /**
     * Constructs the Breit-Wigner distribution function for a given binding energy 
     * and width of the bound ^3He-eta state.
     *
     * This function computes the Breit-Wigner distribution, which describes the 
     * resonance structure of the bound ^3He-eta state.
     *
     * @param x Pointer to the independent variable.
     * @param par Pointer to the array of parameters, where par[0] represents 
     *            the binding energy (Bs), par[1] represents the width of the 
     *            bound state, and par[2] is used internally to pass the bound 
     *            state mass.
     * @return Breit-Wigner distribution function value for the given parameters.
     */
    static TF1 *BreitWigner(Double_t *x, Double_t *par);
};

#endif // PHYSICS_CALCULATOR_H
