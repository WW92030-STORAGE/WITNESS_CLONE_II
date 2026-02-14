#include "WITNESS_EVERYTHINGII.h"

#include <bits/stdc++.h>
using namespace std;

#include "presets.h"
#include "blackboxtests.h"

void unit_test_grid(std::pair<Grid, int> (*func)() = testcase1) {
    Solver solver;

    // use a unit test
    std::pair<Grid, int> test = func();

    
    Grid g = test.first;
    int numsols = test.second;
    solver.grid = &g;

    solver.solve(INT_MAX);

    std::cout << solver.solutions.size() << "/" << numsols << "\n";

    assert(solver.solutions.size() == numsols);
}

void unit_test_rot(std::pair<RotationalGrid, int> (*func)() = rotcase1) {
    Solver solver;

    // use a unit test
    std::pair<RotationalGrid, int> test = func();

    
    RotationalGrid g = test.first;
    int numsols = test.second;
    solver.grid = &g;

    solver.solve(INT_MAX);

    std::cout << solver.solutions.size() << "/" << numsols << "\n";

    assert(solver.solutions.size() == numsols);
}

int main() {
    srand(42069);

	std::cout << "BEGIN\n";
	auto start = std::chrono::high_resolution_clock::now();


    unit_test_grid(testcase1);
    unit_test_grid(testcase2);
    unit_test_grid(testcase3);

    unit_test_rot(rotcase1);
    unit_test_rot(rotcase2);
    unit_test_rot(rotcase3);

    randblocktest();

	auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

	int64_t count = duration.count();
    std::cout << "Time taken: " << count << " us" << std::endl;
	std::cout << "Time taken: " << count * 0.001 << " ms" << std::endl;
	std::cout << "Time taken: " << count * 0.000001 << " s" << std::endl;
    return 0;
}