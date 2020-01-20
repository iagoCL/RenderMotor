#ifndef RENDER_MOTOR_TEXTURE_H
#define RENDER_MOTOR_TEXTURE_H

#include <gl/glew.h>

#include <gl/gl.h>

class Texture {
   private:
    int textureNumber;
    GLuint textureId;
    Texture(unsigned int textureId_, int textureNumber_);
    unsigned char *readTextureFromFile(const char *fileName, unsigned int &width, unsigned int &height);
    GLuint loadTexture(const char *fileName);

   public:
    static int numberOfTextures;

    Texture(const char *fileName);
    ~Texture();
    void sendToShaderProgram(unsigned int uniformId) const;
    void applyAnisotropicFilter() const;
};
#endif  //RENDER_MOTOR_TEXTURE_H