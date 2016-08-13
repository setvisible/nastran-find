#isEmpty(TEMPLATE):TEMPLATE=app
TARGET       = tst_fileinfo
CONFIG      += testcase
QT           = core testlib
SOURCES     += tst_fileinfo.cpp

# Dependancies:
HEADERS += ../../../src/fileinfo.h
SOURCES += ../../../src/fileinfo.cpp
