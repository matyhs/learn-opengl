#include "shader.h"

#include "stb_image.h"

#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

#include <glm/gtc/matrix_transform.hpp>

#include <glm/gtc/type_ptr.hpp>

#include <iostream>
using std::cout;
using std::endl;

#include <cmath>
using std::sin;

float textureVisibility = 0.2f;

void processUserInput(GLFWwindow* window) {
    // close the GLFW window upon hitting escape
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
    else if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        textureVisibility += 0.001f;
        if (textureVisibility >= 1.0f) {
            textureVisibility = 1.0f;
        }
    }
    else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        textureVisibility -= 0.001f;
        if (textureVisibility <= 0.0f) {
            textureVisibility = 0.0f;
        }
    }
}

int main() {
    // Initialize GLFW
    glfwInit();
    // Give hint to GLFW since it is not aware of the OpenGL version that you're using.
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // Give hint to GLFW that we're using the CORE profile, so that we only use the modern functions (no backwards compatibility)
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create the window
    auto window = glfwCreateWindow(800, 600, "Hello world", nullptr, nullptr);
    if (window == nullptr) {
        cout << "Failed to create GLFW window" << endl;
        return -1;
    }

    // Introduce the window to the current context
    glfwMakeContextCurrent(window);
    // Set callback function to the GLFW window on resize
    glfwSetFramebufferSizeCallback(window, [](GLFWwindow* window, int width, int height) {
        if (window != nullptr) {
            // Configure the OpenGL viewport in the window
            glViewport(0, 0, width, height);
        }
    });

    // Initialize GLAD which manages the function pointers for OpenGL 
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        cout << "Failed to initialize GLAD" << endl;
        return -1;
    }

    // import shaders and create the shader program for first triangle
    Shader shader { "./shaders/uniform.vs", "./shaders/uniform.fs" };

    // vertices of the triangle
    GLfloat triangle1[] = {
        // first triangle
        -0.9f, -0.5f, 0.0f,
        -0.1f, -0.5f, 0.0f,
        -0.5f, 0.5f, 0.0f
    };
    // Store the vertex attributes into the GPU's memory through the use of vertex array object (VAO) and vertex buffer object (VBO)
    // VAO is a collection of VBO, with their corresponding attributes 
    // VBO contains the vertex attributes. There can only be one VBO per buffer type
    // 1. Generate the VAO
    GLuint VAO[3];
    glGenVertexArrays(3, VAO);
    // 2. Bind the VAO
    glBindVertexArray(VAO[0]);
    // 3. Generate the VBO
    GLuint VBO[3];
    glGenBuffers(3, VBO);
    // 4. Bind the VBO to a specific buffer. In this scenario, it is bound to the array buffer. 
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    // 5. Store the data to the buffer specified. In this scenario, it will be stored to the VBO bounded to the array buffer.
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangle1), &triangle1, GL_STATIC_DRAW);
    // 6. Provide information to OpenGL on how to read the vertex attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);
    // 7. Unbind the VBO since the vertex attributes are already registered to the VBO. Optional step.
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // 8. Unbind the VAO to prevent accidental update. Optional step.
    glBindVertexArray(0);

    // import shaders and create the shader program for second triangle
    Shader shader2 { "./shaders/inout.vs", "./shaders/inout.fs" };

    GLfloat triangle2[] = {
        // position       // color           
        0.1f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
        0.9f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
    };
    glBindVertexArray(VAO[1]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangle2), &triangle2, GL_STATIC_DRAW);
    // set the position vertex attribute pointer
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)0);
    // enable the vertex attribute. Disabled by default
    glEnableVertexAttribArray(0);
    // set the color vertex attribute pointer
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    // unbind the vertex array and buffer objects
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // import shaders
    Shader shader3 { "./shaders/texture.vs", "./shaders/texture.fs" };
    // create rectangle using element buffer object
    // Element Buffer Object is a buffer that stores indices to help OpenGL on what to draw.
    GLfloat rectangle[] = {
        // position        // color          // texture coords
        0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.85f, 0.85f,
        0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.85f, 0.15f,
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.15f, 0.15f,
        -0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.15f, 0.85f,
    };
    GLuint indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3 // second triangle
    };
    glBindVertexArray(VAO[2]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(rectangle), rectangle, GL_STATIC_DRAW);
    // bind the EBO
    GLuint EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    // set the vertex position attribute pointer
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);
    // set the color attribute pointer
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(3 * sizeof (GLfloat)));
    glEnableVertexAttribArray(1);
    // set the texture coordinates
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);
    // unbind the vertex array and buffer objects
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // load image file and create texture based on the image.
    // Textures are images (e.g. 1D, 2D, 3D) that are used to add details to an object
    // 1. create the texture object
    GLuint textures[4];
    glGenTextures(4, textures);
    // 2. bind the texture object
    glBindTexture(GL_TEXTURE_2D, textures[0]);
    // 3. set texture wrapping
    // Texture wrap defines the behavior of the texture image when texture coodinate lies outside the range of 0 and 1. 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    // 4. set texture filtering
    // Texture filter defines the behavior of the texture image when applying the texture image to a larger object
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    // 5. load image file and generate a texture image
    int width, height, numColorChannel;
    unsigned char *imageData = stbi_load("./textures/container.jpg", &width, &height, &numColorChannel, 0);
    if (imageData) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        cout << "Failed to load image" << endl;
    }
    // 6. Dispose the loaded image
    stbi_image_free(imageData);

    // load the second texture
    glBindTexture(GL_TEXTURE_2D, textures[1]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    stbi_set_flip_vertically_on_load(true);
    imageData = stbi_load("./textures/awesomeface.png", &width, &height, &numColorChannel, 0);
    if (imageData) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        cout << "Failed to load image" << endl;
    }
    stbi_image_free(imageData);

    // set the texture sampler to the texture unit it belongs to. Only need to be set once.
    shader3.Use();
    shader3.SetInt("texture1", 0);
    shader3.SetInt("texture2", 1);

    Shader shader4 { "./shaders/texture.vs", "./shaders/texture.fs" };
    glBindTexture(GL_TEXTURE_2D, textures[2]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    imageData = stbi_load("./textures/container.jpg", &width, &height, &numColorChannel, 0);
    if (imageData) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        cout << "Failed to load image" << endl;
    }
    stbi_image_free(imageData);

    glBindTexture(GL_TEXTURE_2D, textures[3]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    imageData = stbi_load("./textures/awesomeface.png", &width, &height, &numColorChannel, 0);
    if (imageData) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        cout << "Failed to load image" << endl;
    }
    stbi_image_free(imageData);

    shader4.Use();
    shader4.SetInt("texture1", 0);
    shader4.SetInt("texture2", 1);

    // Render Loop. Loop till GLFW window is closed
    while (!glfwWindowShouldClose(window)) {
        // listen to user input
        processUserInput(window);

        // Specify the color value that OpenGL uses to reset the color buffer
        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        // Clears the buffer. In this scenario, the buffer is cleared with the color specfied in glClearColor.
        glClear(GL_COLOR_BUFFER_BIT);

        // draw the triangle
        // Activate shader program
        shader.Use();
        // send color data to fragment shader via uniform. This will send the data directly from CPU to GPU
        // get the current running time to create various colors
        double timeValue = glfwGetTime();
        // use the sin function to generate a value between 0 to 1 based on the current time value
        double greenValue = (sin(timeValue) / 2.0f) + 0.5f;
        // query the location of the uniform variable
        int vertexColorLocation = glGetUniformLocation(shader.GetProgramId(), "vertexColor");
        // Set the color of the triangle
        glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);
        shader.SetFloat("vertexHOffset", 0.2f);

        // Bind the vertex array object containing the vertex attributes
        glBindVertexArray(VAO[0]);
        // Draw command
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // draw second triangle
        shader2.Use();
        glBindVertexArray(VAO[1]);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // do a transformation on the object
        // When combining matrices for transformation, always scale first, then rotate, and lastly, translate. 
        // 1. create an identity matrix with initialized values of 1.
        glm::mat4 trans { 1.0f };
        // 2. translate the location of the object
        trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
        // 3. rotate the object along the z-axis
        trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3{0.0, 0.0, 1.0});
        // 4. scale the object by half its original size
        trans = glm::scale(trans, glm::vec3{0.5, 0.5, 0.5});

        // draw rectangle
        shader3.Use();
        shader3.SetFloat("textureVisibility", textureVisibility);
        // set the transformation matrix.
        GLuint transformLocation = glGetUniformLocation(shader3.GetProgramId(), "transform");
        glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(trans));
        // Optionally, we need to activate the texture unit, as some graphics driver doesn't activate the texture unit 0 by default
        glActiveTexture(GL_TEXTURE0);
        // bind texture
        glBindTexture(GL_TEXTURE_2D, textures[0]);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, textures[1]);
        glBindVertexArray(VAO[2]);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glm::mat4 trans1 { 1.0f };
        trans1 = glm::translate(trans1, glm::vec3{ -0.5f, -0.5f, 0.0f });
        trans1 = glm::scale(trans1, glm::vec3{ 0.3, 0.3, 0.3 });
        shader4.Use();
        GLuint transformLocation1 = glGetUniformLocation(shader4.GetProgramId(), "transform");
        glUniformMatrix4fv(transformLocation1, 1, GL_FALSE, glm::value_ptr(trans1));
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, textures[2]);
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, textures[3]);
        glBindVertexArray(VAO[2]);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // unbind vertex array
        glBindTexture(GL_TEXTURE_2D, 0);
        glBindVertexArray(0);

        // swaps the color buffer. Double buffer is used for rendering to prevent flickering. Swap occurs when the rendering commands are finished.
        // - A front buffer contains the color values for each pixel that serves as the final output image shown in the screen.
        // - A back buffer contains all of the output drawn by the rendering commands
        glfwSwapBuffers(window);
        // Takes care of the GLFW events (e.g. keyboard input, mouse events)
        glfwPollEvents();
    }

    // Deallocate resources
    glDeleteVertexArrays(3, VAO);
    glDeleteBuffers(3, VBO);
    glDeleteBuffers(1, &EBO);
    
    // Clears GLFW resources
    glfwDestroyWindow(window);
    glfwTerminate();
    window = nullptr;

    return 0;
}