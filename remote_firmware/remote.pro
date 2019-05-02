TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt
DEFINES += IS_QT

SOURCES += \
        main.cpp \
    arduinopolyfill.cpp \
    main.cpp \
    mainfunctions.cpp \
    wiinunchuck.cpp \
    stdpolyfill.cpp \
    radio.cpp

HEADERS += \
    arduinopolyfill.h \
    mainfunctions.h \
    types.h \
    wiinunchuck.h \
    stdpolyfill.h \
    config.h \
	radio.h
