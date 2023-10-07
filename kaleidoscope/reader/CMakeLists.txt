add_library (kaleidoscope_reader)

set_target_properties (kaleidoscope_reader PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)

target_sources (
    kaleidoscope_executable PRIVATE
    Reader.hpp
    ConsoleReader.hpp
    ConsoleReader.cpp
    FileReader.hpp
    FileReader.cpp
)
