#pragma once

#include "core/utils/Tool.h"
#include "resources/Resource.h" 

#include <map>
#include <memory>

class ResourceManager : public Tool {

public:

    ResourceManager();

    static std::shared_ptr<ResourceManager> Instance();
    void AddResource(std::shared_ptr<Resource> resource);

    template<typename ResourceType>
    std::shared_ptr<ResourceType>FetchResource(std::string path) {
        return std::dynamic_pointer_cast<ResourceType>(FetchResource(path));
    }

    void LoadResourceList();
    void SaveResourceList();
    
    virtual void ToolMethod();

private:

    std::shared_ptr<Resource> FetchResource(std::string path);

    void LoadResource(std::shared_ptr<Resource> resource);

    void NewResourceDialogue();
    void ShowNewResourceDialogue();
    void CreateResourceOfType(const char* type);

private:

    std::map<long long, std::shared_ptr<Resource>> _Resources;

    static std::shared_ptr<ResourceManager> s_Instance;

    std::shared_ptr<Resource> _TempResource;

    bool _ShowNewResourceDialogue;
    const char* _currentResourceType;

};