/**
 * @file random_generator.h
 * @author AK <alex.nuclearboy@gmail.com>
 * @brief Defines the RandomGenerator class for generating random numbers.
 *
 * This class encapsulates ROOT's TRandom3 to provide an easy-to-use interface
 * for generating random numbers within a specific range. It supports setting
 * a seed for reproducibility.
 *
 * @version 1.1
 * @date 2024-02-21
 * 
 * @note Distributed under the GNU General Public License version 3.0 (GPLv3).
 */

#ifndef RANDOM_GENERATOR_H
#define RANDOM_GENERATOR_H

#include "TRandom3.h"
#include "Rtypes.h"

class RandomGenerator {
public:
    /**
     * Constructor that initializes the random number generator with an optional seed.
     * @param seed An unsigned integer used to seed the generator. A seed of 0
     *        (default) initializes the generator based on the system clock.
     */
    RandomGenerator(UInt_t seed = 0) : rng(seed) {}

    /**
     * Generates a random number within the specified range [min, max).
     * @param min The lower bound of the range (inclusive).
     * @param max The upper bound of the range (exclusive).
     * @return A random double within the specified range.
     */
    Double_t generate(Double_t min, Double_t max) {
        return rng.Uniform(min, max);
    }

private:
    TRandom3 rng;   ///< The underlying ROOT TRandom3 random number generator.
};

#endif // RANDOM_GENERATOR_H
