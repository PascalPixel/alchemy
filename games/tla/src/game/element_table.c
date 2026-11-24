/// @file
/// The element tables: class series by element, element power and resistance
/// by level, the enemies' element presets, the power and resistance curve, and
/// the dragons and sea creatures two weapons' effects look for.
// Part of game.c.

#include "game_internal.h"
#include "enemy_ids.h"

/// The class series by a character's strongest and second element, read as
/// [strongest][second]; The Broken Seal's gClass_SeriesByElements.
const s32 gClass_SeriesByElements[4][4] = {
    {CLASS_SERIES_SQUIRE, CLASS_SERIES_SWORDSMAN, CLASS_SERIES_BRUTE,
     CLASS_SERIES_APPRENTICE},
    {CLASS_SERIES_SEER, CLASS_SERIES_WATER_SEER, CLASS_SERIES_PILGRIM,
     CLASS_SERIES_HERMIT},
    {CLASS_SERIES_BRUTE, CLASS_SERIES_SWORDSMAN, CLASS_SERIES_GUARD,
     CLASS_SERIES_APPRENTICE},
    {CLASS_SERIES_SEER, CLASS_SERIES_HERMIT, CLASS_SERIES_PILGRIM,
     CLASS_SERIES_WIND_SEER},
};

/// The elemental power and resistance of each element level, 0 to 15; The
/// Broken Seal's gElement_PowerResistByLevel.
const struct ElementLevelBonus gElement_PowerResistByLevel[16] = {
    {75, 85},   {80, 90},   {85, 95},   {90, 100},  {95, 105},  {100, 110},
    {105, 115}, {110, 120}, {115, 125}, {120, 130}, {125, 135}, {130, 140},
    {135, 145}, {140, 150}, {145, 160}, {150, 165},
};

// clang-format off
/// The 48 enemy element presets, by EnemyDefinition.elementPreset.
const struct EnemyElementPreset gEnemy_ElementPresetTable[] = {
    {
        .attackElement = ELEMENT_NONE,
        .elementLevels = {1, 1, 1, 1},
        .stats = {
            {.power = 100, .resistance = 100},
            {.power = 100, .resistance = 100},
            {.power = 100, .resistance = 100},
            {.power = 100, .resistance = 100},
        },
    },
    {
        .attackElement = ELEMENT_NONE,
        .elementLevels = {1, 1, 1, 1},
        .stats = {
            {.power = 80, .resistance = 72},
            {.power = 80, .resistance = 72},
            {.power = 80, .resistance = 72},
            {.power = 80, .resistance = 72},
        },
    },
    {
        .attackElement = ELEMENT_NONE,
        .elementLevels = {1, 1, 1, 1},
        .stats = {
            {.power = 80, .resistance = 90},
            {.power = 80, .resistance = 90},
            {.power = 80, .resistance = 90},
            {.power = 80, .resistance = 90},
        },
    },
    {
        .attackElement = ELEMENT_NONE,
        .elementLevels = {1},
        .stats = {
            {.power = 100, .resistance = 48},
            {.power = 80, .resistance = 48},
            {.power = 70, .resistance = 25},
            {.power = 80, .resistance = 72},
        },
    },
    {
        .attackElement = ELEMENT_NONE,
        .elementLevels = {1},
        .stats = {
            {.power = 100, .resistance = 72},
            {.power = 80, .resistance = 72},
            {.power = 70, .resistance = 25},
            {.power = 80, .resistance = 127},
        },
    },
    {
        .attackElement = ELEMENT_NONE,
        .elementLevels = {1},
        .stats = {
            {.power = 95, .resistance = 48},
            {.power = 85, .resistance = 48},
            {.power = 90, .resistance = 25},
            {.power = 85, .resistance = 48},
        },
    },
    {
        .attackElement = ELEMENT_NONE,
        .elementLevels = {1},
        .stats = {
            {.power = 95, .resistance = 72},
            {.power = 85, .resistance = 72},
            {.power = 90, .resistance = 48},
            {.power = 85, .resistance = 100},
        },
    },
    {
        .attackElement = ELEMENT_NONE,
        .elementLevels = {2, 1},
        .stats = {
            {.power = 105, .resistance = 48},
            {.power = 100, .resistance = 100},
            {.power = 80, .resistance = 7},
            {.power = 70, .resistance = 72},
        },
    },
    {
        .attackElement = ELEMENT_NONE,
        .elementLevels = {2, 1},
        .stats = {
            {.power = 105, .resistance = 72},
            {.power = 100, .resistance = 175},
            {.power = 80, .resistance = 25},
            {.power = 70, .resistance = 100},
        },
    },
    {
        .attackElement = ELEMENT_NONE,
        .elementLevels = {1, 0, 0, 2},
        .stats = {
            {.power = 90, .resistance = 100},
            {.power = 90, .resistance = 48},
            {.power = 95, .resistance = 48},
            {.power = 105, .resistance = 7},
        },
    },
    {
        .attackElement = ELEMENT_NONE,
        .elementLevels = {1, 0, 0, 2},
        .stats = {
            {.power = 90, .resistance = 152},
            {.power = 90, .resistance = 72},
            {.power = 95, .resistance = 72},
            {.power = 105, .resistance = 25},
        },
    },
    {
        .attackElement = ELEMENT_NONE,
        .elementLevels = {1, 1, 1, 2},
        .stats = {
            {.power = 95, .resistance = 127},
            {.power = 95, .resistance = 72},
            {.power = 95, .resistance = 48},
            {.power = 110, .resistance = 7},
        },
    },
    {
        .attackElement = ELEMENT_NONE,
        .elementLevels = {1, 1, 1, 2},
        .stats = {
            {.power = 95, .resistance = 175},
            {.power = 95, .resistance = 100},
            {.power = 95, .resistance = 72},
            {.power = 110, .resistance = 25},
        },
    },
    {
        .attackElement = ELEMENT_NONE,
        .elementLevels = {2, 1},
        .stats = {
            {.power = 100, .resistance = 25},
            {.power = 95, .resistance = 72},
            {.power = 80, .resistance = 7},
            {.power = 80, .resistance = 48},
        },
    },
    {
        .attackElement = ELEMENT_NONE,
        .elementLevels = {2, 1},
        .stats = {
            {.power = 100, .resistance = 25},
            {.power = 95, .resistance = 127},
            {.power = 80, .resistance = 72},
            {.power = 80, .resistance = 48},
        },
    },
    {
        .attackElement = ELEMENT_NONE,
        .elementLevels = {0, 1, 1, 2},
        .stats = {
            {.power = 100, .resistance = 48},
            {.power = 95, .resistance = 100},
            {.power = 95, .resistance = 72},
            {.power = 95, .resistance = 7},
        },
    },
    {
        .attackElement = ELEMENT_NONE,
        .elementLevels = {0, 1, 1, 2},
        .stats = {
            {.power = 100, .resistance = 72},
            {.power = 95, .resistance = 152},
            {.power = 95, .resistance = 100},
            {.power = 95, .resistance = 25},
        },
    },
    {
        .attackElement = ELEMENT_NONE,
        .elementLevels = {2, 1, 1, 1},
        .stats = {
            {.power = 100, .resistance = 48},
            {.power = 100, .resistance = 100},
            {.power = 100, .resistance = 100},
            {.power = 95, .resistance = 127},
        },
    },
    {
        .attackElement = ELEMENT_NONE,
        .elementLevels = {4, 2, 2},
        .stats = {
            {.power = 110, .resistance = 152},
            {.power = 95, .resistance = 100},
            {.power = 95, .resistance = 100},
            {.power = 90, .resistance = 48},
        },
    },
    {
        .attackElement = ELEMENT_NONE,
        .elementLevels = {2, 4, 0, 2},
        .stats = {
            {.power = 95, .resistance = 100},
            {.power = 115, .resistance = 175},
            {.power = 85, .resistance = 25},
            {.power = 95, .resistance = 100},
        },
    },
    {
        .attackElement = ELEMENT_NONE,
        .elementLevels = {2, 0, 4, 2},
        .stats = {
            {.power = 95, .resistance = 100},
            {.power = 85, .resistance = 25},
            {.power = 115, .resistance = 175},
            {.power = 95, .resistance = 100},
        },
    },
    {
        .attackElement = ELEMENT_NONE,
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
        .attackElement = ELEMENT_MERCURY,
        .elementLevels = {2, 6, 0, 2},
        .stats = {
            {.power = 95, .resistance = 48},
            {.power = 125, .resistance = 127},
            {.power = 75, .resistance = 7},
            {.power = 95, .resistance = 48},
        },
    },
    {
        .attackElement = ELEMENT_MERCURY,
        .elementLevels = {2, 6, 0, 2},
        .stats = {
            {.power = 95, .resistance = 100},
            {.power = 125, .resistance = 193},
            {.power = 75, .resistance = 25},
            {.power = 95, .resistance = 100},
        },
    },
    {
        .attackElement = ELEMENT_MARS,
        .elementLevels = {2, 0, 6, 2},
        .stats = {
            {.power = 95, .resistance = 48},
            {.power = 75, .resistance = 7},
            {.power = 125, .resistance = 127},
            {.power = 95, .resistance = 48},
        },
    },
    {
        .attackElement = ELEMENT_MARS,
        .elementLevels = {2, 0, 6, 2},
        .stats = {
            {.power = 95, .resistance = 100},
            {.power = 75, .resistance = 25},
            {.power = 125, .resistance = 193},
            {.power = 95, .resistance = 100},
        },
    },
    {
        .attackElement = ELEMENT_JUPITER,
        .elementLevels = {0, 2, 2, 6},
        .stats = {
            {.power = 80, .resistance = 7},
            {.power = 95, .resistance = 48},
            {.power = 95, .resistance = 48},
            {.power = 120, .resistance = 127},
        },
    },
    {
        .attackElement = ELEMENT_JUPITER,
        .elementLevels = {0, 2, 2, 6},
        .stats = {
            {.power = 80, .resistance = 25},
            {.power = 95, .resistance = 100},
            {.power = 95, .resistance = 100},
            {.power = 120, .resistance = 193},
        },
    },
    {
        .attackElement = ELEMENT_NONE,
        .elementLevels = {3, 2, 2, 2},
        .stats = {
            {.power = 100, .resistance = 100},
            {.power = 85, .resistance = 72},
            {.power = 85, .resistance = 72},
            {.power = 85, .resistance = 48},
        },
    },
    {
        .attackElement = ELEMENT_NONE,
        .elementLevels = {3, 2, 2, 2},
        .stats = {
            {.power = 100, .resistance = 127},
            {.power = 85, .resistance = 100},
            {.power = 85, .resistance = 100},
            {.power = 85, .resistance = 72},
        },
    },
    {
        .attackElement = ELEMENT_NONE,
        .elementLevels = {2, 3, 2, 2},
        .stats = {
            {.power = 85, .resistance = 72},
            {.power = 100, .resistance = 100},
            {.power = 85, .resistance = 48},
            {.power = 85, .resistance = 72},
        },
    },
    {
        .attackElement = ELEMENT_NONE,
        .elementLevels = {2, 3, 2, 2},
        .stats = {
            {.power = 85, .resistance = 100},
            {.power = 100, .resistance = 127},
            {.power = 85, .resistance = 72},
            {.power = 85, .resistance = 100},
        },
    },
    {
        .attackElement = ELEMENT_NONE,
        .elementLevels = {2, 2, 3, 2},
        .stats = {
            {.power = 85, .resistance = 72},
            {.power = 85, .resistance = 48},
            {.power = 100, .resistance = 100},
            {.power = 85, .resistance = 72},
        },
    },
    {
        .attackElement = ELEMENT_NONE,
        .elementLevels = {2, 2, 3, 2},
        .stats = {
            {.power = 85, .resistance = 100},
            {.power = 85, .resistance = 72},
            {.power = 100, .resistance = 127},
            {.power = 85, .resistance = 100},
        },
    },
    {
        .attackElement = ELEMENT_NONE,
        .elementLevels = {2, 2, 2, 3},
        .stats = {
            {.power = 85, .resistance = 48},
            {.power = 85, .resistance = 72},
            {.power = 85, .resistance = 72},
            {.power = 100, .resistance = 100},
        },
    },
    {
        .attackElement = ELEMENT_NONE,
        .elementLevels = {2, 2, 2, 3},
        .stats = {
            {.power = 85, .resistance = 72},
            {.power = 85, .resistance = 100},
            {.power = 85, .resistance = 100},
            {.power = 100, .resistance = 127},
        },
    },
    {
        .attackElement = ELEMENT_NONE,
        .elementLevels = {3, 3, 0, 4},
        .stats = {
            {.power = 95, .resistance = 100},
            {.power = 100, .resistance = 127},
            {.power = 75, .resistance = 25},
            {.power = 105, .resistance = 152},
        },
    },
    {
        .attackElement = ELEMENT_NONE,
        .elementLevels = {7, 5, 10, 7},
        .stats = {
            {.power = 100, .resistance = 100},
            {.power = 90, .resistance = 72},
            {.power = 110, .resistance = 175},
            {.power = 100, .resistance = 127},
        },
    },
    {
        .attackElement = ELEMENT_NONE,
        .elementLevels = {8, 6, 11, 8},
        .stats = {
            {.power = 100, .resistance = 127},
            {.power = 95, .resistance = 72},
            {.power = 105, .resistance = 175},
            {.power = 100, .resistance = 100},
        },
    },
    {
        .attackElement = ELEMENT_MARS,
        .elementLevels = {9, 7, 14, 9},
        .stats = {
            {.power = 100, .resistance = 127},
            {.power = 95, .resistance = 72},
            {.power = 110, .resistance = 175},
            {.power = 100, .resistance = 127},
        },
    },
    {
        .attackElement = ELEMENT_NONE,
        .elementLevels = {4, 8, 6, 5},
        .stats = {
            {.power = 100, .resistance = 72},
            {.power = 130, .resistance = 193},
            {.power = 120, .resistance = 127},
            {.power = 110, .resistance = 152},
        },
    },
    {
        .attackElement = ELEMENT_NONE,
        .elementLevels = {0, 2, 1, 4},
        .stats = {
            {.power = 90, .resistance = 152},
            {.power = 95, .resistance = 110},
            {.power = 95, .resistance = 72},
            {.power = 110, .resistance = 48},
        },
    },
    {
        .attackElement = ELEMENT_NONE,
        .elementLevels = {14, 16, 13, 15},
        .stats = {
            {.power = 90, .resistance = 130},
            {.power = 105, .resistance = 180},
            {.power = 85, .resistance = 72},
            {.power = 95, .resistance = 150},
        },
    },
    {
        .attackElement = ELEMENT_NONE,
        .elementLevels = {11, 7, 14, 9},
        .stats = {
            {.power = 95, .resistance = 100},
            {.power = 85, .resistance = 60},
            {.power = 105, .resistance = 175},
            {.power = 95, .resistance = 100},
        },
    },
    {
        .attackElement = ELEMENT_NONE,
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
    ENEMY_AQUA_HYDRA,
    ENEMY_HYDRA,
    ENEMY_PYRODRA,
    ENEMY_SERPENT,
    ENEMY_SERPENT_ONE_BEAM,
    ENEMY_SERPENT_TWO_BEAMS,
    ENEMY_SERPENT_THREE_BEAMS,
    ENEMY_SERPENT_FOUR_BEAMS,
    ENEMY_SERPENT_6,
    ENEMY_SERPENT_7,
    ENEMY_AVIMANDER,
    ENEMY_MACETAIL,
    ENEMY_BOMBANDER,
    ENEMY_WYVERN_CHICK,
    ENEMY_PTERANODON,
    ENEMY_WINGED_LIZARD,
    ENEMY_DINO,
    ENEMY_DINOX,
    ENEMY_DINOSAURUS,
    ENEMY_SEA_DRAGON,
    ENEMY_TURTLE_DRAGON,
    ENEMY_OCEAN_DRAGON,
    ENEMY_WYVERN,
    ENEMY_SKY_DRAGON,
    ENEMY_WINGED_DRAGON,
    ENEMY_BLUE_DRAGON,
    ENEMY_CRUEL_DRAGON,
    ENEMY_DRAGON,
    ENEMY_FLAME_DRAGON_AGATIO,
    ENEMY_FLAME_DRAGON_KARST,
    ENEMY_FIRE_DRAGON,
    ENEMY_SALAMANDER,
    ENEMY_EARTH_LIZARD,
    ENEMY_THUNDER_LIZARD,
    ENEMY_CHIMERA,
    ENEMY_CHIMERA_MAGE,
    ENEMY_GRAND_CHIMERA,
    ENEMY_FUSION_DRAGON,
    ENEMY_DOOM_DRAGON_A,
    ENEMY_DOOM_DRAGON_B,
    ENEMY_DOOM_DRAGON_C,
    ENEMY_DOOM_DRAGON_D,
    ENEMY_DOOM_DRAGON_E,
    ENEMY_DOOM_DRAGON_F,
    ENEMY_DOOM_DRAGON_G,
    ENEMY_DOOM_DRAGON_H,
    ENEMY_DOOM_DRAGON_I,
};

/// The sea creatures, by enemy id: the Cuttles, Jellies, Hydras, Poseidon,
/// the shells and urchins, the Mermen, the sea Dragons, the Seabirds, the
/// Lizard Men and the Kraken. The Trident's use takes effect only on these
/// (Enemy_IsSeaCreature).
const u16 gEnemy_SeaCreatureIds[SEA_CREATURE_ENEMY_COUNT] = {
    ENEMY_CUTTLE,        ENEMY_CALAMAR,      ENEMY_MAN_O_WAR,
    ENEMY_AQUA_JELLY,    ENEMY_AQUA_HYDRA,   ENEMY_HYDRA,
    ENEMY_PYRODRA,       ENEMY_POSEIDON,     ENEMY_POSEIDON_SECOND_ACTION,
    ENEMY_POSEIDON_3,    ENEMY_URCHIN_BEAST, ENEMY_NEEDLE_EGG,
    ENEMY_SEA_HEDGEHOG,  ENEMY_CONCH_SHELL,  ENEMY_SPIRAL_SHELL,
    ENEMY_POISON_SHELL,  ENEMY_MERMAN,       ENEMY_GILLMAN,
    ENEMY_GILLMAN_LORD,  ENEMY_SEA_DRAGON,   ENEMY_TURTLE_DRAGON,
    ENEMY_OCEAN_DRAGON,  ENEMY_SEABIRD,      ENEMY_SEAFOWL,
    ENEMY_GREAT_SEAGULL, ENEMY_LIZARD_MAN,   ENEMY_LIZARD_FIGHTER,
    ENEMY_LIZARD_KING,   ENEMY_KRAKEN,
};
