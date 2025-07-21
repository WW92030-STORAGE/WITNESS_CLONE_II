# BlockGroup

`class BlockGroup : public ColorEntity`

## Description

PuzzleEntity representing a Polyomino

## Notes

- The algorithm used to do the checks is brute force backtracking with some pruning involved. 

## Fields

- `points: Utils::pointSet` - The list of points the BlockGroup covers.
- `value: int = 1` - The value each square in the BlockGroup has
- `fixed: bool = true` - If the blockgroup is fixed or can be rotated
- `boundingbox: Utils::AABB` - The bounding box of the tiles

## Constructors

- `BlockGroup()`
- `BlockGroup(Utils::pointSet ps)`
- `BlockGroup(Utils::pointVec ps)`

## Functions

- `bool operator<(const BlockGroup& other) const` - Compare by value, then if it's fixed, then by bounding box, then points
- `void computeBoundingBox()` - Compute the bounding box
- `void resetBoundingBox()` - Sort the corners of the bounding box
- `void translate(Utils::point p)` - Fixed displacement `p`
- `void moveBLTo(Utils::point p)` - Move the bottom left of the bounding box to a target
- `void normalize()` - Equivalent to `moveBLTo({0, 0})`
- `void rotate(int k = 1)` - Rotate 90 degrees counterclockwise, `k` times.
- `void rotateAround(Utils::point s = std::make_pair(0, 0), int k = 1)` - Rotate around the point `s`

- `Utils::AABB getPlaceables(BlockGroup toPlaceOn)` - A list of targets (represented as an AABB) of where you can move the BL to so that the bounding box of the BlockGroup touches the bounding box of toPlaceOn
- `Utils::AABB getPlaceables(Utils::AABB ab)` - A list of targets to move the BL of the bounding box so the entire shape lies within the AABB.
- `bool OOB(Utils::AABB ab)` - Is at least one square of the BlockGroup outside the AABB?

# Static Instances and Constants

A BlockGroup has a set of points (coordinates, squares, etc.) but they all have the same value. If you stack multiple of them together some places have different values. For this we use a `std::map<Utils::point, int>`.

- `static std::map<Utils::point, int> getSum(std::vector<BlockGroup> bgs)` - Get the sum of stacking the blockgroups, represented as mapping coordinates to sum value. Coordinates that map to zero are not included.
- `static std::map<Utils::point, int> add(std::map<Utils::point, int> sum, BlockGroup bg, int scale = 1)` - Get the result when adding `scale` copies of `bg` to the `sum` map.
- `static std::map<Utils::point, int> scale(std::map<Utils::point, int> sum, int scale = 1)` - Scale the sum
- `static std::map<Utils::point, int> add(std::map<Utils::point, int> sum, std::vector<BlockGroup> bgs, int scale = 1)` - Add all blockgroups in `bgs` to the sum. This is slightly more efficient than doing `add` to each BlockGroup individually.

The next methods deal with checking coverings of BlockGroups. All `checkPlacements*` methods return true when the `small` blockgroup list is able to (under certain constraints) exactly cover the `big` blockgroup (i.e. the `getSum` of the small blockgroups can be made to be equivalent to the `big` blockgroup). The `zeroOK` flag, when enabled, also allows the functions to return true if the `getSum` of the small blockgroups is all zeros (empty set).

- `static bool checkPlacementsFixed(BlockGroup big, std::vector<BlockGroup> small, bool zeroOK = true)` - Without modifying the positions or rotations of any of the blocks.
- `static bool checkPlacementsRecur(BlockGroup big, std::vector<BlockGroup> small, std::vector<BlockGroup>& placements, Utils::AABB restriction = Utils::MAXBB, bool zeroOK = true)` - Helper method: with the blocks in `placements` unaltered but the remaining blocks in `small` free to place arbitrarily. The blocks must also lie entirely within `restriction`.
- `static bool isEvenPossible(BlockGroup big, std::vector<BlockGroup> small, std::vector<BlockGroup>& placements, bool zeroOK = true)` - A simple but weak check to prune some cases.
- `static bool checkPlacements(BlockGroup big, std::vector<BlockGroup> small, Utils::AABB restriction = Utils::MAXBB, bool zeroOK = true, bool precheck = false)` - No pre-placed pieces, all pieces in `small` can be rotated and moved, but all placed pieces must fall within `restriction`. If `precheck` is enabled a simple check is done to prune cases where solving is known impossible.

And finally, a few pre-defined values:

- `const BlockGroup vdomino         = BlockGroup(std::vector<Utils::point>({{0, 0}, {0, 1}}));`
- `const BlockGroup hdomino         = BlockGroup(std::vector<Utils::point>({{0, 0}, {1, 0}}));`
- `const BlockGroup unit            = BlockGroup(std::vector<Utils::point>({{0, 0}}));`
- `const BlockGroup corner          = BlockGroup(std::vector<Utils::point>({{0, 0}, {1, 0}, {0, 1}}));`
- `const BlockGroup I3              = BlockGroup(std::vector<Utils::point>({{0, 0}, {1, 0}, {2, 0}}));`

- `const BlockGroup I               = BlockGroup(std::vector<Utils::point>({{0, 0}, {1, 0}, {2, 0}, {3, 0}}));`
- `const BlockGroup O               = BlockGroup(std::vector<Utils::point>({{0, 0}, {1, 0}, {0, 1}, {1, 1}}));`
- `const BlockGroup T               = BlockGroup(std::vector<Utils::point>({{0, 0}, {1, 0}, {0, 1}, {-1, 0}}));`
- `const BlockGroup S               = BlockGroup(std::vector<Utils::point>({{0, 0}, {0, 1}, {1, 1}, {-1, 0}}));`
- `const BlockGroup Z               = BlockGroup(std::vector<Utils::point>({{0, 0}, {1, 0}, {0, 1}, {-1, 1}}));`
- `const BlockGroup J               = BlockGroup(std::vector<Utils::point>({{0, 0}, {1, 0}, {-1, 1}, {-1, 0}}));`
- `const BlockGroup L               = BlockGroup(std::vector<Utils::point>({{0, 0}, {1, 0}, {1, 1}, {-1, 0}}));`