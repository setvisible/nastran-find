# Nastranfind

Nastranfind is a solution for browsing NASTRAN input deck files for Windows, Mac OS X and Linux. 

Its core is developed in C++, based on the **Curses** library. 

## Screenshots

Main window, looking for `123456` in the toyota yaris model. It found 8447 occurences :

![](screenshots/find_yaris.png)


## Dependencies

### On Linux / Mac OS X

Install NCurses:

    sudo apt-get update
	sudo apt-get install libncurses5-dev
	whereis ncurses
    > ncurses: /usr/include/ncurses.h

[http://www.gnu.org/software/ncurses/ncurses.html](http://www.gnu.org/software/ncurses/ncurses.html "http://www.gnu.org/software/ncurses/ncurses.html")


### On Windows

Install PDCurses:

 - Win9x/Me/NT/2k/XP/Vista --> build the directory `win32`
 - Win7/Win8/Win10 --> build the directory `win32a`

[http://pdcurses.sourceforge.net/](http://pdcurses.sourceforge.net/)


## Compilation

First download `nastranfind-src.tar.gz`


### Build with Qt

Compile and run `nastranfind.pro`.


### Build with CMake

On Linux / Mac OS X 

    tar zxvf nastranfind-src.tar.gz
    cd nastranfind-src
    mkdir -p build
    cd build
    cmake ..
    make

On Windows

    tar zxvf nastranfind-src.tar.gz
    cd nastranfind-src
    mkdir -p build
    cd build
    "C:\Program Files\CMake\bin\cmake.exe" ..
    start .
    REM Double click the Visual Studio project


## Usage

On Linux / Mac OS X 

        ./nastranfind MyFile.bdf

On Windows

        nastranfind.exe MyFile.bdf

__Commands:__

 - Press `F` to find a word
 - Press `A` `S` `Z` `X` or the keypad to browse the results
 - Press `Q` to quit

## License

The code is released under the GNU **LGPLv3** open source license. 

[https://www.gnu.org/licenses/lgpl-3.0.en.html](https://www.gnu.org/licenses/lgpl-3.0.en.html)
