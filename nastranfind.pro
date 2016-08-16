include(nastranfind.pri)

#-------------------------------------------------
# NASTRANFIND
#-------------------------------------------------
TEMPLATE = app
TARGET = nastranfind

# Remove from CONFIG all the Qt dependencies and unnecessary stuff
CONFIG -= lex yacc
CONFIG -= qt qpa link_prl qml_debug precompile_header
CONFIG -= testcase_targets
CONFIG -= import_qpa_plugin import_plugins
CONFIG -= rtti rtti_off
CONFIG -= exceptions
CONFIG -= shared
CONFIG -= no_plugin_manifest
CONFIG -= file_copies copy_dir_files
CONFIG -= incremental_off
CONFIG -= depend_includepath
CONFIG -= windows # BUG: 'windows' prevents std::cout to write in the console.
CONFIG += c++11

#message($${CONFIG})

LANGUAGE = C++

lessThan(QT_VERSION, 5.0) {
    warning("prefere to build it with Qt 5.0")
}

#-------------------------------------------------
# TARGET DIRECTORY
#-------------------------------------------------
# Needs to define a subfolder, as the *.DLL
# cannot be copied in the root folder.
DESTDIR = build

build_pass:CONFIG(debug, debug|release) {
    unix: TARGET = $$join(TARGET,,,_debug)
    else: TARGET = $$join(TARGET,,,d)
}


#-------------------------------------------------
# SOURCES
#-------------------------------------------------
HEADERS  += \
    $$PWD/src/global.h \
    $$PWD/src/application.h \
    $$PWD/src/engine.h \
    $$PWD/src/fileinfo.h \
    $$PWD/src/recentfile.h \
    $$PWD/src/stringhelper.h \
    $$PWD/src/systemdetection.h \
    $$PWD/src/version.h

SOURCES += \
    $$PWD/src/main.cpp\
    $$PWD/src/application.cpp \
    $$PWD/src/engine.cpp \
    $$PWD/src/fileinfo.cpp \
    $$PWD/src/recentfile.cpp \
    $$PWD/src/stringhelper.cpp

OTHER_FILES += \
    $$PWD/README.md \
    $$PWD/TROUBLESHOOTING.md

#-------------------------------------------------
# ICONS
#-------------------------------------------------
win32|unix {
    RC_FILE = $$PWD/icons/icon.rc
}

#-------------------------------------------------
# LIBRARIES
#-------------------------------------------------
win32 {
    #-------------------------------------------------
    # Windows 32
    #-------------------------------------------------
    LIBS += -lShlwapi # PathAppend()

    #-------------------------------------------------
    # PDCurses
    #-------------------------------------------------
    # you may need that if you want to conditionally compile some parts
    # of your code depending on library availability
    DEFINES += HAVE_LIBPDCURSES=1

    #When you build the library as a Windows DLL, you must always define
    #PDC_DLL_BUILD when linking against it. (Or, if you only want to use
    #the DLL, you could add this definition to your curses.h.)
    DEFINES += PDC_DLL_BUILD=1

    # Otherwise cannot resize the window
    DEFINES += PDC_WIDE=1

    # those two, you really need
    INCLUDEPATH += $$PWD/pdcurses/include/
    LIBS += $$PWD/pdcurses/lib/pdcurses.lib

    # Copy *.DLL in the same path as the application
    isEmpty(NASTRAN_FIND_BUILD_TREE) {
        sub_dir = $$_PRO_FILE_PWD_
        sub_dir ~= s,^$$re_escape($$PWD),,
        NASTRAN_FIND_BUILD_TREE = $$clean_path($$OUT_PWD)
        NASTRAN_FIND_BUILD_TREE ~= s,$$re_escape($$sub_dir)$,,
    }

    NASTRAN_FIND_DATA_PATH = $$NASTRAN_FIND_BUILD_TREE/build

    FILES += $$PWD/pdcurses/bin/pdcurses.dll

    copy2build.input = FILES
    copy2build.output = $$NASTRAN_FIND_DATA_PATH/${QMAKE_FUNC_FILE_IN_stripTestShareDir}
    isEmpty(vcproj):copy2build.variable_out = PRE_TARGETDEPS
    win32:copy2build.commands = $$QMAKE_COPY \"${QMAKE_FILE_IN}\" \"${QMAKE_FILE_OUT}\"
    unix:copy2build.commands = $$QMAKE_COPY ${QMAKE_FILE_IN} ${QMAKE_FILE_OUT}
    copy2build.name = COPY ${QMAKE_FILE_IN}
    copy2build.CONFIG += no_link
    QMAKE_EXTRA_COMPILERS += copy2build

}
unix {
    #-------------------------------------------------
    # NCurses
    #-------------------------------------------------
    LIBS += -lncurses

}


