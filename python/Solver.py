import Grid
import PuzzleEntity
import Utils
import BlockGroup
import GridUtils

import random

class Solver:
    grid = Grid.Grid()
    numEnds = 1
    vis = {}
    solutions = []

    def __init__(self, seed = None):
        if seed != None:
            random.seed(seed)

    def seed(self, see):
        random.seed(see)
    
    def path(self, src: Utils.point, prev: Utils.point, numsol = 1):
        if len(self.solutions) >= numsol:
            return
        
        landmark = self.grid.get(src)
        if Utils.instanceof(landmark, PuzzleEntity.Endpoint):
            if not landmark.isStart:
                self.grid.setLine(src, 1)
                if GridUtils.validate(self.grid):
                    self.vis[src] = prev
                    solution = []
                    now = src
                    while now in self.vis and self.vis[now] != now:
                        solution.append(now)
                        now = self.vis[now]
                    
                    solution.append(now)
                    solution.reverse()
                    self.solutions.append(solution)

                    self.vis.pop(src)
                
                self.grid.setLine(src, 0)

                if self.numEnds == 1:
                    return
        nn = []
        for i in self.grid.neighbors(src):
            nn.append(i)
        self.vis[src] = prev
        self.grid.setLine(src, 1)

        offset = random.randint(0, len(nn) - 1)

        for dd in range(len(nn)):
            d = (dd + offset) % len(nn)
            next = nn[d]

            if not self.grid.inBounds(next):
                continue
            if not (self.grid.isPathable(next)):
                continue
            if next in self.vis:
                continue
            if (self.grid.get(next).hasLine) != 0:
                continue

            self.path(next, src, numsol)
            if len(self.solutions) >= numsol:
                break
        
        self.vis.pop(src)
        self.grid.setLine(src, 0)

    def solve(self, numsol = 1):
        self.solutions.clear()
        self.numEnds = 0

        endsnstarts = GridUtils.getSymbols(self.grid, PuzzleEntity.Endpoint)
        for i in endsnstarts:

            if not self.grid.get(i).isStart:
                self.numEnds += 1
                continue
            
            self.vis.clear()
            self.vis[i] = i
            self.path(i, i, numsol)
            if len(self.solutions) >= numsol:
                break

    def apply(self, x):
        if x < 0 or x >= len(self.solutions):
            return

        self.grid.drawPath(self.solutions[x], 1)