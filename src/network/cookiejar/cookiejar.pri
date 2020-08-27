INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

HEADERS += \
  cookiedialog.h \
  cookieexceptionsdialog.h \
  cookieexceptionsmodel.h \
  cookiejar.h \
  cookiemodel.h

SOURCES += \
  cookiedialog.cpp \
  cookieexceptionsmodel.cpp \
  cookiemodel.cpp \
  cookieexceptionsdialog.cpp \
  cookiejar.cpp

FORMS += \
    cookies.ui \
    cookiesexceptions.ui

include($$PWD/networkcookiejar/networkcookiejar.pri)

