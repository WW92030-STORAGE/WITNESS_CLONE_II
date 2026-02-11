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
    std::map<Utils::point, Utils::point> vis; // Each point stores its parent. The parent of the starting point is itself.
    bool touchedWall = false;
    Utils::point wallTouched;
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

        // If we are touching a wall then we have touched a wall
        if (!touchedWall && src.first <= 0 || src.second <= 0 || src.first >= grid->R - 1 || src.second >= grid->C - 1) {
            touchedWall = true;
            wallTouched = src;
        }

        // Reached end = could prune or could continue
        if (auto ep = instanceof<Endpoint, PuzzleEntity>(landmark)) {
            if (!ep->isStart) {
                endsVisited += 1;
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
                if (numEnds == 1 || endsVisited >= numEnds) {
                    endsVisited--;
                    if (touchedWall && wallTouched == src) touchedWall = false;
                    return; // Simple optimization for if you covered all endpoints
                }
            }


        }

        // Depth first search

        Utils::pointVec nn;
        for (auto i : grid->neighbors(src)) nn.push_back(i);

        vis.insert({src, prev});
        grid->setLine(src, true);

        int offset = PRNG.randint(nn.size());

        // int offset = hash(src, prev) % nn.size();

        // Begin pruning

        // If we are blocked in two opposing directions but not the other two then we check the regions of the others
        if (touchedWall) {
            int dx[4] = {1, 0, -1, 0};
            int dy[4] = {0, 1, 0, -1};
            bool blocked[4] = {0, 0, 0, 0};
            for (int d = 0; d < 4; d++) {
                for (int k = 1; !blocked[d] && k <= 2; k++) {
                    Utils::point next = {src.first + dx[d] * k, src.second + dy[d] * k};
                    if (!grid->inBounds(next)) {
                        blocked[d] = 1;
                        break;
                    }
                    if (!(grid->isPathable(next))) {
                        blocked[d] = 1;
                        break;
                    }
                    if (vis.find(next) != vis.end()) {
                        blocked[d] = 1;
                        break;
                    }
                    if ((grid->get(next)->hasLine)) {
                        blocked[d] = 1;
                        break;
                    }
                }
            }
            for (int i = 0; i < 2; i++) {
                if (blocked[i + 1] && blocked[(i + 3) & 3] && !blocked[i] && !blocked[i + 2]) {
                    Utils::point p1 = {src.first + dx[i], src.second + dy[i]};
                    Utils::point p2 = {src.first + dx[i + 2], src.second + dy[i + 2]};
                    Utils::pointSet r1 = GridUtils::floodfill(grid, p1);
                    Utils::pointSet r2 = GridUtils::floodfill(grid, p2);
                    int e1 = 0;
                    int e2 = 0;
                    for (auto p : ends) {
                        if (r1.count(p)) e1++;
                        if (r2.count(p)) e2++;
                    }

                    bool v1 = 0;
                    bool v2 = 0;

                    if (e1 <= 0 && e2 <= 0) {
                        vis.erase(vis.find(src));
                        grid->setLine(src, false);
                        endsVisited--;
                        if (touchedWall && wallTouched == src) touchedWall = false;
                        return;
                    }
                    else if (e1 <= 0 && (v1 = GridUtils::validateRegionNoRecur(grid, r1).size())) {
                        vis.erase(vis.find(src));
                        grid->setLine(src, false);
                        endsVisited--;
                        if (touchedWall && wallTouched == src) touchedWall = false;
                        return;
                    }
                    else if (e2 <= 0 && (v2 = GridUtils::validateRegionNoRecur(grid, r2).size())) {
                        vis.erase(vis.find(src));
                        grid->setLine(src, false);
                        endsVisited--;
                        if (touchedWall && wallTouched == src) touchedWall = false;
                        return;
                    }
                    else if (v1 && v2) {
                        vis.erase(vis.find(src));
                        grid->setLine(src, false);
                        endsVisited--;
                        if (touchedWall && wallTouched == src) touchedWall = false;
                        return;
                    }
                } 
            }
        }

        // End pruning

        // Expand nodes
        for (int dd = 0; dd < nn.size(); dd++) {
            int d = (dd + offset) % nn.size();
            Utils::point next = nn[d];

            // Begin pruning

            if (!grid->inBounds(next)) continue;
            if (!(grid->isPathable(next))) continue;
            if (vis.find(next) != vis.end()) continue;
            if ((grid->get(next)->hasLine)) continue;

            // TODO - Figure out a simple pruning strategy maybe? idk
        

            // End pruning
            
            path(next, src, numsol);
            if (solutions.size() >= numsol) break;
        }

        vis.erase(vis.find(src));
        grid->setLine(src, false);
        endsVisited--;
        if (touchedWall && wallTouched == src) touchedWall = false;
    }

    void solve(int numsol = 1) {
        solutions.clear();
        numEnds = 0;
        endsVisited = 0;
        touchedWall = 0;
        auto endsnstarts = GridUtils::getSymbols<Endpoint>(grid);
        for (auto i : endsnstarts) {
            auto p = grid->get(i);
            if (!instanceof<Endpoint, PuzzleEntity>(p)) {
                continue;
            }
            Endpoint* pp = dynamic_cast<Endpoint*>(p);
            if (!pp->isStart) {
                numEnds++;
                ends.insert(i);
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