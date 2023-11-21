add_library (kaleidoscope_reader)

target_link_libraries(kaleidoscope_reader PRIVATE
    kaleidoscope_base
)

target_sources (
    kaleidoscope_reader PRIVATE
    Reader.hpp
    FileReader.hpp
    FileReader.cpp
    ConsoleReader.hpp
    ConsoleReader.cpp
)
