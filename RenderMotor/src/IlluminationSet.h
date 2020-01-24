#ifndef RENDER_MOTOR_ILLUMINATION_SET_H
#define RENDER_MOTOR_ILLUMINATION_SET_H

#include <vector>
#include "Light.h"
#include "Material.h"
#include "Shader.h"

class Shader;
class Material;
class IlluminationSet {
   private:
    struct LightRenderInfo {
        Light* light;
        GLint uLPos;
        GLint uLDir;
        GLint uLCol;
        GLint uLAngle;
        LightRenderInfo(Light* light_, GLint uLPos_, GLint uLDir_, GLint uLCol_, GLint uLAngle_)
            : light(light_),
              uLPos(uLPos_),
              uLDir(uLDir_),
              uLCol(uLCol_),
              uLAngle(uLAngle_) {
        }
    };
    Shader* shader;
    std::vector<Material*> materials;
    std::vector<LightRenderInfo> lights;

    void sendIlluminationToShader(const glm::mat4& view) const;

   public:
    IlluminationSet(Shader* shader_);
    Shader* getShader() const;
    void renderMaterials(const glm::mat4& view) const;
    void addLight(Light* light);
    void addMaterial(Material* material);
};

#endif  //RENDER_MOTOR_ILLUMINATION_SET_H