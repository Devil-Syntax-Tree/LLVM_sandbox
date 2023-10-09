add_library (kaleidoscope_lexer)

target_link_libraries(kaleidoscope_lexer PRIVATE
    kaleidoscope_base
)

target_sources (kaleidoscope_lexer PRIVATE
    Lexer.hpp
    Lexer.cpp
    Token.hpp
    Token.cpp
)
