MSSF_LIB_PATH = /usr/lib
MSSF_INCLUDE_PATH = /usr/include

target.path = $${MSSF_LIB_PATH}

headerfiles.files = $${HEADERS}
headerfiles.path = $${MSSF_INCLUDE_PATH}/$${TARGET}

CONFIG += link_pkgconfig create_pc create_prl
QMAKE_PKGCONFIG_NAME = lib$$TARGET
QMAKE_PKGCONFIG_DESCRIPTION = lib$$TARGET library
QMAKE_PKGCONFIG_INCDIR = $${MSSF_INCLUDE_PATH}/$${TARGET}
QMAKE_PKGCONFIG_CFLAGS += -I$${MSSF_INCLUDE_PATH}
pkgconfig.path = $$MSSF_INCLUDE_PATH/pkgconfig
pkgconfig.files = $$TARGET.pc

QMAKE_CLEAN += *.pc *.prl

INSTALLS += target headerfiles
