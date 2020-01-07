#include "Interpolation.h"

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

Interpolation::Interpolation(const float totalTime_)
    : totalTime(totalTime_),
      actualTime(0.0f) {
}

void Interpolation::increaseTime(const float timeIncrease) {
    actualTime += timeIncrease;
    if (actualTime >= totalTime) {
        actualTime = actualTime - floorf(actualTime / totalTime) * actualTime;
    }
}

BezierInterpolation::BezierInterpolation(const float totalTime_, std::vector<glm::vec3>* controlPoints_)
    : Interpolation(totalTime_),
      timePerStep(totalTime_ / static_cast<float>(controlPoints_->size() / 2)),
      controlPoints(controlPoints_) {
    for (int i = 0; i < controlPoints_->size(); ++i) {
        glm::vec3 newPos = controlPoints->at(i);
    }
}
std::vector<glm::vec3>* BezierInterpolation::getCirclePoints(const float radio) {
    /*
     * Points needed to follow a circle with center in 0,0 it moves on the plane yz, orbits around the x axis
     * Divide in 4 steps:      * 
     * Step 1 Uses points 0 and 2; point 1 is where the tangents meet
     * Step 2 Uses points 2 and 4; point 3 is where the tangents meet
     * Step 3 Uses points 4 and 6; point 5 is where the tangents meet
     * Step 4 Uses points 6 and 8; point 7 is where the tangents meet
     */
    std::vector<glm::vec3>* circleControlPoints = new std::vector<glm::vec3>();
    circleControlPoints->reserve(9);
    circleControlPoints->push_back(glm::vec3(0.0f, 0.0f, radio));
    circleControlPoints->push_back(glm::vec3(0.0f, radio, radio));
    circleControlPoints->push_back(glm::vec3(0.0f, radio, 0.0f));
    circleControlPoints->push_back(glm::vec3(0.0f, radio, -radio));
    circleControlPoints->push_back(glm::vec3(0.0f, 0.0f, -radio));
    circleControlPoints->push_back(glm::vec3(0.0f, -radio, -radio));
    circleControlPoints->push_back(glm::vec3(0.0f, -radio, 0.0f));
    circleControlPoints->push_back(glm::vec3(0.0f, -radio, radio));
    circleControlPoints->push_back(glm::vec3(0.0f, 0.0f, radio));
    return circleControlPoints;
}
glm::vec3 BezierInterpolation::getPosition() {
    float stepNumber = floorf(actualTime / timePerStep);

    float stepTime = actualTime - stepNumber * timePerStep;
    int stepPoint = 2 * int(stepNumber);

    return pow((timePerStep - stepTime), 2) * controlPoints->at(stepPoint) + 2.0f * (timePerStep - stepTime) * stepTime * controlPoints->at(stepPoint + 1) + pow(stepTime, 2) * controlPoints->at(stepPoint + 2);
}
BezierInterpolation::~BezierInterpolation() {
    delete controlPoints;
}
SplinesInterpolation::SplinesInterpolation(const float totalTime_, std::vector<glm::vec3>* controlPoints_, std::vector<float>* controlTangents_)
    : Interpolation(totalTime_),
      timePerStep(totalTime_ / static_cast<float>(controlPoints_->size() - 1)),
      controlPoints(controlPoints_),
      controlTangents(controlTangents_) {
}
std::vector<glm::vec3>* SplinesInterpolation::getCirclePoints(float radio) {
    /*
     * Points needed to follow a circle with center in 0,0 it moves on the plane xy, orbits around the z axis
     */

    radio = radio * cos(glm::quarter_pi<double>());
    std::vector<glm::vec3>* circleControlPoints = new std::vector<glm::vec3>();
    circleControlPoints->reserve(5);
    circleControlPoints->push_back(glm::vec3(radio, radio, 0.0f));
    circleControlPoints->push_back(glm::vec3(-radio, radio, 0.0f));
    circleControlPoints->push_back(glm::vec3(-radio, -radio, 0.0f));
    circleControlPoints->push_back(glm::vec3(radio, -radio, 0.0f));
    circleControlPoints->push_back(glm::vec3(radio, radio, 0.0f));
    return circleControlPoints;
}
std::vector<float>* SplinesInterpolation::getCircleTangents() {
    /*
     * Tangents needed to follow a circle with center in 0,0 it moves on the plane xy, orbits around the z axis
     */

    std::vector<float>* circleControlTangents = new std::vector<float>();
    circleControlTangents->reserve(5);
    static float tangentAngle = tanf(glm::quarter_pi<float>());
    circleControlTangents->push_back(-tangentAngle);
    circleControlTangents->push_back(tangentAngle);
    circleControlTangents->push_back(-tangentAngle);
    circleControlTangents->push_back(tangentAngle);
    circleControlTangents->push_back(-tangentAngle);

    return circleControlTangents;
}
glm::vec3 SplinesInterpolation::getPosition() {
    float stepNumber = floorf(actualTime / timePerStep);

    float stepTime = actualTime - stepNumber * timePerStep;
    int stepPoint = int(stepNumber);

    float vectorX, vectorY;

    if (stepPoint % 2 == 0) {
        float tempA = controlTangents->at(stepPoint) * (controlPoints->at(stepPoint + 1).x - controlPoints->at(stepPoint).x) - (controlPoints->at(stepPoint + 1).y - controlPoints->at(stepPoint).y);
        float tempB = (controlPoints->at(stepPoint + 1).y - controlPoints->at(stepPoint).y) - controlTangents->at(stepPoint + 1) * (controlPoints->at(stepPoint + 1).x - controlPoints->at(stepPoint).x);

        vectorX = (timePerStep - stepTime) * controlPoints->at(stepPoint).y + stepTime * controlPoints->at(stepPoint + 1).y + (timePerStep - stepTime) * stepTime * (tempA * (timePerStep - stepTime) + tempB * stepTime);
        vectorY = controlPoints->at(stepPoint).x + stepTime * (controlPoints->at(stepPoint + 1).x - controlPoints->at(stepPoint).x);
    } else {
        float tempA = controlTangents->at(stepPoint) * (controlPoints->at(stepPoint + 1).y - controlPoints->at(stepPoint).y) - (controlPoints->at(stepPoint + 1).x - controlPoints->at(stepPoint).x);
        float tempB = (controlPoints->at(stepPoint + 1).x - controlPoints->at(stepPoint).x) - controlTangents->at(stepPoint + 1) * (controlPoints->at(stepPoint + 1).y - controlPoints->at(stepPoint).y);

        vectorY = (timePerStep - stepTime) * controlPoints->at(stepPoint).x + stepTime * controlPoints->at(stepPoint + 1).x + (timePerStep - stepTime) * stepTime * (tempA * (timePerStep - stepTime) + tempB * stepTime);
        vectorX = controlPoints->at(stepPoint).y + stepTime * (controlPoints->at(stepPoint + 1).y - controlPoints->at(stepPoint).y);
    }

    return glm::vec3(vectorY, vectorX, controlPoints->at(stepPoint).z + stepTime * (controlPoints->at(stepPoint + 1).z - controlPoints->at(stepPoint).z));
}
SplinesInterpolation::~SplinesInterpolation() {
    delete controlPoints;
    delete controlTangents;
}