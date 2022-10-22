#include "camera.hpp"

#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

#include <glm/gtc/matrix_transform.hpp>

#include <glm/gtc/type_ptr.hpp>

#include <iostream>
using std::cout;
using std::endl;

#include "shader.h"

#include "stb_image.h"

unsigned int const WIDTH = 800;
unsigned int const HEIGHT = 600;

Camera camera { glm::vec3 { 0.5f, 1.0f, 5.0f } };
float deltaTime = 0.0f;
float lastFrame = 0.0f; 
float lastX = WIDTH / 2.0f;
float lastY = HEIGHT / 2.0f;
bool firstMouse = true;
glm::vec3 lightPos { 1.2f, 0.0f, 2.0f };

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

int loadTexture(char const *path) {
    GLuint textureId = 0;
    int width, height, numOfColorChannel;
    unsigned char *imageData = stbi_load(path, &width, &height, &numOfColorChannel, 0);

    if (imageData) {
        glGenTextures(1, &textureId);
        glBindTexture(GL_TEXTURE_2D, textureId);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        GLenum format = GL_RGBA;
        if (numOfColorChannel == 1) {
            format = GL_RED;
        } else if (numOfColorChannel == 3) {
            format = GL_RGB;
        } else if (numOfColorChannel == 4) {
            format = GL_RGBA;
        }

        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, imageData);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        cout << "Failed to load image" << endl;
    }

    stbi_image_free(imageData);
    return textureId;
}

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    auto window = glfwCreateWindow(WIDTH, HEIGHT, "Lighting", nullptr, nullptr);
    if (window == nullptr) {
        cout << "Unable to create GLFW window" << endl;
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

    // vertex for cube
    GLfloat vertices[] = {
    // positions          // normals           // texture coords
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
    };

    GLuint VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);

    // create a VAO and VBO for the light source.
    // in this scenario, we'll be using the same cube vertices for the light source
    GLuint lightVAO;
    glGenVertexArrays(1, &lightVAO);

    glBindVertexArray(lightVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    GLuint diffuseMap = loadTexture("./textures/container2.png");
    GLuint specularMap = loadTexture("./textures/container2_specular.png");
    GLuint emissionMap = loadTexture("./textures/matrix.jpg");

    Shader lightingShader { "./shaders/lighting.vs", "./shaders/lighting.fs" }; // light source shader
    Shader cubeShader { "./shaders/cubetexture.vs", "./shaders/cubetexture.fs" }; // target object shader
    
    cubeShader.Use();
    cubeShader.SetInt("material.diffuse", 0);
    cubeShader.SetInt("material.specular", 1);
    cubeShader.SetInt("material.emission", 2);

    glEnable(GL_DEPTH_TEST);
    glClearColor(0.07f, 0.13f, 0.17f, 1.0f);

    while (!glfwWindowShouldClose(window)) {
        float current = static_cast<float>(glfwGetTime());
        deltaTime = current - lastFrame;
        lastFrame = current;

        processInput(window);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Draw target object
        cubeShader.Use();
        // cubeShader.SetFloatVec3("objectColor", 1.0f, 0.5f, 0.31f);
        // cubeShader.SetFloatVec3("lightColor", 1.0f, 1.0f, 1.0f);
        cubeShader.SetFloatVec3("lightPos", lightPos);
        // cubeShader.SetFloatVec3("viewPos", camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
        // cubeShader.SetFloatVec3("material.ambient", 1.0f, 0.5f, 0.31f);
        // cubeShader.SetFloatVec3("material.diffuse", 1.0f, 0.5f, 0.31f);
        // cubeShader.SetFloatVec3("material.specular", 0.5f, 0.5f, 0.5f);
        glm::vec3 emissionColor { 1.0, 0.0, 0.0 };
        // emissionColor.x = sin(glfwGetTime());
        cubeShader.SetFloatVec3("material.emissionColor", emissionColor);
        cubeShader.SetFloat("material.shininess", 32.0f);
        cubeShader.SetFloatVec3("light.position", lightPos);
        // changing object color
        glm::vec3 lightColor { 1.0 };
        // lightColor.x = sin(glfwGetTime() * 2.0f);
        // lightColor.y = sin(glfwGetTime() * 0.7f);
        // lightColor.z = sin(glfwGetTime() * 1.3f);
        glm::vec3 diffuseColor = lightColor * glm::vec3 { 0.5f };
        glm::vec3 ambientColor = diffuseColor * glm::vec3 { 0.2f };
        cubeShader.SetFloatVec3("light.ambient", ambientColor);
        cubeShader.SetFloatVec3("light.diffuse", diffuseColor);
        cubeShader.SetFloatVec3("light.specular", glm::vec3 { 1.0f });
        glm::mat4 view = camera.GetViewMatrix();
        cubeShader.SetFloatMatrix("view", view);
        glm::mat4 projection = glm::perspective(camera.GetZoom(), (float)WIDTH/(float)HEIGHT, 0.1f, 100.0f );
        cubeShader.SetFloatMatrix("projection", projection);
        glm::mat4 model { 1.0f };
        model = glm::rotate(model, (float)glfwGetTime() * glm::radians(25.0f), glm::vec3 { 0.5f, 1.0f, 2.0f });
        cubeShader.SetFloatMatrix("model", model); 
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, diffuseMap); 
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, specularMap); 
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, emissionMap);
        glBindVertexArray(VAO);   
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // Draw light source
        lightingShader.Use();
        lightingShader.SetFloatVec3("lightColor", glm::vec3 { 1.0 });
        lightingShader.SetFloatMatrix("view", view);
        lightingShader.SetFloatMatrix("projection", projection);
        model = glm::mat4 { 1.0f };
        lightPos.x = sin(glfwGetTime());
        lightPos.z = cos(glfwGetTime());
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3 { 0.2f });
        lightingShader.SetFloatMatrix("model", model);
        glBindVertexArray(lightVAO);   
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    glDeleteVertexArrays(1, &lightVAO);

    glDeleteTextures(1, &diffuseMap);

    glfwDestroyWindow(window);
    glfwTerminate();
    window = nullptr;

    return 0;
}