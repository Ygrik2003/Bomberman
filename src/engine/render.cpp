#include "render.hpp"
#include "stb_image.h"
#include <iostream>
#include <string>

#define DEBUG(c) std::cout << c << std::endl;


void Render::loadTexture(int32_t number, const char* path){
    uint32_t texture;

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int32_t width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    uint8_t* data = stbi_load(path, &width, &height, &nrChannels, 0);
    // uint8_t* data = stbi_load(path, &width, &height, &nrChannels, STBI_rgb);

    if(data){
        GLenum format;
        if (nrChannels == 1){      
            format = GL_RED;
        }else if (nrChannels == 3){ 
            format = GL_RGB;
        }else if (nrChannels == 4){ 
            format = GL_RGBA;
        }
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    
        stbi_image_free(data);
    } else {
        std::cerr << "Failed to load texture" << std::endl;
    }
    texturesID.insert({number, texture});
}


void Render::initialLoading(std::map<int32_t, std::string> pathes){
    for(std::map<int32_t, std::string>::iterator it = pathes.begin(); it != pathes.end(); it++){
        loadTexture(it->first, it->second.c_str());
    }
}


void Render::deleteBuffers(){
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

Render::~Render(){
    deleteBuffers();
    delete shaderProgram;
}

void Render::initShader(std::string vertShaderPath, std::string fragShaderPath){
    shaderProgram = new ShaderProgram(vertShaderPath.c_str(), fragShaderPath.c_str());
}

Render::Render(std::map<int32_t, std::string> pathes){
    if(!isLoaded){
        initialLoading(pathes);
        isLoaded = true;
    }

    glGenVertexArrays(1, &VAO);  
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
}

void Render::draw(double x, double y, double width, double height, uint32_t textureID){
    int32_t indices[] = {
        0, 1, 3,
        1, 2, 3
    };
    
    float vertices[] = {
        // координаты                                           // цвета            // текстурные координаты
        ((float)x + (float)width), ((float)y + (float)height),  0.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // верхняя правая
        ((float)x + (float)width), ((float)y),                  0.0f, 0.0f, 0.0f,   1.0f, 0.0f,   // нижняя правая
        ((float)x),                ((float)y),                  0.0f, 0.0f, 0.0f,   0.0f, 0.0f,   // нижняя левая
        ((float)x),                ((float)y + (float)height),  0.0f, 0.0f, 0.0f,   0.0f, 1.0f    // верхняя левая 
    };        

    glBindVertexArray(VAO);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Координатные атрибуты
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    // Цветовые атрибуты
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
    // Атрибуты текстурных координат
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(5 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glActiveTexture(GL_TEXTURE0);
    // std::cout << textureID << std::endl;
    glBindTexture(GL_TEXTURE_2D,texturesID.at(textureID));

    shaderProgram->use();
    shaderProgram->setInt("ourTexture0", 0);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void Render::setUniformBool(const std::string& name, bool value){
    shaderProgram->setBool(name, value);
}

void Render::setUniformInt(const std::string& name, int32_t value){
    shaderProgram->setInt(name, value);
}

void Render::setUniformFloat(const std::string& name, float value){
    shaderProgram->setFloat(name, value);
}

void Render::setUniformVec2(const std::string& name, glm::vec2 value){
    shaderProgram->setVec2(name, value);
}

void Render::setUniformVec4(const std::string& name, glm::vec4 value){
    shaderProgram->setVec4(name, value);
}

void Render::setUniformMat4(const std::string& name, glm::mat4 value){
    shaderProgram->setMat4(name, value);
}
