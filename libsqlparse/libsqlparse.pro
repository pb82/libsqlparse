TEMPLATE = lib
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += dynamiclib
TARGET = sqlparse
QMAKE_CXXFLAGS += -std=c++11

SOURCES += \
    ../src/Parser.cpp \
    ../src/Tokens.cpp \
    ../src/BaseParser.cpp

FLEXSOURCES += ../resource/sql.l
OTHER_FILES += $$FLEXSOURCES

flex.commands = flex++ --nomain --noyywrap -o ../../src/Scanner.cpp ${QMAKE_FILE_IN}
flex.input = FLEXSOURCES
flex.output = ../../src/Scanner.cpp
flex.variable_out = SOURCES
flex.CONFIG = target_predeps
flex.clean = ../../src/Scanner.cpp
flex.name = flex++
QMAKE_EXTRA_COMPILERS+=flex

HEADERS += \
    ../include/Token.hpp \
    ../include/Node.hpp \
    ../include/Tokens.hpp \
    ../include/Parser.hpp \
    ../include/exceptions/EndOfStreamException.h \
    ../include/exceptions/IllegalModificationException.h \
    ../include/exceptions/IllegalTokenException.h \
    ../include/BaseParser.hpp \
    ../include/exceptions/UnexpectedTokenException.hpp \
    ../include/parsers/AlterTable.hpp

OTHER_FILES += \
    ../resource/sql.l
