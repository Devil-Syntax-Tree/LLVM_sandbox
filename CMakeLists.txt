cmake_minimum_required (
    VERSION 3.22
)

project (
    LLVM_sandbox
    LANGUAGES C CXX)

find_package (LLVM 17 CONFIG REQUIRED)

add_subdirectory (sandbox)
# add_subdirectory (kaleidoscope)
