# NastranFind

## Changelog

### Version 1.1.0 build Jan 01 2017

 - fixed some global performance bottlenecks:
   * Some `std::string` are now passed by reference, instead of value:
     the code is more brittle but that avoids a lot of useless string copies.
   * `std::stringstream` used instead of `std::filestream`:
     Now the file is entirely memory-mapped before performing intensive text searches.
 - fixed `make install`, added deployment files
 - fixed build chain with CMake (version string not correctly passed)
 - added unit test (single quote in double-quotes statement)
 - added `CHANGELOG.md` and `CHECKLIST-RELEASE.md`
 - updated `CMakeLists.txt` for Windows
 - updated `README.md`


### Version 1.0.4 build Aug 26 2016

 - fixed build issues with debian 8.0
 - added *.DAT example
 - updated `README.md`


### Version 1.0.2 build Aug 18 2016

 - improved performances for large bulk files
 - updated recent files strategy
 - fixed `realpath()` warning
 - fixed config file


### Version 1.0.0 build Aug 15 2016

 - Initial release + minor corrections:
 - fixed build chain with CMake (version string not correctly passed)
 - fixed line endings (normalized)
 - added 3rd-party source `systemdetection.h` from Qt
 - updated 'Include' parsing (regex removed, as it's not stable on Unix)
 - updated `README.md`

