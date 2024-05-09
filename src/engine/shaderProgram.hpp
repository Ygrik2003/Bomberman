#pragma once

#include <glad/glad.h>
#include <iostream>
#include <string>
#include <fstream>
#include <glm/glm.hpp>
#include "fileReader.hpp"

class ShaderProgram{
    private:
        uint32_t vertexShader;
        uint32_t fragmentShader;
        uint32_t shaderProgram;
        std::string getSourceCode(std::string& filePath);
        void settingUp(uint32_t vShader, uint32_t fShader);
    public:
        ShaderProgram(const char* vertexPath, const char* fragmentPath);
        ShaderProgram(uint32_t vShader, const char* fragmentPath);
        ShaderProgram(const char* vertexPath, uint32_t fShader);
        ShaderProgram(uint32_t vShader, uint32_t fShader);
        ~ShaderProgram();
        uint32_t getShaderProgram();
        uint32_t compileShader(std::string path, uint32_t shaderType);
        void use();
        void setBool(const std::string& name, bool value);
        void setInt(const std::string& name, int32_t value);
        void setFloat(const std::string& name, float value);
        void setVec2(const std::string& name, glm::vec2 value);
        void setVec4(const std::string& name, glm::vec4 value);
        void setMat4(const std::string& name, glm::mat4 value);
};