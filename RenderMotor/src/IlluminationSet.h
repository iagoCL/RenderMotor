#ifndef RENDER_MOTOR_ILLUMINATION_SET_H
#define RENDER_MOTOR_ILLUMINATION_SET_H

#include <memory>
#include <vector>
#include "Light.h"
#include "Material.h"
#include "Shader.h"

class Shader;
class Material;
class IlluminationSet {
   private:
    struct LightRenderInfo {
        std::shared_ptr<Light> light;
        GLint uLPos;
        GLint uLDir;
        GLint uLCol;
        GLint uLAngle;
        LightRenderInfo(std::shared_ptr<Light> light_, GLint uLPos_, GLint uLDir_, GLint uLCol_, GLint uLAngle_)
            : light(light_),
              uLPos(uLPos_),
              uLDir(uLDir_),
              uLCol(uLCol_),
              uLAngle(uLAngle_) {
        }
    };
    static unsigned int numIlluminationsSets;
    const unsigned int id;

    std::shared_ptr<Shader> shader;
    std::vector<std::shared_ptr<Material>> materials;
    std::vector<LightRenderInfo> lights;

    void sendIlluminationToShader(const glm::mat4& view) const;
    IlluminationSet(std::shared_ptr<Shader> shader_);

   public:
    static std::shared_ptr<IlluminationSet> createIlluminationSet(std::shared_ptr<Shader> shader_);
    std::shared_ptr<Shader> getShader() const;
    void renderMaterials(const glm::mat4& view) const;
    void addLight(std::shared_ptr<Light> light);
    void addMaterial(std::shared_ptr<Material> material);
    virtual ~IlluminationSet();
};

#endif  //RENDER_MOTOR_ILLUMINATION_SET_H