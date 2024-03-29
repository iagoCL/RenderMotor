#include "Camera.h"

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

unsigned int Camera::numCameras = 0;

Camera::Camera(const glm::vec3& initialPos, const float angle, const float aspectRatio, const float near, const float far)
    : viewMatrix(glm::translate(glm::mat4(1.0f), initialPos) * glm::mat4(1.0f)),
      id(numCameras++) {
    setProjection(angle, aspectRatio, near, far);
    std::cout << "Created Camera: " << id << std::endl;
}

Camera::Camera(const glm::mat4& viewMatrix_, const glm::mat4& projectionMatrix_)
    : viewMatrix(viewMatrix_),
      projectionMatrix(projectionMatrix_),
      id(numCameras++) {
    std::cout << "Created Camera: " << id << std::endl;
}

void Camera::viewRotation(const float angle, const glm::vec3& rotationAxis) {
    viewMatrix = glm::rotate(glm::mat4(1.0f), angle, /*up axis*/ glm::vec3(viewMatrix * glm::vec4(rotationAxis, 0.0f))) * viewMatrix;
}
void Camera::rotate(const float angle, const glm::vec3& rotationAxis) {
    viewMatrix = glm::rotate(glm::mat4(1.0f), angle, /*up axis*/ rotationAxis) * viewMatrix;
}

void Camera::move(const glm::vec3& distance) {
    viewMatrix = glm::translate(glm::mat4(1.0f), distance) * viewMatrix;
}

void Camera::setProjection(const glm::mat4& projectionMatrix_) {
    projectionMatrix = projectionMatrix_;
}

void Camera::setProjection(const float angle, const float aspectRatio, const float near, const float far) {
    projectionMatrix = glm::perspective(glm::radians(angle), aspectRatio, near, far);
}

glm::mat4 Camera::getView() const {
    return viewMatrix;
}

glm::mat4 Camera::getProj() const {
    return projectionMatrix;
}

Camera::~Camera() {
    std::cout << "Deleted camera: " << id << std::endl;
}