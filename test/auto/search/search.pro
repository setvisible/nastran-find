#isEmpty(TEMPLATE):TEMPLATE=app
TARGET       = tst_search
CONFIG      += testcase
QT           = core testlib
SOURCES     += tst_search.cpp

# Dependancies:
HEADERS += $$PWD/../../../src/engine.h
SOURCES += $$PWD/../../../src/engine.cpp
HEADERS += $$PWD/../../../src/fileinfo.h
SOURCES += $$PWD/../../../src/fileinfo.cpp
HEADERS += $$PWD/../../../src/stringhelper.h
SOURCES += $$PWD/../../../src/stringhelper.cpp
HEADERS += $$PWD/../../../src/systemdetection.h
