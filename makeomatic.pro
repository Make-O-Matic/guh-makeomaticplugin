TRANSLATIONS = translations/en_US.ts \
               translations/de_DE.ts

include(plugins.pri)
QT += bluetooth
TARGET = guh_devicepluginmakeomatic

message(============================================)
message("Qt version: $$[QT_VERSION]")
message("Building $$deviceplugin$${TARGET}.so")

SOURCES += \
    devicepluginmakeomatic.cpp \
    glove.cpp

HEADERS += \
    devicepluginmakeomatic.h \
    glove.h
