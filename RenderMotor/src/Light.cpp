#include "Light.h"

#include <gl/glew.h>

#define GLM_FORCE_RADIANS
#include <gl/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
Light::Light(const glm::vec3& color_) {
    setColor(color_);
}
LightPoint::LightPoint(const glm::vec3& position_, const glm::vec3& color_)
    : Light(color_),
      position(position_) {
}
LightDirectional::LightDirectional(const glm::vec3& direction_, const glm::vec3& color_)
    : Light(color_) {
    setDirection(direction_);
}
LightFocal::LightFocal(const glm::vec3& position_, const glm::vec3& color_, const glm::vec3& direction_, const float angle_)
    : LightPoint(position_, color_),
      LightDirectional(direction_, color_),
      angle(angle_) {
}

void Light::setColor(const glm::vec3& color_) {
    color = glm::clamp(color_, glm::vec3(0.0f), glm::vec3(1.0f));
}

void LightDirectional::setDirection(const glm::vec3& direction_) {
    direction = glm::normalize(direction_);
}

void Light::increaseColor(const glm::vec3& increase) {
    setColor(color + increase);
}
void LightPoint::moveOrRotate(const glm::vec3& direction, const float value) {
    move(direction);
}
void LightDirectional::moveOrRotate(const glm::vec3& direction, const float value) {
    rotate(direction, value);
}
void LightFocal::moveOrRotate(const glm::vec3& direction, const float value) {
    rotate(direction, value);
}

void LightPoint::move(const glm::vec3& distance) {
    position += distance;
}

void LightDirectional::rotate(const glm::vec3& rotacion, const float angle) {
    setDirection(glm::vec3(glm::rotate(glm::mat4(1.0f), angle, rotacion) * glm::vec4(direction, 0.0f)));
}

void Light::sendColorToShader(const unsigned int id) const {
    if (id != -1) {
        glUniform3fv(id, 1, &(color[0]));
    } else {
        std::cout << "WARNING: Not sending light color, invalid uniform: " << id << std::endl;
    }
}

void LightPoint::sendPositionToShader(const glm::mat4& view, const unsigned int id) const {
    if (id != -1) {
        glm::vec3 temp = glm::vec3(view * glm::vec4(position, 1.0f));
        glUniform3fv(id, 1, &(temp[0]));
    } else {
        std::cout << "WARNING: Not sending light position, invalid uniform: " << id << std::endl;
    }
}

void LightDirectional::sendDirectionToShader(const glm::mat4& view, const unsigned int id) const {
    if (id != -1) {
        glm::vec3 tmp = glm::normalize(glm::vec3(view * glm::vec4(direction, 0.0f)));
        glUniform3fv(id, 1, &(tmp[0]));
    } else {
        std::cout << "WARNING: Not sending light direction, invalid uniform: " << id << std::endl;
    }
}

void LightFocal::sendAngleToShader(const unsigned int id) const {
    if (id != -1) {
        glUniform1f(id, angle);
    } else {
        std::cout << "WARNING: Not sending light angle, invalid uniform: " << id << std::endl;
    }
}

void LightPoint::sendToShader(const glm::mat4& view, const unsigned int positionId, const unsigned int directionId, const unsigned int angleId) const {
    sendPositionToShader(view, positionId);
}

void LightDirectional::sendToShader(const glm::mat4& view, const unsigned int positionId, const unsigned int directionId, const unsigned int angleId) const {
    sendDirectionToShader(view, directionId);
}

void LightFocal::sendToShader(const glm::mat4& view, const unsigned int positionId, const unsigned int directionId, const unsigned int angleId) const {
    sendPositionToShader(view, positionId);
    sendDirectionToShader(view, directionId);
    sendAngleToShader(angleId);
}

std::string Light::vecToString(const glm::vec3 vector) {
    return "[ " + std::to_string(vector[0]) + ", " + std::to_string(vector[1]) + ", " + std::to_string(vector[2]) + " ]";
}

std::string LightPoint::toString() const {
    return std::string("Point Light: { color: " + Light::vecToString(color) + " , Position: " + Light::vecToString(position) + "}\n");
}
std::string LightDirectional::toString() const {
    return std::string("Directional Light: { color: " + Light::vecToString(color) + " , Position: " + Light::vecToString(direction) + "}\n");
}
std::string LightFocal::toString() const {
    return std::string("Focal Light: { color: " + Light::vecToString(LightDirectional::color) + " , Angle: " + std::to_string(angle) + " , Position: " + Light::vecToString(position) + " , Direction: " + Light::vecToString(direction) + "}\n");
}
