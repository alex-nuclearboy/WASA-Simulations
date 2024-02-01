#ifndef REACTION_SIMULATION_H
#define REACTION_SIMULATION_H

#include <string>
#include <PBeamSmearing.h>
#include <TF1.h>


/**
 * Class representing the nuclear reaction simulation.
 * It encapsulates the functionality to simulate nuclear reactions
 * using the PLUTO framework.
 */
 
class ReactionSimulation {
	/**
     * Simulates nuclear reactions.
     * 
     * @param finalProducts A string representing the final products of the reaction.
     * @param filename The filename for output data.
     * @param iter The iteration number of the simulation.
     */

private:
    PBeamSmearing* smear;
    TF1* momentumFunc;
    TF1* angularFunc; 
    
public:
    ReactionSimulation();
    ~ReactionSimulation();
    void simulate(const std::string& finalProducts, 
				  const std::string& filename, int iter);
};

#endif // REACTION_SIMULATION_H
