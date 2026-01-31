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
