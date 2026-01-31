import Grid
import PuzzleEntity
import Utils
import BlockGroup
import GridUtils
import Solver
import SymmetryGrid

from PuzzleEntity import Color, Endpoint, PathDot, Blob, Star, Triangle, Cancel
from BlockGroup import BlockGroup
from Grid import Grid
from SymmetryGrid import RotationalGrid, HSymmetryGrid, VSymmetryGrid
import Render
from RandGrid import RandGrid

import Presets

# https://gdbrowser.com/100533177. A bunch of terrifying brute force tests.


def diamond():
    s = Solver.Solver()
    for x1 in range(3, 8, 2):
        for y1 in range(0, 9, 2):
            print("FIRST DOT: " + str(x1) + " " + str(y1) + "...")
            for x2 in range(0, 9, 2):
                for y2 in range(1, 6, 2):

                    g = Grid(9, 9)
                    g.defaultDiagonal()
                    g.set((2, 2), PathDot())
                    g.set((6, 2), PathDot())
                    g.set((2, 6), PathDot())
                    g.set((6, 6), PathDot())
                    g.set((0, 4), PathDot())
                    g.set((4, 4), PathDot())
                    g.set((8, 4), PathDot())
                    g.set((4, 0), PathDot())
                    g.set((4, 8), PathDot())

                    g.set((x1, y1), PathDot())
                    g.set((x2, y2), PathDot())

                    s.grid = g
                    s.solve(1)

                    if len(s.solutions) <= 0:
                        print("ERROR SPAWNING DOTS ON " + str(x1) + " " + str(y1) + " . " + str(x2) + " " + str(y2))

def diamond2():
    s = Solver.Solver()
    dx = [1, 0, -1, 0]
    dy = [0, 1, 0, -1]

    for s0 in range(4):
        for s1 in range(4):
            print(s0, s1, "...")
            for s2 in range(4):
                for s3 in range(4):
                    p0 = (2 + dx[s0], 2 + dy[s0])
                    p1 = (6 + dx[s1], 2 + dy[s1])
                    p2 = (2 + dx[s2], 6 + dy[s2])
                    p3 = (6 + dx[s3], 6 + dy[s3])

                    g = Grid(9, 9)
                    g.defaultDiagonal()
                    g.set((2, 2), PathDot())
                    g.set((6, 2), PathDot())
                    g.set((2, 6), PathDot())
                    g.set((6, 6), PathDot())
                    g.set((0, 4), PathDot())
                    g.set((4, 4), PathDot())
                    g.set((8, 4), PathDot())
                    g.set((4, 0), PathDot())
                    g.set((4, 8), PathDot())

                    g.set(p0, PathDot())
                    g.set(p1, PathDot())
                    g.set(p2, PathDot())
                    g.set(p3, PathDot())

                    s.grid = g
                    s.solve(1)

                    if len(s.solutions) <= 0:
                        print("ERROR SPAWNING DOTS ON " + str(p0) + " " + str(p1) + " " + str(p2) + " " + str(p3))


if __name__ == "__main__":
    diamond2()