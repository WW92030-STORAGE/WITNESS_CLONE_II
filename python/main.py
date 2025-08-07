import Grid
import PuzzleEntity
import Utils
import BlockGroup
import GridUtils
import Solver
import SymmetryGrid

from PuzzleEntity import Color, Endpoint, PathDot, Blob, Star, Triangle, Cancel
from Grid import Grid
from SymmetryGrid import RotationalGrid

import Presets

import copy

def simpleTest():
    grid = Grid.Grid()
    grid.defaultDiagonal()

    grid.set((5, 1), PuzzleEntity.Star())
    grid.set((5, 3), PuzzleEntity.Star())
    grid.set((7, 1), PuzzleEntity.Star())

    grid.drawLine((0, 0), (0, 4))
    grid.drawLine((0, 4), (8, 4))
    grid.drawLine((8, 4), (8, 8))


    print(grid)
    print(grid.validatePath())

    print(Utils.instanceof(grid.board[0][0], PuzzleEntity.Endpoint))

    print(GridUtils.getViolations(grid))

def simpleBG():
    bg1 = BlockGroup.BlockGroup({(0, 0), (0, 1), (0, 2)})
    bg2 = BlockGroup.BlockGroup({(0, 0), (0, 1), (1, 0)})
    bg3 = BlockGroup.BlockGroup({(0, 0)})

    bg4 = BlockGroup.BlockGroup({(0, 0), (0, 1)})

    print(BlockGroup.checkPlacements(bg2,[bg3, bg4]))

def cancelTest():
    grid = Presets.canceldotblob()

    grid.drawPath([(0, 0), (0, 2), (2, 2), (2, 0), (8, 0), (8, 8)])

    print(grid)
    print(GridUtils.getViolations(grid))

def solverTest():
    grid = Presets.canceldotblob()

    solver = Solver.Solver()
    solver.grid = grid

    solver.solve()

    print(solver.solutions[0])

    solver.apply(0)

    print(solver.grid)

def rottest():
    grid = RotationalGrid()
    grid.defaultDiagonal()


    # grid.drawPath([(0, 0), (2, 0), (2, 2), (4, 2), (4, 0), (8, 0)])

    print(grid)
    solver = Solver.Solver()
    solver.grid = grid
    solver.solve(Utils.INF)
    print(len(solver.solutions))

    solver.apply(492)
    print(grid)

if __name__ == "__main__":
    rottest()