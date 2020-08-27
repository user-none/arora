TEMPLATE = app
TARGET =
DEPENDPATH += .
INCLUDEPATH += . ../

include(../autotests.pri)

QT = core

# Input
SOURCES = bookmarknode.cpp tst_bookmarknode.cpp
HEADERS = bookmarknode.h
FORMS =
RESOURCE =
