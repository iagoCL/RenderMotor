#include "Node.h"
#include "Model3D.h"
#include "Shader.h"

#include <gl/gl.h>
#include <gl/glew.h>
#include "iostream"

Node::Node(Model3D* mesh_, Material* material_)
    : mesh(mesh_),
      material(material_),
      modelMatrix(glm::mat4(1.0f)) {
    shader = material_->getIlluminationSet()->getShader();
    material->addNode(this);
}

Node::~Node() {
}
 void Node::setModel(glm::mat4 newModel){
   modelMatrix = newModel;
 }
void Node::renderNode(glm::mat4 view, glm::mat4 proj) const {
    int matrixGPUId = shader->getuModelViewMat();
    glm::mat4 modelView = view * modelMatrix;
    if (matrixGPUId != -1) {
        glUniformMatrix4fv(matrixGPUId, 1, GL_FALSE, &(modelView[0][0]));
    }
    matrixGPUId = shader->getuModelViewProjMat();
    if (matrixGPUId != -1) {
        glm::mat4 modelViewProj = proj * modelView;
        glUniformMatrix4fv(matrixGPUId, 1, GL_FALSE, &(modelViewProj[0][0]));
    }
    matrixGPUId = shader->getuNormalMat();
    if (matrixGPUId != -1) {
        glm::mat4 normal = glm::transpose(glm::inverse(modelView));
        glUniformMatrix4fv(matrixGPUId, 1, GL_FALSE, &(normal[0][0]));
    }
    mesh->renderModel();
}
