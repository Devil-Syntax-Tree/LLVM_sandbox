add_library (kaleidoscope_codegen)

target_link_libraries(kaleidoscope_codegen PRIVATE
    kaleidoscope_base
)

set(LLVM_COMPONENTS
    core
    executionengine
    mcjit
    bitwriter
    instcombine
    scalaropts
    # simplifycfg
    # ...otros componentes necesarios
)

llvm_map_components_to_libnames(llvm_libs ${LLVM_COMPONENTS})

link_directories(${LLVM_LIBRARY_DIRS})

target_link_libraries(kaleidoscope_codegen PRIVATE ${llvm_libs})

target_sources (kaleidoscope_codegen PRIVATE
    Codegen.cpp
    Codegen.hpp
    KaleidoscopeJIT.hpp
)
