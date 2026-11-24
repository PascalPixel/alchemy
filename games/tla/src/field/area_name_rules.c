/// @file
/// The area each scene is named for (struct AreaNameRule): the first rule that
/// matches the scene or its area, and the entrance, gives the area name.

#include "edition.h"
#include "types.h"
#include "area_ids.h"
#include "field/field_sequence.h"
#include "field_sequence_internal.h"

/// Each row: {scene or area, entrance (-1 for any), 1 for a scene and 0 for an
/// area, area name}; id -1 ends the table, and a row of zeros follows the end.
/// A scene's number cannot be a symbol in a halfword, so a scene's row names it
/// in a comment.
// clang-format off
const struct AreaNameRule gArea_NameRules[] = {
    {AREA_TITLE_SCREEN, -1, 0, 0},
    {AREA_CLEAR_DATA, -1, 0, 1},
    {AREA_WORLD_MAP, -1, 0, 2},
    {AREA_STAFF_CREDIT, -1, 0, 3},
    {AREA_VENUS_LIGHTHOUSE, -1, 0, 4},
    {AREA_SUHALLA_GATE, -1, 0, 5},
    {AREA_IDEJIMA, -1, 0, 6},
    {11, 7, 1, 7}, // SCENE_DAILA_ROOMS_HOUSES
    {AREA_DAILA, -1, 0, 8},
    {AREA_KANDOREAN_TEMPLE, -1, 0, 9},
    {AREA_KANDOREAN_TEMPLE_GATE, -1, 0, 9},
    {AREA_SHRINE_OF_THE_SEA_GOD, -1, 0, 10},
    {AREA_DEHKAN_PLATEAU, -1, 0, 11},
    {AREA_MADRA, -1, 0, 12},
    {AREA_MADRA_CATACOMBS, -1, 0, 13},
    {AREA_MADRA_DRAWBRIDGE, -1, 0, 14},
    {AREA_OSENIA_CLIFFS, -1, 0, 15},
    {57, 5, 1, 16}, // SCENE_MIKASALLA_ROOMS_HOUSES
    {58, 4, 1, 17}, // SCENE_MIKASALLA_ROOMS_SANCTUM
    {58, 5, 1, 18}, // SCENE_MIKASALLA_ROOMS_SANCTUM
    {AREA_MIKASALLA, -1, 0, 19},
    {AREA_GAROH, -1, 0, 20},
    {AREA_YAMPI_DESERT, -1, 0, 21},
    {AREA_YAMPI_DESERT_CAVE, -1, 0, 22},
    {AREA_AIRS_ROCK, -1, 0, 23},
    {AREA_AIRS_ROCK_INSIDE, -1, 0, 23},
    {AREA_ALHAFRA, -1, 0, 24},
    {AREA_EASTERN_ALHAFRA, -1, 0, 25},
    {AREA_ALHAFRAN_CAVE, -1, 0, 26},
    {AREA_GONDOWAN_CLIFFS, -1, 0, 27},
    {AREA_NARIBWE, -1, 0, 28},
    {AREA_KIBOMBO_MOUNTAINS, -1, 0, 29},
    {AREA_KIBOMBO, -1, 0, 30},
    {AREA_KIBOMBO_CEREMONY, -1, 0, 30},
    {AREA_GABOMBA_STATUE, -1, 0, 31},
    {AREA_GABOMBA_CATACOMBS, -1, 0, 32},
    {AREA_LEMURIAN_SHIP, -1, 0, 33},
    {AREA_EAST_INDRA_SHORE, -1, 0, 34},
    {AREA_WEST_INDRA_ISLET, -1, 0, 35},
    {AREA_EAST_TUNDARIA_ISLET, -1, 0, 36},
    {AREA_NORTH_OSENIA_ISLET, -1, 0, 37},
    {AREA_SOUTHEAST_ANGARA_ISLET, -1, 0, 38},
    {AREA_SEA_OF_TIME_ISLET, -1, 0, 39},
    {AREA_YALLAM, -1, 0, 40},
    {AREA_TAOPO_SWAMP_ENTRANCE, -1, 0, 41},
    {AREA_TAOPO_SWAMP, -1, 0, 41},
    {156, 6, 1, 42}, // SCENE_APOJII_ISLANDS_SHOPS
    {AREA_APOJII_ISLANDS, -1, 0, 43},
    {AREA_APOJII_ISLANDS_CAVE, -1, 0, 44},
    {AREA_GAIA_ROCK, -1, 0, 45},
    {AREA_GAIA_ROCK_INSIDE, -1, 0, 45},
    {AREA_AQUA_ROCK, -1, 0, 46},
    {AREA_AQUA_ROCK_INSIDE, -1, 0, 46},
    {AREA_IZUMO, -1, 0, 47},
    {AREA_IZUMO_RUINS, -1, 0, 48},
    {183, 8, 1, 49}, // SCENE_CHAMPA_ROOMS
    {183, 9, 1, 50}, // SCENE_CHAMPA_ROOMS
    {AREA_CHAMPA, -1, 0, 51},
    {AREA_SEA_OF_TIME, -1, 0, 52},
    {AREA_ANKOHL_RUINS, -1, 0, 53},
    {201, 1, 1, 54}, // SCENE_ANCIENT_LEMURIA
    {AREA_LEMURIA_ENTRANCE, -1, 0, 55},
    {AREA_LEMURIA, -1, 0, 55},
    {AREA_TUNDARIA_TOWER, -1, 0, 56},
    {AREA_SOUTHWEST_ATTEKA_ISLET, -1, 0, 57},
    {AREA_GONDOWAN_SETTLEMENT, -1, 0, 58},
    {AREA_HESPERIA_SETTLEMENT, -1, 0, 59},
    {AREA_KALT_ISLAND, -1, 0, 60},
    {AREA_MAGMA_ROCK, -1, 0, 61},
    {AREA_MAGMA_ROCK_INSIDE, -1, 0, 61},
    {AREA_ATTEKA_INLET, -1, 0, 62},
    {AREA_SHAMAN_VILLAGE_CAVE, -1, 0, 63},
    {AREA_SHAMAN_VILLAGE, -1, 0, 64},
    {AREA_TRIAL_ROAD, -1, 0, 65},
    {AREA_TRIAL_ROAD_CAVE, -1, 0, 65},
    {237, 21, 1, 66}, // SCENE_CONTIGO_SHOPS
    {AREA_CONTIGO, -1, 0, 67},
    {AREA_ANEMOS_SANCTUM, -1, 0, 68},
    {AREA_JUPITER_LIGHTHOUSE, -1, 0, 69},
    {AREA_LOHO, -1, 0, 70},
    {AREA_NORTHERN_REACHES, -1, 0, 71},
    {AREA_PROX, -1, 0, 72},
    {AREA_INDRA_CAVERN, -1, 0, 73},
    {AREA_OSENIA_CAVERN, -1, 0, 74},
    {AREA_ANGARA_CAVERN, -1, 0, 75},
    {AREA_ATTEKA_CAVERN, -1, 0, 76},
    {AREA_ISLET_CAVE, -1, 0, 77},
    {AREA_MARS_LIGHTHOUSE, -1, 0, 78},
    {AREA_TREASURE_ISLE, -1, 0, 79},
    {AREA_ANEMOS_INNER_SANCTUM, -1, 0, 80},
    {AREA_MERCURY_LIGHTHOUSE, -1, 0, 81},
    {AREA_EPILOGUE, -1, 0, 82},
    {AREA_MOUNT_ALEPH_SUMMIT, -1, 0, 83},
    {AREA_LINK_LOBBY, -1, 0, 84},
    {AREA_TEST_0, -1, 0, 85},
    {AREA_TEST_1, -1, 0, 86},
    {AREA_TEST_2, -1, 0, 87},
    {AREA_TEST_3, -1, 0, 88},
    {AREA_TEST_4, -1, 0, 89},
    {AREA_TEST_5, -1, 0, 90},
    {AREA_TEST_6, -1, 0, 91},
    {AREA_TEST_7, -1, 0, 92},
    {321, 96, 1, 93}, // SCENE_ICON_TEST
    {321, 97, 1, 93}, // SCENE_ICON_TEST
    {321, 98, 1, 93}, // SCENE_ICON_TEST
    {321, 99, 1, 93}, // SCENE_ICON_TEST
    {AREA_ICON_TEST, -1, 0, 94},
    {AREA_PSYNERGY_TEST, -1, 0, 95},
    {AREA_PSYNERGY_TEST_2, -1, 0, 96},
    {AREA_SHOP_TEST, -1, 0, 97},
    {-1, -1, 1, -1},
    {0, 0, 0, 0},
};
