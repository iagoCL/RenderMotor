#ifndef RENDER_MOTOR_CONTROL_H
#define RENDER_MOTOR_CONTROL_H
#include "Scene.h"

class Scene;
class Control {
   private:
    Scene* scene;
    int mouseX;
    int mouseY;
    float mouseYAngle = 0;
    std::vector<Light*> lights;
    std::vector<Light*>::iterator selectedLightIt;

    void moveOrRotateSelectedLight(const glm::vec3& displacement, const float value);
    void increaseLightsColors(const glm::vec3& colorIncrease);

   public:
    Control(Scene* scene_);
    void addLight(Light* light);
    void keyboardFunc(unsigned char key, int x, int y);
    void mouseFunc(int button, int state, int x, int y);
    void mouseMotionFunc(int x, int y);

    static void showControls();
};
#endif  //RENDER_MOTOR_CONTROL_H