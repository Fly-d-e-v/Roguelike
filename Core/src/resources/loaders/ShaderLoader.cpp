#include "ShaderLoader.h"

#include "resources/resources/Shader.h"

#include "core/logger/Logger.h"

#include <glad/glad.h>
#include <fstream>
#include <sstream>

std::shared_ptr<class Shader> ShaderLoader::LoadShader(const std::string& vertexPath, const std::string& fragmentPath) {
    std::shared_ptr<Shader> shaderHandle = std::make_shared<Shader>();
    shaderHandle->VertexPath = vertexPath;
    shaderHandle->FragmentPath = fragmentPath;
    LoadShader(shaderHandle);
    return shaderHandle;
}

void ShaderLoader::LoadShader(std::shared_ptr<class Shader> shaderHandle) {
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vsFile;
    std::ifstream fsFile;

    vsFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fsFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try {
        vsFile.open(shaderHandle->VertexPath);
        fsFile.open(shaderHandle->FragmentPath);

        std::stringstream vsStream, fsStream;
        vsStream << vsFile.rdbuf();
        fsStream << fsFile.rdbuf();

        vsFile.close();
        fsFile.close();

        vertexCode = vsStream.str();
        fragmentCode = fsStream.str();

    }
    catch (std::ifstream::failure e) {
        Logger::Log(LogVerbosity::Error, "Shader file  could not be bloaded %s", e.what());
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
        Logger::Log(LogVerbosity::Error, "Vertex Shader compilation failed:  %s", infoLog);
    }

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);

    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        Logger::Log(LogVerbosity::Error, "Fragment Shader compilation failed:  %s", infoLog);
    }

    //Linking Shaders
    shaderHandle->ID = glCreateProgram();
    glAttachShader(shaderHandle->ID, vertexShader);
    glAttachShader(shaderHandle->ID, fragmentShader);
    glLinkProgram(shaderHandle->ID);

    glGetProgramiv(shaderHandle->ID, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(shaderHandle->ID, 512, NULL, infoLog);
        Logger::Log(LogVerbosity::Error, "Shader Linking failed:  %s", infoLog);
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    shaderHandle->IsLoaded = true;
}

