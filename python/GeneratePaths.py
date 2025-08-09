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

import pickle

# Assuming calling from root dir
OUT_DIR = "python/paths"

def dumpPaths(N = 13, M = 13, GridType = RotationalGrid):
    rg = RandGrid(N, M, GridT = GridType)

    rg.pathfind()

    print(N, M, GridType.__name__, ":", len(rg.storedpaths))

    slug = str(N) + "_" + str(M) + "_" + str(GridType.__name__) + ".dat"

    with open(OUT_DIR + "/storedpaths_" + slug, 'wb') as FF:
        output = pickle.dump(rg.storedpaths, FF)
    
    with open(OUT_DIR + "/readablepaths_" + slug, 'w') as FF:
        for path in rg.storedpaths:
            FF.write(str(path) + "\n")


if __name__ == "__main__":
    sizes = [3, 5, 7, 9] # Use higher dimensions at your own risk
    grids = [Grid, RotationalGrid, HSymmetryGrid, VSymmetryGrid]

    for size in sizes:
        for grid in grids:
            # print(str(size), str(grid.__name__))
            dumpPaths(size, size, grid)
    
    '''

    with open(OUT_DIR + "/storedpaths_9_9_Grid.dat", 'rb') as FF:
        storedpaths = pickle.load(FF)
        print(len(storedpaths))

    '''