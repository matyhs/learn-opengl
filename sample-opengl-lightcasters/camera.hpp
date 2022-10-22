#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

float const YAW = -90.0f;
float const PITCH = 0.0f;
float const SPEED = 2.5f;
float const SENSITIVITY = 0.1f;
float const ZOOM = 45.0f;

enum Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

class Camera {
    private:
        glm::vec3 position; // camera position
        glm::vec3 front; // camera direction
        glm::vec3 up; // camera y-axis
        glm::vec3 right; // camera x-axis
        glm::vec3 worldUp; // camera up vector in world space
        float pitch; // camera angle on the x-axis
        float yaw; // camera angle on the y-axis
        float moveSpeed; // movement speed 
        float mouseSensitivity; // mouse sensitivity
        float mouseZoom; // mouse zoom
    public:
        Camera(glm::vec3 position = glm::vec3 { 0.0f, 0.0f, 0.0f },
                glm::vec3 up = glm::vec3 { 0.0f, 1.0f, 0.0f },
                float yaw = YAW, float pitch = PITCH);
        Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);
        // To define a camera, we need to have its position in the world space, the direction it is looking at, the right and up axis.
        // In order to simulate a camera, we use LookAt function which create a matrix using the Gram-Schmidt process. 
        glm::mat4 GetViewMatrix() const { return glm::lookAt(position, position + front, up); }
        glm::mat4 GetViewMatrixManual() const { 
            // 1. Position = known
            // 2. Calculate cameraDirection
            glm::vec3 zaxis = glm::normalize(position - (position + front));
            // 3. Get positive right axis vector
            glm::vec3 xaxis = glm::normalize(glm::cross(glm::normalize(worldUp), zaxis));
            // 4. Calculate camera up vector
            glm::vec3 yaxis = glm::cross(zaxis, xaxis);

            // Create translation and rotation matrix
            // In glm we access elements as mat[col][row] due to column-major layout
            glm::mat4 translation = glm::mat4(1.0f); // Identity matrix by default
            translation[3][0] = -position.x; // Third column, first row
            translation[3][1] = -position.y;
            translation[3][2] = -position.z;
            glm::mat4 rotation = glm::mat4(1.0f);
            rotation[0][0] = xaxis.x; // First column, first row
            rotation[1][0] = xaxis.y;
            rotation[2][0] = xaxis.z;
            rotation[0][1] = yaxis.x; // First column, second row
            rotation[1][1] = yaxis.y;
            rotation[2][1] = yaxis.z;
            rotation[0][2] = zaxis.x; // First column, third row
            rotation[1][2] = zaxis.y;
            rotation[2][2] = zaxis.z; 

            // Return lookAt matrix as combination of translation and rotation matrix
            return rotation * translation; // Remember to read from right to left (first translation then rotation)
        }
        void ProcessKeyboard(Movement direction, float deltaTime);
        void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true);
        void ProcessMouseScroll(float yoffset);
        float GetZoom() const { return glm::radians(mouseZoom); }
        glm::vec3 GetPosition() const { return position; }
        glm::vec3 GetDirection() const { return front; }
    protected:
        void updateCameraVectors();
};