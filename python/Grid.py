import collections

import PuzzleEntity
import Utils

import copy

class Grid:
    R = 3
    C = 3

    board = []

    def init(self):
        if (self.R % 2 == 0):
            self.R += 1
        if (self.C % 2 == 0):
            self.C += 1

        self.board = []

        for r in range(self.R):
            row = []
            for c in range(self.C):
                row.append(PuzzleEntity.PuzzleEntity())
            
            self.board.append(row)
    
    def __init__(self, A = 9, B = 9):
        self.R = A
        self.C = B

        self.init()

    def inBounds(self, p: Utils.point):
        if p[0] < 0 or p[1] < 0 or p[0] >= self.R or p[1] >= self.C:
            return False
        return True
    

    
    def isPathable(self, x, y):
        if (not self.inBounds((x, y))):
            return False
        p = self.board[x][y]
        if not p.isPath:
            return False
        if p.isBlocker:
            return False
        if p.hasLine:
            return False
        return True
    
    
    def get(self, p):
        if not self.inBounds(p):
            return None
        return self.board[p[0]][p[1]]

    def getColor(self, p: Utils.point):
        if self.get(p) is None:
            return PuzzleEntity.Color.NIL
        return self.get(p).color

    def neighbors(self, p):
        res = set()
        for d in range(4):
            next = (p[0] + Utils.dx[d], p[1] + Utils.dy[d])
            if self.inBounds(next):
                res.add(next)
        return res
    
    # GRID MODIFICATIONS

    def reset(self, r, c):
        self.board[r][c] = PuzzleEntity.PuzzleEntity()

    def set(self, p: Utils.point, pe: PuzzleEntity.PuzzleEntity):
        self.board[p[0]][p[1]] = copy.deepcopy(pe)
    
    def clearAllPaths(self):
        for r in range(self.R):
            for c in range(self.C):
                if (r % 2 == 1) and (c % 2 == 1):
                    continue
                if (Utils.instanceof(PuzzleEntity.Endpoint, self.board[r][c])):
                    continue

                self.reset(r, c)

    def defaultPaths(self):
        for r in range(self.R):
            for c in range(self.C):
                if (r % 2 == 1) and (c % 2 == 1):
                    continue
                self.board[r][c].isPath = True

    def defaultDiagonal(self):
        self.set((0, 0), PuzzleEntity.Endpoint(True))
        self.set((self.R - 1, self.C - 1), PuzzleEntity.Endpoint(False))
        self.defaultPaths()

    def setPath(self, p: Utils.point, z):
        if self.inBounds(p):
            self.board[p[0]][p[1]].isPath = z

    def setLine(self, p: Utils.point, z):
        if self.inBounds(p):
            self.board[p[0]][p[1]].hasLine = z

    def setBlocker(self, p: Utils.point, z):
        if self.inBounds(p):
            self.board[p[0]][p[1]].isBlocker = z
        
    def drawLine(self, a: Utils.point, b: Utils.point, index = 1):
        x1 = a[0]
        x2 = b[0]
        y1 = a[1]
        y2 = b[1]
        if x1 != x2 and y1 != y2:
            return
        
        if x1 > x2:
            x1, x2 = x2, x1
        if y1 > y2:
            y1, y2 = y2, y1

        x = x1
        y = y1
        while (x <= x2):
            y = y1
            while (y <= y2):
                self.setLine((x, y), index)
                y += 1
            x += 1
    
    def drawPath(self, path: Utils.pointVec, index = 1):
        for i in range(len(path) - 1):
            self.drawLine(path[i], path[i + 1], index)
    
    def clearLine(self, x1, y1, x2, y2):
        self.drawLine(x1, y1, x2, y2, 0)
    
    def clearAllLines(self):
        for r in range(self.R):
            for c in range(self.C):
                self.setLine((r, c), 0)

    def __repr__(self):
        res = "Grid[" + str(self.R) + ", " + str(self.C) + "]\n"
        for cc in range(self.C):
            c = self.C - 1 - cc
            for r in range(self.R):
                delim = '.'
                if self.board[r][c].hasLine > 0:
                    x = self.board[r][c].hasLine
                    if x == 1:
                        delim = '#'
                    elif x == 2:
                        delim = '@'
                    elif x == 3:
                        delim = '$'
                    elif x == 4:
                        delim = '%'
                elif self.board[r][c].isPath:
                    delim = '+'
                
                res = res + delim + self.board[r][c].disp + delim + " "
            res = res + "\n"
        return res
        
    # VALIDATION UTILS

    def getSingleLine(self, p: Utils.point):
        res = set()
        q = collections.deque()
        q.append(p)

        while (len(q) > 0):
            pp = q.popleft()
            res.add(pp)
            nn = self.neighbors(pp)
            for i in nn:
                if self.get(i).hasLine == 0:
                    continue
                if i in res:
                    continue
                q.append(i)
                res.add(i)
        
        return res
    
    def getLines(self):
        pp = []
        vis = set()

        for r in range(self.R):
            for c in range(self.C):
                p = (r, c)
                if self.get(p).hasLine == 0:
                    continue
                if p in vis:
                    continue

                line = self.getSingleLine(p)
                for ll in line:
                    vis.add(ll)
                if len(line) > 0:
                    pp.append(line)
        
        return pp
    
    def validateSinglePath(self, p: Utils.point):
        line = self.getSingleLine(p)
        activeStarts = set()
        activeEnds = set()

        theStart = None
        theEnd = None

        for i in line:
            pp = self.get(i)
            if Utils.instanceof(pp, PuzzleEntity.Endpoint):
                if len(Utils.intersection(self.neighbors(i), line)) != 1:
                    continue
                if pp.isStart:
                    activeStarts.add(i)
                    theStart = i
                else:
                    activeEnds.add(i)
                    theEnd = i
        
        if len(activeStarts) != 1 or len(activeEnds) != 1:
            return False
        
        for i in line:
            if i in activeStarts:
                continue
            if i in activeEnds:
                continue

            if len(Utils.intersection(self.neighbors(i), line)) != 2:
                return False
        
        if len(line) > 2:
            return len(Utils.intersection(self.neighbors(theStart), line)) > 0 and theEnd not in self.neighbors(theStart)
        
        return theEnd in self.neighbors(theStart)
    
    
    def validatePath(self):
        ss = self.getLines()
        for line in ss:
            if not self.validateSinglePath(next(iter(line))):
                return False
        return True