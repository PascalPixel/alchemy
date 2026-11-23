/// @file
/// Unused: a copy of the speakers' portrait maps, linked among the battle code
/// and read by nothing. The message window looks portraits up in its own
/// gPortrait_CharacterMap and gPortrait_SpriteMap, which hold the same pairs.

#include "sprite_ids.h"
#include "types.h"
#include "game/character_ids.h"

// clang-format off
/// {character, portrait} pairs, ending in -1, as in gPortrait_CharacterMap.
const s16 gBattle_UnusedCharacterPortraits[][2] = {
    {ISAAC, 0},
    {GARET, 1},
    {IVAN, 2},
    {MIA, 3},
    {FELIX, 4},
    {JENNA, 5},
    {SHEBA, 6},
    {PIERS, 8},
    {-1, -1},
};

/// {sprite, portrait} pairs, ending in -1, as in gPortrait_SpriteMap: the
/// Djinn, shopkeepers, townspeople and named people who speak with a portrait,
/// each portrait counted from PORTRAIT_FIRST_SPRITE_PORTRAIT.
const s16 gBattle_UnusedSpritePortraits[][2] = {
    {SPRITE_JUPITER_DJINNI, 0},
    {SPRITE_VENUS_DJINNI, 1},
    {SPRITE_MARS_DJINNI, 2},
    {SPRITE_MERCURY_DJINNI, 3},
    {SPRITE_HEALER, 14},
    {SPRITE_BEARDED_MAN_IN_LONG_ROBE, 15},
    {SPRITE_BALD_MAN_IN_RED, 16},
    {SPRITE_YOUNG_MAN_IN_GREEN, 17},
    {SPRITE_WOMAN_WITH_PONYTAIL, 18},
    {SPRITE_WOMAN_IN_PINK_APRON, 19},
    {SPRITE_MAN_WITH_DARK_HAIR, 20},
    {SPRITE_WEAPON_MERCHANT, 54},
    {SPRITE_ARMOR_MERCHANT, 55},
    {SPRITE_WOMAN_WITH_TEAL_HEADBAND, 56},
    {SPRITE_INNKEEPER, 57},
    {SPRITE_INN_WORKER, 58},
    {SPRITE_WOMAN_WITH_ORANGE_HAIR, 59},
    {SPRITE_ARTIFACT_MERCHANT, 60},
    {SPRITE_KRADEN, 4},
    {SPRITE_KRADEN_SMALL, 4},
    {SPRITE_ALEX, 7},
    {SPRITE_POI, 30},
    {SPRITE_RIKI, 31},
    {SPRITE_TAVI, 32},
    {SPRITE_MADRAS_MAYOR, 33},
    {SPRITE_MADRAS_ELDER, 34},
    {SPRITE_ALHAFRAS_MAYOR, 35},
    {SPRITE_FORTUNE_TELLER, 36},
    {SPRITE_AKAFUBU, 37},
    {SPRITE_AKAFUBU_WITCH_DOCTOR, 37},
    {SPRITE_MOAPA, 38},
    {SPRITE_MAHA, 39},
    {SPRITE_JENNAS_FATHER, 40},
    {SPRITE_JENNAS_MOTHER, 41},
    {SPRITE_BRIGGS, 42},
    {SPRITE_CHAUCHA, 43},
    {SPRITE_EOLEO, 44},
    {SPRITE_OBABA, 45},
    {SPRITE_SUSA, 46},
    {SPRITE_LADY_UZUME, 47},
    {SPRITE_KUSHINADA, 48},
    {SPRITE_HYDROS, 49},
    {SPRITE_CONSERVATO, 50},
    {SPRITE_KARST, 51},
    {SPRITE_AGATIO, 52},
    {SPRITE_KYLE, 9},
    {SPRITE_LUNPA, 61},
    {SPRITE_PUELLE, 62},
    {SPRITE_MASKED_SHAMAN, 63},
    {SPRITE_PROX_ELDER, 64},
    {SPRITE_SUNSHINE, 65},
    {SPRITE_MAN_WITH_ORANGE_BEARD, 66},
    {SPRITE_GIRL_WITH_LONG_BLONDE_HAIR, 67},
    {SPRITE_IZUMO_WOMAN_IN_CREAM, 68},
    {SPRITE_IZUMO_MAN_IN_RED, 69},
    {SPRITE_IZUMO_WOMAN_IN_ORANGE, 70},
    {SPRITE_FEIZHI, 73},
    {SPRITE_HAMMA, 74},
    {SPRITE_DORA, 8},
    {SPRITE_WOMAN_IN_GREEN_HEADSCARF, 10},
    {SPRITE_MAN_IN_TEAL_SHIRT, 13},
    {SPRITE_YOUNG_MAN_WITH_BLUE_HAIR, 71},
    {SPRITE_YOUNG_MAN_WITH_RED_HAIR, 72},
    {SPRITE_WOMAN_WITH_ORANGE_PONYTAIL, 11},
    {SPRITE_BOY_WITH_ORANGE_HAIR, 76},
    {-1, -1},
};
// clang-format on
