import BlockGroup, Grid, GridUtils, PuzzleEntity, Solver, SymmetryGrid, Utils
import math, random

class RandGrid:
    storedpaths = []
    chosenpath = set()
    auxiliaryPaths = {}
    R = 9
    C = 9
    starts = set()
    ends = set()

    GridT = Grid.Grid

    def init(self):
        grid = self.blankGrid()
        grid.defaultDiagonal()
        self.starts = set()
        self.ends = set()

        for r in range(self.R):
            for c in range(self.C):
                p = grid.get((r, c))
                if Utils.instanceof(p, PuzzleEntity.Endpoint):
                    if (p.isStart):
                        self.starts.add((r, c))
                    else:
                        self.ends.add((r, c))
    
    def __init__(self, r = 9, c = 9, seed = None, GridT = Grid.Grid):
        self.storedpaths = []
        self.chosenpath = set()
        self.auxiliaryPaths = {}
        self.starts = set()
        self.ends = set()

        if not (seed == None):
            random.seed(seed)

        self.R = r
        self.C = c
        self.GridT = GridT

        self.init()
    
    def setChosenPath(self, x):
        if x < 0 or x >= len(self.storedpaths):
            return 
        
        self.chosenpath.clear()
        self.auxiliaryPaths.clear()

        for i in self.storedpaths[x]:
            self.chosenpath.add(i)

        grid = self.blankGrid()
        self.applyChosenPath(grid)

        for r in range(self.R):
            for c in range(self.C):
                if (grid.board[r][c].hasLine != 0):
                    line = grid.board[r][c].hasLine
                    if line == 1:
                        continue

                    if line not in self.auxiliaryPaths:
                        self.auxiliaryPaths[line] = set()
                    self.auxiliaryPaths[line].add((r, c))
    
    def pickRandomPath(self):
        self.setChosenPath(random.randint(0, len(self.storedpaths)))
    
    def applyChosenPath(self, grid):
        grid.clearAllLines()
        for i in self.chosenpath:
            grid.setLine(i, 1)

    def isOnChosenPath(self, p):
        return p in self.chosenpath
    
    def isOnAnyPath(self, p):
        if self.isOnChosenPath(p):
            return True
        
        for i in self.auxiliaryPaths:
            if p in self.auxiliaryPaths[i]:
                return True
            
        return False

    def blankGrid(self):
        grid = self.GridT(self.R, self.C)
        for i in self.starts:
            grid.set(i, PuzzleEntity.Endpoint(True))
        for i in self.ends:
            grid.set(i, PuzzleEntity.Endpoint(False))
        
        grid.defaultPaths()
        return grid
    
    def seed(see):
        random.seed(see)

    def pathfind(self, numPaths = Utils.INF, see = None):
        g = self.blankGrid()
        solver = Solver.Solver()
        if (see != None):
            solver.seed(see)
        
        solver.grid = g
        solver.solve(numPaths)

        self.storedpaths = solver.solutions

    def randomChoice(self, n, k):
        v = []
        for i in range(n):
            v.append(i)

        random.shuffle(v)

        while (len(v) > k):
            v.pop()

        return v
    
    def getRandomCells(self, k):
        res = []
        s = set()

        order = []
        for r in range(self.R):
            for c in range(self.C):
                if (r % 2 == 1) and (c % 2 == 1):
                    order.append((r, c))
        
        choice = self.randomChoice(len(order), k)
        for i in choice:
            res.append(order[i])
        
        return res
    
    def getRandomEdges(self, k, noLine = False, line = False):
        res = []
        s = set()

        order = []
        for r in range(self.R):
            for c in range(self.C):
                if noLine and self.isOnAnyPath((r, c)):
                    continue
                if line and not self.isOnAnyPath((r, c)):
                    continue
                if (r, c) in self.starts or (r, c) in self.ends:
                    continue
                if (r % 2) != (c % 2):
                    order.append((r, c))
        
        choice = self.randomChoice(len(order), k)
        for i in choice:
            res.append(order[i])
        
        return res
    
    def getRandomNodes(self, k, noLine = False, line = False):
        res = []
        s = set()

        order = []
        for r in range(self.R):
            for c in range(self.C):
                if noLine and self.isOnAnyPath((r, c)):
                    continue
                if line and not self.isOnAnyPath((r, c)):
                    continue
                if (r, c) in self.starts or (r, c) in self.ends:
                    continue
                if (r % 2 == 0) and (c % 2 == 0):
                    order.append((r, c))
        
        choice = self.randomChoice(len(order), k)
        for i in choice:
            res.append(order[i])
        
        return res
    
    def getRandomPaths(self, k, noLine = False, line = False):
        res = []
        s = set()

        order = []
        for r in range(self.R):
            for c in range(self.C):
                if noLine and self.isOnAnyPath((r, c)):
                    continue
                if line and not self.isOnAnyPath((r, c)):
                    continue
                if (r, c) in self.starts or (r, c) in self.ends:
                    continue
                if (r % 2 == 1) and (c % 2 == 1):
                    continue
                order.append((r, c))
        
        choice = self.randomChoice(len(order), k)
        for i in choice:
            res.append(order[i])
        
        return res
    
    def getRandomColors(self, k = Utils.INF):
        v = []
        for color in PuzzleEntity.COLORS:
            v.append(color)
        
        random.shuffle(v)
        while len(v) > k:
            v.pop()
        return v
    
    # This always generates a 9x9 standard diagonal grid
    def randStars(self):
        v = [0, 0, 0, 0, 1, 1, 1, 1]
        random.shuffle(v)

        vv = self.getRandomCells(8)
        cols = self.getRandomColors(2)

        grid = Grid.Grid(9, 9)
        grid.defaultDiagonal()

        for i in range(len(v)):
            p = vv[i]
            grid.set(p, PuzzleEntity.Star(cols[i % 2]))
        return grid
    
    def randStarsGeneral(self, numPairs = 4, numCuts = 2):
        self.pickRandomPath()
        cutlocs = self.getRandomEdges(numCuts, True)
        grid = self.blankGrid()

        self.applyChosenPath(grid)
        regions = GridUtils.getRegionsCells(grid)
        grid.clearAllLines()


        for i in cutlocs:
            grid.setPath(i, False)

        nReg = len(regions)

        regioncuts = []
        for rno in range(nReg):
            i = 0
            while (i < len(regions[rno]) - 1):
                regioncuts.append(rno)
                i += 2
        
        random.shuffle(regioncuts)

        pairsPerRegion = []
        for i in range(nReg):
            pairsPerRegion.append(0)
        for i in range(min(numPairs, len(regioncuts))):
            pairsPerRegion[regioncuts[i]] += 1
        
        colors = self.getRandomColors()

        for rno in range(nReg):
            region = []
            for i in regions[rno]:
                region.append(i)
            random.shuffle(region)

            for i in range(min(pairsPerRegion[rno], len(colors))):
                grid.set(region[i * 2], PuzzleEntity.Star(colors[i]))
                grid.set(region[i * 2 + 1], PuzzleEntity.Star(colors[i]))
        return grid
    
    def randMaze(self, numCuts = 10):
        self.pickRandomPath()
        cutlocs = self.getRandomEdges(numCuts, True)
        grid = self.blankGrid()
        for i in cutlocs:
            grid.setPath(i, False)
        return grid
    
    def randDots(self, numSymbols = 8, numCuts = 2, threshold = 0.5):
        self.pickRandomPath()
        cutlocs = self.getRandomEdges(numCuts, True)
        symbols = self.getRandomPaths(numSymbols, False, True)

        grid = self.blankGrid()
        self.applyChosenPath(grid)

        for i in cutlocs:
            grid.setPath(i, False)
        for i in symbols:
            if len(self.auxiliaryPaths) != 0 and random.random() < threshold:
                lineno = -1 + grid.get(i).hasLine
                restriction = (1<<lineno)
                grid.set(i, PuzzleEntity.PathDot(restriction))
            else:
                grid.set(i, PuzzleEntity.PathDot())
        grid.clearAllLines()

        return grid
    
    def randBlobs(self, numSymbols = 10, numCols = 3, numCuts = 2):
        self.pickRandomPath()
        cutlocs = self.getRandomEdges(numCuts, True)
        symbols = self.getRandomCells(numSymbols)

        grid = self.blankGrid()
        self.applyChosenPath(grid)

        regions = GridUtils.getRegionsCells(grid)
        grid.clearAllLines()

        for i in cutlocs:
            grid.setPath(i, False)

        colors = self.getRandomColors(numCols)

        for i in symbols:
            region = 0
            for r in range(len(regions)):
                if i in regions[r]:
                    region = r
            grid.set(i, PuzzleEntity.Blob(colors[region % len(colors)]))
        return grid
    
    def randTriangles(self, numSymbols = 10, numCuts = 2):
        self.pickRandomPath()
        cutlocs = self.getRandomEdges(numCuts, True)
        symbols = self.getRandomCells(numSymbols)

        grid = self.blankGrid()
        self.applyChosenPath(grid)

        for i in symbols:
            pp = grid.neighbors(i)
            cc = 0
            for p in pp:
                if grid.get(p).hasLine != 0:
                    cc += 1
            if cc != 0:
                grid.set(i, PuzzleEntity.Triangle(cc))
        grid.clearAllLines()
        return grid