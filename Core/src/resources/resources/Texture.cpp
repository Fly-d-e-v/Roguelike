#include "Texture.h"

#include "imgui/imgui.h"

#include <cereal/archives/xml.hpp>
#include <cereal/types/polymorphic.hpp>

Texture::Texture() {
    _Type = EResourceType::Texture;
}

void Texture::SetPath(const std::string& path) {
    Path = path;
}

void Texture::ImGuiDisplay() {
    Resource::ImGuiDisplay();
    if (IsLoaded) {
        ImGui::Image((void*)(intptr_t)_textureID, ImVec2(256, 256));
    }
}

CEREAL_REGISTER_TYPE(Texture);
CEREAL_REGISTER_POLYMORPHIC_RELATION(Resource, Texture);