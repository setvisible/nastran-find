include(../../shared/static.pro)

#isEmpty(TEMPLATE):TEMPLATE=app
TARGET       = tst_engine
CONFIG      += testcase
QT           = core testlib
SOURCES     += tst_engine.cpp

#TESTDATA = shared/*

# Dependancies:
HEADERS += ../../../src/engine.h
SOURCES += ../../../src/engine.cpp
HEADERS += ../../../src/fileinfo.h
SOURCES += ../../../src/fileinfo.cpp
HEADERS += ../../../src/stringhelper.h
SOURCES += ../../../src/stringhelper.cpp
HEADERS += ../../../src/systemdetection.h
