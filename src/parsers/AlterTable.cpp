#include "../../include/parsers/AlterTable.hpp"

namespace Sql { namespace Parsers {
using namespace Exceptions;

void AlterTable::parse () DEF_THROW {
  push(ALTER_TABLE);

  expect("ALTER");
  expect("TABLE");
  add(NODE_VALUE, expect(VALUE));

  if (is(DOT)) {
    consume();
    add(NODE_VALUE, expect(VALUE));
  }

  if (is("RENAME")) {
    push(RENAME_TO);
    consume();
    expect("TO");
    add(NODE_VALUE, expect(VALUE));
    pop();
  } else if (is("ADD")) {
    if (is("COLUMN")) {
      consume();
    }
    expect(VALUE);
  } else {
    throw UnexpectedTokenException(
          peek().value,
          {"RENAME", "ADD"},
          peek().line);
  }

  pop();
}

} }
