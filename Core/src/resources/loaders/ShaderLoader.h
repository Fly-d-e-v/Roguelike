#pragma once

#include <memory>
#include <string>

class ShaderLoader {

public:

    static std::shared_ptr<class Shader> LoadShader(const std::string& vertexPath , const std::string& fragmentPath);
    static void LoadShader(std::shared_ptr<class Shader> shaderHandle);
};