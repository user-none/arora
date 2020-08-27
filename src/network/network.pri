INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

FORMS += \
    passworddialog.ui \
    proxy.ui

HEADERS += \
    fileaccesshandler.h \
    networkaccessmanager.h \
    networkdiskcache.h \
    networkproxyfactory.h \
    schemeaccesshandler.h

SOURCES += \
    fileaccesshandler.cpp \
    networkaccessmanager.cpp \
    networkdiskcache.cpp \
    networkproxyfactory.cpp \
    schemeaccesshandler.cpp

include(cookiejar/cookiejar.pri)
