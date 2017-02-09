TRANSLATIONS = translations/en_US.ts \
               translations/de_DE.ts

include(plugins.pri)

CONFIG += link_pkgconfig
PKGCONFIG += libmongocxx

QT += bluetooth

TARGET = $$qtLibraryTarget(guh_devicepluginmakeomatic)

message(============================================)
message("Qt version: $$[QT_VERSION]")
message("Building $$deviceplugin$${TARGET}.so")

SOURCES += \
    devicepluginmakeomatic.cpp \
    glove/glove.cpp \
    glove/cobs/cobs.c

HEADERS += \
    devicepluginmakeomatic.h \
    glove/glove.h \
    glove/packet.h \
    glove/cobs/cobs.h
