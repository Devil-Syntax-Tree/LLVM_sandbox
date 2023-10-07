add_library (kaleidoscope_reader)

target_link_libraries(kaleidoscope_reader PRIVATE
    kaleidoscope_base
)

target_sources (
    kaleidoscope_reader PRIVATE
    Reader.hpp
    ConsoleReader.hpp
    ConsoleReader.cpp
    FileReader.hpp
    FileReader.cpp
)
