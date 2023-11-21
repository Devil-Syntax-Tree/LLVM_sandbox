add_library (kaleidoscope_parser)

target_link_libraries(kaleidoscope_parser PRIVATE
    kaleidoscope_base
    LLVMCore
)

target_sources (kaleidoscope_parser PRIVATE
                AST.cpp
                AST.hpp
                Parser.cpp
                Parser.hpp
                Precedence.cpp
                Precedence.hpp
)
