#include "Chat.h"

class PlayerlocalflagAnnounce : public PlayerScript
{

public:

    PlayerlocalflagAnnounce() : PlayerScript("PlayerlocalflagAnnounce") {}

    void OnLogin(Player* player, bool /*firstLogin*/)
    {
        if (player->HasPlayerLocalFlag(PLAYER_LOCAL_FLAG_ACCOUNT_SECURED))
        {
            player->RemovePlayerLocalFlag(PLAYER_LOCAL_FLAG_ACCOUNT_SECURED);
            ChatHandler(player->GetSession()).SendSysMessage("Player Local Flag Account Secured REMOVED! - Sorry about it! Better not playing around with the core code. :)");
        }
    }
};

void AddSC_PlayerlocalflagScripts()
{
    new PlayerlocalflagAnnounce();
}
