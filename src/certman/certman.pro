TARGET = MssfCertManQt
TEMPLATE = lib

include(../common.pri)

CONFIG += link_pkconfig

maemo {
    message("Using Aegis, (AKA V1)")
    PKGCONFIG += aegis-certman
    DEFINES += MAEMO
 } else {
    message("Using Mssf V2")
    PKGCONFIG += mssf-certman
 }

SOURCES += \
    mssfcertman.cpp

HEADERS +=\
    mssfcertman.h

include(../install.pri)
