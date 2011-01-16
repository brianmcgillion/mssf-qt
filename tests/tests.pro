
QT += testlib

 # install
target.path = $$(DESTDIR)/usr/bin
SOURCES += \
    testmssfcryptoqt.cpp

INSTALLS += target
