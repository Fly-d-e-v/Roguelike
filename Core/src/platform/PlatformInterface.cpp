#include "PlatformInterface.h"

#include "PlatformFriendInterface.h"

#include "thirdparty/steam/steam_api.h"

#include <iostream>

bool PlatformInterface::Init()
{
    if (SteamAPI_Init())
    {
        printf("\nSteam Init() succesfull\n");
    }
    else
    {
        printf("\nSteam Init() failed\n");

        return false;
    }

    PlatformFriendInterface::Init();

    SteamAPI_RestartAppIfNecessary(SteamUtils()->GetAppID());

    printf("Steam Name: %s\n", GetUserName());

    auto friends = PlatformFriendInterface::GetOnlineFriendList();

    for (auto friendData : friends)
    {
        printf("%s\n",friendData.UserName.c_str());
    }

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