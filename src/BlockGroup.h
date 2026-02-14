#ifndef BLOCKGROUP_WITCLN
#define BLOCKGROUP_WITCLN

/*

Class representing a group of tiles in the coordinate grid. Tiles can be rotated, can represent +/-1 quantities.
Functions exist to test if a given group of BlockGroups (including +/-) can exactly cover a larger BlockGroup: 
That is, to arrange the small BlockGroups on a region the shape of the large BlockGroup such that:

1. The cells inside the region all have net sum +1
2. All outside cells have sum 0 (If no BlockGroup hits a cell then naturally its sum is zero).

Checks can also restrict the bounding box of the placed BlockGroups (because in the actual puzzles BlockGroups cannot be placed outside the grid).

*/

#include <iostream>

#include <cstdint>
#include <climits>
#include <set>
#include <string>

#include "PuzzleEntity.h"
#include "Utils.h"



class BlockGroup : public ColorEntity {
    public:

    Utils::pointSet points; // These will also be referred to as tiles.
    int value = 1; // How much a tile in this group contributes when placed.
    bool fixed = true; // If this BlockGroup is fixed and cannot rotate.
    Utils::AABB boundingbox; // Current bounding box of the entity

    void init() {
        type = "BlockGroup";
        disp = "BG";
        disp = disp + (fixed ? "X" : "O") + std::to_string(clamp(value, -99, 99));
        while (disp.size() < 6) disp = disp + " ";

        if (value < 0) color = EntityColor::RGB_BLUE;

        computeBoundingBox();
    }

    BlockGroup() : ColorEntity() {
        color = EntityColor::RGB_YELLOW;
        points.insert({0, 0});
        init();
    }

    BlockGroup(Utils::pointSet ps) : ColorEntity() {
        color = EntityColor::RGB_YELLOW;
        for (auto i : ps) points.insert(i);
        init();
    }

    BlockGroup(Utils::pointVec ps) : ColorEntity() {
        color = EntityColor::RGB_YELLOW;
        for (auto i : ps) points.insert(i);
        init();
    }

    BlockGroup(Utils::pointSet ps, int val) : ColorEntity() {
        color = EntityColor::RGB_YELLOW;
        for (auto i : ps) points.insert(i);
        value = val;
        init();
    }

    BlockGroup(Utils::pointVec ps, int val) : ColorEntity() {
        color = EntityColor::RGB_YELLOW;
        for (auto i : ps) points.insert(i);
        value = val;
        init();
    }

    BlockGroup(const BlockGroup& other) : ColorEntity(other) {
        for (auto i : other.points) points.insert(i);
        value = other.value;
        fixed = other.fixed;
        init();
    }

    BlockGroup* clone() override {
        return new BlockGroup(*this);
    }

    bool operator<(const BlockGroup& other) const {
        if (value != other.value) return value < other.value;
        if (fixed != other.fixed) return fixed < other.fixed;
        if (boundingbox != other.boundingbox) return boundingbox < other.boundingbox;
        return points < other.points;
    }


    // Compute the bounding box

    void computeBoundingBox() {
        boundingbox = {{INT_MAX, INT_MAX}, {INT_MIN, INT_MIN}};

        for (auto i : points) {
            boundingbox.first.first = std::min(boundingbox.first.first, i.first);
            boundingbox.first.second = std::min(boundingbox.first.second, i.second);

            boundingbox.second.first = std::max(boundingbox.second.first, i.first);
            boundingbox.second.second = std::max(boundingbox.second.second, i.second);
        }

        boundingbox = Utils::resetBB(boundingbox);
    }

    // Swap corners of the bb if needed
    void resetBoundingBox() {
        boundingbox = Utils::resetBB(boundingbox);
    }

    // Move and rotate the object

    void translate(Utils::point p) {
        Utils::pointSet ss;
        for (auto i : points) ss.insert(Utils::add(i, p));
        points = ss;

        boundingbox.first = Utils::add(boundingbox.first, p);
        boundingbox.second = Utils::add(boundingbox.second, p);
    }

    // Move the BL of this->boundingbox to a location
    void moveBLTo(Utils::point p) {
        translate(Utils::sub(p, boundingbox.first));
    }

    // Move so the lower left corner of the bounding box is at (0, 0)
    void normalize() {
        translate(Utils::inv(boundingbox.first));
    }

    // Rotate counterclockwise by 90 degrees
    void rotate(int k = 1) {
        while (k < 0) k += 4;
        k %= 4;
        if (k <= 0) return;
        if (k > 1) rotate(k - 1);
        Utils::pointSet ss;
        for (auto i : points) ss.insert(Utils::rot(i));
        points = ss;

        boundingbox.first = Utils::rot(boundingbox.first);
        boundingbox.second = Utils::rot(boundingbox.second);

        resetBoundingBox();
    }

    // Rotate around a specific point
    void rotateAround(Utils::point s = std::make_pair(0, 0), int k = 1) {
        translate(Utils::inv(s));
        rotate(k);
        translate(s);
    }

    /*
    
    Arranging blocks and checks
    
    */

    // Where can I move the bounding box's BL so this->boundingbox will overlap or at least touch toPlaceOn.boundingbox
    Utils::AABB getPlaceables(BlockGroup toPlaceOn) {
        auto d = Utils::dims(boundingbox);
        return {Utils::sub(toPlaceOn.boundingbox.first, d), Utils::add(toPlaceOn.boundingbox.second, {1, 1})};
    }

    // Where can I move the bounding box's BL so the entire shape falls inside the AABB provided?
    Utils::AABB getPlaceables(Utils::AABB ab) {
        auto d = Utils::dims(boundingbox);
        return {ab.first, Utils::add(Utils::sub(ab.second, d), {1, 1})};
    }
    
    // Is the current placement out of bounds?
    bool OOB(Utils::AABB ab) {
        for (auto i : points) {
            if (!Utils::contains(ab, i)) return true;
        }
        return false;
    }

    // The sum of placing down all the BGs
    static std::map<Utils::point, int> getSum(std::vector<BlockGroup> bgs) {
        std::map<Utils::point, int> sum;
        return add(sum, bgs);
    }

    // Add another BG to our sum
    static std::map<Utils::point, int> add(std::map<Utils::point, int> sum, BlockGroup bg, int scale = 1) {
        for (auto p : bg.points) {
            if (sum.find(p) == sum.end()) sum.insert({p, 0});
            (sum.find(p))->second += bg.value * scale;
        }

        std::map<Utils::point, int> res; // clear out all 0 sum points
        for (auto i : sum) {
            if (i.second != 0) res.insert(i);
        }
        return res;
    }

    // Scale the sum
    static std::map<Utils::point, int> scale(std::map<Utils::point, int> sum, int scale = 1) {
        std::map<Utils::point, int> res;
        for (auto i : sum) res.insert({i.first, i.second * scale});
        return res;
    }

    // Add another BGs to our sum
    static std::map<Utils::point, int> add(std::map<Utils::point, int> sum, std::vector<BlockGroup> bgs, int scale = 1) {
        for (auto bg : bgs) {
            for (auto p : bg.points) {
                if (sum.find(p) == sum.end()) sum.insert({p, 0});
                (sum.find(p))->second += bg.value * scale;
            }
        }

        std::map<Utils::point, int> res; // clear out all 0 sum points
        for (auto i : sum) {
            if (i.second != 0) res.insert(i);
        }
        return res;
    }

    /*
    
    Testing the coverage of "small" BlockGroups on a "big" BlockGroup. 
    In each method the zeroOK flag is whether or not a total sum of zero in the small BGs is acceptable. Otherwise, the total sum of the small and big BGs must be zero.
    (i.e. the small BGs must cover the shape of the big BG exactly with 1 net tile on each point.)
    
    */

    // Tests if the small BGs cover the big BG without any modifications to position or rotation
    static bool checkPlacementsFixed(BlockGroup big, std::vector<BlockGroup> small, bool zeroOK = true) {
        auto smol = getSum(small);
        
        if (zeroOK && smol.size() <= 0) return true;
        auto total = add(smol, big, -1);
        return total.size() <= 0;
    }

    // Simple weak check -- if this returns false then it is guaranteed that the remaining placements are impossible
    static bool isEvenPossible(BlockGroup big, std::vector<BlockGroup> small, std::vector<BlockGroup>& placements, bool zeroOK = true) {
        auto current = add(getSum(placements), big, -1);
        int start = placements.size();
        int n = small.size();

        int totalremtiles = 0;
        for (int i = start; i < n; i++) totalremtiles += small[i].points.size();
        
        // The first check checks if the total number of remaining placeable tiles is enough. If not we return false
        // This only works if all remaining tiles have the same sign, all tiles in the sum have the same sign, and those two signs are the same.
        bool rp = true;
        bool rn = true;
        bool sp = true;
        bool sn = true;
        for (auto i : current) {
            if (i.second > 0) sn = false;
            if (i.second < 0) sp = false;
        }
        for (int i = start; i < n; i++) {
            if (small[i].value > 0) rn = false;
            if (small[i].value < 0) rp = false;
        }

        if ((rn && sn) || (rp && sp)) {
            if (totalremtiles < current.size()) {
                return false;
            }
        }

        return true;
    }

    static bool checkPlacementsRecur(BlockGroup big, std::vector<BlockGroup> small, std::vector<BlockGroup>& placements, Utils::AABB restriction = Utils::MAXBB, bool zeroOK = true) {
        
        if (placements.size() >= small.size()) return checkPlacementsFixed(big, placements, zeroOK);
        int index = placements.size();

        if (!isEvenPossible(big, small, placements, zeroOK)) return false;
        // std::cout << placements.size() << " " << small.size() << " " << places.size() << "\n";

        BlockGroup tester(small[index]);
        for (int i = (tester.fixed) ? 3 : 0; i < 4; i++) {
            auto bp = tester.getPlaceables(big);
            auto places = Utils::getAllPoints(Utils::intersection(bp, restriction));

            for (auto p : places) {
                tester.moveBLTo(p);
                if (tester.OOB(restriction)) continue;
                placements.push_back(tester);
                if (checkPlacementsRecur(big, small, placements, restriction, zeroOK)) return true;
                placements.pop_back();
            }
            tester.rotate(1);
        }

        return false;
    }

    static bool checkPlacements(BlockGroup big, std::vector<BlockGroup> small, Utils::AABB restriction = Utils::MAXBB, bool zeroOK = true, bool precheck = true) {
        int64_t sum = 0;
        for (auto i : small) {
            sum += i.value * i.points.size();
        }
        if (precheck) {
        if (sum != big.value * big.points.size()) {
            if (zeroOK) {
                if (sum != 0) return false;
            }
            else return false;
        }
        }
        std::vector<BlockGroup> placements;

        std::sort(small.begin(), small.end());

        return checkPlacementsRecur(big, small, placements, restriction, zeroOK);
    }

    /*
    
    Debug/Display

    */

    std::string to_string() {
        std::string res = "BlockGroup[fixed=" + std::to_string(fixed) + ", value=" + std::to_string(value) + ", bb=(";
        res = res + std::to_string(boundingbox.first.first) + ", " + std::to_string(boundingbox.first.second) + "), (";
        res = res + std::to_string(boundingbox.second.first) + ", " + std::to_string(boundingbox.second.second) + ")][";
        bool ss = 0;
        for (auto i : points) {
            if (ss) res = res + ", ";
            ss = true;
            res = res + "(" + std::to_string(i.first) + ", " + std::to_string(i.second) + ")";
        }
        return res + "]";
    }
};

// CONSTANTS AND PRELOADED BLOCKS

namespace BGS {

const BlockGroup vdomino = BlockGroup(std::vector<Utils::point>({{0, 0}, {0, 1}}));
const BlockGroup hdomino = BlockGroup(std::vector<Utils::point>({{0, 0}, {1, 0}}));
const BlockGroup unit = BlockGroup(std::vector<Utils::point>({{0, 0}}));
const BlockGroup corner = BlockGroup(std::vector<Utils::point>({{0, 0}, {1, 0}, {0, 1}})); // Corner shape with the arms up and right
const BlockGroup I3 = BlockGroup(std::vector<Utils::point>({{0, 0}, {1, 0}, {2, 0}})); // Horizontal 3x1 piece


const BlockGroup I = BlockGroup(std::vector<Utils::point>({{0, 0}, {1, 0}, {2, 0}, {3, 0}})); // Horizontal 4x1
const BlockGroup O = BlockGroup(std::vector<Utils::point>({{0, 0}, {1, 0}, {0, 1}, {1, 1}})); // 2x2
const BlockGroup T = BlockGroup(std::vector<Utils::point>({{0, 0}, {1, 0}, {0, 1}, {-1, 0}})); // T shape with the stem doing downwards
const BlockGroup S = BlockGroup(std::vector<Utils::point>({{0, 0}, {0, 1}, {1, 1}, {-1, 0}})); // Horizontal S shape
const BlockGroup Z = BlockGroup(std::vector<Utils::point>({{0, 0}, {1, 0}, {0, 1}, {-1, 1}})); // Horizontal Z shape
const BlockGroup J = BlockGroup(std::vector<Utils::point>({{0, 0}, {1, 0}, {-1, 1}, {-1, 0}})); // Horizontal J shape with the long side at the bottom
const BlockGroup L = BlockGroup(std::vector<Utils::point>({{0, 0}, {1, 0}, {1, 1}, {-1, 0}})); // Horizontal L shape with the long side at the bottom

};

#endif