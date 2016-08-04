TROUBLESHOOTING
===============

### [BUG0001] Debug mode with PDCurses (on Windows)

When compiling with MinGW 5.3.0 32bit under Windows 10,
the window of the application is not resizable when in DEBUG build,
but in RELEASE build, it's rezisable.
 
Maybe a conflict of compilation variables ('Debug'?) in the PDCurses lib ?



