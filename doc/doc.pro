OTHER_FILES += \
    Doxyfile

dox.target = doc
dox.files = Doxyfile
dox.commands = doxygen Doxyfile
dox.depends =

doc_clean.commands = -$(DEL_FILE) html/*
doc_distclean.commands = -$(DEL_DIR) html
doc_distclean.depends = doc_clean
clean.depends += doc_clean
distclean.depends += doc_distclean

QMAKE_EXTRA_TARGETS += dox clean distclean doc_clean doc_distclean
