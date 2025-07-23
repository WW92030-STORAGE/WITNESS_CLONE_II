#ifndef GRID_WITCLN
#define GRID_WITCLN

// #include <iostream>

#include <climits>
#include <vector>
#include <string>
#include <algorithm>
#include <cstdint>

#include "PuzzleEntity.h"
#include "Utils.h"

/*

Class representing a grid of PuzzleEntity objects. For inheritance checks (e.g. instanceof) the grid is a matrix of pointer values.
All entries with any even index are denoted as edges. All others (entries with both odd indices) are denoted as cells.

*/

// You can make different kinds of grids later. The base grid class represents a closed grid in which one line can be drawn at a time.
class Grid {
    public:
    int R = 3;
    int C = 3;

    // WARNING - THE BOARD IS A MATRIX OF POINTERS. DO NOT DIRECTLY SET AN ENTRY TO ANOTHER ADDRESS WITHOUT DELETING THE ORIGINAL.
    // It is highly recommended to use Grid::set() to do this -- you can even just call that with (new X(...)) as the parameter.
    PuzzleEntity*** board;

    virtual void init() {
        // Make sure we have an odd number of rows and columns
        if (R % 2 == 0) R++;
        if (C % 2 == 0) C++;

        board = new PuzzleEntity**[R];
        for (int i = 0; i < R; i++) {
            board[i] = new PuzzleEntity*[C];
            for (int j = 0; j < C; j++) {
                board[i][j] = new PuzzleEntity(); // This is so that each PuzzleEntity can be used in inheritance checking later.
            }
        }
        
    }

    Grid() {
        R = 9;
        C = 9;
        init();
    }

    Grid(int A, int B) {
        R = A;
        C = B;
        init();
    }

    virtual ~Grid() {
        for (int i = 0; i < R; i++) {
            for (int j = 0; j < C; j++) delete board[i][j];
            delete[] board[i];
        }
        delete[] board;
    }

    /*
    
    READ FROM THE GRID
    
    */

    bool inBounds(int x, int y) {
        if (x < 0 || y < 0 || x >= R || y >= C) return false;
        return true;
    }

    bool inBounds(std::pair<int, int> p) {
        return inBounds(p.first, p.second);
    }

    PuzzleEntity* get(int x, int y) {
        if (!inBounds(x, y)) return nullptr;
        return board[x][y];
    }

    PuzzleEntity* get(std::pair<int, int> p) {
        return get(p.first, p.second);
    }

    EntityColor::Color getColor(int x, int y) {
        if (!get(x, y)) return EntityColor::NIL;
        return get(x, y)->color;
    }

    EntityColor::Color getColor(std::pair<int, int> p) {
        return getColor(p.first, p.second);
    }

    Utils::pointSet neighbors(std::pair<int, int> p) {
        Utils::pointSet res;

        for (int d = 0; d < 4; d++) {
            Utils::point next = {p.first + Utils::dx[d], p.second + Utils::dy[d]};
            if (!inBounds(next)) continue;
            res.insert(next);
        }
        return res;
    }

    /*
    
    MODIFICATIONS TO THE GRID
    
    */

    // Resets a cell
    void reset(int r, int c) {
        PuzzleEntity* pe = board[r][c];
        board[r][c] = new PuzzleEntity();

        delete pe;
    }

    // Sets a cell to some thing. IT IS HIGHLY RECOMMENDED TO USE THIS INSTEAD OF SIMPLY SAYING board[r][c] = ...
    void set(int r, int c, PuzzleEntity* p) {
        PuzzleEntity* pe = board[r][c]; // remember that `pe` is simply a number which is the address of the original object in memory
        board[r][c] = p; // place the address of our new object into the board
        delete pe; // delete the original object

    }

    // Remove all path blocks (except endpoints) and clear all floodfill blockers. This will also clear any entities on the paths themselves
    void clearAllPaths() {
        for (int r = 0; r < R; r++) {
            for (int c = 0; c < C; c++) {
                if (r & 1 && c & 1) continue;
                if (instanceof<Endpoint>(board[r][c])) {
                    continue;
                }

                reset(r, c);
            }
        }
    }

    // Put paths on all possible edges
    void defaultPaths() {
        for (int r = 0; r < R; r++) {
            for (int c = 0; c < C; c++) {
                if (r & 1 && c & 1) continue;
                board[r][c]->isPath = true;
            }
        }
    }

    // Put paths on all possible edges and set the start to (0, 0) and the end to top right
    void defaultDiagonal() {
        set(0, 0, new Endpoint(true));
        set(R - 1, C - 1, new Endpoint(false));
        defaultPaths();

    }

    // Set the hasLine of a cell

    void setLine(int x, int y, bool z) {
        if (x < 0 || y < 0 || x >= R || y >= C) return;
        board[x][y]->hasLine = z;
    }

    void setLine(std::pair<int, int> p, bool z) {
        setLine(p.first, p.second, z);
    }

    // TODO something about border cells that are considered non-cells, allowing the user to shape grids within the bounding box.

    // Draw a line. Does not do anything if the boundaries are not a straight line

    void drawLine(int x1, int y1, int x2, int y2) {
        if (x1 != x2 && y1 != y2) return;
        if (x1 > x2) std::swap(x1, x2);
        if (y1 > y2) std::swap(y1, y2);
        for (int i = x1; i <= x2; i++) {
            for (int j = y1; j <= y2; j++) setLine(i, j, true);
        }
    }

    // Erases a line. Does not do checks on whether the result is continuous.
    void clearLine(int x1, int y1, int x2, int y2) {
        if (x1 != x2 && y1 != y2) return;
        if (x1 > x2) std::swap(x1, x2);
        if (y1 > y2) std::swap(y1, y2);
        for (int i = x1; i <= x2; i++) {
            for (int j = y1; j <= y2; j++) setLine(i, j, false);
        }
    }

    /*
    
    VALIDATION UTILS (Not a complete validation)
    
    */
    
    virtual Utils::pointSet getLine() {
        Utils::pointSet res; 
        for (int r = 0; r < R; r++) {
            for (int c = 0; c < C; c++) {
                if (get(r, c) && get(r, c)->hasLine) res.insert({r, c});
            }
        }
        return res;
    }

    // A valid path in this grid has two endpoints with only one neighbor, all others have two degree, and the graph is connected.
    virtual bool validatePath() {
        auto line = getLine();
        Utils::pointSet activeStarts, activeEnds;
        for (auto i : line) {
            PuzzleEntity* pp = get(i);
            if (instanceof<Endpoint>(pp)) {
                if (intersection<Utils::point>(neighbors(i), line).size() != 1) continue;
                if (dynamic_cast<Endpoint*>(pp)->isStart) activeStarts.insert(i);
                else activeEnds.insert(i);
            }
        }
        if (activeStarts.size() != 1 || activeEnds.size() != 1) return 0;

        for (auto i : line) {
            if (Utils::contains(activeStarts, i)) continue;
            if (Utils::contains(activeEnds, i)) continue;
            if (intersection<Utils::point>(neighbors(i), line).size() != 2) return 0;
        }

        // Ensure the graph is connected
        Utils::pointQueue q;
        Utils::pointSet ps;
        q.push(*(activeStarts.begin()));
        while (q.size()) {
            Utils::point now = q.front();
            q.pop();

            for (auto i : neighbors(now)) {
                if (Utils::contains<Utils::point>(ps, i)) continue;
                if (!get(i)->hasLine) continue;
                ps.insert(i);
                q.push(i);
            }
        }

        return Utils::contains<Utils::point>(ps, *(activeEnds.begin()));
    }

    /*
    
    UTILS
    
    */

    // Display the grid

    std::string to_string() {
        std::string res = "Grid[" + std::to_string(R) + ", " + std::to_string(C) + "]\n";
        for (int c = C - 1; c >= 0; c--) {
            for (int r = 0; r < R; r++) {
                char delim = '.';
                if (board[r][c]->hasLine) delim = '#';
                else if (board[r][c]->isPath) delim = '+';
                
                
                res = res + delim + board[r][c]->disp + delim + " ";
            }
            res = res + "\n";
        }
        return res;
    }
};

#endif