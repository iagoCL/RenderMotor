#ifndef RENDER_MOTOR_ANIMATION_H
#define RENDER_MOTOR_ANIMATION_H

#include <functional>

#include "Interpolation.h"
#include "Node.h"

class Animation {
   private:
    std::function<glm::mat4(const float)> updateFunction;

   protected:
    const float totalTime;
    float actualTime;

    virtual void increaseTime(const float timeIncrease);
    Animation(const float totalTime_);

   public:
    Animation(const float totalTime_, std::function<glm::mat4(const float)> updateFunction_);
    virtual glm::mat4 updateAnimation(const float timeIncrease, glm::mat4 view, glm::mat4 proj);
};
class AnimationWithInterpolation : public Animation {
   private:
    Interpolation* interpolation;
    std::function<glm::mat4(const float, Interpolation*)> updateFunction;

   public:
    AnimationWithInterpolation(const float totalTime_, std::function<glm::mat4(const float, Interpolation*)> updateFunction_, Interpolation* interpolation_);
    glm::mat4 updateAnimation(const float timeIncrease, glm::mat4 view, glm::mat4 proj) override;
};
#endif  //RENDER_MOTOR_ANIMATION_H
