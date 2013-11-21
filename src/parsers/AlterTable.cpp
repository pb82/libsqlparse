#include "../../include/parsers/AlterTable.hpp"

namespace Sql { namespace Parsers {
using namespace Exceptions;

void AlterTable::parse () DEF_THROW {
  push(NodeType::ALTER_TABLE);

  expect("ALTER");
  expect("TABLE");
  expect(VALUE);

  if (is(DOT)) {
    consume();
    expect(VALUE);
  }

  if (is("RENAME")) {        
    consume();
    expect("TO");
    expect(VALUE);
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
}

} }
