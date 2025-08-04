#ifndef UTILS_WITCLN
#define UTILS_WITCLN

#include <utility>
#include <set>
#include <map>
#include <queue>
#include <vector>
#include <cmath>

namespace Utils {

typedef std::pair<int, int> point; // Also can be an interval
typedef std::set<point> pointSet;
typedef std::queue<point> pointQueue;
typedef std::vector<point> pointVec;

typedef std::pair<point, point> AABB;

int dx[4] = {1, 0, -1, 0};
int dy[4] = {0, 1, 0, -1};

const point NILPT = {INT_MIN, INT_MIN};
const point MINPT = {INT_MIN, INT_MIN};
const point MAXPT = {INT_MAX, INT_MAX};
const AABB NILBB = {NILPT, NILPT};
const AABB MAXBB = {MINPT, MAXPT};


    /*
    
    Helper functions for non-grids
    
    */

    std::string disp(Utils::point s) {
        return "(" + std::to_string(s.first) + ", " + std::to_string(s.second) + ")";
    }

    std::string disp(Utils::pointSet s) {
        std::string res = "[";
        for (auto i : s) res = res + disp(i);
        return res + "]";
    }

    std::string disp(Utils::pointVec s) {
        std::string res = "[";
        for (auto i : s) res = res + disp(i);
        return res + "]";
    }

    // contains point?
    bool contains(Utils::pointSet s, Utils::point p) { 
        return (s.find(p) != s.end());
    }

    template <typename T>
    bool contains(std::set<T> s, T p) { 
        return (s.find(p) != s.end());
    }

    // Does an AABB contain a point?
    bool contains(AABB ab, Utils::point point) {
        if (point.first < ab.first.first || point.first > ab.second.first) return false;
        if (point.second < ab.first.second || point.second > ab.second.second) return false;
        return true;
    }

    // Using a std::map<T, int> as a frequency table
    template <typename T>
    void push(std::map<T, int>& m, T t) {
        if (m.find(t) == m.end()) m.insert({t, 0});
        (m.find(t))->second++;
    }
    template <typename T>
    void pop(std::map<T, int>& m, T t) {
        if (m.find(t) != m.end()) return;
        (m.find(t))->second--;
        if (m.at(t) <= 0) m.erase(m.find(t));
    }

    // Using a std::map<K, std::set<V>> as a classifier
    template <typename K, typename V>
    void append(std::map<K, std::set<V>>& m, K k, V v) {
        if (m.find(k) == m.end()) m.insert({k, std::set<V>()});
        (m.find(k))->second.insert(v);
    }

    /*
    
    Actions on points and AABBs
    
    */

    std::string to_string(point p) {
        return "(" + std::to_string(p.first) + ", " + std::to_string(p.second) + ")";
    }

    point add(point a, point b) {
        return {a.first + b.first, a.second + b.second};
    }

    point inv(point x) {
        return {-x.first, -x.second};
    }

    point sub(point a, point b) {
        return add(a, inv(b));
    }

    // Rotate counterclockwise by 90 degrees
    point rot(point a) {
        return {-a.second, a.first};
    }

    // sorts the components of p in increasing order
    point makeInterval(point p) {
        if (p.first <= p.second) return p;
        return {p.second, p.first};
    }

    // Sorts the opposing corners of a bounding box to be {BOTLEFT, TOPRIGHT}
    AABB resetBB(AABB boundingbox) {
        point BL = {std::min(boundingbox.first.first, boundingbox.second.first), std::min(boundingbox.first.second, boundingbox.second.second)};
        point TR = {std::max(boundingbox.first.first, boundingbox.second.first), std::max(boundingbox.first.second, boundingbox.second.second)};
        return {BL, TR};
    }

    // Make a bounding box from two intervals
    AABB makeAABB(point xx, point yy) {
        xx = makeInterval(xx);
        yy = makeInterval(yy);
        return {{xx.first, yy.first}, {xx.second, yy.second}};
    }

    // Dimensions of a bounding box. A bounding box is closed on its sides.
    point dims(AABB x) {
        return {std::abs(x.second.first - x.first.first) + 1, std::abs(x.second.second - x.first.second) + 1};
    }

    // Return the x and y intervals of the AABB
    point getX(AABB ab) {
        point p = {ab.first.first, ab.second.first};
        return makeInterval(p);
    }

    point getY(AABB ab) {
        point p = {ab.first.second, ab.second.second};
        return makeInterval(p);
    }

    // Intersection of two intervals, where an interval is a Util::point
    point intervalIntersect(point a, point b) {
        if (a.first > b.first) return intervalIntersect(b, a);

        if (b.first > a.second) return NILPT;
        return makeInterval({std::max(a.first, b.first), std::min(a.second, b.second)});
    }

    // Intersection of two bounding boxes
    AABB intersection(AABB a, AABB b) {
        point xi = intervalIntersect(getX(a), getX(b));
        point yi = intervalIntersect(getY(a), getY(b));
        if (xi == NILPT || yi == NILPT) return NILBB;

        return makeAABB(xi, yi);
    }

    // Get the set of all points in the BB
    pointSet getAllPoints(AABB ab) {
        ab = resetBB(ab);
        pointSet res;
        for (int x = getX(ab).first; x <= getX(ab).second; x++) {
            for (int y = getY(ab).first; y <= getY(ab).second; y++) res.insert({x, y});
        }
        return res;
    }
}

// NO LONGER IN NAMESPACE UTILS

    // Get only the cells with non-path symbols 
    Utils::pointSet getCells(Utils::pointSet s) {
        Utils::pointSet res;
        for (auto i : s) {
            if (i.first & 1 && i.second & 1) res.insert(i);
        }
        return res;
    }

    // Get only the possible path blocks
    Utils::pointSet getEdges(Utils::pointSet s) {
        Utils::pointSet res;
        for (auto i : s) {
            if (!(i.first & 1 && i.second & 1)) res.insert(i);
        }
        return res;
    }

    // Get the neighbors of a point - These might be in weird places in some e.g. pillar grids.
    Utils::pointSet neighbors(std::pair<int, int> p) {
        Utils::pointSet res;

        for (int d = 0; d < 4; d++) {
            Utils::point next = {p.first + Utils::dx[d], p.second + Utils::dy[d]};
            res.insert(next);
        }
        return res;
    }

template <typename AGAINST, typename TEST>
bool instanceof(const TEST* test) {
    return dynamic_cast<const AGAINST*>(test) != nullptr;
}

template <typename T>
std::set<T> intersection(const std::set<T>& a, const std::set<T>& b) {
    if (a.size() > b.size()) return intersection(b, a);
    std::set<T> res;
    for (auto i : a) {
        if (b.find(i) != b.end()) res.insert(i);
    }
    return res;
}

Utils::pointSet intersection(const Utils::pointSet& a, const Utils::pointSet& b) {
    return intersection<Utils::point>(a, b);
}

int clamp(int x, int L, int H) {
    if (x < L) return L;
    if (x > H) return H;
    return x;
}

// Integer to base N <= 10 string

std::string to_string(uint64_t n, int base) {
    if (n == 0) return "0";

    if (base >= 10) return std::to_string(n);
    if (base < 2) return std::to_string(n);

    std::string res;
    while (n) {
        char d = '0' + (n % base);
        res.insert(res.begin(), d);
        n /= base;
    }
    return res;
}

// Count chars in a string
int count_chars(std::string s, char c) {
    int res = 0;
    for (auto i : s) {
        if (i == c) res++;
    }
    return res;
}

int64_t rem(int64_t a, int64_t m) {
    a %= m;
    while (a < 0) a += m;
    return a % m;
}
// Modular exponentiation
int64_t modexp(int64_t b, int64_t e, int64_t m = 1000000007) {
    if (e <= 0) return 1;
    if (e & 1) return rem(b * modexp(b, e - 1, m), m);
    int64_t sqxx = modexp(b, e>>1, m);
    return rem(sqxx * sqxx, m);
}
#endif