#include "Config.h"

#include "imgui-1.73/imgui.h"

Config::Config() {
    _ToolName = "Config Tool";
}

void Config::ToolMethod() {
    if (_ShowTool) {
        ImGui::Begin(_ToolName.c_str());

        ImGui::Text("Configurations Go Here!");

        ImGui::End();
    }
}
