add_executable (sandbox_executable)

set_target_properties (sandbox_executable PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
    OUTPUT_NAME sandbox
)

if (ipo_lto_supported)
    set_target_properties (sandbox_executable PROPERTIES
        INTERPROCEDURAL_OPTIMIZATION TRUE
    )
endif ()

llvm_map_components_to_libnames (llvm_sandbox_required_libs core)

target_link_libraries (sandbox_executable PRIVATE ${llvm_sandbox_required_libs})
target_include_directories (sandbox_executable PRIVATE ${LLVM_INCLUDE_DIRS})
target_compile_definitions (sandbox_executable PRIVATE ${LLVM_DEFINITIONS_LIST})

target_sources (sandbox_executable PRIVATE hello_llvm.cpp)
