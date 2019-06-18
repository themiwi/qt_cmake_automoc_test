# Minimal Example to Demonstrate CMake AUTOMOC Problem

When setting the cache variable `CMAKE_AUTOMOC` to `ON` the build will fail on `WIN32` because `moc` is run
twice on the `mainwindow.h` header. Once for the `mywidgets` library (as it should), and a second time for
the executable that links against the library, which it shouldn't do. When linking the executable this results
in multiple definitions of the moc'ed code with inconsistent linkage:

* once as `__declspec(dllexport)` in the library case
* once as implicit `__declspec(dllimport)` in the executable case.
