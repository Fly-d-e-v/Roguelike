#pragma once

#include <string>

class Utilities {

public:

    static long long ComputeStringHash(std::string const& s);

    static std::string GetRootFolder();
    static std::string GetFileExtention(const std::string& path);
};