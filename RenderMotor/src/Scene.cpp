#include "Scene.h"

#include <gl/glew.h>

#include <gl/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

#include "Shader.h"

Scene::Scene(Camera* camera_)
    : camera(camera_) {
}

void Scene::renderScene() const {
    glm::mat4 view = camera->getView();
    glm::mat4 proj = camera->getProj();
    //std::cout << "Rendering scene with: "<<nodes.size()<<" nodes."<<std::endl;
    for (auto itShader = shaders.begin(); itShader != shaders.end(); ++itShader) {
        (*itShader)->renderShader(view, proj);
    }
}

void Scene::addShader(Shader* shader_) {
    shaders.push_back(shader_);
}
Camera* Scene::getCamera() const {
    return camera;
}

Scene::~Scene() {
}