#ifndef RENDER_MOTOR_LIGHT_H
#define RENDER_MOTOR_LIGHT_H

#include <glm/glm.hpp>

class Light {
   private:
    int type;  //0 - NULL; 1 - POINT; 2 - DIRECTIONAL; 3 - FOCAL
    glm::vec3 direction;
    glm::vec3 position;
    glm::vec3 color;
    float angle;

    void sendDirectionToShader(glm::mat4 view, unsigned int id) const;
    void sendAngleToShader(unsigned int id) const;
    void sendPositionToShader(glm::mat4 view, unsigned int id) const;

   public:
    Light(glm::vec3 position, glm::vec3 color, glm::vec3 direction, float angle = 0.18f);  //Focal constructor
    Light(glm::vec3 positionOrDirection, glm::vec3 color, int type_);                      //Directional or point constructor
    Light(glm::vec3 position, glm::vec3 color);                                            //Point constructor
    Light();                                                                               //Null contructor
    ~Light();
    void sendToShader(glm::mat4 view, unsigned int positionId, unsigned int directionId = -1, unsigned int angleId = -1) const;
    void sendColorToShader(unsigned int id) const;

    void increaseColor(glm::vec3 colorIncrease);
    void move(glm::vec3 distance);
    void rotate(glm::vec3 rotation, float angle);
    int getType() const;
    void setColor(glm::vec3 color_);
    void setDirection(glm::vec3 direction_);
};
#endif  //RENDER_MOTOR_LIGHT_H