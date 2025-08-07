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
from SymmetryGrid import RotationalGrid
import Render

import Presets

# Assuming you are invoking this from the base directory using the Makefile
RENDER_OUTPUT = "grid.png"

def simpleTest():
    grid = Grid()
    grid.defaultDiagonal()

    grid.set((5, 1), PuzzleEntity.Star())
    grid.set((5, 3), PuzzleEntity.Star())
    # grid.set((7, 1), PuzzleEntity.Star())

    grid.drawLine((0, 0), (0, 4))
    grid.drawLine((0, 4), (8, 4))
    grid.drawLine((8, 4), (8, 8))


    print(grid)
    print(grid.validatePath())

    print(Utils.instanceof(grid.board[0][0], PuzzleEntity.Endpoint))

    print(GridUtils.getViolations(grid))

    Render.render(RENDER_OUTPUT, grid)

def simpleBG():
    bg1 = BlockGroup({(0, 0), (0, 1), (0, 2)})
    bg2 = BlockGroup({(0, 0), (0, 1), (1, 0)})
    bg3 = BlockGroup({(0, 0)})

    bg4 = BlockGroup({(0, 0), (0, 1)})

    print(BlockGroup.checkPlacements(bg2,[bg3, bg4]))

def cancelTest():
    grid = Presets.canceldotblob()

    grid.drawPath([(0, 0), (0, 2), (2, 2), (2, 0), (8, 0), (8, 8)])

    print(grid)
    print(GridUtils.getViolations(grid))

    Render.render(RENDER_OUTPUT, grid)

def solverTest():
    grid = Presets.canceldotblob()

    solver = Solver.Solver()
    solver.grid = grid

    solver.solve()

    print(solver.solutions[0])

    solver.apply(0)

    print(solver.grid)

    Render.render(RENDER_OUTPUT, grid)

def rottest():
    grid = RotationalGrid()
    grid.defaultDiagonal()


    # grid.drawPath([(0, 0), (2, 0), (2, 2), (4, 2), (4, 0), (8, 0)])

    print(grid)
    solver = Solver.Solver()
    solver.grid = grid
    solver.solve(5)
    print(len(solver.solutions))

    solver.apply(3)
    print(grid)

    Render.render(RENDER_OUTPUT, grid)

def rottest2():
    grid = Presets.complexsymdots()

    print(grid)
    solver = Solver.Solver()
    solver.grid = grid
    solver.solve(5)
    print(len(solver.solutions))

    solver.apply(0)
    print(grid)

    Render.render(RENDER_OUTPUT, grid)

def rottest3():
    grid = Presets.simplesymblobs()

    print(grid)
    solver = Solver.Solver()
    solver.grid = grid
    solver.solve(5)
    print(len(solver.solutions))

    solver.apply(0)
    print(grid)

    Render.render(RENDER_OUTPUT, grid)

def bgtest():
    grid = Presets.simplebg()

    

    grid.drawPath([(0, 0), (4, 0), (4, 8), (8, 8)])
    print(GridUtils.validate(grid))
    print(grid)

def bgsolvertest():
    grid = Presets.bgwithinv()

    for r in range(grid.R):
        for c in range(grid.C):
            if Utils.instanceof(grid.get((r, c)), BlockGroup):
                print(grid.get((r, c)))
    
    print(grid)


    solver = Solver.Solver()
    solver.grid = grid
    solver.solve(1)

    solver.apply(0)

    print(grid)

    Render.render(RENDER_OUTPUT, grid)

if __name__ == "__main__":
    bgsolvertest()