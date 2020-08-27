TEMPLATE = app
TARGET =
DEPENDPATH += .
INCLUDEPATH += .

include(../autotests.pri)

SOURCES = \
    opensearchengine.cpp \
    opensearchreader.cpp \
    tst_opensearchreader.cpp

HEADERS = \
    opensearchengine.h \
    opensearchreader.h

FORMS =

RESOURCES = \
    opensearchreader.qrc

