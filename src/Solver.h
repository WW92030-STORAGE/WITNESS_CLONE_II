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
    std::map<Utils::point, Utils::point> vis; // Each point stores its parent. The parent of the starting point is itself.
    std::vector<Utils::pointVec> solutions;
    RandGen PRNG;

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

    void path(Utils::point src, Utils::point prev, int numsol = 1) {
        if (solutions.size() >= numsol) return;
        // std::cout << Utils::disp(src) << "\n";
        // Reached an endpoint? Try to solve but you can continue
        auto landmark = grid->get(src);
        if (auto ep = instanceof<Endpoint, PuzzleEntity>(landmark)) {
            if (!ep->isStart) {
                grid->setLine(src, true);
                if (GridUtils::Validate(grid)) {
                    vis.insert({src, prev});
                    Utils::pointVec solution;
                    Utils::point now = src;
                    while (vis.find(now) != vis.end() && vis.at(now) != now) {
                        solution.push_back(now);
                        now = vis.at(now);
                    }
                    solution.push_back(now);
                    std::reverse(solution.begin(), solution.end());
                    solutions.push_back(solution);
                    // std::cout << "PUSHED BACK SOLUTIONS .. " << solutions.size() << "\n";
                }

                grid->setLine(src, false);

                // If there's only one endpoint (which is true for most puzzles people make) there is no reason to skip over it.
                if (numEnds == 1) return; // Simple optimization for if you covered all endpoints
            }


        }

        // Depth first search

        Utils::pointVec nn;
        for (auto i : grid->neighbors(src)) nn.push_back(i);

        vis.insert({src, prev});
        grid->setLine(src, true);

        int offset = PRNG.randint(nn.size());

        // int offset = hash(src, prev) % nn.size();

        for (int dd = 0; dd < nn.size(); dd++) {
            int d = (dd + offset) % nn.size();
            Utils::point next = nn[d];

            if (!grid->inBounds(next)) continue;
            if (!(grid->get(next)->isPath)) continue;
            if (vis.find(next) != vis.end()) continue;
            if ((grid->get(next)->hasLine)) continue;

            // TODO - Figure out a simple pruning strategy maybe? idk
            
            path(next, src, numsol);
            if (solutions.size() >= numsol) break;
        }

        vis.erase(vis.find(src));
        grid->setLine(src, false);
    }

    void solve(int numsol = 1) {
        solutions.clear();
        numEnds = 0;
        auto endsnstarts = GridUtils::getSymbols<Endpoint>(grid);
        for (auto i : endsnstarts) {
            auto p = grid->get(i);
            if (!instanceof<Endpoint, PuzzleEntity>(p)) {
                continue;
            }
            Endpoint* pp = dynamic_cast<Endpoint*>(p);
            if (!pp->isStart) {
                numEnds++;
                continue;
            }

            vis.clear();
            vis.insert({i, i});
            path(i, i, numsol);
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