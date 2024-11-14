/*
Add the following at the end of your worldserver.conf

GamblerNPC.Announce = 1
Gambler.Amount1 = 1
Gambler.Amount2 = 2
Gambler.Amount3 = 3
Gambler.Amount4 = 4
Gambler.Amount5 = 5
Gambler.Jackpot = 50
*/

#include "Define.h"
#include "ScriptMgr.h"
#include "SharedDefines.h"
#include "Unit.h"
#include "Pet.h"
#include "Player.h"
#include "ScriptPCH.h"
#include "ScriptedGossip.h"
#include "GossipDef.h"
#include "World.h"
#include "WorldPacket.h"
#include "WorldSession.h"
#include "Chat.h"
#include "Config.h"

class GamblerAnnounce : public PlayerScript
{

public:

    GamblerAnnounce() : PlayerScript("GamblerAnnounce") {}

    void OnLogin(Player* player, bool firstLogin)
    {
		if (firstLogin) {
			// Announce Module
			if (sConfigMgr->GetBoolDefault("GamblerNPC.Announce", true))
			{
				ChatHandler(player->GetSession()).SendSysMessage("This server is running |cff4CFF00GamblerNPC");
			}
		}
    }
};

class gamble_npc : public CreatureScript
{

public:

    gamble_npc() : CreatureScript("gamble_npc") { }

    struct gambler_passivesAI : public ScriptedAI
    {
        gambler_passivesAI(Creature* creature) : ScriptedAI(creature) { }

        // Get bet amounts from config
        const uint32 Bet1 = sConfigMgr->GetIntDefault("Gambler.Amount1", 1);
        const uint32 Bet2 = sConfigMgr->GetIntDefault("Gambler.Amount2", 2);
        const uint32 Bet3 = sConfigMgr->GetIntDefault("Gambler.Amount3", 3);
        const uint32 Bet4 = sConfigMgr->GetIntDefault("Gambler.Amount4", 4);
        const uint32 Bet5 = sConfigMgr->GetIntDefault("Gambler.Amount5", 5);
        const uint32 Jackpot = sConfigMgr->GetIntDefault("Gambler.Jackpot", 50);

        // How much $$$ the player has
        uint32 Pocket = 0;

        // Bets
        uint32 Bets = 0;		// # of bets placed
        uint32 Wins = 0;		// # of wins
        uint32 Losses = 0;		// # of losses

        // Gossip Hello
        bool OnGossipHello(Player* player) override
        {
            // Ensure the player is valid
            if (!player) {
                return false;  // Return false if no player object is found
            }

            ClearGossipMenuFor(player);

            Bets = 0;						// Reset # of bets placed
            Pocket = player->GetMoney();	// How much gold does the player have?
            //Pocket = Pocket * 10000;		// How much gold does the player have?

            // For the high-rollers
            if (Pocket >= 50000000) // 5000 Gold
            {
                std::ostringstream messageTaunt;
                messageTaunt << "Whadda we have here? A high-roller eh? Step right up " << player->GetName() << "!";
                player->GetSession()->SendNotification("%s", messageTaunt.str().c_str());
            }

            std::ostringstream messageJackpot;
            messageJackpot << "Place your bet. Today's Jackpot is " << Jackpot << " gold.";
            AddGossipItemFor(player, GossipOptionNpc::None, messageJackpot.str(), GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
            AddGossipItemFor(player, GossipOptionNpc::None, "So, how does this game work?", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
            SendGossipMenuFor(player, 601020, me->GetGUID());
            return true;
        }

        void GoldOptionsMenu(Player* player) // mindsear
        {
            std::ostringstream Option1;
            std::ostringstream Option2;
            std::ostringstream Option3;
            std::ostringstream Option4;
            std::ostringstream Option5;
            std::ostringstream messageInstruct;

            Option1 << Bet1 << " Gold";
            Option2 << Bet2 << " Gold";
            Option3 << Bet3 << " Gold";
            Option4 << Bet4 << " Gold";
            Option5 << Bet5 << " Gold";
            AddGossipItemFor(player, GossipOptionNpc::None, Option1.str(), GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);
            AddGossipItemFor(player, GossipOptionNpc::None, Option2.str(), GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 4);
            AddGossipItemFor(player, GossipOptionNpc::None, Option3.str(), GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 5);
            AddGossipItemFor(player, GossipOptionNpc::None, Option4.str(), GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 6);
            AddGossipItemFor(player, GossipOptionNpc::None, Option5.str(), GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 7);
            SendGossipMenuFor(player, 1, me->GetGUID());
        }

        // Gossip Select
        bool OnGossipSelect(Player* player, uint32 /*menuId*/, uint32 gossipListId) override
        {
            uint32 const sender = player->PlayerTalkClass->GetGossipOptionSender(gossipListId);
            uint32 const uiAction = player->PlayerTalkClass->GetGossipOptionAction(gossipListId);

            ClearGossipMenuFor(player);

            std::ostringstream Option1;
            std::ostringstream Option2;
            std::ostringstream Option3;
            std::ostringstream Option4;
            std::ostringstream Option5;
            std::ostringstream messageInstruct;

            if (sender != GOSSIP_SENDER_MAIN)
                return false;

            switch (uiAction)
            {

            case GOSSIP_ACTION_INFO_DEF + 1:
                GoldOptionsMenu(player);
                break;

            case GOSSIP_ACTION_INFO_DEF + 2:
                messageInstruct << "The rules are simple " << player->GetName() << ".. If you roll higher than 50, you win double the bet amount. Otherwise, you lose twice the bet amount. A roll of 100 wins the jackpot. Good Luck!";
                AddGossipItemFor(player, GossipOptionNpc::None, messageInstruct.str(), GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 8);
                AddGossipItemFor(player, GossipOptionNpc::None, "Alright Skinny, I'm up for some gambling.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
                SendGossipMenuFor(player, 610000, me->GetGUID());
                break;

            case GOSSIP_ACTION_INFO_DEF + 3:
                OnGossipSelectGold(player, 1, Bet1);
                GoldOptionsMenu(player);
                break;

            case GOSSIP_ACTION_INFO_DEF + 4:
                OnGossipSelectGold(player, 1, Bet2);
                GoldOptionsMenu(player);
                break;

            case GOSSIP_ACTION_INFO_DEF + 5:
                OnGossipSelectGold(player, 1, Bet3);
                GoldOptionsMenu(player);
                break;

            case GOSSIP_ACTION_INFO_DEF + 6:
                OnGossipSelectGold(player, 1, Bet4);
                GoldOptionsMenu(player);
                break;

            case GOSSIP_ACTION_INFO_DEF + 7:
                OnGossipSelectGold(player, 1, Bet5);
                GoldOptionsMenu(player);
                break;

            case GOSSIP_ACTION_INFO_DEF + 8:
                ClearGossipMenuFor(player);
                OnGossipHello(player);
                break;
            }

            return true;
        }

        // Gossip Select Gold
        bool OnGossipSelectGold(Player* player, uint32 /*gossipListId*/, uint32 gold)
        {
            ClearGossipMenuFor(player);

            uint32 Roll = 0;		// Dice roll
            uint32 Amount = 0;		// Bet amount

            // Generate a "random" number	
            Roll = urand(1, 100);
            Bets = Bets + 1;

            // The house always wins (discourage spamming for the jackpot)
            if (Bets >= 10 && Roll == 100)
            {
                // If they have bet 10 times this session, decrement their roll 
                // by 1 to prevent a roll of 100 and hitting the jackpot. 
                Roll = Roll - 1;
            }

            // Setup the bet amount
            Amount = gold * 10000;			// Convert copper to gold
            Amount = Amount * 2;			// Double the bet amount

            // Losing Streak? Aww.. how about some help.
            // After 5 losses in a row, add +25 to their next roll.
            if (Losses >= 5 && Roll < 50)
            {
                std::ostringstream messageHelp;
                messageHelp << "Lady luck isn't on your side tonight " << player->GetName() << ".";
                me->Whisper(messageHelp.str().c_str(), LANG_UNIVERSAL, player);
                Roll = Roll + 25;
                Losses = 0;
            }

            // For the cheapskates
            Pocket = player->GetMoney();	// How much gold does the player currently have?
            if (Pocket < (Amount / 2))
            {
                std::ostringstream messageTaunt;
                messageTaunt << "Hey, I got no time for cheapskates " << player->GetName() << ". Come back when you have " << ((Amount / 10000) / 2) << " gold!";
                player->AddAura(228, player);	// Polymorph Chicken
                player->AddAura(5782, player);	// Fear
                me->Whisper(messageTaunt.str().c_str(), LANG_UNIVERSAL, player);
                CloseGossipMenuFor(player);
                player->PlayDirectSound(5960); // Goblin Pissed
                me->HandleEmoteCommand(EMOTE_ONESHOT_RUDE);
                return false;
            }

            // Hittin' the jackpot!
            if (Roll == 100)
            {
                std::ostringstream messageAction;
                std::ostringstream messageNotice;
                player->ModifyMoney(Jackpot * 10000);
                player->PlayDirectSound(3337);
                player->CastSpell(player, 47292);
                player->CastSpell(player, 44940);
                messageAction << "The bones come to rest with a total roll of " << Roll << ".";
                messageNotice << "Wowzers " << player->GetName() << "! You hit the jackpot and win " << Jackpot << " gold!";
                me->Whisper(messageAction.str().c_str(), LANG_UNIVERSAL, player);
                player->GetSession()->SendNotification("%s", messageNotice.str().c_str());
                CloseGossipMenuFor(player);
                me->HandleEmoteCommand(EMOTE_ONESHOT_APPLAUD);
                return true;
            }

            // Why does it happen? Because it happens.. Roll the bones.. Roll the bones!
            if (Roll >= 50)
            {
                std::ostringstream messageAction;
                std::ostringstream messageNotice;
                player->ModifyMoney(Amount);
                Wins = Wins + 1;
                Losses = 0;
                player->PlayDirectSound(3337);
                player->CastSpell(player, 47292);
                player->CastSpell(player, 44940);
                messageAction << "The bones come to rest with a total roll of " << Roll << ".";
                messageNotice << "Congratulations " << player->GetName() << ", You've won " << Amount / 10000 << " gold!";
                me->Whisper(messageAction.str().c_str(), LANG_UNIVERSAL, player);
                ChatHandler(player->GetSession()).SendSysMessage("s%", messageNotice.str().c_str());
                me->HandleEmoteCommand(EMOTE_ONESHOT_APPLAUD);
            }
            else
            {
                std::ostringstream messageAction;
                std::ostringstream messageNotice;
                player->ModifyMoney(~Amount + 1u);
                Losses = Losses + 1;
                messageAction << "The bones come to rest with a total roll of " << Roll << ".";
                messageNotice << "Tough luck " << player->GetName() << ", you've lost " << Amount / 10000 << " gold!";
                me->Whisper(messageAction.str().c_str(), LANG_UNIVERSAL, player);
                ChatHandler(player->GetSession()).SendSysMessage("s%", messageNotice.str().c_str());
                me->HandleEmoteCommand(EMOTE_ONESHOT_QUESTION);
            }

            OnGossipSelect(player, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
            return true;
        }

        // Passive Emotes


        uint32 uiAdATimer;
        uint32 uiAdBTimer;
        uint32 uiAdCTimer;

        void Reset() override
        {
            uiAdATimer = 1000;
            uiAdBTimer = 23000;
            uiAdCTimer = 11000;
        }

        // Speak
        void UpdateAI(uint32 diff) override
        {

            if (uiAdATimer <= diff)
            {
                me->Say("Come one, come all! Step right up to Skinny's! Place your bets, Place your bets!", LANG_UNIVERSAL, NULL);
                me->HandleEmoteCommand(EMOTE_ONESHOT_EXCLAMATION);
                me->CastSpell(me, 44940);
                uiAdATimer = 61000;
            }
            else
                uiAdATimer -= diff;

            if (uiAdBTimer <= diff)
            {
                me->Say("Come on! Place your bets, Don't be a chicken!", LANG_UNIVERSAL, NULL);
                me->HandleEmoteCommand(EMOTE_ONESHOT_CHICKEN);
                uiAdBTimer = 61000;
            }
            else
                uiAdBTimer -= diff;

            if (uiAdCTimer <= diff)
            {
                me->Say("Don't make me sad, Come and gamble! Step right up and win today!", LANG_UNIVERSAL, NULL);
                me->HandleEmoteCommand(EMOTE_ONESHOT_CRY);
                uiAdCTimer = 61000;
            }
            else
            {
                uiAdCTimer -= diff;
            }
        }
    };

    // Creature AI
    CreatureAI* GetAI(Creature* creature) const
    {
        return new gambler_passivesAI(creature);
    }
};

void AddNPCGamblerScripts()
{
    new GamblerAnnounce();
    new gamble_npc();
}
