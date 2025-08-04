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
    Utils::pointSet chosenpath; // Chosen path
    std::map<int, Utils::pointSet> auxiliaryPaths; // Additional paths for things like symmetry grids
    int64_t R = 9;
    int64_t C = 9;
    Utils::pointSet starts;
    Utils::pointSet ends;

    void init() {
        if (!(R & 1)) R++;
        if (!(C & 1)) C++;
        starts = Utils::pointSet({{0, 0}});
        ends = Utils::pointSet({{R - 1, C - 1}});
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

    virtual void setChosenPath(int x) {
        if (x < 0 || x >= storedpaths.size()) return;
        auxiliaryPaths.clear();
        chosenpath.clear();
        for (auto i : storedpaths[x]) chosenpath.insert(i);
    }

    void pickRandomPath() {
        setChosenPath(rand.randint(storedpaths.size()));
    }

    virtual void applyChosenPath(Grid* grid) {
        grid->clearAllLines();
        for (auto i : chosenpath) {
            grid->setLine(i, 1);
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

    Grid blankGrid() {
        Grid g(R, C);
        for (auto i : starts) g.set(i, new Endpoint(true));
        for (auto i : ends) g.set(i, new Endpoint(false));

        g.defaultPaths();
        return g;
    }

    void seed(int64_t see) {
        rand.seed(see);
    }

    // Pathfind
    void pathfind(int numPaths = INT_MAX, int see = 0) {
        Grid g = blankGrid();
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
        std::shuffle(v.begin(), v.end(), rand.gen);

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
    std::vector<EntityColor::Color> getRandomColors(int k) {
        if (k >= EntityColor::COLORS.size()) k = EntityColor::COLORS.size();
        std::vector<EntityColor::Color> v;
        for (auto i : EntityColor::COLORS) v.push_back(i);
        std::shuffle(v.begin(), v.end(), rand.gen);

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
        std::shuffle(v.begin(), v.end(), rand.gen);

        auto vv = getRandomCells(8);
        auto cols = getRandomColors(2);


        Grid grid = blankGrid();

        for (int i = 0; i < v.size(); i++) {
            auto p = vv[i];
            grid.set(p.first, p.second, new Star(cols[i & 1]));
        }
        return grid;
    }

    // ALL NUMBERS ARE UPPER BOUNDS
    // Random maze
    Grid randMaze(int cuts = 10) {
        pickRandomPath();
        auto cutlocs = getRandomEdges(cuts, true);

        Grid grid = blankGrid();
        for (auto i : cutlocs) grid.setPath(i, false);

        return grid;
    }

    // Random dots
    Grid randDots(int numSymbols = 8, int numCuts = 2) {
        pickRandomPath();
        auto cutlocs = getRandomEdges(numCuts, true);
        auto symbols = getRandomPaths(numSymbols, false, true);

        Grid grid = blankGrid();
        for (auto i : cutlocs) grid.setPath(i, false);
        for (auto i : symbols) grid.set(i, new PathDot());
        return grid;
    }

    // Random blobs
    Grid randBlobs(int numSymbols = 10, int numCols = 3, int numCuts = 2) {
        pickRandomPath();
        auto cutlocs = getRandomEdges(numCuts, true);
        auto symbols = getRandomCells(numSymbols);

        Grid grid = blankGrid();
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

    Grid randTriangles(int numSymbols = 10, int numCuts = 2) {
        pickRandomPath();
        auto cutlocs = getRandomEdges(numCuts, true);
        auto symbols = getRandomCells(numSymbols);

        Grid grid = blankGrid();
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

    Grid randBlocks(int numSymbols = 3, int numRegions = 2, int numCuts = 2, double rotation = 0.1) {
        pickRandomPath();
        auto cutlocs = getRandomEdges(numCuts, true);

        Grid grid = blankGrid();
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
            std::map<Utils::point, int> blockNo;
            Utils::pointVec seedblock;
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