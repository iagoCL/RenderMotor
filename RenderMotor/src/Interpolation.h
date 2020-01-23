#ifndef RENDER_MOTOR__H
#define RENDER_MOTOR__H

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <vector>

class Interpolation {
   public:
    Interpolation();
    virtual glm::vec3 getPosition(const float actualTime) const = 0;
};
class SplinesInterpolation : public Interpolation {
   private:
    std::vector<glm::vec3>* controlPoints;
    std::vector<float>* controlTangents;
    const float timePerStep;

   public:
    SplinesInterpolation(const float totalTime, std::vector<glm::vec3>* controlPoints_, std::vector<float>* controlTangents_);
    glm::vec3 getPosition(const float actualTime) const override;
    static std::vector<glm::vec3>* getCirclePoints(float radio);
    static std::vector<float>* getCircleTangents();
    ~SplinesInterpolation();
};
class BezierInterpolation : public Interpolation {
   private:
    std::vector<glm::vec3>* controlPoints;
    const float timePerStep;

   public:
    BezierInterpolation(const float totalTime, std::vector<glm::vec3>* controlPoints_);
    glm::vec3 getPosition(const float actualTime) const override;
    static std::vector<glm::vec3>* getCirclePoints(const float radio);
    ~BezierInterpolation();
};
#endif  //