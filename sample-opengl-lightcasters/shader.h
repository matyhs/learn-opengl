#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

class Shader {
    private:
        int program_id;
    public:
        Shader(std::string vertexShaderPath, std::string fragmentShaderPath);
        ~Shader();
        int GetProgramId() const { return program_id; }
        void Use() const;
        void SetFloat(std::string const &uniform, float value) const;
        void SetInt(std::string const &uniform, int value) const;
        void SetBool(std::string const &uniform, bool value) const;
        void SetFloatMatrix(std::string const &uniform, glm::mat4 matrix) const;
        void SetFloatVec3(std::string const &uniform, float x, float y, float z) const;
        void SetFloatVec3(std::string const &uniform, glm::vec3 vector) const;
};