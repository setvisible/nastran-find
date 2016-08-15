#isEmpty(TEMPLATE):TEMPLATE=app
TARGET       = tst_engine-include
CONFIG      += testcase
QT           = core testlib
SOURCES     += tst_engine-include.cpp

# Dependancies:
HEADERS += ../../../src/engine.h
SOURCES += ../../../src/engine.cpp
HEADERS += ../../../src/fileinfo.h
SOURCES += ../../../src/fileinfo.cpp
HEADERS += ../../../src/stringhelper.h
SOURCES += ../../../src/stringhelper.cpp
HEADERS += ../../../src/systemdetection.h
