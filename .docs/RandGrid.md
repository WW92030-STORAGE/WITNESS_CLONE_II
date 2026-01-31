# .RandGrid/RandGrid

`template <typename GridT> class RandGrid<GridT>`

## Description

Generator of random grids. The type `GridT` is assumed to be a subclass of `Grid`.

## Notes

- `GridT` is a type that is held per instance of `RandGrid`.

## Fields

- `PRNG: RandGen` - A randomizer for decision making
- `storedpaths: std::vector<Utils::pointVec>` - A cache of valid paths across the grid. This is populated using `pathfind()`.
- `chosenpath: Utils::pointSet` - The chosen path to generate a puzzle from. This can be set using `setChosenPath()` and randomly picked using `pickRandomPath()`. 
- `auxiliaryPaths: std::map<int, Utils::pointSet>` - Additional paths that correspond to the `chosenpath` when drawn (e.g. symmetry grids).
- `R, C: int64_t = {9, 9}` - Dimensions of the generated grids.
- `start, ends: Utils::pointSet` - The start and end points of each generated grid.

## Constructors

- `RandGrid()`
- `RandGrid(int64_t seed)`
- `RandGrid(int64_t r, int64_t c)`
- `RandGrid(int64_t r, int64_t c, int64_t seed)`

## Functions

- `void init()` - Preinitializes the system, `starts + ends` are populated with the start/endpoints of `GridT::defaultDiagonal()`.
- `virtual void setChosenPath(int x)` - Set the chosen path to the `x`th path in `storedpaths`. This also modifies the `auxiliaryPaths` as needed.
- `void pickRandomPath()` - Set a random path as chosen.
- `virtual void applyChosenPath(GridT* grid)` - Apply the chosen path to the grid, drawing lines.
- `bool isOnChosenPath(Utils::point p)` - Is the point on the primary chosen path?
- `bool isOnAnyPath(Utils::point p)` - Is the point on the chosen path or any auxiliaries?
- `GridT blankGrid()` - Initialize a blank grid using the `starts` and the `ends`.
- `void seed(int64_t see)` - Seed the PRNG.
- `void pathfind(int numPaths = INT_MAX, int see = 0)` - Finds at most `numPaths` paths on the grid, then populates the `storedpaths`. The seed is the seed to seed the internal solver -- if it's zero then the solver is default seeded.
- `std::vector<int> randomChoice(int n, int k)` - Pick `k` random indices in `[0, n]`. The chosen (indices, points, intersections, edges, etc.) are randomly ordered.
- `Utils::pointVec getRandomCells(int k)` - Get at most `k` random cells in the grid. The chosen are randomly ordered.
- `Utils::pointVec getRandomEdges(int k, bool noLine = false, bool line = false)` - Get random non-intersection edges on the grid. If `noLine` is set then the chosen points will be off the primary and auxiliary paths, if `line` is set then the chosen points will be on the primary or auxiliary paths. Obviously if both are set then no points can be chosen.
- `Utils::pointVec getRandomNodes(int k, bool noLine = false, bool line = false)` - Finds at most `k` random intersections on the grid.
- `Utils::pointVec getRandomPaths(int k, bool noLine = false, bool line = false)` - Finds at most `k` random edges (non-intersections + intersections) on the grid.
- `std::vector<EntityColor::Color> getRandomColors(int k = INT_MAX)` - Returns a list of at most `k` random colors in random order. By default all colors are returned.

## Grid Creation Functions

- `Grid randStars()` - This one is special - it always generates a `defaultDiagonal()` `Grid` with eight randomly placed stars, four of each color. Brute-force testing has shown all grids of this kind to be solvable. It is not solvable if there are more stars.
- `GridT randStarsGeneral(int numPairs = 4, int numCuts = 2)` - Creates a grid with at most `numPairs` pairs of stars, and at most `numCuts` cuts.
- `GridT randMaze(int cuts = 10)` - Generates a random maze with at most `numCuts` cuts and no other symbols. (`numCuts` will retain the same meaning for most other methods.)
- `GridT randDots(int numSymbols = 8, int numCuts = 2, double threshold = 0.5)` - Generates a grid with at most `numSymbols` dots. If there are auxiliary lines, dots become restricted to a particular line with probability `threshold`.
- `GridT randBlobs(int numSymbols = 10, int numCols = 3, int numCuts = 2)` - At most `numSymbols` blobs, in at most `numCols` colors.
- `randTriangles(int numSymbols = 10, int numCuts = 2)` - At most `numSymbols` triangle cells.
- `GridT randBlocks(int numSymbols = 3, int numRegions = 2, int numCuts = 2, double rotation = 0.1)` - At most `numSymbols` symbols, divided among at most `numRegions` regions. Each BlockGroup has probability `rotation` of being non-fixed.
- `GridT randBlocksByRegion(int maxBlocksPerRegion = 5, int numRegions = 2, int numCuts = 2, double rotation = 0.1)` - Tries to divide the blocks so each block has on average at most `maxBlocksPerRegion`