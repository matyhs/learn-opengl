#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

#include <glm/gtc/matrix_transform.hpp>

#include <glm/gtc/type_ptr.hpp>

#include <iostream>
using std::cout;
using std::endl;

#include "camera.hpp"

#include "shader.h"

#include "stb_image.h"

unsigned int const SCR_WIDTH = 800;
unsigned int const SCR_HEIGHT = 600; 

Camera camera { glm::vec3 { 0.0f, 0.0f, 3.0f } };
float deltaTime = 0.0f; // time between current frame and last frame
float lastFrame = 0.0f; // time of last frame
float lastX = SCR_WIDTH / 2.0f; // mouse position at x-axis
float lastY = SCR_HEIGHT / 2.0f; // mouse position at y-axis
bool firstMouse = true; // initial mouse movement

void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera.ProcessKeyboard(FORWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera.ProcessKeyboard(RIGHT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera.ProcessKeyboard(LEFT, deltaTime);
    }
}

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    auto window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "3D Graphics", nullptr, nullptr);
    if (window == nullptr) {
        cout << "Unable to create GLFW window" << endl;
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, [](GLFWwindow* window, int width, int height) {
        if (window != nullptr) {
            glViewport(0, 0, width, height);
        }
    });
    glfwSetCursorPosCallback(window, [](GLFWwindow*, double xposRaw, double yposRaw) {        
        float xpos = static_cast<float>(xposRaw);
        float ypos = static_cast<float>(yposRaw);
        
        // set the initial state of the mouse cursor upon entering the window
        if (firstMouse) {
            lastX = xpos;
            lastY = ypos;
            firstMouse = false;
        }

        // calculate for the offset of mouse movement in the x and y axis
        float xoffset = xpos - lastX;
        float yoffset = lastY - ypos;
        lastX = xpos;
        lastY = ypos;

        camera.ProcessMouseMovement(xoffset, yoffset);
    });
    glfwSetScrollCallback(window, [](GLFWwindow*, double, double yoffset) {
        camera.ProcessMouseScroll(yoffset);
    });

    // disable mouse cursor
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        cout << "Unable to initialize GLAD" << endl;
        return -1;
    }

    GLuint VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // The space consumed by an object locally is called a local space. This is irrespective to the world that the object will be imported to.
    GLfloat vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

    // cube positions    
    glm::vec3 cubePositions[] = {
        glm::vec3( 0.0f,  0.0f,  0.0f), 
        glm::vec3( 2.0f,  5.0f, -15.0f), 
        glm::vec3(-1.5f, -2.2f, -2.5f),  
        glm::vec3(-3.8f, -2.0f, -12.3f),  
        glm::vec3( 2.4f, -0.4f, -3.5f),  
        glm::vec3(-1.7f,  3.0f, -7.5f),  
        glm::vec3( 1.3f, -2.0f, -2.5f),  
        glm::vec3( 1.5f,  2.0f, -2.5f), 
        glm::vec3( 1.5f,  0.2f, -1.5f), 
        glm::vec3(-1.3f,  1.0f, -1.5f)  
    };

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    Shader shader { "./shaders/crate.vs", "./shaders/crate.fs" };

    int width, height, numColorChannel;
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    unsigned char *imageData = stbi_load("./textures/container.jpg", &width, &height, &numColorChannel, 0);
    if (imageData) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        cout << "Unable to load image data" << endl;
        return -1;
    }
    stbi_image_free(imageData);

    shader.Use();
    shader.SetInt("texture1", 0);

    // enable depth testing, which is a process in OpenGL to make sure that depth values is considered when there are overlapping fragments.
    // OpenGL uses the z-buffer to determine the depth of a fragment. This is automatically created by OpenGL.
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.07f, 0.13f, 0.17f, 1.0f);

    while(!glfwWindowShouldClose(window)) {
        // calculate for the delta time for each frame
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);

        // clears both the color buffer and the depth buffer before each render iteration
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        glBindVertexArray(VAO);

        // Now, we must consider the space that the object consumes respective of the camera's point of view. This is called a view space.
        // To represent the coordinates that an object lies within the view space, we use a view matrix
        glm::mat4 view = camera.GetViewMatrix();
        shader.SetFloatMatrix("view", glm::value_ptr(view));

        // Lastly, since we're working with 3D object, we must tell OpenGL how to project the 3D coordinates into a 2D normalized device coordinates (NDC). 
        // In OpenGL, NDC are expected to fall within the range of -1.0 and 1.0, and anything that falls outside of that is clipped. This is called a clip space.
        // To represent the converted coordinates, we use a projection matrix. The viewing box that the project matrix creates is called a frustum, and anything that lies inside the frustum will be seen in the screen.
        // A projection is the process of converting 3D coordinates to 2D normalized device coordinates.
        // There are two types of projection: orthographic & perspective. The main difference is that perspective projection will consider the homogenous w component of a vertex, which determines the perspective
        glm::mat4 projection { 1.0f };
        projection = glm::perspective(camera.GetZoom(), (float)SCR_WIDTH/(float)SCR_HEIGHT, 0.1f, 100.0f);
        shader.SetFloatMatrix("projection", glm::value_ptr(projection));

        for ( unsigned int i = 0; i < 10; i++ )
        {
            // Once an object is imported to a world, the space it takes respective to the world is called a world space. 
            // To represent the coordinates that an object lies within the world space, we use a model matrix 
            glm::mat4 model { 1.0f };
            model = glm::translate(model, cubePositions[i]);
            if (i % 3 == 0)
            {
                float angle = 20.0f * (i + 1);
                model = glm::rotate(model, (float)glfwGetTime() * glm::radians(angle), glm::vec3{ 0.5f, 1.0f, 0.0f });
            }
            
            shader.SetFloatMatrix("model", glm::value_ptr(model));
            
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        glBindTexture(GL_TEXTURE_2D, 0);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteTextures(1, &texture);

    glfwDestroyWindow(window);
    glfwTerminate();
    window = nullptr;

    return 0;
}