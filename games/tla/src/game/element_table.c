/// @file
/// The element tables: class series by element, element power and resistance
/// by level, the enemies' element presets, the power and resistance curve, and
/// the classes two ability effects look for.
// Part of game.c.

#include "game_internal.h"

/// The class series by a character's strongest and second element, read as
/// [strongest][second]; The Broken Seal's gClass_SeriesByElements.
const s32 gClass_SeriesByElements[4][4] = {
    {1, 3, 4, 5},
    {8, 6, 9, 10},
    {4, 3, 2, 5},
    {8, 10, 9, 7},
};

/// The elemental power and resistance of each element level, 0 to 15; The
/// Broken Seal's gElement_PowerResistByLevel.
const struct ElementLevelBonus gElement_PowerResistByLevel[16] = {
    {75, 85},   {80, 90},   {85, 95},   {90, 100},  {95, 105},  {100, 110},
    {105, 115}, {110, 120}, {115, 125}, {120, 130}, {125, 135}, {130, 140},
    {135, 145}, {140, 150}, {145, 160}, {150, 165},
};

// clang-format off
/// The 48 enemy element presets, by EnemyDefinition.elementPreset; each is
/// named by the enemies that have it, in the English release's words.
const struct EnemyElementPreset gEnemy_ElementPresetTable[] = {
    {
        .first = ELEMENT_NONE,
        .elementLevels = {1, 1, 1, 1},
        .stats = {
            {.power = 100, .resistance = 100},
            {.power = 100, .resistance = 100},
            {.power = 100, .resistance = 100},
            {.power = 100, .resistance = 100},
        },
    },
    {
        .first = ELEMENT_NONE,
        .elementLevels = {1, 1, 1, 1},
        .stats = {
            {.power = 80, .resistance = 72},
            {.power = 80, .resistance = 72},
            {.power = 80, .resistance = 72},
            {.power = 80, .resistance = 72},
        },
    },
    {
        .first = ELEMENT_NONE,
        .elementLevels = {1, 1, 1, 1},
        .stats = {
            {.power = 80, .resistance = 90},
            {.power = 80, .resistance = 90},
            {.power = 80, .resistance = 90},
            {.power = 80, .resistance = 90},
        },
    },
    {
        .first = ELEMENT_NONE,
        .elementLevels = {1},
        .stats = {
            {.power = 100, .resistance = 48},
            {.power = 80, .resistance = 48},
            {.power = 70, .resistance = 25},
            {.power = 80, .resistance = 72},
        },
    },
    {
        .first = ELEMENT_NONE,
        .elementLevels = {1},
        .stats = {
            {.power = 100, .resistance = 72},
            {.power = 80, .resistance = 72},
            {.power = 70, .resistance = 25},
            {.power = 80, .resistance = 127},
        },
    },
    {
        .first = ELEMENT_NONE,
        .elementLevels = {1},
        .stats = {
            {.power = 95, .resistance = 48},
            {.power = 85, .resistance = 48},
            {.power = 90, .resistance = 25},
            {.power = 85, .resistance = 48},
        },
    },
    {
        .first = ELEMENT_NONE,
        .elementLevels = {1},
        .stats = {
            {.power = 95, .resistance = 72},
            {.power = 85, .resistance = 72},
            {.power = 90, .resistance = 48},
            {.power = 85, .resistance = 100},
        },
    },
    {
        .first = ELEMENT_NONE,
        .elementLevels = {2, 1},
        .stats = {
            {.power = 105, .resistance = 48},
            {.power = 100, .resistance = 100},
            {.power = 80, .resistance = 7},
            {.power = 70, .resistance = 72},
        },
    },
    {
        .first = ELEMENT_NONE,
        .elementLevels = {2, 1},
        .stats = {
            {.power = 105, .resistance = 72},
            {.power = 100, .resistance = 175},
            {.power = 80, .resistance = 25},
            {.power = 70, .resistance = 100},
        },
    },
    {
        .first = ELEMENT_NONE,
        .elementLevels = {1, 0, 0, 2},
        .stats = {
            {.power = 90, .resistance = 100},
            {.power = 90, .resistance = 48},
            {.power = 95, .resistance = 48},
            {.power = 105, .resistance = 7},
        },
    },
    {
        .first = ELEMENT_NONE,
        .elementLevels = {1, 0, 0, 2},
        .stats = {
            {.power = 90, .resistance = 152},
            {.power = 90, .resistance = 72},
            {.power = 95, .resistance = 72},
            {.power = 105, .resistance = 25},
        },
    },
    {
        .first = ELEMENT_NONE,
        .elementLevels = {1, 1, 1, 2},
        .stats = {
            {.power = 95, .resistance = 127},
            {.power = 95, .resistance = 72},
            {.power = 95, .resistance = 48},
            {.power = 110, .resistance = 7},
        },
    },
    {
        .first = ELEMENT_NONE,
        .elementLevels = {1, 1, 1, 2},
        .stats = {
            {.power = 95, .resistance = 175},
            {.power = 95, .resistance = 100},
            {.power = 95, .resistance = 72},
            {.power = 110, .resistance = 25},
        },
    },
    {
        .first = ELEMENT_NONE,
        .elementLevels = {2, 1},
        .stats = {
            {.power = 100, .resistance = 25},
            {.power = 95, .resistance = 72},
            {.power = 80, .resistance = 7},
            {.power = 80, .resistance = 48},
        },
    },
    {
        .first = ELEMENT_NONE,
        .elementLevels = {2, 1},
        .stats = {
            {.power = 100, .resistance = 25},
            {.power = 95, .resistance = 127},
            {.power = 80, .resistance = 72},
            {.power = 80, .resistance = 48},
        },
    },
    {
        .first = ELEMENT_NONE,
        .elementLevels = {0, 1, 1, 2},
        .stats = {
            {.power = 100, .resistance = 48},
            {.power = 95, .resistance = 100},
            {.power = 95, .resistance = 72},
            {.power = 95, .resistance = 7},
        },
    },
    {
        .first = ELEMENT_NONE,
        .elementLevels = {0, 1, 1, 2},
        .stats = {
            {.power = 100, .resistance = 72},
            {.power = 95, .resistance = 152},
            {.power = 95, .resistance = 100},
            {.power = 95, .resistance = 25},
        },
    },
    {
        .first = ELEMENT_NONE,
        .elementLevels = {2, 1, 1, 1},
        .stats = {
            {.power = 100, .resistance = 48},
            {.power = 100, .resistance = 100},
            {.power = 100, .resistance = 100},
            {.power = 95, .resistance = 127},
        },
    },
    {
        .first = ELEMENT_NONE,
        .elementLevels = {4, 2, 2},
        .stats = {
            {.power = 110, .resistance = 152},
            {.power = 95, .resistance = 100},
            {.power = 95, .resistance = 100},
            {.power = 90, .resistance = 48},
        },
    },
    {
        .first = ELEMENT_NONE,
        .elementLevels = {2, 4, 0, 2},
        .stats = {
            {.power = 95, .resistance = 100},
            {.power = 115, .resistance = 175},
            {.power = 85, .resistance = 25},
            {.power = 95, .resistance = 100},
        },
    },
    {
        .first = ELEMENT_NONE,
        .elementLevels = {2, 0, 4, 2},
        .stats = {
            {.power = 95, .resistance = 100},
            {.power = 85, .resistance = 25},
            {.power = 115, .resistance = 175},
            {.power = 95, .resistance = 100},
        },
    },
    {
        .first = ELEMENT_NONE,
        .elementLevels = {0, 2, 2, 4},
        .stats = {
            {.power = 90, .resistance = 48},
            {.power = 95, .resistance = 100},
            {.power = 95, .resistance = 100},
            {.power = 110, .resistance = 152},
        },
    },
    {
        .elementLevels = {6, 2, 2},
        .stats = {
            {.power = 120, .resistance = 100},
            {.power = 95, .resistance = 48},
            {.power = 95, .resistance = 48},
            {.power = 80, .resistance = 7},
        },
    },
    {
        .elementLevels = {6, 2, 2},
        .stats = {
            {.power = 120, .resistance = 193},
            {.power = 95, .resistance = 100},
            {.power = 95, .resistance = 100},
            {.power = 80, .resistance = 25},
        },
    },
    {
        .first = ELEMENT_MERCURY,
        .elementLevels = {2, 6, 0, 2},
        .stats = {
            {.power = 95, .resistance = 48},
            {.power = 125, .resistance = 127},
            {.power = 75, .resistance = 7},
            {.power = 95, .resistance = 48},
        },
    },
    {
        .first = ELEMENT_MERCURY,
        .elementLevels = {2, 6, 0, 2},
        .stats = {
            {.power = 95, .resistance = 100},
            {.power = 125, .resistance = 193},
            {.power = 75, .resistance = 25},
            {.power = 95, .resistance = 100},
        },
    },
    {
        .first = ELEMENT_MARS,
        .elementLevels = {2, 0, 6, 2},
        .stats = {
            {.power = 95, .resistance = 48},
            {.power = 75, .resistance = 7},
            {.power = 125, .resistance = 127},
            {.power = 95, .resistance = 48},
        },
    },
    {
        .first = ELEMENT_MARS,
        .elementLevels = {2, 0, 6, 2},
        .stats = {
            {.power = 95, .resistance = 100},
            {.power = 75, .resistance = 25},
            {.power = 125, .resistance = 193},
            {.power = 95, .resistance = 100},
        },
    },
    {
        .first = ELEMENT_JUPITER,
        .elementLevels = {0, 2, 2, 6},
        .stats = {
            {.power = 80, .resistance = 7},
            {.power = 95, .resistance = 48},
            {.power = 95, .resistance = 48},
            {.power = 120, .resistance = 127},
        },
    },
    {
        .first = ELEMENT_JUPITER,
        .elementLevels = {0, 2, 2, 6},
        .stats = {
            {.power = 80, .resistance = 25},
            {.power = 95, .resistance = 100},
            {.power = 95, .resistance = 100},
            {.power = 120, .resistance = 193},
        },
    },
    {
        .first = ELEMENT_NONE,
        .elementLevels = {3, 2, 2, 2},
        .stats = {
            {.power = 100, .resistance = 100},
            {.power = 85, .resistance = 72},
            {.power = 85, .resistance = 72},
            {.power = 85, .resistance = 48},
        },
    },
    {
        .first = ELEMENT_NONE,
        .elementLevels = {3, 2, 2, 2},
        .stats = {
            {.power = 100, .resistance = 127},
            {.power = 85, .resistance = 100},
            {.power = 85, .resistance = 100},
            {.power = 85, .resistance = 72},
        },
    },
    {
        .first = ELEMENT_NONE,
        .elementLevels = {2, 3, 2, 2},
        .stats = {
            {.power = 85, .resistance = 72},
            {.power = 100, .resistance = 100},
            {.power = 85, .resistance = 48},
            {.power = 85, .resistance = 72},
        },
    },
    {
        .first = ELEMENT_NONE,
        .elementLevels = {2, 3, 2, 2},
        .stats = {
            {.power = 85, .resistance = 100},
            {.power = 100, .resistance = 127},
            {.power = 85, .resistance = 72},
            {.power = 85, .resistance = 100},
        },
    },
    {
        .first = ELEMENT_NONE,
        .elementLevels = {2, 2, 3, 2},
        .stats = {
            {.power = 85, .resistance = 72},
            {.power = 85, .resistance = 48},
            {.power = 100, .resistance = 100},
            {.power = 85, .resistance = 72},
        },
    },
    {
        .first = ELEMENT_NONE,
        .elementLevels = {2, 2, 3, 2},
        .stats = {
            {.power = 85, .resistance = 100},
            {.power = 85, .resistance = 72},
            {.power = 100, .resistance = 127},
            {.power = 85, .resistance = 100},
        },
    },
    {
        .first = ELEMENT_NONE,
        .elementLevels = {2, 2, 2, 3},
        .stats = {
            {.power = 85, .resistance = 48},
            {.power = 85, .resistance = 72},
            {.power = 85, .resistance = 72},
            {.power = 100, .resistance = 100},
        },
    },
    {
        .first = ELEMENT_NONE,
        .elementLevels = {2, 2, 2, 3},
        .stats = {
            {.power = 85, .resistance = 72},
            {.power = 85, .resistance = 100},
            {.power = 85, .resistance = 100},
            {.power = 100, .resistance = 127},
        },
    },
    {
        .first = ELEMENT_NONE,
        .elementLevels = {3, 3, 0, 4},
        .stats = {
            {.power = 95, .resistance = 100},
            {.power = 100, .resistance = 127},
            {.power = 75, .resistance = 25},
            {.power = 105, .resistance = 152},
        },
    },
    {
        .first = ELEMENT_NONE,
        .elementLevels = {7, 5, 10, 7},
        .stats = {
            {.power = 100, .resistance = 100},
            {.power = 90, .resistance = 72},
            {.power = 110, .resistance = 175},
            {.power = 100, .resistance = 127},
        },
    },
    {
        .first = ELEMENT_NONE,
        .elementLevels = {8, 6, 11, 8},
        .stats = {
            {.power = 100, .resistance = 127},
            {.power = 95, .resistance = 72},
            {.power = 105, .resistance = 175},
            {.power = 100, .resistance = 100},
        },
    },
    {
        .first = ELEMENT_MARS,
        .elementLevels = {9, 7, 14, 9},
        .stats = {
            {.power = 100, .resistance = 127},
            {.power = 95, .resistance = 72},
            {.power = 110, .resistance = 175},
            {.power = 100, .resistance = 127},
        },
    },
    {
        .first = ELEMENT_NONE,
        .elementLevels = {4, 8, 6, 5},
        .stats = {
            {.power = 100, .resistance = 72},
            {.power = 130, .resistance = 193},
            {.power = 120, .resistance = 127},
            {.power = 110, .resistance = 152},
        },
    },
    {
        .first = ELEMENT_NONE,
        .elementLevels = {0, 2, 1, 4},
        .stats = {
            {.power = 90, .resistance = 152},
            {.power = 95, .resistance = 110},
            {.power = 95, .resistance = 72},
            {.power = 110, .resistance = 48},
        },
    },
    {
        .first = ELEMENT_NONE,
        .elementLevels = {14, 16, 13, 15},
        .stats = {
            {.power = 90, .resistance = 130},
            {.power = 105, .resistance = 180},
            {.power = 85, .resistance = 72},
            {.power = 95, .resistance = 150},
        },
    },
    {
        .first = ELEMENT_NONE,
        .elementLevels = {11, 7, 14, 9},
        .stats = {
            {.power = 95, .resistance = 100},
            {.power = 85, .resistance = 60},
            {.power = 105, .resistance = 175},
            {.power = 95, .resistance = 100},
        },
    },
    {
        .first = ELEMENT_NONE,
        .elementLevels = {17, 14, 12, 10},
        .stats = {
            {.power = 110, .resistance = 150},
            {.power = 100, .resistance = 190},
            {.power = 100, .resistance = 130},
            {.power = 100, .resistance = 110},
        },
    },
    {
        .elementLevels = {18, 11, 15, 8},
        .stats = {
            {.power = 110, .resistance = 190},
            {.power = 100, .resistance = 130},
            {.power = 105, .resistance = 175},
            {.power = 95, .resistance = 98},
        },
    },
};
// clang-format on

/// The element power and resistance curve, the same in both games: pairs of
/// an input and its value in 256ths, from 200 down to -200, between which
/// Battle_GetElementMultiplier interpolates.
const s16 gElement_PowerResistCurve[5][2] = {
    {200, 256}, {100, 128}, {0, 0}, {-100, -128}, {-200, -256},
};

/// The dragons, by enemy id: the Hydras, Serpents, Dinos, Dragons, Lizards,
/// Chimeras and the Doom Dragon. Raiden's Wrath (the Cloud Brand's unleash)
/// takes effect only on these (Enemy_IsDragon).
const u16 gEnemy_DragonIds[DRAGON_ENEMY_COUNT] = {
    88,  89,  90,  91,  92,  93,  94,  95,  96,  97,  98,  99,
    100, 143, 144, 145, 149, 150, 151, 176, 177, 178, 185, 186,
    187, 191, 192, 193, 194, 195, 196, 299, 300, 301, 323, 324,
    325, 357, 369, 370, 371, 372, 373, 374, 375, 376, 377,
};

/// The sea creatures, by enemy id: the Cuttles, Jellies, Hydras, Poseidon,
/// the shells and urchins, the Mermen, the sea Dragons, the Seabirds, the
/// Lizard Men and the Kraken. The Trident's use takes effect only on these
/// (Enemy_IsSeaCreature).
const u16 gEnemy_SeaCreatureIds[SEA_CREATURE_ENEMY_COUNT] = {
    84,  85,  86,  87,  88,  89,  90,  101, 102, 103, 167, 168, 169, 170, 171,
    172, 173, 174, 175, 176, 177, 178, 179, 180, 181, 330, 331, 332, 348,
};
