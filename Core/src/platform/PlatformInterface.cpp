#include "PlatformInterface.h"

#include "PlatformFriendInterface.h"

#include "core/logger/Logger.h"

#include "thirdparty/steam/steam_api.h"

#include <iostream>

bool PlatformInterface::Init()
{
    if (SteamAPI_Init())
    {
        Logger::Log(LogVerbosity::Info, "Steam Init() successful");
    }
    else
    {
        Logger::Log(LogVerbosity::Error, "Steam Init() Failed!");
        return false;
    }

    PlatformFriendInterface::Init();

    SteamAPI_RestartAppIfNecessary(SteamUtils()->GetAppID());
    Logger::Log(LogVerbosity::Info, "Steam Username: %s", GetUserName());

    return true;
}

const char* PlatformInterface::GetUserName()
{
    return SteamFriends()->GetPersonaName();
}

void PlatformInterface::Deinit()
{
    SteamAPI_Shutdown();
}