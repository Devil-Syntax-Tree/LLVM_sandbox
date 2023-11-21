add_library (kaleidoscope_base INTERFACE)
set_target_properties (kaleidoscope_base PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)

if (ipo_lto_supported)
    set_target_properties (kaleidoscope_base PROPERTIES
        INTERPROCEDURAL_OPTIMIZATION ON
    )
endif ()

add_executable (kaleidoscope_executable)

set_target_properties (kaleidoscope_base PROPERTIES
    OUTPUT_NAME kaleidoscope
)

target_sources (kaleidoscope_executable
    PRIVATE main.cpp
)

target_link_libraries (kaleidoscope_executable PRIVATE
    kaleidoscope_base
)

add_subdirectory (reader)
add_subdirectory (lexer)
add_subdirectory (parser)
add_subdirectory (codegen)
add_subdirectory (executor)


target_link_libraries(kaleidoscope_executable PRIVATE
    kaleidoscope_reader
    kaleidoscope_lexer
    kaleidoscope_parser
    kaleidoscope_codegen
    kaleidoscope_executor
)
