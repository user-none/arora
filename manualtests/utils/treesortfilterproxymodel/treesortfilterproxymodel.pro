TEMPLATE = app
TARGET =
DEPENDPATH += .
INCLUDEPATH += .

include(../../manualtests.pri)

# Input
SOURCES = main_treesortfilterproxymodel.cpp treesortfilterproxymodel.cpp
HEADERS = treesortfilterproxymodel.h

FORMS += treesortfilterproxymodeldialog.ui
