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
import time

import main

WARM = 1
TEST = 8

TEST_FUNC = main.rgb

if __name__ == "__main__":
    AVG = 0
    for i in range(WARM):
        TEST_FUNC()
    
    for i in range(TEST):
        START = time.perf_counter_ns()



        TEST_FUNC()


        END = time.perf_counter_ns()
        ELAPSED_NS = END - START
        print("FULL MS:", ELAPSED_NS * 0.000001)
        AVG += ELAPSED_NS * 0.000001
    
    print("AVG", AVG / TEST)
