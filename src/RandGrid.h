#ifndef RANDGRID_WITCLN
#define RANDGRID_WITCLN

#include "RandGen.h"
#include "Grid.h"
#include "PuzzleEntity.h"
#include "Utils.h"
#include "Solver.h"

/*

Customizable random grid generator. This one is a bit more robust than the original.

*/

template <typename GridT = Grid>
class RandGrid {
    public:
    RandGen PRNG;
    std::vector<Utils::pointVec> storedpaths; // A simple cache of stored paths
    Utils::pointSet chosenpath; // Chosen path
    std::map<int, Utils::pointSet> auxiliaryPaths; // Additional paths for things like symmetry grids
    int64_t R = 9;
    int64_t C = 9;
    Utils::pointSet starts;
    Utils::pointSet ends;

    void init() {
        GridT symgrid = blankGrid();
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

    RandGrid() {
        PRNG = RandGen();
        init();
    }

    RandGrid(int64_t seed) {
        PRNG = RandGen(seed);
        init();
    }

    RandGrid(int64_t r, int64_t c) {
        R = r;
        C = c;
        PRNG = RandGen();
        init();
    }
    
    RandGrid(int64_t r, int64_t c, int64_t seed) {
        R = r;
        C = c;
        PRNG = RandGen(seed);
        init();
    }

    virtual void setChosenPath(int x) {
        if (x < 0 || x >= storedpaths.size()) return;
        chosenpath.clear();
        auxiliaryPaths.clear();
        for (auto i : storedpaths[x]) {
            chosenpath.insert(i);
        }
        GridT grid = blankGrid();
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

    void pickRandomPath() {
        setChosenPath(PRNG.randint(storedpaths.size()));
    }

    virtual void applyChosenPath(GridT* grid) {
        grid->clearAllLines();
        for (auto i : chosenpath) {
            grid->setLine(i.first, i.second, 1);
        }
    }

    bool isOnChosenPath(Utils::point p) {
        return chosenpath.find(p) != chosenpath.end();
    }

    bool isOnAnyPath(Utils::point p) {
        if (isOnChosenPath(p)) return true;
        for (auto i : auxiliaryPaths) {
            if (i.second.find(p) != i.second.end()) return true;
        }
        return false;
    }

    GridT blankGrid() {
        GridT g(R, C);
        for (auto i : starts) g.set(i, new Endpoint(true));
        for (auto i : ends) g.set(i, new Endpoint(false));

        g.defaultPaths();
        return g;
    }

    void seed(int64_t see) {
        PRNG.seed(see);
    }

    // Pathfind
    void pathfind(int numPaths = INT_MAX, int see = 0) {
        GridT g = blankGrid();
        Solver solver;
        if (see) solver.seed(see);
        solver.grid = &g;
        solver.solve(numPaths);
        storedpaths = solver.solutions;
    }

    // Methods for randomly choosing.
    std::vector<int> randomChoice(int n, int k) {
        if (k > n) k = n;
        std::vector<int> v(n);
        for (int i = 0; i < n; i++) v[i] = i;
        std::shuffle(v.begin(), v.end(), PRNG.gen);

        while (v.size() > k && v.size()) v.pop_back();
        return v;
    }

    // Get random cells in the grid
    Utils::pointVec getRandomCells(int k) {
        if (k >= (R/2) * (C/2)) k = (R/2) * (C/2);
        Utils::pointVec res;
        Utils::pointSet s;
        /*
        int MAX_ITER = 1024;

        for (int it = 0; it < MAX_ITER; it++) {
            Utils::point chosen = {1 + 2 * rand.randint(R/2), 1 + 2 * rand.randint(C/2)};
            if (s.find(chosen) != s.end()) continue;
            res.push_back(chosen);
            s.insert(chosen);

            if (res.size() >= k) return res;
        }

        for (int r = 1; r < R; r += 2) {
            for (int c = 1; c < C; c += 2) {
                if (res.size() >= k) break;
                if (s.find({r, c}) == s.end()) res.push_back({r, c});
            }
        }
        */

        Utils::pointVec order;
        for (int r = 0; r < R; r++) {
            for (int c = 0; c < C; c++) {
                if ((r & 1) && (c & 1)) order.push_back({r, c});
            }
        }
        if (k > order.size()) k = order.size();
        auto choice = randomChoice(order.size(), k);
        for (auto i : choice) res.push_back(order[i]);

        
        return res;
    }

    // Get random edges (not intersections) in the grid
    Utils::pointVec getRandomEdges(int k, bool noLine = false, bool line = false) {
        Utils::pointVec res;
        Utils::pointSet s;

        Utils::pointVec order;
        for (int r = 0; r < R; r++) {
            for (int c = 0; c < C; c++) {
                if (noLine && isOnAnyPath({r, c})) continue;
                if (line && !isOnAnyPath({r, c})) continue;
                if (Utils::contains(starts, {r, c})) continue;
                if (Utils::contains(ends, {r, c})) continue;
                if ((r & 1) != (c & 1)) order.push_back({r, c});
            }
        }
        if (k > order.size()) k = order.size();
        auto choice = randomChoice(order.size(), k);
        for (auto i : choice) res.push_back(order[i]);

        
        return res;
    }

    // Get random corners/nodes in the grid.
    Utils::pointVec getRandomNodes(int k, bool noLine = false, bool line = false) {
        Utils::pointVec res;
        Utils::pointSet s;

        Utils::pointVec order;
        for (int r = 0; r < R; r++) {
            for (int c = 0; c < C; c++) {
                if (noLine && isOnAnyPath({r, c})) continue;
                if (line && !isOnAnyPath({r, c})) continue;
                if (Utils::contains(starts, {r, c})) continue;
                if (Utils::contains(ends, {r, c})) continue;
                if ((r & 1) || (c & 1)) continue;
                order.push_back({r, c});
            }
        }
        if (k > order.size()) k = order.size();
        auto choice = randomChoice(order.size(), k);
        for (auto i : choice) res.push_back(order[i]);

        return res;
    }

    // Get random paths (edges + corners)
    Utils::pointVec getRandomPaths(int k, bool noLine = false, bool line = false) {
        Utils::pointVec res;
        Utils::pointSet s;

        Utils::pointVec order;
        for (int r = 0; r < R; r++) {
            for (int c = 0; c < C; c++) {
                if (noLine && isOnAnyPath({r, c})) continue;
                if (line && !isOnAnyPath({r, c})) continue;
                if (Utils::contains(starts, {r, c})) continue;
                if (Utils::contains(ends, {r, c})) continue;
                if ((r & 1) && (c & 1)) continue;
                order.push_back({r, c});
            }
        }
        if (k > order.size()) k = order.size();
        auto choice = randomChoice(order.size(), k);
        for (auto i : choice) res.push_back(order[i]);

        return res;
    }

    // Get random colors
    std::vector<EntityColor::Color> getRandomColors(int k = INT_MAX) {
        if (k >= EntityColor::COLORS.size()) k = EntityColor::COLORS.size();
        std::vector<EntityColor::Color> v;
        for (auto i : EntityColor::COLORS) v.push_back(i);
        std::shuffle(v.begin(), v.end(), PRNG.gen);

        std::vector<EntityColor::Color> res;
        for (int i = 0; i < k && i < v.size(); i++) res.push_back(v[i]);
        return res;
    }

    /*
    
    GENERATORS FOR THE GRIDS
    
    */


    // 8 random stars. Any more than that and there might be unsolvable ones.
    Grid randStars() {
        std::vector<bool> v({0, 0, 0, 0, 1, 1, 1, 1});
        std::shuffle(v.begin(), v.end(), PRNG.gen);

        auto vv = getRandomCells(8);
        auto cols = getRandomColors(2);


        Grid grid = blankGrid();
        grid.defaultDiagonal();

        for (int i = 0; i < v.size(); i++) {
            auto p = vv[i];
            grid.set(p.first, p.second, new Star(cols[i & 1]));
        }
        return grid;
    }
    
    // ALL NUMBERS ARE UPPER BOUNDS
    GridT randStarsGeneral(int numPairs = 4, int numCuts = 2) {
        pickRandomPath();
        auto cutlocs = getRandomEdges(numCuts, true);
        GridT grid = blankGrid();
        applyChosenPath(&grid);
        auto regions = GridUtils::getRegionsCells(&grid);
        grid.clearAllLines();

        for (auto i : cutlocs) grid.setPath(i, false);
        int nReg = regions.size();

        std::vector<int> regioncuts; 
        for (int rno = 0; rno < nReg; rno++) {
            for (int i = 0; i < regions[rno].size(); i += 2) regioncuts.push_back(rno);
        }

        std::shuffle(regioncuts.begin(), regioncuts.end(), PRNG.gen);
        
        std::vector<int> pairsPerRegion(nReg, 0);
        for (int i = 0; i < numPairs && i < regioncuts.size(); i++) pairsPerRegion[regioncuts[i]]++;

        auto colors = getRandomColors();

        for (int rno = 0; rno < nReg; rno++) {
            Utils::pointVec region;
            for (auto i : regions[rno]) region.push_back(i);
            std::shuffle(region.begin(), region.end(), PRNG.gen);
            for (int i = 0; i < pairsPerRegion[rno]; i++) {
                grid.set(region[i<<1], new Star(colors[i]));
                grid.set(region[1 + (i<<1)], new Star(colors[i]));
            }
        }

        return grid;
    }

    // Random maze
    GridT randMaze(int cuts = 10) {
        pickRandomPath();
        auto cutlocs = getRandomEdges(cuts, true);

        GridT grid = blankGrid();
        for (auto i : cutlocs) grid.setPath(i, false);

        return grid;
    }

    // Random dots
    GridT randDots(int numSymbols = 8, int numCuts = 2, double threshold = 0.5) {
        pickRandomPath();
        auto cutlocs = getRandomEdges(numCuts, true);
        auto symbols = getRandomPaths(numSymbols, false, true);

        GridT grid = blankGrid();
        applyChosenPath(&grid);
        for (auto i : cutlocs) grid.setPath(i, false);
        for (auto i : symbols) {
            if (PRNG() < threshold) {
                grid.set(i, new PathDot((1<<(-1 + grid.get(i)->hasLine))));
            }
            else grid.set(i, new PathDot());
        }
        grid.clearAllLines();
        return grid;
    }

    // Random blobs
    GridT randBlobs(int numSymbols = 10, int numCols = 3, int numCuts = 2) {
        pickRandomPath();
        auto cutlocs = getRandomEdges(numCuts, true);
        auto symbols = getRandomCells(numSymbols);

        GridT grid = blankGrid();
        applyChosenPath(&grid);
        auto regions = GridUtils::getRegionsCells(&grid);
        grid.clearAllLines();

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

    GridT randTriangles(int numSymbols = 10, int numCuts = 2) {
        pickRandomPath();
        auto cutlocs = getRandomEdges(numCuts, true);
        auto symbols = getRandomCells(numSymbols);

        GridT grid = blankGrid();
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

    GridT randBlocks(int numSymbols = 3, int numRegions = 2, int numCuts = 2, double rotation = 0.1) {
        pickRandomPath();
        auto cutlocs = getRandomEdges(numCuts, true);

        GridT grid = blankGrid();
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
            std::map<Utils::point, int> blockNo; // What block each point goes
            Utils::pointVec seedblock; // Placement locations of blocks in the region
            std::queue<Utils::point> q;
            int index = 0;
            for (auto i : blockseeds) {
                blockNo.insert({i, index++});
                seedblock.push_back(i);
            }
            for (auto i : blockseeds) q.push(i);

            while (q.size()) {
                auto p = q.front();
                auto number = blockNo[p];
                q.pop();
                std::vector<Utils::point> nexts;
                for (int d = 0; d < 4; d++) {
                    Utils::point next = {p.first + 2 * Utils::dx[d], p.second + 2 * Utils::dy[d]};
                    nexts.push_back(next);
                }
                std::shuffle(nexts.begin(), nexts.end(), PRNG.gen);
                for (int dd = 0; dd < 4; dd++) {
                    Utils::point next = nexts[dd];
                    if (!grid.inBounds(next)) continue;
                    if (blockNo.find(next) != blockNo.end()) continue;
                    if (region.find(next) == region.end()) continue;

                    blockNo.insert({next, number});
                    q.push(next);
                }
            }

            std::shuffle(seedblock.begin(), seedblock.end(), PRNG.gen);

            // Do the thing
            for (int i = 0; i < seedblock.size(); i++) {
                Utils::pointVec transformed;
                for (auto p : blockNo) {
                    if (p.second == i) transformed.push_back({(p.first.first - 1) / 2, (p.first.second - 1) / 2 });
                    BlockGroup bg(transformed);
                    if (PRNG() < rotation) {
                        bg.fixed = false;
                        bg.rotate(PRNG.randint(4));
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