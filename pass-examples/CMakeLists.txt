add_library (
    pass_examples_base INTERFACE
)
set_target_properties (
    pass_examples_base PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)
target_include_directories (
    pass_examples_base
    INTERFACE ${LLVM_INCLUDE_DIRS}
)

add_subdirectory(HelloWorld)
