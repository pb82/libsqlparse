TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
QMAKE_CXXFLAGS += -std=c++11

LIBS += -L../debug -lsqlparse

SOURCES += main.cpp

HEADERS += \
    Catch/catch.hpp

