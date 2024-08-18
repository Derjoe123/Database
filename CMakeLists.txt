project(DB)

set(CMAKE_C_STANDARD 11)

set(CMAKE_C_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror -pedantic")

set(SOURCE_FILES 
  src/main.c
  src/Lexer.c
  src/Token.c
  src/Errors.c)

add_executable(DB ${SOURCE_FILES})
