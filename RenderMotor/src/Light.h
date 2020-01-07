#ifndef RENDER_MOTOR_LIGHT_H
#define RENDER_MOTOR_LIGHT_H

#include <glm/glm.hpp>

class Light {
   private:
    int type;  //0 - no hay luz; 1 - Puntual; 2 - direccional; 3 - focal
    glm::vec3 direction;
    glm::vec3 position;
    glm::vec3 color;
    float angle;

    void sendDirectionToShader(glm::mat4 view, unsigned int id);
    void sendAngleToShader(unsigned int id);
    void sendPositionToShader(glm::mat4 view, unsigned int id);
   public:
    Light(glm::vec3 position, glm::vec3 color, glm::vec3 direction, float angle = 0.18f);  //constructor focal
    Light(glm::vec3 positionOrDirection, glm::vec3 color, int type_);                      //constructor direccional o puntual
    Light(glm::vec3 position, glm::vec3 color);                                            //contructor puntual
    Light();                                                                               //constructor a nula
    ~Light();
    void sendToShader(glm::mat4 view, unsigned int positionId, unsigned int directionId = -1, unsigned int angleId = -1);
    void sendColorToShader(unsigned int id);

    void increaseColor(glm::vec3 colorIncrease);
    void move(glm::vec3 distance);
    void rotate(glm::vec3 rotation, float angle);
    int getType();
    void setColor(glm::vec3 color_);
    void setDirection(glm::vec3 direction_);
};
#endif  //RENDER_MOTOR_LIGHT_H