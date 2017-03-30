include(../../shared/static.pro)

#isEmpty(TEMPLATE):TEMPLATE=app
TARGET       = tst_engine
CONFIG      += testcase
QT           = core testlib
SOURCES     += tst_engine.cpp

#TESTDATA = shared/*

# Include:
INCLUDEPATH += $$PWD/../../../include

# Dependancies:
HEADERS += $$PWD/../../../src/engine.h
SOURCES += $$PWD/../../../src/engine.cpp
HEADERS += $$PWD/../../../src/fileinfo.h
SOURCES += $$PWD/../../../src/fileinfo.cpp
HEADERS += $$PWD/../../../src/stringhelper.h
SOURCES += $$PWD/../../../src/stringhelper.cpp
HEADERS += $$PWD/../../../src/systemdetection.h
