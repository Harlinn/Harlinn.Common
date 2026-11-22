# Harlinn.Common

Harlinn.Common is a header only library which only depends on a 
few standard C++ header files:

- `<array>`
- `<bit>`
- `<cassert>`
- `<cctype>`
- `<chrono>`
- `<climits>`
- `<cmath>`
- `<concepts>`
- `<cstddef>`
- `<cstdint>`
- `<cstdlib>`
- `<cstring>`
- `<cwchar>`
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

The `default`, `release`, and `cxx26` configure presets are available on all supported platforms.