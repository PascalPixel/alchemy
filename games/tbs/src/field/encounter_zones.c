/// @file
/// The 104 encounter zones, by zone number: the zone names in the comments
/// say where each is used.

#include "field/encounter_zone.h"

// clang-format off
const struct EncounterZone gEncounterZones[] = {
    // No encounters
    {0},
    // Sol Sanctum Corridor
    {
        .rate = 37,
        .level = 1,
        .enemies = {7, 8, 9, 10},
        .weights = {4, 8, 8, 4},
    },
    // SolSanctumReliefs (entrances 1, 2)
    {
        .rate = 35,
        .level = 1,
        .enemies = {7, 8, 9, 10},
        .weights = {8, 4, 4, 8},
    },
    // SolSanctumReliefs (entrances 3, 6, 7)
    {
        .rate = 35,
        .level = 1,
        .enemies = {11, 12, 13, 14},
        .weights = {8, 8, 4, 4},
    },
    // Sol Sanctum Maze
    {
        .rate = 35,
        .level = 1,
        .enemies = {11, 12, 13, 14},
        .weights = {4, 4, 8, 8},
    },
    // SolSanctumReliefs (entrance 10)
    {
        .rate = 35,
        .level = 2,
        .enemies = {15, 16, 17, 18},
        .weights = {12, 8, 4, 4},
    },
    // Sol Sanctum Luna Sol Rooms
    {
        .rate = 35,
        .level = 2,
        .enemies = {15, 16, 17, 18},
        .weights = {4, 4, 12, 8},
    },
    // World Map: terrain kind 1, grassland and beach
    {
        .rate = 25,
        .level = 3,
        .enemies = {20, 21, 22, 23, 24, 25, 26, 27},
        .weights = {4, 4, 4, 4, 2, 2, 2, 2},
    },
    // World Map: terrain kind 1, forest
    {
        .rate = 9,
        .level = 3,
        .enemies = {20, 21, 22, 23, 24, 25, 26, 27},
        .weights = {2, 2, 2, 2, 3, 7, 2, 3},
    },
    // Goma Cave Pond
    {
        .rate = 30,
        .level = 4,
        .enemies = {31, 32, 33, 34, 35},
        .weights = {3, 3, 9, 3, 6},
    },
    // Goma Cave Pillar
    {
        .rate = 30,
        .level = 5,
        .enemies = {31, 32, 33, 34, 35, 36, 37},
        .weights = {2, 2, 2, 2, 8, 5, 2},
    },
    // GomaCaveStream
    {
        .rate = 30,
        .level = 5,
        .enemies = {31, 32, 33, 34, 35, 36, 37},
        .weights = {2, 2, 5, 2, 8, 8, 14},
    },
    // Event battles: Vale, Vault, World Map
    {
        .enemies = {1, 2, 3, 4, 5, 29, 28},
        .weights = {10, 10, 10, 10, 10, 10, 1},
    },
    // World Map: terrain kind 2, grassland and beach
    {
        .rate = 25,
        .level = 6,
        .enemies = {50, 51, 52, 53, 54, 55, 56, 57},
        .weights = {4, 4, 4, 4, 4, 2, 2, 2},
    },
    // World Map: terrain kind 2, forest
    {
        .rate = 9,
        .level = 6,
        .enemies = {50, 51, 52, 53, 54, 55, 56, 57},
        .weights = {1, 1, 1, 1, 1, 2, 2, 2},
    },
    // KolimaTunnel
    {
        .rate = 35,
        .level = 6,
        .enemies = {50, 51, 52, 53, 54, 55, 56, 57},
        .weights = {1, 2, 2, 1, 1, 5, 5, 5},
    },
    // Kolima Forest Bridge
    {
        .rate = 40,
        .level = 6,
        .enemies = {59, 60, 61, 62, 63, 64, 65, 66},
        .weights = {10, 5, 1, 1, 1, 1},
    },
    // Kolima Forest Clearing
    {
        .rate = 40,
        .level = 7,
        .enemies = {59, 60, 61, 62, 63, 64, 65, 66},
        .weights = {2, 6, 2, 2, 8, 6, 2, 2},
    },
    // Kolima Forest River
    {
        .rate = 40,
        .level = 7,
        .enemies = {59, 60, 61, 62, 63, 64, 65, 66},
        .weights = {1, 1, 2, 2, 3, 3, 8, 12},
    },
    // Kolima Forest Tree
    {
        .rate = 40,
        .level = 7,
        .enemies = {59, 60, 61, 62, 63, 64, 65, 66},
        .weights = {1, 1, 2, 2, 3, 3, 5, 12},
    },
    // Tret Tree Branch (entrances 1 to 3)
    {
        .rate = 45,
        .level = 7,
        .enemies = {68, 69, 70, 71, 72, 73, 74, 75},
        .weights = {8, 5, 6, 2, 1, 2, 1, 1},
    },
    // Tret Tree Interiors (entrances 1, 11, 16, 21 to 29)
    {
        .rate = 45,
        .level = 7,
        .enemies = {68, 69, 70, 71, 72, 73, 74, 75},
        .weights = {6, 6, 6, 3, 2, 1},
    },
    // Tret Tree Interiors (entrances 2 to 5, 12, 13, 31, 32)
    {
        .rate = 45,
        .level = 8,
        .enemies = {68, 69, 70, 71, 72, 73, 74, 75},
        .weights = {3, 5, 5, 5, 3, 2, 1, 1},
    },
    // Tret Tree Branch (any entrance)
    {
        .rate = 45,
        .level = 8,
        .enemies = {68, 69, 70, 71, 72, 73, 74, 75},
        .weights = {2, 1, 5, 8, 1, 8, 1, 5},
    },
    // Tret Tree Interiors (entrances 6 to 9, 14, 51 to 59)
    {
        .rate = 45,
        .level = 8,
        .enemies = {68, 69, 70, 71, 72, 73, 74, 75},
        .weights = {1, 1, 1, 1, 2, 4, 12, 8},
    },
    // BilibinCaveRapier (any entrance)
    {
        .rate = 30,
        .level = 8,
        .enemies = {77, 78, 79, 80, 81, 82, 83, 84},
        .weights = {6, 6, 4, 4, 2, 2, 1, 1},
    },
    // BilibinCaveIce (any entrance)
    {
        .rate = 30,
        .level = 8,
        .enemies = {77, 78, 79, 80, 81, 82, 83, 84},
        .weights = {2, 2, 2, 4, 4, 5, 7, 7},
    },
    // BilibinCaveRapier (entrances 5, 6)
    {
        .rate = 30,
        .level = 9,
        .enemies = {77, 78, 79, 80, 81, 82, 83, 84},
        .weights = {1, 1, 1, 1, 1, 2, 2, 2},
    },
    // BilibinCaveIce (entrance 5)
    {
        .rate = 30,
        .level = 9,
        .enemies = {77, 78, 79, 80, 81, 82, 83, 84},
        .weights = {1, 1, 1, 1, 1, 2, 2, 2},
    },
    // World Map: terrain kind 21, snowfield (behavior 5)
    {
        .rate = 25,
        .level = 9,
        .enemies = {86, 87, 88, 89, 90},
        .weights = {3, 3, 5, 5, 5},
    },
    // World Map: terrain kind 21, snowfield (behavior 6)
    {
        .rate = 9,
        .level = 9,
        .enemies = {86, 87, 88, 89, 90},
        .weights = {2, 2, 4, 4, 4},
    },
    // Mercury Lighthouse Entrance, MercuryLighthouseLower
    {
        .rate = 40,
        .level = 9,
        .enemies = {92, 93, 94, 95, 96, 97, 98, 99},
        .weights = {4, 4, 4, 2, 2, 2, 2, 2},
    },
    // MercuryLighthouseLower and MercuryLighthouseMiddle
    {
        .rate = 40,
        .level = 9,
        .enemies = {92, 93, 94, 95, 96, 97, 98, 99},
        .weights = {2, 2, 2, 4, 4, 2, 2, 2},
    },
    // MercuryLighthouseMiddle and MercuryLighthouseUpper
    {
        .rate = 40,
        .level = 10,
        .enemies = {97, 98, 99, 100, 101, 102, 103, 104},
        .weights = {2, 2, 2, 4, 4, 4, 2, 2},
    },
    // MercuryLighthouseUpper and MercuryLighthouseExterior
    {
        .rate = 40,
        .level = 11,
        .enemies = {97, 98, 99, 100, 101, 102, 103, 104},
        .weights = {1, 1, 1, 2, 2, 2, 4, 4},
    },
    // MercuryLighthouseExterior (entrances 3, 4)
    {
        .rate = 40,
        .level = 11,
        .enemies = {97, 98, 99, 100, 101, 102, 103, 104},
        .weights = {1, 1, 1, 2, 2, 2, 4, 4},
    },
    // Event battles: Tret Tree, Mercury Lighthouse
    {
        .enemies = {106, 107, 108},
        .weights = {10, 10, 10},
    },
    // World Map: terrain kind 31, grassland
    {
        .rate = 20,
        .level = 11,
        .enemies = {120, 121, 122, 123},
        .weights = {3, 3, 3, 3},
    },
    // World Map: terrain kind 31, forest
    {
        .rate = 9,
        .level = 11,
        .enemies = {120, 121, 122, 123},
        .weights = {1, 1, 1, 1},
    },
    // FuchinFallsCaveEntrance and FuchinFallsCaveZephyr
    {
        .rate = 38,
        .level = 11,
        .enemies = {120, 121, 122, 123, 124, 125, 126, 127},
        .weights = {4, 4, 4, 8, 6, 4, 2, 1},
    },
    // FuchinFallsCaveDragonsEye
    {
        .rate = 38,
        .level = 11,
        .enemies = {120, 121, 122, 123, 124, 125, 126, 127},
        .weights = {1, 2, 2, 3, 6, 6, 3, 2},
    },
    // FuchinFallsCaveDragonStatue
    {
        .rate = 38,
        .level = 11,
        .enemies = {120, 121, 122, 123, 124, 125, 126, 127},
        .weights = {1, 1, 2, 3, 4, 4, 6, 8},
    },
    // MogallForestSign
    {
        .rate = 42,
        .level = 11,
        .enemies = {129, 130, 131, 132, 133, 134, 135, 136},
        .weights = {6, 6, 6, 3, 2, 1, 1, 1},
    },
    // MogallForestQuartz
    {
        .rate = 42,
        .level = 12,
        .enemies = {129, 130, 131, 132, 133, 134, 135, 136},
        .weights = {1, 1, 3, 6, 6, 3, 1, 1},
    },
    // MogallForestKillerApe
    {
        .rate = 42,
        .level = 12,
        .enemies = {129, 130, 131, 132, 133, 134, 135, 136},
        .weights = {1, 1, 2, 2, 3, 4, 5, 5},
    },
    // World Map: terrain kind 3, grassland, desert and beach
    {
        .rate = 25,
        .level = 12,
        .enemies = {141, 142, 143, 144, 145, 146},
        .weights = {3, 3, 1, 3, 1, 1},
    },
    // World Map: terrain kind 3, forest
    {
        .rate = 9,
        .level = 12,
        .enemies = {141, 142, 143, 144, 145, 146},
        .weights = {1, 1, 2, 1, 2, 2},
    },
    // AltinPeakRailTunnel, AltinPeakSecondWaterBeast, AltinPeakSouthEntrance
    {
        .rate = 45,
        .level = 12,
        .enemies = {148, 149, 150, 151, 152, 153, 154, 155},
        .weights = {6, 6, 4, 2, 1, 1, 1, 1},
    },
    // AltinPeakThirdWaterBeast and AltinPeakPool
    {
        .rate = 45,
        .level = 13,
        .enemies = {148, 149, 150, 151, 152, 153, 154, 155},
        .weights = {1, 1, 2, 4, 3, 4, 2, 2},
    },
    // AltinPeakSpritz, AltinPeakJunction, AltinPeakFallingRocks and AltinPeakWestExit
    {
        .rate = 42,
        .level = 13,
        .enemies = {148, 149, 150, 151, 152, 153, 154, 155},
        .weights = {1, 1, 2, 3, 4, 4, 8, 8},
    },
    // LamakanDesertEast
    {
        .rate = 40,
        .level = 13,
        .enemies = {160, 161, 162, 163, 164, 165, 166, 167},
        .weights = {5, 5, 4, 3, 3, 2, 1, 1},
    },
    // LamakanDesertSmog
    {
        .rate = 40,
        .level = 14,
        .enemies = {160, 161, 162, 163, 164, 165, 166, 167},
        .weights = {1, 2, 3, 6, 6, 4, 2, 1},
    },
    // LamakanDesertManticore
    {
        .rate = 40,
        .level = 14,
        .enemies = {160, 161, 162, 163, 164, 165, 166, 167},
        .weights = {1, 1, 1, 2, 3, 4, 4, 5},
    },
    // Event battles: Mogall Forest, Altin Peak, Lamakan Desert
    {
        .enemies = {138, 139, 157, 158, 169, 170},
        .weights = {10, 10, 10, 10, 10, 10},
    },
    // World Map: terrain kind 4, grassland, desert and beach
    {
        .rate = 25,
        .level = 15,
        .enemies = {180, 181, 182, 183, 184, 185, 186, 187},
        .weights = {4, 4, 4, 4, 4, 2, 2, 2},
    },
    // World Map: terrain kind 4, forest
    {
        .rate = 9,
        .level = 15,
        .enemies = {180, 181, 182, 183, 184, 185, 186, 187},
        .weights = {2, 2, 2, 2, 3, 4, 4, 4},
    },
    // ValeCaveKite (entrances 1 to 4)
    {
        .rate = 40,
        .level = 15,
        .enemies = {189, 190, 191, 192, 193, 194, 195, 196},
        .weights = {6, 6, 6, 4, 4, 1, 1, 1},
    },
    // ValeCaveKite (entrances 8 to 10)
    {
        .rate = 40,
        .level = 15,
        .enemies = {189, 190, 191, 192, 193, 194, 195, 196},
        .weights = {4, 4, 4, 6, 6, 2, 2, 2},
    },
    // ValeCaveLogs (entrances 1 to 3)
    {
        .rate = 40,
        .level = 15,
        .enemies = {189, 190, 191, 192, 193, 194, 195, 196},
        .weights = {2, 2, 2, 3, 3, 5, 5, 5},
    },
    // ValeCaveLogs (entrances 4, 5)
    {
        .rate = 40,
        .level = 15,
        .enemies = {189, 190, 191, 192, 193, 194, 195, 196},
        .weights = {1, 1, 1, 2, 2, 5, 5, 5},
    },
    // VaultCavePassage
    {
        .rate = 40,
        .level = 16,
        .enemies = {201, 202, 203, 204, 205, 206, 207, 208},
        .weights = {8, 8, 8, 4, 4, 4, 2, 2},
    },
    // VaultCaveVambrace and VaultCaveMouth
    {
        .rate = 40,
        .level = 16,
        .enemies = {201, 202, 203, 204, 205, 206, 207, 208},
        .weights = {2, 2, 2, 4, 4, 4, 8, 8},
    },
    // Event battles: Tolbi ship
    {
        .level = 17,
        .enemies = {210, 211, 212, 213},
        .weights = {10, 10, 10, 10},
    },
    // Event battles: Crossbone Isle
    {
        .level = 17,
        .enemies = {291, 292, 293, 294},
        .weights = {10, 10, 10, 10},
    },
    // World Map: terrain kind 5, grassland, desert and beach
    {
        .rate = 25,
        .level = 18,
        .enemies = {220, 221, 222, 223, 224, 225, 226, 227},
        .weights = {6, 6, 6, 6, 3, 3, 3, 3},
    },
    // World Map: terrain kind 5, forest
    {
        .rate = 9,
        .level = 18,
        .enemies = {220, 221, 222, 223, 224, 225, 226, 227},
        .weights = {3, 3, 3, 3, 6, 6, 6, 6},
    },
    // AltmillerCaveEntrance
    {
        .rate = 40,
        .level = 17,
        .enemies = {229, 230, 231, 232, 233, 234, 235, 236},
        .weights = {6, 6, 4, 4, 4, 2, 1, 1},
    },
    // AltmillerCaveBabi
    {
        .rate = 40,
        .level = 17,
        .enemies = {229, 230, 231, 232, 233, 234, 235, 236},
        .weights = {4, 4, 6, 6, 4, 4, 2, 1},
    },
    // AltmillerCaveVialChest
    {
        .rate = 40,
        .level = 18,
        .enemies = {229, 230, 231, 232, 233, 234, 235, 236},
        .weights = {3, 3, 4, 6, 6, 4, 2, 1},
    },
    // AltmillerCaveSquall
    {
        .rate = 40,
        .level = 18,
        .enemies = {229, 230, 231, 232, 233, 234, 235, 236},
        .weights = {2, 2, 4, 4, 6, 5, 5, 3},
    },
    // AltmillerCaveCookieChest
    {
        .rate = 40,
        .level = 18,
        .enemies = {229, 230, 231, 232, 233, 234, 235, 236},
        .weights = {2, 2, 3, 3, 4, 6, 6, 4},
    },
    // AltmillerCaveDraught (entrances 50, 51)
    {
        .rate = 40,
        .level = 18,
        .enemies = {229, 230, 231, 232, 233, 234, 235, 236},
        .weights = {1, 1, 1, 2, 2, 4, 6, 6},
    },
    // Event battles: Colosso
    {
        .level = 19,
        .enemies = {238, 239, 240, 241, 242, 243},
        .weights = {10, 10, 10, 10, 10, 10},
    },
    // GondowanCave (any entrance)
    {
        .rate = 30,
        .level = 19,
        .enemies = {250, 251, 252, 253, 254, 255, 256, 257},
        .weights = {3, 3, 4, 4, 5, 5, 6, 6},
    },
    // GondowanCaveRiver (any entrance)
    {
        .rate = 30,
        .level = 20,
        .enemies = {250, 251, 252, 253, 254, 255, 256, 257},
        .weights = {3, 3, 4, 4, 5, 5, 6, 6},
    },
    // GondowanCaveRiver (entrances 1, 2)
    {
        .rate = 30,
        .level = 20,
        .enemies = {250, 251, 252, 253, 254, 255, 256, 257},
        .weights = {3, 3, 4, 4, 5, 5, 6, 6},
    },
    // GondowanCave (entrances 1 to 3)
    {
        .rate = 30,
        .level = 20,
        .enemies = {250, 251, 252, 253, 254, 255, 256, 257},
        .weights = {3, 3, 4, 4, 5, 5, 6, 6},
    },
    // LunpaFortressCave (entrances 1, 2)
    {
        .rate = 35,
        .level = 21,
        .enemies = {259, 260, 261, 262},
        .weights = {2, 2, 2, 2},
    },
    // LunpaFortressCave (any entrance)
    {
        .rate = 35,
        .level = 21,
        .enemies = {259, 260, 261, 262},
        .weights = {2, 2, 2, 2},
    },
    // LunpaFortressToadonpa (entrances 1, 2)
    {
        .rate = 35,
        .level = 21,
        .enemies = {259, 260, 261, 262},
        .weights = {2, 2, 2, 2},
    },
    // World Map: terrain kind 6, grassland, desert and beach
    {
        .rate = 25,
        .level = 22,
        .enemies = {268, 269, 270, 271, 272, 273, 274, 275},
        .weights = {4, 4, 3, 3, 3, 3, 3, 3},
    },
    // World Map: terrain kind 6, forest
    {
        .rate = 9,
        .level = 22,
        .enemies = {268, 269, 270, 271, 272, 273, 274, 275},
        .weights = {2, 2, 3, 3, 3, 4, 4, 4},
    },
    // SuhallaDesertNorth
    {
        .rate = 40,
        .level = 22,
        .enemies = {277, 278, 279, 280, 281, 282, 284, 285},
        .weights = {4, 4, 4, 5, 3, 4, 2, 2},
    },
    // SuhallaDesertSouth
    {
        .rate = 40,
        .level = 22,
        .enemies = {277, 278, 279, 280, 281, 282, 284, 285},
        .weights = {2, 2, 2, 4, 3, 4, 4, 4},
    },
    // Event battles: Crossbone Isle
    {
        .level = 17,
        .enemies = {296, 297, 298, 300, 301, 302},
        .weights = {10, 10, 10, 10, 10, 10},
    },
    // SuhallaGateDew (entrances 1, 2)
    {
        .rate = 30,
        .level = 23,
        .enemies = {310, 311, 312, 313, 314, 315, 316, 317},
        .weights = {6, 6, 6, 3, 3, 2, 2, 2},
    },
    // SuhallaGateDew (any entrance)
    {
        .rate = 30,
        .level = 24,
        .enemies = {310, 311, 312, 313, 314, 315, 316, 317},
        .weights = {2, 3, 3, 6, 6, 3, 3, 3},
    },
    // GatewayCave (entrances 1, 2)
    {
        .rate = 30,
        .level = 24,
        .enemies = {310, 311, 312, 313, 314, 315, 316, 317},
        .weights = {1, 1, 2, 2, 2, 3, 3, 3},
    },
    // GatewayCave (any entrance)
    {
        .rate = 30,
        .level = 24,
        .enemies = {310, 311, 312, 313, 314, 315, 316, 317},
        .weights = {2, 2, 3, 3, 3, 6, 6, 6},
    },
    // VenusLighthouseGroundFloor (any entrance)
    {
        .rate = 40,
        .level = 25,
        .enemies = {319, 320, 321, 322, 323},
        .weights = {4, 4, 4, 4, 4},
    },
    // VenusLighthouseGroundFloor (entrances 20 to 26, 30 to 32)
    {
        .rate = 40,
        .level = 25,
        .enemies = {319, 320, 321, 322, 323},
        .weights = {4, 4, 4, 4, 4},
    },
    // BabiLighthouseEntry, BabiLighthouseLuff, BabiLighthouseStoneDoor, TunnelRuinsLadyStatues (entrances 1 to 3)
    {
        .rate = 35,
        .level = 25,
        .enemies = {328, 329, 330, 331, 332, 333, 334, 335},
        .weights = {6, 6, 6, 4, 4, 4, 2, 2},
    },
    // TunnelRuinsGreatStatue, TunnelRuinsLadyStatues (entrances 8 to 11)
    {
        .rate = 35,
        .level = 25,
        .enemies = {328, 329, 330, 331, 332, 333, 334, 335},
        .weights = {3, 3, 4, 6, 6, 4, 3, 3},
    },
    // TunnelRuinsLadyStatues (entrances 12, 15 to 17)
    {
        .rate = 35,
        .level = 26,
        .enemies = {328, 329, 330, 331, 332, 333, 334, 335},
        .weights = {3, 3, 3, 4, 4, 4, 6, 6},
    },
    // VenusLighthouseFirstFloor (any entrance)
    {
        .rate = 34,
        .level = 27,
        .enemies = {337, 338, 339, 340, 341, 342, 343, 344},
        .weights = {4, 4, 3, 3, 3, 3, 2, 2},
    },
    // VenusLighthouseFirstFloor (entrances 19, 20), VenusLighthouseSecondFloor
    {
        .rate = 32,
        .level = 28,
        .enemies = {337, 338, 339, 340, 341, 342, 343, 344},
        .weights = {3, 3, 4, 4, 3, 3, 2, 2},
    },
    // VenusLighthouseThirdFloor
    {
        .rate = 30,
        .level = 29,
        .enemies = {337, 338, 339, 340, 341, 342, 343, 344},
        .weights = {2, 2, 3, 3, 4, 4, 3, 3},
    },
    // VenusLighthouseFourthFloor
    {
        .rate = 30,
        .level = 30,
        .enemies = {337, 338, 339, 340, 341, 342, 343, 344},
        .weights = {2, 2, 2, 3, 3, 3, 4, 4},
    },
    // Event battles: Suhalla Desert, Venus Lighthouse Aerie, World Map
    {
        .enemies = {347, 346, 264, 265, 266, 287, 288, 289},
        .weights = {10, 10, 30, 10, 10, 80, 10, 10},
    },
    // Mimic chests
    {
        .enemies = {350, 351, 352, 353, 354, 355, 356, 357},
        .weights = {10, 10, 10, 10, 10, 10, 10, 10},
    },
    // Venus Djinn battles
    {
        .enemies = {360, 360, 360, 361, 361, 361, 362},
        .weights = {10, 10, 10, 10, 10, 10, 10},
    },
    // Mercury Djinn battles
    {
        .enemies = {365, 365, 365, 366, 367, 367, 368},
        .weights = {10, 10, 10, 10, 10, 10, 10},
    },
    // Mars Djinn battles
    {
        .enemies = {371, 371, 372, 372, 372, 373, 373},
        .weights = {10, 10, 10, 10, 10, 10, 10},
    },
    // Jupiter Djinn battles
    {
        .enemies = {376, 376, 377, 377, 377, 378, 379},
        .weights = {10, 10, 10, 10, 10, 10, 10},
    },
};
// clang-format on
