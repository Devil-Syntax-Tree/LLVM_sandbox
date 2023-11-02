#include "Reader.hpp"
#include <iostream>

namespace kaleidoscope {

Reader::Reader() : readerType(Type::CONSOLE) {}

Reader::Reader(const std::string& path) : readerType(Type::FILE), file(path) {
    if (!file.is_open()) {
        std::cerr << "Error al abrir el archivo: " << path << std::endl;
    }
}

char Reader::nextCharacter() {
    switch(readerType) {
        case Type::CONSOLE:
            return std::getchar();
        case Type::FILE:
            if (file.is_open() && !file.eof()) {
                char c;
                file.get(c);
                return c;
            }
            return '\0'; // Retorna null si el archivo no está abierto o hemos llegado al final
    }
    return '\0'; // Valor por defecto
}

} // namespace kaleidoscope
