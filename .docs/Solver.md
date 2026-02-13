# solver/Solver

`class Solver`

## Description

Automatic solver for all kinds of Grids.

## Notes

- The stored grid is a pointer. `&grid` is enough in most cases, assuming that you don't use the solver when `grid` goes out of scope or after it is freed.

## Fields

- `grid: Grid*` - The grid we are solving. The represented entity can be a `Grid` or one of its subclasses.
- `numEnds: int` - Variable used for solving.
- `vis: std::map<Utils::point, Utils::point>` - A map of visited points to their immediate predecessors. This should not be used for external reference, but instead...
- `solutions: std::vector<Utils::pointVec>` - A list of discovered solutions. If the number of solutions is restricted this will also be.
- `PRNG: RandGen` - The random number generator used to decide certain orders.

## Constructors

- `Solver()`
- `Solver(int64_t seed)`
- `void seed(int64_t see)`

## Functions

- `void path(Utils::point src, Utils::point prev, int numsol = 1, bool prune = true, bool verbose = false)` - Recursive path function. `src` is the current point, `prev` the most recent point in our current path stub, `numsol` is at most how many solutions will be discovered. `prune` toggles a pruning strategy.
- `void solve(int numsol = 1, bool verbose = false)` - Populate `solutions` with at most `numsol` solutions. Be aware the complexity for this increases exponentially with grid size.
- `void apply(int x = 0)` - Given a populated `solutions` vector, apply the `x`th solution to the stored `grid`.

## Deprecated/Unused Functions

- `int hash(Utils::point a, Utils::point b)` - Quick PRNG hash for two points.