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
    virtual void activateMaterial(glm::mat4 view) const = 0;
    virtual void addNode(Node* node);
    virtual void renderNodes(glm::mat4 view, glm::mat4 proj) const;
};

class SimpleMaterial : public Material {
   public:
    SimpleMaterial(IlluminationSet* illuminationSet_);
    void activateMaterial(glm::mat4 view) const override;
};

class BumpMaterial : public Material {
   protected:
    Texture* diffuseText;
    Texture* emissiveText;
    Texture* specularText;
    Texture* normalsText;

    //uniformes luz
    int* uLPos;
    int* uLDir;
    int* uLCol;
    int* uLAngle;

    //Uniform Textures
    int uColorTex;
    int uEmiTex;
    int uSpecTex;
    int uNormTex;

   public:
    BumpMaterial(IlluminationSet* illuminationSet_, Texture* diffuseText_, Texture* emissiveText_, Texture* specularText_, Texture* normalsText_);
    void activateMaterial(glm::mat4 view) const override;
};

#endif  //RENDER_MOTOR_MATERIAL_H