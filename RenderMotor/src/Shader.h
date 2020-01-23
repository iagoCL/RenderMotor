#ifndef RENDER_MOTOR_SHADER_H
#define RENDER_MOTOR_SHADER_H

#include <gl/glew.h>

#include <gl/gl.h>
#include "IlluminationSet.h"
#include "Scene.h"

class Scene;
class IlluminationSet;
class Shader {
    //Definimos las variables que nos dan acceso a los shaders

   private:
    unsigned int vertexShader;
    unsigned int fragShader;
    unsigned int program;

    //Variables con las que hacedemos a los atributos de los shaders
    //Variables Uniform
    int uModelViewMat;
    int uModelViewProjMat;
    int uNormalMat;

    std::vector<IlluminationSet*> illuminations;
    Scene* scene;

    int loadShader(const GLchar** shaderString, const GLint stringLength, const int type) const;
    void initShader();
    void showShaderError() const;

   public:
    Shader(const char* vertPath, const char* fragPath, Scene* scene_);
    Shader(const GLchar** vertString, const GLint vertStringLength, const GLchar** fragString, const GLint fragStringLength, Scene* scene_);
    ~Shader();

    void renderShader(glm::mat4 view) const;
    void addIllumination(IlluminationSet* illumination);

    int getuNormalMat() const;
    int getuModelViewMat() const;
    int getuModelViewProjMat() const;

    unsigned int getprogram() const;
    int getProgram() const;

    Scene* getScene() const;

    static char* loadStringFromFile(const char* fileName, unsigned int& fileLen);
};
#endif  //RENDER_MOTOR_SHADER_H