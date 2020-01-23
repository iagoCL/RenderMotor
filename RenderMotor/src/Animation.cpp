#include "Animation.h"

Animation::Animation(const float totalTime_, std::function<glm::mat4(const float)> updateFunction_)
    : updateFunction(updateFunction_),
      totalTime(totalTime_),
      actualTime(0.0f) {
}
Animation::Animation(const float totalTime_)
    : totalTime(totalTime_),
      actualTime(0.0f) {
}
AnimationWithInterpolation::AnimationWithInterpolation(const float totalTime_, std::function<glm::mat4(const float, Interpolation*)> updateFunction_, Interpolation* interpolation_)
    : Animation(totalTime_),
      interpolation(interpolation_),
      updateFunction(updateFunction_) {
}

glm::mat4 Animation::updateAnimation(const float timeIncrease, glm::mat4 view, glm::mat4 proj) {
    increaseTime(timeIncrease);
    return updateFunction(actualTime);
}
glm::mat4 AnimationWithInterpolation::updateAnimation(const float timeIncrease, glm::mat4 view, glm::mat4 proj) {
    increaseTime(timeIncrease);
    return updateFunction(actualTime, interpolation);
}

void Animation::increaseTime(const float timeIncrease) {
    actualTime += timeIncrease;
    if (actualTime >= totalTime) {
        actualTime -= floorf(actualTime / totalTime) * actualTime;
    }
}