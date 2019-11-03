#pragma once

#include <string>

class Tool {

public:

    Tool();

    virtual void MenuItemMethod();
    virtual void ToolMethod() = 0;

protected:

    std::string _ToolName;
    bool _ShowTool;

};