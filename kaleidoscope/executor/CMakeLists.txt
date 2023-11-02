add_library (kaleidoscope_executor)

target_link_libraries(kaleidoscope_executor PRIVATE
    kaleidoscope_base
    kaleidoscope_reader
    kaleidoscope_lexer
    kaleidoscope_parser
    kaleidoscope_codegen
)

llvm_map_components_to_libnames(llvm_libs all)

target_link_libraries(kaleidoscope_executor ${llvm_libs})

target_sources (kaleidoscope_executor PRIVATE
    Executor.cpp
    Executor.hpp
)
