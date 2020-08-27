TEMPLATE = app
TARGET =
DEPENDPATH += .
INCLUDEPATH += .

include(../manualtests.pri)

# Input
RESOURCES =
FORMS =
SOURCES = searchbutton.cpp clearbutton.cpp lineedit.cpp searchlineedit.cpp
HEADERS = searchbutton.h clearbutton.h lineedit.h lineedit_p.h searchlineedit.h

SOURCES += main_searchlineedit.cpp
