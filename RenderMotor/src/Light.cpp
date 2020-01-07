#include "Light.h"

#include <gl/glew.h>
#include <gl/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

Light::Light(glm::vec3 position_, glm::vec3 color_, glm::vec3 direction_, float angle_)
    : position(position_), angle(angle_), type(3) {
    setDirection(direction_);
    setColor(color_);
}

Light::Light(glm::vec3 positionOrDirection, glm::vec3 color_, int type_)
    : type(type_),
      position(type_ == 1 ? positionOrDirection : glm::vec3(0.0f)),
      angle(type == 3 ? 0.18f : 0.0f) {
    if (type_ == 1) {
        direction = glm::vec3(0.0f);
    } else {
        setDirection(positionOrDirection);
    }
    setColor(color_);
}

Light::Light(glm::vec3 position_, glm::vec3 color_)
    : position(position_), direction(glm::vec3(0.0f)), angle(0.0f), type(1) {
    setColor(color_);
}

Light::Light()
    : type(0), angle(0.0f), position(glm::vec3(0.0f)), direction(glm::vec3(0.0f)), color(glm::vec3(0.0f)) {
}

Light::~Light() {
}

void Light::increaseColor(glm::vec3 increase) {
    setColor(color + increase);
}

void Light::move(glm::vec3 distance) {
    position += distance;
}

void Light::rotate(glm::vec3 rotacion, float angle) {
    direction = glm::vec3(glm::rotate(glm::mat4(1.0f), angle, rotacion) * glm::vec4(direction, 0.0f));
}

void Light::sendToShader(glm::mat4 view, unsigned int positionId, unsigned int directionId, unsigned int angleId) {
    if (type == 0) {
        std::cout << "WARNING: Sending to shader a Null light";
    } else {
        if (type != 2) {
            sendPositionToShader(view, positionId);
        }
        if (type != 1) {
            sendDirectionToShader(view, directionId);
        }
        if (type == 3) {
            sendAngleToShader(angleId);
        }
    }
}

void Light::sendDirectionToShader(glm::mat4 view, unsigned int id) {
    if (id >= 0) {
        glm::vec3 tmp = glm::normalize(glm::vec3(view * glm::vec4(direction, 0.0f)));
        glUniform3fv(id, 1, &(tmp[0]));
    } else {
        std::cout << "WARNING: Not sending light direction, invalid uniform: " << id << std::endl;
    }
}

void Light::sendAngleToShader(unsigned int id) {
    if (id != -1) {
        glUniform1f(id, angle);
    } else {
        std::cout << "WARNING: Not sending light angle, invalid uniform: " << id << std::endl;
    }
}

void Light::sendPositionToShader(glm::mat4 view, unsigned int id) {
    if (id != -1) {
        glm::vec3 temp = glm::vec3(view * glm::vec4(position, 1.0f));
        glUniform3fv(id, 1, &(temp[0]));
    } else {
        std::cout << "WARNING: Not sending light position, invalid uniform: " << id << std::endl;
    }
}

void Light::sendColorToShader(unsigned int id) {
    if (id != -1) {
        glUniform3fv(id, 1, &(color[0]));
    } else {
        std::cout << "WARNING: Not sending light color, invalid uniform: " << id << std::endl;
    }
}
int Light::getType() {
    return type;
}

void Light::setColor(glm::vec3 color_) {
    color = glm::clamp(color_, glm::vec3(0.0f), glm::vec3(1.0f));
}

void Light::setDirection(glm::vec3 direction_) {
    direction = glm::normalize(direction_);
}