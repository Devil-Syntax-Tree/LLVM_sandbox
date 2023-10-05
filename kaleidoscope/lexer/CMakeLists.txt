add_library (kaleidoscope_lexer)

set_target_properties (kaleidoscope_lexer PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)

target_sources (kaleidoscope_lexer PRIVATE
    Lexer.hpp
    Lexer.cpp
    Token.hpp
    Token.cpp
)
