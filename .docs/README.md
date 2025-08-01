# .docs

Rudimentary documentation for WITNESS_CLONE_II. Use responsibly and at your own risk.  
Documentation is sorted into the same directory structure, except each file is now obviously a documentation file. Classes are denoted by headings.  

## NOTES

- Some `.md` files have multiple classes.

## DEFINITIONS

- Enums distinguish subclasses, typecasting reveals their true form.
- Most classes have copy constructors that take in instances of that class.
- A lot of classes have a `std::string to_string()` function that returns a readable string representation of the class instance.
- NIL values (e.g. `NILPT`) exist for various datatypes, usually consisted of `NaN` float values. Those are usually returned from functions in degenerate or extraneous cases.
- Although the grids use `(R, C)` to denote coordinates, they should be thought of as `(x, y)` coordinates. That means `+x` is right, and `+y` is up. When printing or displaying a grid, the highest `y` values should show up at the top.