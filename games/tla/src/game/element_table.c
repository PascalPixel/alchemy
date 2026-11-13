/// @file
/// The element tables: class series by element, element power and resistance
/// by level, the enemies' element presets, the power and resistance curve, and
/// the classes two ability effects look for.

// ---- games/tla/src/game/element_table.c ----

#include "game.h"

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

/// The 48 enemy element presets, by EnemyDefinition.elementPreset.
// clang-format off
const struct EnemyElementPreset gEnemy_ElementPresetTable[] = {
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

// ---- games/tla/src/game/djinn_table.c ----
// The 80 Djinn, 20 of each element in element order. Each name is the
// English release's (MsgDjinnName plus the Djinni's place).

// clang-format off
const struct DjinnDefinition gDjinn_DefinitionTable[] = {
    // Flint
    {
        .abilityId = 300,
        .hp = 8,
        .pp = 4,
        .atk = 3,
    },
    // Granite
    {
        .abilityId = 301,
        .hp = 9,
        .def = 2,
        .agi = 2,
        .luck = 1,
    },
    // Quartz
    {
        .abilityId = 302,
        .hp = 10,
        .pp = 3,
        .agi = 3,
    },
    // Vine
    {
        .abilityId = 303,
        .hp = 12,
        .pp = 4,
        .def = 3,
        .luck = 1,
    },
    // Sap
    {
        .abilityId = 304,
        .hp = 10,
        .atk = 3,
        .luck = 1,
    },
    // Ground
    {
        .abilityId = 305,
        .hp = 9,
        .pp = 3,
        .agi = 3,
    },
    // Bane
    {
        .abilityId = 306,
        .hp = 12,
        .atk = 4,
    },
    // Echo
    {
        .abilityId = 307,
        .hp = 9,
        .pp = 4,
        .atk = 3,
    },
    // Iron
    {
        .abilityId = 308,
        .hp = 11,
        .def = 2,
        .agi = 3,
    },
    // Steel
    {
        .abilityId = 309,
        .hp = 9,
        .atk = 4,
        .def = 2,
        .luck = 1,
    },
    // Mud
    {
        .abilityId = 310,
        .hp = 10,
        .pp = 4,
        .agi = 3,
    },
    // Flower
    {
        .abilityId = 311,
        .hp = 12,
        .pp = 4,
    },
    // Meld
    {
        .abilityId = 312,
        .hp = 9,
        .agi = 4,
        .luck = 1,
    },
    // Petra
    {
        .abilityId = 313,
        .hp = 11,
        .def = 3,
    },
    // Salt
    {
        .abilityId = 314,
        .hp = 9,
        .pp = 5,
        .luck = 1,
    },
    // Geode
    {
        .abilityId = 315,
        .hp = 12,
        .atk = 6,
    },
    // Mold
    {
        .abilityId = 316,
        .hp = 8,
        .atk = 4,
        .agi = 2,
        .luck = 1,
    },
    // Crystal
    {
        .abilityId = 317,
        .hp = 10,
        .pp = 5,
        .def = 2,
    },
    // Earth18
    {
        .abilityId = 318,
    },
    // Earth19
    {
        .abilityId = 319,
    },
    // Fizz
    {
        .abilityId = 320,
        .hp = 9,
        .pp = 4,
        .def = 3,
    },
    // Sleet
    {
        .abilityId = 321,
        .hp = 12,
        .atk = 3,
        .luck = 1,
    },
    // Mist
    {
        .abilityId = 322,
        .hp = 11,
        .atk = 4,
    },
    // Spritz
    {
        .abilityId = 323,
        .hp = 8,
        .pp = 4,
        .agi = 3,
    },
    // Hail
    {
        .abilityId = 324,
        .hp = 9,
        .atk = 4,
        .luck = 1,
    },
    // Tonic
    {
        .abilityId = 325,
        .hp = 8,
        .pp = 3,
        .def = 2,
        .luck = 2,
    },
    // Dew
    {
        .abilityId = 326,
        .hp = 13,
        .pp = 4,
        .agi = 4,
    },
    // Fog
    {
        .abilityId = 327,
        .hp = 9,
        .def = 2,
        .agi = 2,
        .luck = 1,
    },
    // Sour
    {
        .abilityId = 328,
        .hp = 8,
        .pp = 4,
        .atk = 3,
    },
    // Spring
    {
        .abilityId = 329,
        .hp = 11,
        .pp = 5,
    },
    // Shade
    {
        .abilityId = 330,
        .hp = 9,
        .def = 3,
        .luck = 2,
    },
    // Chill
    {
        .abilityId = 331,
        .hp = 10,
        .pp = 3,
        .def = 2,
    },
    // Steam
    {
        .abilityId = 332,
        .hp = 10,
        .atk = 5,
    },
    // Rime
    {
        .abilityId = 333,
        .hp = 10,
        .pp = 6,
    },
    // Gel
    {
        .abilityId = 334,
        .hp = 9,
        .atk = 5,
        .agi = 2,
    },
    // Eddy
    {
        .abilityId = 335,
        .hp = 9,
        .agi = 3,
        .luck = 2,
    },
    // Balm
    {
        .abilityId = 336,
        .hp = 13,
        .pp = 4,
    },
    // Serac
    {
        .abilityId = 337,
        .hp = 12,
        .atk = 3,
    },
    // Water18
    {
        .abilityId = 338,
    },
    // Water19
    {
        .abilityId = 339,
    },
    // Forge
    {
        .abilityId = 340,
        .hp = 10,
        .atk = 2,
        .agi = 2,
        .luck = 2,
    },
    // Fever
    {
        .abilityId = 341,
        .hp = 8,
        .atk = 3,
        .agi = 2,
    },
    // Corona
    {
        .abilityId = 342,
        .hp = 12,
        .pp = 3,
        .def = 3,
        .luck = 1,
    },
    // Scorch
    {
        .abilityId = 343,
        .hp = 8,
        .atk = 3,
    },
    // Ember
    {
        .abilityId = 344,
        .hp = 9,
        .pp = 4,
        .def = 2,
        .agi = 2,
    },
    // Flash
    {
        .abilityId = 345,
        .hp = 14,
        .pp = 3,
        .def = 2,
    },
    // Torch
    {
        .abilityId = 346,
        .hp = 9,
        .atk = 3,
        .luck = 1,
    },
    // Cannon
    {
        .abilityId = 347,
        .hp = 10,
        .atk = 3,
    },
    // Spark
    {
        .abilityId = 348,
        .hp = 11,
        .pp = 6,
    },
    // Kindle
    {
        .abilityId = 349,
        .hp = 8,
        .atk = 5,
        .luck = 1,
    },
    // Char
    {
        .abilityId = 350,
        .hp = 9,
        .atk = 2,
        .agi = 2,
        .luck = 1,
    },
    // Coal
    {
        .abilityId = 351,
        .hp = 11,
        .pp = 3,
        .agi = 3,
    },
    // Reflux
    {
        .abilityId = 352,
        .hp = 9,
        .def = 3,
        .luck = 2,
    },
    // Core
    {
        .abilityId = 353,
        .hp = 8,
        .atk = 4,
        .def = 2,
    },
    // Tinder
    {
        .abilityId = 354,
        .hp = 12,
        .pp = 5,
    },
    // Shine
    {
        .abilityId = 355,
        .hp = 9,
        .atk = 3,
        .def = 3,
        .agi = 2,
    },
    // Fury
    {
        .abilityId = 356,
        .hp = 12,
        .pp = 4,
    },
    // Fugue
    {
        .abilityId = 357,
        .hp = 11,
        .pp = 4,
        .def = 2,
    },
    // Fire18
    {
        .abilityId = 358,
    },
    // Fire19
    {
        .abilityId = 359,
    },
    // Gust
    {
        .abilityId = 360,
        .hp = 9,
        .atk = 2,
        .agi = 2,
    },
    // Breeze
    {
        .abilityId = 361,
        .hp = 12,
        .pp = 5,
        .def = 2,
        .luck = 1,
    },
    // Zephyr
    {
        .abilityId = 362,
        .hp = 11,
        .pp = 3,
        .agi = 2,
        .luck = 1,
    },
    // Smog
    {
        .abilityId = 363,
        .hp = 9,
        .atk = 3,
    },
    // Kite
    {
        .abilityId = 364,
        .hp = 8,
        .pp = 4,
        .agi = 3,
    },
    // Squall
    {
        .abilityId = 365,
        .hp = 10,
        .atk = 5,
    },
    // Luff
    {
        .abilityId = 366,
        .hp = 11,
        .pp = 5,
        .def = 2,
        .luck = 1,
    },
    // Breath
    {
        .abilityId = 367,
        .hp = 9,
        .def = 3,
        .agi = 4,
    },
    // Blitz
    {
        .abilityId = 368,
        .hp = 10,
        .pp = 4,
        .atk = 3,
    },
    // Ether
    {
        .abilityId = 369,
        .hp = 8,
        .pp = 4,
        .agi = 3,
        .luck = 2,
    },
    // Waft
    {
        .abilityId = 370,
        .hp = 11,
        .atk = 4,
    },
    // Haze
    {
        .abilityId = 371,
        .hp = 10,
        .def = 2,
        .agi = 3,
        .luck = 2,
    },
    // Wheeze
    {
        .abilityId = 372,
        .hp = 9,
        .pp = 3,
        .atk = 5,
    },
    // Aroma
    {
        .abilityId = 373,
        .hp = 11,
        .agi = 3,
        .luck = 2,
    },
    // Whorl
    {
        .abilityId = 374,
        .hp = 9,
        .atk = 4,
        .def = 2,
    },
    // Gasp
    {
        .abilityId = 375,
        .hp = 12,
        .pp = 5,
    },
    // Lull
    {
        .abilityId = 376,
        .hp = 11,
        .pp = 6,
    },
    // Gale
    {
        .abilityId = 377,
        .hp = 10,
        .agi = 5,
        .luck = 3,
    },
    // Wind18
    {
        .abilityId = 378,
    },
    // Wind19
    {
        .abilityId = 379,
    },
};
// clang-format on
