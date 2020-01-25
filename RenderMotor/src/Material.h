#ifndef RENDER_MOTOR_MATERIAL_H
#define RENDER_MOTOR_MATERIAL_H
#include <memory>
#include <vector>
#include "Node.h"
#include "Shader.h"
#include "Texture.h"

class IlluminationSet;
class Node;
class Material {
   protected:
    static unsigned int numMaterials;
    const unsigned int id;

    std::shared_ptr<IlluminationSet> illuminationSet;
    std::vector<std::shared_ptr<Node>> nodes;

    Material(std::shared_ptr<IlluminationSet> illuminationSet);

   public:
    virtual std::shared_ptr<IlluminationSet> getIlluminationSet() const;
    virtual void activateMaterial(const glm::mat4& view) const = 0;
    virtual void addNode(std::shared_ptr<Node> node);
    virtual void renderNodes(const glm::mat4& view) const;
    virtual ~Material();
};

class SimpleMaterial : public Material {
   protected:
    SimpleMaterial(std::shared_ptr<IlluminationSet> illuminationSet_);

   public:
    static std::shared_ptr<SimpleMaterial> createSimpleMaterial(std::shared_ptr<IlluminationSet> illuminationSet_);
    void activateMaterial(const glm::mat4& view) const override;
};

class SimpleTextureMaterial : public Material {
   protected:
    const std::shared_ptr<Texture> diffuseText;

    int uColorTex;

    SimpleTextureMaterial(std::shared_ptr<IlluminationSet> illuminationSet_, std::shared_ptr<Texture> diffuseText_);

   public:
    static std::shared_ptr<SimpleTextureMaterial> createSimpleTextureMaterial(std::shared_ptr<IlluminationSet> illuminationSet_, std::shared_ptr<Texture> diffuseText_);
    void activateMaterial(const glm::mat4& view) const override;
};

class TextureMaterial : public SimpleTextureMaterial {
   protected:
    const std::shared_ptr<Texture> emissiveText;
    const std::shared_ptr<Texture> specularText;

    int uEmiTex;
    int uSpecTex;

    TextureMaterial(std::shared_ptr<IlluminationSet> illuminationSet_, std::shared_ptr<Texture> diffuseText_, std::shared_ptr<Texture> emissiveText_, std::shared_ptr<Texture> specularText_);

   public:
    static std::shared_ptr<TextureMaterial> createTextureMaterial(std::shared_ptr<IlluminationSet> illuminationSet_, std::shared_ptr<Texture> diffuseText_, std::shared_ptr<Texture> emissiveText_, std::shared_ptr<Texture> specularText_);
    void activateMaterial(const glm::mat4& view) const override;
};

class BumpMaterial : public TextureMaterial {
   protected:
    const std::shared_ptr<Texture> normalsText;

    int uNormTex;

    BumpMaterial(std::shared_ptr<IlluminationSet> illuminationSet_, std::shared_ptr<Texture> diffuseText_, std::shared_ptr<Texture> emissiveText_, std::shared_ptr<Texture> specularText_, std::shared_ptr<Texture> normalsText_);

   public:
    static std::shared_ptr<BumpMaterial> createBumpMaterial(std::shared_ptr<IlluminationSet> illuminationSet_, std::shared_ptr<Texture> diffuseText_, std::shared_ptr<Texture> emissiveText_, std::shared_ptr<Texture> specularText_, std::shared_ptr<Texture> normalsText_);
    void activateMaterial(const glm::mat4& view) const override;
};

#endif  //RENDER_MOTOR_MATERIAL_H