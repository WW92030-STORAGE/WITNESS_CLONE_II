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

# Assuming you are invoking this from the base directory using the Makefile
RENDER_OUTPUT = "grid"
RENDER_SOLVED = "solution"

FILE_FORMAT = ".png" # Must have period

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

    Render.render(RENDER_OUTPUT + FILE_FORMAT, grid)

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

    Render.render(RENDER_OUTPUT + FILE_FORMAT, grid)

def solverTest():
    grid = Presets.simplestars()

    solver = Solver.Solver()
    solver.grid = grid

    solver.solve()

    print(solver.solutions[0])

    solver.apply(0)

    print(solver.grid)

    Render.render(RENDER_OUTPUT + FILE_FORMAT, grid)

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

    Render.render(RENDER_OUTPUT + FILE_FORMAT, grid)

def rottest2():
    grid = Presets.complexsymdots()

    print(grid)
    solver = Solver.Solver()
    solver.grid = grid
    solver.solve(5)
    print(len(solver.solutions))

    solver.apply(0)
    print(grid)

    Render.render(RENDER_OUTPUT + FILE_FORMAT, grid)

def rottest3():
    grid = Presets.simplesymblobs()

    print(grid)
    solver = Solver.Solver()
    solver.grid = grid
    solver.solve(5)
    print(len(solver.solutions))

    solver.apply(0)
    print(grid)

    Render.render(RENDER_OUTPUT + FILE_FORMAT, grid)

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

    Render.render(RENDER_OUTPUT + FILE_FORMAT, grid)

def randrottest():
    N = 9
    randgrid = RandGrid(N, N, GridT = Grid)
    randgrid.pathfind()
    print(len(randgrid.storedpaths))

    grid = randgrid.randBlocksByRegion(numRegions = 1)

    solver = Solver.Solver()
    solver.grid = grid
    solver.solve(1)

    # solver.apply(0)

    print(grid)

    Render.render(RENDER_OUTPUT + FILE_FORMAT, grid)
    solver.apply(0)
    Render.render(RENDER_SOLVED + FILE_FORMAT, grid)

def rgb():
    N = 11
    randgrid = RandGrid(N, N, GridT = RotationalGrid)
    randgrid.pathfind()
    print(len(randgrid.storedpaths))

    grid = randgrid.randStarsGeneral(100)

    solver = Solver.Solver()
    solver.grid = grid
    solver.solve(1)

    # solver.apply(0)

    print(grid)

    Render.render(RENDER_OUTPUT + "_R" + FILE_FORMAT, grid, filter = Color.RGB_RED.value)
    Render.render(RENDER_OUTPUT + "_G" + FILE_FORMAT, grid, filter = Color.RGB_GREEN.value)
    Render.render(RENDER_OUTPUT + "_B" + FILE_FORMAT, grid, filter = Color.RGB_BLUE.value)
    Render.render(RENDER_OUTPUT + FILE_FORMAT, grid)
    solver.apply(0)
    Render.render(RENDER_SOLVED + FILE_FORMAT, grid, line = [Color.RGB_WHITE.value, Color.RGB_WHITE.value])



if __name__ == "__main__":
    rgb()