#include "Node.h"
#include "Model3D.h"
#include "Shader.h"

#include <gl/gl.h>
#include <gl/glew.h>
#include <iostream>

unsigned int Node::numNodes = 0;

std::shared_ptr<Node> Node::createNode(std::shared_ptr<Model3D> mesh_, std::shared_ptr<Material> material_) {
    std::shared_ptr<Node> node(new Node(mesh_, material_));
    addToScene(node);
    return node;
}
std::shared_ptr<NodeAnimated> NodeAnimated::createNodeAnimated(std::shared_ptr<Model3D> mesh_, std::shared_ptr<Material> material_, std::shared_ptr<Animation> animation_) {
    std::shared_ptr<NodeAnimated> node(new NodeAnimated(mesh_, material_, animation_));
    addToScene(node);
    return node;
}
Node::Node(std::shared_ptr<Model3D> mesh_, std::shared_ptr<Material> material_)
    : mesh(mesh_),
      id(numNodes++),
      material(material_),
      shader(material_->getIlluminationSet()->getShader()),
      model(glm::mat4(1.0f)) {
    std::cout << "Created node: " << id << std::endl;
}
NodeAnimated::NodeAnimated(std::shared_ptr<Model3D> mesh_, std::shared_ptr<Material> material_, std::shared_ptr<Animation> animation_)
    : Node(mesh_, material_),
      animation(animation_) {
}
Node::~Node() {
    std::cout << "Deleted node: " << id << std::endl;
}
void Node::addToScene(std::shared_ptr<Node> node_) {
    node_->material->addNode(node_);
    node_->shader->getScene()->addNode(node_);
}
void Node::updateNode(const float /*timeIncrease*/, const glm::mat4& view, const glm::mat4& proj) {
    modelView = view * model;
    modelViewProj = proj * modelView;
    normal = glm::transpose(glm::inverse(modelView));
}
void Node::setModel(const glm::mat4& model_) {
    model = model_;
}
void NodeAnimated::updateNode(const float timeIncrease, const glm::mat4& view, const glm::mat4& proj) {
    model = animation->updateAnimation(timeIncrease);
    Node::updateNode(timeIncrease, view, proj);
}
void Node::renderNode() const {
    int matrixGPUId = shader->getuModelViewMat();
    if (matrixGPUId != -1) {
        glUniformMatrix4fv(matrixGPUId, 1, GL_FALSE, &(modelView[0][0]));
    }
    matrixGPUId = shader->getuModelViewProjMat();
    if (matrixGPUId != -1) {
        glUniformMatrix4fv(matrixGPUId, 1, GL_FALSE, &(modelViewProj[0][0]));
    }
    matrixGPUId = shader->getuNormalMat();
    if (matrixGPUId != -1) {
        glUniformMatrix4fv(matrixGPUId, 1, GL_FALSE, &(normal[0][0]));
    }
    mesh->renderModel();
}

std::shared_ptr<Shader> Node::getShader() const {
    return shader;
}
std::shared_ptr<Material> Node::getMaterial() const {
    return material;
}