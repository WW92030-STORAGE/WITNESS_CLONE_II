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
        for (auto i : storedpaths[x]) {
            chosenpath.insert(i);
        }
        SymmetryGrid grid = blankGrid();
        applyChosenPath(&grid);

        for (int r = 0; r < grid.R; r++) {
            for (int c = 0; c < grid.C; c++) {
                if (grid.board[r][c]->hasLine) {
                    int line = grid.board[r][c]->hasLine;
                    if (line == 1) continue;
                    if (auxiliaryPaths.find(line) == auxiliaryPaths.end()) auxiliaryPaths[line] = Utils::pointSet();
                    auxiliaryPaths[line].insert({r, c});
                }
            }
        }
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

    SymmetryGrid randBlocks(int numSymbols = 3, int numRegions = 2, int numCuts = 2, double rotation = 0.1) {
        pickRandomPath();
        auto cutlocs = getRandomEdges(numCuts, true);

        SymmetryGrid grid = blankGrid();
        applyChosenPath(&grid);
        auto regions = GridUtils::getRegionsCells(&grid);
        grid.clearAllLines();

        for (auto i : cutlocs) grid.setPath(i, 0);

        auto regionindices = randomChoice(regions.size(), numRegions);

        // Get symbol seed locations
        Utils::pointVec vv;
        for (auto i : regionindices) {
            auto region = regions[i];
            for (auto p : region) vv.push_back(p);
        }

        auto choice = randomChoice(vv.size(), numSymbols);
        Utils::pointSet seeds;
        for (auto i : choice) seeds.insert(vv[i]);

        for (auto i : regionindices) {
            auto region = regions[i];

            // Apply blocks
            
            auto blockseeds = intersection(seeds, region);
            if (blockseeds.size() <= 0) continue; // no blocks here -- moving on!

            // Use a simple random flood fill to decide what goes where 
            std::map<Utils::point, int> blockNo; // What block each point goes to?
            Utils::pointVec seedblock; // Placement locations of blocks in the region
            std::priority_queue<std::pair<uint64_t, Utils::point>> q;
            int index = 0;
            for (auto i : blockseeds) {
                blockNo.insert({i, index++});
                seedblock.push_back(i);
            }
            for (auto i : blockseeds) q.push({rand.gen(), i});

            while (q.size()) {
                auto p = q.top().second;
                auto number = blockNo[p];
                q.pop();
                int offset = rand.randint(4);
                for (int dd = 0; dd < 4; dd++) {
                    int d = (dd + offset) % 4;
                    Utils::point next = {p.first + 2 * Utils::dx[d], p.second + 2 * Utils::dy[d]};
                    if (!grid.inBounds(next)) continue;
                    if (blockNo.find(next) != blockNo.end()) continue;
                    if (region.find(next) == region.end()) continue;

                    blockNo.insert({next, number});
                    q.push({rand.gen(), next});
                }
            }

            std::shuffle(seedblock.begin(), seedblock.end(), rand.gen);

            // Do the thing
            for (int i = 0; i < seedblock.size(); i++) {
                Utils::pointVec transformed;
                for (auto p : blockNo) {
                    if (p.second == i) transformed.push_back({(p.first.first - 1) / 2, (p.first.second - 1) / 2 });
                    BlockGroup bg(transformed);
                    if (rand() < rotation) {
                        bg.fixed = false;
                        bg.rotate(rand.randint(4));
                    }
                    bg.normalize();
                    grid.set(seedblock[i], new BlockGroup(bg));
                }
            }
        }

        return grid;
        
    }
};

#endif