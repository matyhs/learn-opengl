#include "camera.hpp"

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch) : 
                front(glm::vec3 { 0.0f, 0.0f, -1.0f }),
                moveSpeed(SPEED), mouseSensitivity(SENSITIVITY), mouseZoom(ZOOM)
{
    Camera::position = position;
    Camera::worldUp = up;
    Camera::yaw = yaw;
    Camera::pitch = pitch;
    updateCameraVectors();
}

Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : 
                front(glm::vec3 { 0.0f, 0.0f, -1.0f }),
                moveSpeed(SPEED), mouseSensitivity(SENSITIVITY), mouseZoom(ZOOM)
{
    Camera::position = glm::vec3 { posX, posY, posZ };
    Camera::worldUp = glm::vec3 { upX, upY, upZ };
    Camera::yaw = yaw;
    Camera::pitch = pitch;
    updateCameraVectors();
}

void Camera::ProcessKeyboard(Movement direction, float deltaTime) {
    float velocity = deltaTime * moveSpeed;
    if (direction == FORWARD) {
        position += front * velocity;
    }
    if (direction == BACKWARD) {
        position -= front * velocity;
    }
    if (direction == RIGHT) {
        position += right * velocity;
    }
    if (direction == LEFT) {
        position -= right * velocity;
    }
    // fps mode: we need to stay on the xz-plane
    position.y = 0.0f;
}

void Camera::ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch) {
    // take into consideration the mouse sensitivity
    xoffset *= mouseSensitivity;
    yoffset *= mouseSensitivity;
    // adjust the camera angle based on the amount of mouse movement
    yaw += xoffset;
    pitch += yoffset;
    if (constrainPitch) {
        if (pitch > 89.0f) {
            pitch = 89.0f;
        }
        if (pitch < -89.0f) {
            pitch = -89.0f;
        }
    }

    updateCameraVectors();
}

void Camera::ProcessMouseScroll(float yoffset) {
    mouseZoom -= yoffset;
    if (mouseZoom > 45.0f) {
        mouseZoom = 45.0f;
    }
    if (mouseZoom < 1.0f) {
        mouseZoom = 1.0f;
    }
}

void Camera::updateCameraVectors() {
    // calculate new front
    glm::vec3 newFront;
    newFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    newFront.y = sin(glm::radians(pitch));
    newFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(newFront);

    // calculate right vector
    right = glm::normalize(glm::cross(front, worldUp));
    // calculate up vector
    up = glm::normalize(glm::cross(right, front));
}