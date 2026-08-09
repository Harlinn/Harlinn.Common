# Harlinn.Common

Harlinn.Common is a header only library which only depends on a 
few standard C++ header files:

- `<array>`
- `<bit>`
- `<cassert>`
- `<chrono>`
- `<cmath>`
- `<cwctype>`
- `<iterator>`
- `<limits>`
- `<optional>`
- `<ranges>`
- `<string_view>`
- `<type_traits>`

The library provides functionality used by the Harlinn.Math library and the 
Harlinn.System library.

## Build

To build:

```
cmake --preset default
cmake --build --preset default
ctest --preset default
```

For local development with sibling repos:

```
cmake --preset local-dev
```

