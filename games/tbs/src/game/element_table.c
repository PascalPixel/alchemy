/// @file
/// The element tables: class series by element, element power and resistance
/// by level, the enemies' element presets and the power and resistance curve.

#include "game.h"

/// gClass_SeriesByElements, read as [strongest * 4 + second], a row for each
/// strongest element and a column for each second, in element order (Venus,
/// Mercury, Mars, Jupiter).
// clang-format off
const s32 gClass_SeriesByElements[] = {
    CLASS_SERIES_SQUIRE, CLASS_SERIES_SWORDSMAN, CLASS_SERIES_BRUTE, CLASS_SERIES_APPRENTICE,
    CLASS_SERIES_SEER, CLASS_SERIES_WATER_SEER, CLASS_SERIES_PILGRIM, CLASS_SERIES_HERMIT,
    CLASS_SERIES_BRUTE, CLASS_SERIES_SWORDSMAN, CLASS_SERIES_GUARD, CLASS_SERIES_APPRENTICE,
    CLASS_SERIES_SEER, CLASS_SERIES_HERMIT, CLASS_SERIES_PILGRIM, CLASS_SERIES_WIND_SEER,
};

/// A character's base power and resistance in an element, by its whole element
/// level 0 to 15; Unit_CalculateBaseElements adds the level's tenths to both.
const struct ElementLevelBonus gElement_PowerResistByLevel[16] = {
    {75, 85},   {80, 90},   {85, 95},   {90, 100},
    {95, 105},  {100, 110}, {105, 115}, {110, 120},
    {115, 125}, {120, 130}, {125, 135}, {130, 140},
    {135, 145}, {140, 150}, {145, 160}, {150, 165},
};

/// The 44 enemy element presets, by EnemyDefinition.elementPreset; each is
/// named by the enemies that have it, in the English release's words.
const struct EnemyElementPreset gEnemy_ElementPresetTable[] = {
    // Unused: only ??? has it, and ??? is in no formation.
    {
        .first = ELEMENT_NONE,
        .elementLevels = {1, 1, 1, 1},
        .values = {
            PRESET_STATS(100, 100),
            PRESET_STATS(100, 100),
            PRESET_STATS(100, 100),
            PRESET_STATS(100, 100),
        },
    },
    // Bandit, Thief and Brigand
    {
        .first = ELEMENT_NONE,
        .elementLevels = {1, 1, 1, 1},
        .values = {
            PRESET_STATS(80, 72),
            PRESET_STATS(80, 72),
            PRESET_STATS(80, 72),
            PRESET_STATS(80, 72),
        },
    },
    // Azart, Satrage and Navampa
    {
        .first = ELEMENT_NONE,
        .elementLevels = {1, 1, 1, 1},
        .values = {
            PRESET_STATS(80, 90),
            PRESET_STATS(80, 90),
            PRESET_STATS(80, 90),
            PRESET_STATS(80, 90),
        },
    },
    // Vermin, Mad Vermin, Rat, Armored Rat, Plated Rat, Troll, Cave Troll, Brutal Troll, Mauler, Ravager, Mole, Mad Mole, Mole Mage and Ape
    {
        .first = ELEMENT_NONE,
        .elementLevels = {1, 0, 0, 0},
        .values = {
            PRESET_STATS(100, 48),
            PRESET_STATS(80, 48),
            PRESET_STATS(70, 25),
            PRESET_STATS(80, 72),
        },
    },
    // Grisly, Killer Ape and Dirty Ape
    {
        .first = ELEMENT_NONE,
        .elementLevels = {1, 0, 0, 0},
        .values = {
            PRESET_STATS(100, 72),
            PRESET_STATS(80, 72),
            PRESET_STATS(70, 25),
            PRESET_STATS(80, 127),
        },
    },
    // Spider, Tarantula, Recluse, Grub, Worm and Acid Maggot
    {
        .first = ELEMENT_NONE,
        .elementLevels = {1, 0, 0, 0},
        .values = {
            PRESET_STATS(95, 48),
            PRESET_STATS(85, 48),
            PRESET_STATS(90, 25),
            PRESET_STATS(85, 48),
        },
    },
    // Unused: no enemy has it.
    {
        .first = ELEMENT_NONE,
        .elementLevels = {1, 0, 0, 0},
        .values = {
            PRESET_STATS(95, 72),
            PRESET_STATS(85, 72),
            PRESET_STATS(90, 48),
            PRESET_STATS(85, 100),
        },
    },
    // Wild Mushroom and Death Cap
    {
        .first = ELEMENT_NONE,
        .elementLevels = {2, 1, 0, 0},
        .values = {
            PRESET_STATS(105, 48),
            PRESET_STATS(100, 100),
            PRESET_STATS(80, 7),
            PRESET_STATS(70, 72),
        },
    },
    // Tret
    {
        .first = ELEMENT_NONE,
        .elementLevels = {2, 1, 0, 0},
        .values = {
            PRESET_STATS(105, 72),
            PRESET_STATS(100, 175),
            PRESET_STATS(80, 25),
            PRESET_STATS(70, 100),
        },
    },
    // Drone Bee, Fighter Bee and Warrior Bee
    {
        .first = ELEMENT_NONE,
        .elementLevels = {1, 0, 0, 2},
        .values = {
            PRESET_STATS(90, 100),
            PRESET_STATS(90, 48),
            PRESET_STATS(95, 48),
            PRESET_STATS(105, 7),
        },
    },
    // Unused: no enemy has it.
    {
        .first = ELEMENT_NONE,
        .elementLevels = {1, 0, 0, 2},
        .values = {
            PRESET_STATS(90, 152),
            PRESET_STATS(90, 72),
            PRESET_STATS(95, 72),
            PRESET_STATS(105, 25),
        },
    },
    // Bat, Rabid Bat, Harpy, Virago, Dirge, Foul Dirge and Vile Dirge
    {
        .first = ELEMENT_NONE,
        .elementLevels = {1, 1, 1, 2},
        .values = {
            PRESET_STATS(95, 127),
            PRESET_STATS(95, 72),
            PRESET_STATS(95, 48),
            PRESET_STATS(110, 7),
        },
    },
    // Harridan
    {
        .first = ELEMENT_NONE,
        .elementLevels = {1, 1, 1, 2},
        .values = {
            PRESET_STATS(95, 175),
            PRESET_STATS(95, 100),
            PRESET_STATS(95, 72),
            PRESET_STATS(110, 25),
        },
    },
    // Zombie, Undead, Wight, Ghoul, Fiendish Ghoul and Cannibal Ghoul
    {
        .first = ELEMENT_NONE,
        .elementLevels = {2, 1, 0, 0},
        .values = {
            PRESET_STATS(100, 25),
            PRESET_STATS(95, 72),
            PRESET_STATS(80, 7),
            PRESET_STATS(80, 48),
        },
    },
    // Skeleton, Bone Fighter, Skull Warrior, Will Head, Death Head and Willowisp
    {
        .first = ELEMENT_NONE,
        .elementLevels = {2, 1, 0, 0},
        .values = {
            PRESET_STATS(100, 25),
            PRESET_STATS(95, 127),
            PRESET_STATS(80, 72),
            PRESET_STATS(80, 48),
        },
    },
    // Ghost, Ghost Mage, Horned Ghost, Amaze, Creeper and Spirit
    {
        .first = ELEMENT_NONE,
        .elementLevels = {0, 1, 1, 2},
        .values = {
            PRESET_STATS(100, 48),
            PRESET_STATS(95, 100),
            PRESET_STATS(95, 72),
            PRESET_STATS(95, 7),
        },
    },
    // Lich
    {
        .first = ELEMENT_NONE,
        .elementLevels = {0, 1, 1, 2},
        .values = {
            PRESET_STATS(100, 72),
            PRESET_STATS(95, 152),
            PRESET_STATS(95, 100),
            PRESET_STATS(95, 25),
        },
    },
    // Gargoyle, Clay Gargoyle and Ice Gargoyle
    {
        .first = ELEMENT_NONE,
        .elementLevels = {2, 1, 1, 1},
        .values = {
            PRESET_STATS(100, 48),
            PRESET_STATS(100, 100),
            PRESET_STATS(100, 100),
            PRESET_STATS(95, 127),
        },
    },
    // Unused: no enemy has it.
    {
        .first = ELEMENT_NONE,
        .elementLevels = {4, 2, 2, 0},
        .values = {
            PRESET_STATS(110, 152),
            PRESET_STATS(95, 100),
            PRESET_STATS(95, 100),
            PRESET_STATS(90, 48),
        },
    },
    // Cerebus, Fenrir, Kraken, Toadonpa, Poison Toad and ? (ENEMY_135)
    {
        .first = ELEMENT_NONE,
        .elementLevels = {2, 4, 0, 2},
        .values = {
            PRESET_STATS(95, 100),
            PRESET_STATS(115, 175),
            PRESET_STATS(85, 25),
            PRESET_STATS(95, 100),
        },
    },
    // Manticore, Magicore, Manticore King, Dread Hound, Chimera, Chimera Mage, ? (ENEMY_104), Ant Lion, Roach and Doodle Bug
    {
        .first = ELEMENT_NONE,
        .elementLevels = {2, 0, 4, 2},
        .values = {
            PRESET_STATS(95, 100),
            PRESET_STATS(85, 25),
            PRESET_STATS(115, 175),
            PRESET_STATS(95, 100),
        },
    },
    // Tornado Lizard, Storm Lizard and Tempest Lizard
    {
        .first = ELEMENT_NONE,
        .elementLevels = {0, 2, 2, 4},
        .values = {
            PRESET_STATS(90, 48),
            PRESET_STATS(95, 100),
            PRESET_STATS(95, 100),
            PRESET_STATS(110, 152),
        },
    },
    // Unused: no enemy has it.
    {
        .first = ELEMENT_VENUS,
        .elementLevels = {6, 2, 2, 0},
        .values = {
            PRESET_STATS(120, 100),
            PRESET_STATS(95, 48),
            PRESET_STATS(95, 48),
            PRESET_STATS(80, 7),
        },
    },
    // Earth Lizard, Golem, Earth Golem, Grand Golem and Venus Djinni
    {
        .first = ELEMENT_VENUS,
        .elementLevels = {6, 2, 2, 0},
        .values = {
            PRESET_STATS(120, 193),
            PRESET_STATS(95, 100),
            PRESET_STATS(95, 100),
            PRESET_STATS(80, 25),
        },
    },
    // Slime, Ooze, Slime Beast, Cuttle, Calamar and Man o' War
    {
        .first = ELEMENT_MERCURY,
        .elementLevels = {2, 6, 0, 2},
        .values = {
            PRESET_STATS(95, 48),
            PRESET_STATS(125, 127),
            PRESET_STATS(75, 7),
            PRESET_STATS(95, 48),
        },
    },
    // Living Statue, Hydros Statue and Mercury Djinni
    {
        .first = ELEMENT_MERCURY,
        .elementLevels = {2, 6, 0, 2},
        .values = {
            PRESET_STATS(95, 100),
            PRESET_STATS(125, 193),
            PRESET_STATS(75, 25),
            PRESET_STATS(95, 100),
        },
    },
    // Stone Soldier
    {
        .first = ELEMENT_MARS,
        .elementLevels = {2, 0, 6, 2},
        .values = {
            PRESET_STATS(95, 48),
            PRESET_STATS(75, 7),
            PRESET_STATS(125, 127),
            PRESET_STATS(95, 48),
        },
    },
    // Salamander, Boulder Beast, Raging Rock and Mars Djinni
    {
        .first = ELEMENT_MARS,
        .elementLevels = {2, 0, 6, 2},
        .values = {
            PRESET_STATS(95, 100),
            PRESET_STATS(75, 25),
            PRESET_STATS(125, 193),
            PRESET_STATS(95, 100),
        },
    },
    // Unused: no enemy has it.
    {
        .first = ELEMENT_JUPITER,
        .elementLevels = {0, 2, 2, 6},
        .values = {
            PRESET_STATS(80, 7),
            PRESET_STATS(95, 48),
            PRESET_STATS(95, 48),
            PRESET_STATS(120, 127),
        },
    },
    // Thunder Lizard and Jupiter Djinni
    {
        .first = ELEMENT_JUPITER,
        .elementLevels = {0, 2, 2, 6},
        .values = {
            PRESET_STATS(80, 25),
            PRESET_STATS(95, 100),
            PRESET_STATS(95, 100),
            PRESET_STATS(120, 193),
        },
    },
    // Gnome Wizard, Orc, Orc Captain, Kobold and Goblin
    {
        .first = ELEMENT_NONE,
        .elementLevels = {3, 2, 2, 2},
        .values = {
            PRESET_STATS(100, 100),
            PRESET_STATS(85, 72),
            PRESET_STATS(85, 72),
            PRESET_STATS(85, 48),
        },
    },
    // Orc Lord and Hobgoblin
    {
        .first = ELEMENT_NONE,
        .elementLevels = {3, 2, 2, 2},
        .values = {
            PRESET_STATS(100, 127),
            PRESET_STATS(85, 100),
            PRESET_STATS(85, 100),
            PRESET_STATS(85, 72),
        },
    },
    // Rat Soldier, Rat Fighter and Rat Warrior
    {
        .first = ELEMENT_NONE,
        .elementLevels = {2, 3, 2, 2},
        .values = {
            PRESET_STATS(85, 72),
            PRESET_STATS(100, 100),
            PRESET_STATS(85, 48),
            PRESET_STATS(85, 72),
        },
    },
    // Siren, Succubus, Lizard Man, Lizard Fighter and Lizard King
    {
        .first = ELEMENT_NONE,
        .elementLevels = {2, 3, 2, 2},
        .values = {
            PRESET_STATS(85, 100),
            PRESET_STATS(100, 127),
            PRESET_STATS(85, 72),
            PRESET_STATS(85, 100),
        },
    },
    // Gnome
    {
        .first = ELEMENT_NONE,
        .elementLevels = {2, 2, 3, 2},
        .values = {
            PRESET_STATS(85, 72),
            PRESET_STATS(85, 48),
            PRESET_STATS(100, 100),
            PRESET_STATS(85, 72),
        },
    },
    // Unused: no enemy has it.
    {
        .first = ELEMENT_NONE,
        .elementLevels = {2, 2, 3, 2},
        .values = {
            PRESET_STATS(85, 100),
            PRESET_STATS(85, 72),
            PRESET_STATS(100, 127),
            PRESET_STATS(85, 100),
        },
    },
    // Gnome Mage
    {
        .first = ELEMENT_NONE,
        .elementLevels = {2, 2, 2, 3},
        .values = {
            PRESET_STATS(85, 48),
            PRESET_STATS(85, 72),
            PRESET_STATS(85, 72),
            PRESET_STATS(100, 100),
        },
    },
    // Nightmare
    {
        .first = ELEMENT_NONE,
        .elementLevels = {2, 2, 2, 3},
        .values = {
            PRESET_STATS(85, 72),
            PRESET_STATS(85, 100),
            PRESET_STATS(85, 100),
            PRESET_STATS(100, 127),
        },
    },
    // Mimic
    {
        .first = ELEMENT_NONE,
        .elementLevels = {3, 3, 0, 4},
        .values = {
            PRESET_STATS(95, 100),
            PRESET_STATS(100, 127),
            PRESET_STATS(75, 25),
            PRESET_STATS(105, 152),
        },
    },
    // Mystery Man and Saturos
    {
        .first = ELEMENT_NONE,
        .elementLevels = {7, 5, 10, 7},
        .values = {
            PRESET_STATS(100, 100),
            PRESET_STATS(90, 72),
            PRESET_STATS(110, 175),
            PRESET_STATS(100, 127),
        },
    },
    // Mystery Woman and Menardi
    {
        .first = ELEMENT_NONE,
        .elementLevels = {8, 6, 11, 8},
        .values = {
            PRESET_STATS(100, 127),
            PRESET_STATS(95, 72),
            PRESET_STATS(105, 175),
            PRESET_STATS(100, 100),
        },
    },
    // Fusion Dragon
    {
        .first = ELEMENT_MARS,
        .elementLevels = {9, 7, 14, 9},
        .values = {
            PRESET_STATS(100, 127),
            PRESET_STATS(95, 72),
            PRESET_STATS(110, 175),
            PRESET_STATS(100, 127),
        },
    },
    // Deadbeard
    {
        .first = ELEMENT_NONE,
        .elementLevels = {4, 8, 6, 5},
        .values = {
            PRESET_STATS(100, 72),
            PRESET_STATS(130, 193),
            PRESET_STATS(120, 127),
            PRESET_STATS(110, 152),
        },
    },
    // Gryphon, Wild Gryphon and Wise Gryphon
    {
        .first = ELEMENT_NONE,
        .elementLevels = {0, 2, 1, 4},
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
