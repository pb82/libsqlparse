%option noyywrap
%{
    #include "../include/Token.hpp"
%}

%%

(?i:ABORT)                          return KEYWORD;
(?i:ADD)                            return KEYWORD;
(?i:AFTER)                          return KEYWORD;
(?i:ALL)                            return KEYWORD;
(?i:ALTER)                          return KEYWORD;
(?i:ANALYZE)                        return KEYWORD;
(?i:AND)                            return KEYWORD;
(?i:AS)                             return KEYWORD;
(?i:ASC)                            return KEYWORD;
(?i:ATTACH)                         return KEYWORD;
(?i:AUTOINCREMENT)                  return KEYWORD;
(?i:BEGIN)                          return KEYWORD;
(?i:BETWEEN)                        return KEYWORD;
(?i:BY)                             return KEYWORD;
(?i:CASCADE)                        return KEYWORD;
(?i:CASE)                           return KEYWORD;
(?i:CAST)                           return KEYWORD;
(?i:CHECK)                          return KEYWORD;
(?i:COLLATE)                        return KEYWORD;
(?i:COLUMN)                         return KEYWORD;
(?i:COMMIT)                         return KEYWORD;
(?i:CONFLICT)                       return KEYWORD;
(?i:CONSTRAINT)                     return KEYWORD;
(?i:CREATE)                         return KEYWORD;
(?i:CROSS)                          return KEYWORD;
(?i:CURRENT_DATE)                   return KEYWORD;
(?i:CURRENT_TIME)                   return KEYWORD;
(?i:CURRENT_TIMESTAMP)              return KEYWORD;
(?i:DATEBASE)                       return KEYWORD;
(?i:DEFAULT)                        return KEYWORD;
(?i:DEFERRED)                       return KEYWORD;
(?i:DEFERRABLE)                     return KEYWORD;
(?i:DELETE)                         return KEYWORD;
(?i:DESC)                           return KEYWORD;
(?i:DETACH)                         return KEYWORD;
(?i:DISTINCT)                       return KEYWORD;
(?i:DROP)                           return KEYWORD;
(?i:END)                            return KEYWORD;
(?i:EACH)                           return KEYWORD;
(?i:ELSE)                           return KEYWORD;
(?i:ESCAPE)                         return KEYWORD;
(?i:EXCEPT)                         return KEYWORD;
(?i:EXCLUSIVE)                      return KEYWORD;
(?i:EXISTS)                         return KEYWORD;
(?i:EXPLAIN)                        return KEYWORD;
(?i:FAIL)                           return KEYWORD;
(?i:FOR)                            return KEYWORD;
(?i:FOREIGN)                        return KEYWORD;
(?i:FROM)                           return KEYWORD;
(?i:FULL)                           return KEYWORD;
(?i:GLOB)                           return KEYWORD;
(?i:GROUP)                          return KEYWORD;
(?i:HAVING)                         return KEYWORD;
(?i:IF)                             return KEYWORD;
(?i:IGNORE)                         return KEYWORD;
(?i:IMMEDIATE)                      return KEYWORD;
(?i:IN)                             return KEYWORD;
(?i:INDEX)                          return KEYWORD;
(?i:INITIALLY)                      return KEYWORD;
(?i:INNER)                          return KEYWORD;
(?i:INSERT)                         return KEYWORD;
(?i:INSTEAD)                        return KEYWORD;
(?i:INTERSECT)                      return KEYWORD;
(?i:INTO)                           return KEYWORD;
(?i:IS)                             return KEYWORD;
(?i:ISNULL)                         return KEYWORD;
(?i:JOIN)                           return KEYWORD;
(?i:KEY)                            return KEYWORD;
(?i:LEFT)                           return KEYWORD;
(?i:LIKE)                           return KEYWORD;
(?i:LIMIT)                          return KEYWORD;
(?i:MATCH)                          return KEYWORD;
(?i:NATURAL)                        return KEYWORD;
(?i:NOT)                            return KEYWORD;
(?i:NOTNULL)                        return KEYWORD;
(?i:NULL)                           return KEYWORD;
(?i:OFF)                            return KEYWORD;
(?i:OFFSET)                         return KEYWORD;
(?i:ON)                             return KEYWORD;
(?i:OR)                             return KEYWORD;
(?i:ORDER)                          return KEYWORD;
(?i:OUTER)                          return KEYWORD;
(?i:PLAN)                           return KEYWORD;
(?i:PRAGMA)                         return KEYWORD;
(?i:PRIMARY)                        return KEYWORD;
(?i:QUERY)                          return KEYWORD;
(?i:RAISE)                          return KEYWORD;
(?i:REFERENCES)                     return KEYWORD;
(?i:REGEXP)                         return KEYWORD;
(?i:REINDEX)                        return KEYWORD;
(?i:RENAME)                         return KEYWORD;
(?i:REPLACE)                        return KEYWORD;
(?i:RESTRICT)                       return KEYWORD;
(?i:RIGHT)                          return KEYWORD;
(?i:ROLLBACK)                       return KEYWORD;
(?i:ROW)                            return KEYWORD;
(?i:SELECT)                         return KEYWORD;
(?i:SET)                            return KEYWORD;
(?i:TABLE)                          return KEYWORD;
(?i:TEMP)                           return KEYWORD;
(?i:TEMPORARY)                      return KEYWORD;
(?i:THEN)                           return KEYWORD;
(?i:TO)                             return KEYWORD;
(?i:TRANSACTION)                    return KEYWORD;
(?i:TRIGGER)                        return KEYWORD;
(?i:UNION)                          return KEYWORD;
(?i:UNIQUE)                         return KEYWORD;
(?i:UPDATE)                         return KEYWORD;
(?i:USING)                          return KEYWORD;
(?i:VACUUM)                         return KEYWORD;
(?i:VALUES)                         return KEYWORD;
(?i:VIEW)                           return KEYWORD;
(?i:VIRTUAL)                        return KEYWORD;
(?i:WHEN)                           return KEYWORD;
(?i:WHERE)                          return KEYWORD;

\"(\\.|[^"])*\"                     return IDENTIFIER;

'(?:[^']|'')*'                      return STRING;
[xX]'(?:[^']|'')*'                  return BLOB;
":"[1-9][0-9]*                      return PARAMETER;

[A-Za-z][A-Za-z0-9_$]*              return VALUE;

"("                                 return LP;
")"                                 return RP;
"-"                                 return MINUS;
"+"                                 return PLUS;
";"                                 return SEMI;
"*"                                 return STAR;
"/"                                 return SLASH;
"%"                                 return REM;
(=|==)                              return EQ;
(<>|!=)                             return NE;
"<="                                return LE;
"<"                                 return LT;
">="                                return GE;
">"                                 return GT;
"<<"                                return LSHIFT;
">>"                                return RSHIFT;
","                                 return COMMA;
"&"                                 return BITAND;
"|"                                 return BITOR;
"~"                                 return BITNOT;
"||"                                return CONCAT;
"."                                 return DOT;

[0-9]+                         |
[0-9]+"."[0-9]*                |
"."[0-9]*                      return NUMBER;

[0-9]+[E][+-]?[0-9]+           |
[0-9]+"."[0-9]*[E][+-]?[0-9]+  |
"."[0-9]*[E][+-]?[0-9]+        return NUMBER;

\n                             return NEWLINE;
[ \t]+                         /* Ignore whitespace */

.                              return ERROR;

%%
