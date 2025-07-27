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

/*

Automatic solver for the puzzles. This should never be a place to long term store grids.

*/

class Solver {
    public:
    Grid* grid;
    int numEnds = 1;
    std::map<Utils::point, Utils::point> vis; // Each point stores its parent. The parent of the starting point is itself.
    std::vector<Utils::pointVec> solutions;

    ~Solver() {

    }

    Solver() {
    }

    void path(Utils::point src, Utils::point prev, int numsol = 1) {
        if (solutions.size() >= numsol) return;
        // std::cout << Utils::disp(src) << "\n";
        // Reached an endpoint? Try to solve but you can continue
        auto landmark = grid->get(src);
        if (instanceof<Endpoint, PuzzleEntity>(landmark)) {
            Endpoint* ep = dynamic_cast<Endpoint*>(landmark);
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



        vis.insert({src, prev});
        grid->setLine(src, true);

        int offset = rand() & 3;

        for (int dd = 0; dd < 4; dd++) {
            int d = (dd + offset) & 3;
            Utils::point next = {src.first + Utils::dx[d], src.second + Utils::dy[d]};
            Utils::point next2 = {src.first + Utils::dx[d] * 2, src.second + Utils::dy[d] * 2};

            if (!grid->inBounds(next)) continue;
            if (!(grid->get(next)->isPath)) continue;
            if (vis.find(next) != vis.end()) continue;
            if ((grid->get(next)->hasLine)) continue;

            const bool PRUNE1 = true;

            // don't back into any dead ends
            if (PRUNE1 && !instanceof<Endpoint, PuzzleEntity>(grid->get(next))) {
                if (grid->inBounds(next2)) {
                    auto p = grid->get(next2);
                    if (p->hasLine) continue;
                    if (!p->isPath) continue;
                }
            }
            
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

    void apply(int x) {
        if (x < 0 || x >= solutions.size()) return;

        grid->drawPath(solutions[x]);
    }
};

#endif