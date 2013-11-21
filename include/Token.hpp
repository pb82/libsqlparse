#ifndef TOKEN_HPP
#define TOKEN_HPP

namespace Sql { typedef int TOKEN; }

#define ERROR             -99
#define NEWLINE           -98
#define KEYWORD             1
#define NUMBER              2
#define STRING              3
#define BLOB                4
#define PARAMETER           5
#define VALUE               6
#define LP                  7
#define RP                  8
#define MINUS               9
#define PLUS                10
#define SEMI                11
#define STAR                12
#define SLASH               13
#define REM                 14
#define EQ                  15
#define LE                  16
#define NE                  17
#define LT                  18
#define GE                  19
#define GT                  20
#define LSHIFT              21
#define RSHIFT              22
#define COMMA               23
#define BITAND              24
#define BITOR               25
#define BITNOT              26
#define CONCAT              27
#define DOT                 28
#define IDENTIFIER          29

#endif // TOKEN_HPP
