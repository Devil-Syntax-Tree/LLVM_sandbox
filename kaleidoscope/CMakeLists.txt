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

llvm_map_components_to_libnames (llvm_kaleidoscope_required_libs core)

target_link_libraries (kaleidoscope_executable PRIVATE ${llvm_kaleidoscope_required_libs})
target_include_directories (kaleidoscope_executable PRIVATE ${LLVM_INCLUDE_DIRS})
target_compile_definitions (kaleidoscope_executable PRIVATE ${LLVM_DEFINITIONS_LIST})

target_sources (kaleidoscope_executable 
    PRIVATE main.cpp
)

add_subdirectory(reader)
add_subdirectory(lexer)
