#include <iostream>
#include <fstream>
#include <string>
#include "FileReader.hpp" // Incluye la declaración de la clase Reader

kaleidoscope::FileReader::FileReader(const std::string& filename) : filename_(filename) {
    file_.open(filename_);
    if (!file_) {
        std::cerr << "No se pudo abrir el archivo " << filename_ << std::endl;
    }
}

kaleidoscope::FileReader::~FileReader() {
    if (file_.is_open()) {
        file_.close();
    }
}

void kaleidoscope::FileReader::readFile() {
    if (!file_.is_open()) {
        std::cerr << "El archivo no está abierto." << std::endl;
        return;
    }

    std::string line;
    while (std::getline(file_, line)) {
        std::cout << line << std::endl;
    }
}

char kaleidoscope::FileReader::nextChar() {
    if (!file_.is_open()) {
        std::cerr << "El archivo no está abierto." << std::endl;
        throw std::runtime_error("File not found");
    }

    char curr_char;
    if (file_.get(curr_char)) {
        return curr_char;
    }
    else {
        return EOF;
    }

    return '\0';
}

char kaleidoscope::FileReader::peekNextChar() {
    if (!file_.is_open()) {
        std::cerr << "El archivo no está abierto." << std::endl;
        throw std::runtime_error("File not found");
    }

    std::streampos currentPos = file_.tellg();  // Guarda la posición actual
    char nextChar;
    file_.seekg(0, std::ios::cur);  // Mueve el puntero una posición hacia adelante
    if (file_.get(nextChar)) {
        file_.seekg(currentPos);  // Regresa el puntero a su posición original
        return nextChar;
    } else {
        file_.seekg(currentPos);  // Regresa el puntero a su posición original
        return EOF;
    }
}