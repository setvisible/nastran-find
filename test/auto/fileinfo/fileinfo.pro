#isEmpty(TEMPLATE):TEMPLATE=app
TARGET       = tst_fileinfo
CONFIG      += testcase
QT           = core testlib
SOURCES     += tst_fileinfo.cpp

# Include:
INCLUDEPATH += $$PWD/../../../include

# Dependancies:
HEADERS += $$PWD/../../../src/fileinfo.h
SOURCES += $$PWD/../../../src/fileinfo.cpp
HEADERS += $$PWD/../../../src/systemdetection.h
