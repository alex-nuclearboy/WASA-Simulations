#ifndef RANDOM_GENERATOR_H
#define RANDOM_GENERATOR_H

#include "TRandom3.h"

class RandomGenerator {
public:
    RandomGenerator(unsigned int seed = 0) : rng(seed) {}
    double generate(double min, double max) {
        return rng.Uniform(min, max);
    }

private:
    TRandom3 rng;
};

#endif // RANDOM_GENERATOR_H
