#include "WITNESS_EVERYTHINGII.h"

#include <bits/stdc++.h>
#include "presets.h"
using namespace std;
using namespace Utils;
using namespace EntityColor;


/*

Tests on grids to ensure that all of them have valid solutions.

*/

// 8 stars 2 colors
void stars4x2test() {
    int64_t ERRS = 0;
    int64_t DENOM = 0;

    int R = 9;
    int C = 9;

    Solver solver;

    vector<point> cells;
    for (int r = 1; r < R; r += 2) {
        for (int c = 1; c < C; c += 2) cells.push_back({r, c});
    }

    std::cout << cells.size() << "\n";

    for (int i1 = 0; i1 < cells.size(); i1++) {
        for (int i2 = i1 + 1; i2 < cells.size(); i2++) {
            for (int i3 = i2 + 1; i3 < cells.size(); i3++) {
                for (int i4 = i3 + 1; i4 < cells.size(); i4++) {
                    for (int i5 = i4 + 1; i5 < cells.size(); i5++) {
                        for (int i6 = i5 + 1; i6 < cells.size(); i6++) {
                            for (int i7 = i6 + 1; i7 < cells.size(); i7++) {
                                for (int i8 = i7 + 1; i8 < cells.size(); i8++) {
                                    for (int bits = 0; bits < 128; bits++) {
                                        if (__builtin_popcount(bits) != 4) continue;

                                        vector<int> iv({i1, i2, i3, i4, i5, i6, i7, i8});
                                        vector<point> pv;
                                        for (auto i : iv) pv.push_back(cells[i]);
                                        if (!(DENOM &255)) std::cout << DENOM << "\n";

                                        Grid g(9, 9);
                                        g.defaultDiagonal();
                                        for (int i = 0; i < 8; i++) g.set(pv[i], new Star((bits & (1<<i)) ? RGB_RED : RGB_CYAN));

                                        solver.grid = &g;
                                        solver.solve();
                                        if (solver.solutions.size() <= 0) {
                                            std::cout << "ERR ON PUZZLE " << DENOM << "\n";
                                            ERRS++;
                                        }

                                        DENOM++;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    

    std::cout << ERRS << "/" << DENOM << " INVALID GRIDS\n";
}

// 8 stars 1 colors (not all puzzles are solvable)
void stars8x1test() {
    int64_t ERRS = 0;
    int64_t DENOM = 0;

    int R = 9;
    int C = 9;

    Solver solver;

    vector<point> cells;
    for (int r = 1; r < R; r += 2) {
        for (int c = 1; c < C; c += 2) cells.push_back({r, c});
    }

    std::cout << cells.size() << "\n";

    for (int i1 = 0; i1 < cells.size(); i1++) {
        for (int i2 = i1 + 1; i2 < cells.size(); i2++) {
            for (int i3 = i2 + 1; i3 < cells.size(); i3++) {
                for (int i4 = i3 + 1; i4 < cells.size(); i4++) {
                    for (int i5 = i4 + 1; i5 < cells.size(); i5++) {
                        for (int i6 = i5 + 1; i6 < cells.size(); i6++) {
                            for (int i7 = i6 + 1; i7 < cells.size(); i7++) {
                                for (int i8 = i7 + 1; i8 < cells.size(); i8++) {

                                        vector<int> iv({i1, i2, i3, i4, i5, i6, i7, i8});
                                        vector<point> pv;
                                        for (auto i : iv) pv.push_back(cells[i]);
                                        if (!(DENOM &255)) std::cout << DENOM << "\n";

                                        Grid g(9, 9);
                                        g.defaultDiagonal();
                                        for (int i = 0; i < 8; i++) g.set(pv[i], new Star());

                                        solver.grid = &g;
                                        solver.solve();
                                        if (solver.solutions.size() <= 0) {
                                            std::cout << "ERR ON PUZZLE " << DENOM << "\n";
                                            std::cout << g.to_string() << "\n";
                                            ERRS++;
                                        }

                                        DENOM++;
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    

    std::cout << ERRS << "/" << DENOM << " INVALID GRIDS\n";
}

// 12 stars 1 color (not all puzzles are solvable)
void stars12x1test() {
    int64_t ERRS = 0;
    int64_t DENOM = 0;

    int R = 9;
    int C = 9;

    Solver solver;

    vector<point> cells;
    for (int r = 1; r < R; r += 2) {
        for (int c = 1; c < C; c += 2) cells.push_back({r, c});
    }

    std::cout << cells.size() << "\n";

    // The 4 chosen slots are the empty ones. The remaining slots in go the symbols/

    for (int i1 = 0; i1 < cells.size(); i1++) {
        for (int i2 = i1 + 1; i2 < cells.size(); i2++) {
            for (int i3 = i2 + 1; i3 < cells.size(); i3++) {
                for (int i4 = i3 + 1; i4 < cells.size(); i4++) {
                    std::unordered_set<int> bad({i1, i2, i3, i4});
                    if (!(DENOM &255)) std::cout << DENOM << "\n";
                    DENOM++;

                    Grid g(R, C);
                    g.defaultDiagonal();

                    int index = 0;
                    for (int i = 0; i < cells.size(); i++) {
                        if (bad.find(i) != bad.end()) continue;
                        g.set(cells[index++], new Star());
                    }

                    solver.grid = &g;
                    solver.solve();
                    if (solver.solutions.size() <= 0) {
                        std::cout << "ERR ON PUZZLE " << DENOM << "\n";
                        ERRS++;
                    }
                }
            }
        }
    }

    

    std::cout << ERRS << "/" << DENOM << " INVALID GRIDS\n";
}

int main() {
    srand(42069);

	std::cout << "BEGIN\n";
	auto start = std::chrono::high_resolution_clock::now();


    stars8x1test();



	auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

	int64_t count = duration.count();
    std::cout << "Time taken: " << count << " us" << std::endl;
	std::cout << "Time taken: " << count * 0.001 << " ms" << std::endl;
	std::cout << "Time taken: " << count * 0.000001 << " s" << std::endl;
    return 0;
}