TARGET = MssfCryptoQt
TEMPLATE = lib

include(../common.pri)

CONFIG += link_pkconfig
PKGCONFIG += mssf-crypto

SOURCES += \
    mssfcrypto.cpp

HEADERS +=\
    mssfcrypto.h \
    MssfCrypto

include(../install.pri)
