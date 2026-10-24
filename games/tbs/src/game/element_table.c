/// @file
/// The element tables: class series by element, element power and resistance
/// by level, the enemies' element presets and the power and resistance curve.

#include "game.h"

/// gClass_SeriesByElements, read as [strongest * 4 + second].
// clang-format off
const s32 gClass_SeriesByElements[] = {
    1, 3, 4, 5, 8, 6, 9, 10,
    4, 3, 2, 5, 8, 10, 9, 7,
};

const struct ElementLevelBonus gElement_PowerResistByLevel[16] = {
    {75, 85},   {80, 90},   {85, 95},   {90, 100},
    {95, 105},  {100, 110}, {105, 115}, {110, 120},
    {115, 125}, {120, 130}, {125, 135}, {130, 140},
    {135, 145}, {140, 150}, {145, 160}, {150, 165},
};

/// The 44 enemy element presets, by EnemyDefinition.elementPreset.
const struct EnemyElementPreset gEnemy_ElementPresetTable[] = {
    {
        .first = ELEMENT_NONE,
        .digits = {1, 1, 1, 1},
        .values = {
            PRESET_STATS(100, 100),
            PRESET_STATS(100, 100),
            PRESET_STATS(100, 100),
            PRESET_STATS(100, 100),
        },
    },
    {
        .first = ELEMENT_NONE,
        .digits = {1, 1, 1, 1},
        .values = {
            PRESET_STATS(80, 72),
            PRESET_STATS(80, 72),
            PRESET_STATS(80, 72),
            PRESET_STATS(80, 72),
        },
    },
    {
        .first = ELEMENT_NONE,
        .digits = {1, 1, 1, 1},
        .values = {
            PRESET_STATS(80, 90),
            PRESET_STATS(80, 90),
            PRESET_STATS(80, 90),
            PRESET_STATS(80, 90),
        },
    },
    {
        .first = ELEMENT_NONE,
        .digits = {1, 0, 0, 0},
        .values = {
            PRESET_STATS(100, 48),
            PRESET_STATS(80, 48),
            PRESET_STATS(70, 25),
            PRESET_STATS(80, 72),
        },
    },
    {
        .first = ELEMENT_NONE,
        .digits = {1, 0, 0, 0},
        .values = {
            PRESET_STATS(100, 72),
            PRESET_STATS(80, 72),
            PRESET_STATS(70, 25),
            PRESET_STATS(80, 127),
        },
    },
    {
        .first = ELEMENT_NONE,
        .digits = {1, 0, 0, 0},
        .values = {
            PRESET_STATS(95, 48),
            PRESET_STATS(85, 48),
            PRESET_STATS(90, 25),
            PRESET_STATS(85, 48),
        },
    },
    {
        .first = ELEMENT_NONE,
        .digits = {1, 0, 0, 0},
        .values = {
            PRESET_STATS(95, 72),
            PRESET_STATS(85, 72),
            PRESET_STATS(90, 48),
            PRESET_STATS(85, 100),
        },
    },
    {
        .first = ELEMENT_NONE,
        .digits = {2, 1, 0, 0},
        .values = {
            PRESET_STATS(105, 48),
            PRESET_STATS(100, 100),
            PRESET_STATS(80, 7),
            PRESET_STATS(70, 72),
        },
    },
    {
        .first = ELEMENT_NONE,
        .digits = {2, 1, 0, 0},
        .values = {
            PRESET_STATS(105, 72),
            PRESET_STATS(100, 175),
            PRESET_STATS(80, 25),
            PRESET_STATS(70, 100),
        },
    },
    {
        .first = ELEMENT_NONE,
        .digits = {1, 0, 0, 2},
        .values = {
            PRESET_STATS(90, 100),
            PRESET_STATS(90, 48),
            PRESET_STATS(95, 48),
            PRESET_STATS(105, 7),
        },
    },
    {
        .first = ELEMENT_NONE,
        .digits = {1, 0, 0, 2},
        .values = {
            PRESET_STATS(90, 152),
            PRESET_STATS(90, 72),
            PRESET_STATS(95, 72),
            PRESET_STATS(105, 25),
        },
    },
    {
        .first = ELEMENT_NONE,
        .digits = {1, 1, 1, 2},
        .values = {
            PRESET_STATS(95, 127),
            PRESET_STATS(95, 72),
            PRESET_STATS(95, 48),
            PRESET_STATS(110, 7),
        },
    },
    {
        .first = ELEMENT_NONE,
        .digits = {1, 1, 1, 2},
        .values = {
            PRESET_STATS(95, 175),
            PRESET_STATS(95, 100),
            PRESET_STATS(95, 72),
            PRESET_STATS(110, 25),
        },
    },
    {
        .first = ELEMENT_NONE,
        .digits = {2, 1, 0, 0},
        .values = {
            PRESET_STATS(100, 25),
            PRESET_STATS(95, 72),
            PRESET_STATS(80, 7),
            PRESET_STATS(80, 48),
        },
    },
    {
        .first = ELEMENT_NONE,
        .digits = {2, 1, 0, 0},
        .values = {
            PRESET_STATS(100, 25),
            PRESET_STATS(95, 127),
            PRESET_STATS(80, 72),
            PRESET_STATS(80, 48),
        },
    },
    {
        .first = ELEMENT_NONE,
        .digits = {0, 1, 1, 2},
        .values = {
            PRESET_STATS(100, 48),
            PRESET_STATS(95, 100),
            PRESET_STATS(95, 72),
            PRESET_STATS(95, 7),
        },
    },
    {
        .first = ELEMENT_NONE,
        .digits = {0, 1, 1, 2},
        .values = {
            PRESET_STATS(100, 72),
            PRESET_STATS(95, 152),
            PRESET_STATS(95, 100),
            PRESET_STATS(95, 25),
        },
    },
    {
        .first = ELEMENT_NONE,
        .digits = {2, 1, 1, 1},
        .values = {
            PRESET_STATS(100, 48),
            PRESET_STATS(100, 100),
            PRESET_STATS(100, 100),
            PRESET_STATS(95, 127),
        },
    },
    {
        .first = ELEMENT_NONE,
        .digits = {4, 2, 2, 0},
        .values = {
            PRESET_STATS(110, 152),
            PRESET_STATS(95, 100),
            PRESET_STATS(95, 100),
            PRESET_STATS(90, 48),
        },
    },
    {
        .first = ELEMENT_NONE,
        .digits = {2, 4, 0, 2},
        .values = {
            PRESET_STATS(95, 100),
            PRESET_STATS(115, 175),
            PRESET_STATS(85, 25),
            PRESET_STATS(95, 100),
        },
    },
    {
        .first = ELEMENT_NONE,
        .digits = {2, 0, 4, 2},
        .values = {
            PRESET_STATS(95, 100),
            PRESET_STATS(85, 25),
            PRESET_STATS(115, 175),
            PRESET_STATS(95, 100),
        },
    },
    {
        .first = ELEMENT_NONE,
        .digits = {0, 2, 2, 4},
        .values = {
            PRESET_STATS(90, 48),
            PRESET_STATS(95, 100),
            PRESET_STATS(95, 100),
            PRESET_STATS(110, 152),
        },
    },
    {
        .first = ELEMENT_VENUS,
        .digits = {6, 2, 2, 0},
        .values = {
            PRESET_STATS(120, 100),
            PRESET_STATS(95, 48),
            PRESET_STATS(95, 48),
            PRESET_STATS(80, 7),
        },
    },
    {
        .first = ELEMENT_VENUS,
        .digits = {6, 2, 2, 0},
        .values = {
            PRESET_STATS(120, 193),
            PRESET_STATS(95, 100),
            PRESET_STATS(95, 100),
            PRESET_STATS(80, 25),
        },
    },
    {
        .first = ELEMENT_MERCURY,
        .digits = {2, 6, 0, 2},
        .values = {
            PRESET_STATS(95, 48),
            PRESET_STATS(125, 127),
            PRESET_STATS(75, 7),
            PRESET_STATS(95, 48),
        },
    },
    {
        .first = ELEMENT_MERCURY,
        .digits = {2, 6, 0, 2},
        .values = {
            PRESET_STATS(95, 100),
            PRESET_STATS(125, 193),
            PRESET_STATS(75, 25),
            PRESET_STATS(95, 100),
        },
    },
    {
        .first = ELEMENT_MARS,
        .digits = {2, 0, 6, 2},
        .values = {
            PRESET_STATS(95, 48),
            PRESET_STATS(75, 7),
            PRESET_STATS(125, 127),
            PRESET_STATS(95, 48),
        },
    },
    {
        .first = ELEMENT_MARS,
        .digits = {2, 0, 6, 2},
        .values = {
            PRESET_STATS(95, 100),
            PRESET_STATS(75, 25),
            PRESET_STATS(125, 193),
            PRESET_STATS(95, 100),
        },
    },
    {
        .first = ELEMENT_JUPITER,
        .digits = {0, 2, 2, 6},
        .values = {
            PRESET_STATS(80, 7),
            PRESET_STATS(95, 48),
            PRESET_STATS(95, 48),
            PRESET_STATS(120, 127),
        },
    },
    {
        .first = ELEMENT_JUPITER,
        .digits = {0, 2, 2, 6},
        .values = {
            PRESET_STATS(80, 25),
            PRESET_STATS(95, 100),
            PRESET_STATS(95, 100),
            PRESET_STATS(120, 193),
        },
    },
    {
        .first = ELEMENT_NONE,
        .digits = {3, 2, 2, 2},
        .values = {
            PRESET_STATS(100, 100),
            PRESET_STATS(85, 72),
            PRESET_STATS(85, 72),
            PRESET_STATS(85, 48),
        },
    },
    {
        .first = ELEMENT_NONE,
        .digits = {3, 2, 2, 2},
        .values = {
            PRESET_STATS(100, 127),
            PRESET_STATS(85, 100),
            PRESET_STATS(85, 100),
            PRESET_STATS(85, 72),
        },
    },
    {
        .first = ELEMENT_NONE,
        .digits = {2, 3, 2, 2},
        .values = {
            PRESET_STATS(85, 72),
            PRESET_STATS(100, 100),
            PRESET_STATS(85, 48),
            PRESET_STATS(85, 72),
        },
    },
    {
        .first = ELEMENT_NONE,
        .digits = {2, 3, 2, 2},
        .values = {
            PRESET_STATS(85, 100),
            PRESET_STATS(100, 127),
            PRESET_STATS(85, 72),
            PRESET_STATS(85, 100),
        },
    },
    {
        .first = ELEMENT_NONE,
        .digits = {2, 2, 3, 2},
        .values = {
            PRESET_STATS(85, 72),
            PRESET_STATS(85, 48),
            PRESET_STATS(100, 100),
            PRESET_STATS(85, 72),
        },
    },
    {
        .first = ELEMENT_NONE,
        .digits = {2, 2, 3, 2},
        .values = {
            PRESET_STATS(85, 100),
            PRESET_STATS(85, 72),
            PRESET_STATS(100, 127),
            PRESET_STATS(85, 100),
        },
    },
    {
        .first = ELEMENT_NONE,
        .digits = {2, 2, 2, 3},
        .values = {
            PRESET_STATS(85, 48),
            PRESET_STATS(85, 72),
            PRESET_STATS(85, 72),
            PRESET_STATS(100, 100),
        },
    },
    {
        .first = ELEMENT_NONE,
        .digits = {2, 2, 2, 3},
        .values = {
            PRESET_STATS(85, 72),
            PRESET_STATS(85, 100),
            PRESET_STATS(85, 100),
            PRESET_STATS(100, 127),
        },
    },
    {
        .first = ELEMENT_NONE,
        .digits = {3, 3, 0, 4},
        .values = {
            PRESET_STATS(95, 100),
            PRESET_STATS(100, 127),
            PRESET_STATS(75, 25),
            PRESET_STATS(105, 152),
        },
    },
    {
        .first = ELEMENT_NONE,
        .digits = {7, 5, 10, 7},
        .values = {
            PRESET_STATS(100, 100),
            PRESET_STATS(90, 72),
            PRESET_STATS(110, 175),
            PRESET_STATS(100, 127),
        },
    },
    {
        .first = ELEMENT_NONE,
        .digits = {8, 6, 11, 8},
        .values = {
            PRESET_STATS(100, 127),
            PRESET_STATS(95, 72),
            PRESET_STATS(105, 175),
            PRESET_STATS(100, 100),
        },
    },
    {
        .first = ELEMENT_MARS,
        .digits = {9, 7, 14, 9},
        .values = {
            PRESET_STATS(100, 127),
            PRESET_STATS(95, 72),
            PRESET_STATS(110, 175),
            PRESET_STATS(100, 127),
        },
    },
    {
        .first = ELEMENT_NONE,
        .digits = {4, 8, 6, 5},
        .values = {
            PRESET_STATS(100, 72),
            PRESET_STATS(130, 193),
            PRESET_STATS(120, 127),
            PRESET_STATS(110, 152),
        },
    },
    {
        .first = ELEMENT_NONE,
        .digits = {0, 2, 1, 4},
        .values = {
            PRESET_STATS(90, 152),
            PRESET_STATS(95, 110),
            PRESET_STATS(95, 72),
            PRESET_STATS(110, 48),
        },
    },
};

const s16 gElement_PowerResistCurve[5][2] = {
    {200, 256},
    {100, 128},
    {0, 0},
    {-100, -128},
    {-200, -256},
};
// clang-format on
