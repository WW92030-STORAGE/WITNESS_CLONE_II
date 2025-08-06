# Grid

`class Grid`

## Description

Grids that contain `PuzzleEntity` objects. This documentation encompasses all forms of `Grid` objects and their subclasses in `Grid.h` and `SymmetryGrid.h`.

## Notes

- The grid is twice as large as it looks. A grid of `MxN` internal grid cells is represented as `(2M + 1)x(2N + 1)`.
- Coordinates are in `(x, y)` where `x` goes right and `y` goes up. This also applies to array indexing, so the origin `[0][0] = (0, 0)` is the bottom left of the grid.
- The points (entries) with both odd coordinates are referred to as `internal cells, cells, grid cells, etc.`. All others, with at least one even coordinate, are referred to as `edges`.

- WARNING - DO NOT EVER reassign grid variables. This will result in a segfault. If you want to reassign a grid variable you must use a new variable, and let the scoping system delete the grids when they end scope.

- Subclasses of `Grid` maintain largely the same methods and everything, except with different mechanics (e.g. a Symmetry grid might have `drawLine` draw two distinct lines on the grid.)

## Fields

- `R, C: int = {3, 3}` - Number of Rows and Columns.
- `board: PuzzleEntity***` - A matrix of pointers to `PuzzleEntity` objects.

## Constructors

- `Grid()`
- `Grid(int A, int B)` - `{R, C} = {A, B}`

## Functions

- `void init()` - Ensure the dimensions are odd, and insert default `PuzzleEntity` objects into the matrix.
- `bool inBounds(int x, int y)` - Is a point `(x, y)` in the bounds of the Grid?
- `bool inBounds(std::pair<int, int> p)`
- `PuzzleEntity* get(int x, int y)` - Returns null if OOB
- `PuzzleEntity* get(std::pair<int, int> p)`
- `EntityColor::Color getColor(int x, int y)`
- `EntityColor::Color getColor(std::pair<int, int> p)`
- `virtual Utils::pointSet neighbors(std::pair<int, int> p)` - Gets neighbors that are inside the bounds
- `void reset(int r, int c)` - Reset `board[r][c]` to an empty `PuzzleEntity` (Because this changes the pointer address but not the values in pointesr, the original value is deleted)
- `set(int r, int c, PuzzleEntity* p)` - Set to entity
- `void clearAllPaths()` - Clear all paths (except endpoints) and path dots from the board
- `void defaultPaths()` - Put paths on all coordinates with at least one even coordinate
- `virtual void defaultDiagonal()` - Put paths on all possible coordinates, a start point at (0, 0), and an end point at the top right.
- `virtual void setPath(int x, int y, bool z)` - Set the `isPath` of an entry.
- `virtual void setPath(std::pair<int, int> p, bool z)` 
- `virtual void setLine(int x, int y, uint8_t z)` - Set the `hasLine` of an entry.
- `virtual void setLine(std::pair<int, int> p, uint8_t z)` 
- `void setBlocker(int x, int y, bool z)` - Set the `isBlocker` of an entry.
- `void setBlocker(std::pair<int, int> p, bool z)` 
- `virtual void drawLine(int x1, int y1, int x2, int y2, uint8_t index = 1)` - Draw a Line between two points that are axis aligned.
- `virtual void drawLine(std::pair<int, int> a, std::pair<int, int> b, uint8_t index = 1)`
- `virtual void drawPath(std::vector<std::pair<int, int>> path, uint8_t index = 1)` - Draw a path between consecutive points in the path.
- `virtual void clearLine(int x1, int y1, int x2, int y2)` - Clear the Line from the points in the line connecting the inputs.
- `virtual void clearAllLines()` - Erases all lines.
- `Utils::pointSet getSingleLine(Utils::point p)` - Returns the connected component of `hasLine` entries containing `p` (such a component is called a `path`)
- `std::vector<Utils::pointSet> getLines()` - Returns all connected components of hasLine tiles (such a connected component is called a `path`)
- `bool validateSinglePath(Utils::point p)` - Is the path containing `p` from a start to end, continuous, and non branching?
- `bool validatePath()` - Are all paths (from `getLines()`) from start to end, continuous, and non-branching?

# Static Instances and Constants

- Static instances of the objects and static functions that generate objects, intended for convenience or as special values.