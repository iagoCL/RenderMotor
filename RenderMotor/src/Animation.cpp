#include "Animation.h"

unsigned int Animation::numAnimations = 0;

Animation::Animation(const float totalTime_, std::function<glm::mat4(const float)> updateFunction_)
    : updateFunction(updateFunction_),
      totalTime(totalTime_),
      id(numAnimations++),
      actualTime(0.0f) {
    std::cout << "Created animation: " << id << std::endl;
}
Animation::Animation(const float totalTime_)
    : totalTime(totalTime_),
      id(numAnimations++),
      actualTime(0.0f) {
    std::cout << "Created animation: " << id << std::endl;
}
AnimationWithInterpolation::AnimationWithInterpolation(const float totalTime_, std::function<glm::mat4(const float, Interpolation*)> updateFunction_, std::shared_ptr<Interpolation> interpolation_)
    : Animation(totalTime_),
      interpolation(interpolation_),
      updateFunction(updateFunction_) {
}
Animation::~Animation() {
    std::cout << "Destroyed animation: " << id << std::endl;
}

glm::mat4 Animation::updateAnimation(const float timeIncrease) {
    increaseTime(timeIncrease);
    return updateFunction(actualTime);
}
glm::mat4 AnimationWithInterpolation::updateAnimation(const float timeIncrease) {
    increaseTime(timeIncrease);
    return updateFunction(actualTime, interpolation.get());
}

void Animation::increaseTime(const float timeIncrease) {
    actualTime += timeIncrease;
    if (actualTime >= totalTime) {
        actualTime -= floorf(actualTime / totalTime) * actualTime;
    }
}