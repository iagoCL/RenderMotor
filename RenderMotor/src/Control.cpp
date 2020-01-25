#include "Control.h"
#include <iostream>
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>

unsigned int Control::numControls = 0;

Control::Control(std::shared_ptr<Camera> camera_)
    : camera(camera_),
      id(numControls++),
      mouseX(0),
      mouseY(0),
      mouseYAngle(0.0f) {
    std::cout << "Created Control: " << id << std::endl;
}
Control::~Control() {
    std::cout << "Deleted Control: " << id << std::endl;
}
void Control::keyboardFunc(unsigned char key, int x, int y) {
    std::cout << "Pressed key: " << key << " at: {" << x << ", " << y << "}" << std::endl;
    switch (key) {
        case 'w':
            camera->move(glm::vec3(0.0f, 0.0f, 1.0f));
            std::cout << "Camera moved." << std::endl
                      << std::endl;
            break;
        case 's':
            camera->move(glm::vec3(0.0f, 0.0f, -1.0f));
            std::cout << "Camera moved." << std::endl
                      << std::endl;
            break;
        case 'a':
            camera->move(glm::vec3(1.0f, 0.0f, 0.0f));
            std::cout << "Camera moved." << std::endl
                      << std::endl;
            break;
        case 'd':
            camera->move(glm::vec3(-1.0f, 0.0f, 0.0f));
            std::cout << "Camera moved." << std::endl
                      << std::endl;
            break;
        case 'e':
            camera->viewRotation(-0.1f, glm::vec3(0.0f, 1.0f, 0.0f));
            std::cout << "Camera rotated." << std::endl
                      << std::endl;
            break;
        case 'q':
            camera->viewRotation(0.1f, glm::vec3(0.0f, 1.0f, 0.0f));
            std::cout << "Camera rotated." << std::endl
                      << std::endl;
            break;
            /**/
            //Moving Light
        case 'o':
            moveOrRotateSelectedLight(glm::vec3(0.0f, 0.0f, 1.0f), 0.05f);
            break;
        case 'l':
            moveOrRotateSelectedLight(glm::vec3(0.0f, 0.0f, 1.0f), -0.05f);
            break;
        case 'i':
            moveOrRotateSelectedLight(glm::vec3(1.0f, 0.0f, 0.0f), 0.05f);
            break;
        case 'k':
            moveOrRotateSelectedLight(glm::vec3(1.0f, 0.0f, 0.0f), -0.05f);
            break;
        case 'u':
            moveOrRotateSelectedLight(glm::vec3(0.0f, 1.0f, 0.0f), 0.05f);
            break;
        case 'j':
            moveOrRotateSelectedLight(glm::vec3(0.0f, 1.0f, 0.0f), -0.05f);
            break;
        case 'y':
            if (!lights.empty()) {
                if (++selectedLightIt == lights.end()) {
                    selectedLightIt = lights.begin();
                }
                std::cout << "Selected light: " << (*selectedLightIt)->toString() << std::endl
                          << std::endl;
            }
            break;
        case '7':
            increaseLightsColors(glm::vec3(0.05f, 0.0f, 0.0f));
            break;
        case '4':
            increaseLightsColors(glm::vec3(-0.05f, 0.0f, 0.0f));
            break;
        case '8':
            increaseLightsColors(glm::vec3(0.0f, 0.05f, 0.0f));
            break;
        case '5':
            increaseLightsColors(glm::vec3(0.0f, -0.05f, 0.0f));
            break;
        case '9':
            increaseLightsColors(glm::vec3(0.0f, 0.0f, 0.05f));
            break;
        case '6':
            increaseLightsColors(glm::vec3(0.0f, 0.0f, -0.05f));
            break;
        default:
            std::cout << "Key has not associated action." << std::endl
                      << std::endl;
            break;
    }
}

void Control::moveOrRotateSelectedLight(const glm::vec3& direction, const float value) {
    if (!lights.empty()) {
        (*selectedLightIt)->moveOrRotate(direction, value);
        std::cout << "Light changed to: " << (*selectedLightIt)->toString() << std::endl;
    }
}
void Control::increaseLightsColors(const glm::vec3& colorIncrease) {
    for (auto const& lightIt : lights) {
        lightIt->increaseColor(colorIncrease);
    }
    std::cout << "Light color changed." << std::endl
              << std::endl;
}
void Control::addLight(std::shared_ptr<Light> light) {
    lights.push_back(light);
    selectedLightIt = lights.begin();
}

void Control::mouseFunc(int button, int state, int x, int y) {
    const char* buttonNames[3] = {"left", "center", "right"};
    if (state == 0) {
        mouseX = x;
        mouseY = y;
        std::cout << "Pressed mouse button: " << buttonNames[button] << " at: {" << x << ", " << y << "}" << std::endl
                  << std::endl;

    } else {
        std::cout << "Release mouse button: " << buttonNames[button] << " at {" << x << ", " << y << "}" << std::endl
                  << std::endl;
    }
}
void Control::showControls() {
    std::cout << "Controls:" << std::endl
              << " * { W, S, A, D } : Camera movement." << std::endl
              << " * { E, Q }       : Camera rotation." << std::endl
              << " * { O, L }       : Move/Rotate Selected Light, Z axis." << std::endl
              << " * { I, K }       : Move/Rotate Selected Light, X axis." << std::endl
              << " * { U, J }       : Move/Rotate Selected Light, Y axis." << std::endl
              << " * { 7, 4 }       : Increase/Reduce red component in all lights." << std::endl
              << " * { 8, 5 }       : Increase/Reduce green component in all lights." << std::endl
              << " * { 9, 6 }       : Increase/Reduce blue component in all lights." << std::endl
              << std::endl;
}

void Control::mouseMotionFunc(int x, int y) {
    //Horizontal rotation
    camera->viewRotation(0.005f * static_cast<float>(x - mouseX), glm::vec3(0.0f, 1.0f, 0.0f));

    float yShift = static_cast<float>(y - mouseY) * 0.005f;
    mouseYAngle += yShift;
    //Avoid rotate +- 90 degrees
    if (mouseYAngle > glm::half_pi<float>()) {
        yShift -= (mouseYAngle - glm::half_pi<float>());
        mouseYAngle = glm::half_pi<float>();
    } else if (mouseYAngle < -glm::half_pi<float>()) {
        yShift -= (mouseYAngle + glm::half_pi<float>());
        mouseYAngle = -glm::half_pi<float>();
    }

    camera->rotate(yShift, glm::vec3(1.0f, 0.0f, 0.0f));
    mouseX = x;
    mouseY = y;
}