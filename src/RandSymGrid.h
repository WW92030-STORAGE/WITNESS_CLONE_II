#ifndef RANDSYMGRID_WITCLN
#define RANDSYMGRID_wITCLN

#include <climits>
#include <cstdint>

#include "Randgen.h"
#include "Grid.h"
#include "PuzzleEntity.h"
#include "Utils.h"
#include "Solver.h"
#include "RandGrid.h"
/*

Generators for other grid types. Because there are different classe for different grids we must override various methods using the different grid types.

*/

template <typename SymmetryGrid> 
class RandSymGrid : public RandGrid {
    public:

    void init() {
        SymmetryGrid symgrid = blankGrid();
        symgrid.defaultDiagonal();
        starts.clear();
        ends.clear();
        for (int r = 0; r < R; r++) {
            for (int c = 0; c < C; c++) {
                PuzzleEntity* p = symgrid.get(r, c);
                if (instanceof<Endpoint>(p)) {
                    Endpoint* ep = dynamic_cast<Endpoint*>(p);
                    if (ep->isStart) starts.insert({r, c});
                    else ends.insert({r, c});
                }
            }
        }
    }

    RandSymGrid() : RandGrid() {
        init();
    }

    RandSymGrid(int64_t seed) : RandGrid(seed) {
        init();
    }

    RandSymGrid(int64_t r, int64_t c) : RandGrid(r, c) {
        init();
    }

    RandSymGrid(int64_t r, int64_t c, int64_t seed) : RandGrid(r, c, seed) {
        init();
    }

    SymmetryGrid blankGrid() {
        SymmetryGrid g(R, C);
        for (auto i : starts) g.set(i, new Endpoint(true));
        for (auto i : ends) g.set(i, new Endpoint(false));

        g.defaultPaths();
        return g;
    }

    // Set the chosen path -- this must include all hasLine points when the path is invoked.
    virtual void setChosenPath(int x) {
        if (x < 0 || x >= storedpaths.size()) return;
        chosenpath.clear();
        auxiliaryPaths.clear();
        Utils::pointSet secondPath;
        for (auto i : storedpaths[x]) {
            chosenpath.insert(i);
            secondPath.insert({R - 1 - i.first, C - 1 - i.second});
        }
        auxiliaryPaths[2] = secondPath;
    }

    // Pathfind
    void pathfind(int numPaths = INT_MAX, int see = 0) {
        SymmetryGrid g = blankGrid();
        Solver solver;
        if (see) solver.seed(see);
        solver.grid = &g;
        solver.solve(numPaths);
        storedpaths = solver.solutions;
    }

    /*
    
    GENERATORS FOR THE GRIDS
    
    */

    // ALL NUMBERS ARE UPPER BOUNDS
    // Random maze
    SymmetryGrid randMaze(int cuts = 10) {
        pickRandomPath();
        auto cutlocs = getRandomEdges(cuts, true);

        SymmetryGrid grid = blankGrid();
        for (auto i : cutlocs) grid.setPath(i, false);

        return grid;
    }

    // Random dots. The restricted variable is the probability a given dot restricts to a line.
    SymmetryGrid randDots(int numSymbols = 8, int numCuts = 2, double restricted = 0.5) {
        pickRandomPath();
        auto cutlocs = getRandomEdges(numCuts, true);
        auto symbols = getRandomPaths(numSymbols, false, true);

        SymmetryGrid grid = blankGrid();
        applyChosenPath(&grid);
        for (auto i : cutlocs) grid.setPath(i, false);
        for (auto i : symbols) {
            double threshold = rand();
            if (rand() < restricted) grid.set(i, new PathDot((1<<(-1 + grid.get(i)->hasLine))));
            else grid.set(i, new PathDot());
        }
        grid.clearAllLines();
        return grid;
    }

    SymmetryGrid randTriangles(int numSymbols = 10, int numCuts = 2) {
        pickRandomPath();
        auto cutlocs = getRandomEdges(numCuts, true);
        auto symbols = getRandomCells(numSymbols);

        SymmetryGrid grid = blankGrid();
        applyChosenPath(&grid);

        for (auto i : symbols) {
            auto pp = grid.neighbors(i);
            int cc = 0;
            for (auto p : pp) {
                if (grid.get(p)->hasLine) cc++;
            }
            if (cc) grid.set(i, new Triangle(cc));
        }

        grid.clearAllLines();
        return grid;
    }

    // Random blobs
    SymmetryGrid randBlobs(int numSymbols = 10, int numCols = 2, int numCuts = 2) {
        pickRandomPath();
        auto cutlocs = getRandomEdges(numCuts, true);
        auto symbols = getRandomCells(numSymbols);

        SymmetryGrid grid = blankGrid();
        applyChosenPath(&grid);
        auto regions = GridUtils::getRegionsCells(&grid);
        grid.clearAllLines();

        for (auto i : regions) std::cout << Utils::disp(i) << "\n";

        for (auto i : cutlocs) grid.setPath(i, false);

        // Set the blobs

        auto colors = getRandomColors(numCols);

        for (auto i : symbols) {
            int region = 0;
            for (int r = 0; r < regions.size(); r++) {
                if (Utils::contains(regions[r], i)) region = r;
            }
            grid.set(i, new Blob(colors[region % numCols]));
        }

        return grid;
    }
};

#endif