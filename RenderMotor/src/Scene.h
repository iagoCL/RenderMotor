#ifndef RENDER_MOTOR_SCENE_H
#define RENDER_MOTOR_SCENE_H

#include <glm/glm.hpp>
#include "Camera.h"
#include "Shader.h"

class Shader;
class Scene {
   private:
    Camera* camera;
    std::vector<Shader*> shaders;

   public:
    Scene(Camera* camera_ = new Camera());

    void renderScene() const;

    void addShader(Shader* shader);
    Camera* getCamera() const;

    ~Scene();
};
#endif  //RENDER_MOTOR_SCENE_H