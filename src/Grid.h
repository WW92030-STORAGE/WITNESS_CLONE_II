#ifndef GRID_WITCLN
#define GRID_WITCLN

// #include <iostream>

#include <climits>
#include <vector>
#include <string>
#include <algorithm>
#include <cstdint>

#include "PuzzleEntity.h"
#include "BlockGroup.h"
#include "Utils.h"

/*

Class representing a grid of PuzzleEntity objects. For inheritance checks (e.g. instanceof) the grid is a matrix of pointer values.
All entries with any even index are denoted as edges. All others (entries with both odd indices) are denoted as cells.

*/

// Copy things

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

    Grid(const Grid& other) {
        R = other.R;
        C = other.C;
        init();
        for (int r = 0; r < R; r++) {
            for (int c = 0; c < C; c++) {
                set({r, c}, other.board[r][c]->clone());
            }
        }
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

    inline bool inBounds(int x, int y) {
        if (x < 0 || y < 0 || x >= R || y >= C) return false;
        return true;
    }

    inline bool inBounds(Utils::point p) {
        return inBounds(p.first, p.second);
    }

    // Can I draw a line here where there was not?
    virtual bool isPathable(int x, int y) {
        if (!inBounds(x, y)) return false;
        auto p = board[x][y];
        if (!p->isPath) return false;
        if (p->isBlocker) return false;
        if (p->hasLine) return false;
        return true;
    }

    inline bool isPathable(Utils::point p) {
        return isPathable(p.first, p.second);
    }

    inline PuzzleEntity* get(int x, int y) {
        if (!inBounds(x, y)) return nullptr;
        return board[x][y];
    }

    inline PuzzleEntity* get(Utils::point p) {
        return get(p.first, p.second);
    }

    inline EntityColor::Color getColor(int x, int y) {
        if (!get(x, y)) return EntityColor::NIL;
        return get(x, y)->color;
    }

    inline EntityColor::Color getColor(Utils::point p) {
        return getColor(p.first, p.second);
    }

    // Get the neighbors of a point. This might include non adjacent points.
    virtual Utils::pointSet neighbors(Utils::point p, int i = 1) {
        Utils::pointSet res;

        for (int d = 0; d < 4; d++) {
            Utils::point next = {p.first + i * Utils::dx[d], p.second + i * Utils::dy[d]};
            if (!inBounds(next)) continue;
            res.insert(next);
        }
        return res;
    }

    /*
    
    MODIFICATIONS TO THE GRID
    
    */

    // Resets a cell
    inline void reset(int r, int c) {
        PuzzleEntity* pe = board[r][c];
        board[r][c] = new PuzzleEntity();

        delete pe;
    }

    // Sets a cell to some thing. IT IS HIGHLY RECOMMENDED TO USE THIS INSTEAD OF SIMPLY SAYING board[r][c] = ...
    inline void set(int r, int c, PuzzleEntity* p) {
        PuzzleEntity* pe = board[r][c]; // remember that `pe` is simply a number which is the address of the original object in memory
        board[r][c] = p; // place the address of our new object into the board
        delete pe; // delete the original object
    }

    inline void set(Utils::point p, PuzzleEntity* pe) {
        set(p.first, p.second, pe);
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

    // Set the isPath of a cell

    inline void setPath(int x, int y, bool z) {
        if (x < 0 || y < 0 || x >= R || y >= C) return;
        board[x][y]->isPath = z;
    }

    inline void setPath(Utils::point p, bool z) {
        setPath(p.first, p.second, z);
    }

    // Set the hasLine of a cell

    virtual void setLine(int x, int y, uint8_t z) {
        if (x < 0 || y < 0 || x >= R || y >= C) return;
        board[x][y]->hasLine = z;
    }

    void setLine(Utils::point p, uint8_t z) {
        setLine(p.first, p.second, z);
    }

    void setBlocker(int x, int y, bool z) {
        if (x < 0 || y < 0 || x >= R || y >= C) return;
        board[x][y]->isBlocker = z;
    }

    void setBlocker(Utils::point p, bool z) {
        setBlocker(p.first, p.second, z);
    }

    // Draw a line. Does not do anything if the boundaries are not a straight line

    virtual void drawLine(int x1, int y1, int x2, int y2, uint8_t index = 1) {
        if (x1 != x2 && y1 != y2) return;
        if (x1 > x2) std::swap(x1, x2);
        if (y1 > y2) std::swap(y1, y2);
        for (int i = x1; i <= x2; i++) {
            for (int j = y1; j <= y2; j++) setLine(i, j, index);
        }
    }

    virtual void drawLine(Utils::point a, Utils::point b, uint8_t index = 1) {
        drawLine(a.first, a.second, b.first, b.second, index);
    }
    
    // Draw a path between points
    virtual void drawPath(std::vector<Utils::point> path, uint8_t index = 1) {
        for (int i = 0; i < path.size() - 1; i++) drawLine(path[i], path[i + 1], index);
    }

    // Erases a line. Does not do checks on whether the result is continuous.
    virtual void clearLine(int x1, int y1, int x2, int y2) {
        drawLine(x1, y1, x2, y2, 0);
    }

    virtual void clearAllLines() {
        for (int i = 0; i < R; i++) {
            for (int j = 0; j < C; j++) setLine(i, j, 0);
        }
    }

    /*
    
    VALIDATION UTILS (Not a complete validation)
    
    */

    // Get a single line, the line containing a specified point. This selects all points with a specified color. 
    
    Utils::pointSet getSingleLine(Utils::point p) {
        Utils::pointSet res; 
        int lineColor = get(p)->hasLine;
        if (!lineColor) return res;

        for (int r = 0; r < R; r++) {
            for (int c = 0; c < C; c++) {
                if (board[r][c]->hasLine == lineColor) res.insert({r, c});
            }
        }
        return res;
    }

    std::vector<Utils::pointSet> getLines() {
        std::vector<Utils::pointSet> pp;
        Utils::pointSet vis;

        for (int r = 0; r < R; r++) {
            for (int c = 0; c < C; c++) {
                if (!get(r, c)->hasLine) continue;
                if (vis.find({r, c}) != vis.end()) continue;
                auto line = getSingleLine({r, c});
                for (auto ll : line) vis.insert(ll);
                if (line.size() > 0) pp.push_back(line);
            }
        }

        return pp;
    }

    // A valid path in this grid has two endpoints with only one neighbor, all others have two degree, and the graph is connected.
    bool validateSinglePath(Utils::point p) {
        auto line = getSingleLine(p);
        Utils::pointSet activeStarts, activeEnds;
        // grab the start and end points of the grid, that are also on this line
        Utils::point theStart; // ideally only one here. 
        Utils::point theEnd;
        for (auto i : line) {
            PuzzleEntity* pp = get(i);
            if (auto ep = instanceof<Endpoint>(pp)) {
                // endpoints must have degree 1
                if (intersection<Utils::point>(neighbors(i), line).size() != 1) continue;
                if (ep->isStart) {
                    theStart = i;
                    activeStarts.insert(i);
                }
                else {
                    activeEnds.insert(i);
                    theEnd = i;
                }
            } // Start/ends on the path are not counted since their degree is not 1.
        }
        if (activeStarts.size() != 1 || activeEnds.size() != 1) return 0;

        // make sure all other nodes have degree 2
        for (auto i : line) {
            if (Utils::contains(activeStarts, i)) continue;
            if (Utils::contains(activeEnds, i)) continue;
            if (intersection<Utils::point>(neighbors(i), line).size() != 2) return 0;
        }

        // Now, at this point, there are two remaining types of graphs. One is a path. The other is a pair, and a cycle.
        // Luckily for us in most cases (unless you do something really stupid) the condition is actually really simple
        // If there are no degree 2 nodes, then check if the start connects to the end.
        // Otherwise check if the start connects to an internal node.

        if (line.size() > 2) return intersection<Utils::point>(neighbors(theStart), line).size() && !Utils::contains(neighbors(theEnd), theStart);

        return Utils::contains(neighbors(theEnd), theStart);
    }

    bool validatePath() {
        auto ss = getLines();
        for (auto line : ss) {
            if (!validateSinglePath(*(line.begin()))) return false;
        }
        return true;
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
                if (board[r][c]->hasLine) {
                    uint8_t x = board[r][c]->hasLine;
                    delim = '#';
                    if (x == 1) delim = '#';
                    if (x == 2) delim = '@';
                    if (x == 3) delim = '$';
                    if (x == 4) delim = '%';
                }
                else if (board[r][c]->isPath) delim = '+';
                
                
                res = res + delim + board[r][c]->disp + delim + " ";
            }
            res = res + "\n";
        }
        return res;
    }

    // Highlight region

    std::string highlightCells(Utils::pointSet s) {
        std::string res = "";
        for (int c = C - 2; c >= 0; c -= 2) {
            for (int r = 1; r < R; r += 2) {
                if (Utils::contains(s, {r, c})) res.push_back('#');
                else res.push_back('.');
            }
            res.push_back('\n');
        }
        return res;
    }

    std::string highlight(Utils::pointSet s, bool dl = true) {
        std::string res = "";
        for (int c = C - 1; c >= 0; c --) {
            for (int r = 0; r < R; r ++) {
                if (dl && get(r, c)->hasLine) res.push_back('O');
                else if (Utils::contains(s, {r, c})) res.push_back('#');
                else res.push_back('.');
            }
            res.push_back('\n');
        }
        return res;
    }
};

#endif