#include "camera.hpp"

#include <GLFW/glfw3.h>

#include <iostream>
using std::cout;
using std::endl;

#include "shader.h"

#include "texture.hpp"

unsigned int const WIDTH = 800;
unsigned int const HEIGHT = 600;

Camera camera { glm::vec3 { 0.5f, 1.0f, 5.0f } };
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

    auto window = glfwCreateWindow(WIDTH, HEIGHT, "Light Casters", nullptr, nullptr);
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

    // cube positions
    glm::vec3 cubePositions[] = {
        glm::vec3 { 0.0f,  0.0f,  0.0f },
        glm::vec3 { 2.0f,  5.0f, -15.0f },
        glm::vec3 { -1.5f, -2.2f, -2.5f },
        glm::vec3 { -3.8f, -2.0f, -12.3f },
        glm::vec3 { 2.4f, -0.4f, -3.5f },
        glm::vec3 { -1.7f,  3.0f, -7.5f },
        glm::vec3 { 1.3f, -2.0f, -2.5f },
        glm::vec3 { 1.5f,  2.0f, -2.5f },
        glm::vec3 { 1.5f,  0.2f, -1.5f },
        glm::vec3 { -1.3f,  1.0f, -1.5f }
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

    // light source
    GLuint lightVAO;
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*) 0);
    glEnableVertexAttribArray(0);

    Texture diffuseMap { "./textures/container2.png", 0 };
    Texture specularMap { "./textures/container2_specular.png", 1 };

    Shader cubeShader { "./shaders/cubecombine.vs", "./shaders/cubecombine.fs" };
    Shader lightShader { "./shaders/light.vs", "./shaders/light.fs" };

    cubeShader.Use();
    // material properties
    cubeShader.SetInt("material.diffuse", diffuseMap.GetTextureUnit());
    cubeShader.SetInt("material.specular", specularMap.GetTextureUnit());
    cubeShader.SetFloat("material.shininess", 32.0f);

    glm::vec3 lightColor { 1.0 };
    glm::vec3 diffuseColor = lightColor * glm::vec3 { 0.8f };
    glm::vec3 ambientColor = diffuseColor * glm::vec3 { 0.05f };
    glm::vec3 specularColor = lightColor * glm::vec3 { 0.5f };
    // directional light properties
    cubeShader.SetFloatVec3("directionalLight.ambient", ambientColor);
    cubeShader.SetFloatVec3("directionalLight.diffuse", diffuseColor);
    cubeShader.SetFloatVec3("directionalLight.specular", specularColor);
    cubeShader.SetFloatVec3("directionalLight.direction", glm::vec3 { -0.2f, -1.0f, -0.3f });
    // point light properties
    lightColor = glm::vec3 { 1.0, 0.0, 0.0 };
    diffuseColor = lightColor * glm::vec3 { 0.3f };
    ambientColor = diffuseColor * glm::vec3 { 0.05f };
    specularColor = lightColor * glm::vec3 { 1.0f };
    glm::vec3 pointLightPositions[] = {
        glm::vec3( 0.7f,  0.2f,  2.0f),
        glm::vec3( 2.3f, -3.3f, -4.0f),
        glm::vec3(-4.0f,  2.0f, -12.0f),
        glm::vec3( 0.0f,  0.0f, -3.0f)
    };
    cubeShader.SetFloatVec3("pointLights[0].ambient", ambientColor);
    cubeShader.SetFloatVec3("pointLights[0].diffuse", diffuseColor);
    cubeShader.SetFloatVec3("pointLights[0].specular", specularColor);
    cubeShader.SetFloatVec3("pointLights[0].position", pointLightPositions[0]);
    cubeShader.SetFloat("pointLights[0].constant", 1.0f);
    cubeShader.SetFloat("pointLights[0].linear", 0.045f);
    cubeShader.SetFloat("pointLights[0].quadratic", 0.0075f);

    cubeShader.SetFloatVec3("pointLights[1].ambient", ambientColor);
    cubeShader.SetFloatVec3("pointLights[1].diffuse", diffuseColor);
    cubeShader.SetFloatVec3("pointLights[1].specular", specularColor);
    cubeShader.SetFloatVec3("pointLights[1].position", pointLightPositions[1]);
    cubeShader.SetFloat("pointLights[1].constant", 1.0f);
    cubeShader.SetFloat("pointLights[1].linear", 0.045f);
    cubeShader.SetFloat("pointLights[1].quadratic", 0.0075f);

    cubeShader.SetFloatVec3("pointLights[2].ambient", ambientColor);
    cubeShader.SetFloatVec3("pointLights[2].diffuse", diffuseColor);
    cubeShader.SetFloatVec3("pointLights[2].specular", specularColor);
    cubeShader.SetFloatVec3("pointLights[2].position", pointLightPositions[2]);
    cubeShader.SetFloat("pointLights[2].constant", 1.0f);
    cubeShader.SetFloat("pointLights[2].linear", 0.045f);
    cubeShader.SetFloat("pointLights[2].quadratic", 0.0075f);

    cubeShader.SetFloatVec3("pointLights[3].ambient", ambientColor);
    cubeShader.SetFloatVec3("pointLights[3].diffuse", diffuseColor);
    cubeShader.SetFloatVec3("pointLights[3].specular", specularColor);
    cubeShader.SetFloatVec3("pointLights[3].position", pointLightPositions[3]);
    cubeShader.SetFloat("pointLights[3].constant", 1.0f);
    cubeShader.SetFloat("pointLights[3].linear", 0.045f);
    cubeShader.SetFloat("pointLights[3].quadratic", 0.0075f);
    // spotlight properties
    lightColor = glm::vec3 { 1.0 };
    diffuseColor = lightColor * glm::vec3 { 1.0f };
    ambientColor = diffuseColor * glm::vec3 { 0.0f };
    cubeShader.SetFloatVec3("spotlight.ambient", ambientColor);
    cubeShader.SetFloatVec3("spotlight.diffuse", diffuseColor);
    cubeShader.SetFloatVec3("spotlight.specular", specularColor);
    cubeShader.SetFloat("spotlight.constant", 1.0f);
    cubeShader.SetFloat("spotlight.linear", 0.09f);
    cubeShader.SetFloat("spotlight.quadratic", 0.032f);
    cubeShader.SetFloat("spotlight.innerCutOff", glm::cos(glm::radians(12.5f)));
    cubeShader.SetFloat("spotlight.outerCutOff", glm::cos(glm::radians(17.5f)));

    glEnable(GL_DEPTH_TEST);
    glClearColor(0.07f, 0.13f, 0.17f, 1.0f);

    while (!glfwWindowShouldClose(window)) {
        float current = static_cast<float>(glfwGetTime());
        deltaTime = current - lastFrame;
        lastFrame = current;
        
        processInput(window);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glBindVertexArray(VAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, diffuseMap.GetTextureId());
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, specularMap.GetTextureId());

        cubeShader.Use();
        cubeShader.SetFloatVec3("spotlight.position", camera.GetPosition());
        cubeShader.SetFloatVec3("spotlight.direction", camera.GetDirection());

        glm::mat4 view = camera.GetViewMatrix();
        cubeShader.SetFloatMatrix("view", view);

        glm::mat4 projection = glm::perspective(camera.GetZoom(), (float) WIDTH / (float) HEIGHT, 0.1f, 100.0f);
        cubeShader.SetFloatMatrix("projection", projection);

        for ( unsigned int i = 0; i < 10; i++ ) {
            glm::mat4 model { 1.0 };
            model = glm::translate(model, cubePositions[i]);
            if (i % 3 == 0) {
                float angle = 20.0f * (i + 1);
                model = glm::rotate(model, (float) glfwGetTime() * glm::radians(angle), glm::vec3 { 0.5f, 1.0f, 0.0f });
            }
            cubeShader.SetFloatMatrix("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        lightShader.Use();
        lightShader.SetFloatMatrix("view", view);
        lightShader.SetFloatMatrix("projection", projection);
        for ( unsigned int i = 0; i < 4; i++) {
            glm::mat4 model { 1.0f };
            model = glm::translate(model, pointLightPositions[i]);
            model = glm::rotate(model, glm::radians(45.0f), glm::vec3 { 0.0f, 1.0f, 1.0f });
            model = glm::scale(model, glm::vec3 { 0.2f } );
            lightShader.SetFloatMatrix("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteVertexArrays(1, &lightVAO);
    glDeleteBuffers(1, &VBO);
    GLuint diffuseMapId = diffuseMap.GetTextureId();
    glDeleteTextures(1, &diffuseMapId);
    GLuint specularMapId = specularMap.GetTextureId();
    glDeleteTextures(1, &specularMapId);

    glfwDestroyWindow(window);
    glfwTerminate();
    window = nullptr;

    return 0;
}