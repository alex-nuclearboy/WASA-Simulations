/**
 * @file random_generator.h
 * @author AK <alex.nuclearboy@gmail.com>
 * @brief Defines the RandomGenerator class for generating random numbers
 *        using ROOT's TRandom3 class.
 *
 * Provides an interface for generating random numbers within a specified range,
 * with the ability to set a seed for reproducible outputs.
 *
 * @version 2.0
 * @date 2024-02-23
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
     * Constructs a RandomGenerator object with an optional seed.
     *
     * Initialises the TRandom3 engine. A seed of 0 uses the system clock to seed
     * the generator, providing different sequences of random numbers for each run.
     * Specifying a seed allows for reproducible sequences of random numbers.
     *
     * @param seed Optional seed for the random number generator, defaulting to 0.
     */
    explicit RandomGenerator(UInt_t seed = 0) : rng(seed) {}

    /**
     * Generates a random number within the specified range [min, max).
     *
     * @param min Lower bound of the range (inclusive).
     * @param max Upper bound of the range (exclusive).
     * @return Random double within the specified range.
     */
    Double_t generate(Double_t min, Double_t max) {
        return rng.Uniform(min, max);
    }

    /**
     * Sets the seed of the random number generator.
     *
     * This method allows the user to set a specific seed for the TRandom3
     * random number generator, enabling the reproduction of random number
     * sequences. A seed of 0 reseeds the generator using the system clock.
     *
     * @param seed Seed value to set.
     */
    void setSeed(UInt_t seed) {
        rng.SetSeed(seed);
    }

private:
    TRandom3 rng;   ///< Underlying ROOT TRandom3 random number generator.
};

#endif // RANDOM_GENERATOR_H
