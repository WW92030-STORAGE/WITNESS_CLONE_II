import PuzzleEntity
import Grid
import Utils
import BlockGroup

from collections import deque

def getSymbols(g: Grid, T):
    s = set()
    for r in range(g.R):
        for c in range(g.C):
            pp = g.get((r, c))
            if Utils.instanceof(pp, T):
                s.add((r, c))
    
    return s

def getActiveSymbols(g: Grid, T):
    s = getSymbols(g, T)
    res = set()

    for i in s:
        if (not g.get(i) is None) and g.get(i).isActive:
            res.add(i)
    
    return res

def getSymbolsColor(g: Grid, c: PuzzleEntity.Color):
    res = set()
    for r in range(g.R):
        for c in range(g.C):
            pp = g.get(r, c)
            if (not pp is None) and (pp.color == c):
                res.add((r, c))

    return res

def getColors(g: Grid, s: Utils.pointSet, NotNil = True):
    res = {}
    for i in s:
        if (not g.get(i) is None):
            if NotNil and (g.get(i).color == PuzzleEntity.Color.NIL):
                continue

            Utils.push(res, g.get(i).color)
    return res

def getActiveColors(g: Grid, s: Utils.pointSet, NotNil = True):
    res = {}
    for i in s:
        if (not g.get(i) is None):
            if NotNil and (g.get(i).color == PuzzleEntity.Color.NIL):
                continue
            if not (g.get(i).isActive):
                continue

            Utils.push(res, g.get(i).color)
    return res

def isFFable(g: Grid, next: Utils.point):
    if (not g.inBounds(next)):
        return False
    if (g.get(next).isBlocker):
        return False
    if (g.get(next).hasLine != 0):
        return False
    return True

def floodfill(g: Grid, p: Utils.point):
    vis = set()
    q = deque()
    if (not isFFable(g, p)):
        return vis
    
    q.append(p)
    vis.add(p)

    while len(q) > 0:
        now = q.popleft()

        for next in g.neighbors(now):
            if (not isFFable(g, next)):
                continue
            if next in vis:
                continue
            vis.add(next)
            q.append(next)

    return vis

def floodfillCells(g: Grid, p: Utils.point):
    vis = set()
    res = set()
    q = deque()
    if (not isFFable(g, p)):
        return vis
    
    q.append(p)
    vis.add(p)

    while len(q) > 0:
        now = q.popleft()

        for next in g.neighbors(now):
            if (not isFFable(g, next)):
                continue
            if next in vis:
                continue
            vis.add(next)
            q.append(next)

    for i in vis:
        if (i[0] % 2 == 1) and (i[1] % 2 == 1):
            res.add(i)

    return res

def getRegions(g: Grid):
    v = []
    s = set()
    for r in range(g.R):
        for c in range(g.C):
            p = (r, c)
            if (not isFFable(g, p)):
                continue

            if p in s:
                continue

            region = floodfill(g, p)
            for i in region:
                s.add(i)
            
            v.append(region)
    
    return v

def getRegionsCells(g: Grid):
    v = []
    s = set()
    for r in range(g.R):
        for c in range(g.C):
            if (r % 2 == 0) or (c % 2 == 0):
                continue
            p = (r, c)
            if (not isFFable(g, p)):
                continue

            if p in s:
                continue

            region = floodfillCells(g, p)
            for i in region:
                s.add(i)
            
            v.append(region)
    
    return v

def validateRegionNoRecur(grid: Grid, region: Utils.pointSet):
    violations = set()

    # Local neighborhood checks

    # Check dots

    for i in Utils.intersection(region, getActiveSymbols(grid, PuzzleEntity.PathDot)):
        p = grid.get(i)
        if (not Utils.instanceof(p, PuzzleEntity.PathDot)):
            continue

        if (not p is None) and (p.hasLine == 0):
            violations.add(i)

        restriction = p.restriction
        if restriction == 0:
            continue
        if (restriction & (1<<(p.hasLine - 1))) != 0:
            continue

        violations.add(i)

    # Check triangles

    for i in Utils.intersection(region, getActiveSymbols(grid, PuzzleEntity.Triangle)):
        pp = grid.get(i)
        if (not Utils.instanceof(pp, PuzzleEntity.Triangle)):
            continue

        cc = 0
        for n in grid.neighbors(i):
            if (not grid.get(n) is None) and (grid.get(n).hasLine != 0):
                cc += 1
        
        if (cc != pp.count):
            violations.insert(i)

    entities = Utils.intersection(region, getActiveSymbols(grid, PuzzleEntity.ColorEntity))

    # Region checks occur here

    # Check blobs

    blobs = Utils.intersection(region, getActiveSymbols(grid, PuzzleEntity.Blob))
    colors = set()
    for symbol in blobs:
        colors.add(grid.getColor(symbol))

    if len(colors) > 1:
        for i in blobs:
            violations.add(i)
    
    # Check stars

    stars = getActiveSymbols(grid, PuzzleEntity.Star)
    colorcount = getActiveColors(grid, region)
    for star in stars:
        c = grid.getColor(star)
        if c not in colorcount:
            violations.add(star)
        elif colorcount[c] != 2:
            violations.add(star)
    
    # Advanced region checks here

    bgs = Utils.intersection(region, getActiveSymbols(grid, BlockGroup.BlockGroup))

    convertedregion = set()
    for i in region:
        convertedregion.add(((i[0] - 1) // 2, (i[1] - 1) // 2))

    big = BlockGroup.BlockGroup(convertedregion)
    v = []

    for i in bgs:
        v.append(grid.get(i))
    
    if not BlockGroup.checkPlacements(big, v, ((0, 0), (grid.R // 2, grid.C // 2))):
        for i in bgs:
            violations.add(i)

    return violations

def getViolationsNoRecursion(grid: Grid):
    violations = set()

    # Local neighborhood checks

    # Check dots

    for i in getActiveSymbols(grid, PuzzleEntity.PathDot):
        p = grid.get(i)
        if (not Utils.instanceof(p, PuzzleEntity.PathDot)):
            continue

        if (not p is None) and (p.hasLine == 0):
            violations.add(i)
            continue

        restriction = p.restriction
        if restriction == 0:
            continue
        if (restriction & (1<<(p.hasLine - 1))) != 0:
            continue

        violations.add(i)

    # Check triangles

    for i in getActiveSymbols(grid, PuzzleEntity.Triangle):
        pp = grid.get(i)
        if (not Utils.instanceof(pp, PuzzleEntity.Triangle)):
            continue

        cc = 0
        for n in grid.neighbors(i):
            if (not grid.get(n) is None) and (grid.get(n).hasLine != 0):
                cc += 1
        
        if (cc != pp.count):
            violations.insert(i)

    regions = getRegions(grid)
    regionsCells = getRegionsCells(grid)
    entities = getActiveSymbols(grid, PuzzleEntity.ColorEntity)

    # Region checks occur here

    # Check blobs

    blobs = getActiveSymbols(grid, PuzzleEntity.Blob)
    for region in regionsCells:
        p = Utils.intersection(blobs, region)
        colors = set()
        for symbol in p:
            colors.add(grid.getColor(symbol))

        if len(colors) > 1:
            for i in p:
                violations.add(i)
    
    # Check stars

    stars = getActiveSymbols(grid, PuzzleEntity.Star)
    for region in regionsCells:
        regstars = Utils.intersection(stars, region)
        colorcount = getActiveColors(grid, region)
        for symbol in regstars:
            c = grid.getColor(symbol)
            if c not in colorcount:
                violations.add(symbol)
            elif colorcount[c] != 2:
                violations.add(symbol)

    # Advanced region checks here

    bgs = getActiveSymbols(grid, BlockGroup.BlockGroup)

    for region in regionsCells:
        regbgs = Utils.intersection(bgs, region)
        convertedregion = set()
        for i in region:
            convertedregion.add(((i[0] - 1) // 2, (i[1] - 1) // 2))

        big = BlockGroup.BlockGroup(convertedregion)
        big.normalize()
        v = []
        for i in regbgs:
            v.append(grid.get(i))
    
        if not BlockGroup.checkPlacements(big, v, ((0, 0), (grid.R // 2, grid.C // 2))):
            for i in regbgs:
                violations.add(i)

    return violations
    
def getViolations(grid: Grid):
    # Check recursives here

    violations = getViolationsNoRecursion(grid)
    
    regions = getRegions(grid)
    regionsCells = getRegionsCells(grid)

    res = set()

    deactivated = set()
    cancels = getActiveSymbols(grid, PuzzleEntity.Cancel)

    if len(cancels) > 0:
        for region in regions:
            cancelsInRegion = Utils.intersection(region, cancels)
            violationsInRegion = Utils.intersection(region, violations)
            cancelViolations = set()

            for cancel in cancelsInRegion:
                solvable = False
                if len(violationsInRegion) <= 0:
                    violationsInRegion.add(cancel)
                    cancelViolations.add(cancel)
                    continue

                grid.board[cancel[0]][cancel[1]].isActive = False
                for v in violationsInRegion:
                    grid.board[v[0]][v[1]].isActive = False

                    if not solvable:
                        vvv = getViolations(grid)
                        solvable = solvable or (len(vvv) <= 0)
                    
                    grid.board[v[0]][v[1]].isActive = True

                grid.board[cancel[0]][cancel[1]].isActive = True

                if not solvable:
                    violationsInRegion.add(cancel)
                    cancelViolations.add(cancel)
                else:
                    violationsInRegion.clear()
                    break
            
            for i in violationsInRegion:
                res.add(i)
    else:
        return violations
    
    return res

def Validate(grid: Grid):
    if (not grid.validatePath()):
        return False
    
    return len(getViolations(grid)) <= 0

def validate(grid: Grid):
    return Validate(grid)