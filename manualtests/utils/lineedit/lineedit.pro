TEMPLATE = app
TARGET =
DEPENDPATH += .
INCLUDEPATH += .

include(../../manualtests.pri)

# Input
RESOURCES =
FORMS   = dialog.ui
SOURCES = main_lineedit.cpp lineedit.cpp
HEADERS = lineedit.h lineedit_p.h
