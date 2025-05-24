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

class SkipStartingAnnounce : public PlayerScript
{

public:

    SkipStartingAnnounce() : PlayerScript("SkipStartingAnnounce") {}

    void OnLogin(Player* player, bool /*firstLogin*/)
    {
            // Announce Module
            if (sConfigMgr->GetBoolDefault("SkipStartingNPC.Announce", true))
            {
                ChatHandler(player->GetSession()).SendSysMessage("This server is running |cff4CFF00Skip Allied+ Starting");
            }
    }
};

class SkipStartingNPC : public CreatureScript
{

public:

    SkipStartingNPC() : CreatureScript("SkipStartingNPC") { }

    // Passive Emotes
    struct SkipStartingNPCAI : public ScriptedAI
    {
        SkipStartingNPCAI(Creature* creature) : ScriptedAI(creature) { }

        bool OnGossipHello(Player* player) override
        {
            uint32 playerAreaId = player->GetAreaId();

            switch (player->GetTeamId())
            {
            case TEAM_HORDE:
                AddGossipItemFor(player, GossipOptionNpc::None, "Teleport to Orgrimmar!", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 24);
                break;
            case TEAM_ALLIANCE:
                AddGossipItemFor(player, GossipOptionNpc::None, "Teleport to Stormwind!", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 25);
                break;
            default:
                break;
            }

            // Skip RACE
            switch (player->GetRace())
            {
            case RACE_HIGHMOUNTAIN_TAUREN:
                if (!player->HasItemCount(141394, 1) || !player->HasItemCount(157757, 1) || !player->HasItemCount(157028, 1))
                {
                    AddGossipItemFor(player, GossipOptionNpc::None, "Give provided stuff so i can Skip!", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF);
                }
                break;
            case RACE_NIGHTBORNE:
                if (!player->HasItemCount(156487, 1) || !player->HasSpell(258845))
                {
                    AddGossipItemFor(player, GossipOptionNpc::None, "Give provided stuff so i can Skip!", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
                }
                break;
            case RACE_EARTHEN_DWARF_HORDE:
                    AddGossipItemFor(player, GossipOptionNpc::None, "Give provided stuff so i can Skip!", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
                break;
            case RACE_EARTHEN_DWARF_ALLIANCE:
                    AddGossipItemFor(player, GossipOptionNpc::None, "Give provided stuff so i can Skip!", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);
                break;
            case RACE_ZANDALARI_TROLL:
                if (!player->HasItemCount(157870, 1) || !player->HasSpell(263707))
                {
                    AddGossipItemFor(player, GossipOptionNpc::None, "Give provided stuff so i can Skip!", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 4);
                }
                break;
            case RACE_VOID_ELF:
                if (!player->HasItemCount(156486, 1) || !player->HasSpell(259202))
                {
                    AddGossipItemFor(player, GossipOptionNpc::None, "Give provided stuff so i can Skip!", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 5);
                }
                break;
            case RACE_LIGHTFORGED_DRAENEI:
                if (!player->HasItemCount(155656, 1) || !player->HasSpell(258022))
                {
                    AddGossipItemFor(player, GossipOptionNpc::None, "Give provided stuff so i can Skip!", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 6);
                }
                break;
            case RACE_DARK_IRON_DWARF:
                if (!player->HasItemCount(161331, 1) || !player->HasSpell(271646))
                {
                    AddGossipItemFor(player, GossipOptionNpc::None, "Give provided stuff so i can Skip!", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 7);
                }
                break;
            case RACE_KUL_TIRAN:
                if (!player->HasItemCount(164762, 1) || !player->HasSpell(282682))
                {
                    AddGossipItemFor(player, GossipOptionNpc::None, "Give provided stuff so i can Skip!", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 8);
                }
                break;
            case RACE_MECHAGNOME:
                if (!player->HasItemCount(174067, 1) || !player->HasSpell(305592))
                {
                    AddGossipItemFor(player, GossipOptionNpc::None, "Give provided stuff so i can Skip!", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 9);
                }
                break;
            case RACE_DRACTHYR_HORDE:
                if (!player->HasItemCount(193914, 1) || !player->HasSpell(351239))
                {
                    AddGossipItemFor(player, GossipOptionNpc::None, "Give provided stuff so i can Skip!", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 10);
                }
                break;
            case RACE_DRACTHYR_ALLIANCE:
                if (!player->HasItemCount(193914, 1) || !player->HasSpell(351239))
                {
                    AddGossipItemFor(player, GossipOptionNpc::None, "Give provided stuff so i can Skip!", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 11);
                }
                break;
            case RACE_WORGEN:
                if (!player->HasItemCount(52040, 1) || !player->HasSpell(68992))
                {
                    AddGossipItemFor(player, GossipOptionNpc::None, "Give provided stuff so i can Skip!", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 12);
                }
                break;
            default:
                break;
            }

            // Skip Death Knight CLASS
            if (playerAreaId == 4342)
            {
                switch (player->GetClass())
                {
                case CLASS_DEATH_KNIGHT:
                    if (!player->HasSpell(53428))
                    {
                        AddGossipItemFor(player, GossipOptionNpc::None, "Give provided spells so i can finally Skip!", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 13);
                    }
                    break;
                default:
                    break;
                }
            }

            // Skip Only for Exiles Reach CLASS
            if (playerAreaId == 10453)
            {
                switch (player->GetClass())
                {
                case CLASS_DRUID:
                    if (!player->HasSpell(783))
                    {
                        AddGossipItemFor(player, GossipOptionNpc::None, "Give provided spells so i can finally Skip!", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 14);
                    }
                    break;
                case CLASS_HUNTER:
                    if (!player->HasSpell(83242))
                    {
                        AddGossipItemFor(player, GossipOptionNpc::None, "Give provided spells so i can finally Skip!", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 15);
                    }
                    break;
                case CLASS_MAGE:
                    if (!player->HasSpell(118))
                    {
                        AddGossipItemFor(player, GossipOptionNpc::None, "Give provided spells so i can finally Skip!", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 16);
                    }
                    break;
                case CLASS_MONK:
                    if (!player->HasSpell(322109))
                    {
                        AddGossipItemFor(player, GossipOptionNpc::None, "Give provided spells so i can finally Skip!", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 17);
                    }
                    break;
                case CLASS_PALADIN:
                    if (!player->HasSpell(642))
                    {
                        AddGossipItemFor(player, GossipOptionNpc::None, "Give provided spells so i can finally Skip!", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 18);
                    }
                    break;
                case CLASS_PRIEST:
                    if (!player->HasSpell(2006))
                    {
                        AddGossipItemFor(player, GossipOptionNpc::None, "Give provided spells so i can finally Skip!", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 19);
                    }
                    break;
                case CLASS_ROGUE:
                    if (!player->HasSpell(315584))
                    {
                        AddGossipItemFor(player, GossipOptionNpc::None, "Give provided spells so i can finally Skip!", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 20);
                    }
                    break;
                case CLASS_SHAMAN:
                    if (!player->HasSpell(2645))
                    {
                        AddGossipItemFor(player, GossipOptionNpc::None, "Give provided spells so i can finally Skip!", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 21);
                    }
                    break;
                case CLASS_WARLOCK:
                    if (!player->HasSpell(697))
                    {
                        AddGossipItemFor(player, GossipOptionNpc::None, "Give provided spells so i can finally Skip!", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 22);
                    }
                    break;
                case CLASS_WARRIOR:
                    if (!player->HasSpell(163201))
                    {
                        AddGossipItemFor(player, GossipOptionNpc::None, "Give provided spells so i can finally Skip!", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 23);
                    }
                    break;
                default:
                    break;
                }
            }

            // Announce Module
            if (sConfigMgr->GetIntDefault("SkipStartingNPC.SetLevel20", 1))
            {
                if (player->GetLevel() < 20)
                {
                    AddGossipItemFor(player, GossipOptionNpc::None, "Set Level to 20! Enjoy or not!", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 26);
                }
            }

            SendGossipMenuFor(player, 68, me->GetGUID());
            return true;
        }

        bool OnGossipSelect(Player* player, uint32 /*menuId*/, uint32 gossipListId) override
        {
            uint32 const action = player->PlayerTalkClass->GetGossipOptionAction(gossipListId);
            ClearGossipMenuFor(player);

            player->ModifyMoney(50000, true);

            switch (action)
            {
            case 1000: // HIGHMOUNTAIN TAUREN
                player->AddItem(141394, 1);
                player->AddItem(157757, 1);
                player->AddItem(157028, 1);
                player->ModifyCurrency(1553, 750);
                break;
            case 1001: // NIGHTBORNE
                player->AddItem(156487, 1);
                break;
            case 1002: // EARTHEN DWARF HORDE
                break;
            case 1003: // EARTHEN DWARF ALLIANCE
                break;
            case 1004: // ZANDALARI TROLL
                player->AddItem(157870, 1);
                break;
            case 1005: // VOID ELF
                player->AddItem(156486, 1);
                break;
            case 1006: // LIGHTFORGED DRAENEI
                player->AddItem(155656, 1);
                break;
            case 1007: // DARK IRON DWARF
                player->ModifyCurrency(1553, 1);
                player->AddItem(155656, 1);
                player->AddItem(161331, 1);
                break;
            case 1008: // KUL TIRAN
                player->AddItem(164762, 1);
                break;
            case 1009: // MECHAGNOME
                player->AddItem(174067, 1);
                break;
            case 1010: // DRACHTHYR HORDE
                player->LearnSpell(356995, true);
                player->LearnSpell(381322, true);
                player->LearnSpell(355913, true);
                player->LearnSpell(358267, true);
                player->LearnSpell(357210, true);
                player->LearnSpell(351239, true);
                player->AddItem(193914, 1);
                break;
            case 1011: // DRACHTHYR ALLIANCE
                player->LearnSpell(356995, true);
                player->LearnSpell(381322, true);
                player->LearnSpell(355913, true);
                player->LearnSpell(358267, true);
                player->LearnSpell(357210, true);
                player->LearnSpell(351239, true);
                player->AddItem(193914, 1);
                break;
            case 1012: // WORGEN
                player->LearnSpell(68992, true);
                player->AddItem(52040, 1);
                player->SetReputation(1134, 3000);
                break;
            case 1013: // DEATH KNIGHT CLASS
                player->LearnSpell(53428, true); // Runeforging
                player->LearnSpell(50977, true); // Death Gate
                player->SetReputation(72, 6000); // Stormwind Faction
                break;
            case 1014: // DRUID CLASS EXILES REACH
                player->LearnSpell(783, true);
                break;
            case 1015: // HUTNER CLASS EXILES REACH
                player->LearnSpell(83242, true);
                break;
            case 1016: // MAGE CLASS EXILES REACH
                player->LearnSpell(118, true);
                break;
            case 1017: // MONK CLASS EXILES REACH
                player->LearnSpell(322109, true);
                break;
            case 1018: // PALADIN CLASS EXILES REACH
                player->LearnSpell(642, true);
                break;
            case 1019: // PRIEST CLASS EXILES REACH
                player->LearnSpell(2006, true);
                break;
            case 1020: // ROGUE CLASS EXILES REACH
                player->LearnSpell(315584, true);
                break;
            case 1021: // SHAMAN CLASS EXILES REACH
                player->LearnSpell(2645, true);
                break;
            case 1022: // WARLOCK CLASS EXILES REACH
                player->LearnSpell(697, true);
                break;
            case 1023: // WARRIOR CLASS EXILES REACH
                player->LearnSpell(163201, true);
                break;
            case 1024: // HORDE TELEPORTER
                player->TeleportTo(1, 1570.931641f, -4396.939453f, 16.000856f, 0.452704f);
                break;
            case 1025: // ALLIANCE TELEPORTER
                player->TeleportTo(0, -8833.070312f, 622.778015f, 93.931702f, 0.677100f);
                break;
            case 1026: // SET PLAYER LEVEL TO 20
                player->SetLevel(20, true);
                break;
            default:
                break;
            }
            // Goodbye
            CloseGossipMenuFor(player);
            return true;
        }
    };

    // CREATURE AI
    CreatureAI* GetAI(Creature* creature) const override
    {
        return new SkipStartingNPCAI(creature);
    }
};

void AddSC_SkipStartingNPCScripts()
{
    new SkipStartingAnnounce();
    new SkipStartingNPC();
}
