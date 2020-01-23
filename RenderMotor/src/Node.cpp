#include "Node.h"
#include "Model3D.h"
#include "Shader.h"

#include <gl/gl.h>
#include <gl/glew.h>
#include <iostream>

Node::Node(Model3D* mesh_, Material* material_)
    : mesh(mesh_),
      material(material_),
      model(glm::mat4(1.0f)) {
    shader = material_->getIlluminationSet()->getShader();
    material->addNode(this);
    shader->getScene()->addNode(this);
}

Node::~Node() {
}
void Node::updateNode(const float /*timeIncrease*/, glm::mat4 view, glm::mat4 proj) {
    modelView = view * model;
    modelViewProj = proj * modelView;
    normal = glm::transpose(glm::inverse(modelView));
}
void Node::setModel(glm::mat4 model_) {
    model = model_;
}
NodeAnimated::NodeAnimated(Model3D* mesh_, Material* material_, Animation* animation_)
    : Node(mesh_, material_),
      animation(animation_) {
}
void NodeAnimated::updateNode(const float timeIncrease, glm::mat4 view, glm::mat4 proj) {
    model = animation->updateAnimation(timeIncrease, view, proj);
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

Shader* Node::getShader() const {
    return shader;
}
Material* Node::getMaterial() const {
    return material;
}