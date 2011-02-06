TARGET    = MssfCredsQt
TEMPLATE  = lib
QT       += dbus

include(../common.pri)

LIBS += -lcreds

maemo {
    message("Using creds2, (AKA V1)")
    DEFINES += MAEMO
 }

SOURCES += \
    dbuscontextaccessmanager.cpp \
    credentialsmanager.cpp

HEADERS +=\
    dbuscontextaccessmanager.h \
    credentialsmanager.h \
    DBusContextAccessManager \
    CredentialsManager

include(../install.pri)
