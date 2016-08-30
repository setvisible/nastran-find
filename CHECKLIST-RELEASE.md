# Checklist for Release

### Testing

 - Make sure nastranfind works
 - Run the auto tests
 - Run the manual & performance tests
 - Ensure they all passed


### Pre-Building

 - Change `NF_VERSION` in `version.pri`
 - Change `MAJOR.MINOR.PATCH` in `CMakeLists.txt`
 - List the changes with `git log --pretty=oneline` and added them to `CHANGELOG`
 - Make sure `CHANGELOG` has a proper date set
 - Commit (use **Git GUI**)
 - Tag the commit with a new version number: `git tag -a vXXX -m "NastranFind version XXX"` where XXX is the version
 - Push with `git push origin vXXX`
 - Verify with `git show vXXX`


### Building the targets

- Build the 32 bits target as 'Release'
- Build the 64 bits target as 'Release'

### Post-Building

#### [Unix only]

- Check the version in both targets with `$ nastranfind --version`
- Tar the target with:
 * the GLIDER examples

--> Target name : `nastranfind-vXXXX-unix32.zip` where XXX is the version


#### [Windows only]

- Check the version in both targets  with `> nastranfind.exe --version`
- Zip the target with:
 * the GLIDER examples
 * the PDCurses dll
 * the Std C lib dll

--> Target name : `nastranfind-vXXXX-win32.zip` where XXX is the version


### Releasing on Github.io

- Go to the [releases](https://github.com/setvisible/nastran-find/releases) page
- Edit the tag XXX
- Upload the targets
- Publish as Pre-Release and write the announce:
 - Title: "NastranFind version XXX"
 - Description: the list of changes


That's all folks

