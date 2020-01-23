#include "Shader.h"

#include <fstream>
#include <iostream>
#include <string>

Shader::Shader(const char *vertPath, const char *fragPath, Scene *scene_) : scene(scene_) {
    unsigned int fileLen;
    char *fileString = loadStringFromFile(vertPath, fileLen);
    vertexShader = loadShader(const_cast<const GLchar **>(static_cast<GLchar **>(&fileString)), static_cast<const GLint>(fileLen), GL_VERTEX_SHADER);
    delete fileString;
    fileString = loadStringFromFile(fragPath, fileLen);
    fragShader = loadShader(const_cast<const GLchar **>(static_cast<GLchar **>(&fileString)), static_cast<const GLint>(fileLen), GL_FRAGMENT_SHADER);
    delete fileString;
    initShader();
}
Shader::Shader(const GLchar **vertString, const GLint vertStringLength, const GLchar **fragString, const GLint fragStringLength, Scene *scene_)
    : vertexShader(loadShader(vertString, vertStringLength, GL_VERTEX_SHADER)),
      fragShader(loadShader(fragString, fragStringLength, GL_FRAGMENT_SHADER)),
      scene(scene_) {
    initShader();
}

Shader::~Shader() {
    glDetachShader(program, vertexShader);
    glDetachShader(program, fragShader);
    glDeleteShader(vertexShader);
    glDeleteShader(fragShader);
    glDeleteProgram(program);
}
void Shader::addIllumination(IlluminationSet *illumination) {
    illuminations.push_back(illumination);
}
void Shader::renderShader(glm::mat4 view) const {
    glUseProgram(program);
    for (auto itIllumination = illuminations.begin(); itIllumination != illuminations.end(); ++itIllumination) {
        (*itIllumination)->renderMaterials(view);
    }
}
int Shader::loadShader(const GLchar **shaderString, const GLint stringLength, const int type) const {
    GLuint shader;
    shader = glCreateShader(type);
    glShaderSource(shader, 1 /*One string*/, shaderString, &stringLength);
    glCompileShader(shader);

    //Check and error retrieval
    GLint compiled;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
    if (!compiled) {
        GLint logLen;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLen);
        char *logString = new char[logLen];
        glGetShaderInfoLog(shader, logLen, NULL, logString);
        std::cout << "\n Error al compilar el shader: " << logString << std::endl;
        delete logString;
        glDeleteShader(shader);
        exit(-1);
    }
    return shader;
}

void Shader::initShader() {
    program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragShader);
    glBindAttribLocation(program, 0, "inPos");
    glBindAttribLocation(program, 1, "inColor");
    glBindAttribLocation(program, 2, "inNormal");
    glBindAttribLocation(program, 3, "inTexCoord");
    glBindAttribLocation(program, 4, "inTangent");

    glLinkProgram(program);

    GLint linked;
    glGetProgramiv(program, GL_LINK_STATUS, &linked);
    if (!linked) {
        GLint logLen;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLen);
        char *logString = new char[logLen];
        glGetProgramInfoLog(program, logLen, NULL, logString);
        std::cout << "Error linking shader: " << logString << std::endl;
        delete logString;
        glDeleteProgram(program);
        exit(-1);
    }

    uNormalMat = glGetUniformLocation(program, "normal");
    uModelViewMat = glGetUniformLocation(program, "modelView");
    uModelViewProjMat = glGetUniformLocation(program, "modelViewProj");

    scene->addShader(this);
}

unsigned int Shader::getprogram() const {
    return program;
}

int Shader::getuModelViewMat() const {
    return uModelViewMat;
}

int Shader::getuModelViewProjMat() const {
    return uModelViewProjMat;
}

int Shader::getuNormalMat() const {
    return uNormalMat;
}

int Shader::getProgram() const {
    return program;
}

Scene *Shader::getScene() const {
    return scene;
}

char *Shader::loadStringFromFile(const char *fileName, unsigned int &fileLen) {
    // Loads the file
    std::ifstream file;
    file.open(fileName, std::ios::in);
    if (!file) {
        return 0;
    }

    // File length
    file.seekg(0, std::ios::end);
    fileLen = unsigned int(file.tellg());
    file.seekg(std::ios::beg);

    // Allocates enough memory
    char *source = new char[fileLen + 1];

    //Reads the file
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