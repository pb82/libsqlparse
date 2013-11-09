%option noyywrap
%{
    #include "../include/Token.hpp"
%}

%%

[-]?[0-9]+                          |
[-]?[0-9]+"."[0-9]*                 |
[-]?"."[0-9]*                                 return NUMBER;

[-]?[0-9]+[eE][+-]?[0-9]+           |
[-]?[0-9]+"."[0-9]*[eE][+-]?[0-9]+  |
[-]?"."[0-9]*[eE][+-]?[0-9]+                  return NUMBER;

\n                              /* Ignore newlines */
[ \t]+                          /* Ignore whitespace */

.                                             return ERROR;

%%
