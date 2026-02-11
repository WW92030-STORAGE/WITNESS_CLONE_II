import Grid
import PuzzleEntity
import Utils
import BlockGroup
import GridUtils

class RotationalGrid(Grid.Grid):
    def __init__(self, A = 11, B = 11):
        super().__init__(A, B)

    def setLine(self, p: Utils.point, z):
        super().setLine(p, z)
        super().setLine((self.R - p[0] - 1, self.C - p[1] - 1), z + 1 if z != 0 else z)

    def isPathable(self, x, y):
        for i in range(2):
            if (not self.inBounds((x, y))):
                return False
            p = self.board[x][y]
            if not p.isPath:
                return False
            if p.isBlocker:
                return False
            if p.hasLine:
                return False
            x = self.R - x - 1
            y = self.C - y - 1
        return True

    def defaultDiagonal(self):
        super().defaultPaths()
        self.set((0, 0), PuzzleEntity.Endpoint(True))
        self.set((self.R - 1, 0), PuzzleEntity.Endpoint(False))
        self.set((0, self.C - 1), PuzzleEntity.Endpoint(False))
        self.set((self.R - 1, self.C - 1), PuzzleEntity.Endpoint(True))

class VSymmetryGrid(Grid.Grid):
    def __init__(self, A = 11, B = 11):
        super().__init__(A, B)

    def setLine(self, p: Utils.point, z):
        super().setLine(p, z)
        super().setLine((p[0], self.C - p[1] - 1), z + 1 if z != 0 else z)

    def isPathable(self, x, y):
        for i in range(2):
            if (not self.inBounds((x, y))):
                return False
            p = self.board[x][y]
            if not p.isPath:
                return False
            if p.isBlocker:
                return False
            if p.hasLine:
                return False
            # x = self.R - x - 1
            y = self.C - y - 1
        return True

    def defaultDiagonal(self):
        super().defaultPaths()
        self.set((0, 0), PuzzleEntity.Endpoint(True))
        self.set((self.R - 1, 0), PuzzleEntity.Endpoint(False))
        self.set((0, self.C - 1), PuzzleEntity.Endpoint(True))
        self.set((self.R - 1, self.C - 1), PuzzleEntity.Endpoint(False))

class HSymmetryGrid(Grid.Grid):
    def __init__(self, A = 11, B = 11):
        super().__init__(A, B)

    def setLine(self, p: Utils.point, z):
        super().setLine(p, z)
        super().setLine((self.R - p[0] - 1, p[1]), z + 1 if z != 0 else z)

    def defaultDiagonal(self):
        super().defaultPaths()
        self.set((0, 0), PuzzleEntity.Endpoint(True))
        self.set((self.R - 1, 0), PuzzleEntity.Endpoint(True))
        self.set((0, self.C - 1), PuzzleEntity.Endpoint(False))
        self.set((self.R - 1, self.C - 1), PuzzleEntity.Endpoint(False))

    def isPathable(self, x, y):
        for i in range(2):
            if (not self.inBounds((x, y))):
                return False
            p = self.board[x][y]
            if not p.isPath:
                return False
            if p.isBlocker:
                return False
            if p.hasLine:
                return False
            x = self.R - x - 1
            # y = self.C - y - 1
        return True
