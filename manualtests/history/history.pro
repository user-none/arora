TEMPLATE = app
TARGET =
DEPENDPATH += .
INCLUDEPATH += .

include(../manualtests.pri)
include(../../autotests/modeltest/modeltest.pri)

# Input
SOURCES += main_history.cpp
HEADERS +=
