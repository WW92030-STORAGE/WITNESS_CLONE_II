import Grid
import PuzzleEntity
import Utils
import BlockGroup
import GridUtils

from PuzzleEntity import Color, Endpoint, PathDot, Blob, Star, Triangle, Cancel
from Grid import Grid

import copy

def canceldotblob():
    grid = Grid(9, 9)
    grid.defaultDiagonal()

    grid.set((1, 1), Blob(Color.RGB_RED))
    grid.set((1, 7), Blob(Color.RGB_RED))

    grid.set((8, 0), PathDot())
    grid.set((0, 8), PathDot())

    grid.set((7, 1), Cancel())

    return grid