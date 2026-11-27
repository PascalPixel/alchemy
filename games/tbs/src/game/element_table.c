/* @file */
/* The element tables: class series by element, element power and resistance */
/* by level, the enemies' element presets and the power and resistance curve. */

#include "game/game.h"

/* Class_SeriesByElements, read as [strongest * 4 + second], a row for each */
/* strongest element and a column for each second, in element order (Venus, */
/* Mercury, Mars, Jupiter). */
/* clang-format off */
const s32 Class_SeriesByElements[] = {
    CLASS_SERIES_SQUIRE, CLASS_SERIES_SWORDSMAN, CLASS_SERIES_BRUTE, CLASS_SERIES_APPRENTICE,
    CLASS_SERIES_SEER, CLASS_SERIES_WATER_SEER, CLASS_SERIES_PILGRIM, CLASS_SERIES_HERMIT,
    CLASS_SERIES_BRUTE, CLASS_SERIES_SWORDSMAN, CLASS_SERIES_GUARD, CLASS_SERIES_APPRENTICE,
    CLASS_SERIES_SEER, CLASS_SERIES_HERMIT, CLASS_SERIES_PILGRIM, CLASS_SERIES_WIND_SEER,
};

/* A character's base power and resistance in an element, by its whole element */
/* level 0 to 15; gmUnitCalcBaseElements adds the level's tenths to both. */
ELEMENT_LEVEL_BONUS Element_PowerResistByLevel[16]
    __attribute__((section(".rodata"))) = {
    {75, 85},   {80, 90},   {85, 95},   {90, 100},
    {95, 105},  {100, 110}, {105, 115}, {110, 120},
    {115, 125}, {120, 130}, {125, 135}, {130, 140},
    {135, 145}, {140, 150}, {145, 160}, {150, 165},
};

/* The 44 enemy element presets, by ENEMY_DEFINITION.elementPreset; each is */
/* named by the enemies that have it, in the English release's words. */
const ENEMY_ELEMENT_PRESET Enemy_ElementPresetTable[] = {
    /* Unused: only ??? has it, and ??? is in no formation. */
    [ELEMENT_PRESET_DEFAULT] = {
        .attackElement = ELEMENT_NONE,
        .elementLevels = {1, 1, 1, 1},
        .stats = {
            {.power = 100, .resistance = 100},
            {.power = 100, .resistance = 100},
            {.power = 100, .resistance = 100},
            {.power = 100, .resistance = 100},
        },
    },
    /* Bandit, Thief and Brigand */
    [ELEMENT_PRESET_BANDIT] = {
        .attackElement = ELEMENT_NONE,
        .elementLevels = {1, 1, 1, 1},
        .stats = {
            {.power = 80, .resistance = 72},
            {.power = 80, .resistance = 72},
            {.power = 80, .resistance = 72},
            {.power = 80, .resistance = 72},
        },
    },
    /* Azart, Satrage and Navampa */
    [ELEMENT_PRESET_AZART] = {
        .attackElement = ELEMENT_NONE,
        .elementLevels = {1, 1, 1, 1},
        .stats = {
            {.power = 80, .resistance = 90},
            {.power = 80, .resistance = 90},
            {.power = 80, .resistance = 90},
            {.power = 80, .resistance = 90},
        },
    },
    /* Vermin, Mad Vermin, Rat, Armored Rat, Plated Rat, Troll, Cave Troll, Brutal Troll, Mauler, Ravager, Mole, Mad Mole, Mole Mage and Ape */
    [ELEMENT_PRESET_VERMIN] = {
        .attackElement = ELEMENT_NONE,
        .elementLevels = {1, 0, 0, 0},
        .stats = {
            {.power = 100, .resistance = 48},
            {.power = 80, .resistance = 48},
            {.power = 70, .resistance = 25},
            {.power = 80, .resistance = 72},
        },
    },
    /* Grisly, Killer Ape and Dirty Ape */
    [ELEMENT_PRESET_GRISLY] = {
        .attackElement = ELEMENT_NONE,
        .elementLevels = {1, 0, 0, 0},
        .stats = {
            {.power = 100, .resistance = 72},
            {.power = 80, .resistance = 72},
            {.power = 70, .resistance = 25},
            {.power = 80, .resistance = 127},
        },
    },
    /* Spider, Tarantula, Recluse, Grub, Worm and Acid Maggot */
    [ELEMENT_PRESET_SPIDER] = {
        .attackElement = ELEMENT_NONE,
        .elementLevels = {1, 0, 0, 0},
        .stats = {
            {.power = 95, .resistance = 48},
            {.power = 85, .resistance = 48},
            {.power = 90, .resistance = 25},
            {.power = 85, .resistance = 48},
        },
    },
    /* Unused: no enemy has it. */
    {
        .attackElement = ELEMENT_NONE,
        .elementLevels = {1, 0, 0, 0},
        .stats = {
            {.power = 95, .resistance = 72},
            {.power = 85, .resistance = 72},
            {.power = 90, .resistance = 48},
            {.power = 85, .resistance = 100},
        },
    },
    /* Wild Mushroom and Death Cap */
    [ELEMENT_PRESET_WILD_MUSHROOM] = {
        .attackElement = ELEMENT_NONE,
        .elementLevels = {2, 1, 0, 0},
        .stats = {
            {.power = 105, .resistance = 48},
            {.power = 100, .resistance = 100},
            {.power = 80, .resistance = 7},
            {.power = 70, .resistance = 72},
        },
    },
    /* Tret */
    [ELEMENT_PRESET_TRET] = {
        .attackElement = ELEMENT_NONE,
        .elementLevels = {2, 1, 0, 0},
        .stats = {
            {.power = 105, .resistance = 72},
            {.power = 100, .resistance = 175},
            {.power = 80, .resistance = 25},
            {.power = 70, .resistance = 100},
        },
    },
    /* Drone Bee, Fighter Bee and Warrior Bee */
    [ELEMENT_PRESET_DRONE_BEE] = {
        .attackElement = ELEMENT_NONE,
        .elementLevels = {1, 0, 0, 2},
        .stats = {
            {.power = 90, .resistance = 100},
            {.power = 90, .resistance = 48},
            {.power = 95, .resistance = 48},
            {.power = 105, .resistance = 7},
        },
    },
    /* Unused: no enemy has it. */
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
    /* Bat, Rabid Bat, Harpy, Virago, Dirge, Foul Dirge and Vile Dirge */
    [ELEMENT_PRESET_BAT] = {
        .attackElement = ELEMENT_NONE,
        .elementLevels = {1, 1, 1, 2},
        .stats = {
            {.power = 95, .resistance = 127},
            {.power = 95, .resistance = 72},
            {.power = 95, .resistance = 48},
            {.power = 110, .resistance = 7},
        },
    },
    /* Harridan */
    [ELEMENT_PRESET_HARRIDAN] = {
        .attackElement = ELEMENT_NONE,
        .elementLevels = {1, 1, 1, 2},
        .stats = {
            {.power = 95, .resistance = 175},
            {.power = 95, .resistance = 100},
            {.power = 95, .resistance = 72},
            {.power = 110, .resistance = 25},
        },
    },
    /* Zombie, Undead, Wight, Ghoul, Fiendish Ghoul and Cannibal Ghoul */
    [ELEMENT_PRESET_ZOMBIE] = {
        .attackElement = ELEMENT_NONE,
        .elementLevels = {2, 1, 0, 0},
        .stats = {
            {.power = 100, .resistance = 25},
            {.power = 95, .resistance = 72},
            {.power = 80, .resistance = 7},
            {.power = 80, .resistance = 48},
        },
    },
    /* Skeleton, Bone Fighter, Skull Warrior, Will Head, Death Head and Willowisp */
    [ELEMENT_PRESET_SKELETON] = {
        .attackElement = ELEMENT_NONE,
        .elementLevels = {2, 1, 0, 0},
        .stats = {
            {.power = 100, .resistance = 25},
            {.power = 95, .resistance = 127},
            {.power = 80, .resistance = 72},
            {.power = 80, .resistance = 48},
        },
    },
    /* Ghost, Ghost Mage, Horned Ghost, Amaze, Creeper and Spirit */
    [ELEMENT_PRESET_GHOST] = {
        .attackElement = ELEMENT_NONE,
        .elementLevels = {0, 1, 1, 2},
        .stats = {
            {.power = 100, .resistance = 48},
            {.power = 95, .resistance = 100},
            {.power = 95, .resistance = 72},
            {.power = 95, .resistance = 7},
        },
    },
    /* Lich */
    [ELEMENT_PRESET_LICH] = {
        .attackElement = ELEMENT_NONE,
        .elementLevels = {0, 1, 1, 2},
        .stats = {
            {.power = 100, .resistance = 72},
            {.power = 95, .resistance = 152},
            {.power = 95, .resistance = 100},
            {.power = 95, .resistance = 25},
        },
    },
    /* Gargoyle, Clay Gargoyle and Ice Gargoyle */
    [ELEMENT_PRESET_GARGOYLE] = {
        .attackElement = ELEMENT_NONE,
        .elementLevels = {2, 1, 1, 1},
        .stats = {
            {.power = 100, .resistance = 48},
            {.power = 100, .resistance = 100},
            {.power = 100, .resistance = 100},
            {.power = 95, .resistance = 127},
        },
    },
    /* Unused: no enemy has it. */
    {
        .attackElement = ELEMENT_NONE,
        .elementLevels = {4, 2, 2, 0},
        .stats = {
            {.power = 110, .resistance = 152},
            {.power = 95, .resistance = 100},
            {.power = 95, .resistance = 100},
            {.power = 90, .resistance = 48},
        },
    },
    /* Cerebus, Fenrir, Kraken, Toadonpa, Poison Toad and ? (ENEMY_UNUSED_TOADONPA) */
    [ELEMENT_PRESET_CEREBUS] = {
        .attackElement = ELEMENT_NONE,
        .elementLevels = {2, 4, 0, 2},
        .stats = {
            {.power = 95, .resistance = 100},
            {.power = 115, .resistance = 175},
            {.power = 85, .resistance = 25},
            {.power = 95, .resistance = 100},
        },
    },
    /* Manticore, Magicore, Manticore King, Dread Hound, Chimera, Chimera Mage, ? (ENEMY_UNUSED_CHIMERA), Ant Lion, Roach and Doodle Bug */
    [ELEMENT_PRESET_MANTICORE] = {
        .attackElement = ELEMENT_NONE,
        .elementLevels = {2, 0, 4, 2},
        .stats = {
            {.power = 95, .resistance = 100},
            {.power = 85, .resistance = 25},
            {.power = 115, .resistance = 175},
            {.power = 95, .resistance = 100},
        },
    },
    /* Tornado Lizard, Storm Lizard and Tempest Lizard */
    [ELEMENT_PRESET_TORNADO_LIZARD] = {
        .attackElement = ELEMENT_NONE,
        .elementLevels = {0, 2, 2, 4},
        .stats = {
            {.power = 90, .resistance = 48},
            {.power = 95, .resistance = 100},
            {.power = 95, .resistance = 100},
            {.power = 110, .resistance = 152},
        },
    },
    /* Unused: no enemy has it. */
    {
        .attackElement = ELEMENT_VENUS,
        .elementLevels = {6, 2, 2, 0},
        .stats = {
            {.power = 120, .resistance = 100},
            {.power = 95, .resistance = 48},
            {.power = 95, .resistance = 48},
            {.power = 80, .resistance = 7},
        },
    },
    /* Earth Lizard, Golem, Earth Golem, Grand Golem and Venus Djinni */
    [ELEMENT_PRESET_EARTH_LIZARD] = {
        .attackElement = ELEMENT_VENUS,
        .elementLevels = {6, 2, 2, 0},
        .stats = {
            {.power = 120, .resistance = 193},
            {.power = 95, .resistance = 100},
            {.power = 95, .resistance = 100},
            {.power = 80, .resistance = 25},
        },
    },
    /* Slime, Ooze, Slime Beast, Cuttle, Calamar and Man o' War */
    [ELEMENT_PRESET_SLIME] = {
        .attackElement = ELEMENT_MERCURY,
        .elementLevels = {2, 6, 0, 2},
        .stats = {
            {.power = 95, .resistance = 48},
            {.power = 125, .resistance = 127},
            {.power = 75, .resistance = 7},
            {.power = 95, .resistance = 48},
        },
    },
    /* Living Statue, Hydros Statue and Mercury Djinni */
    [ELEMENT_PRESET_LIVING_STATUE] = {
        .attackElement = ELEMENT_MERCURY,
        .elementLevels = {2, 6, 0, 2},
        .stats = {
            {.power = 95, .resistance = 100},
            {.power = 125, .resistance = 193},
            {.power = 75, .resistance = 25},
            {.power = 95, .resistance = 100},
        },
    },
    /* Stone Soldier */
    [ELEMENT_PRESET_STONE_SOLDIER] = {
        .attackElement = ELEMENT_MARS,
        .elementLevels = {2, 0, 6, 2},
        .stats = {
            {.power = 95, .resistance = 48},
            {.power = 75, .resistance = 7},
            {.power = 125, .resistance = 127},
            {.power = 95, .resistance = 48},
        },
    },
    /* Salamander, Boulder Beast, Raging Rock and Mars Djinni */
    [ELEMENT_PRESET_SALAMANDER] = {
        .attackElement = ELEMENT_MARS,
        .elementLevels = {2, 0, 6, 2},
        .stats = {
            {.power = 95, .resistance = 100},
            {.power = 75, .resistance = 25},
            {.power = 125, .resistance = 193},
            {.power = 95, .resistance = 100},
        },
    },
    /* Unused: no enemy has it. */
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
    /* Thunder Lizard and Jupiter Djinni */
    [ELEMENT_PRESET_THUNDER_LIZARD] = {
        .attackElement = ELEMENT_JUPITER,
        .elementLevels = {0, 2, 2, 6},
        .stats = {
            {.power = 80, .resistance = 25},
            {.power = 95, .resistance = 100},
            {.power = 95, .resistance = 100},
            {.power = 120, .resistance = 193},
        },
    },
    /* Gnome Wizard, Orc, Orc Captain, Kobold and Goblin */
    [ELEMENT_PRESET_GNOME_WIZARD] = {
        .attackElement = ELEMENT_NONE,
        .elementLevels = {3, 2, 2, 2},
        .stats = {
            {.power = 100, .resistance = 100},
            {.power = 85, .resistance = 72},
            {.power = 85, .resistance = 72},
            {.power = 85, .resistance = 48},
        },
    },
    /* Orc Lord and Hobgoblin */
    [ELEMENT_PRESET_ORC_LORD] = {
        .attackElement = ELEMENT_NONE,
        .elementLevels = {3, 2, 2, 2},
        .stats = {
            {.power = 100, .resistance = 127},
            {.power = 85, .resistance = 100},
            {.power = 85, .resistance = 100},
            {.power = 85, .resistance = 72},
        },
    },
    /* Rat Soldier, Rat Fighter and Rat Warrior */
    [ELEMENT_PRESET_RAT_SOLDIER] = {
        .attackElement = ELEMENT_NONE,
        .elementLevels = {2, 3, 2, 2},
        .stats = {
            {.power = 85, .resistance = 72},
            {.power = 100, .resistance = 100},
            {.power = 85, .resistance = 48},
            {.power = 85, .resistance = 72},
        },
    },
    /* Siren, Succubus, Lizard Man, Lizard Fighter and Lizard King */
    [ELEMENT_PRESET_SIREN] = {
        .attackElement = ELEMENT_NONE,
        .elementLevels = {2, 3, 2, 2},
        .stats = {
            {.power = 85, .resistance = 100},
            {.power = 100, .resistance = 127},
            {.power = 85, .resistance = 72},
            {.power = 85, .resistance = 100},
        },
    },
    /* Gnome */
    [ELEMENT_PRESET_GNOME] = {
        .attackElement = ELEMENT_NONE,
        .elementLevels = {2, 2, 3, 2},
        .stats = {
            {.power = 85, .resistance = 72},
            {.power = 85, .resistance = 48},
            {.power = 100, .resistance = 100},
            {.power = 85, .resistance = 72},
        },
    },
    /* Unused: no enemy has it. */
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
    /* Gnome Mage */
    [ELEMENT_PRESET_GNOME_MAGE] = {
        .attackElement = ELEMENT_NONE,
        .elementLevels = {2, 2, 2, 3},
        .stats = {
            {.power = 85, .resistance = 48},
            {.power = 85, .resistance = 72},
            {.power = 85, .resistance = 72},
            {.power = 100, .resistance = 100},
        },
    },
    /* Nightmare */
    [ELEMENT_PRESET_NIGHTMARE] = {
        .attackElement = ELEMENT_NONE,
        .elementLevels = {2, 2, 2, 3},
        .stats = {
            {.power = 85, .resistance = 72},
            {.power = 85, .resistance = 100},
            {.power = 85, .resistance = 100},
            {.power = 100, .resistance = 127},
        },
    },
    /* Mimic */
    [ELEMENT_PRESET_MIMIC] = {
        .attackElement = ELEMENT_NONE,
        .elementLevels = {3, 3, 0, 4},
        .stats = {
            {.power = 95, .resistance = 100},
            {.power = 100, .resistance = 127},
            {.power = 75, .resistance = 25},
            {.power = 105, .resistance = 152},
        },
    },
    /* Mystery Man and Saturos */
    [ELEMENT_PRESET_SATUROS] = {
        .attackElement = ELEMENT_NONE,
        .elementLevels = {7, 5, 10, 7},
        .stats = {
            {.power = 100, .resistance = 100},
            {.power = 90, .resistance = 72},
            {.power = 110, .resistance = 175},
            {.power = 100, .resistance = 127},
        },
    },
    /* Mystery Woman and Menardi */
    [ELEMENT_PRESET_MENARDI] = {
        .attackElement = ELEMENT_NONE,
        .elementLevels = {8, 6, 11, 8},
        .stats = {
            {.power = 100, .resistance = 127},
            {.power = 95, .resistance = 72},
            {.power = 105, .resistance = 175},
            {.power = 100, .resistance = 100},
        },
    },
    /* Fusion Dragon */
    [ELEMENT_PRESET_FUSION_DRAGON] = {
        .attackElement = ELEMENT_MARS,
        .elementLevels = {9, 7, 14, 9},
        .stats = {
            {.power = 100, .resistance = 127},
            {.power = 95, .resistance = 72},
            {.power = 110, .resistance = 175},
            {.power = 100, .resistance = 127},
        },
    },
    /* Deadbeard */
    [ELEMENT_PRESET_DEADBEARD] = {
        .attackElement = ELEMENT_NONE,
        .elementLevels = {4, 8, 6, 5},
        .stats = {
            {.power = 100, .resistance = 72},
            {.power = 130, .resistance = 193},
            {.power = 120, .resistance = 127},
            {.power = 110, .resistance = 152},
        },
    },
    /* Gryphon, Wild Gryphon and Wise Gryphon */
    [ELEMENT_PRESET_GRYPHON] = {
        .attackElement = ELEMENT_NONE,
        .elementLevels = {0, 2, 1, 4},
        .stats = {
            {.power = 90, .resistance = 152},
            {.power = 95, .resistance = 110},
            {.power = 95, .resistance = 72},
            {.power = 110, .resistance = 48},
        },
    },
};

const s16 Element_PowerResistCurve[5][2] = {
    {200, 256},
    {100, 128},
    {0, 0},
    {-100, -128},
    {-200, -256},
};
/* clang-format on */
