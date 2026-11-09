/// @file
/// The element tables: class series by element, element power and resistance
/// by level, the enemies' element presets, the power and resistance curve, and
/// the classes two ability effects look for.

#include "game.h"

/// The class series by a character's strongest and second element, read as
/// [strongest][second]; The Broken Seal's gClass_SeriesByElements.
const s32 Data_080c6604[4][4] = {
    {1, 3, 4, 5},
    {8, 6, 9, 10},
    {4, 3, 2, 5},
    {8, 10, 9, 7},
};

/// The elemental power and resistance of each element level, 0 to 15; The
/// Broken Seal's gElement_PowerResistByLevel.
const struct ElementLevelBonus Data_080c6644[16] = {
    {75, 85},   {80, 90},   {85, 95},   {90, 100},  {95, 105},  {100, 110},
    {105, 115}, {110, 120}, {115, 125}, {120, 130}, {125, 135}, {130, 140},
    {135, 145}, {140, 150}, {145, 160}, {150, 165},
};

/// The 48 enemy element presets, by EnemyDefinition.elementPreset.
// clang-format off
const struct EnemyElementPreset gData_080c6684[] = {
    {
        .first = ELEMENT_NONE,
        .digits = {1, 1, 1, 1},
        .values = {
            {.power = 100, .resistance = 100},
            {.power = 100, .resistance = 100},
            {.power = 100, .resistance = 100},
            {.power = 100, .resistance = 100},
        },
    },
    {
        .first = ELEMENT_NONE,
        .digits = {1, 1, 1, 1},
        .values = {
            {.power = 80, .resistance = 72},
            {.power = 80, .resistance = 72},
            {.power = 80, .resistance = 72},
            {.power = 80, .resistance = 72},
        },
    },
    {
        .first = ELEMENT_NONE,
        .digits = {1, 1, 1, 1},
        .values = {
            {.power = 80, .resistance = 90},
            {.power = 80, .resistance = 90},
            {.power = 80, .resistance = 90},
            {.power = 80, .resistance = 90},
        },
    },
    {
        .first = ELEMENT_NONE,
        .digits = {1},
        .values = {
            {.power = 100, .resistance = 48},
            {.power = 80, .resistance = 48},
            {.power = 70, .resistance = 25},
            {.power = 80, .resistance = 72},
        },
    },
    {
        .first = ELEMENT_NONE,
        .digits = {1},
        .values = {
            {.power = 100, .resistance = 72},
            {.power = 80, .resistance = 72},
            {.power = 70, .resistance = 25},
            {.power = 80, .resistance = 127},
        },
    },
    {
        .first = ELEMENT_NONE,
        .digits = {1},
        .values = {
            {.power = 95, .resistance = 48},
            {.power = 85, .resistance = 48},
            {.power = 90, .resistance = 25},
            {.power = 85, .resistance = 48},
        },
    },
    {
        .first = ELEMENT_NONE,
        .digits = {1},
        .values = {
            {.power = 95, .resistance = 72},
            {.power = 85, .resistance = 72},
            {.power = 90, .resistance = 48},
            {.power = 85, .resistance = 100},
        },
    },
    {
        .first = ELEMENT_NONE,
        .digits = {2, 1},
        .values = {
            {.power = 105, .resistance = 48},
            {.power = 100, .resistance = 100},
            {.power = 80, .resistance = 7},
            {.power = 70, .resistance = 72},
        },
    },
    {
        .first = ELEMENT_NONE,
        .digits = {2, 1},
        .values = {
            {.power = 105, .resistance = 72},
            {.power = 100, .resistance = 175},
            {.power = 80, .resistance = 25},
            {.power = 70, .resistance = 100},
        },
    },
    {
        .first = ELEMENT_NONE,
        .digits = {1, 0, 0, 2},
        .values = {
            {.power = 90, .resistance = 100},
            {.power = 90, .resistance = 48},
            {.power = 95, .resistance = 48},
            {.power = 105, .resistance = 7},
        },
    },
    {
        .first = ELEMENT_NONE,
        .digits = {1, 0, 0, 2},
        .values = {
            {.power = 90, .resistance = 152},
            {.power = 90, .resistance = 72},
            {.power = 95, .resistance = 72},
            {.power = 105, .resistance = 25},
        },
    },
    {
        .first = ELEMENT_NONE,
        .digits = {1, 1, 1, 2},
        .values = {
            {.power = 95, .resistance = 127},
            {.power = 95, .resistance = 72},
            {.power = 95, .resistance = 48},
            {.power = 110, .resistance = 7},
        },
    },
    {
        .first = ELEMENT_NONE,
        .digits = {1, 1, 1, 2},
        .values = {
            {.power = 95, .resistance = 175},
            {.power = 95, .resistance = 100},
            {.power = 95, .resistance = 72},
            {.power = 110, .resistance = 25},
        },
    },
    {
        .first = ELEMENT_NONE,
        .digits = {2, 1},
        .values = {
            {.power = 100, .resistance = 25},
            {.power = 95, .resistance = 72},
            {.power = 80, .resistance = 7},
            {.power = 80, .resistance = 48},
        },
    },
    {
        .first = ELEMENT_NONE,
        .digits = {2, 1},
        .values = {
            {.power = 100, .resistance = 25},
            {.power = 95, .resistance = 127},
            {.power = 80, .resistance = 72},
            {.power = 80, .resistance = 48},
        },
    },
    {
        .first = ELEMENT_NONE,
        .digits = {0, 1, 1, 2},
        .values = {
            {.power = 100, .resistance = 48},
            {.power = 95, .resistance = 100},
            {.power = 95, .resistance = 72},
            {.power = 95, .resistance = 7},
        },
    },
    {
        .first = ELEMENT_NONE,
        .digits = {0, 1, 1, 2},
        .values = {
            {.power = 100, .resistance = 72},
            {.power = 95, .resistance = 152},
            {.power = 95, .resistance = 100},
            {.power = 95, .resistance = 25},
        },
    },
    {
        .first = ELEMENT_NONE,
        .digits = {2, 1, 1, 1},
        .values = {
            {.power = 100, .resistance = 48},
            {.power = 100, .resistance = 100},
            {.power = 100, .resistance = 100},
            {.power = 95, .resistance = 127},
        },
    },
    {
        .first = ELEMENT_NONE,
        .digits = {4, 2, 2},
        .values = {
            {.power = 110, .resistance = 152},
            {.power = 95, .resistance = 100},
            {.power = 95, .resistance = 100},
            {.power = 90, .resistance = 48},
        },
    },
    {
        .first = ELEMENT_NONE,
        .digits = {2, 4, 0, 2},
        .values = {
            {.power = 95, .resistance = 100},
            {.power = 115, .resistance = 175},
            {.power = 85, .resistance = 25},
            {.power = 95, .resistance = 100},
        },
    },
    {
        .first = ELEMENT_NONE,
        .digits = {2, 0, 4, 2},
        .values = {
            {.power = 95, .resistance = 100},
            {.power = 85, .resistance = 25},
            {.power = 115, .resistance = 175},
            {.power = 95, .resistance = 100},
        },
    },
    {
        .first = ELEMENT_NONE,
        .digits = {0, 2, 2, 4},
        .values = {
            {.power = 90, .resistance = 48},
            {.power = 95, .resistance = 100},
            {.power = 95, .resistance = 100},
            {.power = 110, .resistance = 152},
        },
    },
    {
        .digits = {6, 2, 2},
        .values = {
            {.power = 120, .resistance = 100},
            {.power = 95, .resistance = 48},
            {.power = 95, .resistance = 48},
            {.power = 80, .resistance = 7},
        },
    },
    {
        .digits = {6, 2, 2},
        .values = {
            {.power = 120, .resistance = 193},
            {.power = 95, .resistance = 100},
            {.power = 95, .resistance = 100},
            {.power = 80, .resistance = 25},
        },
    },
    {
        .first = ELEMENT_MERCURY,
        .digits = {2, 6, 0, 2},
        .values = {
            {.power = 95, .resistance = 48},
            {.power = 125, .resistance = 127},
            {.power = 75, .resistance = 7},
            {.power = 95, .resistance = 48},
        },
    },
    {
        .first = ELEMENT_MERCURY,
        .digits = {2, 6, 0, 2},
        .values = {
            {.power = 95, .resistance = 100},
            {.power = 125, .resistance = 193},
            {.power = 75, .resistance = 25},
            {.power = 95, .resistance = 100},
        },
    },
    {
        .first = ELEMENT_MARS,
        .digits = {2, 0, 6, 2},
        .values = {
            {.power = 95, .resistance = 48},
            {.power = 75, .resistance = 7},
            {.power = 125, .resistance = 127},
            {.power = 95, .resistance = 48},
        },
    },
    {
        .first = ELEMENT_MARS,
        .digits = {2, 0, 6, 2},
        .values = {
            {.power = 95, .resistance = 100},
            {.power = 75, .resistance = 25},
            {.power = 125, .resistance = 193},
            {.power = 95, .resistance = 100},
        },
    },
    {
        .first = ELEMENT_JUPITER,
        .digits = {0, 2, 2, 6},
        .values = {
            {.power = 80, .resistance = 7},
            {.power = 95, .resistance = 48},
            {.power = 95, .resistance = 48},
            {.power = 120, .resistance = 127},
        },
    },
    {
        .first = ELEMENT_JUPITER,
        .digits = {0, 2, 2, 6},
        .values = {
            {.power = 80, .resistance = 25},
            {.power = 95, .resistance = 100},
            {.power = 95, .resistance = 100},
            {.power = 120, .resistance = 193},
        },
    },
    {
        .first = ELEMENT_NONE,
        .digits = {3, 2, 2, 2},
        .values = {
            {.power = 100, .resistance = 100},
            {.power = 85, .resistance = 72},
            {.power = 85, .resistance = 72},
            {.power = 85, .resistance = 48},
        },
    },
    {
        .first = ELEMENT_NONE,
        .digits = {3, 2, 2, 2},
        .values = {
            {.power = 100, .resistance = 127},
            {.power = 85, .resistance = 100},
            {.power = 85, .resistance = 100},
            {.power = 85, .resistance = 72},
        },
    },
    {
        .first = ELEMENT_NONE,
        .digits = {2, 3, 2, 2},
        .values = {
            {.power = 85, .resistance = 72},
            {.power = 100, .resistance = 100},
            {.power = 85, .resistance = 48},
            {.power = 85, .resistance = 72},
        },
    },
    {
        .first = ELEMENT_NONE,
        .digits = {2, 3, 2, 2},
        .values = {
            {.power = 85, .resistance = 100},
            {.power = 100, .resistance = 127},
            {.power = 85, .resistance = 72},
            {.power = 85, .resistance = 100},
        },
    },
    {
        .first = ELEMENT_NONE,
        .digits = {2, 2, 3, 2},
        .values = {
            {.power = 85, .resistance = 72},
            {.power = 85, .resistance = 48},
            {.power = 100, .resistance = 100},
            {.power = 85, .resistance = 72},
        },
    },
    {
        .first = ELEMENT_NONE,
        .digits = {2, 2, 3, 2},
        .values = {
            {.power = 85, .resistance = 100},
            {.power = 85, .resistance = 72},
            {.power = 100, .resistance = 127},
            {.power = 85, .resistance = 100},
        },
    },
    {
        .first = ELEMENT_NONE,
        .digits = {2, 2, 2, 3},
        .values = {
            {.power = 85, .resistance = 48},
            {.power = 85, .resistance = 72},
            {.power = 85, .resistance = 72},
            {.power = 100, .resistance = 100},
        },
    },
    {
        .first = ELEMENT_NONE,
        .digits = {2, 2, 2, 3},
        .values = {
            {.power = 85, .resistance = 72},
            {.power = 85, .resistance = 100},
            {.power = 85, .resistance = 100},
            {.power = 100, .resistance = 127},
        },
    },
    {
        .first = ELEMENT_NONE,
        .digits = {3, 3, 0, 4},
        .values = {
            {.power = 95, .resistance = 100},
            {.power = 100, .resistance = 127},
            {.power = 75, .resistance = 25},
            {.power = 105, .resistance = 152},
        },
    },
    {
        .first = ELEMENT_NONE,
        .digits = {7, 5, 10, 7},
        .values = {
            {.power = 100, .resistance = 100},
            {.power = 90, .resistance = 72},
            {.power = 110, .resistance = 175},
            {.power = 100, .resistance = 127},
        },
    },
    {
        .first = ELEMENT_NONE,
        .digits = {8, 6, 11, 8},
        .values = {
            {.power = 100, .resistance = 127},
            {.power = 95, .resistance = 72},
            {.power = 105, .resistance = 175},
            {.power = 100, .resistance = 100},
        },
    },
    {
        .first = ELEMENT_MARS,
        .digits = {9, 7, 14, 9},
        .values = {
            {.power = 100, .resistance = 127},
            {.power = 95, .resistance = 72},
            {.power = 110, .resistance = 175},
            {.power = 100, .resistance = 127},
        },
    },
    {
        .first = ELEMENT_NONE,
        .digits = {4, 8, 6, 5},
        .values = {
            {.power = 100, .resistance = 72},
            {.power = 130, .resistance = 193},
            {.power = 120, .resistance = 127},
            {.power = 110, .resistance = 152},
        },
    },
    {
        .first = ELEMENT_NONE,
        .digits = {0, 2, 1, 4},
        .values = {
            {.power = 90, .resistance = 152},
            {.power = 95, .resistance = 110},
            {.power = 95, .resistance = 72},
            {.power = 110, .resistance = 48},
        },
    },
    {
        .first = ELEMENT_NONE,
        .digits = {14, 16, 13, 15},
        .values = {
            {.power = 90, .resistance = 130},
            {.power = 105, .resistance = 180},
            {.power = 85, .resistance = 72},
            {.power = 95, .resistance = 150},
        },
    },
    {
        .first = ELEMENT_NONE,
        .digits = {11, 7, 14, 9},
        .values = {
            {.power = 95, .resistance = 100},
            {.power = 85, .resistance = 60},
            {.power = 105, .resistance = 175},
            {.power = 95, .resistance = 100},
        },
    },
    {
        .first = ELEMENT_NONE,
        .digits = {17, 14, 12, 10},
        .values = {
            {.power = 110, .resistance = 150},
            {.power = 100, .resistance = 190},
            {.power = 100, .resistance = 130},
            {.power = 100, .resistance = 110},
        },
    },
    {
        .digits = {18, 11, 15, 8},
        .values = {
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

/// The classes Battle_IsEffect90Class looks for.
const u16 gBattle_Effect90Classes[47] = {
    88,  89,  90,  91,  92,  93,  94,  95,  96,  97,  98,  99,
    100, 143, 144, 145, 149, 150, 151, 176, 177, 178, 185, 186,
    187, 191, 192, 193, 194, 195, 196, 299, 300, 301, 323, 324,
    325, 357, 369, 370, 371, 372, 373, 374, 375, 376, 377,
};

/// The classes Battle_IsEffect91Class looks for.
const u16 gBattle_Effect91Classes[29] = {
    84,  85,  86,  87,  88,  89,  90,  101, 102, 103, 167, 168, 169, 170, 171,
    172, 173, 174, 175, 176, 177, 178, 179, 180, 181, 330, 331, 332, 348,
};
