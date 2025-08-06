#ifndef RANDGEN_WITCLN
#define RANDGEN_WITCLN

#include <random>
#include <ctime>

/*

A wrapper class for pseudo random number generators. These operate independently of each other, to prevent interference.

*/

class RandGen {
    public:
    std::mt19937 gen;

    RandGen() {
        gen = std::mt19937(rand());
    }

    RandGen(int64_t seed) {
        gen = std::mt19937(seed);
    }

    void seed(int64_t see) {
        gen.seed(see);
    }

    // All generators are inclusive on the left but exclusive on the right
    double random() {
        return (double)(gen()) / (double)(gen.max());
    }

    double operator()() {
        return random();
    }

    int64_t randint(int64_t b) {
        return (int64_t)(random() * b);
    }

    int64_t randint(int a, int b) {
        return a + randint(b - a);
    }
};

#endif