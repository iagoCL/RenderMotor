#ifndef RENDER_MOTOR_MATERIAL_H
#define RENDER_MOTOR_MATERIAL_H
#include <vector>
#include "Node.h"
#include "Shader.h"
#include "Texture.h"

class IlluminationSet;
class Node;
class Material {
   protected:
    IlluminationSet* illuminationSet;
    std::vector<Node*> nodes;

   public:
    Material(IlluminationSet* illuminationSet);
    virtual IlluminationSet* getIlluminationSet() const;
    virtual void activateMaterial(const glm::mat4& view) const = 0;
    virtual void addNode(Node* node);
    virtual void renderNodes(const glm::mat4& view) const;
};

class SimpleMaterial : public Material {
   public:
    SimpleMaterial(IlluminationSet* illuminationSet_);
    void activateMaterial(const glm::mat4& view) const override;
};

class SimpleTextureMaterial : public Material {
   protected:
    const Texture* diffuseText;

    int uColorTex;

   public:
    SimpleTextureMaterial(IlluminationSet* illuminationSet_, Texture* diffuseText_);
    void activateMaterial(const glm::mat4& view) const override;
};

class TextureMaterial : public SimpleTextureMaterial {
   protected:
    const Texture* emissiveText;
    const Texture* specularText;

    int uEmiTex;
    int uSpecTex;

   public:
    TextureMaterial(IlluminationSet* illuminationSet_, Texture* diffuseText_, Texture* emissiveText_, Texture* specularText_);
    void activateMaterial(const glm::mat4& view) const override;
};

class BumpMaterial : public TextureMaterial {
   protected:
    const Texture* normalsText;

    int uNormTex;

   public:
    BumpMaterial(IlluminationSet* illuminationSet_, Texture* diffuseText_, Texture* emissiveText_, Texture* specularText_, Texture* normalsText_);
    void activateMaterial(const glm::mat4& view) const override;
};

#endif  //RENDER_MOTOR_MATERIAL_H