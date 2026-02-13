#ifndef SOLVER_WITCLN
#define SOLVER_WITCLN

#include "Grid.h"
#include "GridUtils.h"
#include "PuzzleEntity.h"
#include "Utils.h"
#include "BlockGroup.h"

#include <algorithm>
#include <vector>
#include <map>

#include "RandGen.h"


/*

Automatic solver for the puzzles. This should never be a place to long term store grids.

*/

class Solver {
    public:
    Grid* grid;
    int numEnds = 1;
    Utils::pointSet ends;
    int endsVisited = 0;
    std::vector<std::vector<Utils::point>> vis; // Each point stores its parent. The parent of the starting point is itself.
    std::vector<Utils::pointVec> solutions;
    RandGen PRNG;

    // vis
    Utils::point NIL = {-100, -100};
    inline void fullResetVis() {
        for (int i = 0; i < vis.size(); i++) {
            for (int j = 0; j < vis[i].size(); j++) vis[i][j] = NIL;
        }
    }

    inline void setVis(Utils::point a, Utils::point b) {
        // std::cout << "setVis(" << Utils::to_string(a) << "): " << Utils::to_string(vis[a.first][a.second]) << " > " << Utils::to_string(b) << "\n";
        vis[a.first][a.second] = b;
    }

    inline void resetVis(Utils::point p) {
        // std::cout << "resetVis(" << Utils::to_string(p) << ")\n";
        vis[p.first][p.second] = NIL;
    }

    inline bool containsVis(Utils::point p) {
        // std::cout << "containsVis(" << Utils::to_string(p) << ") ? " << Utils::to_string(vis[p.first][p.second]) << "\n";
        return vis[p.first][p.second] != NIL;
    }

    inline Utils::point getVis(Utils::point p) {
        // std::cout << "getVis(" << Utils::to_string(p) << ") = " << Utils::to_string(vis[p.first][p.second]) << "\n";
        return vis[p.first][p.second];
    }



    // solver

    ~Solver() {

    }

    Solver() {
        PRNG = RandGen();
    }

    Solver(int64_t seed) {
        PRNG = RandGen(seed);
    }

    void seed(int64_t see) {
        PRNG.seed(see);
    }

    // These numbers are taken from the Minecraft slime chunk algorithm.
    int hash(Utils::point a, Utils::point b) {
        int res1 = (a.first + a.second) ^ 0x4c1906;
        int res2 = (b.first + b.second) ^ 0x4307a7L;

        return (res1 + res2) ^ 0x3ad8025f;
    }

    // Does this region violate stuff?
    bool regionPruningValidation(Utils::pointSet& region) {
        // Are there cancels?
        auto cancels = GridUtils::getActiveSymbols<Cancel>(grid);
        if (intersection(cancels, region).size()) return false;

        // Validation of region otherwise
        return GridUtils::validateRegionNoRecur(grid, region).size();
    }

    void path(Utils::point src, Utils::point prev, int numsol = 1, bool prune = false, bool verbose = false) {
        if (solutions.size() >= numsol) return;
        if (verbose) std::cout << Utils::disp(src) << "\n";
        
        
        // Reached an endpoint? Try to solve but you can continue
        auto landmark = grid->get(src);

        bool reachedEndInCurStep = false;

        // Reached end = could prune or could continue
        if (auto ep = instanceof<Endpoint, PuzzleEntity>(landmark)) {
            if (!ep->isStart) {
                reachedEndInCurStep = true;
                endsVisited += 1;
                grid->setLine(src, true);
                if (verbose) std::cout << "Validating...\n";
                bool solved = GridUtils::Validate(grid, verbose);
                if (verbose) std::cout << "Validated\n";
                if (solved) {
                    setVis(src, prev);
                    Utils::pointVec solution;
                    Utils::point now = src;
                    while (containsVis(src) && getVis(now) != now) {
                        Utils::point next = getVis(now);
                        solution.push_back(now);
                        solution.push_back({(now.first + next.first)>>1, (now.second + next.second)>>1});
                        now = next;
                    }
                    solution.push_back(now);
                    std::reverse(solution.begin(), solution.end());
                    solutions.push_back(solution);

                    resetVis(src);
                }

                grid->setLine(src, false);

                // If there's only one endpoint (which is true for most puzzles people make) there is no reason to skip over it.
                if (numEnds == 1 || endsVisited >= numEnds) {
                    endsVisited--;
                    return; // Simple optimization for if you covered all endpoints
                }
            }


        }

        // Begin pruning

        // If we are blocked in two opposing directions but not the other two then we check the regions of the others
        if (prune) {

            for (int i = 0; i < 4; i++) {
                Utils::point p0 = {src.first + Utils::dx[i], src.second + Utils::dy[i]};
                Utils::point p1 = {src.first + Utils::dx[(i + 1) & 3], src.second + Utils::dy[(i + 1) & 3]};
                Utils::point p2 = {src.first + Utils::dx[(i + 3) & 3], src.second + Utils::dy[(i + 3) & 3]};
                bool b1 = !grid->inBounds(p0);
                bool b3 = !grid->inBounds(p1) || !(grid->isPathable(p1)) || containsVis(p1) || (grid->get(p1)->hasLine);
                bool b4 = !grid->inBounds(p2) || !(grid->isPathable(p2)) || containsVis(p2) || (grid->get(p2)->hasLine);
                if (b1 && !b3 && !b4) {
                    if (verbose) std::cout << "PRUNING ALLOWED\n" << Utils::to_string(src) << " " << Utils::to_string(p0) << " | " << Utils::to_string(p1) << "\n";

                    grid->setLine(src, 1);

                    Utils::pointSet r1 = GridUtils::floodfill(grid, p1);
                    if (verbose) std::cout << grid->highlight(r1) << "\n" << Utils::to_string(p2) << "\n";
                    if (Utils::contains(r1, p2)) {
                        if (verbose) std::cout << "PRUNING HALTED!!!\n";
                        grid->setLine(src, 0);
                        continue;
                    }
                    Utils::pointSet r2 = GridUtils::floodfill(grid, p2);
                    if (verbose) std::cout << "REGIONS COLLECTED\n";
                    
                    bool v1 = regionPruningValidation(r1);
                    if (verbose) std::cout << "v1 " << v1 << "\n";
                    if (!v1) {
                        grid->setLine(src, 0);
                        continue;
                    }
                    bool v2 = regionPruningValidation(r2);
                    if (verbose) std::cout << "v2 " << v2 << "\n";
                    if (!v2) {
                        grid->setLine(src, 0);
                        continue;
                    }
                    
                    // both v1, v2 bad
                    if (reachedEndInCurStep) endsVisited--;
                    grid->setLine(src, 0);
                    return;
                } 
            }
        }

        // End pruning

        // Depth first search

        setVis(src, prev);
        grid->setLine(src, true);

        // int offset = 0;
        int offset = PRNG.randint(4);

        // int offset = hash(src, prev) % nn.size();

        // Expand nodes
        for (int dd = 0; dd < 4; dd++) {
            int d = (dd + offset) & 3;
            Utils::point next = {src.first + 2 * Utils::dx[d], src.second + 2 * Utils::dy[d]};
            Utils::point hit = {src.first + Utils::dx[d], src.second + Utils::dy[d]};

            // Begin pruning
            if (!grid->inBounds(next)) continue;
            if (!(grid->isPathable(next))) continue;
            if (containsVis(next)) continue;
            if ((grid->get(next)->hasLine)) continue;
            if (!grid->inBounds(hit)) continue;
            if (!(grid->isPathable(hit))) continue;
            if ((grid->get(hit)->hasLine)) continue;
            if (containsVis(hit)) continue;

            // TODO - Figure out a simple pruning strategy maybe? idk
        

            // End pruning

            grid->setLine(hit, true);
            path(next, src, numsol, prune, verbose);
            grid->setLine(hit, false);
            if (solutions.size() >= numsol) break;
        }

        resetVis(src);
        grid->setLine(src, false);
        if (reachedEndInCurStep) endsVisited--;
    }

    void solve(int numsol = 1, bool prune = false, bool verbose = false) {
        vis = std::vector<std::vector<Utils::point>>(grid->R, std::vector<Utils::point>(grid->C, NIL));
        ends.clear();
        solutions.clear();
        numEnds = 0;
        endsVisited = 0;

        Utils::pointSet starts;
        auto endsnstarts = GridUtils::getSymbols<Endpoint>(grid);
        for (auto i : endsnstarts) {
            auto p = grid->get(i);
            Endpoint* pp = dynamic_cast<Endpoint*>(p);
            if (!pp->isStart) {
                numEnds++;
                ends.insert(i);
            }
            else starts.insert(i);
        }

        for (auto i : starts) {
            fullResetVis();
            setVis(i, i);
            path(i, i, numsol, prune, verbose);
            if (solutions.size() >= numsol) {
                break;
            }
        }
    }

    void apply(int x = 0) {
        if (x < 0 || x >= solutions.size()) return;

        // for (auto i : solutions[x]) grid->setLine(i, 1);
        grid->drawPath(solutions[x], 1);
    }
};

#endif