#isEmpty(TEMPLATE):TEMPLATE=app
TARGET       = tst_stringhelper
CONFIG      += testcase
QT           = core testlib
SOURCES     += tst_stringhelper.cpp

# Dependancies:
HEADERS += ../../../src/stringhelper.h
SOURCES += ../../../src/stringhelper.cpp
