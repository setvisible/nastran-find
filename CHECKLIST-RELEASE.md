# Checklist for Release

### Testing

 - Make sure NastranFind works
 - Run auto tests
 - Run manual & performance tests
 - Ensure they all passed


### Pre-Building

 - Change `NF_VERSION` in `version.pri`
 - Change `MAJOR.MINOR.PATCH` in `CMakeLists.txt`
 - List the changes since the last tag with `git log --pretty=oneline --decorate=short` and add them to `CHANGELOG`
 - Reorganize the comments by alphabetic order
 - Remove non relevant comments
 - Auto-correct the comments with LibreOffice's Writer
 - Make sure `CHANGELOG` has a proper date set


### Building the targets

#### [Windows only]

 - Create a build repertory (apart from the source repertory).
 - Configure & Generate with CMake-GUI:
  * select "MinGW Makefiles"
  * select an install directory (CMAKE_INSTALL_PREFIX)
  * select "Release" (CMAKE_BUILD_TYPE)
  * select the MinGW 32bits compiler
 - Go to the build repertory
 - Run `make install`
 - Go to the install repertory and check that it contains:
  * the GLIDER examples
  * the PDCurses DLL
  * the STD library DLLs:
    o libgcc_s_dw2-1.dll
    o libstdc++-6.dll
    o libwinpthread-1.dll
 - Run `nastranfind.exe -v` to check the version number
 - Zip the install directory as `NastranFind-X.X.X-Windows_MinGW_32bit.zip` where X.X.X is the version

#### [Ubuntu, Debian]

 - same thing as Windows, but:
  * select "Unix Makefiles"
  * select "nastranfind" as install directory (CMAKE_INSTALL_PREFIX)
 - Go to the install repertory
 - Run `./nastranfind -v` to check the version number
 - Go above the install dir
 - Run `tar czvf NastranFind-X.X.X-Unix_MinGW_32bit.tar.gz nastranfind/` where X.X.X is the version
  * Check that the tar archive contains the directory, not the files.

### Post-Building

 - Commit (use **Git GUI**)
 - Tag the commit with a new version number: `git tag -a vX.X.X -m "NastranFind version X.X.X"` where X.X.X is the version
 - Push with `git push origin vX.X.X`
 - Verify with `git show vX.X.X`


### Releasing on Github.io

- Go to the [releases](https://github.com/setvisible/nastran-find/releases) page
- Edit the tag XXX
- Upload the targets
- Publish as Pre-Release and write the announce:
 * Title: "NastranFind version XXX"
 * Description: the list of changes


That's all folks

