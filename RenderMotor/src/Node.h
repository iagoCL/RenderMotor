#ifndef RENDER_MOTOR_NODE_H
#define RENDER_MOTOR_NODE_H

#include <iostream>
#include <vector>
#include "Model3D.h"
#include "Shader.h"

class Shader;
class Material;
class Node {
   private:
    Model3D* mesh;
    Shader* shader;
    Material* material;
    glm::mat4 modelMatrix;

   public:
    Node(Model3D* mesh_, Material* material_);
    void renderNode(glm::mat4 view, glm::mat4 proj) const;
    void setModel(glm::mat4 modelMatrix_);

    ~Node();
};
#endif  //RENDER_MOTOR_NODE_H