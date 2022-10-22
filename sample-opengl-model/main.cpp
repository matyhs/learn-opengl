#include "camera.hpp"

#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
using std::cout;
using std::endl;

#include "model.hpp"

unsigned int const WIDTH = 800;
unsigned int const HEIGHT = 600;

Camera camera { glm::vec3 { 0.0f, 0.0f, 3.0f } };
float deltaTime = 0.0f;
float lastFrame = 0.0f; 
float lastX = WIDTH / 2.0f;
float lastY = HEIGHT / 2.0f;
bool firstMouse = true;

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera.ProcessKeyboard(FORWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera.ProcessKeyboard(LEFT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera.ProcessKeyboard(RIGHT, deltaTime);
    }
}

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    auto window = glfwCreateWindow(WIDTH, HEIGHT, "3D Model", nullptr, nullptr);
    if (window == nullptr) {
        cout << "Failed to create GLFW window" << endl;
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, [](GLFWwindow*, int width, int height) {
        glViewport(0, 0, width, height);
    });
    glfwSetCursorPosCallback(window, [](GLFWwindow*, double xposin, double yposin) {
        float xpos = static_cast<float>(xposin);
        float ypos = static_cast<float>(yposin);
        if (firstMouse) {
            lastX = xpos;
            lastY = ypos;
            firstMouse = false;
        }

        float xoffset = xpos - lastX;
        float yoffset = lastY - ypos;

        lastX = xpos;
        lastY = ypos;
        camera.ProcessMouseMovement(xoffset, yoffset);
    });
    glfwSetScrollCallback(window, [](GLFWwindow*, double, double yoffset) {
        camera.ProcessMouseScroll(static_cast<float>(yoffset));
    });

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        cout << "Unable to initialize GLAD" << endl;
        return -1;
    }

    Shader shader { "./shader/model.vs", "./shader/model.fs" };
    // Instantiate the model from file
    Model backpack { "./model/backpack.obj" };

    shader.Use();

    glm::vec3 lightColor { 1.0 };
    glm::vec3 diffuseColor = lightColor * glm::vec3 { 0.8f };
    glm::vec3 ambientColor = diffuseColor * glm::vec3 { 0.2f };
    glm::vec3 specularColor = lightColor * glm::vec3 { 0.5f };
    // directional light properties
    shader.SetFloatVec3("directionalLight.ambient", ambientColor);
    shader.SetFloatVec3("directionalLight.diffuse", diffuseColor);
    shader.SetFloatVec3("directionalLight.specular", specularColor);
    shader.SetFloatVec3("directionalLight.direction", glm::vec3 { -0.2f, -1.0f, -0.3f });
    // point light properties
    glm::vec3 pointLightPos { -4.0f,  2.0f, 2.0f };
    shader.SetFloatVec3("pointLight.ambient", ambientColor);
    shader.SetFloatVec3("pointLight.diffuse", diffuseColor);
    shader.SetFloatVec3("pointLight.specular", specularColor);
    shader.SetFloatVec3("pointLight.position", pointLightPos);
    shader.SetFloat("pointLight.constant", 1.0f);
    shader.SetFloat("pointLight.linear", 0.045f);
    shader.SetFloat("pointLight.quadratic", 0.0075f);

    glEnable(GL_DEPTH_TEST);
    glClearColor(0.05f, 0.05f, 0.05f, 1.0f);

    while (!glfwWindowShouldClose(window)) {
        processInput(window);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.Use();

        glm::mat4 projection = glm::perspective(glm::radians(camera.GetZoom()), (float)WIDTH/(float)HEIGHT, 0.1f, 100.0f);
        shader.SetFloatMatrix("projection", projection);

        glm::mat4 view = camera.GetViewMatrix();
        shader.SetFloatMatrix("view", view);

        glm::mat4 model { 1.0f };
        model = glm::rotate(model, glm::radians(25.0f), glm::vec3 { 0.5f, 1.0f, 0.0f });
        model = glm::translate(model, glm::vec3 { 0.0f, 0.0f, 0.0f });
        model = glm::scale(model, glm::vec3 { 0.01f, 0.01f, 0.01f });
        shader.SetFloatMatrix("model", model);

        backpack.Draw(shader);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}