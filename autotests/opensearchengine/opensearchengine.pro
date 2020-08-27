TEMPLATE = app
TARGET =
DEPENDPATH += .
INCLUDEPATH += .

include(../autotests.pri)

SOURCES = \
    opensearchengine.cpp \
    opensearchenginedelegate.cpp \
    tst_opensearchengine.cpp

HEADERS = \
    opensearchengine.h \
    opensearchenginedelegate.h

FORMS =

RESOURCES = \
    opensearchengine.qrc
