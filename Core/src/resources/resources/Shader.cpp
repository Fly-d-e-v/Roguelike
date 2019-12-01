#include "Shader.h"

#include "imgui/imgui.h"

#include <cereal/types/polymorphic.hpp>
#include <cereal/archives/xml.hpp>

#include <glad/glad.h>

Shader::Shader() {
    _Type = EResourceType::Shader;
}

void Shader::SetPath(const std::string& path) {
    Path = path;
}

void Shader::ImGuiDisplay() {
    Resource::ImGuiDisplay();

    ImGui::Text("Vertex: %s", VertexPath.c_str());
    ImGui::Text("Fragment: %s", FragmentPath.c_str());

}

void Shader::ImGuiNewDisplay() {

    char vertex[256] = "";
    char fragment[256] = "";

    for (uint8_t i = 0; i < VertexPath.length(); i++) {
        vertex[i] = VertexPath[i];
    }

    for (uint8_t i = 0; i < FragmentPath.length(); i++) {
        fragment[i] = FragmentPath[i];
    }

    ImGui::InputText("Vertex Path",vertex,IM_ARRAYSIZE(vertex));
    ImGui::InputText("Fragment Path",fragment, IM_ARRAYSIZE(fragment));

    VertexPath = vertex;
    FragmentPath = fragment;

}

void Shader::Use()
{
	glUseProgram(ID);
}

CEREAL_REGISTER_TYPE(Shader)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Resource, Shader)