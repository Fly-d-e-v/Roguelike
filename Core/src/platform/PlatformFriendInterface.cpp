#include "PlatformFriendInterface.h"

std::vector<FriendData> PlatformFriendInterface::m_Friends = std::vector<FriendData>();

void PlatformFriendInterface::Init()
{
    BuildFriendList();
}

const std::vector<FriendData>& PlatformFriendInterface::GetFullFriendList()
{
    return m_Friends;
}

std::vector<FriendData> PlatformFriendInterface::GetOnlineFriendList()
{
    std::vector<FriendData> onlineFriends = std::vector<FriendData>();
    
    for (FriendData friendData : m_Friends)
    {
        switch (friendData.State)
        {
        case EPersonaState::k_EPersonaStateOnline:
        case EPersonaState::k_EPersonaStateAway:
        case EPersonaState::k_EPersonaStateBusy:
            onlineFriends.push_back(friendData);
        default:
            continue;
        }
    }

    return onlineFriends;
}

void PlatformFriendInterface::BuildFriendList()
{
    int numFriends = SteamFriends()->GetFriendCount(EFriendFlags::k_EFriendFlagAll);
    for (int i = 0; i < numFriends; i++)
    {
        auto steamID = SteamFriends()->GetFriendByIndex(i, EFriendFlags::k_EFriendFlagAll);

        FriendData friendData = FriendData();
        friendData.ID = steamID;

        m_Friends.push_back(friendData);
    }

    RefreshFriends();
}

void PlatformFriendInterface::RefreshFriends()
{
    for (FriendData& friendData : m_Friends)
    {
        friendData.UserName = SteamFriends()->GetFriendPersonaName(friendData.ID);
        friendData.State = SteamFriends()->GetFriendPersonaState(friendData.ID);
    }
}
