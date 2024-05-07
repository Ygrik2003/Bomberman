#include "shaderProgram.hpp"
#include <glm/gtc/type_ptr.hpp>

void ShaderProgram::settingUp(uint32_t vShader, uint32_t fShader){
    vertexShader = vShader;
    fragmentShader = fShader;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vShader);
    glAttachShader(shaderProgram, fShader);
    glLinkProgram(shaderProgram);

    int32_t success;
    char infoLog[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "PROGRAM LINKING FAIL " << std::endl << infoLog << std::endl;
    }

    this->use();
}

ShaderProgram::ShaderProgram(uint32_t vShader, uint32_t fShader){
    this->settingUp(vShader, fShader);
}

ShaderProgram::ShaderProgram(const char* vertexPath, const char* fragmentPath){
    uint32_t vShader = compileShader(vertexPath, GL_VERTEX_SHADER);
    uint32_t fShader = compileShader(fragmentPath, GL_FRAGMENT_SHADER);
    this->settingUp(vShader, fShader);
}

ShaderProgram::ShaderProgram(uint32_t vShader, const char* fragmentPath){
    uint32_t fShader = compileShader(fragmentPath, GL_FRAGMENT_SHADER);
    this->settingUp(vShader, fShader);
}
ShaderProgram::ShaderProgram(const char* vertexPath, uint32_t fShader){
    uint32_t vShader = compileShader(vertexPath, GL_VERTEX_SHADER);
    this->settingUp(vShader, fShader);
}

ShaderProgram::~ShaderProgram(){
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

std::string ShaderProgram::getSourceCode(std::string& filePath){
    std::ifstream fileStream(filePath.c_str());
    if (!fileStream.is_open()) {
        std::cerr << "Could not open file: " << filePath << std::endl;
        return "";
    }

    std::string shaderCode = "";
    std::string line;
    while(std::getline(fileStream, line)){
        shaderCode.append(line + "\n");
    }
    fileStream.close();

    return shaderCode;
}

uint32_t ShaderProgram::compileShader(std::string path, uint32_t shaderType){
    int32_t success;
    char infoLog[512];
    std::string str = getSourceCode(path);
    const char* shaderSource = str.c_str();
    uint32_t shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &shaderSource, NULL);
    glCompileShader(shader);

    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if(!success){
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cout << "ERROR SHADER "<< shaderType <<" COMPILATION_FAILED" << std::endl << infoLog << std::endl;
    }

    return shader;
}

uint32_t ShaderProgram::getShaderProgram(){
    return shaderProgram;
}

void ShaderProgram::use(){
    glUseProgram(shaderProgram);
}

void ShaderProgram::setBool(const std::string& name, bool value){
    glUniform1i(glGetUniformLocation(shaderProgram, name.c_str()), (int)value);
}

void ShaderProgram::setInt(const std::string& name, int32_t value){
    glUniform1i(glGetUniformLocation(shaderProgram, name.c_str()), value);
}

void ShaderProgram::setFloat(const std::string& name, float value){
    glUniform1f(glGetUniformLocation(shaderProgram, name.c_str()), value);
}

void ShaderProgram::setVec4(const std::string& name, glm::vec4 value){
    glUniform4fv(glGetUniformLocation(shaderProgram, name.c_str()), GL_FALSE, value_ptr(value));
}

void ShaderProgram::setMat4(const std::string& name, glm::mat4 value){
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, name.c_str()), 1, GL_FALSE, value_ptr(value));
}