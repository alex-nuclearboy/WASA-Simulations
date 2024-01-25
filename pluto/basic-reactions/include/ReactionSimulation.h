#ifndef ___REACTION_SIMULATION_H___
#define ___REACTION_SIMULATION_H___

#include <string>

class ReactionSimulation {
public:
    void simulate(const std::string& finalProducts, const std::string& filename, int iter);
};

#endif // ___REACTION_SIMULATION_H___
