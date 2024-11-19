project(DB)

set(CMAKE_C_STANDARD 11)

set(CMAKE_C_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror -pedantic")

set(PARSER_SOURCE_FILES 
  src/Parser/ParserMain.c
  src/Parser/Lexer.c
  src/Parser/Token.c
  src/Parser/Parser.c
  src/Parser/SymbolTree.c
  src/Common/Errors.c)

add_executable(DBParser ${PARSER_SOURCE_FILES})
