/**
 * This plugin can be used for common player customizations
*/

#include "ScriptMgr.h"
#include "Player.h"
#include "Config.h"
#include "Chat.h"
#include "GossipDef.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "Language.h"
#include "DatabaseEnv.h"
#include "WorldSession.h"
#include "npc_promotion.h"
#include "World.h"

static bool promotionEnable, mountEnable, bagEnable, equippedbags;
static int promotionCount, moneyRewardConst, mountPromotion, bagReward;
static int classConfArmor, LevelForPromotion;

class announce_module : public PlayerScript
{
public:
    announce_module() : PlayerScript("announce_module") { }

    void OnLogin(Player* player, bool /*firstLogin*/)
    {
        if (sConfigMgr->GetBoolDefault("announce_module.enableHelloWorld", true))
        {
            ChatHandler(player->GetSession()).SendSysMessage("This server is running |cff4CFF00Promotion - NPC");
        }
    }
};

class npc_promotion : public CreatureScript
    {
    public:
        npc_promotion() : CreatureScript("npc_promotion") { }

        struct npc_promotionAI : public ScriptedAI
        {
            npc_promotionAI(Creature* creature) : ScriptedAI(creature) { }

        bool OnGossipHello(Player* player) override
        {

            if (player->GetClass() == CLASS_DEATH_KNIGHT)
            {
                return true;
            }

            if (player->GetClass() == CLASS_EVOKER)
            {
                return true;
            }

            if (player->GetLevel() != 1 && player->GetLevel() != 4 && player->GetLevel() != 8 && player->GetLevel() != 58)
            {
                if (player)
                {
                    SendGossipMenuFor(player, 600002, me->GetGUID());
                }
                return true;
            }

            switch (player->GetClass())
            {
            case CLASS_WARRIOR:
                AddGossipItemFor(player, GossipOptionNpc::None, "PvP Dps", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
                AddGossipItemFor(player, GossipOptionNpc::None, "PvP Tank", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
                break;
            case CLASS_WARLOCK:
                AddGossipItemFor(player, GossipOptionNpc::None, "PvP Dps", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);
                break;
            case CLASS_DRUID:
                AddGossipItemFor(player, GossipOptionNpc::None, "PvP Tank", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 4);
                AddGossipItemFor(player, GossipOptionNpc::None, "PvP Heal", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 5);
                AddGossipItemFor(player, GossipOptionNpc::None, "PvP Balance", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 6);
                break;
            case CLASS_HUNTER:
                AddGossipItemFor(player, GossipOptionNpc::None, "PvP Dps ", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 7);
                break;
            case CLASS_MAGE:
                AddGossipItemFor(player, GossipOptionNpc::None, "PvP Dps", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 8);
                break;
            case CLASS_ROGUE:
                AddGossipItemFor(player, GossipOptionNpc::None, "PvP Dps", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 9);
                break;
            case CLASS_PRIEST:
                AddGossipItemFor(player, GossipOptionNpc::None, "PvP Heal", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 10);
                AddGossipItemFor(player, GossipOptionNpc::None, "PvP Shadow", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 11);
                break;
            case CLASS_PALADIN:
                AddGossipItemFor(player, GossipOptionNpc::None, "PvP Tank", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 12);
                AddGossipItemFor(player, GossipOptionNpc::None, "PvP Heal", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 13);
                AddGossipItemFor(player, GossipOptionNpc::None, "PvP Dps", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 14);
                break;
            case CLASS_SHAMAN:
                AddGossipItemFor(player, GossipOptionNpc::None, "PvP Elemental", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 15);
                AddGossipItemFor(player, GossipOptionNpc::None, "PvP Restoration", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 16);
                AddGossipItemFor(player, GossipOptionNpc::None, "PvP Enhancement", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 17);
                break;
            default:
                break;
            }
            SendGossipMenuFor(player, 600000, me->GetGUID());
            return true;
        }

        void MoneyReward(Player* player)
        {
            player->ModifyMoney(sConfigMgr->GetIntDefault("MoneyRewardValue", 25000000));
        }

        void WarriorPromotionDps(Player* player)
        {
            player->EquipNewItem(EQUIPMENT_SLOT_HEAD, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_WARRIOR_DPS_HEAD", 41350)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_NECK, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_WARRIOR_DPS_NECK", 45812)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_SHOULDERS, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_WARRIOR_DPS_SHOULDERS", 41351)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_CHEST, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_WARRIOR_DPS_CHEST", 41353)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_WAIST, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_WARRIOR_DPS_WAIST", 41352)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_LEGS, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_WARRIOR_DPS_LEGS", 41347)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_FEET, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_WARRIOR_DPS_FEET", 41348)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_WRISTS, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_WARRIOR_DPS_WRISTS", 41354)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_HANDS, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_WARRIOR_DPS_HANDS", 41349)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_FINGER1, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_WARRIOR_DPS_FINGER1", 45809)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_FINGER2, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_WARRIOR_DPS_FINGER2", 45809)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_TRINKET1, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_WARRIOR_DPS_TRINKET1", 37064)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_TRINKET2, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_WARRIOR_DPS_TRINKET2", 38359)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_BACK, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_WARRIOR_DPS_BACK", 45811)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_MAINHAND, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_WARRIOR_DPS_MAINHAND", 37108)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_OFFHAND, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_WARRIOR_DPS_OFFHAND", 36962)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_RANGED, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_WARRIOR_DPS_RANGED", 36981)), ItemContext::NONE, true);
        }

        void WarriorPromotionTank(Player* player)
        {
            player->EquipNewItem(EQUIPMENT_SLOT_HEAD, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_WARRIOR_TANK_HEAD", 41350)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_NECK, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_WARRIOR_TANK_NECK", 45812)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_SHOULDERS, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_WARRIOR_TANK_SHOULDERS", 41351)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_CHEST, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_WARRIOR_TANK_CHEST", 41353)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_WAIST, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_WARRIOR_TANK_WAIST", 41352)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_LEGS, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_WARRIOR_TANK_LEGS", 41347)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_FEET, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_WARRIOR_TANK_FEET", 41348)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_WRISTS, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_WARRIOR_TANK_WRISTS", 41354)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_HANDS, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_WARRIOR_TANK_HANDS", 41349)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_FINGER1, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_WARRIOR_TANK_FINGER1", 45809)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_FINGER2, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_WARRIOR_TANK_FINGER2", 45809)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_TRINKET1, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_WARRIOR_TANK_TRINKET1", 36993)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_TRINKET2, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_WARRIOR_TANK_TRINKET2", 38359)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_BACK, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_WARRIOR_TANK_BACK", 45811)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_MAINHAND, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_WARRIOR_TANK_MAINHAND", 36984)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_OFFHAND, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_WARRIOR_TANK_OFFHAND", 37107)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_RANGED, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_WARRIOR_TANK_RANGED", 36981)), ItemContext::NONE, true);
        }

        void PaladinPromotionTank(Player* player)
        {
            player->EquipNewItem(EQUIPMENT_SLOT_HEAD, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_PALADIN_TANK_HEAD", 41350)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_NECK, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_PALADIN_TANK_NECK", 45812)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_SHOULDERS, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_PALADIN_TANK_SHOULDERS", 41351)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_CHEST, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_PALADIN_TANK_CHEST", 41353)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_WAIST, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_PALADIN_TANK_WAIST", 41352)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_LEGS, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_PALADIN_TANK_LEGS", 41347)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_FEET, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_PALADIN_TANK_FEET", 41348)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_WRISTS, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_PALADIN_TANK_WRISTS", 41354)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_HANDS, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_PALADIN_TANK_HANDS", 41349)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_FINGER1, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_PALADIN_TANK_FINGER1", 45809)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_FINGER2, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_PALADIN_TANK_FINGER2", 45809)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_TRINKET1, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_PALADIN_TANK_TRINKET1", 36993)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_TRINKET2, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_PALADIN_TANK_TRINKET2", 38359)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_BACK, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_PALADIN_TANK_BACK", 45811)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_MAINHAND, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_PALADIN_TANK_MAINHAND", 36984)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_OFFHAND, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_PALADIN_TANK_OFFHAND", 37107)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_RANGED, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_PALADIN_TANK_RANGED", 38363)), ItemContext::NONE, true);
        }

        void PaladinPromotionHeal(Player* player)
        {
            player->EquipNewItem(EQUIPMENT_SLOT_HEAD, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_PALADIN_HEAL_HEAD", 42728)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_NECK, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_PALADIN_HEAL_NECK", 45813)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_SHOULDERS, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_PALADIN_HEAL_SHOULDERS", 42727)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_CHEST, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_PALADIN_HEAL_CHEST", 42725)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_WAIST, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_PALADIN_HEAL_WAIST", 42729)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_LEGS, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_PALADIN_HEAL_LEGS", 42726)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_FEET, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_PALADIN_HEAL_FEET", 42730)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_WRISTS, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_PALADIN_HEAL_WRISTS", 42723)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_HANDS, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_PALADIN_HEAL_HANDS", 42724)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_FINGER1, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_PALADIN_HEAL_FINGER1", 45808)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_FINGER2, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_PALADIN_HEAL_FINGER2", 45808)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_TRINKET1, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_PALADIN_HEAL_TRINKET1", 37111)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_TRINKET2, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_PALADIN_HEAL_TRINKET2", 36972)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_BACK, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_PALADIN_HEAL_BACK", 45810)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_MAINHAND, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_PALADIN_HEAL_MAINHAND", 37060)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_OFFHAND, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_PALADIN_HEAL_OFFHAND", 37061)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_RANGED, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_PALADIN_HEAL_RANGED", 38364)), ItemContext::NONE, true);
        }

        void PaladinPromotionDps(Player* player)
        {
            player->EquipNewItem(EQUIPMENT_SLOT_HEAD, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_PALADIN_DPS_HEAD", 41350)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_NECK, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_PALADIN_DPS_NECK", 45812)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_SHOULDERS, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_PALADIN_DPS_SHOULDERS", 41351)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_CHEST, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_PALADIN_DPS_CHEST", 41353)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_WAIST, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_PALADIN_DPS_WAIST", 41352)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_LEGS, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_PALADIN_DPS_LEGS", 41347)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_FEET, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_PALADIN_DPS_FEET", 41348)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_WRISTS, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_PALADIN_DPS_WRISTS", 41354)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_HANDS, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_PALADIN_DPS_HANDS", 41349)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_FINGER1, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_PALADIN_DPS_FINGER1", 45809)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_FINGER2, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_PALADIN_DPS_FINGER2", 45809)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_TRINKET1, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_PALADIN_DPS_TRINKET1", 37064)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_TRINKET2, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_PALADIN_DPS_TRINKET2", 38359)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_BACK, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_PALADIN_DPS_BACK", 45811)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_MAINHAND, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_PALADIN_DPS_MAINHAND", 37108)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_RANGED, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_PALADIN_DPS_RANGED", 38362)), ItemContext::NONE, true);
        }

        void HunterPromotion(Player* player)
        {
            player->EquipNewItem(EQUIPMENT_SLOT_HEAD, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_HUNTER_HEAD", 43447)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_NECK, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_HUNTER_NECK", 45812)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_SHOULDERS, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_HUNTER_SHOULDERS", 43449)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_CHEST, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_HUNTER_CHEST", 43445)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_WAIST, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_HUNTER_WAIST", 43442)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_LEGS, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_HUNTER_LEGS", 43448)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_FEET, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_HUNTER_FEET", 43443)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_WRISTS, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_HUNTER_WRISTS", 43444)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_HANDS, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_HUNTER_HANDS", 43446)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_FINGER1, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_HUNTER_FINGER1", 45809)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_FINGER2, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_HUNTER_FINGER2", 45809)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_TRINKET1, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_HUNTER_TRINKET1", 37064)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_TRINKET2, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_HUNTER_TRINKET2", 38359)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_BACK, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_HUNTER_BACK", 45811)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_MAINHAND, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_HUNTER_MAINHAND", 36980)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_RANGED, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_HUNTER_RANGED", 37050)), ItemContext::NONE, true);
        }

        void RoguePromotion(Player* player)
        {
            player->EquipNewItem(EQUIPMENT_SLOT_HEAD, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_ROGUE_HEAD", 43260)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_NECK, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_ROGUE_NECK", 45812)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_SHOULDERS, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_ROGUE_SHOULDERS", 43433)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_CHEST, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_ROGUE_CHEST", 43434)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_WAIST, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_ROGUE_WAIST", 43437)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_LEGS, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_ROGUE_LEGS", 43438)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_FEET, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_ROGUE_FEET", 43439)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_WRISTS, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_ROGUE_WRISTS", 43435)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_HANDS, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_ROGUE_HANDS", 43436)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_FINGER1, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_ROGUE_FINGER1", 45809)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_FINGER2, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_ROGUE_FINGER2", 45809)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_TRINKET1, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_ROGUE_TRINKET1", 37064)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_TRINKET2, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_ROGUE_TRINKET2", 38359)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_BACK, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_ROGUE_BACK", 45811)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_MAINHAND, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_ROGUE_MAINHAND", 36944)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_OFFHAND, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_ROGUE_OFFHAND", 37037)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_RANGED, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_ROGUE_RANGED", 36981)), ItemContext::NONE, true);
        }

        void PriestPromotionHeal(Player* player)
        {
            player->EquipNewItem(EQUIPMENT_SLOT_HEAD, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_PRIEST_HEAL_HEAD", 43971)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_NECK, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_PRIEST_HEAL_NECK", 45813)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_SHOULDERS, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_PRIEST_HEAL_SHOULDERS", 43973)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_CHEST, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_PRIEST_HEAL_CHEST", 43972)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_WAIST, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_PRIEST_HEAL_WAIST", 43969)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_LEGS, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_PRIEST_HEAL_LEGS", 43975)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_FEET, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_PRIEST_HEAL_FEET", 43970)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_WRISTS, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_PRIEST_HEAL_WRISTS", 43974)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_HANDS, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_PRIEST_HEAL_HANDS", 41516)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_FINGER1, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_PRIEST_HEAL_FINGER1", 45808)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_FINGER2, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_PRIEST_HEAL_FINGER2", 45808)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_TRINKET1, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_PRIEST_HEAL_TRINKET1", 36972)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_TRINKET2, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_PRIEST_HEAL_TRINKET2", 37111)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_BACK, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_PRIEST_HEAL_BACK", 45810)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_MAINHAND, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_PRIEST_HEAL_MAINHAND", 36975)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_RANGED, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_PRIEST_HEAL_RANGED", 37038)), ItemContext::NONE, true);
        }

        void PriestPromotionShadow(Player* player)
        {
            player->EquipNewItem(EQUIPMENT_SLOT_HEAD, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_PRIEST_SHADOW_HEAD", 43971)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_NECK, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_PRIEST_SHADOW_NECK", 45813)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_SHOULDERS, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_PRIEST_SHADOW_SHOULDERS", 43973)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_CHEST, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_PRIEST_SHADOW_CHEST", 43972)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_WAIST, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_PRIEST_SHADOW_WAIST", 43969)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_LEGS, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_PRIEST_SHADOW_LEGS", 43975)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_FEET, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_PRIEST_SHADOW_FEET", 43970)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_WRISTS, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_PRIEST_SHADOW_WRISTS", 43974)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_HANDS, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_PRIEST_SHADOW_HANDS", 41516)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_FINGER1, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_PRIEST_SHADOW_FINGER1", 45808)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_FINGER2, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_PRIEST_SHADOW_FINGER2", 45808)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_TRINKET1, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_PRIEST_SHADOW_TRINKET1", 36972)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_TRINKET2, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_PRIEST_SHADOW_TRINKET2", 38358)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_BACK, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_PRIEST_SHADOW_BACK", 45810)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_MAINHAND, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_PRIEST_SHADOW_MAINHAND", 36975)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_RANGED, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_PRIEST_SHADOW_RANGED", 37038)), ItemContext::NONE, true);

        }

        void ShamanPromotionCaster(Player* player)
        {
            player->EquipNewItem(EQUIPMENT_SLOT_HEAD, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_SHAMAN_CASTER_HEAD", 43455)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_NECK, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_SHAMAN_CASTER_NECK", 45813)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_SHOULDERS, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_SHAMAN_CASTER_SHOULDERS", 43457)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_CHEST, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_SHAMAN_CASTER_CHEST", 43453)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_WAIST, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_SHAMAN_CASTER_WAIST", 43450)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_LEGS, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_SHAMAN_CASTER_LEGS", 43456)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_FEET, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_SHAMAN_CASTER_FEET", 43451)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_WRISTS, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_SHAMAN_CASTER_WRISTS", 43452)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_HANDS, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_SHAMAN_CASTER_HANDS", 43454)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_FINGER1, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_SHAMAN_CASTER_FINGER1", 45808)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_FINGER2, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_SHAMAN_CASTER_FINGER2", 45808)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_TRINKET1, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_SHAMAN_CASTER_TRINKET1", 36972)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_TRINKET2, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_SHAMAN_CASTER_TRINKET2", 38358)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_BACK, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_SHAMAN_CASTER_BACK", 45810)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_MAINHAND, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_SHAMAN_CASTER_MAINHAND", 37112)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_OFFHAND, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_SHAMAN_CASTER_OFFHAND", 37061)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_RANGED, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_SHAMAN_CASTER_RANGED", 38361)), ItemContext::NONE, true);
        }

        void ShamanPromotionMelee(Player* player)
        {
            player->EquipNewItem(EQUIPMENT_SLOT_HEAD, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_SHAMAN_MELEE_HEAD", 43447)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_NECK, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_SHAMAN_MELEE_NECK", 45812)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_SHOULDERS, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_SHAMAN_MELEE_SHOULDERS", 43449)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_CHEST, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_SHAMAN_MELEE_CHEST", 43445)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_WAIST, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_SHAMAN_MELEE_WAIST", 43442)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_LEGS, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_SHAMAN_MELEE_LEGS", 43448)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_FEET, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_SHAMAN_MELEE_FEET", 43443)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_WRISTS, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_SHAMAN_MELEE_WRISTS", 43444)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_HANDS, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_SHAMAN_MELEE_HANDS", 43446)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_FINGER1, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_SHAMAN_MELEE_FINGER1", 45809)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_FINGER2, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_SHAMAN_MELEE_FINGER2", 45809)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_TRINKET1, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_SHAMAN_MELEE_TRINKET1", 37064)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_TRINKET2, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_SHAMAN_MELEE_TRINKET2", 38359)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_BACK, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_SHAMAN_MELEE_BACK", 45811)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_MAINHAND, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_SHAMAN_MELEE_MAINHAND", 35630)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_OFFHAND, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_SHAMAN_MELEE_OFFHAND", 36944)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_RANGED, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_SHAMAN_MELEE_RANGED", 38367)), ItemContext::NONE, true);
        }

        void ShamanPromotionHeal(Player* player)
        {
            player->EquipNewItem(EQUIPMENT_SLOT_HEAD, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_SHAMAN_HEAL_HEAD", 43455)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_NECK, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_SHAMAN_HEAL_NECK", 45813)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_SHOULDERS, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_SHAMAN_HEAL_SHOULDERS", 43457)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_CHEST, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_SHAMAN_HEAL_CHEST", 43453)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_WAIST, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_SHAMAN_HEAL_WAIST", 43450)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_LEGS, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_SHAMAN_HEAL_LEGS", 43456)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_FEET, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_SHAMAN_HEAL_FEET", 43451)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_WRISTS, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_SHAMAN_HEAL_WRISTS", 43452)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_HANDS, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_SHAMAN_HEAL_HANDS", 43454)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_FINGER1, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_SHAMAN_HEAL_FINGER1", 45808)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_FINGER2, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_SHAMAN_HEAL_FINGER2", 45808)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_TRINKET1, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_SHAMAN_HEAL_TRINKET1", 36972)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_TRINKET2, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_SHAMAN_HEAL_TRINKET2", 37111)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_BACK, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_SHAMAN_HEAL_BACK", 45810)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_MAINHAND, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_SHAMAN_HEAL_MAINHAND", 37112)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_OFFHAND, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_SHAMAN_HEAL_OFFHAND", 37061)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_RANGED, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_SHAMAN_HEAL_RANGED", 38368)), ItemContext::NONE, true);
        }

        void MagePromotionDps(Player* player)
        {
            player->EquipNewItem(EQUIPMENT_SLOT_HEAD, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_MAGE_HEAD", 43971)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_NECK, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_MAGE_NECK", 45813)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_SHOULDERS, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_MAGE_SHOULDERS", 43973)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_CHEST, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_MAGE_CHEST", 43972)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_WAIST, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_MAGE_WAIST", 43969)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_LEGS, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_MAGE_LEGS", 43975)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_FEET, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_MAGE_FEET", 43970)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_WRISTS, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_MAGE_WRISTS", 43974)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_HANDS, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_MAGE_HANDS", 41516)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_FINGER1, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_MAGE_FINGER1", 45808)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_FINGER2, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_MAGE_FINGER2", 45808)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_TRINKET1, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_MAGE_TRINKET1", 36972)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_TRINKET2, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_MAGE_TRINKET2", 38358)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_BACK, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_MAGE_BACK", 45810)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_MAINHAND, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_MAGE_MAINHAND", 36975)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_RANGED, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_MAGE_RANGED", 37038)), ItemContext::NONE, true);

        }

        void WarlockPromotionDps(Player* player)
        {
            player->EquipNewItem(EQUIPMENT_SLOT_HEAD, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_WARLOCK_HEAD", 43971)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_NECK, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_WARLOCK_NECK", 45813)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_SHOULDERS, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_WARLOCK_SHOULDERS", 43973)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_CHEST, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_WARLOCK_CHEST", 43972)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_WAIST, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_WARLOCK_WAIST", 43969)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_LEGS, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_WARLOCK_LEGS", 43975)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_FEET, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_WARLOCK_FEET", 43970)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_WRISTS, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_WARLOCK_WRISTS", 43974)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_HANDS, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_WARLOCK_HANDS", 41516)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_FINGER1, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_WARLOCK_FINGER1", 45808)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_FINGER2, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_WARLOCK_FINGER2", 45808)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_TRINKET1, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_WARLOCK_TRINKET1", 36972)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_TRINKET2, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_WARLOCK_TRINKET2", 38358)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_BACK, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_WARLOCK_BACK", 45810)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_MAINHAND, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_WARLOCK_MAINHAND", 36975)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_RANGED, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_WARLOCK_RANGED", 37038)), ItemContext::NONE, true);
        }

        void DruidPromotionTank(Player* player)
        {
            player->EquipNewItem(EQUIPMENT_SLOT_HEAD, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_DRUID_TANK_HEAD", 43260)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_NECK, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_DRUID_TANK_NECK", 45812)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_SHOULDERS, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_DRUID_TANK_SHOULDERS", 43433)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_CHEST, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_DRUID_TANK_CHEST", 43434)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_WAIST, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_DRUID_TANK_WAIST", 43437)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_LEGS, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_DRUID_TANK_LEGS", 43438)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_FEET, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_DRUID_TANK_FEET", 43439)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_WRISTS, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_DRUID_TANK_WRISTS", 43435)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_HANDS, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_DRUID_TANK_HANDS", 43436)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_FINGER1, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_DRUID_TANK_FINGER1", 45809)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_FINGER2, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_DRUID_TANK_FINGER2", 45809)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_TRINKET1, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_DRUID_TANK_TRINKET1", 37064)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_TRINKET2, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_DRUID_TANK_TRINKET2", 38359)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_BACK, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_DRUID_TANK_BACK", 45811)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_MAINHAND, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_DRUID_TANK_MAINHAND", 36980)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_RANGED, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_DRUID_TANK_RANGED", 38365)), ItemContext::NONE, true);
        }

        void DruidPromotionHeal(Player* player)
        {
            player->EquipNewItem(EQUIPMENT_SLOT_HEAD, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_DRUID_HEAL_HEAD", 43261)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_NECK, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_DRUID_HEAL_NECK", 45813)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_SHOULDERS, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_DRUID_HEAL_SHOULDERS", 43262)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_CHEST, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_DRUID_HEAL_CHEST", 43263)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_WAIST, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_DRUID_HEAL_WAIST", 43266)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_LEGS, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_DRUID_HEAL_LEGS", 43271)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_FEET, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_DRUID_HEAL_FEET", 43273)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_WRISTS, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_DRUID_HEAL_WRISTS", 43264)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_HANDS, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_DRUID_HEAL_HANDS", 43265)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_FINGER1, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_DRUID_HEAL_FINGER1", 45808)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_FINGER2, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_DRUID_HEAL_FINGER2", 45808)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_TRINKET1, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_DRUID_HEAL_TRINKET1", 36972)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_TRINKET2, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_DRUID_HEAL_TRINKET2", 37111)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_BACK, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_DRUID_HEAL_BACK", 45810)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_MAINHAND, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_DRUID_HEAL_MAINHAND", 36975)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_RANGED, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_DRUID_HEAL_RANGED", 38366)), ItemContext::NONE, true);
        }

        void DruidPromotionMelee(Player* player)
        {
            player->EquipNewItem(EQUIPMENT_SLOT_HEAD, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_DRUID_MELEE_HEAD", 43260)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_NECK, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_DRUID_MELEE_NECK", 45812)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_SHOULDERS, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_DRUID_MELEE_SHOULDERS", 43433)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_CHEST, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_DRUID_MELEE_CHEST", 43434)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_WAIST, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_DRUID_MELEE_WAIST", 43437)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_LEGS, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_DRUID_MELEE_LEGS", 43438)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_FEET, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_DRUID_MELEE_FEET", 43439)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_WRISTS, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_DRUID_MELEE_WRISTS", 43435)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_HANDS, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_DRUID_MELEE_HANDS", 43436)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_FINGER1, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_DRUID_MELEE_FINGER1", 45809)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_FINGER2, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_DRUID_MELEE_FINGER2", 45809)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_TRINKET1, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_DRUID_MELEE_TRINKET1", 37064)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_TRINKET2, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_DRUID_MELEE_TRINKET2", 38359)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_BACK, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_DRUID_MELEE_BACK", 45811)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_MAINHAND, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_DRUID_MELEE_MAINHAND", 36980)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_RANGED, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_DRUID_MELEE_RANGED", 38365)), ItemContext::NONE, true);
        }

        void DruidPromotionCaster(Player* player)
        {
            player->EquipNewItem(EQUIPMENT_SLOT_HEAD, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_DRUID_CASTER_HEAD", 43261)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_NECK, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_DRUID_CASTER_NECK", 45813)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_SHOULDERS, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_DRUID_CASTER_SHOULDERS", 43262)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_CHEST, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_DRUID_CASTER_CHEST", 43263)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_WAIST, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_DRUID_CASTER_WAIST", 43266)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_LEGS, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_DRUID_CASTER_LEGS", 43271)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_FEET, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_DRUID_CASTER_FEET", 43273)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_WRISTS, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_DRUID_CASTER_WRISTS", 43264)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_HANDS, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_DRUID_CASTER_HANDS", 43265)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_FINGER1, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_DRUID_CASTER_FINGER1", 45808)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_FINGER2, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_DRUID_CASTER_FINGER2", 45808)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_TRINKET1, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_DRUID_CASTER_TRINKET1", 36972)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_TRINKET2, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_DRUID_CASTER_TRINKET2", 38358)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_BACK, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_DRUID_CASTER_BACK", 45810)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_MAINHAND, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_DRUID_CASTER_MAINHAND", 36975)), ItemContext::NONE, true);
            player->EquipNewItem(EQUIPMENT_SLOT_RANGED, (sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_DRUID_CASTER_RANGED", 38360)), ItemContext::NONE, true);
        }

        bool OnGossipSelect(Player* player, uint32 /*menuId*/, uint32 gossipListId) override
        {
            //uint32 const action = player->PlayerTalkClass->GetGossipOptionAction(gossipListId);
            uint32 accountID = player->GetSession()->GetAccountId();
            QueryResult result = CharacterDatabase.PQuery("SELECT COUNT(`guid`) FROM `characters` WHERE `account` = {}", accountID);
            Field* fields = result->Fetch();
            uint32 pjts = fields[0].GetUInt32();

            // Ensure the player is valid
            if (!player)
                return false;

            ClearGossipMenuFor(player);

            if (promotionEnable && ((int)pjts <= promotionCount))
            {
                if (gossipListId > GOSSIP_ACTION_INFO_DEF && gossipListId < 1020)
                    player->GiveLevel(sConfigMgr->GetIntDefault("LevelForPromotion", 80)); // Level

                //Money 2,5k
                MoneyReward(player);

                 // Skill Max
                player->UpdateSkillsForLevel();
                /*UpdateSkillsToMaxSkillsForLevel();*/

                // Riding
                player->LearnSpell((SKILL_RIDING_75), true);
                player->LearnSpell((SKILL_RIDING_100), true);
                player->LearnSpell((SKILL_RIDING_FLYING), true);
                player->LearnSpell((SKILL_RIDING_ARTISING), true);

                me->Whisper("You Got Your Promotion!", LANG_UNIVERSAL, player);

                if (mountEnable)
                {
                    player->LearnSpell(sConfigMgr->GetIntDefault("mountPromotion", 42777), true); //Swift Spectral Tiger
                }
        
                //Bags
                if (bagEnable)
                {
                    if (equippedbags)
                    {
                        for (int slot = INVENTORY_SLOT_BAG_START; slot < INVENTORY_SLOT_BAG_END; slot++)
                            if (player->GetItemByPos(INVENTORY_SLOT_BAG_0, slot))
                                player->DestroyItem(INVENTORY_SLOT_BAG_0, slot, true);

                        for (int slot = INVENTORY_SLOT_BAG_START; slot < INVENTORY_SLOT_BAG_END; slot++)
                            player->EquipNewItem(slot, (sConfigMgr->GetIntDefault("bagReward.Id", 14156)), ItemContext::NONE, true);
                    }
                    else
                    {
                        player->AddItem((sConfigMgr->GetIntDefault("bagReward.Id", 14156)), 4);
                    }
                }

                switch (gossipListId)
                {
                    case 1001:
                        WarriorPromotionDps(player);
                        break;
                    case 1002: 
                        WarriorPromotionTank(player);
                        break;
                    case 1003:
                        WarlockPromotionDps(player);
                        break;
                    case 1004:
                        DruidPromotionTank(player);
                        break;
                    case 1005:
                        DruidPromotionHeal(player);
                        break;
                    case 1006:
                        DruidPromotionCaster(player);
                        break;
                    case 1007:
                        HunterPromotion(player);
                        break;
                    case 1008:
                        MagePromotionDps(player);
                        break;
                    case 1009:
                        RoguePromotion(player);
                        break;
                    case 1010:
                        PriestPromotionHeal(player);
                        break;
                    case 1011:
                        PriestPromotionShadow(player);
                        break;
                    case 1012:
                        PaladinPromotionTank(player);
                        break;
                    case 1013:
                        PaladinPromotionHeal(player);
                        break;
                    case 1014:
                        PaladinPromotionDps(player);
                        break;
                    default:
                        break;
                }
            }
            else
            {
                me->Whisper("You Got Your Promotion already! Or you have at least one character level 1+ on this realm.", LANG_UNIVERSAL, player);
                SendGossipMenuFor(player, 600001, me->GetGUID());
            }
            CloseGossipMenuFor(player);
            return true;
        }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_promotionAI(creature);
    }
};

class mod_promotion_conf : public WorldScript
{
public:
    mod_promotion_conf() : WorldScript("mod_promotion.conf") { }

    void OnConfigLoad(bool reload)
    {
        if (!reload)
        {
            // Config For Module
            promotionEnable = sConfigMgr->GetBoolDefault("promotionEnable.enable", true);
            promotionCount = sConfigMgr->GetIntDefault("promotion.count", 1);
            moneyRewardConst = sConfigMgr->GetIntDefault("MoneyRewardValue", 25000000);
            LevelForPromotion = sConfigMgr->GetIntDefault("LevelForPromotion", 80);
            mountPromotion = sConfigMgr->GetIntDefault("mountPromotion", 42277);
            mountEnable = sConfigMgr->GetBoolDefault("mountEnable.enable", true);
            bagEnable = sConfigMgr->GetBoolDefault("bagEnable.enable", true);
            equippedbags = sConfigMgr->GetBoolDefault("equippedbags.enable", true);
            bagReward = sConfigMgr->GetIntDefault("bagReward.Id", 14156);

            /*
            * ARMOR CONFIGURATION FOR CLASSES
            */
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_WARRIOR_TANK_HEAD", 41350);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_WARRIOR_TANK_NECK", 45812);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_WARRIOR_TANK_SHOULDERS", 41351);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_WARRIOR_TANK_CHEST", 41353);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_WARRIOR_TANK_WAIST", 41352);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_WARRIOR_TANK_LEGS", 41347);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_WARRIOR_TANK_FEET", 41348);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_WARRIOR_TANK_WRISTS", 41354);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_WARRIOR_TANK_HANDS", 41349);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_WARRIOR_TANK_FINGER1", 45809);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_WARRIOR_TANK_FINGER2", 45809);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_WARRIOR_TANK_TRINKET1", 36993);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_WARRIOR_TANK_TRINKET2", 38359);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_WARRIOR_TANK_BACK", 45811);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_WARRIOR_TANK_MAINHAND", 36984);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_WARRIOR_TANK_OFFHAND", 37107);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_WARRIOR_TANK_RANGED", 36981);

            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_WARRIOR_DPS_HEAD", 41350);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_WARRIOR_DPS_NECK", 45812);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_WARRIOR_DPS_SHOULDERS", 41351);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_WARRIOR_DPS_CHEST", 41353);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_WARRIOR_DPS_WAIST", 41352);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_WARRIOR_DPS_LEGS", 41347);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_WARRIOR_DPS_FEET", 41348);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_WARRIOR_DPS_WRISTS", 41354);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_WARRIOR_DPS_HANDS", 41349);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_WARRIOR_DPS_FINGER1", 45809);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_WARRIOR_DPS_FINGER2", 45809);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_WARRIOR_DPS_TRINKET1", 37064);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_WARRIOR_DPS_TRINKET2", 38359);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_WARRIOR_DPS_BACK", 45811);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_WARRIOR_DPS_MAINHAND", 37108);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_WARRIOR_DPS_OFFHAND", 36962);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_WARRIOR_DPS_RANGED", 36981);

            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_PALADIN_TANK_HEAD", 41350);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_PALADIN_TANK_NECK", 45812);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_PALADIN_TANK_SHOULDERS", 41351);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_PALADIN_TANK_CHEST", 41353);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_PALADIN_TANK_WAIST", 41352);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_PALADIN_TANK_LEGS", 41347);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_PALADIN_TANK_FEET", 41348);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_PALADIN_TANK_WRISTS", 41354);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_PALADIN_TANK_HANDS", 41349);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_PALADIN_TANK_FINGER1", 45809);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_PALADIN_TANK_FINGER2", 45809);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_PALADIN_TANK_TRINKET1", 36993);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_PALADIN_TANK_TRINKET2", 38359);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_PALADIN_TANK_BACK", 45811);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_PALADIN_TANK_MAINHAND", 36984);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_PALADIN_TANK_OFFHAND", 37107);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_PALADIN_TANK_RANGED", 38363);

            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_PALADIN_HEAL_HEAD", 42728);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_PALADIN_HEAL_NECK", 45813);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_PALADIN_HEAL_SHOULDERS", 42727);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_PALADIN_HEAL_CHEST", 42725);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_PALADIN_HEAL_WAIST", 42729);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_PALADIN_HEAL_LEGS", 42726);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_PALADIN_HEAL_FEET", 42730);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_PALADIN_HEAL_WRISTS", 42723);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_PALADIN_HEAL_HANDS", 42724);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_PALADIN_HEAL_FINGER1", 45808);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_PALADIN_HEAL_FINGER2", 45808);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_PALADIN_HEAL_TRINKET1", 37111);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_PALADIN_HEAL_TRINKET2", 36972);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_PALADIN_HEAL_BACK", 45810);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_PALADIN_HEAL_MAINHAND", 37060);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_PALADIN_HEAL_OFFHAND", 37061);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_PALADIN_HEAL_RANGED", 38364);

            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_PALADIN_DPS_HEAD", 41350);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_PALADIN_DPS_NECK", 45812);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_PALADIN_DPS_SHOULDERS", 41351);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_PALADIN_DPS_CHEST", 41353);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_PALADIN_DPS_WAIST", 41352);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_PALADIN_DPS_LEGS", 41347);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_PALADIN_DPS_FEET", 41348);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_PALADIN_DPS_WRISTS", 41354);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_PALADIN_DPS_HANDS", 41349);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_PALADIN_DPS_FINGER1", 45809);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_PALADIN_DPS_FINGER2", 45809);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_PALADIN_DPS_TRINKET1", 37064);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_PALADIN_DPS_TRINKET2", 38359);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_PALADIN_DPS_BACK", 45811);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_PALADIN_DPS_MAINHAND", 37108);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_PALADIN_DPS_RANGED", 38362);

            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_HUNTER_HEAD", 43447);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_HUNTER_NECK", 45812);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_HUNTER_SHOULDERS", 43449);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_HUNTER_CHEST", 43445);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_HUNTER_WAIST", 43442);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_HUNTER_LEGS", 43448);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_HUNTER_FEET", 43443);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_HUNTER_WRISTS", 43444);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_HUNTER_HANDS", 43446);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_HUNTER_FINGER1", 45809);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_HUNTER_FINGER2", 45809);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_HUNTER_TRINKET1", 37064);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_HUNTER_TRINKET2", 38359);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_HUNTER_BACK", 45811);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_HUNTER_MAINHAND", 36980);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_HUNTER_RANGED", 37050);

            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_ROGUE_HEAD", 43260);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_ROGUE_NECK", 45812);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_ROGUE_SHOULDERS", 43433);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_ROGUE_CHEST", 43434);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_ROGUE_WAIST", 43437);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_ROGUE_LEGS", 43438);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_ROGUE_FEET", 43439);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_ROGUE_WRISTS", 43435);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_ROGUE_HANDS", 43436);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_ROGUE_FINGER1", 45809);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_ROGUE_FINGER2", 45809);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_ROGUE_TRINKET1", 37064);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_ROGUE_TRINKET2", 38359);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_ROGUE_BACK", 45811);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_ROGUE_MAINHAND", 36944);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_ROGUE_OFFHAND", 37037);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_ROGUE_RANGED", 36981);

            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_PRIEST_HEAL_HEAD", 43971);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_PRIEST_HEAL_NECK", 45813);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_PRIEST_HEAL_SHOULDERS", 43973);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_PRIEST_HEAL_CHEST", 43972);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_PRIEST_HEAL_WAIST", 43969);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_PRIEST_HEAL_LEGS", 43975);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_PRIEST_HEAL_FEET", 43970);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_PRIEST_HEAL_WRISTS", 43974);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_PRIEST_HEAL_HANDS", 41516);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_PRIEST_HEAL_FINGER1", 45808);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_PRIEST_HEAL_FINGER2", 45808);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_PRIEST_HEAL_TRINKET1", 36972);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_PRIEST_HEAL_TRINKET2", 37111);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_PRIEST_HEAL_BACK", 45810);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_PRIEST_HEAL_MAINHAND", 36975);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_PRIEST_HEAL_RANGED", 37038);

            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_PRIEST_SHADOW_HEAD", 43971);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_PRIEST_SHADOW_NECK", 45813);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_PRIEST_SHADOW_SHOULDERS", 43973);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_PRIEST_SHADOW_CHEST", 43972);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_PRIEST_SHADOW_WAIST", 43969);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_PRIEST_SHADOW_LEGS", 43975);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_PRIEST_SHADOW_FEET", 43970);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_PRIEST_SHADOW_WRISTS", 43974);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_PRIEST_SHADOW_HANDS", 41516);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_PRIEST_SHADOW_FINGER1", 45808);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_PRIEST_SHADOW_FINGER2", 45808);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_PRIEST_SHADOW_TRINKET1", 36972);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_PRIEST_SHADOW_TRINKET2", 38358);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_PRIEST_SHADOW_BACK", 45810);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_PRIEST_SHADOW_MAINHAND", 36975);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_PRIEST_SHADOW_RANGED", 37038);

            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_SHAMAN_CASTER_HEAD", 43455);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_SHAMAN_CASTER_NECK", 45813);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_SHAMAN_CASTER_SHOULDERS", 43457);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_SHAMAN_CASTER_CHEST", 43453);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_SHAMAN_CASTER_WAIST", 43450);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_SHAMAN_CASTER_LEGS", 43456);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_SHAMAN_CASTER_FEET", 43451);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_SHAMAN_CASTER_WRISTS", 43452);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_SHAMAN_CASTER_HANDS", 43454);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_SHAMAN_CASTER_FINGER1", 45808);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_SHAMAN_CASTER_FINGER2", 45808);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_SHAMAN_CASTER_TRINKET1", 36972);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_SHAMAN_CASTER_TRINKET2", 38358);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_SHAMAN_CASTER_BACK", 45810);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_SHAMAN_CASTER_MAINHAND", 37112);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_SHAMAN_CASTER_OFFHAND", 37061);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_SHAMAN_CASTER_RANGED", 38361);

            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_SHAMAN_MELEE_HEAD", 43447);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_SHAMAN_MELEE_NECK", 45812);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_SHAMAN_MELEE_SHOULDERS", 43449);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_SHAMAN_MELEE_CHEST", 43445);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_SHAMAN_MELEE_WAIST", 43442);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_SHAMAN_MELEE_LEGS", 43448);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_SHAMAN_MELEE_FEET", 43443);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_SHAMAN_MELEE_WRISTS", 43444);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_SHAMAN_MELEE_HANDS", 43446);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_SHAMAN_MELEE_FINGER1", 45809);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_SHAMAN_MELEE_FINGER2", 45809);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_SHAMAN_MELEE_TRINKET1", 37064);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_SHAMAN_MELEE_TRINKET2", 38359);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_SHAMAN_MELEE_BACK", 45811);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_SHAMAN_MELEE_MAINHAND", 35630);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_SHAMAN_MELEE_OFFHAND", 36944);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_SHAMAN_MELEE_RANGED", 38367);

            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_SHAMAN_HEAL_HEAD", 43455);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_SHAMAN_HEAL_NECK", 45813);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_SHAMAN_HEAL_SHOULDERS", 43457);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_SHAMAN_HEAL_CHEST", 43453);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_SHAMAN_HEAL_WAIST", 43450);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_SHAMAN_HEAL_LEGS", 43456);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_SHAMAN_HEAL_FEET", 43451);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_SHAMAN_HEAL_WRISTS", 43452);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_SHAMAN_HEAL_HANDS", 43454);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_SHAMAN_HEAL_FINGER1", 45808);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_SHAMAN_HEAL_FINGER2", 45808);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_SHAMAN_HEAL_TRINKET1", 36972);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_SHAMAN_HEAL_TRINKET2", 37111);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_SHAMAN_HEAL_BACK", 45810);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_SHAMAN_HEAL_MAINHAND", 37112);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_SHAMAN_HEAL_OFFHAND", 37061);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_SHAMAN_HEAL_RANGED", 38368);

            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_MAGE_HEAD", 43971);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_MAGE_NECK", 45813);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_MAGE_SHOULDERS", 43973);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_MAGE_CHEST", 43972);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_MAGE_WAIST", 43969);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_MAGE_LEGS", 43975);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_MAGE_FEET", 43970);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_MAGE_WRISTS", 43974);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_MAGE_HANDS", 41516);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_MAGE_FINGER1", 45808);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_MAGE_FINGER2", 45808);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_MAGE_TRINKET1", 36972);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_MAGE_TRINKET2", 38358);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_MAGE_BACK", 45810);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_MAGE_MAINHAND", 36975);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_MAGE_RANGED", 37038);

            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_WARLOCK_HEAD", 43971);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_WARLOCK_NECK", 45813);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_WARLOCK_SHOULDERS", 43973);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_WARLOCK_CHEST", 43972);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_WARLOCK_WAIST", 43969);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_WARLOCK_LEGS", 43975);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_WARLOCK_FEET", 43970);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_WARLOCK_WRISTS", 43974);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_WARLOCK_HANDS", 41516);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_WARLOCK_FINGER1", 45808);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_WARLOCK_FINGER2", 45808);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_WARLOCK_TRINKET1", 36972);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_WARLOCK_TRINKET2", 38358);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_WARLOCK_BACK", 45810);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_WARLOCK_MAINHAND", 36975);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_WARLOCK_RANGED", 37038);

            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_DRUID_TANK_HEAD", 43260);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_DRUID_TANK_NECK", 45812);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_DRUID_TANK_SHOULDERS", 43433);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_DRUID_TANK_CHEST", 43434);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_DRUID_TANK_WAIST", 43437);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_DRUID_TANK_LEGS", 43438);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_DRUID_TANK_FEET", 43439);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_DRUID_TANK_WRISTS", 43435);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_DRUID_TANK_HANDS", 43436);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_DRUID_TANK_FINGER1", 45809);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_DRUID_TANK_FINGER2", 45809);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_DRUID_TANK_TRINKET1", 37064);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_DRUID_TANK_TRINKET2", 38359);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_DRUID_TANK_BACK", 45811);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_DRUID_TANK_MAINHAND", 36980);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_DRUID_TANK_RANGED", 38365);

            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_DRUID_HEAL_HEAD", 43261);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_DRUID_HEAL_NECK", 45813);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_DRUID_HEAL_SHOULDERS", 43262);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_DRUID_HEAL_CHEST", 43263);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_DRUID_HEAL_WAIST", 43266);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_DRUID_HEAL_LEGS", 43271);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_DRUID_HEAL_FEET", 43273);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_DRUID_HEAL_WRISTS", 43264);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_DRUID_HEAL_HANDS", 43265);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_DRUID_HEAL_FINGER1", 45808);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_DRUID_HEAL_FINGER2", 45808);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_DRUID_HEAL_TRINKET1", 36972);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_DRUID_HEAL_TRINKET2", 37111);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_DRUID_HEAL_BACK", 45810);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_DRUID_HEAL_MAINHAND", 36975);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_DRUID_HEAL_RANGED", 38366);

            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_DRUID_MELEE_HEAD", 43260);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_DRUID_MELEE_NECK", 45812);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_DRUID_MELEE_SHOULDERS", 43433);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_DRUID_MELEE_CHEST", 43434);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_DRUID_MELEE_WAIST", 43437);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_DRUID_MELEE_LEGS", 43438);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_DRUID_MELEE_FEET", 43439);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_DRUID_MELEE_WRISTS", 43435);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_DRUID_MELEE_HANDS", 43436);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_DRUID_MELEE_FINGER1", 45809);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_DRUID_MELEE_FINGER2", 45809);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_DRUID_MELEE_TRINKET1", 37064);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_DRUID_MELEE_TRINKET2", 38359);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_DRUID_MELEE_BACK", 45811);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_DRUID_MELEE_MAINHAND", 36980);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_DRUID_MELEE_RANGED", 38365);

            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_DRUID_CASTER_HEAD", 43261);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_DRUID_CASTER_NECK", 45813);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_DRUID_CASTER_SHOULDERS", 43262);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_DRUID_CASTER_CHEST", 43263);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_DRUID_CASTER_WAIST", 43266);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_DRUID_CASTER_LEGS", 43271);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_DRUID_CASTER_FEET", 43273);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_DRUID_CASTER_WRISTS", 43264);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_DRUID_CASTER_HANDS", 43265);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_DRUID_CASTER_FINGER1", 45808);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_DRUID_CASTER_FINGER2", 45808);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_DRUID_CASTER_TRINKET1", 36972);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_DRUID_CASTER_TRINKET2", 38358);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_DRUID_CASTER_BACK", 45810);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_DRUID_CASTER_MAINHAND", 36975);
            classConfArmor = sConfigMgr->GetIntDefault("EQUIPMENT_SLOT_DRUID_CASTER_RANGED", 38360);
        }
    }
};

void AddSC_npc_promotion()
{
    new announce_module();
    new npc_promotion();
    new mod_promotion_conf();
}
