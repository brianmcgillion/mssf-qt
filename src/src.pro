TEMPLATE = subdirs
SUBDIRS = creds crypto

GLOBAL.files = ./global/mssf-qt_global.h
GLOBAL.path = /usr/include/mssf-qt

INSTALLS += GLOBAL
