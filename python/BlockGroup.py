from typing import List
import copy

import PuzzleEntity
import Utils

class BlockGroup(PuzzleEntity.ColorEntity):
    points = set()
    value = 1
    fixed = True
    boundingbox = Utils.MAXBB

    def init(self):
        self.type = "BlockGroup"
        self.disp = "BG"
        self.disp = self.disp + ("X" if self.fixed else "O") + str(Utils.clamp(self.value, -99, 99))
        while len(self.disp) < 6:
            self.disp = self.disp + " "

        if self.value < 0:
            self.color = PuzzleEntity.Color.RGB_BLUE
        
        self.computeBoundingBox()
    
    def __init__(self, ps = {(0, 0)}, color = PuzzleEntity.Color.RGB_YELLOW):
        self.points = set()
        for point in ps:
            self.points.add(point)
        self.color = color

        self.init()
    
    def computeBoundingBox(self):
        bb = [[Utils.INF, Utils.INF], [-1 * Utils.INF, -1 * Utils.INF]]

        

        for i in self.points:
            bb[0][0] = min(bb[0][0], i[0])
            bb[0][1] = min(bb[0][1], i[1])
            bb[1][0] = max(bb[1][0], i[0])
            bb[1][1] = max(bb[1][1], i[1])
        
        aabb = ((bb[0][0], bb[0][1]), (bb[1][0], bb[1][1]))
        aabb = Utils.resetBB(aabb)
        self.boundingbox = aabb

    def resetBoundingBox(self):
        self.boundingbox = Utils.resetBB(self.boundingbox)

    def translate(self, p: Utils.point):
        ss = set()
        for i in self.points:
            ss.add(Utils.add(i, p))
        
        self.points = ss

        self.boundingbox = (Utils.add(self.boundingbox[0], p), Utils.add(self.boundingbox[1], p))

    def moveBLTo(self, p: Utils.point):
        self.translate(Utils.sub(p, self.boundingbox[0]))
    
    def normalize(self):
        self.translate(Utils.inv(self.boundingbox[0]))

    def rotate(self, k = 1):
        while k < 0:
            k += 8
        k %= 4

        if k <= 0:
            return
        
        if k > 1:
            self.rotate(k - 1)

        ss = set()
        for i in self.points:
            ss.add(Utils.rot(i))
        self.points = ss

        self.boundingbox = (Utils.rot(self.boundingbox[0]), Utils.rot(self.boundingbox[1]))

        self.resetBoundingBox()

    def rotateAround(self, s: Utils.point = (0, 0), k = 1):
        self.translate(Utils.inv(s))
        self.rotate(k)
        self.translate(s)
    
    # ARRANGING + CHECKS

    def getPlaceables(self, toPlaceOn):
        d = Utils.dims(self.boundingbox)
        if (Utils.instanceof(toPlaceOn, BlockGroup)):
            return (Utils.sub(toPlaceOn.boundingbox[0], d), Utils.add(toPlaceOn.boundingbox[1], (1, 1)))
        elif (type(toPlaceOn) == Utils.AABB):
            return (toPlaceOn[0], Utils.add(Utils.sub(toPlaceOn[1], d), (1, 1)))
        
    def OOB(self, ab: Utils.AABB):
        for i in self.points:
            if not Utils.contains(ab, i):
                return True
        return False
    
    def __str__(self):
        res = "BlockGroup[fixed=" + str(self.fixed) + ", value="
        res = res + str(self.value) + ", bb=" + str(self.boundingbox)
        res = res + "][" + str(self.points) + "]"
        return res
    
def getSum(bgs):
    sum = {}
    return add(sum, bgs)
    
def add(sum, bg, scale = 1):
    if (Utils.instanceof(bg, BlockGroup)):
        for p in bg.points:
            if p not in sum:
                sum[p] = 0
            sum[p] += bg.value * scale

    else:
        for i in range(len(bg)):
            gb = bg[i]
            sum = add(sum, gb, scale)
    
    res = {}
    for i in sum:
        if sum[i] != 0:
            res[i] = sum[i]

    return res

def scale(sum, scale = 1):
    res = {}
    for i in sum:
        res[i] = sum[i] * scale
    return res
    

def checkPlacementsFixed(big: BlockGroup, small: List[BlockGroup], zeroOK = True):
    smol = getSum(small)
    if zeroOK and len(smol) <= 0:
        return True
    
    total = add(smol, big, -1)
    return len(total) <= 0

def isEvenPossible(big, small, placements, zeroOK = True):
    current = add(getSum(placements), big, -1)
    start = len(placements)
    n = len(small)

    totalremtiles = 0
    i = start
    while (i < n):
        totalremtiles += len(small[i].points)
        i += 1
    
    rp = True
    rn = True
    sp = True
    sn = True

    for i in current:
        if current[i] > 0:
            sn = False
        if current[i] < 0:
            sp = False
    
    i = start
    while (i < n):
        if small[i].value > 0:
            rn = False
        if small[i].value < 0:
            rp = False
        i += 1
    
    if ((rn and sn) or (rp and sp)):
        if totalremtiles < len(current):
            return False
    
    return True

def checkPlacementsRecur(big, small, placements, restriction = Utils.MAXBB, zeroOK = True):
    if len(placements) >= len(small):
        return checkPlacementsFixed(big, placements, zeroOK)
    
    index = len(placements)

    if not isEvenPossible(big, small, placements, zeroOK):
        return False

    tester = copy.deepcopy(small[index])
    i = 0
    if tester.fixed:
        i = 3

    while (i < 4):
        bp = tester.getPlaceables(big)
        places = Utils.getAllPoints(Utils.intersectionAABB(bp, restriction))

        for p in places:
            tester.moveBLTo(p)
            if tester.OOB(restriction):
                continue
            placements.append(tester)
            if checkPlacementsRecur(big, small, placements, restriction, zeroOK):
                return True
            placements.pop()
        
        tester.rotate(1)

        i += 1
    return False

def checkPlacements(big, small, restriction = Utils.MAXBB, zeroOK = True, precheck = True):
    sum = 0
    for i in small:
        sum += i.value * len(i.points)
    
    if precheck:
        if sum != big.value * len(big.points):
            if zeroOK:
                if sum != 0:
                    return False
            return False
        
    placements = []
    return checkPlacementsRecur(big, small, placements, restriction, zeroOK)

def vdomino():
    return BlockGroup([(0, 0), (0, 1)])
def hdomino():
    return BlockGroup([(0, 0), (1, 0)])
def unit():
    return BlockGroup([(0, 0)])
def corner():
    return BlockGroup([(0, 0), (1, 0), (0, 1)])
def I3():
    return BlockGroup([(0, 0), (1, 0), (2, 0)])

def I():
    return BlockGroup([(0, 0), (1, 0), (2, 0), (3, 0)])
def O():
    return BlockGroup([(0, 0), (1, 0), (0, 1), (1, 1)])
def T():
    return BlockGroup([(0, 0), (1, 0), (0, 1), (-1, 0)])
def S():
    return BlockGroup([(0, 0), (1, 0), (1, 1), (-1, 0)])
def Z():
    return BlockGroup([(0, 0), (1, 0), (0, 1), (-1, 1)])
def J():
    return BlockGroup([(0, 0), (1, 0), (-1, 1), (-1, 0)])
def L():
    return BlockGroup([(0, 0), (1, 0), (1, 1), (-1, 0)])