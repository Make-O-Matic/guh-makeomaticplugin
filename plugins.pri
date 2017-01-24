TEMPLATE = lib
CONFIG += plugin

QT += network

QMAKE_CXXFLAGS += -Werror -std=c++11 -g
QMAKE_LFLAGS += -std=c++11

INCLUDEPATH += /usr/include/guh
LIBS += -lguh

# Check for Bluetoot LE support (Qt >= 5.4)
equals(QT_MAJOR_VERSION, 5):greaterThan(QT_MINOR_VERSION, 3) {
    DEFINES += BLUETOOTH_LE
}

# Create plugininfo file
infofile.output = plugininfo.h
infofile.depends = /usr/bin/guh-generateplugininfo
infofile.CONFIG = no_link
JSONFILES = deviceplugin"$$TARGET".json
infofile.input = JSONFILES
infofile.commands = /usr/bin/guh-generateplugininfo -j ${QMAKE_FILE_NAME} -o ${QMAKE_FILE_OUT} -b $$OUT_PWD -t $$TRANSLATIONS; \
                    rsync -a "$$OUT_PWD"/translations/*.qm $$shadowed($$PWD)/translations/;

QMAKE_EXTRA_COMPILERS += infofile
PRE_TARGETDEPS += compiler_infofile_make_all

# Install translation files
translations.path = /usr/share/guh/translations
translations.files = $$[QT_SOURCE_TREE]/translations/*.qm

# Install plugin
target.path = /usr/lib/guh/plugins/
INSTALLS += target translations

