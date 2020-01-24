#include "Texture.h"

#include <FreeImage.h>
#include <windows.h>
#include <chrono>
#include <iostream>

int Texture::numberOfTextures = 0;

Texture::Texture(const int textureId_, const GLuint textureNumber_)
    : textureId(textureId_), textureNumber(textureNumber_) {
}

Texture::Texture(const char *fileName)
    : textureNumber(numberOfTextures++), textureId(loadTexture(fileName)) {
}

Texture::~Texture() {
    glDeleteTextures(1, &textureId);
    std::cout << "\nRemoved texture: " << textureId << std::endl;
}

GLuint Texture::loadTexture(const char *fileName) {  //Loads the texture in memory
    const auto start = std::chrono::high_resolution_clock::now();
    unsigned int width, height;
    unsigned char *map = readTextureFromFile(fileName, width, height);
    if (!map) {
        std::cout << "Error reading image of texture number: " << textureNumber << " from file: " << fileName << std::endl;
        exit(-1);
    }

    GLuint texId;
    glGenTextures(1, &texId);             //Reserve space for the new texture
    glBindTexture(GL_TEXTURE_2D, texId);  //We bind the new texture
    //We load the texture in the GPU
    glTexImage2D(GL_TEXTURE_2D,
                 0 /*level of detail, 0 is the mipmap with more details*/,
                 GL_RGBA8, width, height, 0 /*Border*/, GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid *)map);

    delete[] map;  // Removes the texture from the CPU

    glGenerateMipmap(GL_TEXTURE_2D);  //Uses the selected mipmap level to generate all the others.

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);  //Filter when the sampling is not enough
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    const auto end = std::chrono::high_resolution_clock::now();
    const auto timeDiff = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
    std::cout << "Texture: " << fileName << " loaded in: " << timeDiff << " nanoSeconds = " << 1e-9f * static_cast<float>(timeDiff) << " seconds." << std::endl;

    return texId;
}

unsigned char *Texture::readTextureFromFile(const char *fileName, unsigned int &width, unsigned int &height) {
    FreeImage_Initialise(TRUE);

    FREE_IMAGE_FORMAT format = FreeImage_GetFileType(fileName, 0);
    if (format == FIF_UNKNOWN) {
        format = FreeImage_GetFIFFromFilename(fileName);
    }
    if ((format == FIF_UNKNOWN) || !FreeImage_FIFSupportsReading(format)) {
        std::cout << "Error reading image from file: " << fileName << ": unsupported format." << std::endl;
        return NULL;
    }

    FIBITMAP *img = FreeImage_Load(format, fileName);
    if (img == NULL) {
        std::cout << "Error reading image from file: " << fileName << ": Not capable of reading the image." << std::endl;
        return NULL;
    }

    FIBITMAP *tempImg = img;
    img = FreeImage_ConvertTo32Bits(img);
    FreeImage_Unload(tempImg);

    width = FreeImage_GetWidth(img);
    height = FreeImage_GetHeight(img);

    // BGRA a RGBA
    unsigned char *map = new unsigned char[4 * width * height];
    char *buff = (char *)FreeImage_GetBits(img);

    for (unsigned int j = 0; j < width * height; j++) {
        map[j * 4 + 0] = buff[j * 4 + 2];
        map[j * 4 + 1] = buff[j * 4 + 1];
        map[j * 4 + 2] = buff[j * 4 + 0];
        map[j * 4 + 3] = buff[j * 4 + 3];
    }

    FreeImage_Unload(img);
    FreeImage_DeInitialise();
    if (!map) {
        std::cout << "Error reading image from file: " << fileName << ": Converting to bytes." << std::endl;
        exit(-1);
    }

    return map;
}

void Texture::sendToShaderProgram(const unsigned int uniformId) const {
    glActiveTexture(GL_TEXTURE0 + textureNumber);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glUniform1i(uniformId, textureNumber);
}

void Texture::applyAnisotropicFilter() const {
    int extensionNumber = 0;
    glGetIntegerv(GL_NUM_EXTENSIONS, &extensionNumber);  //Total number of extensions
    bool found = false;
    //Workaround around of a GLEW bug to find extensions in some graphics cards
    while ((0 < --extensionNumber) && !found) {  //Loop and checks all the available extensions
        found = strcmp(((char *)glGetStringi(GL_EXTENSIONS, extensionNumber - 1)), "GL_EXT_texture_filter_anisotropic");
    }
    if (found) {
        glBindTexture(GL_TEXTURE_2D, textureId);  // Activates the texture
        float maxAnisotropicFilter;
        glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAnisotropicFilter);  //Gets the maximum anistropic filter possible
        std::cout << "\n Texture " << textureNumber << ": applying anisotropic filter of: " << maxAnisotropicFilter << std::endl;
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, maxAnisotropicFilter);  //Sets the anisotropic level to the maximum possible
    } else {
        std::cout << "\n Texture " << textureNumber << ": anisotropic filter is not supported." << std::endl;
    }
}