add_executable (kaleidoscope_executable)

set_target_properties (kaleidoscope_executable PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
    OUTPUT_NAME kaleidoscope
)

if (ipo_lto_supported)
    set_target_properties (kaleidoscope_executable PROPERTIES
        INTERPROCEDURAL_OPTIMIZATION ON
    )
endif ()

target_sources (kaleidoscope_executable 
    PRIVATE main.cpp
)

add_subdirectory(reader)
add_subdirectory(lexer)

target_link_libraries(kaleidoscope_executable PRIVATE
    kaleidoscope_reader
    kaleidoscope_lexer
)
