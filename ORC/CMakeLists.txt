add_executable (orc_example_executable)

set_target_properties (orc_example_executable PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
    OUTPUT_NAME orc_example
)

if (ipo_lto_supported)
    set_target_properties (orc_example_executable PROPERTIES
        INTERPROCEDURAL_OPTIMIZATION TRUE
    )
endif ()

llvm_map_components_to_libnames (llvm_orc_example_required_libs core support orcjit native X86 nativecodegen)

target_link_libraries (orc_example_executable PRIVATE ${llvm_orc_example_required_libs})
target_include_directories (orc_example_executable PRIVATE ${LLVM_INCLUDE_DIRS})
target_compile_definitions (orc_example_executable PRIVATE ${LLVM_DEFINITIONS_LIST})

export_executable_symbols (orc_example_executable)

target_sources (orc_example_executable PRIVATE
    HelloORC.cpp
)
