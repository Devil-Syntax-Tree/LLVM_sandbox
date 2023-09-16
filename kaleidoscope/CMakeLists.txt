add_executable(kaleidoscope)
set_target_properties (kaleidoscope PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)
target_sources(kaleidoscope PRIVATE Lexer.hpp Token.hpp main.cpp)