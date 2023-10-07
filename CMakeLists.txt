cmake_minimum_required (
    VERSION 3.22
)

project (
    LLVM_sandbox
    LANGUAGES C CXX)

include(CheckIPOSupported)
check_ipo_supported (RESULT ipo_lto_supported OUTPUT ipo_output LANGUAGES CXX)
if (NOT ipo_lto_supported)
    message("Interprocedural optimization is not suported...")
    message(ipo_output)
endif ()


find_package (LLVM 17 CONFIG REQUIRED)
message (STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message (STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")
separate_arguments (LLVM_DEFINITIONS_LIST NATIVE_COMMAND ${LLVM_DEFINITIONS})

add_subdirectory (sandbox)
add_subdirectory (kaleidoscope)
