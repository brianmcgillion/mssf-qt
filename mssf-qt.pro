TEMPLATE = subdirs
SUBDIRS = src tests doc

QTCONFIGFILES.files = mssf-qt.prf
QTCONFIGFILES.path = /usr/share/qt4/mkspecs/features

INSTALLS += QTCONFIGFILES
