#include "WITNESS_EVERYTHINGII.h"

#include <bits/stdc++.h>
using namespace std;

#include "presets.h"

void simpletest() {
        PuzzleEntity* pe = new Endpoint();
    std::cout << pe->type << "\n";
    std::cout << instanceof<Endpoint>(pe) << "\n";
    delete pe;

    auto grid = simplegrid();

    grid.drawLine(0, 0, 4, 0);
    grid.drawLine(4, 0, 4, 8);
    grid.drawLine(4, 8, 8, 8);

    /*

    grid.set(1, 0, new Endpoint(false));
    grid.drawLine(0, 0, 1, 0);
    grid.drawLine(3, 3, 5, 3);
    grid.drawLine(3, 3, 3, 5);
    grid.drawLine(5, 5, 5, 3);
    grid.drawLine(5, 5, 3, 5);

    */


    std::cout << grid.to_string() << "\n";

    auto yourmom = GridUtils::getRegionsCells(&grid);
    for (auto sss : yourmom) std::cout << Utils::disp(sss) << "\n";
    std::cout << "VERDICT: " << GridUtils::Validate(&grid) << "\n";
}

void simpletrix() {

    auto grid = simpletriangle();

    grid.drawLine(0, 0, 0, 8);
    grid.drawLine(0, 8, 4, 8);
    grid.drawLine(4, 8, 4, 6);
    grid.drawLine(4, 6, 2, 6);
    grid.drawLine(2, 6, 2, 2);
    grid.drawLine(2, 2, 8, 2);
    grid.drawLine(8, 2, 8, 8);

    std::cout << grid.to_string() << "\n";
    std::cout << "VERDICT: " << GridUtils::Validate(&grid) << "\n";
}

void bgunit() {
    BlockGroup hd(BGS::hdomino);
    BlockGroup cell(BGS::unit);
    cell.translate({0, 1});
    BlockGroup L(BGS::corner);
    L.rotate(2);
    L.fixed = false;

    BlockGroup bigone(std::vector<Utils::point>{{0, 0}, {0, 1}, {0, 2}, {1, 0}, {1, 1}, {2, 0}});
    cout << BlockGroup::checkPlacements(bigone, vector<BlockGroup>({L, hd, cell}));
}

void bggrid() {
    Grid grid = simplebg();

    grid.drawLine(0, 0, 4, 0);
    grid.drawLine(4, 0, 4, 8);
    grid.drawLine(4, 8, 8, 8);

    /*

    grid.set(1, 0, new Endpoint(false));
    grid.drawLine(0, 0, 1, 0);
    grid.drawLine(3, 3, 5, 3);
    grid.drawLine(3, 3, 3, 5);
    grid.drawLine(5, 5, 5, 3);
    grid.drawLine(5, 5, 3, 5);

    */


    std::cout << grid.to_string() << "\n";

    auto yourmom = GridUtils::getRegionsCells(&grid);
    for (auto sss : yourmom) std::cout << Utils::disp(sss) << "\n";
    std::cout << "VERDICT: " << GridUtils::Validate(&grid) << "\n";
}

void bggrid2() {

    Grid grid = bgwithinv();

    grid.drawLine(0, 0, 0, 2);
    grid.drawLine(0, 2, 6, 2);
    grid.drawLine(6, 2, 6, 6);
    grid.drawLine(6, 6, 4, 6);
    grid.drawLine(4, 6, 4, 8);
    grid.drawLine(4, 8, 8, 8);


    std::cout << grid.to_string() << "\n";
    std::cout << "VERDICT: " << GridUtils::Validate(&grid) << "\n";
}

void bggrid3() {

    Grid grid = nullbg();

    grid.drawLine(0, 0, 8, 0);
    grid.drawLine(8, 0, 8, 8);


    std::cout << grid.to_string() << "\n";
    std::cout << "VERDICT: " << GridUtils::Validate(&grid) << "\n";
}

void simplecancel() {

    Grid grid = cancelblobs();

    const bool LSHAPE = false;

    if (LSHAPE) {

    grid.drawLine(0, 0, 4, 0);
    grid.drawLine(4, 0, 4, 8);
    grid.drawLine(4, 8, 8, 8);

    } else {
        grid.drawLine(0, 0, 0, 4);
        grid.drawLine(0, 4, 4, 4);
        grid.drawLine(4, 4, 4, 8);
        grid.drawLine(4, 8, 8, 8);
    }


    std::cout << grid.to_string() << "\n";
    std::cout << "VERDICT: " << GridUtils::Validate(&grid) << "\n";
}


void cancelbg() {
    Grid grid = cancelwithbgs();

    grid.drawLine(0, 0, 2, 0);
    grid.drawLine(2, 0, 2, 4);
    grid.drawLine(2, 4, 6, 4);
    grid.drawLine(6, 4, 6 ,6);


    std::cout << grid.to_string() << "\n";
    std::cout << "VERDICT: " << GridUtils::Validate(&grid) << "\n";
}

void cancelbg2() {
    Grid grid = cancelwithbgs2();



    grid.drawLine(0, 0, 0, 2);
    grid.drawLine(0, 2, 8, 2);
    grid.drawLine(8, 2, 8, 6);
    grid.drawLine(8, 6, 4, 6);
    grid.drawLine(4, 6, 4, 8);
    grid.drawLine(4, 8, 8, 8);


    std::cout << grid.to_string() << "\n";
    std::cout << "VERDICT: " << GridUtils::Validate(&grid) << "\n";
}

void cancelcolors() {
    Grid grid = cancelstarscolors();

    grid.drawLine(0, 0, 2, 0);
    grid.drawLine(2, 0, 2, 8);
    grid.drawLine(2, 8, 6, 8);
    grid.drawLine(6, 8, 6, 6);
    grid.drawLine(6, 6, 4, 6);
    grid.drawLine(4, 6, 4, 0);
    grid.drawLine(4, 0, 6, 0);
    grid.drawLine(6, 0, 6, 4);
    grid.drawLine(6, 4, 8, 4);
    grid.drawLine(8, 4, 8, 8);

    std::cout << grid.to_string() << "\n";
    std::cout << "VERDICT: " << GridUtils::Validate(&grid) << "\n";
}

void multiplecancels() {
    Grid grid = cancel4x();

    grid.drawLine(0, 0, 8, 0);
    grid.drawLine(8, 0, 8, 8);

    std::cout << grid.to_string() << "\n";
    std::cout << "VERDICT: " << GridUtils::Validate(&grid) << "\n";
}

void cancelcolors2() {
    Grid grid = cancelcolorsdiag();


    const bool FAIL = false;

    if (FAIL) {

    grid.drawLine(0, 0, 8, 0);
    grid.drawLine(8, 0, 8, 8);

    } else {
        grid.drawLine(0, 0, 2, 0);
        grid.drawLine(2, 0, 2, 6);
        grid.drawLine(2, 6, 4, 6);
        grid.drawLine(4, 6, 4, 2);
        grid.drawLine(4, 2, 6, 2);
        grid.drawLine(6, 2, 6, 8);
        grid.drawLine(6, 8, 8, 8);
    }
    std::cout << grid.to_string() << "\n";
    std::cout << "VERDICT: " << GridUtils::Validate(&grid) << "\n";
}

void canceldot() {
    Grid grid = cancelwithdot();

    const bool FAIL = false;

    grid.drawLine(0, 0, 8, 0);
    grid.drawLine(8, 0, 8, 8);
    std::cout << grid.to_string() << "\n";
    std::cout << "VERDICT: " << GridUtils::Validate(&grid) << "\n";
}


void canceldot2() {
    Grid grid = canceldotblob();

    const bool FAIL = false;

    // grid.drawLine(0, 0, 8, 0);
    grid.drawLine(0, 0, 0, 2);
    grid.drawLine(0, 2, 2, 2);
    grid.drawLine(2, 2, 2, 0);
    grid.drawLine(2, 0, 8, 0);
    grid.drawLine(8, 0, 8, 8);
    std::cout << grid.to_string() << "\n";
    std::cout << "VERDICT: " << GridUtils::Validate(&grid) << "\n";
}

void solvertest() {
    Solver solver;

    Grid g = canceldotblob();
    cout << g.to_string() << "\n";
    solver.grid = &g;

    solver.solve(INT_MAX);

    if (solver.solutions.size() > 0) std::cout << Utils::disp(solver.solutions[0]) << "\n";
    else std::cout << "NO SOLUTIONS :(\n";
    std::cout << solver.solutions.size() << " SOLUTIONS FOUND\n";

    solver.apply(0);
    std::cout << g.to_string() << "\n";
}

void rotation1() {
    RotationalGrid grid(9, 9);
    grid.defaultDiagonal();

    grid.set(3, 1, new Star(EntityColor::RGB_RED));
    grid.set(7, 1, new Star(EntityColor::RGB_RED));

    grid.set(1, 7, new Star(EntityColor::RGB_RED));
    grid.set(5, 7, new Star(EntityColor::RGB_RED));

    Solver s;
    s.grid = &grid;

    s.solve(1);
    if (s.solutions.size()) {
        std::cout << Utils::disp(s.solutions[0]) << "\n";
        s.apply(0);
    }

    std::cout << grid.to_string() << "\n";
}

void vertical1() {
    VSymmetryGrid grid(9, 9);
    grid.defaultDiagonal();

    grid.set(4, 1, new PathDot());
    grid.set(8, 1, new PathDot());
    grid.set(2, 6, new PathDot());
    grid.set(6, 7, new PathDot());

    Solver s;
    s.grid = &grid;

    s.solve(1);
    if (s.solutions.size()) {
        std::cout << Utils::disp(s.solutions[0]) << "\n";
        s.apply(0);
    }

    std::cout << grid.to_string() << "\n";
}

void horizontal1() {
    HSymmetryGrid grid(9, 9);
    grid.defaultDiagonal();

    grid.set(8, 1, new PathDot());
    grid.set(2, 6, new PathDot());
    grid.set(6, 7, new PathDot());

    Solver s;
    s.grid = &grid;

    s.solve(1);
    if (s.solutions.size()) {
        std::cout << Utils::disp(s.solutions[0]) << "\n";
        s.apply(0);
    }

    std::cout << grid.to_string() << "\n";
}

void symdot() {
    HSymmetryGrid grid(9, 9);
    grid.defaultDiagonal();

    grid.set(8, 1, new PathDot());
    grid.set(2, 6, new PathDot());
    grid.set(6, 7, new PathDot(2));

    Solver s;
    s.grid = &grid;

    s.solve(1);
    if (s.solutions.size()) {
        std::cout << Utils::disp(s.solutions[0]) << "\n";
        s.apply(0);
    }

    std::cout << grid.to_string() << "\n";
}


void symdot2() {
    RotationalGrid grid(11, 11);
    grid.defaultDiagonal();

    grid.set(6, 2, new PathDot(1));
    grid.set(4, 4, new PathDot(0));
    grid.set(2, 6, new PathDot(2));
    grid.set(10, 5, new PathDot(0));
    grid.set(1, 10, new PathDot(0));
    grid.set(6, 9, new PathDot(0));
    grid.set(10, 8, new PathDot(2));
    Solver s;
    s.grid = &grid;

    s.solve(4);
    std::cout << s.solutions.size() << "\n";
    if (s.solutions.size()) {
        std::cout << Utils::disp(s.solutions[0]) << "\n";
        s.apply(0);
    }

    std::cout << grid.to_string() << "\n";
}

void symblob() {
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



    Solver s;
    s.grid = &grid;

    s.solve(2);
    std::cout << s.solutions.size() << "\n";
    if (s.solutions.size()) {
        std::cout << Utils::disp(s.solutions[0]) << "\n";
        s.apply(0);
    }

    std::cout << grid.to_string() << "\n";
}

void badpaths() {
    RotationalGrid grid(9, 9);
    grid.defaultDiagonal();

    grid.drawLine(0, 0, 8, 0);
    grid.drawLine(0, 8, 8, 8);
    grid.drawLine(2, 2, 4, 2);

    std::cout << GridUtils::Validate(&grid) << "\n";
}


int main() {
    srand(42069);

	std::cout << "BEGIN\n";
	auto start = std::chrono::high_resolution_clock::now();


    symblob();




	auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

	int64_t count = duration.count();
    std::cout << "Time taken: " << count << " us" << std::endl;
	std::cout << "Time taken: " << count * 0.001 << " ms" << std::endl;
	std::cout << "Time taken: " << count * 0.000001 << " s" << std::endl;
    return 0;
}