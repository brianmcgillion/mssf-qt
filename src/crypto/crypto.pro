TARGET = MssfCryptoQt
TEMPLATE = lib

include(../common.pri)

CONFIG += link_pkconfig
PKGCONFIG += mssf-crypto

SOURCES += \
    mssfcrypto.cpp \
    mssfstorage.cpp

HEADERS +=\
    mssfcrypto.h \
    MssfCrypto \
    mssfstorage.h \
    MssfStorage

include(../install.pri)
