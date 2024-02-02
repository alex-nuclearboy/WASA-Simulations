#ifndef REACTION_GENERATOR_H
#define REACTION_GENERATOR_H

#include <string>
#include <PBeamSmearing.h>
#include <TF1.h>

/**
 * Class representing the nuclear reaction simulation.
 * It encapsulates the functionality to simulate nuclear reactions
 * using the PLUTO framework.
 */

class ReactionGenerator {
    /**
     * Simulates nuclear reactions.
     *
     * @param final_products A string representing the final products of the reaction.
     * @param file_name The filename for output data.
     * @param iter The iteration number of the simulation.
     */
    
public:
    ReactionGenerator();
    ~ReactionGenerator();
    void simulate(const std::string& final_products,
                  const std::string& file_name, int iter);
                  
private:
    PBeamSmearing* smear;
    TF1* momentum_function;
    TF1* angular_function;

    static const double p_beam_lower; // Lower beam momentum boundary
    static const double p_beam_upper; // Upper beam momentum boundary
};

#endif  // REACTION_GENERATOR_H
