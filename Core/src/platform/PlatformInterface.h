#pragma once

#include <string>
#include <vector>

class PlatformInterface
{

public:

    static bool Init();

    static const char* GetUserName();

    static void Deinit();

};

