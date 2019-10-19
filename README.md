# Minimal Example to Demonstrate CMake AUTOMOC Problem

When setting the cache variable `CMAKE_AUTOMOC` to `ON` the build will fail on `WIN32` because `moc` is run
twice on the `mainwindow.h` header. Once for the `mywidgets` library (as it should), and a second time for
the executable that links against the library, which it shouldn't do. When linking the executable this results
in multiple definitions of the moc'ed code with inconsistent linkage:

* once as `__declspec(dllexport)` in the library case
* once as implicit `__declspec(dllimport)` in the executable case.

## Solution

The problem was that the header `mainwindow.h` header was declared as `PUBLIC` in `CMakeLists.txt`. As such
it will populate the `INTERFACE_SOURCES` property and will be considered as a source for the consuming
(i.e. linking) targets. Hence, CMake did what it was supposed to do, and compiled the file twice.

**Consequence**: RTFM and really know what `PUBLIC` and `INTERFACE` do... 
