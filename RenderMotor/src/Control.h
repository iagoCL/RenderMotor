#ifndef RENDER_MOTOR_CONTROL_H
#define RENDER_MOTOR_CONTROL_H
#include <memory>
#include "Scene.h"

class Scene;
class Control {
   private:
    static unsigned int numControls;
    const unsigned int id;
    std::shared_ptr<Camera> camera;
    int mouseX;
    int mouseY;
    float mouseYAngle = 0;
    std::vector<std::shared_ptr<Light>> lights;
    std::vector<std::shared_ptr<Light>>::iterator selectedLightIt;

    void moveOrRotateSelectedLight(const glm::vec3& displacement, const float value);
    void increaseLightsColors(const glm::vec3& colorIncrease);

   public:
    Control(std::shared_ptr<Camera> camera_);
    ~Control();
    void addLight(std::shared_ptr<Light> light);
    void keyboardFunc(unsigned char key, int x, int y);
    void mouseFunc(int button, int state, int x, int y);
    void mouseMotionFunc(int x, int y);

    static void showControls();
};
#endif  //RENDER_MOTOR_CONTROL_H