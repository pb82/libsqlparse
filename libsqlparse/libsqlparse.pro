TEMPLATE = lib
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += dynamiclib
TARGET = sqlparse
QMAKE_CXXFLAGS += -std=c++11

SOURCES += \
    ../src/Parser.cpp \
    ../src/Tokens.cpp \
    ../src/BaseParser.cpp \
    ../src/parsers/AlterTable.cpp \
    ../src/parsers/ColumnDef.cpp \
    ../src/parsers/TypeName.cpp \
    ../src/parsers/ColumnConstraint.cpp \
    ../src/parsers/ConflictClause.cpp \
    ../src/parsers/Expression.cpp \
    ../src/parsers/Select.cpp \
    ../src/parsers/ForeignKeyClause.cpp \
    ../src/parsers/Analyze.cpp \
    ../src/parsers/Attach.cpp \
    ../src/parsers/Begin.cpp \
    ../src/parsers/Commit.cpp \
    ../src/parsers/Rollback.cpp \
    ../src/parsers/Create.cpp \
    ../src/parsers/Update.cpp \
    ../src/parsers/Insert.cpp \
    ../src/parsers/Delete.cpp \
    ../src/parsers/QualifiedTableName.cpp

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
    ../include/BaseParser.hpp \
    ../include/exceptions/UnexpectedTokenException.hpp \
    ../include/parsers/AlterTable.hpp \
    ../include/parsers/ColumnDef.hpp \
    ../include/parsers/TypeName.hpp \
    ../include/parsers/ConflictClause.hpp \
    ../include/parsers/ColumnConstraint.hpp \
    ../include/parsers/Expression.hpp \
    ../include/parsers/Select.hpp \
    ../include/exceptions/EndOfStreamException.hpp \
    ../include/exceptions/IllegalModificationException.hpp \
    ../include/exceptions/IllegalParserStateException.hpp \
    ../include/exceptions/IllegalTokenException.hpp \
    ../include/exceptions/UnknonwSubsetException.hpp \
    ../include/parsers/ForeignKeyClause.hpp \
    ../include/parsers/Analyze.hpp \
    ../include/parsers/Attach.hpp \
    ../include/parsers/Begin.hpp \
    ../include/parsers/Commit.hpp \
    ../include/parsers/Rollback.hpp \
    ../include/parsers/Create.hpp \
    ../include/exceptions/StreamNotEmptyException.hpp \
    ../include/parsers/Update.hpp \
    ../include/parsers/Insert.hpp \
    ../include/parsers/Delete.hpp \
    ../include/parsers/QualifiedTableName.hpp

OTHER_FILES += \
    ../resource/sql.l
