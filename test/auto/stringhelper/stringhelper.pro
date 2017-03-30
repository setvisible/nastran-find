#isEmpty(TEMPLATE):TEMPLATE=app
TARGET       = tst_stringhelper
CONFIG      += testcase
QT           = core testlib
SOURCES     += tst_stringhelper.cpp

# Dependancies:
HEADERS += $$PWD/../../../src/stringhelper.h
SOURCES += $$PWD/../../../src/stringhelper.cpp
