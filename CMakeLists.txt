cmake_minimum_required(VERSION 2.8)
project(nastranfind)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_INCLUDE_CURRENT_DIR_IN_INTERFACE ON)


set(MY_SOURCES
    ./src/global.h
    ./src/stringhelper.h
    ./src/application.h
    ./src/application.cpp
    ./src/engine.h
    ./src/engine.cpp
    ./src/recentfile.h
    ./src/recentfile.cpp
    ./src/main.cpp
    )

add_executable(nastranfind WIN32 ${MY_SOURCES})

if(WIN32)
    # =========================================================
    # PDCurses - Public Domain Curses
    # http://www.projectpluto.com/win32a.htm
    # (implementation of Curses for Windows)
    # =========================================================
    # searching for include directory
    find_path(PDCURSES_INCLUDE_DIR curses.h PATHS ./pdcurses/include)

    # searching for library file
    find_library(PDCURSES_LIBRARY pdcurses PATHS ./pdcurses/lib)

    # searching for dll file
    find_file(PDCURSES_DLL pdcurses.dll PATHS ./pdcurses/bin/)
    message(${PDCURSES_DLL})
    if (PDCURSES_INCLUDE_DIR AND PDCURSES_LIBRARY AND PDCURSES_DLL)
        # you may need that if further action in your CMakeLists.txt depends
        # on detecting your library
        set(PDCURSES_FOUND TRUE)

        # you may need that if you want to conditionally compile some parts
        # of your code depending on library availability
        add_definitions(-DHAVE_LIBPDCURSES=1)

        #When you build the library as a Windows DLL, you must always define
        #PDC_DLL_BUILD when linking against it. (Or, if you only want to use
        #the DLL, you could add this definition to your curses.h.)
        add_definitions(-DPDC_DLL_BUILD=1)

        # those two, you really need
        include_directories(${PDCURSES_INCLUDE_DIR})
        set(YOUR_LIBRARIES ${YOUR_LIBRARIES} ${PDCURSES_LIBRARY})

        # Copy *.DLL
        install(FILES ${PDCURSES_DLL}
                DESTINATION ${CMAKE_BINARY_DIR})

    else()
        message(FATAL_ERROR "Package Curses required, but not found!")
    endif ()

elseif(UNIX)
    # =========================================================
    # NCurses Library
    # include..: /usr/include/curses.h
    # lib......: /usr/lib/i386-linux-gnu/libcurses.a
    # =========================================================
    find_package(Curses REQUIRED) 

    set(YOUR_LIBRARIES ${YOUR_LIBRARIES} ${CURSES_LIBRARIES})
    include_directories(${CURSES_INCLUDE_DIR})

endif()

target_link_libraries(nastranfind ${YOUR_LIBRARIES})

