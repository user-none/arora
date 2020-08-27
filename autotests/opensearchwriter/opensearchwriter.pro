TEMPLATE = app
TARGET =
DEPENDPATH += .
INCLUDEPATH += .

include(../autotests.pri)

SOURCES = \
    opensearchengine.cpp \
    opensearchwriter.cpp \
    tst_opensearchwriter.cpp

HEADERS = \
    opensearchengine.h \
    opensearchwriter.h

FORMS =

RESOURCES = \
    opensearchwriter.qrc

