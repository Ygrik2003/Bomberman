#pragma once

#include <glad/glad.h>
#include <map>
#include "shaderProgram.hpp"

class Render {
    private:
        bool isLoaded = false;
        ShaderProgram* shaderProgram;
        uint32_t VBO, VAO, EBO;
        std::map<int32_t, uint32_t> texturesID;
        // std::vector<uint32_t> texturesID;
        void loadTexture(int32_t number, const char* path);
        void initialLoading(std::map<int32_t, std::string> pathes);
        void deleteBuffers();

    public:
        Render(std::map<int32_t, std::string> pathes);
        ~Render();
        void initShader(std::string vertShaderPath, std::string fragShaderPath);
        void setUniformBool(const std::string& name, bool value);
        void setUniformInt(const std::string& name, int32_t value);
        void setUniformFloat(const std::string& name, float value);
        void setUniformVec2(const std::string& name, glm::vec2 value);
        void setUniformVec4(const std::string& name, glm::vec4 value);
        void setUniformMat4(const std::string& name, glm::mat4 value);
        void draw(double x, double y, double width, double height, uint32_t textureID);
};