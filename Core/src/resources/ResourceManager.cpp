#include "ResourceManager.h"

#include "core/utils/Utilities.h"

#include "resources/loaders/ShaderLoader.h"
#include "resources/resources/Shader.h"

#include "resources/loaders/TextureLoader.h"
#include "resources/resources/Texture.h"

#include <cereal/archives/xml.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/types/map.hpp>
#include <fstream>
#include <istream>
#include <ostream>

std::shared_ptr<ResourceManager> ResourceManager::s_Instance = nullptr;

std::shared_ptr<ResourceManager> ResourceManager::Instance() {
    if (s_Instance == nullptr) {
        s_Instance = std::make_shared<ResourceManager>();
    }
    return s_Instance;
}

void ResourceManager::AddResource(std::shared_ptr<Resource> resource) {
    long long GUID = Utilities::ComputeStringHash(resource->Path);
    if (_Resources.find(GUID) == _Resources.end()) {
        resource->GUID = GUID;
        _Resources.insert(std::make_pair(GUID, resource));
    }
    else {
        printf("[RESOURCE MANAGER] - [%s] already exists in resource library", resource->Path.c_str());
    }
}

std::shared_ptr<Resource> ResourceManager::FetchResource(std::string path) {

    auto resourcePair = _Resources.find(Utilities::ComputeStringHash(path));
    if (resourcePair != _Resources.end()) {
        LoadResource(resourcePair->second);
    }
    return resourcePair->second;
}

void ResourceManager::LoadResourceList() {
    try {
        std::ifstream is = std::ifstream("Resources/Resources.xml");
        {
            cereal::XMLInputArchive iarchive(is);
            iarchive(_Resources);
        }
    }
    catch (std::exception e) {
        printf("Failed to LoadConfig File: %s\n", e.what());
    }
}

void ResourceManager::SaveResourceList() {
    try {
        std::ofstream os = std::ofstream("Resources/Resources.xml");
        {
            cereal::XMLOutputArchive oarchive(os);
            oarchive(_Resources);
        }
    }
    catch (std::exception e) {
        printf("Failed to Save Config File: %s\n", e.what());
    }
}

void ResourceManager::LoadResource(std::shared_ptr<Resource> resource) {
    if (!resource->IsLoaded) {
        switch (resource->GetType()) {
        case EResourceType::Shader:
            ShaderLoader::LoadShader(std::static_pointer_cast<Shader>(resource));
            break;
        case EResourceType::Texture:
            TextureLoader::LoadTexture(std::static_pointer_cast<Texture>(resource));
            break;
        case EResourceType::Audio:
            break;
        }
    }
}
