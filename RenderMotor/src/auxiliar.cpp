#include <windows.h>

#include <fstream>

// Carga de texturas
#include <FreeImage.h>
#define _CRT_SECURE_DEPRECATE_MEMORY
#include <memory.h>

//////////////////////////////////////////
// Funciones auxiliares ya implementadas

// Funciones para la carga de los shaders
char *loadStringFromFile(const char *fileName, unsigned int &fileLen) {
    // Se carga el fichero
    std::ifstream file;
    file.open(fileName, std::ios::in);
    if (!file) {
        return 0;
    }

    // Se calcula la longitud del fichero
    file.seekg(0, std::ios::end);
    fileLen = unsigned int(file.tellg());
    file.seekg(std::ios::beg);

    // Se lee el fichero
    char *source = new char[fileLen + 1];

    int i = 0;
    while (file.good()) {
        source[i] = char(file.get());
        if (!file.eof()) {
            ++i;
        } else {
            fileLen = i;
        }
    }
    source[fileLen] = '\0';
    file.close();

    return source;
}