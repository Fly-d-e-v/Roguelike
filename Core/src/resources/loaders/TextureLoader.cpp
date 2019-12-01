#include "TextureLoader.h"

#include "resources/resources/Texture.h"
#include "core/utils/Utilities.h"

#include "core/logger/Logger.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"

std::shared_ptr<class Texture> TextureLoader::LoadTexture(const std::string& path) {

    stbi_set_flip_vertically_on_load(true);
    {
        int width, height, nrChannels;
        unsigned char* textureData = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
        std::shared_ptr<Texture> textureHandle = std::make_shared<Texture>();
        textureHandle->GUID = Utilities::ComputeStringHash(path);
        if (textureData) {
            glGenTextures(1, &textureHandle->_textureID);
            glBindTexture(GL_TEXTURE_2D, textureHandle->_textureID);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData);
        }
        else {
            Logger::Log(LogVerbosity::Error, "Failed To Load Texture: %s", path.c_str());
            return nullptr;
        }
        stbi_image_free(textureData);
        textureHandle->IsLoaded = true;
        return textureHandle;
    }
}

void TextureLoader::LoadTexture(std::shared_ptr<Texture> textureHandle) {
    stbi_set_flip_vertically_on_load(true);
    {
        int width, height, nrChannels;
        unsigned char* textureData = stbi_load(textureHandle->Path.c_str(), &width, &height, &nrChannels, 0);
        if (textureData) {
            glGenTextures(1, &textureHandle->_textureID);
            glBindTexture(GL_TEXTURE_2D, textureHandle->_textureID);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData);
        }
        else {
            Logger::Log(LogVerbosity::Error, "Failed To Load Texture: %s", textureHandle->Path.c_str());
        }
        stbi_image_free(textureData);
        textureHandle->IsLoaded = true;
    }
}