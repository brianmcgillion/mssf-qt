TARGET    = MssfCredsQt
TEMPLATE  = lib
QT       += dbus

include(../common.pri)

maemo {
    message("Using creds2, (AKA V1)")
    DEFINES += MAEMO
    LIBS += -lcreds
 } else {
    LIBS += -lcreds3
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
