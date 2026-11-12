/// @file
/// The 104 encounter zones, by zone number. A field zone's comment gives its
/// area name (gArea_NameRules) and the scenes and entrances that take it
/// (gScene_EncounterRules); a World Map zone's, its encounter region and the
/// battle environments that take it (gWorldMap_EncounterRules); a zone that
/// code names, its name (enum EncounterZoneId).

#include "field/encounter_zone.h"

// clang-format off
const struct EncounterZone gEncounterZones[] = {
    // ENCOUNTER_ZONE_NONE
    {0},
    // Sol Sanctum: SceneId_SolSanctumCorridor
    {
        .rate = 37,
        .level = 1,
        .formations = {7, 8, 9, 10},
        .weights = {4, 8, 8, 4},
    },
    // Sol Sanctum: SceneId_SolSanctumReliefs entrances 1 and 2
    {
        .rate = 35,
        .level = 1,
        .formations = {7, 8, 9, 10},
        .weights = {8, 4, 4, 8},
    },
    // Sol Sanctum: SceneId_SolSanctumReliefs entrances 3, 6 and 7
    {
        .rate = 35,
        .level = 1,
        .formations = {11, 12, 13, 14},
        .weights = {8, 8, 4, 4},
    },
    // Sol Sanctum: SceneId_SolSanctumMaze
    {
        .rate = 35,
        .level = 1,
        .formations = {11, 12, 13, 14},
        .weights = {4, 4, 8, 8},
    },
    // Sol Sanctum: SceneId_SolSanctumReliefs entrance 10
    {
        .rate = 35,
        .level = 2,
        .formations = {15, 16, 17, 18},
        .weights = {12, 8, 4, 4},
    },
    // Sol Sanctum: SceneId_SolSanctumLunaSolRooms
    {
        .rate = 35,
        .level = 2,
        .formations = {15, 16, 17, 18},
        .weights = {4, 4, 12, 8},
    },
    // World Map, encounter region 1: beach, shallows and grassland
    {
        .rate = 25,
        .level = 3,
        .formations = {20, 21, 22, 23, 24, 25, 26, 27},
        .weights = {4, 4, 4, 4, 2, 2, 2, 2},
    },
    // World Map, encounter region 1: forest
    {
        .rate = 9,
        .level = 3,
        .formations = {20, 21, 22, 23, 24, 25, 26, 27},
        .weights = {2, 2, 2, 2, 3, 7, 2, 3},
    },
    // Goma Cave: SceneId_GomaCavePond
    {
        .rate = 30,
        .level = 4,
        .formations = {31, 32, 33, 34, 35},
        .weights = {3, 3, 9, 3, 6},
    },
    // Goma Cave: SceneId_GomaCavePillar
    {
        .rate = 30,
        .level = 5,
        .formations = {31, 32, 33, 34, 35, 36, 37},
        .weights = {2, 2, 2, 2, 8, 5, 2},
    },
    // Goma Cave: SceneId_GomaCaveStream
    {
        .rate = 30,
        .level = 5,
        .formations = {31, 32, 33, 34, 35, 36, 37},
        .weights = {2, 2, 5, 2, 8, 8, 14},
    },
    // ENCOUNTER_ZONE_SCRIPTED_VALE_VAULT
    {
        .formations = {1, 2, 3, 4, 5, 29, 28},
        .weights = {10, 10, 10, 10, 10, 10, 1},
    },
    // World Map, encounter region 2: beach, shallows and grassland
    {
        .rate = 25,
        .level = 6,
        .formations = {50, 51, 52, 53, 54, 55, 56, 57},
        .weights = {4, 4, 4, 4, 4, 2, 2, 2},
    },
    // World Map, encounter region 2: forest
    {
        .rate = 9,
        .level = 6,
        .formations = {50, 51, 52, 53, 54, 55, 56, 57},
        .weights = {1, 1, 1, 1, 1, 2, 2, 2},
    },
    // Kolima Tunnel: SceneId_KolimaTunnel
    {
        .rate = 35,
        .level = 6,
        .formations = {50, 51, 52, 53, 54, 55, 56, 57},
        .weights = {1, 2, 2, 1, 1, 5, 5, 5},
    },
    // Kolima Forest: SceneId_KolimaForestEntrance
    {
        .rate = 40,
        .level = 6,
        .formations = {59, 60, 61, 62, 63, 64, 65, 66},
        .weights = {10, 5, 1, 1, 1, 1},
    },
    // Kolima Forest: SceneId_KolimaForestGlade
    {
        .rate = 40,
        .level = 7,
        .formations = {59, 60, 61, 62, 63, 64, 65, 66},
        .weights = {2, 6, 2, 2, 8, 6, 2, 2},
    },
    // Kolima Forest: SceneId_KolimaForestCliff
    {
        .rate = 40,
        .level = 7,
        .formations = {59, 60, 61, 62, 63, 64, 65, 66},
        .weights = {1, 1, 2, 2, 3, 3, 8, 12},
    },
    // Kolima Forest: SceneId_KolimaForestTree
    {
        .rate = 40,
        .level = 7,
        .formations = {59, 60, 61, 62, 63, 64, 65, 66},
        .weights = {1, 1, 2, 2, 3, 3, 5, 12},
    },
    // Tret Tree: SceneId_TretTreeBranch entrances 1 to 3
    {
        .rate = 45,
        .level = 7,
        .formations = {68, 69, 70, 71, 72, 73, 74, 75},
        .weights = {8, 5, 6, 2, 1, 2, 1, 1},
    },
    // Tret Tree: SceneId_TreeRooms entrances 1, 11, 16 and 21 to 29
    {
        .rate = 45,
        .level = 7,
        .formations = {68, 69, 70, 71, 72, 73, 74, 75},
        .weights = {6, 6, 6, 3, 2, 1},
    },
    // Tret Tree: SceneId_TreeRooms entrances 2 to 5, 12, 13, 31 and 32
    {
        .rate = 45,
        .level = 8,
        .formations = {68, 69, 70, 71, 72, 73, 74, 75},
        .weights = {3, 5, 5, 5, 3, 2, 1, 1},
    },
    // Tret Tree: SceneId_TretTreeBranch, its other entrances
    {
        .rate = 45,
        .level = 8,
        .formations = {68, 69, 70, 71, 72, 73, 74, 75},
        .weights = {2, 1, 5, 8, 1, 8, 1, 5},
    },
    // Tret Tree: SceneId_TreeRooms entrances 6 to 9, 14 and 51 to 59
    {
        .rate = 45,
        .level = 8,
        .formations = {68, 69, 70, 71, 72, 73, 74, 75},
        .weights = {1, 1, 1, 1, 2, 4, 12, 8},
    },
    // Bilibin Cave: SceneId_BilibinCaveRapier, its other entrances
    {
        .rate = 30,
        .level = 8,
        .formations = {77, 78, 79, 80, 81, 82, 83, 84},
        .weights = {6, 6, 4, 4, 2, 2, 1, 1},
    },
    // Bilibin Cave: SceneId_BilibinCaveIce, its other entrances
    {
        .rate = 30,
        .level = 8,
        .formations = {77, 78, 79, 80, 81, 82, 83, 84},
        .weights = {2, 2, 2, 4, 4, 5, 7, 7},
    },
    // Bilibin Cave: SceneId_BilibinCaveRapier entrances 5 and 6
    {
        .rate = 30,
        .level = 9,
        .formations = {77, 78, 79, 80, 81, 82, 83, 84},
        .weights = {1, 1, 1, 1, 1, 2, 2, 2},
    },
    // Bilibin Cave: SceneId_BilibinCaveIce entrance 5
    {
        .rate = 30,
        .level = 9,
        .formations = {77, 78, 79, 80, 81, 82, 83, 84},
        .weights = {1, 1, 1, 1, 1, 2, 2, 2},
    },
    // World Map, WORLD_MAP_ENCOUNTER_REGION_SNOWFIELD: snowfield
    {
        .rate = 25,
        .level = 9,
        .formations = {86, 87, 88, 89, 90},
        .weights = {3, 3, 5, 5, 5},
    },
    // World Map, WORLD_MAP_ENCOUNTER_REGION_SNOWFIELD: snow forest
    {
        .rate = 9,
        .level = 9,
        .formations = {86, 87, 88, 89, 90},
        .weights = {2, 2, 4, 4, 4},
    },
    // Mercury Lighthouse: SceneId_MercuryLighthouseEntrance entrances 7 to 12 and 16 to 20, SceneId_MercuryLighthouseLower entrances 1 to 3
    {
        .rate = 40,
        .level = 9,
        .formations = {92, 93, 94, 95, 96, 97, 98, 99},
        .weights = {4, 4, 4, 2, 2, 2, 2, 2},
    },
    // Mercury Lighthouse: SceneId_MercuryLighthouseLower entrances 5 to 8 and 11, SceneId_MercuryLighthouseMiddle entrances 3 and 4
    {
        .rate = 40,
        .level = 9,
        .formations = {92, 93, 94, 95, 96, 97, 98, 99},
        .weights = {2, 2, 2, 4, 4, 2, 2, 2},
    },
    // Mercury Lighthouse: SceneId_MercuryLighthouseUpper entrances 1 to 3, SceneId_MercuryLighthouseMiddle entrances 5, 6 and 10
    {
        .rate = 40,
        .level = 10,
        .formations = {97, 98, 99, 100, 101, 102, 103, 104},
        .weights = {2, 2, 2, 4, 4, 4, 2, 2},
    },
    // Mercury Lighthouse: SceneId_MercuryLighthouseExterior entrances 1 and 2, SceneId_MercuryLighthouseUpper entrances 12 and 13
    {
        .rate = 40,
        .level = 11,
        .formations = {97, 98, 99, 100, 101, 102, 103, 104},
        .weights = {1, 1, 1, 2, 2, 2, 4, 4},
    },
    // Mercury Lighthouse: SceneId_MercuryLighthouseExterior entrances 3 and 4
    {
        .rate = 40,
        .level = 11,
        .formations = {97, 98, 99, 100, 101, 102, 103, 104},
        .weights = {1, 1, 1, 2, 2, 2, 4, 4},
    },
    // ENCOUNTER_ZONE_SCRIPTED_TRET_MERCURY
    {
        .formations = {106, 107, 108},
        .weights = {10, 10, 10},
    },
    // World Map, encounter region 31: grassland
    {
        .rate = 20,
        .level = 11,
        .formations = {120, 121, 122, 123},
        .weights = {3, 3, 3, 3},
    },
    // World Map, encounter region 31: forest
    {
        .rate = 9,
        .level = 11,
        .formations = {120, 121, 122, 123},
        .weights = {1, 1, 1, 1},
    },
    // Fuchin Falls Cave: SceneId_FuchinFallsCaveEntrance, SceneId_FuchinFallsCaveZephyr
    {
        .rate = 38,
        .level = 11,
        .formations = {120, 121, 122, 123, 124, 125, 126, 127},
        .weights = {4, 4, 4, 8, 6, 4, 2, 1},
    },
    // Fuchin Falls Cave: SceneId_FuchinFallsCaveDragonsEye
    {
        .rate = 38,
        .level = 11,
        .formations = {120, 121, 122, 123, 124, 125, 126, 127},
        .weights = {1, 2, 2, 3, 6, 6, 3, 2},
    },
    // Fuchin Falls Cave: SceneId_FuchinFallsCaveDragonStatue
    {
        .rate = 38,
        .level = 11,
        .formations = {120, 121, 122, 123, 124, 125, 126, 127},
        .weights = {1, 1, 2, 3, 4, 4, 6, 8},
    },
    // Mogall Forest: SceneId_MogallForestSign
    {
        .rate = 42,
        .level = 11,
        .formations = {129, 130, 131, 132, 133, 134, 135, 136},
        .weights = {6, 6, 6, 3, 2, 1, 1, 1},
    },
    // Mogall Forest: SceneId_MogallForestQuartz
    {
        .rate = 42,
        .level = 12,
        .formations = {129, 130, 131, 132, 133, 134, 135, 136},
        .weights = {1, 1, 3, 6, 6, 3, 1, 1},
    },
    // Mogall Forest: SceneId_MogallForestKillerApe
    {
        .rate = 42,
        .level = 12,
        .formations = {129, 130, 131, 132, 133, 134, 135, 136},
        .weights = {1, 1, 2, 2, 3, 4, 5, 5},
    },
    // World Map, encounter region 3: desert, beach, shallows and grassland
    {
        .rate = 25,
        .level = 12,
        .formations = {141, 142, 143, 144, 145, 146},
        .weights = {3, 3, 1, 3, 1, 1},
    },
    // World Map, encounter region 3: forest
    {
        .rate = 9,
        .level = 12,
        .formations = {141, 142, 143, 144, 145, 146},
        .weights = {1, 1, 2, 1, 2, 2},
    },
    // Altin Peak: SceneId_AltinPeakRailTunnel, SceneId_AltinPeakSecondWaterBeast, SceneId_AltinPeakSouthEntrance
    {
        .rate = 45,
        .level = 12,
        .formations = {148, 149, 150, 151, 152, 153, 154, 155},
        .weights = {6, 6, 4, 2, 1, 1, 1, 1},
    },
    // Altin Peak: SceneId_AltinPeakThirdWaterBeast, SceneId_AltinPeakPool
    {
        .rate = 45,
        .level = 13,
        .formations = {148, 149, 150, 151, 152, 153, 154, 155},
        .weights = {1, 1, 2, 4, 3, 4, 2, 2},
    },
    // Altin Peak: SceneId_AltinPeakSpritz, SceneId_AltinPeakJunction, SceneId_AltinPeakFallingRocks entrances 1 and 2, SceneId_AltinPeakWestExit
    {
        .rate = 42,
        .level = 13,
        .formations = {148, 149, 150, 151, 152, 153, 154, 155},
        .weights = {1, 1, 2, 3, 4, 4, 8, 8},
    },
    // Lamakan Desert: SceneId_LamakanDesertEast
    {
        .rate = 40,
        .level = 13,
        .formations = {160, 161, 162, 163, 164, 165, 166, 167},
        .weights = {5, 5, 4, 3, 3, 2, 1, 1},
    },
    // Lamakan Desert: SceneId_LamakanDesertSmog
    {
        .rate = 40,
        .level = 14,
        .formations = {160, 161, 162, 163, 164, 165, 166, 167},
        .weights = {1, 2, 3, 6, 6, 4, 2, 1},
    },
    // Lamakan Desert: SceneId_LamakanDesertManticore
    {
        .rate = 40,
        .level = 14,
        .formations = {160, 161, 162, 163, 164, 165, 166, 167},
        .weights = {1, 1, 1, 2, 3, 4, 4, 5},
    },
    // ENCOUNTER_ZONE_SCRIPTED_MOGALL_ALTIN_LAMAKAN
    {
        .formations = {138, 139, 157, 158, 169, 170},
        .weights = {10, 10, 10, 10, 10, 10},
    },
    // World Map, encounter region 4: desert, beach, shallows and grassland
    {
        .rate = 25,
        .level = 15,
        .formations = {180, 181, 182, 183, 184, 185, 186, 187},
        .weights = {4, 4, 4, 4, 4, 2, 2, 2},
    },
    // World Map, encounter region 4: forest
    {
        .rate = 9,
        .level = 15,
        .formations = {180, 181, 182, 183, 184, 185, 186, 187},
        .weights = {2, 2, 2, 2, 3, 4, 4, 4},
    },
    // Vale Cave: SceneId_ValeCaveKite entrances 1 to 4
    {
        .rate = 40,
        .level = 15,
        .formations = {189, 190, 191, 192, 193, 194, 195, 196},
        .weights = {6, 6, 6, 4, 4, 1, 1, 1},
    },
    // Vale Cave: SceneId_ValeCaveKite entrances 8 to 10
    {
        .rate = 40,
        .level = 15,
        .formations = {189, 190, 191, 192, 193, 194, 195, 196},
        .weights = {4, 4, 4, 6, 6, 2, 2, 2},
    },
    // Vale Cave: SceneId_ValeCaveLogs entrances 1 to 3
    {
        .rate = 40,
        .level = 15,
        .formations = {189, 190, 191, 192, 193, 194, 195, 196},
        .weights = {2, 2, 2, 3, 3, 5, 5, 5},
    },
    // Vale Cave: SceneId_ValeCaveLogs entrances 4 and 5
    {
        .rate = 40,
        .level = 15,
        .formations = {189, 190, 191, 192, 193, 194, 195, 196},
        .weights = {1, 1, 1, 2, 2, 5, 5, 5},
    },
    // Vault Cave: SceneId_VaultCavePassage
    {
        .rate = 40,
        .level = 16,
        .formations = {201, 202, 203, 204, 205, 206, 207, 208},
        .weights = {8, 8, 8, 4, 4, 4, 2, 2},
    },
    // Vault Cave: SceneId_VaultCaveVambrace, SceneId_VaultCaveMouth
    {
        .rate = 40,
        .level = 16,
        .formations = {201, 202, 203, 204, 205, 206, 207, 208},
        .weights = {2, 2, 2, 4, 4, 4, 8, 8},
    },
    // ENCOUNTER_ZONE_TOLBI_SHIP
    {
        .level = 17,
        .formations = {210, 211, 212, 213},
        .weights = {10, 10, 10, 10},
    },
    // ENCOUNTER_ZONE_CROSSBONE_ISLE_FIRST_GUARDS
    {
        .level = 17,
        .formations = {291, 292, 293, 294},
        .weights = {10, 10, 10, 10},
    },
    // World Map, encounter region 5: desert, beach, shallows and grassland
    {
        .rate = 25,
        .level = 18,
        .formations = {220, 221, 222, 223, 224, 225, 226, 227},
        .weights = {6, 6, 6, 6, 3, 3, 3, 3},
    },
    // World Map, encounter region 5: forest
    {
        .rate = 9,
        .level = 18,
        .formations = {220, 221, 222, 223, 224, 225, 226, 227},
        .weights = {3, 3, 3, 3, 6, 6, 6, 6},
    },
    // Altmiller Cave: SceneId_AltmillerCaveEntrance
    {
        .rate = 40,
        .level = 17,
        .formations = {229, 230, 231, 232, 233, 234, 235, 236},
        .weights = {6, 6, 4, 4, 4, 2, 1, 1},
    },
    // Altmiller Cave: SceneId_AltmillerCaveBabi
    {
        .rate = 40,
        .level = 17,
        .formations = {229, 230, 231, 232, 233, 234, 235, 236},
        .weights = {4, 4, 6, 6, 4, 4, 2, 1},
    },
    // Altmiller Cave: SceneId_AltmillerCaveVialChest
    {
        .rate = 40,
        .level = 18,
        .formations = {229, 230, 231, 232, 233, 234, 235, 236},
        .weights = {3, 3, 4, 6, 6, 4, 2, 1},
    },
    // Altmiller Cave: SceneId_AltmillerCaveSquall
    {
        .rate = 40,
        .level = 18,
        .formations = {229, 230, 231, 232, 233, 234, 235, 236},
        .weights = {2, 2, 4, 4, 6, 5, 5, 3},
    },
    // Altmiller Cave: SceneId_AltmillerCaveCookieChest
    {
        .rate = 40,
        .level = 18,
        .formations = {229, 230, 231, 232, 233, 234, 235, 236},
        .weights = {2, 2, 3, 3, 4, 6, 6, 4},
    },
    // Altmiller Cave: SceneId_AltmillerCaveDraught entrances 50 and 51
    {
        .rate = 40,
        .level = 18,
        .formations = {229, 230, 231, 232, 233, 234, 235, 236},
        .weights = {1, 1, 1, 2, 2, 4, 6, 6},
    },
    // ENCOUNTER_ZONE_COLOSSO
    {
        .level = 19,
        .formations = {238, 239, 240, 241, 242, 243},
        .weights = {10, 10, 10, 10, 10, 10},
    },
    // Gondowan Cave: SceneId_GondowanCave, its other entrances
    {
        .rate = 30,
        .level = 19,
        .formations = {250, 251, 252, 253, 254, 255, 256, 257},
        .weights = {3, 3, 4, 4, 5, 5, 6, 6},
    },
    // Gondowan Cave: SceneId_GondowanCaveRiver, its other entrances
    {
        .rate = 30,
        .level = 20,
        .formations = {250, 251, 252, 253, 254, 255, 256, 257},
        .weights = {3, 3, 4, 4, 5, 5, 6, 6},
    },
    // Gondowan Cave: SceneId_GondowanCaveRiver entrances 1 and 2
    {
        .rate = 30,
        .level = 20,
        .formations = {250, 251, 252, 253, 254, 255, 256, 257},
        .weights = {3, 3, 4, 4, 5, 5, 6, 6},
    },
    // Gondowan Cave: SceneId_GondowanCave entrances 1 to 3
    {
        .rate = 30,
        .level = 20,
        .formations = {250, 251, 252, 253, 254, 255, 256, 257},
        .weights = {3, 3, 4, 4, 5, 5, 6, 6},
    },
    // Lunpa Fortress: SceneId_LunpaFortressCave entrances 1 and 2
    {
        .rate = 35,
        .level = 21,
        .formations = {259, 260, 261, 262},
        .weights = {2, 2, 2, 2},
    },
    // Lunpa Fortress: SceneId_LunpaFortressCave, its other entrances
    {
        .rate = 35,
        .level = 21,
        .formations = {259, 260, 261, 262},
        .weights = {2, 2, 2, 2},
    },
    // Lunpa Fortress: SceneId_LunpaFortressToadonpa entrances 1 and 2
    {
        .rate = 35,
        .level = 21,
        .formations = {259, 260, 261, 262},
        .weights = {2, 2, 2, 2},
    },
    // World Map, encounter region 6: desert, beach, shallows and grassland
    {
        .rate = 25,
        .level = 22,
        .formations = {268, 269, 270, 271, 272, 273, 274, 275},
        .weights = {4, 4, 3, 3, 3, 3, 3, 3},
    },
    // World Map, encounter region 6: forest
    {
        .rate = 9,
        .level = 22,
        .formations = {268, 269, 270, 271, 272, 273, 274, 275},
        .weights = {2, 2, 3, 3, 3, 4, 4, 4},
    },
    // Suhalla Desert: SceneId_SuhallaDesertNorth
    {
        .rate = 40,
        .level = 22,
        .formations = {277, 278, 279, 280, 281, 282, 284, 285},
        .weights = {4, 4, 4, 5, 3, 4, 2, 2},
    },
    // Suhalla Desert: SceneId_SuhallaDesertSouth
    {
        .rate = 40,
        .level = 22,
        .formations = {277, 278, 279, 280, 281, 282, 284, 285},
        .weights = {2, 2, 2, 4, 3, 4, 4, 4},
    },
    // ENCOUNTER_ZONE_CROSSBONE_ISLE_LAST_GUARDS
    {
        .level = 17,
        .formations = {296, 297, 298, 300, 301, 302},
        .weights = {10, 10, 10, 10, 10, 10},
    },
    // Suhalla Gate: SceneId_SuhallaGateDew entrances 1 and 2
    {
        .rate = 30,
        .level = 23,
        .formations = {310, 311, 312, 313, 314, 315, 316, 317},
        .weights = {6, 6, 6, 3, 3, 2, 2, 2},
    },
    // Suhalla Gate: SceneId_SuhallaGateDew, its other entrances
    {
        .rate = 30,
        .level = 24,
        .formations = {310, 311, 312, 313, 314, 315, 316, 317},
        .weights = {2, 3, 3, 6, 6, 3, 3, 3},
    },
    // Gateway Cave: SceneId_GatewayCave entrances 1 and 2
    {
        .rate = 30,
        .level = 24,
        .formations = {310, 311, 312, 313, 314, 315, 316, 317},
        .weights = {1, 1, 2, 2, 2, 3, 3, 3},
    },
    // Gateway Cave: SceneId_GatewayCave, its other entrances
    {
        .rate = 30,
        .level = 24,
        .formations = {310, 311, 312, 313, 314, 315, 316, 317},
        .weights = {2, 2, 3, 3, 3, 6, 6, 6},
    },
    // Venus Lighthouse: SceneId_VenusLighthouseGroundFloor, its other entrances
    {
        .rate = 40,
        .level = 25,
        .formations = {319, 320, 321, 322, 323},
        .weights = {4, 4, 4, 4, 4},
    },
    // Venus Lighthouse: SceneId_VenusLighthouseGroundFloor entrances 20 to 26 and 30 to 32
    {
        .rate = 40,
        .level = 25,
        .formations = {319, 320, 321, 322, 323},
        .weights = {4, 4, 4, 4, 4},
    },
    // Babi Lighthouse Entry and Babi Lighthouse: SceneId_BabiLighthouseEntry, SceneId_BabiLighthouseLuff, SceneId_BabiLighthouseStoneDoor, SceneId_TunnelRuinsLadyStatues entrances 1 to 3
    {
        .rate = 35,
        .level = 25,
        .formations = {328, 329, 330, 331, 332, 333, 334, 335},
        .weights = {6, 6, 6, 4, 4, 4, 2, 2},
    },
    // Tunnel Ruins: SceneId_TunnelRuinsGreatStatue entrances 1, 2, 5 to 7 and 14 to 18, SceneId_TunnelRuinsLadyStatues entrances 8 to 11
    {
        .rate = 35,
        .level = 25,
        .formations = {328, 329, 330, 331, 332, 333, 334, 335},
        .weights = {3, 3, 4, 6, 6, 4, 3, 3},
    },
    // Tunnel Ruins: SceneId_TunnelRuinsLadyStatues entrances 12 and 15 to 17
    {
        .rate = 35,
        .level = 26,
        .formations = {328, 329, 330, 331, 332, 333, 334, 335},
        .weights = {3, 3, 3, 4, 4, 4, 6, 6},
    },
    // Venus Lighthouse: SceneId_VenusLighthouseFirstFloor, its other entrances
    {
        .rate = 34,
        .level = 27,
        .formations = {337, 338, 339, 340, 341, 342, 343, 344},
        .weights = {4, 4, 3, 3, 3, 3, 2, 2},
    },
    // Venus Lighthouse: SceneId_VenusLighthouseFirstFloor entrances 19 and 20, SceneId_VenusLighthouseSecondFloor
    {
        .rate = 32,
        .level = 28,
        .formations = {337, 338, 339, 340, 341, 342, 343, 344},
        .weights = {3, 3, 4, 4, 3, 3, 2, 2},
    },
    // Venus Lighthouse: SceneId_VenusLighthouseThirdFloor
    {
        .rate = 30,
        .level = 29,
        .formations = {337, 338, 339, 340, 341, 342, 343, 344},
        .weights = {2, 2, 3, 3, 4, 4, 3, 3},
    },
    // Venus Lighthouse: SceneId_VenusLighthouseFourthFloor
    {
        .rate = 30,
        .level = 30,
        .formations = {337, 338, 339, 340, 341, 342, 343, 344},
        .weights = {2, 2, 2, 3, 3, 3, 4, 4},
    },
    // ENCOUNTER_ZONE_SCRIPTED
    {
        .formations = {347, 346, 264, 265, 266, 287, 288, 289},
        .weights = {10, 10, 30, 10, 10, 80, 10, 10},
    },
    // ENCOUNTER_ZONE_MIMIC
    {
        .formations = {350, 351, 352, 353, 354, 355, 356, 357},
        .weights = {10, 10, 10, 10, 10, 10, 10, 10},
    },
    // ENCOUNTER_ZONE_VENUS_DJINN
    {
        .formations = {360, 360, 360, 361, 361, 361, 362},
        .weights = {10, 10, 10, 10, 10, 10, 10},
    },
    // ENCOUNTER_ZONE_MERCURY_DJINN
    {
        .formations = {365, 365, 365, 366, 367, 367, 368},
        .weights = {10, 10, 10, 10, 10, 10, 10},
    },
    // ENCOUNTER_ZONE_MARS_DJINN
    {
        .formations = {371, 371, 372, 372, 372, 373, 373},
        .weights = {10, 10, 10, 10, 10, 10, 10},
    },
    // ENCOUNTER_ZONE_JUPITER_DJINN
    {
        .formations = {376, 376, 377, 377, 377, 378, 379},
        .weights = {10, 10, 10, 10, 10, 10, 10},
    },
};
// clang-format on
