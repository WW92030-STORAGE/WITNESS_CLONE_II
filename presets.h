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