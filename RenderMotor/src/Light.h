#ifndef RENDER_MOTOR_LIGHT_H
#define RENDER_MOTOR_LIGHT_H

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

class Light {
   private:
    int type;  //0 - NULL; 1 - POINT; 2 - DIRECTIONAL; 3 - FOCAL
    glm::vec3 direction;
    glm::vec3 position;
    glm::vec3 color;
    float angle;

    void sendDirectionToShader(const glm::mat4& view, const unsigned int id) const;
    void sendAngleToShader(const unsigned int id) const;
    void sendPositionToShader(const glm::mat4& view, unsigned int id) const;

   public:
    Light(const glm::vec3& position, const glm::vec3& color, const glm::vec3& direction, const float angle = 0.18f);  //Focal constructor
    Light(const glm::vec3& positionOrDirection, const glm::vec3& color, const int type_);                             //Directional or point constructor
    Light(const glm::vec3& position, const glm::vec3& color);                                                         //Point constructor
    Light();                                                                                                          //Null contructor
    ~Light();
    void sendToShader(const glm::mat4& view, const unsigned int positionId, const unsigned int directionId = -1, const unsigned int angleId = -1) const;
    void sendColorToShader(const unsigned int id) const;

    void increaseColor(const glm::vec3& colorIncrease);
    void moveOrRotate(const glm::vec3& direction, const float value);
    void move(const glm::vec3& distance);
    void rotate(const glm::vec3& rotation, const float angle);
    int getType() const;
    void setColor(const glm::vec3& color_);
    void setDirection(const glm::vec3& direction_);
};
#endif  //RENDER_MOTOR_LIGHT_H