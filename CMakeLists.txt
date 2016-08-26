project(nastranfind)

set(NF_VERSION "1.0.0")
set(PACKAGE_VERSION_MAJOR "1")
set(PACKAGE_VERSION_MINOR "0")
set(PACKAGE_VERSION_PATCH "0")
set(PACKAGE_VERSION "${PACKAGE_VERSION_MAJOR}.${PACKAGE_VERSION_MINOR}.${PACKAGE_VERSION_PATCH}")

cmake_minimum_required(VERSION 2.6.0 FATAL_ERROR)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING
            "Choose the type of build, options are: Debug Release RelWithDebInfo MinSizeRel."
            FORCE)
endif(NOT CMAKE_BUILD_TYPE)

set(MY_SOURCES
    ./icons/icon.rc
    ./src/global.h
    ./src/application.h
    ./src/application.cpp
    ./src/engine.h
    ./src/engine.cpp
    ./src/fileinfo.h
    ./src/fileinfo.cpp
    ./src/recentfile.h
    ./src/recentfile.cpp
    ./src/stringhelper.h
    ./src/stringhelper.cpp
    ./src/systemdetection.h
    ./src/version.h
    ./src/main.cpp
    )

add_executable(nastranfind WIN32 ${MY_SOURCES})


### Windows shell lightweight utility functions
if(WIN32)
    ## Remark: MinGw32 uses its own 'shlwapi.h'. Do not link to the C:\Windows\System one.
    ## So, don't use the following lines:
    #find_library(SHLWAPI_LIBRARY Shlwapi)
    #set(YOUR_LIBRARIES ${YOUR_LIBRARIES} ${SHLWAPI_LIBRARY})
    ## ...but instead use this one:
    set(YOUR_LIBRARIES ${YOUR_LIBRARIES} "-lShlwapi")
else()
    set(SHLWAPI)
endif()


### Curses
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
    message(STATUS "PDCurses DLL found in: " ${PDCURSES_DLL})
    if(PDCURSES_INCLUDE_DIR AND PDCURSES_LIBRARY AND PDCURSES_DLL)
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
    endif()

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

