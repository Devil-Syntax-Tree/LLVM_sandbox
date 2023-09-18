cmake_minimum_required (
    VERSION 3.22 
)

project (
    LLVM_sandbox
    LANGUAGES CXX)

find_package (LLVM CONFIG REQUIRED)

# define executable/target
add_executable (sandbox_executable)

# set C++20 version
set_target_properties (sandbox_executable PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)

llvm_map_components_to_libnames (llvm_libs core)

target_link_libraries (sandbox_executable PRIVATE ${llvm_libs})

# add include directories. 
# it makes header files available to #include <header>
target_include_directories (sandbox_executable PRIVATE ${LLVM_INCLUDE_DIRS})

# add compile definitions/ macros to sandbox
target_compile_definitions (sandbox_executable PRIVATE ${LLVM_DEFINITIONS_LIST})

add_subdirectory (sandbox)

# kaleidoscope implementation
add_subdirectory(kaleidoscope)
