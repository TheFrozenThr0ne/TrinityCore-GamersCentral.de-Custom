#include <map>
#include "Log.h"
#include "Config.h"
#include "ScriptMgr.h"
#include "Unit.h"
#include "Player.h"
#include "Pet.h"
#include "Map.h"
#include "Group.h"
#include "InstanceScript.h"
#include "Chat.h"
#include <math.h>
#include <unordered_map>
#include "ObjectGuid.h"
#include <sstream>

bool SoloCraftEnable = 1;
bool SoloCraftAnnounceModule = 1;
bool SoloCraftDebuffEnable = 1;
bool SolocraftXPBalEnabled = 1;
bool SolocraftXPEnabled = 1;
bool SolocraftNoXPFlag = 0;
float SoloCraftSpellMult = 1.0;
float SoloCraftStatsMult = 100.0;
float SoloCraftXPMod = 1.0;
uint32 SolocraftLevelDiff = 1;
uint32 SolocraftDungeonLevel = 1;
std::unordered_map<uint8, uint32> classes;
std::unordered_map<uint32, uint32> dungeons;
std::unordered_map<uint32, float> diff_Multiplier;
std::unordered_map<uint32, float> diff_Multiplier_Heroics;

float D5 = 1.0;
float D10 = 1.0;
float D25 = 1.0;
float D40 = 1.0;
float D50 = 1.0;
float D649H10 = 1.0;
float D649H25 = 1.0;
float MYTHIC = 1.0;

class SolocraftConfig : public WorldScript
{
public:
    SolocraftConfig() : WorldScript("SolocraftConfig") {}

    void OnConfigLoad(bool /*reload*/)
    {
        // Load Configuration Settings
        SetInitialWorldSettings();
    }

    // Load Configuration Settings
    void SetInitialWorldSettings()
    {
        SoloCraftEnable = sConfigMgr->GetBoolDefault("Solocraft.Enable", 1);
        SoloCraftAnnounceModule = sConfigMgr->GetBoolDefault("Solocraft.Announce", 1);

        //Balancing
        SoloCraftDebuffEnable = sConfigMgr->GetBoolDefault("SoloCraft.Debuff.Enable", 1);
        SoloCraftSpellMult = sConfigMgr->GetFloatDefault("SoloCraft.Spellpower.Mult", 2.5);
        SoloCraftStatsMult = sConfigMgr->GetFloatDefault("SoloCraft.Stats.Mult", 100.0);
        classes =
        {
            {1, sConfigMgr->GetIntDefault("SoloCraft.WARRIOR", 100) },
            {2, sConfigMgr->GetIntDefault("SoloCraft.PALADIN", 100) },
            {3, sConfigMgr->GetIntDefault("SoloCraft.HUNTER", 100) },
            {4, sConfigMgr->GetIntDefault("SoloCraft.ROGUE", 100) },
            {5, sConfigMgr->GetIntDefault("SoloCraft.PRIEST", 100) },
            {6, sConfigMgr->GetIntDefault("SoloCraft.DEATH_KNIGHT", 100) },
            {7, sConfigMgr->GetIntDefault("SoloCraft.SHAMAN", 100) },
            {8, sConfigMgr->GetIntDefault("SoloCraft.MAGE", 100) },
            {9, sConfigMgr->GetIntDefault("SoloCraft.WARLOCK", 100) },
            {10, sConfigMgr->GetIntDefault("SoloCraft.MONK", 100) },
            {11, sConfigMgr->GetIntDefault("SoloCraft.DRUID", 100) },
            {12, sConfigMgr->GetIntDefault("SoloCraft.DEMON_HUNTER", 100) },
            {13, sConfigMgr->GetIntDefault("SoloCraft.EVOKER", 100) },
        };

        //XP Enabled
        SolocraftXPEnabled = sConfigMgr->GetBoolDefault("Solocraft.XP.Enabled", 1);

        //XP Balancing
        SolocraftXPBalEnabled = sConfigMgr->GetBoolDefault("Solocraft.XPBal.Enabled", 1);

        //Level Thresholds
        SolocraftLevelDiff = sConfigMgr->GetIntDefault("Solocraft.Max.Level.Diff", 10);

        //Catch All Dungeon Level Threshold
        SolocraftDungeonLevel = sConfigMgr->GetIntDefault("Solocraft.Dungeon.Level", 80);

        // Dungeon Base Level
        dungeons =
        {
            // Wow Classic
            {33, sConfigMgr->GetIntDefault("Solocraft.ShadowfangKeep.Level", 15) },
            {34, sConfigMgr->GetIntDefault("Solocraft.Stockades.Level", 22) },
            {36, sConfigMgr->GetIntDefault("Solocraft.Deadmines.Level", 18) },
            {43, sConfigMgr->GetIntDefault("Solocraft.WailingCaverns.Level", 17) },
            {47, sConfigMgr->GetIntDefault("Solocraft.RazorfenKraulInstance.Level", 30) },
            {48, sConfigMgr->GetIntDefault("Solocraft.Blackfathom.Level", 20) },
            {70, sConfigMgr->GetIntDefault("Solocraft.Uldaman.Level", 40) },
            {90, sConfigMgr->GetIntDefault("Solocraft.GnomeragonInstance.Level", 24) },
            {109, sConfigMgr->GetIntDefault("Solocraft.SunkenTemple.Level", 50) },
            {129, sConfigMgr->GetIntDefault("Solocraft.RazorfenDowns.Level", 40) },
            {189, sConfigMgr->GetIntDefault("Solocraft.MonasteryInstances.Level", 35) },                  // Scarlet Monastery
            {209, sConfigMgr->GetIntDefault("Solocraft.TanarisInstance.Level", 44) },                     // Zul'Farrak
            {229, sConfigMgr->GetIntDefault("Solocraft.BlackRockSpire.Level", 55) },
            {230, sConfigMgr->GetIntDefault("Solocraft.BlackrockDepths.Level", 50) },
            {249, sConfigMgr->GetIntDefault("Solocraft.OnyxiaLairInstance.Level", 60) },
            {289, sConfigMgr->GetIntDefault("Solocraft.SchoolofNecromancy.Level", 55) },                  // Scholomance
            {309, sConfigMgr->GetIntDefault("Solocraft.Zul'gurub.Level", 60) },
            {329, sConfigMgr->GetIntDefault("Solocraft.Stratholme.Level", 55) },
            {349, sConfigMgr->GetIntDefault("Solocraft.Mauradon.Level", 48) },
            {389, sConfigMgr->GetIntDefault("Solocraft.OrgrimmarInstance.Level", 15) },                   // Ragefire Chasm
            {409, sConfigMgr->GetIntDefault("Solocraft.MoltenCore.Level", 60) },
            {429, sConfigMgr->GetIntDefault("Solocraft.DireMaul.Level", 48) },
            {469, sConfigMgr->GetIntDefault("Solocraft.BlackwingLair.Level", 40) },
            {509, sConfigMgr->GetIntDefault("Solocraft.AhnQiraj.Level", 60) },                            // Ruins of Ahn'Qiraj
            {531, sConfigMgr->GetIntDefault("Solocraft.AhnQirajTemple.Level", 60) },

            // BC Instances
            {269, sConfigMgr->GetIntDefault("Solocraft.CavernsOfTime.Level", 68) },                       // The Black Morass
            {532, sConfigMgr->GetIntDefault("Solocraft.Karazahn.Level", 68) },
            {534, sConfigMgr->GetIntDefault("Solocraft.HyjalPast.Level", 70) },                           // The Battle for Mount Hyjal - Hyjal Summit
            {540, sConfigMgr->GetIntDefault("Solocraft.HellfireMilitary.Level", 68) },                    // The Shattered Halls
            {542, sConfigMgr->GetIntDefault("Solocraft.HellfireDemon.Level", 68) },                       // The Blood Furnace
            {543, sConfigMgr->GetIntDefault("Solocraft.HellfireRampart.Level", 68) },
            {544, sConfigMgr->GetIntDefault("Solocraft.HellfireRaid.Level", 68) },                        // Magtheridon's Lair
            {545, sConfigMgr->GetIntDefault("Solocraft.CoilfangPumping.Level", 68) },                     // The Steamvault
            {546, sConfigMgr->GetIntDefault("Solocraft.CoilfangMarsh.Level", 68) },                       // The Underbog
            {547, sConfigMgr->GetIntDefault("Solocraft.CoilfangDraenei.Level", 68) },                     // The Slavepens
            {548, sConfigMgr->GetIntDefault("Solocraft.CoilfangRaid.Level", 70) },                        // Serpentshrine Cavern
            {550, sConfigMgr->GetIntDefault("Solocraft.TempestKeepRaid.Level", 70) },                     // The Eye
            {552, sConfigMgr->GetIntDefault("Solocraft.TempestKeepArcane.Level", 68) },                   // The Arcatraz
            {553, sConfigMgr->GetIntDefault("Solocraft.TempestKeepAtrium.Level", 68) },                   // The Botanica
            {554, sConfigMgr->GetIntDefault("Solocraft.TempestKeepFactory.Level", 68) },                  // The Mechanar
            {555, sConfigMgr->GetIntDefault("Solocraft.AuchindounShadow.Level", 68) },                    // Shadow Labyrinth
            {556, sConfigMgr->GetIntDefault("Solocraft.AuchindounDemon.Level", 68) },                     // Sethekk Halls
            {557, sConfigMgr->GetIntDefault("Solocraft.AuchindounEthereal.Level", 68) },                  // Mana-Tombs
            {558, sConfigMgr->GetIntDefault("Solocraft.AuchindounDraenei.Level", 68) },                   // Auchenai Crypts
            {560, sConfigMgr->GetIntDefault("Solocraft.HillsbradPast.Level", 68) },                       // Old Hillsbrad Foothills
            {564, sConfigMgr->GetIntDefault("Solocraft.BlackTemple.Level", 70) },
            {565, sConfigMgr->GetIntDefault("Solocraft.GruulsLair.Level", 70) },
            {568, sConfigMgr->GetIntDefault("Solocraft.ZulAman.Level", 68) },
            {580, sConfigMgr->GetIntDefault("Solocraft.SunwellPlateau.Level", 70) },
            {585, sConfigMgr->GetIntDefault("Solocraft.Sunwell5ManFix.Level", 68) },                      // Magister's Terrace

            // WOTLK Instances
            {533, sConfigMgr->GetIntDefault("Solocraft.StratholmeRaid.Level", 78) },                      // Naxxramas
            {574, sConfigMgr->GetIntDefault("Solocraft.Valgarde70.Level", 78) },                          // Utgarde Keep
            {575, sConfigMgr->GetIntDefault("Solocraft.UtgardePinnacle.Level", 78) },
            {576, sConfigMgr->GetIntDefault("Solocraft.Nexus70.Level", 78) },                             // The Nexus
            {578, sConfigMgr->GetIntDefault("Solocraft.Nexus80.Level", 78) },                             // The Occulus
            {595, sConfigMgr->GetIntDefault("Solocraft.StratholmeCOT.Level", 78) },                       // The Culling of Stratholme
            {599, sConfigMgr->GetIntDefault("Solocraft.Ulduar70.Level", 78) },                            // Halls of Stone
            {600, sConfigMgr->GetIntDefault("Solocraft.DrakTheronKeep.Level", 78) },                      // Drak'Tharon Keep
            {601, sConfigMgr->GetIntDefault("Solocraft.Azjol_Uppercity.Level", 78) },                     // Azjol-Nerub
            {602, sConfigMgr->GetIntDefault("Solocraft.Ulduar80.Level", 78) },                            // Halls of Lighting
            {603, sConfigMgr->GetIntDefault("Solocraft.UlduarRaid.Level", 80) },                          // Ulduar
            {604, sConfigMgr->GetIntDefault("Solocraft.GunDrak.Level", 78) },
            {608, sConfigMgr->GetIntDefault("Solocraft.DalaranPrison.Level", 78) },                       // Violet Hold
            {615, sConfigMgr->GetIntDefault("Solocraft.ChamberOfAspectsBlack.Level", 80) },               // The Obsidian Sanctum
            {616, sConfigMgr->GetIntDefault("Solocraft.NexusRaid.Level", 80) },                           // The Eye of Eternity
            {619, sConfigMgr->GetIntDefault("Solocraft.Azjol_LowerCity.Level", 78) },                     // Ahn'kahet: The Old Kingdom
            {631, sConfigMgr->GetIntDefault("Solocraft.IcecrownCitadel.Level", 80) },                     // Icecrown Citadel
            {632, sConfigMgr->GetIntDefault("Solocraft.IcecrownCitadel5Man.Level", 78) },                 // The Forge of Souls
            {649, sConfigMgr->GetIntDefault("Solocraft.ArgentTournamentRaid.Level", 80) },                // Trial of the Crusader
            {650, sConfigMgr->GetIntDefault("Solocraft.ArgentTournamentDungeon.Level", 80) },             // Trial of the Champion
            {658, sConfigMgr->GetIntDefault("Solocraft.QuarryOfTears.Level", 78) },                       // Pit of Saron
            {668, sConfigMgr->GetIntDefault("Solocraft.HallsOfReflection.Level", 78) },                   // Halls of Reflection
            {724, sConfigMgr->GetIntDefault("Solocraft.ChamberOfAspectsRed.Level", 80) },                 // The Ruby Sanctum
        };

        // Dungeon Difficulty | Catch alls
        D5 = sConfigMgr->GetFloatDefault("Solocraft.Dungeon", 5.0);
        D10 = sConfigMgr->GetFloatDefault("Solocraft.Heroic", 10.0);
        D25 = sConfigMgr->GetFloatDefault("Solocraft.Raid25", 25.0);
        D40 = sConfigMgr->GetFloatDefault("Solocraft.Raid40", 40.0);
        D50 = sConfigMgr->GetFloatDefault("Solocraft.Mythic", 50.0);

        diff_Multiplier =
        {
            // WOW Classic Instances
            {33, sConfigMgr->GetFloatDefault("Solocraft.ShadowfangKeep", 5.0) },
            {34, sConfigMgr->GetFloatDefault("Solocraft.Stockades", 5.0) },
            {36, sConfigMgr->GetFloatDefault("Solocraft.Deadmines", 5.0) },
            {43, sConfigMgr->GetFloatDefault("Solocraft.WailingCaverns", 5.0) },
            {47, sConfigMgr->GetFloatDefault("Solocraft.RazorfenKraulInstance", 5.0) },
            {48, sConfigMgr->GetFloatDefault("Solocraft.Blackfathom", 5.0) },
            {70, sConfigMgr->GetFloatDefault("Solocraft.Uldaman", 5.0) },
            {90, sConfigMgr->GetFloatDefault("Solocraft.GnomeragonInstance", 5.0) },
            {109, sConfigMgr->GetFloatDefault("Solocraft.SunkenTemple", 5.0) },
            {129, sConfigMgr->GetFloatDefault("Solocraft.RazorfenDowns", 5.0) },
            {189, sConfigMgr->GetFloatDefault("Solocraft.MonasteryInstances", 5.0) },                     // Scarlet
            {209, sConfigMgr->GetFloatDefault("Solocraft.TanarisInstance", 5.0) },                        // Zul'Farrak
            {229, sConfigMgr->GetFloatDefault("Solocraft.BlackRockSpire", 10.0) },
            {230, sConfigMgr->GetFloatDefault("Solocraft.BlackrockDepths", 5.0) },
            {249, sConfigMgr->GetFloatDefault("Solocraft.OnyxiaLairInstance", 40.0) },
            {289, sConfigMgr->GetFloatDefault("Solocraft.SchoolofNecromancy", 5.0) },                     // Scholo
            {309, sConfigMgr->GetFloatDefault("Solocraft.Zul'gurub", 20.0) },
            {329, sConfigMgr->GetFloatDefault("Solocraft.Stratholme", 5.0) },
            {349, sConfigMgr->GetFloatDefault("Solocraft.Mauradon", 5.0) },
            {389, sConfigMgr->GetFloatDefault("Solocraft.OrgrimmarInstance", 5.0) },                      // Ragefire
            {409, sConfigMgr->GetFloatDefault("Solocraft.MoltenCore", 40.0) },
            {429, sConfigMgr->GetFloatDefault("Solocraft.DireMaul", 5.0) },
            {469, sConfigMgr->GetFloatDefault("Solocraft.BlackwingLair", 40.0) },
            {509, sConfigMgr->GetFloatDefault("Solocraft.AhnQiraj", 20.0) },
            {531, sConfigMgr->GetFloatDefault("Solocraft.AhnQirajTemple", 40.0) },

            // BC Instances
            {269, sConfigMgr->GetFloatDefault("Solocraft.CavernsOfTime", 5.0) },                          // Black Morass
            {532, sConfigMgr->GetFloatDefault("Solocraft.Karazahn", 10.0) },
            {534, sConfigMgr->GetFloatDefault("Solocraft.HyjalPast", 25.0) },                             // Mount Hyjal
            {540, sConfigMgr->GetFloatDefault("Solocraft.HellfireMilitary", 5.0) },                       // The Shattered Halls
            {542, sConfigMgr->GetFloatDefault("Solocraft.HellfireDemon", 5.0) },                          // The Blood Furnace
            {543, sConfigMgr->GetFloatDefault("Solocraft.HellfireRampart", 5.0) },
            {544, sConfigMgr->GetFloatDefault("Solocraft.HellfireRaid", 25.0) },                          // Magtheridon's Lair
            {545, sConfigMgr->GetFloatDefault("Solocraft.CoilfangPumping", 5.0) },                        // The Steamvault
            {546, sConfigMgr->GetFloatDefault("Solocraft.CoilfangMarsh", 5.0) },                          // The Underbog
            {547, sConfigMgr->GetFloatDefault("Solocraft.CoilfangDraenei", 5.0) },                        // The Slavepens
            {548, sConfigMgr->GetFloatDefault("Solocraft.CoilfangRaid", 25.0) },                          // Serpentshrine Cavern
            {550, sConfigMgr->GetFloatDefault("Solocraft.TempestKeepRaid", 25.0) },                       // The Eye
            {552, sConfigMgr->GetFloatDefault("Solocraft.TempestKeepArcane", 5.0) },                      // The Arcatraz
            {553, sConfigMgr->GetFloatDefault("Solocraft.TempestKeepAtrium", 5.0) },                      // The Botanica
            {554, sConfigMgr->GetFloatDefault("Solocraft.TempestKeepFactory", 5.0) },                     // The Mechanar
            {555, sConfigMgr->GetFloatDefault("Solocraft.AuchindounShadow", 5.0) },                       // Shadow Labyrinth
            {556, sConfigMgr->GetFloatDefault("Solocraft.AuchindounDemon", 5.0) },                        // Sethekk Halls
            {557, sConfigMgr->GetFloatDefault("Solocraft.AuchindounEthereal", 5.0) },                     // Mana-Tombs
            {558, sConfigMgr->GetFloatDefault("Solocraft.AuchindounDraenei", 5.0) },                      // Auchenai Crypts
            {560, sConfigMgr->GetFloatDefault("Solocraft.HillsbradPast", 5.0) },                          // Old Hillsbrad Foothills
            {564, sConfigMgr->GetFloatDefault("Solocraft.BlackTemple", 25.0) },
            {565, sConfigMgr->GetFloatDefault("Solocraft.GruulsLair", 25.0) },
            {568, sConfigMgr->GetFloatDefault("Solocraft.ZulAman", 5.0) },
            {580, sConfigMgr->GetFloatDefault("Solocraft.SunwellPlateau", 25.0) },
            {585, sConfigMgr->GetFloatDefault("Solocraft.Sunwell5ManFix", 5.0) },                         // Magister's Terrace

            // WOTLK Instances
            {533, sConfigMgr->GetFloatDefault("Solocraft.StratholmeRaid", 10.0) },                        //  Nax 10
            {574, sConfigMgr->GetFloatDefault("Solocraft.Valgarde70", 5.0) },                             // Utgarde Keep
            {575, sConfigMgr->GetFloatDefault("Solocraft.UtgardePinnacle", 5.0) },
            {576, sConfigMgr->GetFloatDefault("Solocraft.Nexus70", 5.0) },                                // The Nexus
            {578, sConfigMgr->GetFloatDefault("Solocraft.Nexus80", 5.0) },                                // The Occulus
            {595, sConfigMgr->GetFloatDefault("Solocraft.StratholmeCOT", 5.0) },                          // The Culling of Stratholme
            {599, sConfigMgr->GetFloatDefault("Solocraft.Ulduar70", 5.0) },                               // Halls of Stone
            {600, sConfigMgr->GetFloatDefault("Solocraft.DrakTheronKeep", 5.0) },                         // Drak'Tharon Keep
            {601, sConfigMgr->GetFloatDefault("Solocraft.Azjol_Uppercity", 5.0) },                        // Azjol-Nerub
            {602, sConfigMgr->GetFloatDefault("Solocraft.Ulduar80", 5.0) },                               // Halls of Lighting
            {603, sConfigMgr->GetFloatDefault("Solocraft.UlduarRaid", 10.0) },                            // Ulduar 10
            {604, sConfigMgr->GetFloatDefault("Solocraft.GunDrak", 5.0) },
            {608, sConfigMgr->GetFloatDefault("Solocraft.DalaranPrison", 5.0) },                          // Violet Hold
            {615, sConfigMgr->GetFloatDefault("Solocraft.ChamberOfAspectsBlack", 10.0) },                 // The Obsidian Sanctum 10
            {616, sConfigMgr->GetFloatDefault("Solocraft.NexusRaid", 10.0) },                             // The Eye of Eternity 10
            {619, sConfigMgr->GetFloatDefault("Solocraft.Azjol_LowerCity", 5.0) },                        // Ahn'kahet: The Old Kingdom
            {631, sConfigMgr->GetFloatDefault("Solocraft.IcecrownCitadel", 10.0) },                       // Icecrown Citadel 10
            {632, sConfigMgr->GetFloatDefault("Solocraft.IcecrownCitadel5Man", 5.0) },                    // The Forge of Souls
            {649, sConfigMgr->GetFloatDefault("Solocraft.ArgentTournamentRaid", 10.0) },                  // Trial of the Crusader 10
            {650, sConfigMgr->GetFloatDefault("Solocraft.ArgentTournamentDungeon", 5.0) },                // Trial of the Champion
            {658, sConfigMgr->GetFloatDefault("Solocraft.QuarryOfTears", 5.0) },                          // Pit of Saron
            {668, sConfigMgr->GetFloatDefault("Solocraft.HallsOfReflection", 5.0) },                      // Halls of Reflection
            {724, sConfigMgr->GetFloatDefault("Solocraft.ChamberOfAspectsRed", 10.0) },                   // The Ruby Sanctum 10
        };

        // Heroics
        diff_Multiplier_Heroics =
        {
            // BC Instances Heroics
            {269, sConfigMgr->GetFloatDefault("Solocraft.CavernsOfTimeH", 5.0) },                         // Black Morass H
            {540, sConfigMgr->GetFloatDefault("Solocraft.HellfireMilitaryH", 5.0) },                      // The Shattered Halls H
            {542, sConfigMgr->GetFloatDefault("Solocraft.HellfireDemonH", 5.0) },                         // The Blood Furnace H
            {543, sConfigMgr->GetFloatDefault("Solocraft.HellfireRampartH", 5.0) },                       // Heroic
            {545, sConfigMgr->GetFloatDefault("Solocraft.CoilfangPumpingH", 5.0) },                       // The Steamvault
            {546, sConfigMgr->GetFloatDefault("Solocraft.CoilfangMarshH", 5.0) },                         // The Underbog
            {547, sConfigMgr->GetFloatDefault("Solocraft.CoilfangDraeneiH", 5.0) },                       // The Slavepens H
            {552, sConfigMgr->GetFloatDefault("Solocraft.TempestKeepArcaneH", 5.0) },                     // The Arcatraz H
            {553, sConfigMgr->GetFloatDefault("Solocraft.TempestKeepAtriumH", 5.0) },                     // The Botanica H
            {554, sConfigMgr->GetFloatDefault("Solocraft.TempestKeepFactoryH", 5.0) },                    // The Mechanar H
            {555, sConfigMgr->GetFloatDefault("Solocraft.AuchindounShadowH", 5.0) },                      // Shadow Labyrinth H
            {556, sConfigMgr->GetFloatDefault("Solocraft.AuchindounDemonH", 5.0) },                       // Sethekk Halls H
            {557, sConfigMgr->GetFloatDefault("Solocraft.AuchindounEtherealH", 5.0) },                    // Mana-Tombs H
            {558, sConfigMgr->GetFloatDefault("Solocraft.AuchindounDraeneiH", 5.0) },                     // Auchenai Crypts H
            {560, sConfigMgr->GetFloatDefault("Solocraft.HillsbradPastH", 5.0) },                         // Old Hillsbrad Foothills H
            {568, sConfigMgr->GetFloatDefault("Solocraft.ZulAmanH", 5.0) },                               // Zul'Aman H
            {585, sConfigMgr->GetFloatDefault("Solocraft.Sunwell5ManFixH", 5.0) },                        // Magister's Terrace H

            // WOTLK Instances Heroics
            {533, sConfigMgr->GetFloatDefault("Solocraft.StratholmeRaidH", 25.0) },                       // Naxxramas 25
            {574, sConfigMgr->GetFloatDefault("Solocraft.Valgarde70H", 5.0) },                            // Utgarde Keep H
            {575, sConfigMgr->GetFloatDefault("Solocraft.UtgardePinnacleH", 5.0) },                       // Utgarde Pinnacle H
            {576, sConfigMgr->GetFloatDefault("Solocraft.Nexus70H", 5.0) },                               // The Nexus H
            {578, sConfigMgr->GetFloatDefault("Solocraft.Nexus80H", 5.0) },                               // The Occulus H
            {595, sConfigMgr->GetFloatDefault("Solocraft.StratholmeCOTH", 5.0) },                         // The Culling of Stratholme H
            {599, sConfigMgr->GetFloatDefault("Solocraft.Ulduar70H", 5.0) },                              // Halls of Stone H
            {600, sConfigMgr->GetFloatDefault("Solocraft.DrakTheronKeepH", 5.0) },                        // Drak'Tharon Keep H
            {601, sConfigMgr->GetFloatDefault("Solocraft.Azjol_UppercityH", 5.0) },                       // Azjol-Nerub H
            {602, sConfigMgr->GetFloatDefault("Solocraft.Ulduar80H", 5.0) },                              // Halls of Lighting H
            {603, sConfigMgr->GetFloatDefault("Solocraft.UlduarRaidH", 25.0) },                           // Ulduar 25
            {604, sConfigMgr->GetFloatDefault("Solocraft.GunDrakH", 5.0) },                               // Gundrak H
            {608, sConfigMgr->GetFloatDefault("Solocraft.DalaranPrisonH", 5.0) },                         // Violet Hold H
            {615, sConfigMgr->GetFloatDefault("Solocraft.ChamberOfAspectsBlackH", 25.0) },                // The Obsidian Sanctum 25
            {616, sConfigMgr->GetFloatDefault("Solocraft.NexusRaidH", 25.0) },                            // The Eye of Eternity 25
            {619, sConfigMgr->GetFloatDefault("Solocraft.Azjol_LowerCityH", 5.0) },                       // Ahn'kahet: The Old Kingdom H
            {631, sConfigMgr->GetFloatDefault("Solocraft.IcecrownCitadelH", 25.0) },                      // Icecrown Citadel 25
            {632, sConfigMgr->GetFloatDefault("Solocraft.IcecrownCitadel5ManH", 5.0) },                   // The Forge of Souls
            {649, sConfigMgr->GetFloatDefault("Solocraft.ArgentTournamentRaidH", 25.0) },                 // Trial of the Crusader 25
            {650, sConfigMgr->GetFloatDefault("Solocraft.ArgentTournamentDungeonH", 5.0) },               // Trial of the Champion H
            {658, sConfigMgr->GetFloatDefault("Solocraft.QuarryOfTearsH", 5.0) },                         // Pit of Saron H
            {668, sConfigMgr->GetFloatDefault("Solocraft.HallsOfReflectionH", 5.0) },                     // Halls of Reflection H
            {724, sConfigMgr->GetFloatDefault("Solocraft.ChamberOfAspectsRedH", 25.0) },                  // The Ruby Sanctum 25
        };

        //Unique Raids beyond the heroic and normal versions of themselves
        D649H10 = sConfigMgr->GetFloatDefault("Solocraft.ArgentTournamentRaidH10", 10.0);  //Trial of the Crusader 10 Heroic
        D649H25 = sConfigMgr->GetFloatDefault("Solocraft.ArgentTournamentRaidH25", 25.0);  //Trial of the Crusader 25 Heroic
        MYTHIC = sConfigMgr->GetFloatDefault("Solocraft.ArgentTournamentMythic", 50.0);  //Trial of the Crusader 10 Heroic
    }
};

class SolocraftAnnounce : public PlayerScript
{
private:
    std::map<ObjectGuid, bool> playerInInstanceMap;

public:
    SolocraftAnnounce() : PlayerScript("SolocraftAnnounce") {}

    void OnLogin(Player* player, bool /*firstLogin*/) override
    {
        if (SoloCraftEnable && SoloCraftAnnounceModule)
        {
            ChatHandler(player->GetSession()).SendSysMessage("This server is running |cff4CFF00SoloCraft");
        }
    }

    void OnLogout(Player* player) override
    {
        QueryResult result = CharacterDatabase.PQuery("SELECT `GUID` FROM `custom_solocraft_character_stats` WHERE `GUID`={}", player->GetGUID().GetCounter());
        if (result)
        {
            //Remove database entry as the player has logged out
            CharacterDatabase.PExecute("DELETE FROM `custom_solocraft_character_stats` WHERE `GUID`={}", player->GetGUID().GetCounter());
        }
        playerInInstanceMap.erase(player->GetGUID());
    }

    void OnMapChanged(Player* player) override
    {
        if (player->GetMap()->IsDungeon() || player->GetMap()->IsRaid())
        {
            playerInInstanceMap[player->GetGUID()] = true;
        }
        else
        {
            playerInInstanceMap[player->GetGUID()] = false;
        }
    }

    void OnGiveXP(Player* player, uint32& amount, Unit* /*victim*/) override
    {
        if (SolocraftXPBalEnabled && playerInInstanceMap[player->GetGUID()])
        {
            // Decrease Experience based on number of players and difficulty of instance (0 to 100%)
            amount = uint32(amount * SoloCraftXPMod);
        }
    }
};

class SolocraftPlayerInstanceHandler : public PlayerScript
{
public:
    SolocraftPlayerInstanceHandler() : PlayerScript("SolocraftPlayerInstanceHandler") {}

    void OnMapChanged(Player* player) override
    {
        if (sConfigMgr->GetBoolDefault("Solocraft.Enable", true))
        {
            Map* map = player->GetMap();
            float difficulty = CalculateDifficulty(map);
            uint32 dunLevel = CalculateDungeonLevel(map);
            uint32 numInGroup = GetNumInGroup(player);
            uint32 classBalance = GetClassBalance(player);
            ApplyBuffs(player, map, difficulty, dunLevel, numInGroup, classBalance);
        }
    }

    // Set the instance difficulty
    float CalculateDifficulty(Map* map)
    {
        if (map)
        {
            if (map->Is25ManRaid())
            {
                if (map->IsHeroic() && map->GetId() == 649)
                {
                    return D649H25;
                }
                else if (diff_Multiplier_Heroics.find(map->GetId()) == diff_Multiplier_Heroics.end())
                {
                    return D25;
                }
                else
                    return diff_Multiplier_Heroics[map->GetId()];
            }

            if (map->IsHeroic())
            {
                if (map->GetId() == 649)
                {
                    return D649H10;
                }
                else if (diff_Multiplier_Heroics.find(map->GetId()) == diff_Multiplier_Heroics.end())
                {
                    return D10;
                }
                else
                    return diff_Multiplier_Heroics[map->GetId()];
            }

            if (map->IsMythic())
            {
                if (map->GetId() == 649)
                {
                    return D649H10;
                }
                else if (diff_Multiplier_Heroics.find(map->GetId()) == diff_Multiplier_Heroics.end())
                {
                    return D25;
                }
                else
                    return diff_Multiplier_Heroics[map->GetId()];
            }

            if (diff_Multiplier.find(map->GetId()) == diff_Multiplier.end())
            {
                if (map->IsDungeon())
                {
                    return D5;
                }
                else if (map->IsRaid())
                {
                    return D40;
                }
            }
            else
                return diff_Multiplier[map->GetId()];
        }

        return 0;
    }

    // Set the Dungeon Level
    uint32 CalculateDungeonLevel(Map* map)
    {
        if (dungeons.find(map->GetId()) == dungeons.end())
        {
            return SolocraftDungeonLevel;
        }
        else
        {
            return dungeons[map->GetId()];
        }
    }

    // Get the group's size
    uint32 GetNumInGroup(Player* player)
    {
        uint32 numInGroup = 1;
        Group* group = player->GetGroup();

        if (group)
        {
            Group::MemberSlotList const& groupMembers = group->GetMemberSlots();
            numInGroup = groupMembers.size();
        }

        return numInGroup;
    }

    // Get the Player's class balance debuff
    uint32 GetClassBalance(Player* player)
    {
        if (!player) {
            return 0;  // Return a default or error value if the player is null
        }

        switch (player->GetClass()) {
        case CLASS_WARRIOR:
            // Logic for Warrior class balance
            return 100;

        case CLASS_DRUID:
            // Logic for Druid class balance
            return 90;

        case CLASS_MAGE:
            // Logic for Mage class balance
            return 80;

            // Add other classes here
        case CLASS_ROGUE:
            return 85;

        case CLASS_PRIEST:
            return 70;

            // Continue for all relevant classes

        default:
            return 50;  // Return a default balance for unknown or unsupported classes
        }
    }

    // Get the current group members GUIDS and return the total sum of the difficulty offset by all group members currently in the dungeon
    float GetGroupDifficulty(Player* player)
    {
        float GroupDifficulty = 0.0;
        Group* group = player->GetGroup();

        if (group)
        {
            Group::MemberSlotList const& groupMembers = group->GetMemberSlots();
            for (Group::member_citerator itr = groupMembers.begin(); itr != groupMembers.end(); ++itr)
            {
                if (itr->guid != player->GetGUID())
                {
                    QueryResult result = CharacterDatabase.PQuery("SELECT `GUID`, `Difficulty`, `GroupSize` FROM `custom_solocraft_character_stats` WHERE `GUID`={}", itr->guid.GetCounter());
                    if (result)
                    {
                        if ((*result)[1].GetFloat() > 0)
                        {
                            GroupDifficulty = GroupDifficulty + (*result)[1].GetFloat();
                        }
                    }
                }
            }
        }

        return GroupDifficulty;
    }

    // Resets buffers
    void ClearBuffs(Player* player)
    {
        //Database query to get offset from the last instance player exited
        QueryResult result = CharacterDatabase.PQuery("SELECT `GUID`, `Difficulty`, `GroupSize`, `SpellPower`, `Stats` FROM `custom_solocraft_character_stats` WHERE `GUID`={}", player->GetGUID().GetCounter());
        uint32 SpellPowerBonus = 0;

        if (result)
        {
            float difficulty = (*result)[1].GetFloat();
            SpellPowerBonus = (*result)[3].GetUInt32();
            float StatsMultPct = (*result)[4].GetFloat();
            SoloCraftXPMod = 1.0;

            for (uint32 i = STAT_STRENGTH; i < MAX_STATS; ++i)
            {
                player->ApplyStatPctModifier(UnitMods(UNIT_MOD_STAT_START + i), TOTAL_PCT, 100.f / (1.f + float(difficulty * StatsMultPct) / 100.f) - 100.f);
            }

            if (player->HasPlayerFlag(PLAYER_FLAGS_NO_XP_GAIN) && !SolocraftNoXPFlag)
            {
                player->RemovePlayerFlag(PLAYER_FLAGS_NO_XP_GAIN);
            }

            SolocraftNoXPFlag = 0;
            CharacterDatabase.PExecute("DELETE FROM custom_solocraft_character_stats WHERE GUID = {}", player->GetGUID().GetCounter());
        }

        if (player->GetPowerType() == POWER_MANA || player->GetClass() == CLASS_DRUID)
        {
            player->ApplySpellPowerBonus(SpellPowerBonus, false);
        }
    }

    // Apply the player buffs
    void ApplyBuffs(Player* player, Map* map, float difficulty, int dunLevel, int numInGroup, int classBalance)
    {
        // Check whether to buff the player or check to debuff back to normal
        if (difficulty != 0)
        {
            std::ostringstream ss;

            int SpellPowerBonus = 0;

            // Check for an existing No XP Gain flag - other mod compatibility
            if (player->HasPlayerFlag(PLAYER_FLAGS_NO_XP_GAIN))
            {
                SolocraftNoXPFlag = 1;
            }

            // If a player is too high level for dungeon don't buff but if in a group will count towards the group offset balancing.
            if (player->GetLevel() <= dunLevel + SolocraftLevelDiff)
            {

                // Get Current members total difficulty offset and if it exceeds the difficulty offset of the dungeon then debuff new group members coming in until all members leave and re-enter. This happens when a player already inside dungeon invite others to the group but the player already has the full difficulty offset.
                float GroupDifficulty = GetGroupDifficulty(player);

                // Check to either debuff or buff player entering dungeon.  Debuff must be enabled in Config
                if (GroupDifficulty >= difficulty && SoloCraftDebuffEnable == 1)
                {
                    // Current dungeon offset exceeded - Debuff player modified by ClassBalance Adjustment
                    difficulty = (-abs(difficulty)) + ((((float)classBalance / 100) * difficulty) / numInGroup);
                    // Float variables suck
                    difficulty = roundf(difficulty * 100) / 100;

                    // Disable player XP gain if debuff applied
                    if (!player->HasPlayerFlag(PLAYER_FLAGS_NO_XP_GAIN) && SolocraftXPBalEnabled)
                    {
                        player->SetPlayerFlag(PLAYER_FLAGS_NO_XP_GAIN);
                    }
                }
                else
                {
                    // Current Dungeon offset not exceeded - Buff player
                    // Group difficulty and ClassBalance Adjustment
                    difficulty = (((float)classBalance / 100) * difficulty) / numInGroup;
                    // Float variables suck - two decimal rounding
                    difficulty = roundf(difficulty * 100) / 100;

                    // Set XP Modifier
                    SoloCraftXPMod = (1.04 / difficulty) - 0.02;
                    SoloCraftXPMod = roundf(SoloCraftXPMod * 100) / 100;

                    // Check for negative XP modifier - Disable XP Gain
                    if (SoloCraftXPMod < 0)
                    {
                        SoloCraftXPMod = 0;
                        if (!player->HasPlayerFlag(PLAYER_FLAGS_NO_XP_GAIN) && SolocraftXPBalEnabled)
                        {
                            player->SetPlayerFlag(PLAYER_FLAGS_NO_XP_GAIN);
                        }
                    }

                    // Check XP modifier for over max limit and adjust
                    if (SoloCraftXPMod > 1)
                    {
                        SoloCraftXPMod = 1.0;
                    }
                }

                // Check Database for a current dungeon entry
                QueryResult result = CharacterDatabase.PQuery("SELECT `GUID`, `Difficulty`, `GroupSize`, `SpellPower`, `Stats` FROM `custom_solocraft_character_stats` WHERE `GUID`={}", player->GetGUID().GetCounter());

                // Modify Player Stats
                // STATS defined/enum in SharedDefines.h
                for (int32 i = STAT_STRENGTH; i < MAX_STATS; ++i)
                {
                    // Check for Dungeon to Dungeon Transfer and remove old buff
                    if (result)
                    {
                        player->ApplyStatPctModifier(UnitMods(UNIT_MOD_STAT_START + i), TOTAL_PCT, (*result)[1].GetFloat() * (*result)[4].GetFloat());
                    }
                    // Buff the player
                    // Unitmods enum UNIT_MOD_STAT_START defined in Unit.h line 391
                    player->ApplyStatPctModifier(UnitMods(UNIT_MOD_STAT_START + i), TOTAL_PCT, difficulty * SoloCraftStatsMult);

                }

                // Set player health
                // Defined in Unit.h line 1524
                player->SetFullHealth();

                if (player->IsPet())
                {
                    // Set Pet Health
                    player->CastSpell(player, 6962, true);

                }

                // Spellcaster Stat modify
                if (player->GetPowerType() == POWER_MANA || player->GetClass() == CLASS_DRUID)
                {
                    // Buff the player's mana
                    player->SetPower(POWER_MANA, player->GetMaxPower(POWER_MANA));

                    // Check for Dungeon to Dungeon Transfer and remove old Spellpower buff
                    if (result)
                    {
                        // remove spellpower bonus
                        player->ApplySpellPowerBonus((*result)[3].GetUInt32() * (*result)[4].GetFloat(), false);
                    }

                    // Buff Spellpower
                    // Debuffed characters do not get spellpower
                    if (difficulty > 0)
                    {
                        SpellPowerBonus = static_cast<int>((player->GetLevel() * SoloCraftSpellMult) * difficulty);
                        player->ApplySpellPowerBonus(SpellPowerBonus, true);
                    }
                }

                // XP Gain Disabled
                if (!SolocraftXPEnabled)
                {
                    SoloCraftXPMod = 0;
                    if (!player->HasPlayerFlag(PLAYER_FLAGS_NO_XP_GAIN))
                    {
                        player->SetPlayerFlag(PLAYER_FLAGS_NO_XP_GAIN);
                    }
                }

                // Announcements
                if (difficulty > 0)
                {
                    // Announce to player - Buff
                    if (!SolocraftXPEnabled)
                    {
                        ss << "|cffFF0000[SoloCraft] |cffFF8000" << player->GetName() << " entered %s  - Difficulty Offset: %0.2f. Spellpower Bonus: %i. Class Balance Weight: %i.  XP Gain: |cffFF0000Disabled";
                        ChatHandler(player->GetSession()).PSendSysMessage(ss.str().c_str(), map->GetMapName(), difficulty, SpellPowerBonus, classBalance);
                    }
                    else
                    {
                        if (!SolocraftXPBalEnabled)
                        {
                            ss << "|cffFF0000[SoloCraft] |cffFF8000" << player->GetName() << " entered %s  - Difficulty Offset: %0.2f. Spellpower Bonus: %i. Class Balance Weight: %i.  XP Balancing: |cffFF0000Disabled";
                            ChatHandler(player->GetSession()).PSendSysMessage(ss.str().c_str(), map->GetMapName(), difficulty, SpellPowerBonus, classBalance);
                        }
                        else
                        {
                            ss << "|cffFF0000[SoloCraft] |cffFF8000" << player->GetName() << " entered %s  - Difficulty Offset: %0.2f. Spellpower Bonus: %i. Class Balance Weight: %i.  XP Balancing: |cff4CFF00Enabled";
                            ChatHandler(player->GetSession()).PSendSysMessage(ss.str().c_str(), map->GetMapName(), difficulty, SpellPowerBonus, classBalance);
                        }
                    }
                }
                else
                {
                    // Announce to player - Debuff
                    if (!SolocraftXPBalEnabled && SolocraftXPEnabled)
                    {
                        ss << "|cffFF0000[SoloCraft] |cffFF8000" << player->GetName() << " entered %s  - |cffFF0000BE ADVISED - You have been debuffed by offset: %0.2f with a Class Balance Weight: %i. |cffFF8000 A group member already inside has the dungeon's full buff offset.  No Spellpower buff will be applied to spell casters.  ALL group members must exit the dungeon and re-enter to receive a balanced offset.";
                        ChatHandler(player->GetSession()).PSendSysMessage(ss.str().c_str(), map->GetMapName(), difficulty, classBalance);
                    }
                    else
                    {
                        ss << "|cffFF0000[SoloCraft] |cffFF8000" << player->GetName() << " entered %s  - |cffFF0000BE ADVISED - You have been debuffed by offset: %0.2f with a Class Balance Weight: %i and no XP will be awarded. |cffFF8000 A group member already inside has the dungeon's full buff offset.  No Spellpower buff will be applied to spell casters.  ALL group members must exit the dungeon and re-enter to receive a balanced offset.";
                        ChatHandler(player->GetSession()).PSendSysMessage(ss.str().c_str(), map->GetMapName(), difficulty, classBalance);
                    }
                }

                // Save Player Dungeon Offsets to Database
                CharacterDatabase.PExecute("REPLACE INTO custom_solocraft_character_stats (GUID, Difficulty, GroupSize, SpellPower, Stats) VALUES ({}, {}, {}, {}, {})", player->GetGUID().GetCounter(), difficulty, numInGroup, SpellPowerBonus, SoloCraftStatsMult);
            }
            else
            {
                // Announce to player - Over Max Level Threshold
                ss << "|cffFF0000[SoloCraft] |cffFF8000" << player->GetName() << " entered %s  - |cffFF0000You have not been buffed. |cffFF8000 Your level is higher than the max level (%i) threshold for this dungeon.";
                ChatHandler(player->GetSession()).PSendSysMessage(ss.str().c_str(), map->GetMapName(), dunLevel + SolocraftLevelDiff);
                ClearBuffs(player); // Check to revert player back to normal
            }
        }
        else
        {
            ClearBuffs(player); // Check to revert player back to normal - Moving this here fixed logout and login while in instance buff and debuff issues
        }
    }

private:
    std::map<uint32, float> _unitDifficulty;
};

void AddSolocraftScripts()
{
    new SolocraftConfig();
    new SolocraftAnnounce();
    new SolocraftPlayerInstanceHandler();
}
