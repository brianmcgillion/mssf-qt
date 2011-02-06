MSSF_LIB_PATH = /usr/lib
MSSF_INCLUDE_PATH = /usr/include/mssf-qt

target.path = $${MSSF_LIB_PATH}

headerfiles.files = $${HEADERS}
headerfiles.path = $${MSSF_INCLUDE_PATH}

CONFIG += link_pkgconfig create_pc create_prl
QMAKE_PKGCONFIG_NAME = lib$$TARGET
QMAKE_PKGCONFIG_DESCRIPTION = lib$$TARGET library
QMAKE_PKGCONFIG_INCDIR = $${MSSF_INCLUDE_PATH}
QMAKE_PKGCONFIG_CFLAGS += -I$${MSSF_INCLUDE_PATH}
QMAKE_PKGCONFIG_DESTDIR = pkgconfig

QMAKE_CLEAN += *.pc *.prl

pkgconfig_clean.commands = -$(DEL_FILE) pkgconfig/*
pkgconfig_distclean.commands = -$(DEL_DIR) pkgconfig
pkgconfig_distclean.depends = pkgconfig_clean
clean.depends += pkgconfig_clean
distclean.depends += pkgconfig_distclean

QMAKE_EXTRA_TARGETS += clean distclean pkgconfig_clean pkgconfig_distclean

INSTALLS += target headerfiles
