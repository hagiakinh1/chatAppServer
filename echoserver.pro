QT       += core websockets sql
QT       -= gui

TARGET = echoserver
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

SOURCES += \
    main.cpp \
    echoserver.cpp\
    dataaccessobject.cpp\
    dataaccessobject.h

HEADERS += \
    echoserver.h

EXAMPLE_FILES += echoclient.html

target.path = $$[QT_INSTALL_EXAMPLES]/websockets/echoserver
INSTALLS += target
