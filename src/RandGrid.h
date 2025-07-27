#ifndef RANDGRID_WITCLN
#define RANDGRID_WITCLN

#include "Randgen.h"
#include "Grid.h"
#include "PuzzleEntity.h"
#include "Utils.h"
#include "Solver.h"

/*

Customizable random grid generator. This one is a bit more robust than the original.

*/

class RandGrid {
    public:
    RandGen rand;
    std::vector<Utils::pointVec> storedpaths; // A simple cache of stored paths
    int64_t R = 9;
    int64_t C = 9;
    Utils::pointVec starts;
    Utils::pointVec ends;

    void init() {
        if (!(R & 1)) R++;
        if (!(C & 1)) C++;
        starts = Utils::pointVec({{0, 0}});
        ends = Utils::pointVec({{R - 1, C - 1}});
    }

    Grid blankGrid() {
        Grid g(R, C);
        for (auto i : starts) g.set(i, new Endpoint(true));
        for (auto i : ends) g.set(i, new Endpoint(false));

        g.defaultPaths();
        return g;
    }

    RandGrid() {
        rand = RandGen();
        init();
    }

    RandGrid(int64_t seed) {
        rand = RandGen(seed);
        init();
    }

    RandGrid(int64_t r, int64_t c) {
        R = r;
        C = c;
        rand = RandGen();
        init();
    }
    
    RandGrid(int64_t r, int64_t c, int64_t seed) {
        R = r;
        C = c;
        rand = RandGen(seed);
        init();
    }

    void seed(int64_t see) {
        rand.seed(see);
    }



    void pathfind(int numPaths = INT_MAX) {
        Grid g = blankGrid();
        Solver solver;
        solver.grid = &g;
        solver.solve(numPaths);
        storedpaths = solver.solutions;
    }
};

#endif