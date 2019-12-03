#include "ResourceManager.h"

#include "core/utils/Utilities.h"

#include "resources/loaders/ShaderLoader.h"
#include "resources/resources/Shader.h"

#include "resources/loaders/TextureLoader.h"
#include "resources/resources/Texture.h"

#include  "core/logger/Logger.h"

#include "imgui/imgui.h"

#include <cereal/archives/xml.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/types/map.hpp>
#include <fstream>
#include <istream>
#include <ostream>

std::shared_ptr<ResourceManager> ResourceManager::s_Instance = nullptr;

ResourceManager::ResourceManager() {
    _ToolName = "Resource Manager";
    _ShowTool = true;
    _TempResource = nullptr;
    _ShowNewResourceDialogue = false;
}

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
        Logger::Log(LogVerbosity::Warning, "[%s] already exists in resource library", resource->Path.c_str());
    }
}

std::shared_ptr<Resource> ResourceManager::FetchResource(std::string path) {

    std::map<long long, std::shared_ptr<Resource>>::iterator resourcePair = _Resources.find(Utilities::ComputeStringHash(path));
    if (resourcePair != _Resources.end()) {
        LoadResource(resourcePair->second);
    }
    else {
        return  nullptr;
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
        Logger::Log(LogVerbosity::Error, "Failed to load Resource Dictionary: %s", e.what());
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
        Logger::Log(LogVerbosity::Error, "Failed to save Resource Dictionary:  %s", e.what());
    }
}

void ResourceManager::ToolMethod() {
    if (_ShowTool) {
        if (ImGui::Begin("Resource Manager", &_ShowTool, ImGuiWindowFlags_MenuBar)) {

            ImGui::BeginMenuBar();

            if (ImGui::BeginMenu("File")) {

                if (ImGui::MenuItem("New")) {
                    ShowNewResourceDialogue();
                }
                ImGui::Separator();
                if (ImGui::MenuItem("Save")) {
                    SaveResourceList();
                }

                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();

            ImGui::Text("Resources");
            ImGui::Separator();
            for (auto resource : _Resources) {
                if (ImGui::CollapsingHeader(Utilities::GetFileName(resource.second->Path).c_str())) {
                    resource.second->ImGuiDisplay();
                }
            }
            ImGui::Separator();

        }
        ImGui::End();
        if (_ShowNewResourceDialogue) {
            NewResourceDialogue();
        }
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

void ResourceManager::NewResourceDialogue() {

    const char* resourceTypes[] = { "None", "Texture", "Shader", "Sound" };

    if (ImGui::Begin("New Resource",&_ShowNewResourceDialogue)) {

        if (ImGui::BeginCombo("Type", _currentResourceType)) {

            for (int n = 0; n < IM_ARRAYSIZE(resourceTypes); n++) {
                bool is_selected = (_currentResourceType == resourceTypes[n]);
                if (ImGui::Selectable(resourceTypes[n], is_selected)) {
                    _currentResourceType = resourceTypes[n];
                    CreateResourceOfType(_currentResourceType);
                }
                if (is_selected)
                    ImGui::SetItemDefaultFocus();
            }
        
            ImGui::EndCombo();
        }

        if (_TempResource) {

            const float footer_height_to_reserve = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing(); // 1 separator, 1 input text
            ImGui::BeginChild("ScrollingRegion", ImVec2(0, -footer_height_to_reserve), true, ImGuiWindowFlags_HorizontalScrollbar); // Leave room for 1 separator + 1 InputText

            _TempResource->ImGuiNewDisplay();

            ImGui::EndChild();

            if (ImGui::Button("Add")) {
                AddResource(_TempResource);
                _currentResourceType = "None";
                _TempResource = nullptr;
                _ShowNewResourceDialogue = false;
            }
        }
    }
    ImGui::End();
}

void ResourceManager::ShowNewResourceDialogue() {
    _ShowNewResourceDialogue = true;
    _TempResource = nullptr;
    _currentResourceType = "None";
}

void ResourceManager::CreateResourceOfType(const char* type) {
    if (strstr(type, "None")) {
        _TempResource = nullptr;
    } else if (strstr(type, "Texture")) {
        _TempResource = std::make_shared<Texture>();
    }
    else if (strstr(type,"Shader")) {
        _TempResource = std::make_shared<Shader>();
    }
    else if (strstr(type, "Sound")) {
        _TempResource = nullptr;
    }
}
