#pragma once

#include <string>

class Utilities {

public:

    static long long ComputeStringHash(std::string const& s);

    std::string GetRootFolder();

};