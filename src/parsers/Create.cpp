#include "../../include/parsers/Create.hpp"

namespace Sql { namespace Parsers {
using namespace Exceptions;

void Create::parse () DEF_THROW {
    push("create");
    expect("CREATE");

    if (is(oneOf("UNIQUE", "INDEX"))) {
        parseCreateIndex ();
    }

    else if (is(oneOf("TEMP", "TEMPORARY"))) {
        if (is(1, "TABLE")) {
            parseCreateTable ();
        }

        else if (is(1, "VIEW")) {
            parseCreateView ();
        }

        else if (is(1, "TRIGGER")) {
            parseCreateTrigger ();
        }

        else {
            expect(oneOf("TABLE", "VIEW", "TRIGGER"));
        }
    }

    else if (is("TABLE")) {
        parseCreateTable ();
    }

    else if (is("VIEW")) {
        parseCreateView ();
    }

    else if (is("VIRTUAL")) {
        parseCreateVirtualTable ();
    }

    else if (is("TRIGGER")) {
        parseCreateTrigger ();
    }

    else {
        expect(oneOf("UNIQUE", "INDEX", "TEMP", "TEMPORARY", "TABLE", "VIEW", "VIRTUAL"));
    }

    pop();
}

void Create::parseCreateTrigger () DEF_THROW {
    push("trigger");

    if (is(oneOf("TEMP", "TEMPORARY"))) {
        add("modifier", next ());
    }

    expect("TRIGGER");
    if (is("IF")) {
        push("if");
        expect("IF");
        add("modifier", expect("NOT"));
        add("condition", expect("EXISTS"));
        pop ();
    }

    if (has(2) && isName (0) && is(1, DOT) && isName (2)) {
        add("database-name", expectName ());
        expect(DOT);
        add("trigger-name", expectName ());
    } else {
        add("trigger-name", expectName ());
    }

    if (is(oneOf("BEFORE", "AFTER"))) {
        add("modifier", next());
    }

    if (is("INSTEAD")) {
        add("modifier", next());
        expect ("OF");
    }


    if (is("DELETE")) {
        add("on", next());
        expect("ON");
    }

    else if (is("INSERT")) {
        add("on", next());
        expect("ON");
    }

    else if (is("UPDATE")) {
        add("on", next());
        if (!is("ON")) {
            expect("OF");
            push("of");
            parseNameList ("column-name");
            pop ();
        }
        expect("ON");
    }

    else {
        expect(oneOf("DELETE", "INSERT", "UPDATE"));
    }

    add("table-name", expectName ());

    if (is("FOR")) {
        consume ();
        push ("for");
        add("modifier", expect("EACH"));
        add("entity", expect("ROW"));
        pop ();
    }

    if (is("WHEN")) {
        consume ();
        push("when");
        getParser ("EXPRESSION").parse ();
        pop ();
    }

    expect ("BEGIN");
    parseTriggerActionList ();
    expect ("END");

    pop ();
}

void Create::parseTriggerActionList () DEF_THROW {
    while (hasNext ()) {
        if (is("UPDATE")) {
            getParser ("UPDATE").parse ();
        }

        else if (is("INSERT")) {
            getParser ("INSERT").parse ();
        }

        else if (is("DELETE")) {
            getParser ("DELETE").parse ();
        }

        else if (is("SELECT")) {
            getParser ("SELECT").parse ();
        }

        else {
            expect(oneOf("UPDATE", "INSERT", "DELETE", "SELECT"));
        }

        if (hasNext () && is(SEMI)) {
            consume();
            continue;
        } else {
            break;
        }
    }
}

void Create::parseCreateVirtualTable () DEF_THROW {
    push("virtual-table");

    expect("VIRTUAL");
    expect("TABLE");
    if (is("IF")) {
        push("if");
        expect("IF");
        add("modifier", expect("NOT"));
        add("condition", expect("EXISTS"));
        pop ();
    }

    if (has(2) && isName (0) && is(1, DOT) && isName (2)) {
        add("database-name", expectName ());
        expect(DOT);
        add("table-name", expectName ());
    } else {
        add("table-name", expectName ());
    }

    expect("USING");
    add("module-name", expectName ());
    if (hasNext () && is(LP)) {
        consume ();
        parseNameList ("module-argument");
        expect(RP);
    }

    pop ();
}

void Create::parseCreateView () DEF_THROW {
    push("view");

    if (is(oneOf("TEMP", "TEMPORARY"))) {
        add("modifier", next ());
    }

    expect("VIEW");
    if (is("IF")) {
        push("if");
        expect("IF");
        add("modifier", expect("NOT"));
        add("condition", expect("EXISTS"));
        pop ();
    }

    if (has(2) && isName (0) && is(1, DOT) && isName (2)) {
        add("database-name", expectName ());
        expect(DOT);
        add("table-name", expectName ());
    } else {
        add("table-name", expectName ());
    }

    expect("AS");
    push("as");
    getParser ("SELECT").parse ();
    pop ();

    pop ();
}

void Create::parseCreateTable () DEF_THROW {
    push("table");

    if (is(oneOf("TEMP", "TEMPORARY"))) {
        add("modifier", next ());
    }

    expect("TABLE");
    if (is("IF")) {
        push("if");
        expect("IF");
        add("modifier", expect("NOT"));
        add("condition", expect("EXISTS"));
        pop ();
    }

    if (has(2) && isName (0) && is(1, DOT) && isName (2)) {
        add("database-name", expectName ());
        expect(DOT);
        add("table-name", expectName ());
    } else {
        add("table-name", expectName ());
    }

    if (is(LP)) {
        consume ();
        parseCreateColumnList ();
        expect(RP);
        if (hasNext () && is("WITHOUT")) {
            consume ();
            add("without", expect("ROWID"));
        }
    } else if (is("AS")) {
        consume ();
        push("as");
        getParser ("SELECT").parse ();
        pop ();
    } else {
        throw UnexpectedTokenException(peek().value, {
                                           "(",
                                           "AS"
                                       }, peek().line);
    }

    pop ();
}

void Create::parseCreateIndex () DEF_THROW {
    push("index");

    if (is("UNIQUE")) {
        add("modifier", next ());
    }

    expect("INDEX");
    if (is("IF")) {
        expect("IF");
        expect("NOT");
        expect("EXISTS");
    }

    if (has(2) && isName (0) && is(1, DOT) && isName (2)) {
        add("database-name", expectName ());
        consume ();
        add("index-name", expectName ());
    } else {
        add("index-name", expectName ());
    }

    expect("ON");
    add("table-name", expectName ());
    expect(LP);
    parseColumnList ();
    expect(RP);

    if (hasNext () && is("WHERE")) {
        consume ();
        push("where");
        getParser ("EXPRESSION").parse ();
        pop ();
    }

    pop ();
}

void Create::parseTableConstraint () DEF_THROW {
    push("table-constraint");

    if (is("CONSTRAINT")) {
        consume ();
        add("name", expectName ());
    }

    if (is("PRIMARY")) {
        push("primary-key");
        consume ();
        expect("KEY");
        expect(LP);
        parseColumnList ();
        expect(RP);
        getParser ("CONFLICT").parse ();
        pop ();
    }

    else if (is("UNIQUE")) {
        push("unique");
        consume ();
        expect(LP);
        parseColumnList ();
        expect(RP);
        getParser ("CONFLICT").parse ();
        pop ();
    }

    else if (is("CHECK")) {
        push ("check");
        consume ();
        expect(LP);
        getParser ("EXPRESSION").parse ();
        expect(RP);
        pop ();
    }

    else if (is("FOREIGN")) {
        push("foreign");
        consume ();
        expect("KEY");
        expect(LP);
        parseNameList ("column-name");
        expect(RP);
        getParser ("REFERENCES").parse ();
        pop ();
    }

    else {
        throw UnexpectedTokenException(peek().value, {
                                           "CONSTRAINT",
                                           "PRIMARY",
                                           "UNIQUE",
                                           "CHECK",
                                           "FOREIGN"
                                       }, peek().line);
    }

    pop ();
}

void Create::parseCreateColumnList () DEF_THROW {
    while (hasNext ()) {
        if (isConstraint ()) {
            parseTableConstraint ();
        } else {
            getParser ("COLDEF").parse ();
        }
        if (hasNext () && is(COMMA)) {
            consume();
            continue;
        } else {
            break;
        }
    }
}

void Create::parseTableConstraintList () DEF_THROW {
    while (hasNext ()) {
        parseTableConstraint ();
        if (hasNext () && is(COMMA)) {
            consume();
            continue;
        } else {
            break;
        }
    }
}

void Create::parseColumnList () DEF_THROW {
    while (hasNext ()) {
        parseIndexedColumn ();
        if (hasNext () && is(COMMA)) {
            consume();
            continue;
        } else {
            break;
        }
    }
}

void Create::parseIndexedColumn () DEF_THROW {
    push("indexed-column");
    add("name", expectName ());

    if (is("COLLATE")) {
        consume ();
        add("collation-name", expectName ());
    }

    if (is(oneOf("ASC", "DESC"))) {
        add("order", next ());
    }

    pop ();
}

} }

