# Harlinn.Common

[Harlinn.Common](https://github.com/Harlinn/Harlinn.Common) is a header-only 
support library for [Harlinn.Math](https://github.com/Harlinn/Harlinn.Math) 
and [Harlinn.System](https://github.com/Harlinn/Harlinn.System). It provides 
shared functionality that depends only on the standard C and C++ libraries, 
not on operating-system-specific APIs.

## Project status

These libraries are in their early stages. Harlinn.Common currently compiles for 
Windows, Linux, and macOS. Harlinn.Math currently builds only on Windows, and work 
on Harlinn.System has not yet started.

Harlinn.System is intended to provide access to functionality that must be 
implemented differently across operating systems. The initial goal is to 
support the latest Windows, Linux, and macOS versions available on GitHub-hosted runners.

Harlinn.Math, Harlinn.System, and Harlinn.Common are based on code first 
published as part of the Windows-only [Harlinn.Windows](https://github.com/Harlinn/Harlinn.Windows) 
repository. The libraries are being developed to separate shared, standard-library-only 
functionality from math and platform-specific system functionality.

## Build

Configure, build, and run the tests with the default C++23 preset:

```sh
cmake --preset default
cmake --build --preset default
ctest --preset default
```

The `default`, `release`, and `cxx26` configure presets are available on Windows, Linux, and macOS.