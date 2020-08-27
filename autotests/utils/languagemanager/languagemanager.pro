TEMPLATE = app
TARGET =
DEPENDPATH += .
INCLUDEPATH += . ../

include(../../autotests.pri)
include(../../../src/locale/locale.pri)

# Input
SOURCES = languagemanager.cpp tst_languagemanager.cpp
HEADERS = languagemanager.h
FORMS =
RESOURCES =
