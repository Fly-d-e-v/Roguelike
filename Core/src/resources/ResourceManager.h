#pragma once

#include "resources/Resource.h" 

#include <map>
#include <memory>

class ResourceManager {

public:

    static std::shared_ptr<ResourceManager> Instance();
    void AddResource(std::shared_ptr<Resource> resource);

    template<typename ResourceType>
    std::shared_ptr<ResourceType>FetchResource(std::string path) {
        return std::dynamic_pointer_cast<ResourceType>(FetchResource(path));
    }

    void LoadResourceList();
    void SaveResourceList();

private:

    std::shared_ptr<Resource> FetchResource(std::string path);

    void LoadResource(std::shared_ptr<Resource> resource);

private:

    std::map<long long, std::shared_ptr<Resource>> _Resources;

    static std::shared_ptr<ResourceManager> s_Instance;

};