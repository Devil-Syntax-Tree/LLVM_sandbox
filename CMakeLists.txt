cmake_minimum_required (
    VERSION 3.22
)

project (
    LLVM_sandbox
    LANGUAGES C CXX
)

# IPO
include (CheckIPOSupported)
check_ipo_supported (
    RESULT ipo_lto_supported
    OUTPUT ipo_output
    LANGUAGES C CXX
)
if (NOT ipo_lto_supported)
    message ("Interprocedural optimization is not suported...")
    message (ipo_output)
endif ()

# LLVM
find_package (LLVM 17 CONFIG REQUIRED)
message (STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message (STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")
message (
    "LLVM STATUS:
    Definitions ${LLVM_DEFINITIONS}
    Includes ${LLVM_INCLUDE_DIRS}
    Libraries ${LLVM_LIBRARY_DIRS}
    Targets ${LLVM_TARGETS_TO_BUILD}"
)
separate_arguments (LLVM_DEFINITIONS_LIST NATIVE_COMMAND ${LLVM_DEFINITIONS})

add_subdirectory (sandbox)
add_subdirectory (kaleidoscope)
add_subdirectory (pass-examples)
