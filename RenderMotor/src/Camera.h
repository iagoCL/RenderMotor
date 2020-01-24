#ifndef RENDER_MOTOR_CAMERA_H
#define RENDER_MOTOR_CAMERA_H

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

class Camera {
   private:
    glm::mat4 viewMatrix;
    glm::mat4 projectionMatrix;

   public:
    Camera(const glm::vec3& initialPos = glm::vec3(0.0f, 0.0f, -10.0f), float angle = 60.0f, float aspectRatio = 1.0f, float near = 0.1f, float far = 100.0f);
    Camera(const glm::mat4& viewMatrix_, const glm::mat4& projectionMatrix_);
    void viewRotation(float angle, const glm::vec3& rotationAxis);
    void rotate(float angle, const glm::vec3& rotationAxis);
    void move(const glm::vec3& distance);
    void setProjection(const glm::mat4& projectionMatrix_);
    void setProjection(float angle, float aspectRatio, float near, float far);
    glm::mat4 getView() const;
    glm::mat4 getProj() const;
    ~Camera();
};
#endif  //RENDER_MOTOR_CAMERA_H