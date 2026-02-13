#ifndef PRESETS_AWHGQE9GHEBG9PEBGEQBNIBOQNGBPQ
#define PRESETS_AWHGQE9GHEBG9PEBGEQBNIBOQNGBPQ

#include <bits/stdc++.h>
using namespace std;

#include "WITNESS_EVERYTHINGII.h"

Grid simplegrid() {
        Grid grid(9, 9);

    grid.set(0, 0, new Endpoint(true));
    grid.set(8, 8, new Endpoint(false));
    grid.set(4, 4, new PathDot());

    grid.set(1, 1, new Blob(EntityColor::RGB_WHITE));
    grid.set(1, 7, new Blob(EntityColor::RGB_WHITE));
    grid.set(7, 7, new Star(EntityColor::RGB_WHITE)); // violation
    grid.set(7, 1, new Star(EntityColor::RGB_BLACK));
    grid.set(7, 3, new Star(EntityColor::RGB_BLACK));

    grid.set(7, 5, new Blob(EntityColor::RGB_WHITE)); // violation

    grid.set(3, 1, new Triangle(2));

    grid.defaultPaths();
    return grid;
}

Grid simplebg() {

    Grid grid(9, 9);

    grid.set(0, 0, new Endpoint(true));
    grid.set(8, 8, new Endpoint(false));
    grid.set(1, 1, new BlockGroup(BGS::O));
    grid.set(1, 7, new BlockGroup(BGS::O));

    grid.defaultPaths();
    return grid;
}

Grid simpletriangle() {

    Grid grid(9, 9);

    grid.set(0, 0, new Endpoint(true));
    grid.set(8, 8, new Endpoint(false));
    grid.set(1, 1, new Triangle(1));
    grid.set(3, 1, new Triangle(1));
    grid.set(5, 1, new Triangle(1));
    grid.set(7, 1, new Triangle(1));

    grid.set(1, 3, new Triangle(2));
    grid.set(1, 5, new Triangle(2));
    grid.set(1, 7, new Triangle(2));


    grid.defaultPaths();
    return grid;
}

Grid bgwithinv() {
        Grid grid(9, 9); // 4x4 internal cells

    grid.set(0, 0, new Endpoint(true));
    grid.set(8, 8, new Endpoint(false));
    grid.set(1, 1, new BlockGroup(BGS::J));
    grid.set(7, 1, new BlockGroup(BGS::L));

    BlockGroup seven(BGS::L);
    seven.rotate(1);
    grid.set(7, 7, new BlockGroup(seven));

    BlockGroup invO(BGS::O);
    invO.value = -1; // <<<<<<<<<<<<<<<<<<<<<<<
    grid.set(5, 7, new BlockGroup(invO));

    grid.defaultPaths();
    return grid;
}

Grid nullbg() {
        Grid grid(9, 9); // 4x4 internal cells

    grid.set(0, 0, new Endpoint(true));
    grid.set(8, 8, new Endpoint(false));
    grid.set(1, 1, new BlockGroup(BGS::hdomino));
    grid.set(1, 3, new BlockGroup(BGS::hdomino));

    BlockGroup invO(BGS::O);
    invO.value = -1; // <<<<<<<<<<<<<<<<<<<<<<<
    grid.set(5, 7, new BlockGroup(invO));

    grid.defaultPaths();
    return grid;
}

Grid cancelblobs() {
        Grid grid(9, 9); // 4x4 internal cells

    grid.set(0, 0, new Endpoint(true));
    grid.set(8, 8, new Endpoint(false));
    grid.defaultPaths();

    grid.set(1, 1, new Blob(EntityColor::RGB_RED));
    grid.set(1, 7, new Blob(EntityColor::RGB_RED));

    grid.set(7, 1, new Blob(EntityColor::RGB_WHITE));
    grid.set(7, 7, new Blob(EntityColor::RGB_WHITE));
    grid.set(5, 3, new Cancel());
    // grid.set(5, 5, new Cancel());
    // grid.set(5, 7, new Cancel());
    return grid;
}

Grid cancelwithbgs() {
        Grid grid(7, 7); // 3x3 internal cells

    grid.set(0, 0, new Endpoint(true));
    grid.set(6, 6, new Endpoint(false));
    grid.defaultPaths();

    grid.set(1, 5, new BlockGroup(BGS::vdomino));
    grid.set(1, 3, new BlockGroup(BGS::I3));
    BlockGroup vi(BGS::I);
    vi.rotate(1);
    grid.set(5, 5, new BlockGroup(vi));

    grid.set(3, 5, new Cancel());
    return grid;
}

Grid cancelwithbgs2() {
        Grid grid(9, 9);
    grid.defaultDiagonal();

    BlockGroup vi(BGS::I3);
    vi.rotate(1);
    grid.set(5, 1, new BlockGroup(vi));
    grid.set(7, 1, new BlockGroup(BGS::I));

    grid.set(1, 7, new Star(EntityColor::RGB_RED));
    grid.set(3, 7, new Star(EntityColor::RGB_RED));
    grid.set(5, 7, new Star(EntityColor::RGB_RED));
    grid.set(7, 7, new Star(EntityColor::RGB_RED));

    grid.set(1, 1, new Cancel());
    return grid;
}

Grid cancelstarscolors() {
        Grid grid(9, 9);
    grid.defaultDiagonal();

    grid.set(1, 1, new Cancel(EntityColor::RGB_RED));
    grid.set(1, 3, new Star(EntityColor::RGB_RED));
    grid.set(1, 5, new Star(EntityColor::RGB_RED));
    grid.set(1, 7, new Star(EntityColor::RGB_RED));
    grid.set(3, 7, new Star(EntityColor::RGB_RED));
    grid.set(5, 7, new Star(EntityColor::RGB_RED));
    grid.set(7, 7, new Star(EntityColor::RGB_RED));
    grid.set(7, 5, new Star(EntityColor::RGB_RED));
    grid.set(7, 3, new Star(EntityColor::RGB_RED));
    grid.set(7, 1, new Star(EntityColor::RGB_RED));
    return grid;
}

Grid cancel4x() {
    Grid grid(9, 9);
    grid.defaultDiagonal();

    grid.set(1, 1, new Cancel(EntityColor::RGB_RED));
    grid.set(1, 3, new Cancel(EntityColor::RGB_RED));
    grid.set(1, 5, new Cancel(EntityColor::RGB_RED));
    grid.set(1, 7, new Cancel(EntityColor::RGB_RED));

    grid.set(1, 7, new Star(EntityColor::RGB_RED));
    return grid;
}

Grid cancelcolorsdiag() {
        Grid grid(9, 9);
    grid.defaultDiagonal();

    grid.set(1, 7, new Cancel(EntityColor::RGB_RED));
    grid.set(3, 5, new Star(EntityColor::RGB_RED));
    grid.set(5, 3, new Star(EntityColor::RGB_BLUE));
    grid.set(7, 1, new Cancel(EntityColor::RGB_BLUE));
    return grid;
}

Grid cancelwithdot() {
        Grid grid(9, 9);
    grid.defaultDiagonal();

    grid.set(0, 8, new PathDot());
    grid.set(7, 1, new Cancel());
    return grid;
}

Grid canceldotblob() {
        Grid grid(9, 9);
    grid.defaultDiagonal();

    grid.set(0, 8, new PathDot());
    grid.set(8, 0, new PathDot());
    grid.set(1, 1, new Blob(EntityColor::RGB_BLACK));
    grid.set(1, 7, new Blob(EntityColor::RGB_BLUE));


    grid.set(7, 1, new Cancel());
    return grid;
}

HSymmetryGrid simplesymdots() {
    HSymmetryGrid grid(9, 9);
    grid.defaultDiagonal();

    grid.set(8, 1, new PathDot());
    grid.set(2, 6, new PathDot());
    grid.set(6, 7, new PathDot(2));
    return grid;
}

RotationalGrid complexsymdots() {
        RotationalGrid grid(11, 11);
    grid.defaultDiagonal();

    grid.set(6, 2, new PathDot(1));
    grid.set(4, 4, new PathDot(0));
    grid.set(2, 6, new PathDot(2));
    grid.set(10, 5, new PathDot(0));
    grid.set(1, 10, new PathDot(0));
    grid.set(6, 9, new PathDot(0));
    grid.set(10, 8, new PathDot(2));
    return grid;
}

RotationalGrid simplesymblobs() {
        RotationalGrid grid(11, 11);
    grid.defaultDiagonal();

    grid.set(7, 1, new Blob(EntityColor::RGB_WHITE));
    grid.set(5, 3, new Blob(EntityColor::RGB_WHITE));
    grid.set(1, 3, new Blob(EntityColor::RGB_WHITE));
    grid.set(5, 9, new Blob(EntityColor::RGB_WHITE));
    
    grid.set(9, 1, new Blob(EntityColor::RGB_BLACK));
    grid.set(9, 3, new Blob(EntityColor::RGB_BLACK));
    grid.set(3, 5, new Blob(EntityColor::RGB_BLACK));
    grid.set(7, 7, new Blob(EntityColor::RGB_BLACK));
    grid.set(10, 1, new PathDot());
    return grid;
}

RotationalGrid weirddualshapes() {
    RotationalGrid grid(13, 13);
    grid.defaultDiagonal();

    grid.set(5, 1, new BlockGroup(BGS::corner));
    grid.set(7, 1, new BlockGroup(BGS::L));
    BlockGroup ss(BGS::S);
    ss.rotate(1);
    BlockGroup corner(BGS::corner);
    corner.rotate(3);

    grid.set(5, 11, new BlockGroup(ss));
    grid.set(7, 11, new BlockGroup(corner));
    return grid;
}

// Starting here a preset is of the form {Grid, N} where N is the number of solutions to the Grid (Grid can be of any type)

std::pair<Grid, int> testcase1() {
    Grid grid(9, 9);
    grid.defaultDiagonal();

    grid.set(1, 1, new Star(EntityColor::RGB_WHITE));
    grid.set(1, 5, new Star(EntityColor::RGB_WHITE));
    grid.set(5, 5, new Star(EntityColor::RGB_WHITE));
    grid.set(5, 1, new Star(EntityColor::RGB_WHITE));

    grid.set(3, 3, new Blob(EntityColor::RGB_RED));
    grid.set(7, 7, new Blob(EntityColor::RGB_RED));
    grid.set(7, 3, new Blob(EntityColor::RGB_CYAN));
    grid.set(3, 7, new Blob(EntityColor::RGB_CYAN));

    return {grid, 380};
}

std::pair<Grid, int> testcase2() {
    Grid grid(9, 9);
    grid.defaultDiagonal();

    BlockGroup lshape(BGS::L);
    lshape.fixed = false;

    grid.set(3, 3, new BlockGroup(lshape));

    return {grid, 68};
}

std::pair<Grid, int> testcase3() {
    Grid grid(9, 9);
    grid.defaultDiagonal();

    for (int i = 0; i <= 4; i++) {
        for (int j = 0; j <= 4; j++) {
            if ((i + j) & 1) grid.set(2 * i, 2 * j, new PathDot());
        }
    }

    grid.set(3, 3, new Cancel());

    return {grid, 288};
}

std::pair<Grid, int> testcase4() {
    Grid grid(9, 9);
    grid.defaultDiagonal();

    for (int i = 0; i <= 4; i++) {
        for (int j = 0; j <= 4; j++) {
            if ((i + j) & 1) grid.set(2 * i, 2 * j, new PathDot());
        }
    }

    grid.set(3, 3, new Cancel());

    return {grid, 288};
}

// Rotational grid tests

std::pair<RotationalGrid, int> rotcase1() {
    RotationalGrid grid(11, 11);
    grid.defaultDiagonal();

    grid.set(5, 3, new Triangle(2));
    grid.set(3, 7, new Triangle(2));
    grid.set(7, 9, new Triangle(1));
    grid.set(9, 5, new Triangle(1));

    return {grid, 68};
}

std::pair<RotationalGrid, int> rotcase2() {
    RotationalGrid grid(11, 11);
    grid.defaultDiagonal();

    BlockGroup rotcorner(BGS::corner);
    rotcorner.fixed = false;
    grid.set(3, 1, new BlockGroup(rotcorner));

    return {grid, 152};
}

std::pair<RotationalGrid, int> rotcase3() {
    RotationalGrid grid(11, 11);
    grid.defaultDiagonal();

    return {grid, 3340};
}



#endif