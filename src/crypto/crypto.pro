TARGET = MssfCryptoQt
TEMPLATE = lib

include(../common.pri)

CONFIG += link_pkconfig

maemo {
    message("Using Aegis, (AKA V1)")
    PKGCONFIG += aegis-crypto
    DEFINES += MAEMO
 } else {
    message("Using Mssf V2")
    PKGCONFIG += mssf-crypto
 }

SOURCES += \
    mssfcrypto.cpp \
    mssfstorage.cpp \
    protectedfile.cpp

PUBLIC_HEADERS += \
    mssfcrypto.h \
    MssfCrypto \
    mssfstorage.h \
    MssfStorage \
    protectedfile.h \
    ProtectedFile

PRIVATE_HEADERS += \
    mssfstorage_p.h \
    protectedfile_p.h

HEADERS += \
    $$PUBLIC_HEADERS \
    $$PRIVATE_HEADERS

include(../install.pri)
