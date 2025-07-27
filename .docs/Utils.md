# Utils/Utils

`namespace Utils`

## Description

Utility typedefs, classes, and functions.

## Notes

- `A::B` refers to the `B` "thing" in the `A` class, namespace, etc.
- This is the `namespace Utils`. 
- The standard form of an AABB is `{BL, TR}` where `BL` is the bottom left corner and `TR` is the top right corner.

## Typedefs

- `point: std::pair<int, int>` - Ordered pair of integers
- `pointSet: std::set<point>`
- `pointQueue: std::queue<point>`
- `pointVec: std::vector<point>`
- `AABB: std::pair<point, point>` - Axis aligned bounding box

## Functions

- `std::string disp(Utils::point s)`
- `std::string disp(Utils::pointSet s)`
- `std::string disp(Utils::pointVec s)`
- `bool contains(Utils::pointSet s, Utils::point p)` - Does a pointSet contain a point?
- `template <typename T> bool contains(std::set<T> s, T p)` - Does a set contain a certain key?
- `bool contains(AABB ab, Utils::point point)` - Is a point inside an AABB?

The following two methods denote using a `std::map<T, int>` as a histogram/frequency table/multiset:

- `template <typename T> void push(std::map<T, int>& m, T t)` - Increment the count of the `t: T` object.
- `template <typename T> void pop(std::map<T, int>& m, T t)` - Decrement the count, remove if results zero, does nothing if already zero.

- `template <typename K, typename V> void append(std::map<K, std::set<V>>& m, K k, V v)` - Use a `std::map<K, std::set<V>>` as a classifier, with multiple entries under keys.

All remaining methods are operations on points and AABBs.

- `std::string to_string(point p)`
- `point add(point a, point b)` - Vector `a + b`
- `point inv(point x)` - Reflect across the origin
- `point sub(point a, point b)` - Vector `a - b`
- `point rot(point a)` - Rotate counterclockwise 90 degrees
- `point makeInterval(point p)` - Sort the components in increasing order
- `AABB resetBB(AABB boundingbox)` - Convert an AABB to standard form with bottom left corner and top right corner stored.
- `AABB makeAABB(point xx, point yy)` - Make an AABB with two opposing corners.
- `point dims(AABB x)` - Dimensions of an AABB, inclusive of the ends.
- `point getX(AABB ab)` - Get the span of the X coordinates.
- `point getY(AABB ab)` - Get the span of the Y coordinates.
- `intervalIntersect(point a, point b)` - Intersection of two intervals
- `intersection(AABB a, AABB b)` - Intersection of two bounding boxes
- `getAllPoints(AABB ab)` - Get all the points in the AABB

# Static Instances and Constants

- `dx: int[4] = {1, 0, -1, 0}` - D-pad with cardinal directions
- `dy: int[4] = {0, 1, 0, -1}`

- `NILPT: const point = {INT_MIN, INT_MIN}`
- `MINPT: const point = {INT_MIN, INT_MIN}`
- `MAXPT: const point = {INT_MAX, INT_MAX}`
- `NILBB: const AABB = {NILPT, NILPT}`
- `MAXBB: const AABB = {MINPT, MAXPT}`



---

# Utils



## Description

Utility things outside `namespace Utils`

## Notes

- This is not `namespace Utils`

## Functions

- `Utils::pointSet getCells(Utils::pointSet s)` - Returns the points that have both odd coordinates. (These points form the grid cells of a puzzle grid, while all others form edges.)
- `Utils::pointSet getEdges(Utils::pointSet s)` - Returns the points with at least one even coordinate.
- `Utils::pointSet neighbors(std::pair<int, int> p)` - Get the neighbors of a point in the cardinal directions
- `template <typename AGAINST, typename TEST> bool instanceof(const TEST* test)` - Is the `TEST test` an instance of `AGAINST`? (Equivalent to `test instanceof AGAINST` in Java).
- `template <typename T> std::set<T> intersection(const std::set<T>& a, const std::set<T>& b)` - Intersection of two sets
- `Utils::pointSet intersection(const Utils::pointSet& a, const Utils::pointSet& b)` - Intersection of two sets
- `int clamp(int x, int L, int H)` - Returns the clamped value for `x` between `L` and `H`.