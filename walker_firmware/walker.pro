TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt
DEFINES += IS_QT

SOURCES += \
	main.cpp \
    walker.cpp \
    walkerleg.cpp \
    walkerjoint.cpp \
    arduinopolyfill.cpp \
    mainfunctions.cpp \
    stdpolyfill.cpp \
    servodriver.cpp \
    wiinunchuck.cpp \
    radio.cpp \
    beeper.cpp \
    fabrik2d.cpp

HEADERS += \
    walker.h \
    walkerleg.h \
    walkerjoint.h \
    types.h \
    arduinopolyfill.h \
    config.h \
    mainfunctions.h \
    stdpolyfill.h \
    servodriver.h \
    esp8266_watchdog.h \
    wiinunchuck.h \
    radio.h \
    beeper.h \
    fabrik2d.h \
    kinematic.h
