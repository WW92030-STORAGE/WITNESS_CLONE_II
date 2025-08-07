from typing import Tuple, Set, List, Dict, Any
from collections import deque

INF = (2 ** 31) - 1

point = Tuple[int, int]
pointSet = Set[point]
pointVec = List[point]
pointQueue = deque[point]

AABB = Tuple[point, point]

dx = [1, 0, -1, 0]
dy = [0, 1, 0, -1]

NILPT = (-1 * INF, -1 * INF)
MINPT = (-1 * INF, -1 * INF)
MAXPT = (INF, INF)
NILBB = (NILPT, NILPT)
MAXBB = (MINPT, MAXPT)

# Some methods are deprecated or removed in the python context

def disp(s: point):
    return str(s)

def disp(s: pointVec):
    return str(s)

def contains(ab: AABB, p: point):
    if (p[0] < ab[0][0]) or (p[0] > ab[1][0]):
        return False
    if (p[1] < ab[0][1]) or (p[1] > ab[1][1]):
        return False
    return True

# Using a Dict[T, int] as a frequency table
def push(m: Dict[Any, int], t):
    if t not in m:
        m[t] = 0
    m[t] += 1

def pop(m: Dict[Any, int], t):
    if t not in m:
        return
    m[t] -= 1
    if m[t] <= 0:
        m.discard(t)

# Using a Dict[K, Set[V]] as a classifier

def append(m: Dict[Any, Set[Any]], k, v):
    if k not in m:
        m[k] = set()
    
    m[k].add(v)

# Actions on points and AABBs

def add(a: point, b: point):
    return (a[0] + b[0], a[1] + b[1])

def inv(x: point):
    return (-1 * x[0], -1 * x[1])

def sub(a: point, b: point):
    return add(a, inv(b))

def rot(p: point):
    return (-1 * p[1], p[0])

def makeInterval(p: point):
    if (p[0] < p[1]):
        return (p[0], p[1])
    return (p[1], p[0])

def resetBB(boundingbox: AABB):
    BL = (min(boundingbox[0][0], boundingbox[1][0]), min(boundingbox[0][1], boundingbox[1][1]))
    TR = (max(boundingbox[0][0], boundingbox[1][0]), max(boundingbox[0][1], boundingbox[1][1]))
    return (BL, TR)

def makeAABB(xx: point, yy: point):
    x = makeInterval(xx)
    y = makeInterval(yy)
    return ((x[0], y[0]), (x[1], y[1]))

def dims(x: AABB):
    return (abs(x[1][0] - x[0][0]) + 1, abs(x[1][1] - x[0][1]) + 1)

def getX(ab: AABB):
    return makeInterval((ab[0][0], ab[1][0]))

def getY(ab: AABB):
    return makeInterval((ab[0][1], ab[1][1]))

def intervalIntersect(a: point, b: point):
    if (a[0] > b[0]):
        return intervalIntersect(b, a)
    if (b[0] > a[1]):
        return NILPT
    return makeInterval((max(a[0], b[0]), min(a[1], b[1])))

def intersectionAABB(a: AABB, b: AABB):
    xi = intervalIntersect(getX(a), getX(b))
    yi = intervalIntersect(getY(a), getY(b))
    if xi == NILPT or yi == NILPT:
        return NILBB
    return makeAABB(xi, yi)

def getAllPoints(ab: AABB):
    ab = resetBB(ab)
    
    res = set()
    x = getX(ab)[0]
    while x <= getX(ab)[1]:
        y = getY(ab)[0]
        while y <= getY(ab)[1]:
            res.add((x, y))
            y += 1
        x += 1
    return res

# NO LONGER IN NAMESPACE UTILS

def getCells(s: pointSet):
    res = set()
    for i in s:
        if (i[0] % 2 == 1) and (i[1] % 2 == 1):
            res.add(i)
    return res

def getEdges(s: pointSet):
    res = set()
    for i in s:
        if (i[0] % 2 == 0) or (i[1] % 2 == 0):
            res.add(i)
    return res

def neighbors(p: point):
    res = set()
    for d in range(4):
        next = (p[0] + dx[d], p[1] + dy[d])
        res.add(next)
    return res

def instanceof(T: Any, C: Any):
    return isinstance(T, C)

def intersection(a: Set[Any], b: Set[Any]):
    if (len(a) > len(b)):
        return intersection(b, a)
    res = set()
    for i in a:
        if i in b:
            res.add(i)
    return res

def clamp(x, L, R):
    if x < L:
        return L
    if x > R:
        return R
    return x

def to_string(n: int, b: int):
    if n == 0:
        return "0"
    base = clamp(base, 2, 9)

    res = ""
    N = n
    while (N > 0):
        d = chr(ord('0') + (N % base))
        res = str(d) + res
        N //= base
    
    return res

def count_chars(s, c):
    res = 0
    for i in s:
        if i == c:
            res += 1

    return res

def rem(a, m):
    a %= m
    while (a < 0):
        a += m
    return a % m

def modexp(b, e, m = 1000000007):
    if (e <= 0):
        return 1
    if (e % 2 == 0):
        return rem(b * modexp(b, e - 1, m), m)
    sqxx = modexp(b, e // 2, m)
    return rem(sqxx * sqxx, m)

if __name__ == "__main__":
    pt = {}
    append(pt, 4, 2)
    append(pt, 5, 2)
    append(pt, 4, 3)
    print(pt)


