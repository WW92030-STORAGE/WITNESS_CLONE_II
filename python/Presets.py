import Grid
import PuzzleEntity
import Utils
import BlockGroup
import GridUtils

from PuzzleEntity import Color, Endpoint, PathDot, Blob, Star, Triangle, Cancel
from Grid import Grid
from SymmetryGrid import RotationalGrid, HSymmetryGrid, VSymmetryGrid

import copy

def simplebg():
    grid = Grid()
    grid.defaultDiagonal()

    grid.set((1, 1), BlockGroup.O())
    grid.set((1, 7), BlockGroup.O())
    return grid

def bgwithinv():
    grid = Grid()
    grid.defaultDiagonal()

    grid.set((1, 1), BlockGroup.J())
    grid.set((7, 1), BlockGroup.L())

    seven = BlockGroup.L()
    seven.rotate(1)
    grid.set((7, 7), seven)

    invO = BlockGroup.O()
    invO.value = -1
    invO.color = Color.RGB_BLUE
    grid.set((5, 7), invO)

    return grid



def canceldotblob():
    grid = Grid(9, 9)
    grid.defaultDiagonal()

    grid.set((1, 1), Blob(Color.RGB_WHITE))
    grid.set((1, 7), Blob(Color.RGB_BLACK))

    grid.set((8, 0), PathDot())
    grid.set((0, 8), PathDot())

    grid.set((7, 1), Cancel())

    return grid

def complexsymdots():
    grid = RotationalGrid(11, 11)
    grid.defaultDiagonal()
    grid.set((6, 2), PathDot(1))
    grid.set((4, 4), PathDot(0))
    grid.set((2, 6), PathDot(2))
    grid.set((10, 5), PathDot(0))
    grid.set((1, 10), PathDot(0))
    grid.set((6, 9), PathDot(0))
    grid.set((10, 8), PathDot(2))

    return grid

def simplesymblobs():
    grid = RotationalGrid(11, 11)
    grid.defaultDiagonal()
    grid.set((7, 1),  Blob(PuzzleEntity.Color.RGB_WHITE))
    grid.set((5, 3),  Blob(PuzzleEntity.Color.RGB_WHITE))
    grid.set((1, 3),  Blob(PuzzleEntity.Color.RGB_WHITE))
    grid.set((5, 9),  Blob(PuzzleEntity.Color.RGB_WHITE))
    
    grid.set((9, 1),  Blob(PuzzleEntity.Color.RGB_BLACK))
    grid.set((9, 3),  Blob(PuzzleEntity.Color.RGB_BLACK))
    grid.set((3, 5),  Blob(PuzzleEntity.Color.RGB_BLACK))
    grid.set((7, 7),  Blob(PuzzleEntity.Color.RGB_BLACK))

    return grid