#ifndef PUZENT_WITCLN
#define PUZENT_WITCLN

#include <climits>
#include <cstdint>
#include <string>
#include <vector>

namespace EntityColor {

enum Color { // Color is an enum to prevent exploiting loopholes (e.g. 2 similar colors considered different)
    NIL = INT_MAX, // Empty color
    
    RGB_RED = 0xFF0000,
    RGB_GREEN = 0x00FF00,
    RGB_BLUE = 0x0000FF,
    RGB_YELLOW = 0xFFFF00,
    RGB_CYAN = 0x00FFFF,
    RGB_MAGENTA = 0xFF00FF,
    RGB_BLACK = 0,
    RGB_WHITE = 0xFFFFFF,
    
    RGB_ORANGE = 0xFF8000,
    RGB_GREY = 0x808080,
    RGB_DARK = 0x404040,
    RGB_LIGHT = 0x0C0C0C,
    
    // Furries!
    
    SP2 = 0xCEB7FF,
    SP1 = 0x8AD8FF
};

};



class PuzzleEntity {
    public:
    EntityColor::Color color = EntityColor::NIL;
    bool isPath = false; // Is Pathable (false if cut or is symbol cell)
    uint8_t hasLine = false; // Is there a path here
    bool isBlocker = false; 
    // Blocks BFS searches and the placement of certain objects into the position -- acts like a hole in the grid. Ideally if this option is set then isPath should also be false.
    std::string type = "PuzzleEntity"; // Naming convention: The "type" of an object is its class name.
    std::string disp = "OBJECT"; // Naming convention: This string must be 6 chars long.

    bool isActive = true; // Does this symbol affect the results?

    void init() {
    }

    PuzzleEntity() {
        init(); // Call your init() LAST.
    }

    PuzzleEntity(const PuzzleEntity& other) {
        color = other.color;
        isPath = other.isPath;
        isBlocker = other.isBlocker;
        type = other.type;
        disp = other.disp;
        isActive = other.isActive;
        init();
    }

    PuzzleEntity clear() {
        return PuzzleEntity();
    }

    virtual ~PuzzleEntity() {
    }

    std::vector<uint8_t> getregs() {
        return std::vector<uint8_t>({isPath, hasLine, isBlocker});
    }

    void setregs(std::vector<uint8_t> v) {
        if (v.size()) isPath = v[0];
        if (v.size() > 1) hasLine = v[1];
        if (v.size() > 2) isBlocker = v[2];
    }

    void setregs(uint8_t v[3]) {
        isPath = v[0];
        hasLine = v[1];
        isBlocker = v[2];
    }
};

class Endpoint : public PuzzleEntity {
    public:
    bool isStart = true;

    void init() {
        type = "Endpoint";
        disp = "ENDPT_";
        if (isStart) disp = "START_";
    }

    Endpoint() : PuzzleEntity() {
        isPath = true;
        init();
    }

    Endpoint(bool s) : PuzzleEntity() {
        isPath = true;
        isStart = s;
        init();
    }

    Endpoint(const Endpoint& other) : PuzzleEntity(other) {
        isStart = other.isStart;
        init();
    }
};

class PathDot : public PuzzleEntity {
    public:
    uint8_t restriction = 0; // If restriction != 0 then it only passes if a line of index k passes over it, such that the (1<<(k - 1)) bit is set.

    void init() {
        type = "PathDot";
        disp = "PDOT" + std::to_string(restriction);
        while (disp.size() < 6) disp = disp + "_";
    }

    PathDot() : PuzzleEntity() {
        isPath = true;
        init();
    }

    PathDot(uint8_t res) : PuzzleEntity() {
        isPath = true;
        restriction = res;
        init();
    }

    PathDot(const PathDot& other) : PuzzleEntity(other) {
        restriction = other.restriction;
        init();
    }
};

class ColorEntity : public PuzzleEntity {
    public:

    void init() {
        type = "ColoredEntity";
        disp = "COLORX";
    }

    ColorEntity() : PuzzleEntity() {
        color = EntityColor::RGB_BLACK;
        init();
    }

    ColorEntity(EntityColor::Color c) : PuzzleEntity() {
        color = c;
        init();
    }

    ColorEntity(const ColorEntity& other) : PuzzleEntity(other) {
        color = other.color;
        init();
    }
};

class Blob : public ColorEntity {
    public:

    void init() {
        type = "Blob";
        disp = " BLOB ";
    }

    Blob() : ColorEntity() {
        init();
    }

    Blob(EntityColor::Color c) : ColorEntity(c) {
        init();
    }

    Blob(const Blob& other) : ColorEntity(other) {
        init();
    }
};

class Star : public ColorEntity {
    public:

    void init() {
        type = "Star";
        disp = " STAR ";
    }

    Star() : ColorEntity() {
        init();
    }

    Star(EntityColor::Color c) : ColorEntity(c) {
        init();
    }

    Star(const Star& other) : ColorEntity(other) {
        init();
    }
};

class Triangle : public ColorEntity {
    public:
    int count = 1;

    void init() {
        type = "Triangle";
        disp = " TRIX" + std::to_string(count);
    }

    Triangle() : ColorEntity() {
        color = EntityColor::RGB_ORANGE;
        init();
    }

    Triangle(int c) : ColorEntity() {
        color = EntityColor::RGB_ORANGE;
        count = c;
        init();
    }

    Triangle(EntityColor::Color c) : ColorEntity(c) {
        color = c;
        init();
    }

    Triangle(int x, EntityColor::Color c) : ColorEntity(c) {
        count = x;
        init();
    }

    Triangle(const Triangle& other) : ColorEntity(other) {
        color = other.color;
        count = other.count;
        init();
    }
};

class Cancel : public ColorEntity {
    public:

    void init() {
        type = "Cancel";
        disp = "CANCEL";
    }

    Cancel() : ColorEntity() {
        color = EntityColor::RGB_WHITE;
        init();
    }

    Cancel(EntityColor::Color c) : ColorEntity(c) {
        color = c;
        init();
    }

    Cancel(const Cancel& other) : ColorEntity(other) {
        color = other.color;
        init();
    }
};

#endif