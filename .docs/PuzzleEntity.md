# PuzzleEntity/EntityColor

`namespace EntityColor`

## Description

Enum of colors

## Notes

- The colors store their hex codes.

## Static Instances and Constants

- `enum Color:`
  - `NIL = INT_MAX` - Empty color
    
  - `RGB_RED = 0xFF0000`
  - `RGB_GREEN = 0x00FF00`
  - `RGB_BLUE = 0x0000FF`
  - `RGB_YELLOW = 0xFFFF00`
  - `RGB_CYAN = 0x00FFFF`
  - `RGB_MAGENTA = 0xFF00FF`
  - `RGB_BLACK = 0`
  - `RGB_WHITE = 0xFFFFFF`
  - `RGB_ORANGE = 0xFF8000`
  - `RGB_GREY = 0x808080`
  - `RGB_DARK = 0x404040`
  - `RGB_LIGHT = 0x0C0C0C`
  - `SP2 = 0xCEB7FF`
  - `SP1 = 0x8AD8FF`

---

# PuzzleEntity

`class PuzzleEntity`

## Description

This is what comprises a grid. All points in a grid, including the actual grid cells and the edges, are PuzzleEntities.

## Notes

- Again, everything in a Grid is a `PuzzleEntity`.

## Fields

- `color: EntityColor::Color = EntityColor::NIL` - Color of the object
- `isPath, isBlocker: bool = {false, false}` - Can we draw a path, does the cell block floodfills?
- `hasLine: uint8_t = 0` - Does the cell have a path already drawn? (That's the boolean value -- the integer value is the index of the line)
- `type: std::string = "PuzzleEntity"` - The class name as a string
- `disp: std::string = "OBJECT"` - A six letter symbol representing the object. This can change based on internal properties of the object but must be consistent.
- `isActive" bool = true` - Is the symbol active in the grid? (Used for things like cancels)

## Constructors

- `PuzzleEntity() {init()}`

## Functions

- `void init()` - This should be called at the END of each constructor
- `PuzzleEntity clear()` - Returns an empty PuzzleEntity
- `std::vector<uint8_t> getregs()` - Gets `isPath, hasLine, isBlocker`
- `void setregs(std::vector<uint8_t> v)` - Sets the registers in that order

---

# PuzzleEntity/ColorEntity

`class ColorEntity : public PuzzleEntity`

## Description

Entities that can have color.

## Constructors

- `ColorEntity()` - Default color is `RGB_BLACK`.
- `ColorEntity(EntityColor::Color c)`

## Functions

- `void init()` - `{type, disp} = {"ColorEntity", "COLORX"}`

---

# PuzzleEntity/Endpoint

`class Endpoint : public PuzzleEntity`

## Description

Starting and ending points

## Fields

- `isStart: bool = true` - Is the point a starting point else an ending point?

## Constructors

- `Endpoint()`
- `Endpoint(bool s)` - Sets `isStart` to `s`

## Functions

- `void init()` - `{type, disp} = {"Endpoint", "START_" if isStart else "ENDPT_"}`

---

# PuzzleEntity/PathDot

`class PathDot : public PuzzleEntity`

## Description

Path dots

## Fields

- `restriction: int8_t = 0` - Restrictions for specific lines in symmetry puzzles

## Constructors

- `PathDot()`

## Functions

- `void init()` - `{type, disp} = {"PathDot", "PATHDT"}`

---

# PuzzleEntity/PathDot

`class PathDot : public PuzzleEntity`

## Description

Path dots

## Fields

- `isStart: bool = true` - Is the point a starting point else an ending point?

## Constructors

- `PathDot()`

## Functions

- `void init()` - `{type, disp} = {"PathDot", "PATHDT"}`

---

# PuzzleEntity/Blob

`class Blob : public ColorEntity`

## Description

Blobs that must be separated by color. 

## Constructors

- `Blob()`
- `Blob(EntityColor::Color c)`

## Functions

- `void init()` - `{type, disp} = {"Blob", " BLOB "}`

---

# PuzzleEntity/Star

`class Star : public ColorEntity`

## Description

Stars that must be paired with exactly one other of its color.

## Constructors

- `Star()`
- `Star(EntityColor::Color c)`

## Functions

- `void init()` - `{type, disp} = {"Star", " STAR "}`

---

# PuzzleEntity/Triangle

`class Triangle : public ColorEntity`

## Description

Triangles that impose constraints on the line.

## Fields

- `count: int` - Number of triangles in the symbol

## Constructors

- `Triangle()`
- `Triangle(EntityColor::Color c)`
- `Triangle(int x)`
- `Triangle(int x, EntityColor::Color c)`

## Functions

- `void init()` - `{type, disp} = {"Triangle", " TRIX" + std::to_string(count)}`

---

# PuzzleEntity/Cancel

`class Cancel : public ColorEntity`

## Description

Cancels that negate symbols in their region.

## Constructors

- `Cancel()`
- `Cancel(EntityColor::Color c)`

## Functions

- `void init()` - `{type, disp} = {"Cancel", "CANCEL"}`