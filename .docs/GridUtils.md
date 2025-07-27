# GridUtils/GridUtils

`namespace GridUtils`

## Description

Grid utils and solution checking

## Notes

- The solution checker might not be the best at its job but it does what it's supposed to.

## Functions

- `template <typename T> Utils::pointSet getSymbols(Grid* g)` - Get locations for all symbols of a certain type
- `template <typename T> Utils::pointSet getActiveSymbols(Grid* g)` - Get locations for all ACTIVE symbols of a type
- `Utils::pointSet getSymbolsColor(Grid* g, EntityColor::Color c)` - Get all locations for symbols of a color
- `std::map<EntityColor::Color, int> getColors(Grid* g, Utils::pointSet s, bool NotNil = true)` - Get the counts of colors within the `s` point set.
- `std::map<EntityColor::Color, int> getActiveColors(Grid* g, Utils::pointSet s, bool NotNil = true)` - Get the counts of colors of active symbols within `s`.
- `bool isFFable(Grid* g, Utils::point next)` - We can flood fill (FF) to a point if it's in bounds, does not have line drawn on it, and does not block FF.
- `Utils::pointSet floodfill(Grid* g, Utils::point p)` - Flood fill across edges and grid cells, starting from a point `p`
- `Utils::pointSet floodfill(Grid* g, int x, int y)`
- `Utils::pointSet floodfillCells(Grid* g, Utils::point p)` - Flood fill but only look at and record grid cells. Edges with lines and blockers still block.
- `std::vector<Utils::pointSet> getRegions(Grid* g)` - Get a list of points, separated by region. Line points and blockers are excluded.
- `std::vector<Utils::pointSet> getRegionsCells(Grid* g)` - Get a list of grid cells separated by region.

- `bool validateRegionNoRecur(Grid* grid, Utils::pointSet region)` - Validate all symbols except those that rely on recursively doing more sub-validations and those that rely on other points outside their region, in a region. 
- `Utils::pointSet getViolationsNoRecursion(Grid* grid)` - Validate all symbols except those that rely on recursively doing more sub-validations. Returns a set of violating points.
- `Utils::pointSet getViolations(Grid* grid)` - Validate everything including recursive. Returns a list of violating points.
- `bool Validate(Grid* grid)` - Returns if the grid has a valid solution.