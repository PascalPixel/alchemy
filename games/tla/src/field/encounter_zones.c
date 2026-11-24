/// @file
/// The 110 encounter zones, by zone number: each zone's rate, level,
/// formations and their weights. A field zone's comment gives its area name
/// (gArea_NameRules) and the scenes and entrances that take it
/// (gScene_EncounterRules); a World Map zone's, its encounter regions and
/// the battle environments that take it (gWorldMap_EncounterRules); a Djinni
/// zone's, the Djinn whose battles it holds (gEncounter_AreaEntryTable).

#include "battle/formations.h"
#include "field/encounter_zone.h"

/// Each encounter zone's battles, a row a zone, the zone the ground under the
/// party gives.
// clang-format off
const struct EncounterZone gEncounterZones[] = {
    // None: a ground whose zone is 0 has no random battles
    {0},
    // Djinni battles: DJINNI_FLINT to DJINNI_SALT
    {
        .formations = {1, 2, 3, 4, 5, 6, 7, 8},
        .weights = {10, 10, 10, 10, 10, 10, 10, 10},
    },
    // Djinni battles: DJINNI_GEODE to DJINNI_EARTH19
    {
        .formations = {9, 10, 11},
        .weights = {10, 10, 10},
    },
    // Djinni battles: DJINNI_FIZZ to DJINNI_GEL
    {
        .formations = {13, 14, 15, 16, 17, 18, 19, 20},
        .weights = {10, 10, 10, 10, 10, 10, 10, 10},
    },
    // Djinni battles: DJINNI_EDDY to DJINNI_WATER19
    {
        .formations = {21, 22, 23},
        .weights = {10, 10, 10},
    },
    // Djinni battles: DJINNI_FORGE to DJINNI_TINDER
    {
        .formations = {25, 26, 27, 28, 29, 30, 31, 32},
        .weights = {10, 10, 10, 10, 10, 10, 10, 10},
    },
    // Djinni battles: DJINNI_SHINE to DJINNI_FIRE19
    {
        .formations = {33, 34, 35},
        .weights = {10, 10, 10},
    },
    // Djinni battles: DJINNI_GUST to DJINNI_WHORL
    {
        .formations = {37, 38, 39, 40, 41, 42, 43, 44},
        .weights = {10, 10, 10, 10, 10, 10, 10, 10},
    },
    // Djinni battles: DJINNI_GASP to DJINNI_WIND19
    {
        .formations = {45, 46, 47},
        .weights = {10, 10, 10},
    },
    // Story battles: at Venus Lighthouse's entrance (slot 0), Suhalla Gate (1
    // to 3), the Kandorean Temple (4) and on the World Map (5)
    {
        .formations = {49, 50, 51, 52, 53, 637},
        .weights = {10, 10, 10, 10, 10, 10},
    },
    // ENCOUNTER_ZONE_SCRIPTED_YAMPI_DESERT_ALHAFRA
    {
        .formations = {55, 56},
        .weights = {10, 10},
    },
    // ENCOUNTER_ZONE_AQUA_JELLIES
    {
        .formations = {58, 59},
        .weights = {50, 10},
    },
    // ENCOUNTER_ZONE_SERPENT
    {
        .formations = {61, 62, 63, 64, 65, 66, 67},
        .weights = {10, 10, 10, 10, 10, 10, 10},
    },
    // ENCOUNTER_ZONE_MOAPA
    {
        .formations = {69, 70, 71, 72, 73, FORMATION_KARST_AND_AGATIO_A,
                       FORMATION_KARST_AND_AGATIO_B,
                       FORMATION_KARST_AND_AGATIO_C},
        .weights = {10, 10, 10, 10, 10, 10, 10, 10},
    },
    // World Map, encounter region 1: beach, shallows and grassland
    {
        .rate = 35,
        .level = 4,
        .formations = {241, 242, 243, 244, 245, 246, 247, 248},
        .weights = {4, 4, 4, 4, 2, 2, 2, 2},
    },
    // World Map, encounter region 1: forest
    {
        .rate = 10,
        .level = 4,
        .formations = {241, 242, 243, 244, 245, 246, 247, 248},
        .weights = {1, 1, 1, 1, 3, 3, 3, 3},
    },
    // Kandorean Temple: SceneId_KandoreanTempleMimic, its other entrances, SceneId_KandoreanTempleFog, its other entrances
    {
        .rate = 50,
        .level = 4,
        .formations = {251, 252, 253, 254, 255, 256, 257, 258},
        .weights = {20, 20, 20, 10, 10, 5, 5, 5},
    },
    // Kandorean Temple: SceneId_KandoreanTempleLifts, SceneId_KandoreanTemplePillar entrance 2, SceneId_KandoreanTempleCurrent
    {
        .rate = 50,
        .level = 4,
        .formations = {251, 252, 253, 254, 255, 256, 257, 258},
        .weights = {5, 5, 10, 15, 15, 10, 5, 5},
    },
    // Kandorean Temple: SceneId_KandoreanTempleMimic entrances 2 and 3, SceneId_KandoreanTempleChestbeaters, SceneId_KandoreanTempleFog entrances 3 and 4
    {
        .rate = 50,
        .level = 4,
        .formations = {251, 252, 253, 254, 255, 256, 257, 258},
        .weights = {5, 5, 5, 10, 10, 20, 20, 20},
    },
    // Shrine of the Sea God: SceneId_ShrineOfTheSeaGodCrossroads entrances 3 to 5, SceneId_ShrineOfTheSeaGodFootprints, SceneId_ShrineOfTheSeaGodBreath
    {
        .rate = 50,
        .level = 5,
        .formations = {261, 262, 263, 264, 265, 266, 267, 268},
        .weights = {20, 20, 20, 10, 10, 10, 5, 5},
    },
    // Shrine of the Sea God: SceneId_ShrineOfTheSeaGodCrossroads, its other entrances, SceneId_ShrineOfTheSeaGodPillars, SceneId_ShrineOfTheSeaGodSeaGodsTear
    {
        .rate = 50,
        .level = 5,
        .formations = {261, 262, 263, 264, 265, 266, 267, 268},
        .weights = {8, 8, 10, 20, 20, 5, 5, 5},
    },
    // Shrine of the Sea God: SceneId_ShrineOfTheSeaGodLadders
    {
        .rate = 50,
        .level = 5,
        .formations = {261, 262, 263, 264, 265, 266, 267, 268},
        .weights = {5, 5, 5, 10, 10, 20, 20, 20},
    },
    // Dehkan Plateau: SceneId_DehkanPlateauFullMetalVestChest, SceneId_DehkanPlateauThemisAxeChest
    {
        .rate = 45,
        .level = 6,
        .formations = {271, 272, 273, 274, 275, 276, 277, 274},
        .weights = {20, 20, 20, 10, 8, 8, 5, 5},
    },
    // Dehkan Plateau: SceneId_DehkanPlateauTightropes, SceneId_DehkanPlateauMintChest, its other entrances
    {
        .rate = 45,
        .level = 6,
        .formations = {271, 272, 273, 274, 275, 276, 277, 274},
        .weights = {5, 5, 10, 20, 20, 10, 5, 5},
    },
    // Dehkan Plateau: SceneId_DehkanPlateauMintChest entrance 3, SceneId_DehkanPlateauPoundCube, SceneId_DehkanPlateauCannon
    {
        .rate = 45,
        .level = 6,
        .formations = {271, 272, 273, 274, 275, 276, 277, 274},
        .weights = {5, 5, 5, 8, 8, 10, 20, 20},
    },
    // World Map, encounter region 11: beach, shallows and grassland
    {
        .rate = 35,
        .level = 7,
        .formations = {281, 282, 283, 284, 285, 286, 287, 288},
        .weights = {9, 9, 9, 9, 3, 3, 3, 3},
    },
    // World Map, encounter region 11: forest
    {
        .rate = 10,
        .level = 7,
        .formations = {281, 282, 283, 284, 285, 286, 287, 288},
        .weights = {2, 2, 2, 2, 10, 10, 10, 10},
    },
    // Madra Catacombs: SceneId_MadraCatacombsEntrance, SceneId_MadraCatacombsLadders, SceneId_MadraCatacombsChampaTunnel
    {
        .rate = 50,
        .level = 8,
        .formations = {291, 292, 293, 294, 295, 296},
        .weights = {20, 20, 15, 10, 10, 10},
    },
    // Madra Catacombs: SceneId_MadraCatacombsStoneDoor, SceneId_MadraCatacombsBookshelf, SceneId_MadraCatacombsTablet
    {
        .rate = 50,
        .level = 8,
        .formations = {291, 292, 293, 294, 295, 296},
        .weights = {10, 10, 15, 20, 20, 20},
    },
    // World Map, encounter regions 2 and 12: beach, shallows and grassland
    {
        .rate = 35,
        .level = 9,
        .formations = {301, 302, 303, 304, 305, 306, 307, 308},
        .weights = {4, 4, 4, 4, 2, 2, 2, 2},
    },
    // World Map, encounter regions 2 and 12: forest
    {
        .rate = 10,
        .level = 9,
        .formations = {301, 302, 303, 304, 305, 306, 307, 308},
        .weights = {2, 2, 2, 2, 4, 4, 4, 4},
    },
    // Yampi Desert: SceneId_YampiDesertGuardianRingChest, SceneId_YampiDesertBlitz
    {
        .rate = 60,
        .level = 9,
        .formations = {311, 312, 313, 314, 315, 316, 317, 318},
        .weights = {20, 20, 10, 10, 10, 5, 5},
    },
    // Yampi Desert: SceneId_YampiDesertPsynergyStone, SceneId_YampiDesertCoinsPuddle, SceneId_YampiDesertSpringCave, SceneId_YampiDesertBlowMaceChest
    {
        .rate = 60,
        .level = 9,
        .formations = {311, 312, 313, 314, 315, 316, 317, 318},
        .weights = {10, 10, 20, 20, 10, 10, 5, 5},
    },
    // Yampi Desert: SceneId_YampiDesertSandPass, SceneId_YampiDesertTrainersWhipChest, SceneId_YampiDesertHiddenPsynergyStone
    {
        .rate = 60,
        .level = 9,
        .formations = {311, 312, 313, 314, 315, 316, 317, 318},
        .weights = {5, 5, 5, 5, 10, 10, 20, 20},
    },
    // Air's Rock: SceneId_AirsRockStormBrandChest
    {
        .rate = 55,
        .level = 10,
        .formations = {321, 322, 323, 324, 325, 326, 327},
        .weights = {20, 20, 15, 12, 10, 8, 5},
    },
    // Air's Rock: SceneId_AirsRockEntrance, SceneId_AirsRockFujinShieldChest entrances 8 and 9
    {
        .rate = 55,
        .level = 10,
        .formations = {321, 322, 323, 324, 325, 326, 327},
        .weights = {5, 5, 8, 14, 16, 20, 20},
    },
    // Air's Rock: SceneId_AirsRockInsideHall, its other entrances, SceneId_AirsRockInsideVialChest, SceneId_AirsRockInsideClarityCircletChest, SceneId_AirsRockInsidePairedStakes, SceneId_AirsRockInsideFaceVents
    {
        .rate = 65,
        .level = 11,
        .formations = {331, 332, 333, 334, 335, 336, 337, 338},
        .weights = {20, 20, 10, 10, 5, 5},
    },
    // Air's Rock: SceneId_AirsRockInsideHall entrances 4, 6, 7 and 10, SceneId_AirsRockInsideTightropes, SceneId_AirsRockInsideElixirChest
    {
        .rate = 65,
        .level = 12,
        .formations = {331, 332, 333, 334, 335, 336, 337, 338},
        .weights = {10, 10, 20, 20, 10, 10, 5, 5},
    },
    // Air's Rock: SceneId_AirsRockInsideHall entrances 5, 14 and 15, SceneId_AirsRockInsideTightropes, SceneId_AirsRockInsideCoinsChest
    {
        .rate = 65,
        .level = 12,
        .formations = {331, 332, 333, 334, 335, 336, 337, 338},
        .weights = {5, 5, 5, 5, 10, 10, 20, 20},
    },
    // Alhafran Cave: SceneId_AlhafranCave, SceneId_AlhafraJail, its other entrances
    {
        .rate = 50,
        .level = 13,
        .formations = {511, 512, 513, 514, 515, 516, 517, 518},
        .weights = {5, 5, 5, 5, 10, 10, 20, 20},
    },
    // Gondowan Cliffs: SceneId_GondowanCliffs entrance 2
    {
        .rate = 45,
        .level = 14,
        .formations = {341, 342, 343, 344, 345, 346, 347, 348},
        .weights = {20, 20, 20, 20, 10, 10, 10, 10},
    },
    // Gondowan Cliffs: SceneId_GondowanCliffs, its other entrances
    {
        .rate = 45,
        .level = 14,
        .formations = {341, 342, 343, 344, 345, 346, 347, 348},
        .weights = {10, 10, 10, 10, 20, 20, 20, 20},
    },
    // World Map, encounter regions 3 and 18: desert, beach, shallows and grassland
    {
        .rate = 35,
        .level = 16,
        .formations = {351, 352, 353, 354, 355, 356, 357, 358},
        .weights = {10, 10, 10, 10, 5, 5, 5, 5},
    },
    // World Map, encounter regions 3 and 18: forest
    {
        .rate = 10,
        .level = 16,
        .formations = {351, 352, 353, 354, 355, 356, 357, 358},
        .weights = {2, 2, 2, 2, 4, 4, 4, 4},
    },
    // Kibombo Mountains: SceneId_KibomboMountainsDiskAxeChest
    {
        .rate = 35,
        .level = 16,
        .formations = {361, 362, 363, 364, 365, 366, 367, 368},
        .weights = {20, 20, 20, 20, 10, 10, 10, 5},
    },
    // Kibombo Mountains: SceneId_KibomboMountainsWaft, SceneId_KibomboMountainsDiskAxeChest, SceneId_KibomboMountainsCavern
    {
        .rate = 35,
        .level = 16,
        .formations = {361, 362, 363, 364, 365, 366, 367, 368},
        .weights = {5, 5, 5, 5, 10, 10, 10, 20},
    },
    // Gabomba Statue: SceneId_GabombaStatueBoneArmletChest, SceneId_GabombaStatuePinion
    {
        .rate = 50,
        .level = 17,
        .formations = {371, 372, 373, 374, 375, 376, 377, 378},
        .weights = {20, 20, 10, 10, 5, 5},
    },
    // Gabomba Statue: SceneId_GabombaStatueElixirChest entrances 1 and 2, SceneId_GabombaStatueSteel entrances 1 and 4, SceneId_GabombaStatueMouse
    {
        .rate = 50,
        .level = 17,
        .formations = {371, 372, 373, 374, 375, 376, 377, 378},
        .weights = {10, 10, 20, 20, 20, 10, 5, 5},
    },
    // Gabomba Statue: SceneId_GabombaStatueElixirChest, its other entrances, SceneId_GabombaStatueSteel, its other entrances, SceneId_GabombaStatueCogs, SceneId_GabombaStatueChamberIdol
    {
        .rate = 50,
        .level = 17,
        .formations = {371, 372, 373, 374, 375, 376, 377, 378},
        .weights = {5, 5, 5, 10, 10, 20, 20, 20},
    },
    // Gabomba Catacombs: SceneId_GabombaCatacombsTeleporters, SceneId_GabombaCatacombsMud
    {
        .rate = 50,
        .level = 18,
        .formations = {521, 522, 523, 524, 525, 526, 527, 528},
        .weights = {20, 20, 20, 15, 10, 10, 10, 10},
    },
    // Gabomba Catacombs: SceneId_GabombaCatacombsHiddenWall, SceneId_GabombaCatacombsTomegathericon, SceneId_GabombaCatacombsStream
    {
        .rate = 50,
        .level = 18,
        .formations = {521, 522, 523, 524, 525, 526, 527, 528},
        .weights = {5, 5, 5, 10, 10, 20, 20, 20},
    },
    // Lemurian Ship: SceneId_LemurianShipCabins
    {
        .rate = 45,
        .level = 18,
        .formations = {381, 382, 383, 384},
        .weights = {5, 10, 8, 6},
    },
    // World Map aboard the ship, encounter regions 7 and 9: sea
    {
        .rate = 65,
        .level = 20,
        .formations = {391, 392, 393, 394, 395, 396, 397, 398},
        .weights = {10, 10, 10, 10, 10, 10, 10, 10},
    },
    // World Map, encounter region 4: desert, beach, shallows and grassland
    {
        .rate = 35,
        .level = 21,
        .formations = {401, 402, 403, 404, 405, 406, 407, 408},
        .weights = {6, 6, 6, 6, 3, 3, 3, 3},
    },
    // World Map, encounter region 4: forest
    {
        .rate = 10,
        .level = 21,
        .formations = {401, 402, 403, 404, 405, 406, 407, 408},
        .weights = {2, 2, 2, 2, 4, 4, 4, 4},
    },
    // Taopo Swamp: SceneId_TaopoSwampEntrance, SceneId_TaopoSwampMud, SceneId_TaopoSwampTearStones, SceneId_TaopoSwampFlower, SceneId_TaopoSwampVents
    {
        .rate = 50,
        .level = 21,
        .formations = {411, 412, 413, 414, 415, 416, 417, 418},
        .weights = {20, 20, 20, 10, 5, 5, 5},
    },
    // Taopo Swamp: SceneId_TaopoSwampPool, SceneId_TaopoSwampPillarGap, SceneId_TaopoSwampDrips
    {
        .rate = 50,
        .level = 21,
        .formations = {411, 412, 413, 414, 415, 416, 417, 418},
        .weights = {10, 10, 10, 20, 20, 20, 5, 5},
    },
    // Taopo Swamp: SceneId_TaopoSwampLavaRocks, SceneId_TaopoSwampTremor, SceneId_TaopoSwampStarDust
    {
        .rate = 50,
        .level = 21,
        .formations = {411, 412, 413, 414, 415, 416, 417, 418},
        .weights = {5, 5, 5, 10, 10, 10, 20, 20},
    },
    // Apojii Islands Cave: SceneId_ApojiiIslandsCave, SceneId_AquaRockMoai
    {
        .rate = 50,
        .level = 22,
        .formations = {421, 422, 423, 424, 425, 426, 427, 428},
        .weights = {20, 20, 5, 5},
    },
    // Aqua Rock: SceneId_AquaRockMistSabreChest
    {
        .rate = 50,
        .level = 22,
        .formations = {421, 422, 423, 424, 425, 426, 427, 428},
        .weights = {10, 20, 20, 5, 5},
    },
    // Aqua Rock: SceneId_AquaRockInsideCrystalPowderChest, SceneId_AquaRockInsideVialChest
    {
        .rate = 50,
        .level = 22,
        .formations = {421, 422, 423, 424, 425, 426, 427, 428},
        .weights = {10, 10, 20, 20, 5, 5},
    },
    // Aqua Rock: SceneId_AquaRockInsideTightrope, SceneId_AquaRockInsideTearStone
    {
        .rate = 50,
        .level = 22,
        .formations = {421, 422, 423, 424, 425, 426, 427, 428},
        .weights = {5, 5, 10, 10, 20, 20, 5, 5},
    },
    // Aqua Rock: SceneId_AquaRockInsideWaterOfLifeChest, SceneId_AquaRockInsideSteam, SceneId_AquaRockInsideAquariusStone, SceneId_AquaRockInsideRustySwordChest, its other entrances, SceneId_AquaRockInsidePsynergyStone
    {
        .rate = 50,
        .level = 22,
        .formations = {421, 422, 423, 424, 425, 426, 427, 428},
        .weights = {5, 5, 5, 5, 10, 10, 20, 20},
    },
    // Gaia Rock: SceneId_GaiaRockSusa, SceneId_GaiaRockStatueSpots, its other entrances, SceneId_GaiaRockDancingIdol
    {
        .rate = 55,
        .level = 23,
        .formations = {431, 432, 433, 434, 435, 436, 437, 438},
        .weights = {20, 20, 5, 5},
    },
    // Gaia Rock: SceneId_GaiaRockStatueSpots entrances 3 and 4
    {
        .rate = 55,
        .level = 23,
        .formations = {431, 432, 433, 434, 435, 436, 437, 438},
        .weights = {10, 20, 20, 5, 5},
    },
    // Gaia Rock: SceneId_GaiaRockInsideMaze, its other entrances, SceneId_GaiaRockInsideFirstAltar
    {
        .rate = 55,
        .level = 23,
        .formations = {431, 432, 433, 434, 435, 436, 437, 438},
        .weights = {10, 10, 20, 20, 5, 5},
    },
    // Gaia Rock: SceneId_GaiaRockInsideMaze, its other entrances, SceneId_GaiaRockInsideTwinAltars, SceneId_GaiaRockInsidePlatforms
    {
        .rate = 55,
        .level = 23,
        .formations = {431, 432, 433, 434, 435, 436, 437, 438},
        .weights = {5, 5, 10, 10, 20, 20, 5, 5},
    },
    // Gaia Rock: SceneId_GaiaRockInsideAltar
    {
        .rate = 55,
        .level = 23,
        .formations = {431, 432, 433, 434, 435, 436, 437, 438},
        .weights = {0, 0, 5, 5, 10, 10, 20, 20},
    },
    // Izumo Ruins: SceneId_IzumoRuinsFirstRoom
    {
        .rate = 50,
        .level = 24,
        .formations = {531, 532, 533, 534, 535, 536, 537, 538},
        .weights = {20, 20, 20, 20, 10, 10, 10, 10},
    },
    // Izumo Ruins: SceneId_IzumoRuinsSecondRoom
    {
        .rate = 50,
        .level = 24,
        .formations = {531, 532, 533, 534, 535, 536, 537, 538},
        .weights = {10, 10, 10, 10, 20, 20, 30, 30},
    },
    // Islet Cave: SceneId_IsletCaveHalls
    {
        .rate = 50,
        .level = 25,
        .formations = {541, 542, 543, 544, 545, 546, 547, 548},
        .weights = {10, 10, 10, 10, 10, 10, 10, 10},
    },
    // Ankohl Ruins: SceneId_AnkohlRuinsStoneFaces, SceneId_AnkohlRuinsThanatosMaceChest, SceneId_AnkohlRuinsGreatFace
    {
        .rate = 60,
        .level = 25,
        .formations = {441, 442, 443, 444, 445, 446, 447, 448},
        .weights = {20, 20, 20, 10, 10, 5, 5},
    },
    // Ankohl Ruins: SceneId_AnkohlRuinsFaceHalves
    {
        .rate = 60,
        .level = 25,
        .formations = {441, 442, 443, 444, 445, 446, 447, 448},
        .weights = {5, 5, 10, 15, 15, 10, 10},
    },
    // Ankohl Ruins: SceneId_AnkohlRuinsCrystalPowderChest
    {
        .rate = 60,
        .level = 25,
        .formations = {441, 442, 443, 444, 445, 446, 447, 448},
        .weights = {5, 5, 5, 10, 10, 20, 20, 20},
    },
    // Tundaria Tower: SceneId_TundariaTowerCenterProng, SceneId_TundariaTowerIceSlide
    {
        .rate = 50,
        .level = 26,
        .formations = {451, 452, 453, 454, 455, 456, 457, 458},
        .weights = {15, 15, 15, 10, 10, 5, 5, 5},
    },
    // Tundaria Tower: SceneId_TundariaTowerReflux, SceneId_TundariaTowerIceColumns
    {
        .rate = 50,
        .level = 26,
        .formations = {451, 452, 453, 454, 455, 456, 457, 458},
        .weights = {5, 5, 10, 20, 20, 15, 10, 5},
    },
    // Tundaria Tower: SceneId_TundariaTowerPillarPuzzle
    {
        .rate = 50,
        .level = 26,
        .formations = {451, 452, 453, 454, 455, 456, 457, 458},
        .weights = {5, 5, 5, 10, 10, 15, 20, 20},
    },
    // World Map aboard the ship, encounter region 8: sea
    {
        .rate = 60,
        .level = 29,
        .formations = {461, 462, 463, 464, 465, 466, 467, 468},
        .weights = {10, 10, 10, 10, 10, 10, 10, 10},
    },
    // World Map, encounter regions 5, 15 and 16: desert, beach, shallows, grassland, snowfield and blizzard
    {
        .rate = 35,
        .level = 30,
        .formations = {471, 472, 473, 474, 475, 476, 477, 478},
        .weights = {6, 6, 6, 6, 3, 3, 3, 3},
    },
    // World Map, encounter regions 5, 15 and 16: forest, snow forest and blizzard forest
    {
        .rate = 10,
        .level = 30,
        .formations = {471, 472, 473, 474, 475, 476, 477, 478},
        .weights = {2, 2, 2, 2, 4, 4, 4, 4},
    },
    // Treasure Isle: SceneId_TreasureIsleEntrance, SceneId_TreasureIsleChests, SceneId_TreasureIsleJestersArmletChest
    {
        .rate = 50,
        .level = 29,
        .formations = {551, 552, 553, 554, 555, 556, 557, 558},
        .weights = {10, 10, 10, 10, 10, 10, 10, 10},
    },
    // Shaman Village Cave: SceneId_ShamanVillageCave
    {
        .rate = 50,
        .level = 29,
        .formations = {561, 562, 563, 564, 565, 566, 567, 568},
        .weights = {10, 10, 10, 10, 10, 10, 10, 10},
    },
    // Jupiter Lighthouse: SceneId_JupiterLighthouseBasementTeleporters, SceneId_JupiterLighthouseRoomGateButton, SceneId_JupiterLighthouseRoomBeam, SceneId_JupiterLighthouseRoomErinyesTunicChest entrances 3, 4, 8 and 9, SceneId_JupiterLighthouseRoomLockedDoors, its other entrances
    {
        .rate = 55,
        .level = 30,
        .formations = {481, 482, 483, 484, 485, 486, 487, 488},
        .weights = {20, 20, 10, 10, 5},
    },
    // Jupiter Lighthouse: SceneId_JupiterLighthouseRoomErinyesTunicChest entrances 3, 4, 8 and 9, SceneId_JupiterLighthouseRoomLockedDoors entrances 5 and 7, SceneId_JupiterLighthouseRoomRedKey, SceneId_JupiterLighthouseRoomCrackedFloor, SceneId_JupiterLighthouseRoomBlueKey, SceneId_JupiterLighthouseRoomM
    {
        .rate = 55,
        .level = 30,
        .formations = {481, 482, 483, 484, 485, 486, 487, 488},
        .weights = {10, 10, 20, 20, 20, 5, 5},
    },
    // Jupiter Lighthouse: SceneId_JupiterLighthouseWhorl, SceneId_JupiterLighthouseRoomPhaetonsBladeChest
    {
        .rate = 55,
        .level = 30,
        .formations = {481, 482, 483, 484, 485, 486, 487, 488},
        .weights = {5, 5, 5, 5, 10, 10, 20, 20},
    },
    // Trial Road: SceneId_TrialRoadCaveRock, SceneId_TrialRoadGasp
    {
        .rate = 50,
        .level = 30,
        .formations = {571, 572, 573, 574, 575, 576, 577, 578},
        .weights = {2, 2, 2, 2, 4, 4, 4, 4},
    },
    // Magma Rock: SceneId_MagmaRockSlide, SceneId_MagmaRockSalamanderTailChest entrances 13 to 17
    {
        .rate = 55,
        .level = 31,
        .formations = {491, 492, 493, 494, 495, 496, 497, 498},
        .weights = {20, 20, 10, 5, 5},
    },
    // Magma Rock: SceneId_MagmaRockSalamanderTailChest, its other entrances, SceneId_MagmaRockStatue
    {
        .rate = 55,
        .level = 31,
        .formations = {491, 492, 493, 494, 495, 496, 497, 498},
        .weights = {10, 20, 20, 5, 5, 5},
    },
    // Magma Rock: SceneId_MagmaRockInsideLuckyMedalChest, SceneId_MagmaRockInsideEntrance
    {
        .rate = 55,
        .level = 31,
        .formations = {491, 492, 493, 494, 495, 496, 497, 498},
        .weights = {10, 15, 15, 5, 5, 5, 10},
    },
    // Magma Rock: SceneId_MagmaRockInsideFury, SceneId_MagmaRockInsideCrackedPillars entrances 1 to 7, SceneId_MagmaRockInsideGolemCoreChest
    {
        .rate = 55,
        .level = 32,
        .formations = {491, 492, 493, 494, 495, 496, 497, 498},
        .weights = {5, 5, 10, 10, 5, 2, 20, 5},
    },
    // Magma Rock: SceneId_MagmaRockInsideCrackedPillars, its other entrances, SceneId_MagmaRockInsideSalamanderTailChest, SceneId_MagmaRockInsideEmberStone, SceneId_MagmaRockInsideBlazeTablet
    {
        .rate = 55,
        .level = 32,
        .formations = {491, 492, 493, 494, 495, 496, 497, 498},
        .weights = {5, 5, 5, 10, 5, 5, 20, 20},
    },
    // Northern Reaches: SceneId_NorthernReaches
    {
        .rate = 40,
        .level = 32,
        .formations = {581, 582, 583, 584, 585, 586, 587, 588},
        .weights = {10, 10, 10, 10, 10, 3, 3, 3},
    },
    // Mars Lighthouse: SceneId_MarsLighthouseEntranceFugue, SceneId_MarsLighthouseEntranceTeleportLapis, SceneId_MarsLighthouseEntranceBalm, SceneId_MarsLighthouseEntranceHiddenCircle
    {
        .rate = 50,
        .level = 33,
        .formations = {501, 502, 503, 504, 505, 506, 507, 508},
        .weights = {20, 5, 20, 5, 5, 5},
    },
    // Mars Lighthouse: SceneId_MarsLighthouseEntranceValkyrieMailChest, SceneId_MarsLighthouseEntranceSolBladeChest, SceneId_MarsLighthouseStarRoom, SceneId_MarsLighthouseRoomMInscriptions
    {
        .rate = 50,
        .level = 33,
        .formations = {501, 502, 503, 504, 505, 506, 507, 508},
        .weights = {10, 5, 15, 20, 5, 10, 5, 5},
    },
    // Mars Lighthouse: SceneId_MarsLighthouseMercuryLedge, SceneId_MarsLighthouseJupiterLedge, SceneId_MarsLighthouseRoomMAlastorsHoodChest, SceneId_MarsLighthouseVenusLedge, SceneId_MarsLighthouseMarsTower, SceneId_MarsLighthouseMercuryTower, SceneId_MarsLighthouseJupiterTower, SceneId_MarsLighthouseVenusTower
    {
        .rate = 50,
        .level = 33,
        .formations = {501, 502, 503, 504, 505, 506, 507, 508},
        .weights = {5, 5, 5, 10, 5, 20, 20, 20},
    },
    // Treasure Isle: SceneId_TreasureIsleGale
    {
        .rate = 45,
        .level = 35,
        .formations = {591, 592, 593, 594, 595, 596, 597, 598},
        .weights = {10, 10, 10, 10, 10, 10, 10, 10},
    },
    // Yampi Desert Cave: SceneId_YampiDesertCaveWaterOfLifeChest, SceneId_YampiDesertCaveGuardian, its other entrances
    {
        .rate = 45,
        .level = 37,
        .formations = {601, 602, 603, 604, 605, 606, 607, 608},
        .weights = {20, 20, 20, 10, 10, 5, 5, 5},
    },
    // Yampi Desert Cave: SceneId_YampiDesertCaveGuardian entrances 15 to 17, SceneId_YampiDesertCaveCrystal
    {
        .rate = 45,
        .level = 37,
        .formations = {601, 602, 603, 604, 605, 606, 607, 608},
        .weights = {5, 5, 5, 10, 10, 20, 20, 20},
    },
    // Islet Cave: SceneId_IsletCaveCorridors
    {
        .rate = 15,
        .level = 38,
        .formations = {611, 612, 613, 614, 615, 616, 617, 618},
        .weights = {20, 20, 5, 5, 10, 10, 10, 10},
    },
    // Islet Cave: SceneId_IsletCaveCorridors
    {
        .rate = 15,
        .level = 38,
        .formations = {611, 612, 613, 614, 615, 616, 617, 618},
        .weights = {10, 10, 10, 10, 20, 20, 20, 20},
    },
    // Anemos Inner Sanctum: SceneId_AnemosInnerSanctumElevator, SceneId_AnemosInnerSanctumSwitches, SceneId_AnemosInnerSanctumOrihalconChest entrances 1 and 2
    {
        .rate = 35,
        .level = 42,
        .formations = {621, 622, 623, 624, 625, 626, 627, 628},
        .weights = {20, 20, 20, 20, 10, 10, 10, 10},
    },
    // Anemos Inner Sanctum: SceneId_AnemosInnerSanctumOrihalconChest, its other entrances, SceneId_AnemosInnerSanctumGuardian
    {
        .rate = 35,
        .level = 42,
        .formations = {621, 622, 623, 624, 625, 626, 627, 628},
        .weights = {10, 10, 10, 10, 20, 20, 20, 20},
    },
    // ENCOUNTER_ZONE_SCRIPTED
    {
        .formations = {77, FORMATION_DOOM_DRAGON_A, 635, 636,
                       FORMATION_KARST_AND_AGATIO_VS_ISAAC},
        .weights = {10, 10, 10, 10, 10},
    },
    // ENCOUNTER_ZONE_HIDDEN_BOSSES
    {
        .formations = {631, 632, 633, 634},
        .weights = {10, 10, 10, 10},
    },
    // The Mimics' battles, slots 0 to 7 (ENCOUNTER_ZONE_MIMIC)
    {
        .formations = {651, 652, 653, 654, 655, 656, 657, 658},
        .weights = {10, 10, 10, 10, 10, 10, 10, 10},
    },
    // The Mimics' battles, slot 8 on
    {
        .formations = {659},
        .weights = {10},
    },
    // ENCOUNTER_ZONE_MAD_PLANTS
    {
        .formations = {641, 642, 643, 644, 645},
        .weights = {10, 10, 10, 10, 10},
    },
    // World Map, encounter region 4: snowfield and snow forest
    {
        .rate = 35,
        .level = 27,
        .formations = {451, 452, 453, 454, 455, 456, 457, 458},
        .weights = {20, 20, 20, 10},
    },
    // Zone 108: no encounter rule or scene names it
    {
        .rate = 35,
        .level = 34,
        .formations = {495, 496, 497, 498, 501, 502, 503, 504},
        .weights = {5, 3, 15, 15, 15, 3, 15, 15},
    },
    // Zone 109: no encounter rule or scene names it
    {
        .rate = 10,
        .level = 34,
        .formations = {495, 496, 497, 498, 501, 502, 503, 504},
        .weights = {5, 3, 15, 15, 15, 3, 15, 15},
    },
};
// clang-format on
