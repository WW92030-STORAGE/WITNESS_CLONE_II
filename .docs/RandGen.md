# RandGen/RandGen

`class RandGen`

## Description

Pseudo-random number generators that wrap Mersenne twisters from `<random>`.

## Notes

- If you need the raw PRNG for use cases such as `std::shuffle` you can simply use the `gen` instance variable.

## Fields

- `gen: std::mt19937` - The pseudorandom number generator that is used here.

## Constructors

- `RandGen()` - Seeds with the output of `rand()`
- `RandGen(int64_t seed)` - Resets the seed

## Functions

- `void seed(int64_t see)` - Resets the seed
- `double random()` - `[0, 1)` uniform
- `double operator()()` - `random()`
- `int64_t randint(int64_t b)` - `[0, b)`
- `int64_t randint(int a, int b)` - `[a, b)`