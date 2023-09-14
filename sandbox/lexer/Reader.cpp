#include <iostream>
#include <fstream>
#include <string>
#include <lexer/Reader.h> // Incluye la declaración de la clase Reader

Reader::Reader(const std::string& filename) : filename_(filename) {
    file_.open(filename_);
    if (!file_) {
        std::cerr << "No se pudo abrir el archivo " << filename_ << std::endl;
    }
}

Reader::~Reader() {
    if (file_.is_open()) {
        file_.close();
    }
}

void Reader::readFile() {
    if (!file_.is_open()) {
        std::cerr << "El archivo no está abierto." << std::endl;
        return;
    }

    std::string line;
    while (std::getline(file_, line)) {
        std::cout << line << std::endl;
    }
}

char Reader::readChar() {
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

char Reader::peekNextChar() {
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