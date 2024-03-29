#ifndef RENDER_MOTOR_TEXTURE_H
#define RENDER_MOTOR_TEXTURE_H

#include <gl/glew.h>

#include <gl/gl.h>

class Texture {
   private:
    const int textureNumber;
    const GLuint textureId;

    Texture(const int textureId_, const GLuint textureNumber_);
    unsigned char *readTextureFromFile(const char *fileName, unsigned int &width, unsigned int &height);
    GLuint loadTexture(const char *fileName);

   public:
    static int numTextures;

    Texture(const char *fileName);
    ~Texture();
    void sendToShaderProgram(const unsigned int uniformId) const;
    void applyAnisotropicFilter() const;
};
#endif  //RENDER_MOTOR_TEXTURE_H