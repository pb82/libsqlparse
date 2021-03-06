#define CATCH_CONFIG_RUNNER
#include <iostream>
#include <sstream>

#include "./Catch/catch.hpp"
#include "../include/Parser.hpp"

using namespace Sql;
using namespace Sql::Exceptions;

TEST_CASE( "base/tokenizer", "Tokenizer tests" ) {
    Parser p;
    p.feed ("1 2 3");
    REQUIRE(p.getTokenCount () == 3);

    p.reset ();
    p.feed ("1 'a' x'a' \"b\"");
    REQUIRE(p.getTokenCount () == 4);
}

TEST_CASE( "base/parser", "Parser tests" ) {
    Parser p;
    p.feed ("alter table db.users rename to user");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("ALTER TABLE db.users RENAME TO user");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("ALTER TABLE db.users ADD Id");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("ALTER TABLE db.users ADD Id INTEGER");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("ALTER TABLE db.users ADD Id INTEGER(1)");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("ALTER TABLE db.users ADD Id INTEGER(1,2)");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("ALTER TABLE db.users ADD Id INTEGER(1,2) PRIMARY KEY");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("ALTER TABLE db.users ADD Id INTEGER(1,2) PRIMARY KEY ASC");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("ALTER TABLE db.users ADD Id INTEGER(1,2) PRIMARY KEY DESC");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("ALTER TABLE db.users ADD Id INTEGER(1,2) PRIMARY KEY DESC AUTOINCREMENT");
    REQUIRE_NOTHROW(p.parse ());

    std::stringstream ss;
    ss << "ALTER TABLE db.users ADD Id INTEGER(1,2) PRIMARY KEY DESC\n";
    ss << "ON CONFLICT ROLLBACK\n";
    ss << "AUTOINCREMENT";

    p.reset ();
    p.feed (ss);
    REQUIRE_NOTHROW(p.parse ());

    std::stringstream ss1;
    ss1 << "ALTER TABLE db.users ADD Id INTEGER(1,2) PRIMARY KEY DESC\n";
    ss1 << "ON CONFLICT ABORT\n";

    p.reset ();
    p.feed (ss1);
    REQUIRE_NOTHROW(p.parse ());

    std::stringstream ss2;
    ss2 << "ALTER TABLE db.users ADD Id INTEGER(1,2) PRIMARY KEY\n";
    ss2 << "ON CONFLICT FAIL\n";

    p.reset ();
    p.feed (ss2);
    REQUIRE_NOTHROW(p.parse ());

    std::stringstream ss3;
    ss3 << "ALTER TABLE db.users ADD Id INTEGER(1,2) PRIMARY KEY\n";
    ss3 << "ON CONFLICT IGNORE AUTOINCREMENT\n";

    p.reset ();
    p.feed (ss3);
    REQUIRE_NOTHROW(p.parse ());

    std::stringstream ss4;
    ss4 << "ALTER TABLE users ADD Id PRIMARY KEY\n";
    ss4 << "ON CONFLICT REPLACE AUTOINCREMENT\n";

    p.reset ();
    p.feed (ss4);
    REQUIRE_NOTHROW(p.parse ());

    std::stringstream ss5;
    ss5 << "ALTER TABLE users ADD Id NOT NULL\n";

    p.reset ();
    p.feed (ss5);
    REQUIRE_NOTHROW(p.parse ());

    std::stringstream ss6;
    ss6 << "ALTER TABLE users ADD Id NOT NULL ON CONFLICT FAIL\n";

    p.reset ();
    p.feed (ss6);
    REQUIRE_NOTHROW(p.parse ());

    std::stringstream ss7;
    ss7 << "ALTER TABLE users ADD Id UNIQUE\n";

    p.reset ();
    p.feed (ss7);
    REQUIRE_NOTHROW(p.parse ());

    std::stringstream ss8;
    ss8 << "ALTER TABLE users ADD Id UNIQUE ON CONFLICT IGNORE\n";

    p.reset ();
    p.feed (ss8);
    REQUIRE_NOTHROW(p.parse ());

    std::stringstream ss9;
    ss9 << "ALTER TABLE users ADD Id CHECK(1+1 IS NOT 3)";

    p.reset ();
    p.feed (ss9);
    REQUIRE_NOTHROW(p.parse ());

    std::stringstream ss10;
    ss10 << "ALTER TABLE users ADD Id CHECK(1+1 IS 3)";

    p.reset ();
    p.feed (ss10);
    REQUIRE_NOTHROW(p.parse ());

    std::stringstream ss11;
    ss11 << "ALTER TABLE users ADD Id CHECK(1+1 IS ())";

    p.reset ();
    p.feed (ss11);
    REQUIRE_NOTHROW(p.parse ());

    std::stringstream ss12;
    ss12 << "SELECT *";

    p.reset ();
    p.feed (ss12);
    REQUIRE_NOTHROW(p.parse ());


    std::stringstream ss13;
    ss13 << "SELECT DISTINCT *";

    p.reset ();
    p.feed (ss13);
    REQUIRE_NOTHROW(p.parse ());


    std::stringstream ss14;
    ss14 << "SELECT ALL *";

    p.reset ();
    p.feed (ss14);
    REQUIRE_NOTHROW(p.parse ());


    std::stringstream ss15;
    ss15 << "SELECT users.*";

    p.reset ();
    p.feed (ss15);
    REQUIRE_NOTHROW(p.parse ());

    std::stringstream ss16;
    ss16 << "SELECT DISTINCT users.*";

    p.reset ();
    p.feed (ss16);
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("ALTER TABLE users ADD Id CHECK(id)");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("ALTER TABLE users ADD Id CHECK(users.id)");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("ALTER TABLE users ADD Id CHECK(db.users.id)");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("ALTER TABLE users ADD Id CHECK(fun())");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("ALTER TABLE users ADD Id CHECK(fun(1))");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("ALTER TABLE users ADD Id CHECK(fun(DISTINCT 1))");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("ALTER TABLE users ADD Id CHECK(fun(DISTINCT 1,2))");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("ALTER TABLE users ADD Id CHECK(fun(1,2,3))");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("ALTER TABLE users ADD Id CHECK(fun(db))");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("ALTER TABLE users ADD Id CHECK(fun(db.users))");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("ALTER TABLE users ADD Id CHECK(fun(db.users.name))");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("ALTER TABLE users ADD Id CHECK(fun((1+1)))");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("ALTER TABLE users ADD Id CHECK(fun_ction((1+1)+(SELECT *)))");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("ALTER TABLE users ADD Id CHECK(fun_ction((1+1)+(SELECT *) + 5))");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("ALTER TABLE users ADD Id CHECK(CAST (1 as INTEGER))");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("ALTER TABLE users ADD Id CHECK(CAST ((1 - 2)+(2*5) as INTEGER))");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("ALTER TABLE users ADD Id CHECK(CAST ((1 - 2)+(2*5) as number(8)))");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("ALTER TABLE users ADD Id CHECK(CAST ((1 - 2)+(2*5) as number(8,0)))");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("ALTER TABLE users ADD Id CHECK(CAST ('-1' as number(8,0)))");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("ALTER TABLE users ADD Id CHECK(CAST (-1 as number(8,0)))");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("ALTER TABLE users ADD Id CHECK(CAST (~1 as number(8,0)))");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("ALTER TABLE users ADD Id CHECK(EXISTS(SELECT *))");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("ALTER TABLE users ADD Id CHECK(NOT EXISTS(SELECT *))");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("ALTER TABLE users ADD Id CHECK(CASE WHEN 1 THEN '1' END)");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("ALTER TABLE users ADD Id CHECK(CASE WHEN 1 THEN '1' WHEN 2 THEN '2' END)");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("ALTER TABLE users ADD Id CHECK(CASE WHEN 1 THEN '1' WHEN 2 THEN '2' ELSE (1+1) END)");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("ALTER TABLE users ADD Id CHECK(CASE WHEN 1 THEN '1' ELSE (1+1) END)");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("ALTER TABLE users ADD Id CHECK(CASE :1 WHEN 1 THEN '1' END)");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("ALTER TABLE users ADD Id CHECK(CASE 1 WHEN 1 THEN '1' WHEN 2 THEN '2' END)");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("ALTER TABLE users ADD Id CHECK(CASE '1' WHEN 1 THEN '1' WHEN 2 THEN '2' ELSE (1+1) END)");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("ALTER TABLE users ADD Id CHECK(CASE (SELECT *) WHEN 1 THEN '1' ELSE (1+1) END)");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("ALTER TABLE users ADD Id CHECK(RAISE(IGNORE))");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("ALTER TABLE users ADD Id CHECK(RAISE(ROLLBACK,'test'))");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("ALTER TABLE users ADD Id CHECK(RAISE(ABORT,'test'))");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("ALTER TABLE users ADD Id CHECK(RAISE(FAIL,'test'))");
    REQUIRE_NOTHROW(p.parse ());
}

TEST_CASE( "base/expression", "Sqlite Expressions" ) {

    Parser p;
    p.feed ("ALTER TABLE users ADD Id CHECK(1)");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("ALTER TABLE users ADD Id CHECK(1.5)");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("ALTER TABLE users ADD Id CHECK(-1.5)");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("ALTER TABLE users ADD Id CHECK(-1.5E+9)");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("ALTER TABLE users ADD Id CHECK('a')");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("ALTER TABLE users ADD Id CHECK(x'blob')");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("ALTER TABLE users ADD Id CHECK(NULL)");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("ALTER TABLE users ADD Id CHECK(CURRENT_DATE)");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("ALTER TABLE users ADD Id CHECK(CURRENT_TIME)");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("ALTER TABLE users ADD Id CHECK(CURRENT_TIMESTAMP)");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("ALTER TABLE users ADD Id CHECK(:1)");
    REQUIRE_NOTHROW(p.parse ());



    p.reset ();
    p.feed ("ALTER TABLE users ADD Id CHECK(db.users.id)");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("ALTER TABLE users ADD Id CHECK(users.id)");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("ALTER TABLE users ADD Id CHECK(id)");
    REQUIRE_NOTHROW(p.parse ());


    p.reset ();
    p.feed ("ALTER TABLE users ADD Id CHECK(-1*1)");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("ALTER TABLE users ADD Id CHECK(+5*5)");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("ALTER TABLE users ADD Id CHECK(~(1+1))");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("ALTER TABLE users ADD Id CHECK(NOT CURRENT_DATE)");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("ALTER TABLE users ADD Id CHECK(5 || '5')");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("ALTER TABLE users ADD Id CHECK(:1 / 2)");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("ALTER TABLE users ADD Id CHECK(:1 >> 2)");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("ALTER TABLE users ADD Id CHECK(:1 << 2)");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("ALTER TABLE users ADD Id CHECK(:1 | 2)");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("ALTER TABLE users ADD Id CHECK(:1 & 2)");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("ALTER TABLE users ADD Id CHECK(:1 < 2)");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("ALTER TABLE users ADD Id CHECK(:1 > 2)");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("ALTER TABLE users ADD Id CHECK(:1 <= 2)");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("ALTER TABLE users ADD Id CHECK(:1 >= 2)");
    REQUIRE_NOTHROW(p.parse ());


    p.reset ();
    p.feed ("ALTER TABLE users ADD Id CHECK(1 > 2 || 2 < 5)");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("ALTER TABLE users ADD Id CHECK(:1 > 1 || :2 < :3)");
    REQUIRE_NOTHROW(p.parse ());




    p.reset ();
    p.feed ("ALTER TABLE users ADD Id CHECK(:1 = 1 || :2 < :3)");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("ALTER TABLE users ADD Id CHECK(:1 == 1 || :2 < :3)");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("ALTER TABLE users ADD Id CHECK(:1 != 1 || :2 < :3)");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("ALTER TABLE users ADD Id CHECK(:1 <> 1 || :2 < :3)");
    REQUIRE_NOTHROW(p.parse ());



    p.reset ();
    p.feed ("ALTER TABLE users ADD Id CHECK(:1 IS :2)");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("ALTER TABLE users ADD Id CHECK(:1 IS NOT NULL)");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("ALTER TABLE users ADD Id CHECK(name LIKE 'test')");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("ALTER TABLE users ADD Id CHECK(name NOT LIKE 'test')");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("ALTER TABLE users ADD Id CHECK(type glob 1)");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("ALTER TABLE users ADD Id CHECK(type match 1)");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("ALTER TABLE users ADD Id CHECK(type regexp 1)");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("ALTER TABLE users ADD Id CHECK(1 or 2)");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("ALTER TABLE users ADD Id CHECK(:1 and :2)");
    REQUIRE_NOTHROW(p.parse ());


    p.reset ();
    p.feed ("ALTER TABLE users ADD Id CHECK((1+1)*((1*:1) && 4 >> 1))");
    REQUIRE_NOTHROW(p.parse ());



    p.reset ();
    p.feed ("ALTER TABLE users ADD Id CHECK(check_fun())");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("ALTER TABLE users ADD Id CHECK(check_fun(*))");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("ALTER TABLE users ADD Id CHECK(check_fun(1,2,3))");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("ALTER TABLE users ADD Id CHECK(check_fun('a','b','c'))");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("ALTER TABLE users ADD Id CHECK(check_fun(distinct :1,2,3))");
    REQUIRE_NOTHROW(p.parse ());



    p.reset ();
    p.feed ("ALTER TABLE users ADD Id CHECK(check_fun() ISNULL)");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("ALTER TABLE users ADD Id CHECK(check_fun(*) NOTNULL)");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("ALTER TABLE users ADD Id CHECK(check_fun(1,2,3) NOT NULL)");
    REQUIRE_NOTHROW(p.parse ());



    p.reset ();
    p.feed ("ALTER TABLE users ADD Id CHECK((1))");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("ALTER TABLE users ADD Id CHECK(((1)))");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("ALTER TABLE users ADD Id CHECK(((1+:1)))");
    REQUIRE_NOTHROW(p.parse ());


    p.reset ();
    p.feed ("ALTER TABLE users ADD Id CHECK(CAST (id AS INTEGER))");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("ALTER TABLE users ADD Id CHECK(CAST (1 AS STRING))");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("ALTER TABLE users ADD Id CHECK(CAST ('1' AS INTEGER))");
    REQUIRE_NOTHROW(p.parse ());


    p.reset ();
    p.feed ("ALTER TABLE users ADD Id CHECK(name COLLATE GERMAN)");
    REQUIRE_NOTHROW(p.parse ());




    p.reset ();
    p.feed ("ALTER TABLE users ADD Id CHECK(1 LIKE :1)");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("ALTER TABLE users ADD Id CHECK(1 NOT LIKE :1)");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("ALTER TABLE users ADD Id CHECK(1 LIKE :1 ESCAPE '1')");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("ALTER TABLE users ADD Id CHECK(1 NOT LIKE :1 ESCAPE '1')");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("ALTER TABLE users ADD Id CHECK(1 NOT GLOB :1 ESCAPE '1')");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("ALTER TABLE users ADD Id CHECK(1 NOT REGEXP :1 ESCAPE '1')");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("ALTER TABLE users ADD Id CHECK(1 NOT MATCH :1 ESCAPE '1')");
    REQUIRE_NOTHROW(p.parse ());


    p.reset ();
    p.feed ("ALTER TABLE users ADD Id CHECK(1 GLOB :1 ESCAPE '1')");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("ALTER TABLE users ADD Id CHECK(1 REGEXP :1 ESCAPE '1')");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("ALTER TABLE users ADD Id CHECK(1 MATCH :1 ESCAPE '1')");
    REQUIRE_NOTHROW(p.parse ());


    p.reset ();
    p.feed ("ALTER TABLE users ADD Id CHECK(1 ISNULL)");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("ALTER TABLE users ADD Id CHECK(1 NOTNULL)");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("ALTER TABLE users ADD Id CHECK(1 NOT NULL)");
    REQUIRE_NOTHROW(p.parse ());


    p.reset ();
    p.feed ("ALTER TABLE users ADD Id CHECK(1 is :1)");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("ALTER TABLE users ADD Id CHECK(1 is not :1)");
    REQUIRE_NOTHROW(p.parse ());


    p.reset ();
    p.feed ("ALTER TABLE users ADD Id CHECK(Id between 1 and 10000)");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("ALTER TABLE users ADD Id CHECK(Id between (1 and 2) and 10000)");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("ALTER TABLE users ADD Id CHECK(Id between (1 and :1 between 2 and 4) and 10000)");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("ALTER TABLE users ADD Id CHECK(Id between (:1 between 1 and 2) and ('2' between 'a' and 'z'))");
    REQUIRE_NOTHROW(p.parse ());


    p.reset ();
    p.feed ("ALTER TABLE users ADD Id CHECK(Id in db)");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("ALTER TABLE users ADD Id CHECK(Id in db.users)");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("ALTER TABLE users ADD Id CHECK(Id in (SELECT *))");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("ALTER TABLE users ADD Id CHECK(Id in ())");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("ALTER TABLE users ADD Id CHECK(Id in (1))");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("ALTER TABLE users ADD Id CHECK(Id in (1,2))");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("ALTER TABLE users ADD Id CHECK(Id in (1,2,3))");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("ALTER TABLE users ADD Id CHECK(Id in (1,:2,'3',x'4'))");
    REQUIRE_NOTHROW(p.parse ());




    p.reset ();
    p.feed ("ALTER TABLE users ADD Id CHECK(Id not in ())");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("ALTER TABLE users ADD Id CHECK(Id NOT in (1))");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("ALTER TABLE users ADD Id CHECK(Id Not in (1,2))");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("ALTER TABLE users ADD Id CHECK(Id noT in (1,2,3))");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("ALTER TABLE users ADD Id CHECK(Id NoT in (1,:2,'3',x'4'))");
    REQUIRE_NOTHROW(p.parse ());




    p.reset ();
    p.feed ("ALTER TABLE users ADD Id CHECK((SELECT *))");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("ALTER TABLE users ADD Id CHECK(exists (SELECT *))");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("ALTER TABLE users ADD Id CHECK(not exists (SELECT *))");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("ALTER TABLE users ADD Id CHECK(case when 1 then '1' end)");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("ALTER TABLE users ADD Id CHECK(case get_val(:1 + 1) when 1 then '1' end)");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("ALTER TABLE users ADD Id CHECK(case :1 when 1 then '1' end)");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("ALTER TABLE users ADD Id CHECK(case :1 when 1 then '1' else '2' end)");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("ALTER TABLE users ADD Id CHECK(case :1 when +1 then '1' WHEN -1 THEN '-1' else '2' end)");
    REQUIRE_NOTHROW(p.parse ());

}

TEST_CASE( "statements/alter", "Alter statements" ) {
    Parser p;
    p.feed ("ALTER TABLE users RENAME TO user");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("ALTER TABLE db.users RENAME TO user");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("ALTER TABLE db.users ADD Id Integer");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("ALTER TABLE db.users ADD COLUMN Id Integer");
    REQUIRE_NOTHROW(p.parse ());

}

TEST_CASE( "constraints/column", "Column constraints" ) {
    Parser p;
    p.feed ("ALTER TABLE users ADD COLUMN Id Integer PRIMARY KEY");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("ALTER TABLE users ADD COLUMN Id Integer CONSTRAINT PK PRIMARY KEY");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("ALTER TABLE users ADD COLUMN Id Integer CONSTRAINT 'PK' PRIMARY KEY");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("ALTER TABLE users ADD COLUMN Id Integer CONSTRAINT [PK] PRIMARY KEY");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("ALTER TABLE users ADD COLUMN Id Integer CONSTRAINT `PK` PRIMARY KEY");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("ALTER TABLE users ADD COLUMN Id Integer CONSTRAINT `PK` PRIMARY KEY NOT NULL UNIQUE");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("ALTER TABLE users ADD COLUMN Id Integer CONSTRAINT \"PK\" DEFAULT 1");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("ALTER TABLE users ADD COLUMN Id Integer CONSTRAINT \"PK\" DEFAULT -1");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("ALTER TABLE users ADD COLUMN Id Integer CONSTRAINT \"PK\" DEFAULT +1");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("ALTER TABLE users ADD COLUMN Id Integer CONSTRAINT DEF DEFAULT 'a'");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("ALTER TABLE users ADD COLUMN Id Integer CONSTRAINT DEF DEFAULT CURRENT_DATE");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("ALTER TABLE users ADD COLUMN Id Integer CONSTRAINT DEF DEFAULT (1+2+3+4+5)");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("ALTER TABLE users ADD COLUMN Id Integer CONSTRAINT DEF COLLATE GERMAN");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("ALTER TABLE users ADD COLUMN Id Integer DEFAULT (1+2+3+4+5)");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("ALTER TABLE users ADD COLUMN Id Integer COLLATE GERMAN");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("ALTER TABLE employee ADD COLUMN Id Integer REFERENCES user");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("ALTER TABLE employee ADD COLUMN Id Integer REFERENCES user(id)");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("ALTER TABLE employee ADD COLUMN Id Integer REFERENCES user(id,name)");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("ALTER TABLE employee ADD COLUMN Id Integer REFERENCES user MATCH id");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("ALTER TABLE employee ADD COLUMN Id Integer REFERENCES user MATCH id MATCH name");
    REQUIRE_NOTHROW(p.parse ());



    p.reset ();
    p.feed ("ALTER TABLE employee ADD COLUMN Id Integer REFERENCES user ON DELETE SET NULL");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("ALTER TABLE employee ADD COLUMN Id Integer REFERENCES user ON DELETE SET DEFAULT");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("ALTER TABLE employee ADD COLUMN Id Integer REFERENCES user ON DELETE CASCADE");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("ALTER TABLE employee ADD COLUMN Id Integer REFERENCES user ON DELETE RESTRICT");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("ALTER TABLE employee ADD COLUMN Id Integer REFERENCES user ON DELETE NO ACTION");
    REQUIRE_NOTHROW(p.parse ());


    p.reset ();
    p.feed ("ALTER TABLE employee ADD COLUMN Id Integer REFERENCES user ON DELETE NO ACTION MATCH id ON UPDATE SET NULL MATCH name");
    REQUIRE_NOTHROW(p.parse ());


    p.reset ();
    p.feed ("ALTER TABLE employee ADD COLUMN Id Integer REFERENCES user ON DELETE SET DEFAULT DEFERRABLE");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("ALTER TABLE employee ADD COLUMN Id Integer REFERENCES user NOT DEFERRABLE");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("ALTER TABLE employee ADD COLUMN Id Integer REFERENCES user NOT DEFERRABLE INITIALLY DEFERRED");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("ALTER TABLE employee ADD COLUMN Id Integer REFERENCES user DEFERRABLE INITIALLY IMMEDIATE");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("ALTER TABLE employee ADD COLUMN Id Integer REFERENCES user ON DELETE NO ACTION MATCH id ON UPDATE SET NULL MATCH name NOT DEFERRABLE INITIALLY DEFERRED");
    REQUIRE_NOTHROW(p.parse ());
}

TEST_CASE( "statements/analyze", "Analyze statements" ) {
    Parser p;
    p.feed ("ANALYZE");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("ANALYZE sqlite_master");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("ANALYZE db.sqlite_master");
    REQUIRE_NOTHROW(p.parse ());
}


TEST_CASE( "statements/attach", "Attach statements" ) {
    Parser p;
    p.feed ("ATTACH 'test.db' AS test");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("ATTACH DATABASE 'test.db' AS test");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("ATTACH DATABASE 'test' + '.' + 'db' AS [test]");
    REQUIRE_NOTHROW(p.parse ());
}

TEST_CASE( "statements/begin", "Begin statements" ) {
    Parser p;
    p.feed ("BEGIN");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("BEGIN TRANSACTION");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("BEGIN DEFERRED TRANSACTION");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("BEGIN IMMEDIATE TRANSACTION");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("BEGIN EXCLUSIVE TRANSACTION");
    REQUIRE_NOTHROW(p.parse ());
}

TEST_CASE( "statements/commit", "Commit statements" ) {
    Parser p;
    p.feed ("COMMIT");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("COMMIT TRANSACTION");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("END");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("END TRANSACTION");
    REQUIRE_NOTHROW(p.parse ());
}

TEST_CASE( "statements/rollback", "Rollback statements" ) {
    Parser p;
    p.feed ("ROLLBACK");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("ROLLBACK TRANSACTION");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("ROLLBACK TRANSACTION TO sp");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("ROLLBACK TRANSACTION TO SAVEPOINT sp");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("ROLLBACK TRANSACTION TO SAVEPOINT savepoint");
    REQUIRE_NOTHROW(p.parse ());
}

TEST_CASE( "statements/create", "Create index" ) {
    Parser p;
    p.feed ("CREATE INDEX i ON user(id)");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("CREATE INDEX i ON user(id, name)");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("CREATE UNIQUE INDEX i ON user(id, name)");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("CREATE UNIQUE INDEX IF NOT EXISTS i ON user(id, name)");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("CREATE INDEX IF NOT EXISTS i ON user(id, name)");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("CREATE INDEX IF NOT EXISTS db.userindex ON user(id, name)");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("CREATE INDEX IF NOT EXISTS db.userindex ON user(id, name) WHERE 1 == 1");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("CREATE UNIQUE INDEX IF NOT EXISTS i ON user(id ASC, name DESC)");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("CREATE UNIQUE INDEX IF NOT EXISTS i ON user(id COLLATE German ASC, name DESC)");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("CREATE UNIQUE INDEX IF NOT EXISTS i ON user(id COLLATE 'German' ASC, name DESC)");
    REQUIRE_NOTHROW(p.parse ());



    p.reset ();
    p.feed ("CREATE TABLE user AS SELECT *");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("CREATE TEMP TABLE user AS SELECT *");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("CREATE TEMPORARY TABLE user AS SELECT *");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("CREATE TEMPORARY TABLE db.user AS SELECT *");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("CREATE TEMPORARY TABLE IF NOT EXISTS db.user AS SELECT *");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("CREATE TEMP TABLE user (Id INTEGER, Name String)");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("CREATE TEMP TABLE user (Id INTEGER PRIMARY KEY AUTOINCREMENT, Name String);");
    REQUIRE_NOTHROW(p.parse ());

    std::stringstream ss;
    ss << "CREATE TEMP TABLE user (Id INTEGER PRIMARY KEY AUTOINCREMENT, Name String\n";
    ss << " ,CONSTRAINT name_unique UNIQUE(name))\n";

    p.reset ();
    p.feed (ss);
    REQUIRE_NOTHROW(p.parse ());

    std::stringstream ss1;
    ss1 << "CREATE TABLE user (Id INTEGER PRIMARY KEY AUTOINCREMENT, Name String\n";
    ss1 << " ,CONSTRAINT name_unique UNIQUE(name)\n";
    ss1 << " ,CHECK(id > 0));\n";


    p.reset ();
    p.feed (ss1);
    REQUIRE_NOTHROW(p.parse ());

    std::stringstream ss2;
    ss2 << "CREATE TABLE user (Id INTEGER PRIMARY KEY ON CONFLICT ROLLBACK AUTOINCREMENT";
    ss2 << " ,Name String DEFAULT NULL\n";
    ss2 << " ,CONSTRAINT name_unique UNIQUE(name)\n";
    ss2 << " ,CHECK(id > 0)) WITHOUT ROWID;\n";


    p.reset ();
    p.feed (ss2);
    REQUIRE_NOTHROW(p.parse ());


    p.reset ();
    p.feed ("CREATE VIEW user AS SELECT *");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("CREATE TEMPORARY VIEW master.user AS SELECT *");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("CREATE TEMPORARY VIEW IF NOT EXISTS master.user AS SELECT *");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("CREATE VIRTUAL TABLE foo USING perl ");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("CREATE VIRTUAL TABLE master.foo USING perl ");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("CREATE VIRTUAL TABLE foo USING perl (\"MyVirtualTable\", foo, bar);");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("CREATE VIRTUAL TABLE IF NOT EXISTS foo USING perl (\"MyVirtualTable\", foo, bar);");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("CREATE TRIGGER t delete on User BEGIN delete from user END;");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("CREATE TRIGGER master.user delete on User BEGIN delete from user END;");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("CREATE TEMP TRIGGER master.user delete on User BEGIN delete from user END;");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("CREATE TEMPORARY TRIGGER master.user delete on User BEGIN delete from user END;");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("CREATE TEMPORARY TRIGGER if not exists master.user delete on User BEGIN delete from user END;");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("CREATE TEMPORARY TRIGGER if not exists master.user BEFORE delete on User BEGIN delete from user END;");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("CREATE TEMPORARY TRIGGER if not exists master.user AFTER delete on User BEGIN delete from user END;");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("CREATE TEMPORARY TRIGGER if not exists master.user INSTEAD OF delete on User BEGIN delete from user END;");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("CREATE TEMPORARY TRIGGER if not exists master.user INSTEAD OF INSERT on User BEGIN delete from user END;");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("CREATE TEMPORARY TRIGGER if not exists master.user INSTEAD OF UPDATE OF Name on User BEGIN delete from user END;");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("CREATE TEMPORARY TRIGGER if not exists master.user INSTEAD OF UPDATE OF Name, Id on User BEGIN delete from user END;");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("CREATE TEMPORARY TRIGGER if not exists master.user INSTEAD OF UPDATE OF Name, Id, Description on User BEGIN delete from user END;");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("CREATE TEMPORARY TRIGGER if not exists master.user INSTEAD OF UPDATE OF Name, Id, Description on User For each row BEGIN delete from user END;");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("CREATE TEMPORARY TRIGGER if not exists master.user INSTEAD OF UPDATE OF Name, Id, Description on User For each row WHEN 1 == 1 and 2 == 2 BEGIN delete from user END;");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("CREATE TEMPORARY TRIGGER if not exists master.user INSTEAD OF UPDATE OF Name, Id, Description on User For each row WHEN 1 == 1 and 2 == 2 BEGIN delete from user; update or abort user set Id = 0 where Id > 0 END;");
    REQUIRE_NOTHROW(p.parse ());
}


TEST_CASE( "statements/update", "Update statements" ) {
    Parser p;
    p.feed ("UPDATE User SET name = 'admin'");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("UPDATE User SET name = 'admin';");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("UPDATE OR ROLLBACK User SET name = 'admin';");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("UPDATE OR ABORT User SET name = 'admin';");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("UPDATE OR REPLACE User SET name = 'admin';");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("UPDATE OR FAIL User SET name = 'admin';");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("UPDATE OR IGNORE User SET name = 'admin';");
    REQUIRE_NOTHROW(p.parse ());


    p.reset ();
    p.feed ("UPDATE OR ROLLBACK master.User SET name = 'admin';");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("UPDATE OR ROLLBACK master.User indexed BY user_idx SET name = 'admin';");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("UPDATE OR ROLLBACK master.User not indexed SET name = 'admin';");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("UPDATE OR ROLLBACK master.User not indexed SET name = 'admin', id = 1;");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("UPDATE OR ROLLBACK master.User not indexed SET name = 'admin', id = 1 WHERE id <= 0;");
    REQUIRE_NOTHROW(p.parse ());
}

TEST_CASE( "statements/delete", "Delete statements" ) {
    Parser p;
    p.feed ("Delete from User Where Id > 500;");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("Delete from User Where Id > 500");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("Delete from master.User Where Id > 500");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("Delete from master.User Indexed by idx_user Where Id > 500");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("Delete from master.User NOT INDEXED Where Id > 500");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("Delete from master.User;");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("Delete from User;");
    REQUIRE_NOTHROW(p.parse ());
}

TEST_CASE( "statements/select", "SELECT statements" ) {
    Parser p;
    p.feed ("SELECT *;");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("SELECT DISTINCT *");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("SELECT ALL *");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("SELECT ALL Id");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("SELECT ALL Id, Name");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("SELECT ALL Id, User.Name");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("SELECT ALL User.Id, User.Name");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("SELECT ALL User.Id, Orga.*");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("SELECT ALL User.Id, Orga.* FROM User");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("SELECT ALL User.Id, Orga.* FROM User, Orga");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("SELECT ALL User.Id, O.* FROM User, Orga as O");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("SELECT ALL User.Id, O.* FROM User, Orga as O");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("SELECT ALL User.Id, O.* FROM User, Orga as O ON 1 = 1");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("SELECT ALL User.Id, O.* FROM User, Orga as O USING(Id)");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("SELECT ALL User.Id, O.* FROM User JOIN Orga ON O.Id = Orga.Id");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("SELECT ALL User.Id, O.* FROM User LEFT JOIN Orga ON O.Id = Orga.Id");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("SELECT ALL User.Id, O.* FROM User LEFT OUTER JOIN Orga ON O.Id = Orga.Id");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("SELECT ALL User.Id, O.* FROM User INNER JOIN Orga ON O.Id = Orga.Id");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("SELECT ALL User.Id, O.* FROM User NATURAL JOIN Orga ON O.Id = Orga.Id");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("SELECT ALL User.Id, O.* FROM User NATURAL LEFT JOIN Orga ON O.Id = Orga.Id");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("SELECT ALL User.Id, O.* FROM User NATURAL LEFT OUTER JOIN Orga ON O.Id = Orga.Id");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("SELECT ALL User.Id, O.* FROM User NATURAL INNER JOIN Orga ON O.Id = Orga.Id");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("SELECT ALL User.Id, O.* FROM User NATURAL INNER JOIN Orga using(id);");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("SELECT ALL User.Id, O.* FROM User NATURAL INNER JOIN Orga using(id) WHERE O.Id > 5");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("SELECT ALL User.Id, O.* FROM User NATURAL INNER JOIN Orga using(id) WHERE O.Id > 5 AND User.Name like 'TEST'");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("SELECT ALL User.Id, O.* FROM User NATURAL INNER JOIN Orga using(id) WHERE O.Id > 5 AND User.Name like 'TEST' or User.name like '%TEST'");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("SELECT ALL User.Id, O.* FROM User NATURAL INNER JOIN Orga using(id) WHERE O.Id > 5 AND User.Name like 'TEST' or User.name like '%TEST' GROUP BY User.Id");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("SELECT ALL User.Id, O.* FROM User NATURAL INNER JOIN Orga using(id) WHERE O.Id > 5 GROUP BY ID HAVING Id > 7");
    REQUIRE_NOTHROW(p.parse ());



    p.printSyntaxTree (std::cout);
}

int main (int argc, char* const argv[]) {
     exit(Catch::Main( argc, argv ));
     return 0;
}
