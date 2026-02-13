#ifndef GRIDUTILS_WITCLN
#define GRIDUTILS_WITCLN

#include <vector>
#include <algorithm>
#include <map>
#include <set>
#include <queue>
#include <string>

#include "PuzzleEntity.h"
#include "Grid.h"
#include "Utils.h"
#include "BlockGroup.h"

#include <iostream>

/*

Utility methods for grids, and also solution checking.

*/

namespace GridUtils {

    /* 
    
    Helper functions for grids

    */

    // Get all positions of a certain symbol
    template <typename T>
    Utils::pointSet getSymbols(Grid* g) {
        Utils::pointSet s;
        for (int r = 0; r < g->R; r++) {
            for (int c = 0; c < g->C; c++) {
                PuzzleEntity* pp = g->get(r, c);
                if (instanceof<T>(pp)) s.insert({r, c});
            }
        }
        return s;
    }


    // Get the active symbols of a type
    template <typename T>
    Utils::pointSet getActiveSymbols(Grid* g) {
        Utils::pointSet s = getSymbols<T>(g);
        Utils::pointSet res;
        for (auto i : s) {
            if (g->get(i) && g->get(i)->isActive) res.insert(i);
        }
        return res;
    }

    // Get the symbols of a color
    Utils::pointSet getSymbolsColor(Grid* g, EntityColor::Color c) {
        Utils::pointSet res;
        for (int r = 0; r < g->R; r++) {
            for (int c = 0; c < g->C; c++) {
                PuzzleEntity* pp = g->get(r, c);
                if (pp && pp->color == c) res.insert({r, c});
            }
        }
        return res;
    }

    // Get the counts of colors
    std::map<EntityColor::Color, int> getColors(Grid* g, Utils::pointSet s, bool NotNil = true) {
        std::map<EntityColor::Color, int> res;
        for (auto i : s) {
            if (g->get(i)) {
                if (NotNil && g->get(i)->color == EntityColor::NIL) continue;
                Utils::push(res, g->get(i)->color);
            }
        }
        return res;
    }

        // Get the counts of colors
    std::map<EntityColor::Color, int> getActiveColors(Grid* g, Utils::pointSet s, bool NotNil = true) {
        std::map<EntityColor::Color, int> res;
        for (auto i : s) {
            if (g->get(i)) {
                if (NotNil && g->get(i)->color == EntityColor::NIL) continue;
                if (!g->get(i)->isActive) continue;
                Utils::push(res, g->get(i)->color);
            }
        }
        return res;
    }

    // Can we flood fill to this cell?
    bool isFFable(Grid* g, Utils::point next) {
        if (!g->inBounds(next)) return false;
        if (g->get(next)->isBlocker) return false;
        if (g->get(next)->hasLine) return false;
        return true;
    }

    // Flood fill a region, adhering to any blockers.
    Utils::pointSet floodfill(Grid* g, Utils::point p) {
        bool* vis2 = new bool[g->R * g->C];
        for (int i = 0; i < g->R * g->C; i++) vis2[i] = 0;
        Utils::pointSet vis;
        Utils::pointQueue q;
        if (!isFFable(g, p)) {
            delete[] vis2;
            return vis;
        }
        q.push(p);
        vis2[p.first * g->C + p.second] = 1;
        vis.insert(p);

        while (q.size()) {
            Utils::point now = q.front();
            q.pop();

            for (int d = 0; d < 4; d++) {
                Utils::point next = {now.first + Utils::dx[d], now.second + Utils::dy[d]};
                if (!isFFable(g, next)) continue;
                if (vis2[next.first * g->C + next.second]) continue;
                vis2[next.first * g->C + next.second] = 1;
                vis.insert(next);
                q.push(next);
            }
        }
        delete[] vis2;
        return vis;
    }

    Utils::pointSet floodfill(Grid* g, int x, int y) {
        return floodfill(g, {x, y});
    }

    // Flood fill a region, adhering to any blockers. This time the flood fill extends only across non-edges and will skip over any edges. 
    // Cells that share an edge as neighbor are considered adjacent in this model.
    Utils::pointSet floodfillCells(Grid* g, Utils::point p) {
        bool* vis2 = new bool[g->R * g->C];
        for (int i = 0; i < g->R * g->C; i++) vis2[i] = 0;
        Utils::pointSet res;
        Utils::pointQueue q;
        if (!isFFable(g, p)) {
            delete[] vis2;
            return res;
        }
        q.push(p);
        vis2[p.first * g->C + p.second] = 1;

        while (q.size()) {
            Utils::point now = q.front();
            q.pop();

            for (int d = 0; d < 4; d++) {
                Utils::point next = {now.first + Utils::dx[d], now.second + Utils::dy[d]};
                if (!isFFable(g, next)) continue;
                if (vis2[next.first * g->C + next.second]) continue;
                vis2[next.first * g->C + next.second] = 1;
                q.push(next);
            }
        }
        for (int i = 0; i < g->R * g->C; i++) {
            if (!vis2[i]) continue;
            int r = i / g->C;
            int c = i % g->C;
            if ((r & 1) && (c & 1)) res.insert({r, c});
        }
        delete[] vis2;
        return res;
    }

    std::vector<Utils::pointSet> getRegions(Grid* g) {
        std::vector<Utils::pointSet> v;
        Utils::pointSet s;
        for (int r = 0; r < g->R; r++) {
            for (int c = 0; c < g->C; c++) {
                Utils::point p = {r, c};
                if (!isFFable(g, p)) continue;
                if (Utils::contains(s, p)) continue;
                auto region = floodfill(g, p);
                for (auto i : region) s.insert(i);
                v.push_back(region);
            }
        }
        return v;
    }

    // Get the regions but only looks at cells.
    std::vector<Utils::pointSet> getRegionsCells(Grid* g) {
        std::vector<Utils::pointSet> v;
        Utils::pointSet s;
        for (int r = 1; r < g->R; r += 2) {
            for (int c = 1; c < g->C; c += 2) {
                Utils::point p = {r, c};
                if (!isFFable(g, p)) continue;
                if (Utils::contains(s, p)) continue;
                auto region = floodfillCells(g, p);
                for (auto i : region) s.insert(i);
                v.push_back(region);
            }
        }
        return v;
    }

    /*
    
    ACTUALLY VALIDATING THE GRIDS
    
    */

    // Retrieve a set of wrong symbol positions, ignoring cancels.

    Utils::pointSet validateRegionNoRecur(Grid* grid, Utils::pointSet& region) {
        Utils::pointSet violations;

        /*
        
        IN ACCORDANCE WITH THE FOUR STEP PROTOCOL
        ALL SYMBOLS THAT OPERATE WITHIN A LOCAL NEIGHBORHOOD (AND NOT A CONTIGUOUS REGION) SHOULD BE CHECKED HERE
        
        */

        // Check dots
        for (auto i : intersection(region, getActiveSymbols<PathDot>(grid))) {
            PuzzleEntity* p = grid->get(i);
            if (!instanceof<PathDot>(p)) continue;
            if (p && !p->hasLine) {
                violations.insert(i);
                continue;
            }
            PathDot* pd = dynamic_cast<PathDot*>(p);
            uint64_t restriction = pd->restriction;
            if (restriction == 0) continue; // If restriction = 0 continue else check bits
            if (restriction & (1<<(p->hasLine - 1))) continue;
            violations.insert(i);
        }

        // Check triangles
        for (auto i : intersection(region, getActiveSymbols<Triangle>(grid))) {
            PuzzleEntity* pp = grid->get(i);
            if (!instanceof<Triangle>(pp)) continue;
            Triangle* trix = dynamic_cast<Triangle*>(pp);
            int cc = 0;
            for (auto n : grid->neighbors(i)) {
                if (grid->get(n) && grid->get(n)->hasLine) cc++;
            }
            if (cc != trix->count) violations.insert(i);
        }

        auto entities = intersection(region, getActiveSymbols<ColorEntity>(grid)); // All colored symbols in the grid


        /*
        
        IN ACCORDANCE WITH THE FOUR STEP PROTOCOL
        ALL SYMBOLS THAT OPERATE WITHIN A CONTINUOUS REGION BUT ONLY INTERACT WITH OTHER SYMBOLS IN THE REGION BY THEIR TYPE OR COLOR SHOULD BE CHECKED HERE
        
        */

        // Check blobs
        auto blobs = intersection(region, getActiveSymbols<Blob>(grid));
        std::set<EntityColor::Color> colors;
        for (auto symbol : blobs) colors.insert(grid->getColor(symbol));

        if (colors.size() > 1) {
            for (auto i : blobs) violations.insert(i);
        }

        // Check stars
        auto stars = getActiveSymbols<Star>(grid);
        auto colorcount = getActiveColors(grid, region);

        for (auto star : stars) {
            EntityColor::Color c = grid->getColor(star);
            if (colorcount.find(c) == colorcount.end()) violations.insert(star);
            else {
                if (colorcount.at(c) != 2) violations.insert(star);
            }
        }


        /*
        
        IN ACCORDANCE WITH THE FOUR STEP PROTOCOL
        ALL SYMBOLS THAT OPERATE WITHIN A CONTINUOUS REGION AND ALSO DIRECTLY WITH THE METADATA OF OTHER SYMBOLS IN THE REGION SHOULD BE CHECKED HERE
        
        */

        // Check BlockGroups
        auto bgs = intersection(region, getActiveSymbols<BlockGroup>(grid));

        // Make the region into a shape
        Utils::pointSet convertedregion;
        for (auto i : region) convertedregion.insert({(i.first - 1) / 2, (i.second - 1) / 2});

        BlockGroup big(convertedregion);
        std::vector<BlockGroup> v;
        for (auto i : bgs) v.push_back(BlockGroup(*(dynamic_cast<BlockGroup*>(grid->get(i)))));

        if (!BlockGroup::checkPlacements(big, v, {{0, 0}, {grid->R / 2, grid->C / 2}})) {
            for (auto i : bgs) violations.insert(i);
        }

        /*
        
        IN ACCORDANCE WITH THE FOUR STEP PROTOCOL
        ALL SYMBOLS THAT CAN MODIFY OTHER SYMBOLS WITHIN ITS REGION (OR ARE MODIFIED BY OTHER SYMBOLS IN ITS REGION) SHOULD NOT BE CHECKED IN THIS METHOD
        
        */

        return violations;
    }

    Utils::pointSet getViolationsNoRecursion(Grid* grid, bool verbose = false) {
        Utils::pointSet violations;

        /*
        
        IN ACCORDANCE WITH THE FOUR STEP PROTOCOL
        ALL SYMBOLS THAT OPERATE WITHIN A LOCAL NEIGHBORHOOD (AND NOT A CONTIGUOUS REGION) SHOULD BE CHECKED HERE
        
        */

        // Check dots
        if (verbose) std::cout << "CHECK DOTS\n";
        for (auto i : getActiveSymbols<PathDot>(grid)) {
            PuzzleEntity* p = grid->get(i);
            if (auto pd = instanceof<PathDot>(p)) {
                if (p && !p->hasLine) violations.insert(i);
                uint64_t restriction = pd->restriction;
                if (restriction == 0) continue; // If restriction = 0 continue else check bits
                // std::cout << restriction << " " << (int)(p->hasLine) << "\n";
                if (restriction & (1<<(p->hasLine - 1))) continue;
                violations.insert(i);
            }
        }

        // Check triangles
        if (verbose) std::cout << "CHECK TRIANGLES\n";
        for (auto i : getActiveSymbols<Triangle>(grid)) {
            PuzzleEntity* pp = grid->get(i);
            if (auto trix = instanceof<Triangle>(pp)) {
                int cc = 0;
                for (auto n : grid->neighbors(i)) {
                    if (grid->get(n) && grid->get(n)->hasLine) cc++;
                }
                if (cc != trix->count) violations.insert(i);
            }
        }

        auto entities = getActiveSymbols<ColorEntity>(grid); // All colored symbols in the grid
        if (!entities.size()) return violations;

        auto regionsCells = getRegionsCells(grid);

        /*
        
        IN ACCORDANCE WITH THE FOUR STEP PROTOCOL
        ALL SYMBOLS THAT OPERATE WITHIN A CONTINUOUS REGION BUT ONLY INTERACT WITH OTHER SYMBOLS IN THE REGION BY THEIR TYPE OR COLOR SHOULD BE CHECKED HERE
        
        */

        // Check blobs
        if (verbose) std::cout << "CHECK BLOBS\n";
        auto blobs = getActiveSymbols<Blob>(grid);
        for (auto region : regionsCells) {
            auto p = intersection(blobs, region);
            std::set<EntityColor::Color> colors;
            for (auto symbol : p) colors.insert(grid->getColor(symbol));

            if (colors.size() > 1) {
                for (auto i : p) violations.insert(i);
            }
        }

        // Check stars
        if (verbose) std::cout << "CHECK STARS\n";
        auto stars = getActiveSymbols<Star>(grid);
        for (auto region : regionsCells) {
            auto regstars = intersection(stars, region);
            auto colorcount = getActiveColors(grid, region);

            for (auto star : regstars) {
                EntityColor::Color c = grid->getColor(star);
                if (colorcount.find(c) == colorcount.end()) violations.insert(star);
                else {
                    if (colorcount.at(c) != 2) violations.insert(star);
                }
            }
        }


        /*
        
        IN ACCORDANCE WITH THE FOUR STEP PROTOCOL
        ALL SYMBOLS THAT OPERATE WITHIN A CONTINUOUS REGION AND ALSO DIRECTLY WITH THE METADATA OF OTHER SYMBOLS IN THE REGION SHOULD BE CHECKED HERE
        
        */

        // Check BlockGroups
        if (verbose) std::cout << "CHECK BGS\n";
        auto bgs = getActiveSymbols<BlockGroup>(grid);
        if (bgs.size()) {

        if (verbose) std::cout << "REGION COUNT " << regionsCells.size() << "\n";

        for (auto region : regionsCells) {
            if (verbose) std::cout << grid->highlightCells(region) << "\n";
            auto regbgs = intersection(bgs, region);
            if (verbose) std::cout << "BGS in region: " << regbgs.size() << "\n";
            if (!regbgs.size()) continue;


            // Make the region into a shape
            Utils::pointSet convertedregion;
            for (auto i : region) convertedregion.insert({(i.first - 1) / 2, (i.second - 1) / 2});

            BlockGroup big(convertedregion);
            std::vector<BlockGroup> v;
            for (auto i : regbgs) v.push_back(BlockGroup(*(dynamic_cast<BlockGroup*>(grid->get(i)))));

            if (!BlockGroup::checkPlacements(big, v, {{0, 0}, {grid->R / 2, grid->C / 2}})) {
                if (verbose) std::cout << "BAD BLOCKGROUP\n";
                for (auto i : regbgs) violations.insert(i);
            }
        }

        }

        /*
        
        IN ACCORDANCE WITH THE FOUR STEP PROTOCOL
        ALL SYMBOLS THAT CAN MODIFY OTHER SYMBOLS WITHIN ITS REGION (OR ARE MODIFIED BY OTHER SYMBOLS IN ITS REGION) SHOULD NOT BE CHECKED IN THIS METHOD
        
        */

        return violations;
    }

    Utils::pointSet getViolations(Grid* grid, bool verbose = false) {
        
        // IN ACCORDANCE WITH THE FOUR STEP PROTOCOL
        // ALL SYMBOLS THAT CAN MODIFY OTHER SYMBOLS WITHIN ITS REGION (OR ARE MODIFIED BY OTHER SYMBOLS IN ITS REGION) SHOULD BE CHECKED IN THIS METHOD
        // YOU MUST DECIDE THE ORDER THE CHECKS GO IN. IN VANILLA WITNESS THERE IS ONLY ONE SUCH SYMBOL BUT THERE MIGHT BE MORE.

        /*
        
        THe way the cancel symbols work is that:

        inside a region, iterating across all cancels:
            we iterate through all other symbols that have been judged as wrong, removing one at a time and replacing afterwards
                for each symbol: remove the cancel, and recursively check if the resulting grid is solvable
            if we get a case where the solvable is true then the cancel is judged as right and move to the next cancel
            otherwise the cancel is marked as wrong and added to the list of region violations.

            At the end of processing a region all stuff in the region violations is added to the total violations.
        
        */

        Utils::pointSet violations = getViolationsNoRecursion(grid, verbose);

        if (verbose) std::cout << "Validated non-recursive stuff\n";

        Utils::pointSet res; // Final violations list

        // Check cancels
        Utils::pointSet deactivated;
        auto cancels = getActiveSymbols<Cancel>(grid);
        if (cancels.size() > 0) {
            auto regions = getRegions(grid);
            std::vector<Utils::pointSet> regionsCells;

            for (auto region : regions) {
                regionsCells.push_back(Utils::pointSet());
                for (auto p : region) {
                    if (p.first & 1 && p.second & 1) regionsCells[regionsCells.size() - 1].insert(p);
                }
            }
            if (verbose) std::cout << "Got regions\n";
            
            for (auto region : regions) { // For each region...
                auto cancelsInRegion = intersection<Utils::point>(region, cancels);
                auto violationsInRegion = intersection<Utils::point>(region, violations); // This is the set of violations in the region
                Utils::pointSet cancelViolations;

                /*
                
                Iterate across cancels. Each cancel will attempt to remove a single violation, taking it in the process.
                If there are no violations to remove, the cancel itself will be judged as a violation.

                It removes the violating symbol, taking it in the process. The board is then recursively checked with those two symbols deactivated.
                The board will return a set of violations. If this set is nonempty for all symbols then we add a cancel to the violation and continues.
                Otherwise we have reached a point where the board is completely solvable this way so this region is cleared.
                
                */

                const bool DEBUG = false;

                if (DEBUG) std::cout << Utils::disp(cancels) << "CC\n";

                for (auto cancel : cancelsInRegion) {
                    bool solvable = false;
                    if (violationsInRegion.size() <= 0) {
                        if (DEBUG)  std::cout << "[" << cancel.first << " " << cancel.second << "] HAS NO VIOLATIONS SO IT IS ONE\n";
                        violationsInRegion.insert(cancel);
                        cancelViolations.insert(cancel);
                        continue;
                    }
                    grid->board[cancel.first][cancel.second]->isActive = false;
                    for (auto v : violationsInRegion) {
                        if (DEBUG)  std::cout << "[" << cancel.first << " " << cancel.second << "]/[" << v.first << " " << v.second << "] TESTING\n";
                        grid->board[v.first][v.second]->isActive = false;
                        if (!solvable) {
                            auto vvv = getViolations(grid);
                            if (DEBUG)  std::cout << "[" << cancel.first << " " << cancel.second << "]/[" << v.first << " " << v.second << "]" << Utils::disp(vvv) << " sss\n";
                            solvable |= (vvv.size() <= 0);
                        }
                        grid->board[v.first][v.second]->isActive = true;
                    }
                    grid->board[cancel.first][cancel.second]->isActive = true;

                    if (DEBUG)  std::cout << "[" << cancel.first << " " << cancel.second << "]" << solvable << "\n";

                    if (!solvable) {
                        violationsInRegion.insert(cancel);
                        cancelViolations.insert(cancel);
                    } else {
                        violationsInRegion.clear();
                        break;
                    }
                }
                for (auto i : violationsInRegion) res.insert(i);
            }
        } else return violations;
        return res;
    }

    // Validate the entire fucking grid

    bool Validate(Grid* grid, bool verbose = false) {
        if (!grid->validatePath()) {
            if (verbose) std::cout << "BAD PATH\n";
            return false;
        }

        if (verbose) std::cout << "Path validated\n";
        
        return getViolations(grid, verbose).size() == 0; // Tentative!!!!
    }

    bool validate(Grid* grid, bool verbose = false) {
        return Validate(grid, verbose);
    }
}  

#endif