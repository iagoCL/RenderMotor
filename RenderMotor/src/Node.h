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
    static unsigned int numNodes;
    const unsigned int id;

    const std::shared_ptr<Model3D> mesh;
    const std::shared_ptr<Shader> shader;
    const std::shared_ptr<Material> material;

    glm::mat4 model;
    glm::mat4 modelView;
    glm::mat4 modelViewProj;
    glm::mat4 normal;

    Node(std::shared_ptr<Model3D> mesh_, std::shared_ptr<Material> material_);
    static void addToScene(std::shared_ptr<Node> node_);

   public:
    static std::shared_ptr<Node> createNode(std::shared_ptr<Model3D> mesh_, std::shared_ptr<Material> material_);
    virtual void renderNode() const;
    virtual void updateNode(const float timeIncrease, const glm::mat4& view, const glm::mat4& proj);
    void setModel(const glm::mat4& model);

    virtual std::shared_ptr<Shader> getShader() const;
    virtual std::shared_ptr<Material> getMaterial() const;

    ~Node();
};
class NodeAnimated : public Node {
   protected:
    const std::shared_ptr<Animation> animation;

    NodeAnimated(std::shared_ptr<Model3D> mesh_, std::shared_ptr<Material> material_, std::shared_ptr<Animation> animation_);

   public:
    static std::shared_ptr<NodeAnimated> createNodeAnimated(std::shared_ptr<Model3D> mesh_, std::shared_ptr<Material> material_, std::shared_ptr<Animation> animation_);
    virtual void updateNode(const float timeIncrease, const glm::mat4& view, const glm::mat4& proj) override;
};
#endif  //RENDER_MOTOR_NODE_H