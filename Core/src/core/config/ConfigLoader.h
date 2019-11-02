#pragma once

#include <memory>

class ConfigLoader {
public:
    static void LoadConfig(class Config& configptr);
    static void SaveConfig(class Config& configptr);
};