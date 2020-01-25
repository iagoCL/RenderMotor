#ifndef RENDER_MOTOR_LIGHT_H
#define RENDER_MOTOR_LIGHT_H

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <string>

class Light {
   protected:
    static unsigned int numLights;
    const unsigned int id;

    glm::vec3 color;

    Light(const glm::vec3& color_);

   public:
    static std::string vecToString(const glm::vec3 vector);

    virtual void sendToShader(const glm::mat4& view, const unsigned int positionId, const unsigned int directionId = -1, const unsigned int angleId = -1) const = 0;
    virtual void sendColorToShader(const unsigned int id) const;

    virtual std::string toString() const;

    virtual void increaseColor(const glm::vec3& colorIncrease);
    virtual void moveOrRotate(const glm::vec3& direction, const float value) = 0;
    virtual void setColor(const glm::vec3& color_);
    virtual ~Light();
};
class LightPoint : public Light {
   protected:
    glm::vec3 position;

    void sendPositionToShader(const glm::mat4& view, unsigned int id) const;

   public:
    LightPoint(const glm::vec3& position, const glm::vec3& color);

    virtual void sendToShader(const glm::mat4& view, const unsigned int positionId, const unsigned int directionId = -1, const unsigned int angleId = -1) const override;
    virtual std::string toString() const override;
    virtual void moveOrRotate(const glm::vec3& direction, const float value) override;
    virtual void move(const glm::vec3& distance);
};
class LightDirectional : public Light {
   protected:
    glm::vec3 direction;

    void sendDirectionToShader(const glm::mat4& view, const unsigned int id) const;
    void setDirection(const glm::vec3& direction_);

   public:
    LightDirectional(const glm::vec3& direction_, const glm::vec3& color);

    virtual void sendToShader(const glm::mat4& view, const unsigned int positionId, const unsigned int directionId = -1, const unsigned int angleId = -1) const override;
    virtual std::string toString() const override;

    virtual void moveOrRotate(const glm::vec3& direction, const float value) override;
    virtual void rotate(const glm::vec3& rotation, const float angle);
};
class LightFocal : public LightDirectional, public LightPoint {
   protected:
    float angle;

    void sendAngleToShader(const unsigned int id) const;

   public:
    LightFocal(const glm::vec3& position, const glm::vec3& color, const glm::vec3& direction, const float angle = 0.18f);  //Focal constructor

    virtual void sendToShader(const glm::mat4& view, const unsigned int positionId, const unsigned int directionId = -1, const unsigned int angleId = -1) const override;
    virtual std::string toString() const override;

    virtual void moveOrRotate(const glm::vec3& direction, const float value) override;
};
#endif  //RENDER_MOTOR_LIGHT_H