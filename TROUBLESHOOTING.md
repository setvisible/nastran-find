TROUBLESHOOTING
===============

## [RESOLVED] Application cannot be resized in PDCurses (BUG001)

By default, PDCurses sets the application in a fixed-size window.

**SOLUTION**

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


### [BUG002] PDCurses bad settings storage (on Windows)

On Windows, at close event, the position of the application's window is
stored in the register:

    HKEY_CURRENT_USER\SOFTWARE\PDCurses\%name_of_the_app%

However the position can be outside the screen limits (ex: temporary screen).
The application doesn't appear but runs.

Proposition for the correction: --resetConfig argument to delete the HKEY.

The work-around is to delete this key *manually*.


### [BUG003] PDCurses tabulation character  (tab char /t)

Wrong position of occurence textboxes when the line contains tab char.




