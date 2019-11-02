#include "ShaderLoader.h"

#include "resources/resources/Shader.h"

#include <glad/glad.h>
#include <fstream>
#include <sstream>

std::shared_ptr<class Shader> ShaderLoader::LoadShader(const std::string& vertexPath, const std::string& fragmentPath) {
    
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vsFile;
    std::ifstream fsFile;

    vsFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fsFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try {
        vsFile.open(vertexPath);
        fsFile.open(fragmentPath);

        std::stringstream vsStream, fsStream;
        vsStream << vsFile.rdbuf();
        fsStream << fsFile.rdbuf();

        vsFile.close();
        fsFile.close();

        vertexCode = vsStream.str();
        fragmentCode = fsStream.str();

    }
    catch (std::ifstream::failure e) {
        printf("Error: Shader file couldn't be loaded: %s", e.what());
        return nullptr;
    }

    const char* vertexShaderSource = vertexCode.c_str();
    const char* fragmentShaderSource = fragmentCode.c_str();

    //Compile shaders
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    int success;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        fprintf(stderr, "Error: Vertex shader compilation failed %s\n", infoLog);

    }

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);

    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        fprintf(stderr, "Error: Fragment Shader compilation failed %s\n", infoLog);
    }

    std::shared_ptr<Shader> shaderHandle = std::make_shared<Shader>();

    //Linking Shaders
    shaderHandle->ID = glCreateProgram();
    glAttachShader(shaderHandle->ID, vertexShader);
    glAttachShader(shaderHandle->ID, fragmentShader);
    glLinkProgram(shaderHandle->ID);

    glGetProgramiv(shaderHandle->ID, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(shaderHandle->ID, 512, NULL, infoLog);
        fprintf(stderr, "Error: Shader program linking failed %s\n", infoLog);
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderHandle;
}
