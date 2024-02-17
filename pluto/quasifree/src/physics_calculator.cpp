#include "PhysicsCalculator.h"
#include "TMath.h"

Double_t PhysicsCalculator::calculateEnergy(Double_t momentum, Double_t mass)
{
    return TMath::Sqrt(momentum * momentum + mass * mass);
}

Double_t PhysicsCalculator::calculateInvariantMass(Double_t beam_mass, Double_t target_mass, Double_t beam_energy)
{
    return TMath::Sqrt(beam_mass * beam_mass + target_mass * target_mass + 2 * target_mass * beam_energy);
}

Double_t PhysicsCalculator::calculateEffectiveProtonMass(Double_t target_proton_momentum)
{
    return TMath::Sqrt(DEUTERON_MASS * DEUTERON_MASS + NEUTRON_MASS * NEUTRON_MASS - 2 * DEUTERON_MASS * TMath::Sqrt(NEUTRON_MASS * NEUTRON_MASS + target_proton_momentum * target_proton_momentum));
}

Double_t PhysicsCalculator::calculateEffectiveProtonMomentum(Double_t beam_proton_energy, Double_t target_proton_energy, Double_t beam_proton_momentum, Double_t target_proton_momentum, Double_t angle, Double_t effective_proton_mass)
{
    Double_t protons_inv_mass = (beam_proton_energy + target_proton_energy) * (beam_proton_energy + target_proton_energy) - beam_proton_momentum * beam_proton_momentum - target_proton_momentum * target_proton_momentum - 2 * beam_proton_momentum * target_proton_momentum * TMath::Cos(angle);
    Double_t part = (protons_inv_mass - PROTON_MASS * PROTON_MASS - effective_proton_mass * effective_proton_mass) / (2 * effective_proton_mass);
    return TMath::Sqrt(part * part - PROTON_MASS * PROTON_MASS);
}

Double_t PhysicsCalculator::calculateMomentum(Double_t px, Double_t py, Double_t pz) 
{
    return TMath::Sqrt(px * px + py * py + pz * pz);
}

Double_t PhysicsCalculator::calculateBetaCM(Double_t beam_momentum, Double_t beam_mass, Double_t target_mass) 
{
    return beam_momentum / (target_mass + TMath::Sqrt(beam_mass * beam_mass + beam_momentum * beam_momentum));
}

Double_t PhysicsCalculator::calculateGammaCM(Double_t beta_cm) 
{
    return 1 / (TMath::Sqrt(1 - beta_cm * beta_cm));
}

TLorentzVector PhysicsCalculator::createFourVector(Double_t mass, Double_t momentum, Double_t theta, Double_t phi) 
{
    TVector3 vec;
    vec.SetMagThetaPhi(momentum, theta, phi);
    TLorentzVector vec4;
    vec4.SetVectM(vec, mass);
    return vec4;
}