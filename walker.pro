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
    gaits/gaitdictionary_4_2.cpp \
    gaitdictionarymgr.cpp \
    arduinopolyfill.cpp \
    mainfunctions.cpp \
    stdpolyfill.cpp

HEADERS += \
    oscillator.h \
    gait.h \
    walker.h \
    walkerleg.h \
    walkerjoint.h \
    gaits/gaits_4_2.h \
    gaitdictionary.h \
    gaits/gaitdictionary_4_2.h \
    types.h \
    gaitdictionarymgr.h \
    arduinopolyfill.h \
    config.h \
    mainfunctions.h \
    stdpolyfill.h
