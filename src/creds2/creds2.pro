TARGET = MssfCreds2Qt
TEMPLATE = lib
QT       += dbus

include(../common.pri)

LIBS += -lcreds

SOURCES += \
    dbuscontextaccessmanager.cpp \
    credentialsmanager.cpp

HEADERS +=\
    dbuscontextaccessmanager.h \
    credentialsmanager.h \
    DBusContextAccessManager \
    CredentialsManager

include(../install.pri)
