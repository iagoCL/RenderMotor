#ifndef RENDER_MOTOR_ANIMATION_H
#define RENDER_MOTOR_ANIMATION_H

#include <functional>
#include <memory>

#include "Interpolation.h"
#include "Node.h"

class Animation {
   private:
    static unsigned int numAnimations;
    const unsigned int id;
    std::function<glm::mat4(const float)> updateFunction;

   protected:
    const float totalTime;
    float actualTime;

    virtual void increaseTime(const float timeIncrease);
    Animation(const float totalTime_);

   public:
    virtual ~Animation();
    Animation(const float totalTime_, std::function<glm::mat4(const float)> updateFunction_);
    virtual glm::mat4 updateAnimation(const float timeIncrease);
};
class AnimationWithInterpolation : public Animation {
   private:
    std::shared_ptr<Interpolation> interpolation;
    std::function<glm::mat4(const float, Interpolation*)> updateFunction;

   public:
    AnimationWithInterpolation(const float totalTime_, std::function<glm::mat4(const float, Interpolation*)> updateFunction_, std::shared_ptr<Interpolation> interpolation_);
    glm::mat4 updateAnimation(const float timeIncrease) override;
};
#endif  //RENDER_MOTOR_ANIMATION_H
