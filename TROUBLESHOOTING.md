TROUBLESHOOTING
===============

## [BUG001][RESOLVED] Application cannot be resized in PDCurses

### Problem

By default, PDCurses sets the application in a fixed-size window.

### Solution

Add the following variables,  before calling `initscr( )` or `Xinitscr( )`:

    ttytype[0] = 25;   /* Allow 25 to 90 lines... */
    ttytype[1] = 90;
    ttytype[2] = 80;   /* ...and 80 to 200 columns */
    ttytype[3] = (unsigned char)200;
    // ...
    initscr();

Then, the settings of the application that is stored in the register,
under the following path :

    HKEY_CURRENT_USER\SOFTWARE\PDCurses\%name_of_the_app%

changes from :

    myapp = "80x25,12,-1098,182,1;25,25,80,80:Courier New"

to
    myapp = "80x25,12,-1098,182,1;25,90,80,200:Courier New"

The application is now resizable.


## [BUG002] PDCurses bad settings storage (on Windows)

### Problem

On Windows, at close event, the position of the application's window is
stored in the register:

    HKEY_CURRENT_USER\SOFTWARE\PDCurses\%name_of_the_app%

However the position can be outside the screen limits (ex: temporary screen).
The application doesn't appear but runs.

Proposition for the correction: --resetConfig argument to delete the HKEY.

The work-around is to delete this key *manually*.

### Solution

\todo

## [BUG003] PDCurses tabulation character  (tab char /t)

### Problem

Wrong position of occurrence textboxes when the line contains tab char.

### Solution

\todo

## [BUG004] GCC regex fails on Linux

### Problem

Regex doesn't as expected with GCC 4.9 :

    throw_regex_error(regex_constants::error_escape)

### Solution

\todo

## [BUG005][RESOLVED]  Qt and NCurses

### Problem

When running the code with C++ and NCurses in QtCreator:

    #include <ncurses.h>
    int main()
    {
      initscr();
      printw("Hello world!");
      refresh();
      getch();
      endwin();
    }

It says: 

	Error opening terminal:unknown


### Solution

In QtCreator:

Go to **Project** -> **Run settings** -> **Environment**

1. Make sure "Run in Terminal" is checked.

2. Add the `TERM` variable with `xterm` as value.


## [BUG006][RESOLVED]  Curses and keys detection

### Problem

\todo

Under Windows:
 - when the programme is MinGW32-compiled from QtCreator (with QMake),
 the keyboard's arrow keys are detected and we can navigate.
 - when the programme is MinGW32-compiled from CMake, neither
 the arrow keys nor the window's resizing aren't detected.

Under Unix:
 - no problem

rem: see maybe the options of CMake?

### Solution

\todo


