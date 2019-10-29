#pragma once

#include "thirdparty/steam/isteamfriends.h"

#include <string>
#include <vector>

struct FriendData
{
    CSteamID ID;
    std::string UserName;
    EPersonaState State;
};

class PlatformFriendInterface
{

public:

    static void Init();

    static const std::vector<FriendData>& GetFullFriendList();
    static std::vector<FriendData> GetOnlineFriendList();

private:

    static void BuildFriendList();
    static void RefreshFriends();

    static std::vector<FriendData> m_Friends;


};

