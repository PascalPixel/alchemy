// Unused: two tables pairing a speaker with a portrait, which no code names. The window code's own,
// PortraitCharacterMap and PortraitSpriteMap, are the ones the game reads.

#include "game/character_ids.h"
#include "sprite_ids.h"
#include "types.h"

// clang-format off
// {character, portrait} pairs, as in PortraitCharacterMap.
const Sint16 BattleUnusedCharacterPortraits[][2] = {
    {SPRITE_ISAAC_CHILD, PORTRAIT_ISAAC_CHILD},
    {GARET, PORTRAIT_GARET},
    {SPRITE_GARET_CHILD, PORTRAIT_GARET_CHILD},
    {IVAN, PORTRAIT_IVAN},
    {MIA, PORTRAIT_MIA},
    {FELIX, PORTRAIT_FELIX},
    {JENNA, PORTRAIT_JENNA},
    {SHEBA, PORTRAIT_SHEBA},
    {-1, -1},
};

// {sprite, portrait} pairs, as in PortraitSpriteMap: portrait n is the row PORTRAIT_JUPITER_DJINNI + n.
const Sint16 BattleUnusedSpritePortraits[][2] = {
    {SPRITE_JUPITER_DJINNI, 0},
    {SPRITE_VENUS_DJINNI, 1},
    {SPRITE_MARS_DJINNI, 2},
    {SPRITE_MERCURY_DJINNI, 3},
    {SPRITE_KRADEN, 4},
    {SPRITE_SATUROS, 5},
    {SPRITE_MENARDI, 6},
    {SPRITE_ALEX, 7},
    {SPRITE_DORA, 8},
    {SPRITE_KYLE, 9},
    {SPRITE_WOMAN_IN_GREEN_HEADSCARF, 10},
    {SPRITE_WOMAN_WITH_ORANGE_PONYTAIL, 11},
    {SPRITE_BOY_WITH_ORANGE_HAIR, 12},
    {SPRITE_MAN_IN_TEAL_SHIRT, 13},
    {SPRITE_MCCOY, 14},
    {SPRITE_WOMAN_WITH_PURPLE_HAIR, 15},
    {SPRITE_TRET_RAISED, 16},
    {SPRITE_LAUREL, 17},
    {SPRITE_NYUNPA, 18},
    {SPRITE_HAMMET, 19},
    {SPRITE_LAYANA, 20},
    {SPRITE_FEH, 21},
    {SPRITE_FEIZHI, 22},
    {SPRITE_HSU, 23},
    {SPRITE_DONPA, 24},
    {SPRITE_DODONPA, 25},
    {SPRITE_BABI, 26},
    {SPRITE_IODEM, 27},
    {SPRITE_HAMA, 28},
    {SPRITE_KAJA, 29},
    {SPRITE_MAN_WITH_WHITE_BEARD_IN_GREEN, 30},
    {SPRITE_YOUNG_MAN_WITH_BLUE_HAIR, 31},
    {SPRITE_YOUNG_MAN_WITH_RED_HAIR, 32},
    {SPRITE_MAN_IN_TALL_HAT, 33},
    {SPRITE_BEARDED_MAN_IN_LONG_ROBE, 34},
    {SPRITE_BALD_MAN_IN_RED, 35},
    {SPRITE_YOUNG_MAN_IN_GREEN, 36},
    {SPRITE_WOMAN_WITH_PONYTAIL, 37},
    {SPRITE_WOMAN_IN_PINK_APRON, 38},
    {SPRITE_MAN_WITH_DARK_HAIR, 39},
    {SPRITE_ISAAC_CHILD, 40},
    {SPRITE_GARET_CHILD, 41},
    {SPRITE_FELIX_CHILD, 42},
    {SPRITE_JENNA_CHILD, 43},
    {SPRITE_FELIX_MASKED, 44},
    {SPRITE_TRET, 45},
    {SPRITE_FELIX, 46},
    {SPRITE_INVISIBLE_BABI, 47},
    {SPRITE_FARAN, 48},
    {-1, -1},
};
// clang-format on
