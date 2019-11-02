#include "ConfigLoader.h"

#include "Config.h"

#include <cereal/archives/xml.hpp>
#include <cereal/types/memory.hpp>
#include <fstream>
#include <istream>
#include <ostream>

void ConfigLoader::LoadConfig(Config& configptr) {
    try {
        std::ifstream is = std::ifstream("Config.xml");
        {
            cereal::XMLInputArchive iarchive(is);
            iarchive(configptr);
        }
    }
    catch (std::exception e) {
        printf("Failed to LoadConfig File: %s\n", e.what());
    }
}

void ConfigLoader::SaveConfig(Config& configptr) {
    try {
        std::ofstream os = std::ofstream("Config.xml");
        {
            cereal::XMLOutputArchive oarchive(os);
            oarchive(configptr);
        }
    }
    catch (std::exception e) {
        printf("Failed to Save Config File: %s\n", e.what());
    }
}
