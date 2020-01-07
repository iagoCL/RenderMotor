#include "Camera.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(glm::vec3 initialPos, float angle, float aspectRatio, float near, float far)
    : viewMatrix(glm::translate(glm::mat4(1.0f), initialPos) * glm::mat4(1.0f)) {
    setProjection(angle, aspectRatio, near, far);
}

Camera::Camera(glm::mat4 viewMatrix_, glm::mat4 projectionMatrix_)
    : viewMatrix(viewMatrix_), projectionMatrix(projectionMatrix_) {
}

void Camera::viewRotation(float angle, glm::vec3 rotationAxis) {
    viewMatrix = glm::rotate(glm::mat4(1.0f), angle, /*up axis*/ glm::vec3(viewMatrix * glm::vec4(rotationAxis, 0.0f))) * viewMatrix;
}
void Camera::rotate(float angle, glm::vec3 rotationAxis) {
    viewMatrix = glm::rotate(glm::mat4(1.0f), angle, /*up axis*/ rotationAxis) * viewMatrix;
}

void Camera::move(glm::vec3 distance) {
    viewMatrix = glm::translate(glm::mat4(1.0f), distance) * viewMatrix;
}

void Camera::setProjection(glm::mat4 projectionMatrix_) {
    projectionMatrix = projectionMatrix_;
}

void Camera::setProjection(float angle, float aspectRatio, float near, float far) {
    projectionMatrix = glm::perspective(glm::radians(angle), aspectRatio, near, far);
}

glm::mat4 Camera::getView() {
    return viewMatrix;
}

glm::mat4 Camera::getProj() {
    return projectionMatrix;
}

Camera::~Camera() {
}