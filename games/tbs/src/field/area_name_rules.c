/// @file
/// The area each scene is named for (struct AreaNameRule): the first rule that
/// matches the scene or its area, and the entrance, gives the area name.

#include "edition.h"
#include "types.h"
#include "field_sequence_internal.h"

/// Each row: {scene or area, entrance (-1 for any), 1 for a scene and 0 for an
/// area, area name}; id -1 ends the table, and a row of zeros follows the end.
/// A scene's number cannot be a symbol in a halfword, so a scene's row names it
/// in a comment.
// clang-format off
const struct AreaNameRule gArea_NameRules[] = {
    {0, -1, 0, 0},
    {1, -1, 0, 1},
    {2, -1, 0, 2},
    {9, 1, 1, 3}, // SCENE_SANCTUM
    {9, 2, 1, 3}, // SCENE_SANCTUM
    {9, 20, 1, 3}, // SCENE_SANCTUM
    {9, 21, 1, 3}, // SCENE_SANCTUM
    {9, 12, 1, 4}, // SCENE_SANCTUM
    {9, 11, 1, 4}, // SCENE_SANCTUM
    {9, 10, 1, 4}, // SCENE_SANCTUM
    {9, 35, 1, 5}, // SCENE_SANCTUM
    {9, 32, 1, 6}, // SCENE_SANCTUM
    {9, 29, 1, 7}, // SCENE_SANCTUM
    {9, -1, 1, 8}, // SCENE_SANCTUM
    {8, 19, 1, 9}, // SCENE_COTTAGE_ROOMS_B
    {3, -1, 0, 10},
    {19, -1, 1, 11}, // SCENE_SOL_ENTRANCE
    {17, -1, 1, 12}, // SCENE_SOL_SANCTUM_ELEMENTAL_STAR_ROOM
    {18, -1, 1, 12}, // SCENE_SOL_SANCTUM_ERUPTION
    {4, -1, 0, 13},
    {105, 7, 1, 14}, // SCENE_ROCK_ROOMS
    {105, 11, 1, 14}, // SCENE_ROCK_ROOMS
    {22, 2, 1, 15}, // SCENE_STONE_ROOMS
    {5, -1, 0, 16},
    {6, -1, 0, 17},
    {7, -1, 0, 18},
    {32, -1, 1, 19}, // SCENE_BILIBIN_TREASURY
    {8, -1, 0, 20},
    {34, -1, 1, 21}, // SCENE_MC_COY_PALACE_GROUNDS
    {9, -1, 0, 21},
    {10, -1, 0, 22},
    {38, -1, 1, 23}, // SCENE_KOLIMA_TUNNEL
    {11, -1, 0, 24},
    {12, -1, 0, 25},
    {13, -1, 0, 26},
    {14, -1, 0, 27},
    {49, -1, 1, 28}, // SCENE_BILIBIN_CAVE_ENTRANCE
    {15, -1, 0, 29},
    {51, 7, 1, 30}, // SCENE_ICE_ROOMS
    {17, -1, 0, 31},
    {18, -1, 0, 32},
    {58, -1, 1, 33}, // SCENE_MERCURY_LIGHTHOUSE_AERIE
    {19, -1, 0, 34},
    {61, 2, 1, 35}, // SCENE_LACQUER_ROOMS_A
    {61, 3, 1, 36}, // SCENE_LACQUER_ROOMS_A
    {20, -1, 0, 37},
    {21, -1, 0, 38},
    {22, -1, 0, 39},
    {23, -1, 0, 40},
    {24, -1, 0, 41},
#if LANGUAGE != LANGUAGE_JAPANESE
    {80, 6, 1, 41}, // SCENE_ALTIN_PEAK_SOUTH_ENTRANCE
    {80, 7, 1, 41}, // SCENE_ALTIN_PEAK_SOUTH_ENTRANCE
#endif
    {25, -1, 0, 42},
    {26, -1, 0, 43},
    {27, -1, 0, 44},
    {28, -1, 0, 45},
    {29, -1, 0, 46},
    {30, -1, 0, 47},
    {31, -1, 0, 48},
    {101, -1, 1, 49}, // SCENE_KALAY_SANCTUM
    {32, -1, 0, 50},
    {103, -1, 1, 51}, // SCENE_HAMMET_PALACE
    {33, -1, 0, 51},
    {105, 10, 1, 52}, // SCENE_ROCK_ROOMS
    {34, -1, 0, 53},
    {35, -1, 0, 54},
    {108, 2, 1, 55}, // SCENE_COTTAGE_ROOMS_E
    {36, -1, 0, 56},
    {37, -1, 0, 57},
    {38, -1, 0, 58},
    {113, -1, 1, 59}, // SCENE_CROSSBONE_ISLE
    {125, -1, 1, 60}, // SCENE_PIRATE_SHIP
    {39, -1, 0, 61},
    {136, 8, 1, 62}, // SCENE_STUCCO_ROOMS_B
    {136, 30, 1, 62}, // SCENE_STUCCO_ROOMS_B
    {40, -1, 0, 63},
    {139, -1, 1, 64}, // SCENE_BABI_PALACE_ENTRANCE
    {138, -1, 1, 64}, // SCENE_BABI_PALACE
    {141, -1, 1, 65}, // SCENE_COLOSSEUM_TUNNEL
    {142, -1, 1, 66}, // SCENE_COLOSSEUM_ENTRANCE
    {140, 2, 1, 67}, // SCENE_COLOSSEUM
    {140, 4, 1, 67}, // SCENE_COLOSSEUM
    {140, -1, 1, 68}, // SCENE_COLOSSEUM
    {143, -1, 1, 69}, // SCENE_COLOSSEUM_AZART_MATCH
    {144, -1, 1, 70}, // SCENE_COLOSSEUM_SATRAGE_MATCH
    {145, -1, 1, 71}, // SCENE_COLOSSEUM_NAVAMPA_MATCH
    {43, -1, 0, 72},
    {44, -1, 0, 73},
    {45, -1, 0, 74},
    {46, -1, 0, 75},
    {47, -1, 0, 76},
    {48, -1, 0, 77},
    {49, -1, 0, 78},
    {50, -1, 0, 79},
    {168, 2, 1, 80}, // SCENE_TIMBER_ROOMS_A
    {168, 6, 1, 80}, // SCENE_TIMBER_ROOMS_A
    {51, -1, 0, 81},
    {171, -1, 1, 82}, // SCENE_GATEWAY_CAVE
    {52, -1, 0, 83},
    {173, 1, 1, 84}, // SCENE_TUNNEL_RUINS_LADY_STATUES
    {173, 2, 1, 84}, // SCENE_TUNNEL_RUINS_LADY_STATUES
    {173, 3, 1, 84}, // SCENE_TUNNEL_RUINS_LADY_STATUES
    {53, -1, 0, 85},
    {176, -1, 1, 86}, // SCENE_BABI_LIGHTHOUSE_ENTRY
    {177, -1, 1, 87}, // SCENE_ROAD_TO_BABI_LIGHTHOUSE
    {54, -1, 0, 88},
    {179, 11, 1, 89}, // SCENE_TIMBER_ROOMS_B
    {179, 12, 1, 89}, // SCENE_TIMBER_ROOMS_B
    {55, -1, 0, 90},
    {182, -1, 1, 91}, // SCENE_VENUS_LIGHTHOUSE_GROUND_FLOOR
    {56, -1, 0, 92},
    {187, -1, 1, 93}, // SCENE_VENUS_LIGHTHOUSE_AERIE
    {57, -1, 0, 94},
    {58, -1, 0, 95},
    {59, -1, 0, 96},
    {190, 1, 1, 97}, // SCENE_BATTLE_ARENA
    {61, -1, 0, 98},
    {62, -1, 0, 99},
    {63, -1, 0, 100},
    {64, -1, 0, 101},
    {65, -1, 0, 102},
    {66, -1, 0, 103},
    {67, -1, 0, 104},
    {68, -1, 0, 105},
    {69, -1, 0, 106},
    {70, -1, 0, 107},
    {-1, -1, 1, -1},
    {0, 0, 0, 0},
};
