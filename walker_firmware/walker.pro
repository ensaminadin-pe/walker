TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt
DEFINES += IS_QT

SOURCES += \
	main.cpp \
    gait.cpp \
    oscillator.cpp \
    walker.cpp \
    walkerleg.cpp \
    walkerjoint.cpp \
    gaitdictionary.cpp \
    gaitdictionarymgr.cpp \
    arduinopolyfill.cpp \
    mainfunctions.cpp \
    stdpolyfill.cpp \
    servodriver.cpp \
    gaitdictionary_4_2.cpp \
    wiinunchuck.cpp \
    radio.cpp

HEADERS += \
    oscillator.h \
    gait.h \
    walker.h \
    walkerleg.h \
    walkerjoint.h \
    gaitdictionary.h \
    types.h \
    gaitdictionarymgr.h \
    arduinopolyfill.h \
    config.h \
    mainfunctions.h \
    stdpolyfill.h \
    servodriver.h \
    gaitdictionary_4_2.h \
    gaits_4_2.h \
    esp8266_watchdog.h \
    wiinunchuck.h \
    radio.h
