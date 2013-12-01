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
    p.feed ("alter table db.users rename to db.user");
    REQUIRE_NOTHROW(p.parse ());

    p.reset ();
    p.feed ("ALTER TABLE db.users RENAME TO db.user");
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

}


int main (int argc, char* const argv[]) {
     exit(Catch::Main( argc, argv ));
     return 0;
}
