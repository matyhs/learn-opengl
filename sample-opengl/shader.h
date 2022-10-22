#include <glad/glad.h>
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
        void SetFloat(std::string const &uniform, float value);
        void SetInt(std::string const &uniform, int value);
        void SetBool(std::string const &uniform, bool value);
};