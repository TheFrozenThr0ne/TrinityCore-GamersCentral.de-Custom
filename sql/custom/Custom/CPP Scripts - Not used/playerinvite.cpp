// Needs to be Implanted before: https://github.com/TrinityCore/TrinityCoreCustomChanges/commit/ebf94710f023e6d1bbf1b998d8014a3c7bdb5e31
// Player.cpp
// ScriptMgr.cpp
// ScriptMgr.h
//
// 
//
// 
// Lucky Number made by Talamortis

#include "Configuration/Config.h"
#include "Chat.h"
#include "WorldSession.h"
#include "ChannelMgr.h"
#include "Channel.h"

class PlayerInviteAnnounce : public PlayerScript
{
public:

    PlayerInviteAnnounce() : PlayerScript("PlayerInviteAnnounce") {}

    void OnLogin(Player* player, bool /*firstLogin*/)
    {
        // Announce Module
        if (sConfigMgr->GetBoolDefault("PlayerInvite.Announce", true))
        {
            ChatHandler(player->GetSession()).SendSysMessage("This server is running |cff4CFF00PlayerCustomChannelInvite");
        }
    }
};

class PlayerInvite : public PlayerScript
{
public:
    PlayerInvite() : PlayerScript("PlayerInvite") { }
    
    void OnLogin(Player* player, bool /*firstLogin*/) override
    {
            if (ChannelMgr* cMgr = ChannelMgr::ForTeam(ALLIANCE))
            {
                cMgr->ForTeam(ALLIANCE);

                if (Channel* channel = cMgr->GetCustomChannel("World"))
                {
                    channel->JoinChannel(player->GetSession()->GetPlayer(), "");
                    channel->Invite(player->GetSession()->GetPlayer(), player->GetSession()->GetPlayer()->GetName());
                    channel->LeaveChannel(player->GetSession()->GetPlayer(), true);
                }
            }
    }
};

void AddPlayerInviteScripts()
{
    new PlayerInviteAnnounce();
    new PlayerInvite();
}
