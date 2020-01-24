#ifndef RENDER_MOTOR_NODE_H
#define RENDER_MOTOR_NODE_H

#include <iostream>
#include <vector>
#include "Animation.h"
#include "Model3D.h"
#include "Shader.h"

class Shader;
class Material;
class Animation;
class Node {
   protected:
    Model3D* mesh;
    Shader* shader;
    Material* material;

    glm::mat4 model;
    glm::mat4 modelView;
    glm::mat4 modelViewProj;
    glm::mat4 normal;

   public:
    Node(Model3D* mesh_, Material* material_);
    virtual void renderNode() const;
    virtual void updateNode(const float timeIncrease, const glm::mat4& view, const glm::mat4& proj);
    void setModel(const glm::mat4& model);

    virtual Shader* getShader() const;
    virtual Material* getMaterial() const;

    ~Node();
};
class NodeAnimated : public Node {
   protected:
    Animation* animation;

   public:
    NodeAnimated(Model3D* mesh_, Material* material_, Animation* animation_);
    virtual void updateNode(const float timeIncrease, const glm::mat4& view, const glm::mat4& proj) override;
};
#endif  //RENDER_MOTOR_NODE_H