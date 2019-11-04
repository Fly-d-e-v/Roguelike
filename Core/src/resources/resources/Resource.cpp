#include "Resource.h"

#include "imgui/imgui.h"

Resource::Resource() {
    IsLoaded = false;
}

EResourceType Resource::GetType() const {
    return _Type;
}

void Resource::ImGuiDisplay() {
    ImGui::Text("GUID: %i", GUID);
    ImGui::Text("Path: %s", Path.c_str());
    ImGui::Text("Loaded: %s", IsLoaded ? "true" : "false");
}
