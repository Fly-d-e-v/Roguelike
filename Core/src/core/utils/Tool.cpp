#include "Tool.h"

#include "imgui-1.73/imgui.h"

Tool::Tool() {
    _ToolName = "unnamed tool";
    _ShowTool = false;
}

void Tool::MenuItemMethod() {
    if (ImGui::MenuItem(_ToolName.c_str())) {
        _ShowTool = !_ShowTool;
    }
}
