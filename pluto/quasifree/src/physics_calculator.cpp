/**
 * @file physics_calculator.cpp
 * @author AK <alex.nuclearboy@gmail.com>
 * @brief Implementation of the PhysicsCalculator class for various physics
 *        calculations used in simulations.
 *
 * Implements static methods of the PhysicsCalculator class, designed to perform 
 * a wide range of physics calculations essential for the simulation and analysis 
 * of nuclear and particle physics reactions. It includes methods for energy 
 * and momentum calculations, invariant mass computation, and construction of 
 * four-vectors, using the ROOT library's mathematical tools for precision.
 *
 * @version 2.0
 * @date 2024-02-23
 *
 * @note Distributed under the GNU General Public License version 3.0 (GPLv3).
 */

#include "physics_calculator.h"
#include "constants.h"
#include "TMath.h"

const Double_t m_d = Constants::DEUTERON_MASS;
const Double_t m_n = Constants::NEUTRON_MASS;
const Double_t m_p = Constants::PROTON_MASS;
const Double_t m_he3 = Constants::HELIUM_3_MASS;
const Double_t m_eta = Constants::ETA_MASS;
const Double_t p_beam_low = Constants::BEAM_MOMENTUM_MIN;
const Double_t p_beam_upp = Constants::BEAM_MOMENTUM_MAX;

Double_t PhysicsCalculator::calculateEnergy(Double_t p, Double_t m) 
{
    // Use Pythagorean theorem in energy-momentum space
    return TMath::Sqrt(p * p + m * m);
}

Double_t PhysicsCalculator::calculateInvariantMass(
    Double_t m1, Double_t m2, Double_t p)
{
    // Invariant mass for a system of two particles, where one particle (m1) 
    // is moving with momentum p, and the other (m2) is at rest
    return TMath::Sqrt(m1 * m1 + m2 * m2 + 2 * m2 * TMath::Sqrt(m1 * m1 + p * p));
}

Double_t PhysicsCalculator::calculateEffectiveProtonMass(Double_t p)
{
    // Effective mass considering neutron-proton system within the deuteron
    return TMath::Sqrt(m_d * m_d + m_n * m_n 
                       - 2 * m_d * TMath::Sqrt(m_n * m_n + p * p));
}

Double_t PhysicsCalculator::calculateEffectiveProtonMomentum(
    Double_t e1, Double_t e2, Double_t p1, Double_t p2, 
    Double_t angle, Double_t m_p_eff)
{
    // Calculate effective beam proton momentum in proton-proton frame.
    // First, calculate the invariant mass squared of the two-proton system
    Double_t inv_mass = (e1 + e2) * (e1 + e2) - p1 * p1 - p2 * p2 
                       - 2 * p1 * p2 * TMath::Cos(angle);
    // Next, compute the term inside the square root
    Double_t term = (inv_mass - m_p * m_p - m_p_eff * m_p_eff) / (2 * m_p_eff);

    // Finally, calculate and return the effective proton momentum
    return TMath::Sqrt(term * term - m_p * m_p);
}

Double_t PhysicsCalculator::calculateMomentum(
    Double_t px, Double_t py, Double_t pz)
{
    // Total momentum from its components
    return TMath::Sqrt(px * px + py * py + pz * pz);
}

Double_t PhysicsCalculator::calculateBetaCM(
    Double_t p, Double_t m1, Double_t m2)
{
    // Beta for center-of-mass frame
    return p / (m2 + TMath::Sqrt(m1 * m1 + p * p));
}

Double_t PhysicsCalculator::calculateGammaCM(Double_t beta)
{
    // Gamma for center-of-mass frame, derived from beta
    return 1 / (TMath::Sqrt(1 - beta * beta));
}

TLorentzVector PhysicsCalculator::createFourVector(
    Double_t m, Double_t p, Double_t theta, Double_t phi)
{
    // Create a TLorentzVector for a particle given its mass, momentum, and angles
    TVector3 vec;
    vec.SetMagThetaPhi(p, theta, phi);
    TLorentzVector vec4;
    vec4.SetVectM(vec, m);
    return vec4;
}

TF1* PhysicsCalculator::BreitWigner(Double_t *x, Double_t *par)
{
    // Calculate the mesic nucleus mass as the sum of eta meson and helium-3 
    // nucleus masses, subtracting the binding energy (Bs).
    Double_t mass_bs = m_eta + m_he3 - par[0];
    par[2] = mass_bs; //  Assign mass_bs to the third parameter for use in the formula.

    // Calculate the invariant mass range for the distribution.
    Double_t begin = PhysicsCalculator::calculateInvariantMass(m_p, m_d, p_beam_low);
    Double_t end = PhysicsCalculator::calculateInvariantMass(m_p, m_d, p_beam_upp);

    // Define the Breit-Wigner function using ROOT's TF1, where [2] corresponds to mass_bs.
    TF1* bw_function = new TF1("BreitWigner", 
        "[1] / (2 * TMath::Pi() * ((x[0] - [2]) * (x[0] - [2]) + [1] * [1] / 4))", 
        begin, end);

    // Set the initial parameter values for the function.
    bw_function->SetParameters(par[0], par[1], mass_bs);

    return bw_function;
}
