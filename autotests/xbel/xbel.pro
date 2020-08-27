TEMPLATE = app
TARGET =
DEPENDPATH += .
INCLUDEPATH += .

include(../autotests.pri)

# Input
SOURCES = \
    tst_xbel.cpp \
    bookmarks/bookmarknode.cpp \
    bookmarks/xbel/xbelreader.cpp \
    bookmarks/xbel/xbelwriter.cpp

HEADERS = \
    bookmarks/bookmarknode.h \
    bookmarks/xbel/xbelreader.h \
    bookmarks/xbel/xbelwriter.h
