target_sources (sandbox_executable PRIVATE main.cpp)

add_subdirectory (lexer)
add_subdirectory (parser)
add_subdirectory (codegen)