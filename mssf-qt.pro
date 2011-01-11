TEMPLATE = subdirs
SUBDIRS = src doc tests

QTCONFIGFILES.files = mssf-qt.prf
QTCONFIGFILES.path = /usr/share/qt4/mkspecs/features

INSTALLS += QTCONFIGFILES
