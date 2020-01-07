#ifndef RENDER_MOTOR_NODE_H
#define RENDER_MOTOR_NODE_H

#include <iostream>
#include "Camera.h"
#include "Light.h"
#include "Model3D.h"
#include "Shader.h"
#include "Texture.h"
class Node {
   private:
    Camera* camara;

   public:
    Model3D** malla;
    Light** luz;
    Shader* shaderCompilado;
    Texture** texturaColor;
    Texture** texturaEmi;
    Texture** texturaSpec;
    Texture** texturaNorm;
    int numMallas;
    int numLuces;

    //nodo(glm::mat4 view1, glm::mat4 proj1, int numeroMallas, malla * Malla1[], int numeroLuces, luz * Luz1[], shaderCompilado * shaderCompilado1, textura * textura1[] = nullptr, textura * textura2[] = nullptr, textura * textura3[] = nullptr, textura * textura4[] = nullptr);
    Node(Camera* cam, int numeroMallas, Model3D* Malla1[], int numeroLuces, Light* Luz1[], Shader* shaderCompilado1, Texture* textColor[] = nullptr, Texture* textEmissi[] = nullptr, Texture* textSpec[] = nullptr, Texture* textNorm[] = nullptr);

    ~Node();

    void renderizarNodo();
};
#endif //RENDER_MOTOR_NODE_H