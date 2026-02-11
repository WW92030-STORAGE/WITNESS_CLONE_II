#ifndef SYMGRID_WITCLN
#define SYMGRID_WITCLN

#include <iostream>

#include <climits>
#include <vector>
#include <string>
#include <algorithm>
#include <cstdint>

#include "PuzzleEntity.h"
#include "Utils.h"
#include "Grid.h"

/*
Symmetry grids with two lines going across them. 
*/

class RotationalGrid : public Grid {
    public:
    RotationalGrid() : Grid() {

    }

    RotationalGrid(int A, int B) : Grid(A, B) {

    }

    virtual void setLine(int x, int y, uint8_t z) {
        Grid::setLine(x, y, z);
        Grid::setLine(R - x - 1, C - y - 1, (z == 0) ? (z) : (z + 1));
    }

    /*
    virtual void drawLine(int x1, int y1, int x2, int y2, uint8_t index = 1) {
        Grid::drawLine(x1, y1, x2, y2, index);
        Grid::drawLine(R - x1 - 1, C - y1 - 1, R - x2 - 1, C - y2 - 1, (index == 0) ? (index) : (index + 1));
    }
    */

    virtual bool isPathable(int x, int y) {
        for (int i = 0; i < 2; i++) {
            if (!inBounds(x, y)) return false;
            auto p = board[x][y];
            if (!p->isPath) return false;
            if (p->isBlocker) return false;
            if (p->hasLine) return false;
            x = R - x - 1;
            y = C - y - 1;
        }
        return true;
    }

    virtual void defaultDiagonal() {
        set(0, 0, new Endpoint(true));
        set(R - 1, C - 1, new Endpoint(true));
        set(0, C - 1, new Endpoint(false));
        set(R - 1, 0, new Endpoint(false));
        defaultPaths();
    }
};

class VSymmetryGrid : public Grid {
    public:
    VSymmetryGrid() : Grid() {

    }

    VSymmetryGrid(int A, int B) : Grid(A, B) {

    }

    virtual void setLine(int x, int y, uint8_t z) {
        Grid::setLine(x, y, z);
        Grid::setLine(x, C - y - 1, (z == 0) ? (z) : (z + 1));
    }

    /*
    virtual void drawLine(int x1, int y1, int x2, int y2, uint8_t index = 1) {
        Grid::drawLine(x1, y1, x2, y2, index);
        Grid::drawLine(x1, C - y1 - 1, x2, C - y2 - 1, (index == 0) ? (index) : (index + 1));
    }
    */

    virtual bool isPathable(int x, int y) {
        for (int i = 0; i < 2; i++) {
            if (!inBounds(x, y)) return false;
            auto p = board[x][y];
            if (!p->isPath) return false;
            if (p->isBlocker) return false;
            if (p->hasLine) return false;
            // x = R - x - 1;
            y = C - y - 1;
        }
        return true;
    }

    virtual void defaultDiagonal() {
        set(0, 0, new Endpoint(true));
        set(R - 1, C - 1, new Endpoint(false));
        set(0, C - 1, new Endpoint(true));
        set(R - 1, 0, new Endpoint(false));
        defaultPaths();
    }
};

class HSymmetryGrid : public Grid {
    public:
    HSymmetryGrid() : Grid() {

    }

    HSymmetryGrid(int A, int B) : Grid(A, B) {

    }

    virtual void setLine(int x, int y, uint8_t z) {
        Grid::setLine(x, y, z);
        Grid::setLine(R - x - 1, y, (z == 0) ? (z) : (z + 1));
    }
    /*
    virtual void drawLine(int x1, int y1, int x2, int y2, uint8_t index = 1) {
        Grid::drawLine(x1, y1, x2, y2, index);
        Grid::drawLine(R - x1 - 1, y1, R - x2 - 1, y2, (index == 0) ? (index) : (index + 1));
    }
    */

    virtual bool isPathable(int x, int y) {
        for (int i = 0; i < 2; i++) {
            if (!inBounds(x, y)) return false;
            auto p = board[x][y];
            if (!p->isPath) return false;
            if (p->isBlocker) return false;
            if (p->hasLine) return false;
            x = R - x - 1;
            // y = C - y - 1;
        }
        return true;
    }

    virtual void defaultDiagonal() {
        set(0, 0, new Endpoint(true));
        set(R - 1, C - 1, new Endpoint(false));
        set(0, C - 1, new Endpoint(false));
        set(R - 1, 0, new Endpoint(true));
        defaultPaths();
    }
};

#endif