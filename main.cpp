#include "WITNESS_EVERYTHINGII.h"

#include <bits/stdc++.h>
using namespace std;

void simpletest() {
        PuzzleEntity* pe = new Endpoint();
    std::cout << pe->type << "\n";
    std::cout << instanceof<Endpoint>(pe) << "\n";
    delete pe;

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

    Grid grid(9, 9);

    grid.set(0, 0, new Endpoint(true));
    grid.set(8, 8, new Endpoint(false));
    grid.set(1, 1, new BlockGroup(BGS::O));
    grid.set(1, 7, new BlockGroup(BGS::O));

    grid.defaultPaths();

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

    Grid grid(9, 9); // 4x4 internal cells

    grid.set(0, 0, new Endpoint(true));
    grid.set(8, 8, new Endpoint(false));
    grid.set(1, 1, new BlockGroup(BGS::hdomino));
    grid.set(1, 3, new BlockGroup(BGS::hdomino));

    BlockGroup invO(BGS::O);
    invO.value = -1; // <<<<<<<<<<<<<<<<<<<<<<<
    grid.set(5, 7, new BlockGroup(invO));

    grid.defaultPaths();

    grid.drawLine(0, 0, 8, 0);
    grid.drawLine(8, 0, 8, 8);


    std::cout << grid.to_string() << "\n";
    std::cout << "VERDICT: " << GridUtils::Validate(&grid) << "\n";
}

void simplecancel() {

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

    grid.drawLine(0, 0, 2, 0);
    grid.drawLine(2, 0, 2, 4);
    grid.drawLine(2, 4, 6, 4);
    grid.drawLine(6, 4, 6 ,6);


    std::cout << grid.to_string() << "\n";
    std::cout << "VERDICT: " << GridUtils::Validate(&grid) << "\n";
}

void cancelbg2() {

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

    Grid grid(9, 9);
    grid.defaultDiagonal();

    grid.set(1, 1, new Cancel(EntityColor::RGB_RED));
    grid.set(1, 3, new Cancel(EntityColor::RGB_RED));
    grid.set(1, 5, new Cancel(EntityColor::RGB_RED));
    grid.set(1, 7, new Cancel(EntityColor::RGB_RED));

    grid.set(1, 7, new Star(EntityColor::RGB_RED));

    grid.drawLine(0, 0, 8, 0);
    grid.drawLine(8, 0, 8, 8);

    std::cout << grid.to_string() << "\n";
    std::cout << "VERDICT: " << GridUtils::Validate(&grid) << "\n";
}

void cancelcolors2() {

    Grid grid(9, 9);
    grid.defaultDiagonal();

    grid.set(1, 7, new Cancel(EntityColor::RGB_RED));
    grid.set(3, 5, new Star(EntityColor::RGB_RED));
    grid.set(5, 3, new Star(EntityColor::RGB_BLUE));
    grid.set(7, 1, new Cancel(EntityColor::RGB_BLUE));

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

    Grid grid(9, 9);
    grid.defaultDiagonal();

    grid.set(0, 8, new PathDot());
    grid.set(7, 1, new Cancel());

    const bool FAIL = false;

    grid.drawLine(0, 0, 8, 0);
    grid.drawLine(8, 0, 8, 8);
    std::cout << grid.to_string() << "\n";
    std::cout << "VERDICT: " << GridUtils::Validate(&grid) << "\n";
}


int main() {
    srand(time(0));

	std::cout << "BEGIN\n";
	auto start = std::chrono::high_resolution_clock::now();





    bggrid3();









	auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

	int64_t count = duration.count();
    std::cout << "Time taken: " << count << " us" << std::endl;
	std::cout << "Time taken: " << count * 0.001 << " ms" << std::endl;
	std::cout << "Time taken: " << count * 0.000001 << " s" << std::endl;
    return 0;
}