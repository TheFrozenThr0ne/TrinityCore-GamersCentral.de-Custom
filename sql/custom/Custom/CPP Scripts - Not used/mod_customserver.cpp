/*
# Custom Login Modifications #
------------------------------------------------------------------------------------------------------------------
### Description ###
------------------------------------------------------------------------------------------------------------------
This module performs several actions on new players. It has the option to give new players BOA starting gear,
additional weapon skills, and special abilities such as custom spells. It can also set the reputation of the player
to exalted with all capital cities for their faction granting them the Ambassador title. This is typically done in
the core's config file, but it's bugged (as of 2017.08.23) in AzerothCore. It can also announce when players login
or logoff the server.
### Features ###
------------------------------------------------------------------------------------------------------------------
- Player ([ Faction ] - Name - Logon/Logoff message) notification can be announced to the world
- New characters can receive items, bags, and class-specific heirlooms
- New characters can receive additional weapon skills
- New characters can receive special abilities
- New characters can receive exalted rep with capital cities (Title: Ambassador) on first login
### Data ###
------------------------------------------------------------------------------------------------------------------
- Type: Player/Server
- Script: CustomLogin
- Config: Yes
    - Enable Module
    - Enable Module Announce
    - Enable Announce Player Login/Logoff
    - Enable Starting Gear for new players
    - Enable Additional Weapon Skills for new players
    - Enable Special Abilities for new players
    - Enable Reputation Boost for new players
- SQL: No
### Version ###
------------------------------------------------------------------------------------------------------------------
- v2017.07.26 - Release
- v2017.07.29 - Clean up code, Add rep gain, Add config options
### Credits ###
------------------------------------------------------------------------------------------------------------------
- [Blizzard Entertainment](http://blizzard.com)
- [TrinityCore](https://github.com/TrinityCore/TrinityCore/blob/3.3.5/THANKS)
- [SunwellCore](http://www.azerothcore.org/pages/sunwell.pl/)
- [AzerothCore](https://github.com/AzerothCore/azerothcore-wotlk/graphs/contributors)
- [AzerothCore Discord](https://discord.gg/gkt4y2x)
- [EMUDevs](https://youtube.com/user/EmuDevs)
- [AC-Web](http://ac-web.org/)
- [ModCraft.io](http://modcraft.io/)
- [OwnedCore](http://ownedcore.com/)
- [OregonCore](https://wiki.oregon-core.net/)
- [Wowhead.com](http://wowhead.com)
- [AoWoW](https://wotlk.evowow.com/)
### License ###
------------------------------------------------------------------------------------------------------------------
- This code and content is released under the [GNU AGPL v3](https://github.com/azerothcore/azerothcore-wotlk/blob/master/LICENSE-AGPL3).
*/


#include "Player.h"
#include "Config.h"
#include "Chat.h"
#include "World.h"
#include <sstream>

class CustomLogin : public PlayerScript
{

public:
    CustomLogin() : PlayerScript("CustomLogin") { }

    void OnLogin(Player* player, bool firstLogin)
    {
        if (firstLogin) {
            // If enabled..
            if (sConfigMgr->GetBoolDefault("CustomLogin.Enable", true))
            {
                // If enabled, give heirloom and other items
                if (sConfigMgr->GetBoolDefault("CustomLogin.BoA", true))
                {
                    // Define Equipment
                    uint32 shoulders = 0, chest = 0, trinket = 0, weapon = 0, weapon2 = 0, weapon3 = 0, shoulders2 = 0, chest2 = 0, trinket2 = 0;
                    const uint32 bag = 154696;		// Foror's Crate of Endless Resist Gear Storage (36 Slot)
                    const uint32 ring = 50255;		// Dread Pirate Ring (5% XP Boost)

                    // Outfit the character with bags and heirlooms that match their class
                    // NOTE: Some classes have more than one heirloom option per slot
                    switch (player->GetClass())
                    {

                    case CLASS_WARRIOR:
                        shoulders = 42949;
                        chest = 48685;
                        trinket = 42991;
                        weapon = 42943;
                        weapon2 = 44092;
                        weapon3 = 44093;
                        break;

                    case CLASS_PALADIN:
                        shoulders = 42949;
                        chest = 48685;
                        trinket = 42991;
                        weapon = 42945;
                        weapon2 = 44092;
                        break;

                    case CLASS_HUNTER:
                        shoulders = 42950;
                        chest = 48677;
                        trinket = 42991;
                        weapon = 42943;
                        weapon2 = 42946;
                        weapon3 = 44093;
                        break;

                    case CLASS_ROGUE:
                        shoulders = 42952;
                        chest = 48689;
                        trinket = 42991;
                        weapon = 42944;
                        weapon2 = 42944;
                        break;

                    case CLASS_PRIEST:
                        shoulders = 42985;
                        chest = 48691;
                        trinket = 42992;
                        weapon = 42947;
                        break;

                    case CLASS_DEATH_KNIGHT:
                        shoulders = 42949;
                        chest = 48685;
                        trinket = 42991;
                        weapon = 42945;
                        weapon2 = 44092;
                        weapon3 = 42943;
                        break;

                    case CLASS_SHAMAN:
                        shoulders = 42951;
                        chest = 48683;
                        trinket = 42992;
                        weapon = 42948;
                        shoulders2 = 42951;
                        chest2 = 48683;
                        weapon2 = 42947;
                        break;

                    case CLASS_MAGE:
                        shoulders = 42985;
                        chest = 48691;
                        trinket = 42992;
                        weapon = 42947;
                        break;

                    case CLASS_WARLOCK:
                        shoulders = 42985;
                        chest = 48691;
                        trinket = 42992;
                        weapon = 42947;
                        break;

                    case CLASS_DRUID:
                        shoulders = 42984;
                        chest = 48687;
                        trinket = 42992;
                        weapon = 42948;
                        shoulders2 = 42952;
                        chest2 = 48689;
                        trinket2 = 42991;
                        weapon2 = 48718;
                        break;

                    case CLASS_MONK:
                        shoulders = 42984;
                        chest = 48687;
                        trinket = 42992;
                        weapon = 42948;
                        shoulders2 = 42952;
                        chest2 = 48689;
                        trinket2 = 42991;
                        weapon2 = 48718;
                        break;

                    case CLASS_DEMON_HUNTER:
                        shoulders = 42984;
                        chest = 48687;
                        trinket = 42992;
                        weapon = 42948;
                        shoulders2 = 42952;
                        chest2 = 48689;
                        trinket2 = 42991;
                        weapon2 = 48718;
                        break;

                    default:
                        break;
                    }

                    // Hand out the heirlooms. I prefer only the ring and trinkets for new characters.
                    switch (player->GetClass())
                    {

                    case CLASS_DEATH_KNIGHT:
                        player->AddItem(trinket, 2);
                        player->AddItem(ring, 1);
                        player->AddItem(shoulders, 1);
                        player->AddItem(chest, 1);
                        player->AddItem(weapon, 1);
                        player->AddItem(weapon2, 1);
                        player->AddItem(weapon3, 1);
                        player->AddItem(bag, 2);
                        break;

                    case CLASS_PALADIN:
                        player->AddItem(trinket, 2);
                        player->AddItem(ring, 1);
                        player->AddItem(shoulders, 1);
                        player->AddItem(chest, 1);
                        player->AddItem(weapon, 1);
                       // player->AddItem(weapon2, 1);
                        player->AddItem(bag, 2);
                        break;

                    case CLASS_WARRIOR:
                        player->AddItem(trinket, 2);
                        player->AddItem(ring, 1);
                        player->AddItem(shoulders, 1);
                        player->AddItem(chest, 1);
                        player->AddItem(weapon, 1);
                        player->AddItem(weapon2, 1);
                        player->AddItem(weapon3, 1);
                        player->AddItem(bag, 2);
                        break;

                    case CLASS_HUNTER:
                        player->AddItem(trinket, 2);
                        player->AddItem(ring, 1);
                        player->AddItem(shoulders, 1);
                        player->AddItem(chest, 1);
                        player->AddItem(weapon, 1);
                        player->AddItem(weapon2, 1);
                        player->AddItem(weapon3, 1);
                        player->AddItem(bag, 2);
                        break;

                    case CLASS_ROGUE:
                        player->AddItem(trinket, 2);
                        player->AddItem(ring, 1);
                        player->AddItem(shoulders, 1);
                        player->AddItem(chest, 1);
                        player->AddItem(weapon, 1);
                        player->AddItem(weapon2, 1);
                        player->AddItem(bag, 2);
                        break;

                    case CLASS_DRUID:
                        player->AddItem(trinket, 2);
                        player->AddItem(trinket2, 2);
                        player->AddItem(ring, 1);
                        player->AddItem(shoulders, 1);
                        player->AddItem(chest, 1);
                        player->AddItem(weapon, 1);
                        player->AddItem(shoulders2, 1);
                        player->AddItem(chest2, 1);
                        player->AddItem(weapon2, 1);
                        player->AddItem(bag, 2);
                        break;

                    case CLASS_MONK:
                        player->AddItem(trinket, 2);
                        player->AddItem(trinket2, 2);
                        player->AddItem(ring, 1);
                        player->AddItem(shoulders, 1);
                        player->AddItem(chest, 1);
                        player->AddItem(weapon, 1);
                        player->AddItem(shoulders2, 1);
                        player->AddItem(chest2, 1);
                        player->AddItem(weapon2, 1);
                        player->AddItem(bag, 2);
                        break;

                    case CLASS_DEMON_HUNTER:
                        player->AddItem(trinket, 2);
                        player->AddItem(trinket2, 2);
                        player->AddItem(ring, 1);
                        player->AddItem(shoulders, 1);
                        player->AddItem(chest, 1);
                        player->AddItem(weapon, 1);
                        player->AddItem(shoulders2, 1);
                        player->AddItem(chest2, 1);
                        player->AddItem(weapon2, 1);
                        player->AddItem(bag, 2);
                        break;

                    case CLASS_SHAMAN:
                        player->AddItem(trinket, 2);
                        player->AddItem(ring, 1);
                        player->AddItem(shoulders, 1);
                        player->AddItem(chest, 1);
                        player->AddItem(weapon, 1);
                        player->AddItem(shoulders2, 1);
                        player->AddItem(chest2, 1);
                        player->AddItem(weapon2, 1);
                        player->AddItem(bag, 2);
                        break;

                    default:
                        player->AddItem(trinket, 2);
                        player->AddItem(ring, 1);
                        player->AddItem(shoulders, 1);
                        player->AddItem(chest, 1);
                        player->AddItem(weapon, 1);
                        player->AddItem(bag, 2);
                        break;
                    }

                    // Inform the player they have new items
                    std::ostringstream ss;
                    ss << "|cffFF0000[CustomLogin]:|cffFF8000 The outfitter has placed Heirloom gear in your backpack.";
                    ChatHandler(player->GetSession()).SendSysMessage(ss.str().c_str());
                }

                // If enabled, learn additional skills
                if (sConfigMgr->GetBoolDefault("CustomLogin.Skills", true))
                {
                    switch (player->GetClass())
                    {

                        /*
                            // Skill Reference
                            player->learnSpell(204);	// Defense
                            player->learnSpell(264);	// Bows
                            player->learnSpell(5011);	// Crossbow
                            player->learnSpell(674);	// Dual Wield
                            player->learnSpell(15590);	// Fists
                            player->learnSpell(266);	// Guns
                            player->learnSpell(196);	// Axes
                            player->learnSpell(198);	// Maces
                            player->learnSpell(201);	// Swords
                            player->learnSpell(750);	// Plate Mail
                            player->learnSpell(200);	// PoleArms
                            player->learnSpell(9116);	// Shields
                            player->learnSpell(197);	// 2H Axe
                            player->learnSpell(199);	// 2H Mace
                            player->learnSpell(202);	// 2H Sword
                            player->learnSpell(227);	// Staves
                            player->learnSpell(2567);	// Thrown
                        */

                    case CLASS_PALADIN:
                        player->LearnSpell(196, true);	// Axes
                        player->LearnSpell(750, true);	// Plate Mail
                        player->LearnSpell(200, true);	// PoleArms
                        player->LearnSpell(197, true);	// 2H Axe
                        player->LearnSpell(199, true);	// 2H Mace
                        break;

                    case CLASS_MONK:
                        player->LearnSpell(196, true);	// Axes
                        player->LearnSpell(198, true);	// Maces
                        player->LearnSpell(201, true);	// Swords
                        player->LearnSpell(227, true);	// Staves
                        player->LearnSpell(200, true);	// PoleArms
                        break;

                    case CLASS_DEMON_HUNTER:
                        player->LearnSpell(196, true);	// Axes
                        player->LearnSpell(201, true);	// Swords
                        break;

                    case CLASS_SHAMAN:
                        player->LearnSpell(15590, true);	// Fists
                        player->LearnSpell(8737, true);	// Mail
                        player->LearnSpell(196, true);	// Axes
                        player->LearnSpell(197, true);	// 2H Axe
                        player->LearnSpell(199, true);	// 2H Mace
                        break;

                    case CLASS_WARRIOR:
                        player->LearnSpell(264, true);	// Bows
                        player->LearnSpell(5011, true);	// Crossbow
                        player->LearnSpell(674, true);	// Dual Wield
                        player->LearnSpell(15590, true);	// Fists
                        player->LearnSpell(266, true);	// Guns
                        player->LearnSpell(750, true);	// Plate Mail
                        player->LearnSpell(200, true);	// PoleArms
                        player->LearnSpell(199, true);	// 2H Mace
                        player->LearnSpell(227, true);	// Staves
                        break;

                    case CLASS_HUNTER:
                        player->LearnSpell(674, true);	// Dual Wield
                        player->LearnSpell(15590, true);	// Fists
                        player->LearnSpell(266, true);	// Guns
                        player->LearnSpell(8737, true);	// Mail
                        player->LearnSpell(200, true);	// PoleArms
                        player->LearnSpell(227, true);	// Staves
                        player->LearnSpell(202, true);	// 2H Sword
                        break;

                    case CLASS_ROGUE:
                        player->LearnSpell(264, true);	// Bows
                        player->LearnSpell(5011, true);	// Crossbow
                        player->LearnSpell(15590, true);	// Fists
                        player->LearnSpell(266, true);	// Guns
                        player->LearnSpell(196, true);	// Axes
                        player->LearnSpell(198, true);	// Maces
                        player->LearnSpell(201, true);	// Swords
                        break;

                    case CLASS_DRUID:
                        player->LearnSpell(1180, true);	// Daggers
                        player->LearnSpell(15590, true);	// Fists
                        player->LearnSpell(198, true);	// Maces
                        player->LearnSpell(200, true);	// PoleArms
                        player->LearnSpell(227, true);	// Staves
                        player->LearnSpell(199, true);	// 2H Mace
                        break;

                    case CLASS_MAGE:
                        player->LearnSpell(201, true);	// Swords
                        break;

                    case CLASS_WARLOCK:
                        player->LearnSpell(201, true);	// Swords
                        break;

                    case CLASS_PRIEST:
                        player->LearnSpell(1180, true);	// Daggers
                        break;

                    case CLASS_DEATH_KNIGHT:
                        player->LearnSpell(198, true);	// Maces
                        player->LearnSpell(199, true);	// 2H Mace
                        break;

                    default:
                        break;
                    }

                    // Inform the player they have new skills
                    std::ostringstream ss;
                    ss << "|cffFF0000[CustomLogin]:|cffFF8000 You have been granted additional weapon skills.";
                    ChatHandler(player->GetSession()).SendSysMessage(ss.str().c_str());
                }

                // If enabled.. learn special skills abilities
                if (sConfigMgr->GetBoolDefault("CustomLogin.SpecialAbility", true))
                {
                    // Learn Specialized Skills
                    player->LearnSpell(1784, true);	// Stealth
                    player->LearnSpell(921, true);	// Pick Pocket
                    player->LearnSpell(1804, true);	// Lockpicking
                    player->LearnSpell(11305, true);	// Sprint (3)
                    player->LearnSpell(5384, true);	// Feign Death
                    // player->learnSpell(475);	// Remove Curse

                    // Add a few teleportation runes
                    player->AddItem(17031, 5);	// Rune of Teleportation

                    // Learn Teleports
                    switch (player->GetTeamId())
                    {

                    case TEAM_ALLIANCE:

                        // Alliance Teleports
                        player->LearnSpell(3565, true);	// Darnassus
                        player->LearnSpell(32271, true);	// Exodar
                        player->LearnSpell(3562, true);	// Ironforge
                        player->LearnSpell(33690, true);	// Shattrath
                        player->LearnSpell(3561, true);	// Stormwind
                        break;

                    case TEAM_HORDE:

                        // Horde Teleports
                        player->LearnSpell(3567, true);	// Orgrimmar
                        player->LearnSpell(35715, true);	// Shattrath
                        player->LearnSpell(32272, true);	// Silvermoon
                        player->LearnSpell(3566, true);	// Thunder Bluff
                        player->LearnSpell(3563, true);	// Undercity
                        break;

                    default:
                        break;
                    }

                    // Inform the player they have new skills
                    std::ostringstream ss;
                    ss << "|cffFF0000[CustomLogin]:|cffFF8000 Your spellbook has been scribed with special abilities.";
                    ChatHandler(player->GetSession()).SendSysMessage(ss.str().c_str());
                }

                // If enabled.. set exalted factions (AzerothCore config for rep not working as of 2017-08-25)
                if (sConfigMgr->GetBoolDefault("CustomLogin.Reputation", true))
                {
                    switch (player->GetTeamId())
                    {

                        // Alliance Capital Cities
                    case TEAM_ALLIANCE:
                        player->SetReputation(47, 3000);	// IronForge
                        player->SetReputation(72, 3000);	// Stormwind 
                        player->SetReputation(69, 3000);	// Darnassus
                        player->SetReputation(389, 3000);	// Gnomeregan
                        player->SetReputation(930, 3000);	// Exodar
                        break;

                        // Horde Capital Cities
                    case TEAM_HORDE:
                        player->SetReputation(68, 3000);	// Undercity
                        player->SetReputation(76, 3000);	// Orgrimmar
                        player->SetReputation(81, 3000);	// Thunder Bluff
                        player->SetReputation(530, 3000);	// DarkSpear
                        player->SetReputation(911, 3000);	// Silvermoon
                        break;

                    default:
                        break;
                    }

                    // Inform the player they have exalted reputations
                    std::ostringstream ss;
                    ss << "|cffFF0000[CustomLogin]:|cffFF8000 Your are now Exalted with your faction's capital cities " << player->GetName() << ".";
                    ChatHandler(player->GetSession()).SendSysMessage(ss.str().c_str());
                }
            }
        }
        // If enabled..
        if (sConfigMgr->GetBoolDefault("CustomLogin.Enable", true))
        {
                // Announce Module
            if (sConfigMgr->GetBoolDefault("CustomLogin.Announce", true))
            {
                ChatHandler(player->GetSession()).SendSysMessage("This server is running |cff4CFF00CustomLogin");
            }
        }
    }
};

void AddCustomLoginScripts()
{
    new CustomLogin();
}
