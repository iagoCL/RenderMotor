#ifndef RENDER_MOTOR_SHADER_H
#define RENDER_MOTOR_SHADER_H

#include <gl/glew.h>
#include <memory>

#include <gl/gl.h>
#include "IlluminationSet.h"
#include "Scene.h"

class Scene;
class IlluminationSet;
class Shader {
   private:
    unsigned int vertexShader;
    unsigned int fragShader;
    unsigned int program;

    int uModelViewMat;
    int uModelViewProjMat;
    int uNormalMat;

    std::vector<std::shared_ptr<IlluminationSet>> illuminations;
    std::shared_ptr<Scene> scene;

    int loadShader(const GLchar** shaderString, const GLint stringLength, const int type) const;
    void initShader();
    void showShaderError() const;
    Shader(const GLchar** vertString, const GLint vertStringLength, const GLchar** fragString, const GLint fragStringLength, std::shared_ptr<Scene> scene_);

   public:
    static std::shared_ptr<Shader> createShaderFromFiles(const char* vertPath, const char* fragPath, std::shared_ptr<Scene> scene_);
    static std::shared_ptr<Shader> createShader(const GLchar** vertString, const GLint vertStringLength, const GLchar** fragString, const GLint fragStringLength, std::shared_ptr<Scene> scene_);
    ~Shader();
    void renderShader(const glm::mat4& view) const;
    void addIllumination(std::shared_ptr<IlluminationSet> illumination);

    int getuNormalMat() const;
    int getuModelViewMat() const;
    int getuModelViewProjMat() const;

    unsigned int getprogram() const;
    int getProgram() const;

    std::shared_ptr<Scene> getScene() const;

    static char* loadStringFromFile(const char* fileName, unsigned int& fileLen);
};
#endif  //RENDER_MOTOR_SHADER_H