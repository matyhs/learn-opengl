#include "shader.h"

using std::string;
using std::ifstream;
using std::stringstream;
using std::cout;
using std::endl;

Shader::Shader(string vertexShaderPath, string fragmentShaderPath) {
    string vertexCode;
    string fragmentCode;
    ifstream vertexShaderFile;
    ifstream fragmentShaderFile;
    // make sure that ifstream object can throw exception
    vertexShaderFile.exceptions(ifstream::failbit | ifstream::badbit);
    fragmentShaderFile.exceptions(ifstream::failbit | ifstream::badbit);
    try {
        // 1. Open the shader files
        vertexShaderFile.open(vertexShaderPath);
        fragmentShaderFile.open(fragmentShaderPath);
        // 2. Read shader files buffer content as string stream
        stringstream vertexShaderStream, fragmentShaderStream;
        vertexShaderStream << vertexShaderFile.rdbuf();
        fragmentShaderStream << fragmentShaderFile.rdbuf();
        // 3. Close shader files
        vertexShaderFile.close();
        fragmentShaderFile.close();
        // 4. Convert stream to string
        vertexCode = vertexShaderStream.str();
        fragmentCode = fragmentShaderStream.str();
    } catch (ifstream::failure e) {
        cout << "Unable to read shader file" << endl;
    }
    // transform to C string
    char const *vShaderCode = vertexCode.c_str();
    char const *fShaderCode = fragmentCode.c_str();

    // compile the shader code
    GLint success;
    GLchar infoLog[512];
    // Shaders are functions for processing the vertex attributes. They are executed in the graphics pipeline. To use multiple shaders, we link all shaders into a Shader program 
    // In OpenGL, both the vertex shader and fragment shader are required to be implemented because there are no default shaders for them. 
    // Geometry shader is optional. For shape assembly, rasterization, and test & blending shaders, these are already provided by the OpenGL library.
    // Vertex Shader is used to transform 3D coordinates
    // 1. Create a vertex shader object
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    // 2. Attach vertex shader source code to the vertex shader object
    glShaderSource(vertexShader, 1, &vShaderCode, nullptr);
    // 3. Compile the vertex shader source code
    glCompileShader(vertexShader);
    // 4. Verify if vertex shader source code compiles successfully
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
        cout << "Vertex shader compilation error" << infoLog << endl;
    }
    // Fragment Shader calculates the final color of a pixel
    // 1. Create a fragment shader object
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    // 2. Attach fragment shader source code to the fragment shader object;
    glShaderSource(fragmentShader, 1, &fShaderCode, nullptr);
    // 3. Compile the fragment shader source code
    glCompileShader(fragmentShader);
    // 4. Verify if fragment shader source code compiles successfully;
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
        cout << "Fragment shader compilation error" << infoLog << endl;
    }
    // Shader program is a compilation of multiple linked shaders
    // 1. Create a shader program
    program_id = glCreateProgram();
    // 2. Attach the related shaders to the shader program
    glAttachShader(program_id, vertexShader);
    glAttachShader(program_id, fragmentShader);
    // 3. Link all of the shaders to the shader program
    glLinkProgram(program_id);
    // 4. Verify if shaders link successfully to the shader program
    glGetProgramiv(program_id, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(program_id, 512, nullptr, infoLog);
        cout << "Shader program link error" << infoLog << endl;
    }
    // clean up shaders
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

Shader::~Shader() {
    glDeleteProgram(program_id);
}

void Shader::Use() const {
    glUseProgram(program_id);
}

void Shader::SetBool(string const &uniform, bool value) const {
    glUniform1i(glGetUniformLocation(program_id, uniform.c_str()), (int)value);
}

void Shader::SetInt(string const &uniform, int value) const {
    glUniform1i(glGetUniformLocation(program_id, uniform.c_str()), value);
}

void Shader::SetFloat(string const &uniform, float value) const {
    glUniform1f(glGetUniformLocation(program_id, uniform.c_str()), value);
}

void Shader::SetFloatMatrix(string const &uniform, glm::mat4 matrix) const {
    glUniformMatrix4fv(glGetUniformLocation(program_id, uniform.c_str()), 1, GL_FALSE, &matrix[0][0]);
}

void Shader::SetFloatVec3(std::string const &uniform, float x, float y, float z) const {
    glUniform3f(glGetUniformLocation(program_id, uniform.c_str()), x, y, z);
}

void Shader::SetFloatVec3(std::string const &uniform, glm::vec3 vector) const {
    glUniform3fv(glGetUniformLocation(program_id, uniform.c_str()), 1, &vector[0]);
}