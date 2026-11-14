/// @file
/// The 734 abilities, by ability id. Each name is the English release's
/// (MsgAbilityNothing plus the ability id).
// Part of game.c.

#include "../game_internal.h"

// clang-format off
const struct AbilityDefinition gAbilityDefinitions[] = {
    // Nothing
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_NONE,
        .range = 1,
    },
    // Attack
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_NONE,
        .range = 1,
    },
    // Defend
    {
        .targetMode = ABILITY_TARGET_SELF,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_NO_POWER,
        .element = ELEMENT_NONE,
        .effect = ABILITY_EFFECT_DEFEND,
        .range = 1,
    },
    // Quake
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .icon = 1,
        .range = 2,
        .ppCost = 4,
        .power = 12,
    },
    // Earthquake
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .icon = 2,
        .range = 3,
        .ppCost = 7,
        .power = 35,
    },
    // Quake Sphere
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .icon = 3,
        .range = 4,
        .ppCost = 15,
        .power = 65,
    },
    // Spire
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .icon = 4,
        .range = 1,
        .ppCost = 5,
        .power = 40,
    },
    // Clay Spire
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .icon = 5,
        .range = 2,
        .ppCost = 13,
        .power = 85,
    },
    // Stone Spire
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .icon = 6,
        .range = 2,
        .ppCost = 22,
        .power = 160,
    },
    // Gaia
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .icon = 7,
        .range = 2,
        .ppCost = 7,
        .power = 40,
    },
    // Mother Gaia
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .icon = 8,
        .range = 3,
        .ppCost = 17,
        .power = 100,
    },
    // Grand Gaia
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .icon = 9,
        .range = 3,
        .ppCost = 32,
        .power = 200,
    },
    // Growth
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .icon = 10,
        .fieldPsynergy = FIELD_PSYNERGY_GROWTH,
        .range = 1,
        .ppCost = 4,
        .power = 25,
    },
    // Mad Growth
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .icon = 11,
        .range = 2,
        .ppCost = 10,
        .power = 60,
    },
    // Wild Growth
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .icon = 12,
        .range = 3,
        .ppCost = 19,
        .power = 110,
    },
    // Thorn
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .icon = 13,
        .range = 2,
        .ppCost = 6,
        .power = 35,
    },
    // Briar
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .icon = 14,
        .range = 2,
        .ppCost = 11,
        .power = 70,
    },
    // Nettle
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .icon = 15,
        .range = 3,
        .ppCost = 23,
        .power = 140,
    },
    // ?
    {0},
    // ?
    {0},
    // ?
    {0},
    // ?
    {0},
    // ?
    {0},
    // ?
    {0},
    // Frost
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_MERCURY,
        .icon = 16,
        .fieldPsynergy = FIELD_PSYNERGY_FROST,
        .range = 2,
        .ppCost = 5,
        .power = 20,
    },
    // Tundra
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_MERCURY,
        .icon = 17,
        .range = 2,
        .ppCost = 8,
        .power = 45,
    },
    // Glacier
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_MERCURY,
        .icon = 18,
        .range = 2,
        .ppCost = 15,
        .power = 100,
    },
    // Ice
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_MERCURY,
        .icon = 19,
        .range = 1,
        .ppCost = 5,
        .power = 35,
    },
    // Ice Horn
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_MERCURY,
        .icon = 20,
        .range = 2,
        .ppCost = 11,
        .power = 70,
    },
    // Ice Missile
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_MERCURY,
        .icon = 21,
        .range = 2,
        .ppCost = 23,
        .power = 160,
    },
    // Prism
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_MERCURY,
        .icon = 22,
        .range = 2,
        .ppCost = 7,
        .power = 40,
    },
    // Hail Prism
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_MERCURY,
        .icon = 23,
        .range = 3,
        .ppCost = 16,
        .power = 90,
    },
    // Freeze Prism
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_MERCURY,
        .icon = 24,
        .range = 3,
        .ppCost = 31,
        .power = 190,
    },
    // Douse
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_MERCURY,
        .icon = 28,
        .fieldPsynergy = FIELD_PSYNERGY_DOUSE,
        .range = 2,
        .ppCost = 5,
        .power = 25,
    },
    // Drench
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_MERCURY,
        .icon = 29,
        .range = 2,
        .ppCost = 10,
        .power = 60,
    },
    // Deluge
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_MERCURY,
        .icon = 30,
        .range = 3,
        .ppCost = 20,
        .power = 120,
    },
    // Froth
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_MERCURY,
        .icon = 25,
        .range = 2,
        .ppCost = 5,
        .power = 28,
    },
    // Froth Sphere
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_MERCURY,
        .icon = 26,
        .range = 3,
        .ppCost = 12,
        .power = 65,
    },
    // Froth Spiral
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_MERCURY,
        .icon = 27,
        .range = 4,
        .ppCost = 31,
        .power = 150,
    },
    // Cool
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_MERCURY,
        .icon = 173,
        .range = 2,
        .ppCost = 6,
        .power = 35,
    },
    // Supercool
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_MERCURY,
        .icon = 174,
        .range = 3,
        .ppCost = 14,
        .power = 80,
    },
    // Megacool
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_MERCURY,
        .icon = 175,
        .range = 4,
        .ppCost = 33,
        .power = 180,
    },
    // ?
    {0},
    // ?
    {0},
    // ?
    {0},
    // Flare
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_MARS,
        .icon = 31,
        .range = 2,
        .ppCost = 4,
        .power = 15,
    },
    // Flare Wall
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_MARS,
        .icon = 32,
        .range = 2,
        .ppCost = 7,
        .power = 40,
    },
    // Flare Storm
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_MARS,
        .icon = 33,
        .range = 2,
        .ppCost = 12,
        .power = 80,
    },
    // Fire
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_MARS,
        .icon = 34,
        .range = 2,
        .ppCost = 6,
        .power = 35,
    },
    // Fireball
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_MARS,
        .icon = 35,
        .range = 3,
        .ppCost = 12,
        .power = 65,
    },
    // Inferno
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_MARS,
        .icon = 36,
        .range = 3,
        .ppCost = 23,
        .power = 140,
    },
    // Volcano
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_MARS,
        .icon = 37,
        .range = 1,
        .ppCost = 6,
        .power = 45,
    },
    // Eruption
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_MARS,
        .icon = 38,
        .range = 2,
        .ppCost = 14,
        .power = 90,
    },
    // Pyroclasm
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_MARS,
        .icon = 39,
        .range = 3,
        .ppCost = 29,
        .power = 180,
    },
    // Blast
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_MARS,
        .icon = 40,
        .range = 2,
        .ppCost = 5,
        .power = 25,
    },
    // Mad Blast
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_MARS,
        .icon = 41,
        .range = 2,
        .ppCost = 9,
        .power = 50,
    },
    // Fiery Blast
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_MARS,
        .icon = 42,
        .range = 3,
        .ppCost = 19,
        .power = 110,
    },
    // Blast
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_MARS,
        .icon = 43,
        .range = 2,
        .ppCost = 7,
        .power = 40,
    },
    // Nova
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_MARS,
        .icon = 44,
        .range = 3,
        .ppCost = 13,
        .power = 70,
    },
    // Supernova
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_MARS,
        .icon = 45,
        .range = 4,
        .ppCost = 31,
        .power = 150,
    },
    // Fume
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_MARS,
        .icon = 176,
        .range = 1,
        .ppCost = 6,
        .power = 50,
    },
    // Serpent Fume
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_MARS,
        .icon = 177,
        .range = 1,
        .ppCost = 14,
        .power = 130,
    },
    // Dragon Fume
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_MARS,
        .icon = 178,
        .range = 1,
        .ppCost = 35,
        .power = 230,
    },
    // Beam
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_MARS,
        .icon = 179,
        .range = 2,
        .ppCost = 7,
        .power = 45,
    },
    // Cycle Beam
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_MARS,
        .icon = 180,
        .range = 3,
        .ppCost = 14,
        .power = 80,
    },
    // Searing Beam
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_MARS,
        .icon = 181,
        .range = 4,
        .ppCost = 36,
        .power = 170,
    },
    // Bolt
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_JUPITER,
        .icon = 46,
        .range = 1,
        .ppCost = 4,
        .power = 20,
    },
    // Flash Bolt
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_JUPITER,
        .icon = 47,
        .range = 2,
        .ppCost = 7,
        .power = 40,
    },
    // Blue Bolt
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_JUPITER,
        .icon = 48,
        .range = 2,
        .ppCost = 14,
        .power = 90,
    },
    // Ray
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_JUPITER,
        .icon = 49,
        .range = 2,
        .ppCost = 6,
        .power = 35,
    },
    // Storm Ray
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_JUPITER,
        .icon = 50,
        .range = 2,
        .ppCost = 10,
        .power = 65,
    },
    // Destruct Ray
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_JUPITER,
        .icon = 51,
        .range = 2,
        .ppCost = 21,
        .power = 150,
    },
    // Plasma
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_JUPITER,
        .icon = 52,
        .range = 2,
        .ppCost = 8,
        .power = 45,
    },
    // Shine Plasma
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_JUPITER,
        .icon = 53,
        .range = 3,
        .ppCost = 18,
        .power = 100,
    },
    // Spark Plasma
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_JUPITER,
        .icon = 54,
        .range = 4,
        .ppCost = 37,
        .power = 180,
    },
    // Slash
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_JUPITER,
        .icon = 55,
        .range = 1,
        .ppCost = 4,
        .power = 25,
    },
    // Wind Slash
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_JUPITER,
        .icon = 56,
        .range = 2,
        .ppCost = 9,
        .power = 50,
    },
    // Sonic Slash
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_JUPITER,
        .icon = 57,
        .range = 3,
        .ppCost = 20,
        .power = 120,
    },
    // Whirlwind
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_JUPITER,
        .icon = 58,
        .fieldPsynergy = FIELD_PSYNERGY_WHIRLWIND,
        .range = 2,
        .ppCost = 5,
        .power = 20,
    },
    // Tornado
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_JUPITER,
        .icon = 59,
        .range = 3,
        .ppCost = 14,
        .power = 80,
    },
    // Tempest
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_JUPITER,
        .icon = 60,
        .range = 3,
        .ppCost = 27,
        .power = 160,
    },
    // ?
    {0},
    // ?
    {0},
    // ?
    {0},
    // ?
    {0},
    // ?
    {0},
    // ?
    {0},
    // Aura
    {
        .targetMode = ABILITY_TARGET_ALLIES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_USABLE_IN_FIELD | ABILITY_FORMULA_HEAL_HP,
        .element = ELEMENT_MARS,
        .icon = 182,
        .range = ABILITY_RANGE_ALL,
        .ppCost = 7,
        .power = 50,
    },
    // Healing Aura
    {
        .targetMode = ABILITY_TARGET_ALLIES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_USABLE_IN_FIELD | ABILITY_FORMULA_HEAL_HP,
        .element = ELEMENT_MARS,
        .icon = 183,
        .range = ABILITY_RANGE_ALL,
        .ppCost = 11,
        .power = 100,
    },
    // Cool Aura
    {
        .targetMode = ABILITY_TARGET_ALLIES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_USABLE_IN_FIELD | ABILITY_FORMULA_HEAL_HP,
        .element = ELEMENT_MARS,
        .icon = 184,
        .range = ABILITY_RANGE_ALL,
        .ppCost = 16,
        .power = 200,
    },
    // Cure
    {
        .targetMode = ABILITY_TARGET_ALLIES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_USABLE_IN_FIELD | ABILITY_FORMULA_HEAL_HP,
        .icon = 61,
        .range = 1,
        .ppCost = 3,
        .power = 70,
    },
    // Cure Well
    {
        .targetMode = ABILITY_TARGET_ALLIES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_USABLE_IN_FIELD | ABILITY_FORMULA_HEAL_HP,
        .icon = 62,
        .range = 1,
        .ppCost = 7,
        .power = 150,
    },
    // Potent Cure
    {
        .targetMode = ABILITY_TARGET_ALLIES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_USABLE_IN_FIELD | ABILITY_FORMULA_HEAL_HP,
        .icon = 63,
        .range = 1,
        .ppCost = 10,
        .power = 300,
    },
    // Ply
    {
        .targetMode = ABILITY_TARGET_ALLIES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_USABLE_IN_FIELD | ABILITY_FORMULA_HEAL_HP,
        .element = ELEMENT_MERCURY,
        .icon = 64,
        .fieldPsynergy = FIELD_PSYNERGY_PLY,
        .range = 1,
        .ppCost = 4,
        .power = 100,
    },
    // Ply Well
    {
        .targetMode = ABILITY_TARGET_ALLIES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_USABLE_IN_FIELD | ABILITY_FORMULA_HEAL_HP,
        .element = ELEMENT_MERCURY,
        .icon = 65,
        .fieldPsynergy = FIELD_PSYNERGY_PLY,
        .range = 1,
        .ppCost = 8,
        .power = 200,
    },
    // Pure Ply
    {
        .targetMode = ABILITY_TARGET_ALLIES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_USABLE_IN_FIELD | ABILITY_FORMULA_HEAL_HP,
        .element = ELEMENT_MERCURY,
        .icon = 66,
        .fieldPsynergy = FIELD_PSYNERGY_PLY,
        .range = 1,
        .ppCost = 12,
        .power = 1000,
    },
    // Wish
    {
        .targetMode = ABILITY_TARGET_ALLIES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_USABLE_IN_FIELD | ABILITY_FORMULA_HEAL_HP,
        .element = ELEMENT_MERCURY,
        .icon = 67,
        .range = ABILITY_RANGE_ALL,
        .ppCost = 9,
        .power = 80,
    },
    // Wish Well
    {
        .targetMode = ABILITY_TARGET_ALLIES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_USABLE_IN_FIELD | ABILITY_FORMULA_HEAL_HP,
        .element = ELEMENT_MERCURY,
        .icon = 68,
        .range = ABILITY_RANGE_ALL,
        .ppCost = 13,
        .power = 160,
    },
    // Pure Wish
    {
        .targetMode = ABILITY_TARGET_ALLIES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_USABLE_IN_FIELD | ABILITY_FORMULA_HEAL_HP,
        .element = ELEMENT_MERCURY,
        .icon = 69,
        .range = ABILITY_RANGE_ALL,
        .ppCost = 20,
        .power = 400,
    },
    // Cure Poison
    {
        .targetMode = ABILITY_TARGET_ALLIES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_USABLE_IN_FIELD | ABILITY_FORMULA_EFFECT,
        .element = ELEMENT_MERCURY,
        .effect = ABILITY_EFFECT_CURE_POISON,
        .icon = 70,
        .range = 1,
        .ppCost = 2,
    },
    // Restore
    {
        .targetMode = ABILITY_TARGET_ALLIES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_EFFECT,
        .element = ELEMENT_MERCURY,
        .effect = ABILITY_EFFECT_RESTORE,
        .icon = 71,
        .range = 1,
        .ppCost = 3,
    },
    // Revive
    {
        .targetMode = ABILITY_TARGET_ALLIES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_USABLE_IN_FIELD | ABILITY_FORMULA_EFFECT,
        .effect = ABILITY_EFFECT_REVIVE,
        .icon = 72,
        .range = 1,
        .ppCost = 15,
    },
    // Impact
    {
        .targetMode = ABILITY_TARGET_ALLIES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_EFFECT,
        .element = ELEMENT_JUPITER,
        .effect = ABILITY_EFFECT_ATTACK_UP_2,
        .icon = 73,
        .range = 1,
        .ppCost = 7,
    },
    // High Impact
    {
        .targetMode = ABILITY_TARGET_ALLIES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_EFFECT,
        .element = ELEMENT_JUPITER,
        .effect = ABILITY_EFFECT_ATTACK_UP,
        .icon = 74,
        .range = ABILITY_RANGE_ALL,
        .ppCost = 12,
    },
    // Dull
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_EFFECT,
        .element = ELEMENT_JUPITER,
        .effect = ABILITY_EFFECT_ATTACK_DOWN_2,
        .icon = 75,
        .range = 1,
        .ppCost = 6,
    },
    // Blunt
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_EFFECT,
        .element = ELEMENT_JUPITER,
        .effect = ABILITY_EFFECT_ATTACK_DOWN,
        .icon = 76,
        .range = 2,
        .ppCost = 11,
    },
    // Guard
    {
        .targetMode = ABILITY_TARGET_ALLIES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_EFFECT,
        .element = ELEMENT_MARS,
        .effect = ABILITY_EFFECT_DEFENSE_UP_2,
        .icon = 77,
        .range = 1,
        .ppCost = 3,
    },
    // Protect
    {
        .targetMode = ABILITY_TARGET_ALLIES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_EFFECT,
        .element = ELEMENT_MARS,
        .effect = ABILITY_EFFECT_DEFENSE_UP,
        .icon = 78,
        .range = ABILITY_RANGE_ALL,
        .ppCost = 5,
    },
    // Impair
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_EFFECT,
        .element = ELEMENT_MARS,
        .effect = ABILITY_EFFECT_DEFENSE_DOWN_2,
        .icon = 79,
        .range = 1,
        .ppCost = 4,
    },
    // Debilitate
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_EFFECT,
        .element = ELEMENT_MARS,
        .effect = ABILITY_EFFECT_DEFENSE_DOWN,
        .icon = 80,
        .range = 2,
        .ppCost = 6,
    },
    // Ward
    {
        .targetMode = ABILITY_TARGET_ALLIES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_EFFECT,
        .element = ELEMENT_JUPITER,
        .effect = ABILITY_EFFECT_RESISTANCE_UP_2,
        .icon = 81,
        .range = 1,
        .ppCost = 3,
    },
    // Resist
    {
        .targetMode = ABILITY_TARGET_ALLIES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_EFFECT,
        .element = ELEMENT_JUPITER,
        .effect = ABILITY_EFFECT_RESISTANCE_UP,
        .icon = 82,
        .range = ABILITY_RANGE_ALL,
        .ppCost = 5,
    },
    // Weaken
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_EFFECT,
        .element = ELEMENT_JUPITER,
        .effect = ABILITY_EFFECT_RESISTANCE_DOWN_2,
        .icon = 83,
        .range = 1,
        .ppCost = 4,
    },
    // Enfeeble
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_EFFECT,
        .element = ELEMENT_JUPITER,
        .effect = ABILITY_EFFECT_RESISTANCE_DOWN,
        .icon = 84,
        .range = 2,
        .ppCost = 6,
    },
    // Taint
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_EFFECT,
        .effect = ABILITY_EFFECT_POISON,
        .range = 1,
        .ppCost = 4,
    },
    // Poison
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_EFFECT,
        .effect = ABILITY_EFFECT_DEADLY_POISON,
        .range = 1,
        .ppCost = 6,
    },
    // Delude
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_EFFECT,
        .element = ELEMENT_JUPITER,
        .effect = ABILITY_EFFECT_DELUSION,
        .icon = 85,
        .range = 2,
        .ppCost = 4,
    },
    // Confuse
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_EFFECT,
        .element = ELEMENT_JUPITER,
        .effect = ABILITY_EFFECT_CONFUSION,
        .range = 2,
        .ppCost = 6,
    },
    // Charm
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_EFFECT,
        .element = ELEMENT_JUPITER,
        .effect = ABILITY_EFFECT_CHARM,
        .range = 1,
        .ppCost = 9,
    },
    // Paralyze
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_EFFECT,
        .element = ELEMENT_JUPITER,
        .effect = ABILITY_EFFECT_STUN,
        .range = 1,
        .ppCost = 7,
    },
    // Sleep
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_EFFECT,
        .element = ELEMENT_JUPITER,
        .effect = ABILITY_EFFECT_SLEEP,
        .icon = 86,
        .range = 2,
        .ppCost = 5,
    },
    // Bind
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_EFFECT,
        .element = ELEMENT_JUPITER,
        .effect = ABILITY_EFFECT_SEAL,
        .icon = 87,
        .range = 1,
        .ppCost = 4,
    },
    // Haunt
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_EFFECT,
        .effect = ABILITY_EFFECT_HAUNT,
        .icon = 88,
        .range = 2,
        .ppCost = 5,
    },
    // Curse
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_EFFECT,
        .effect = ABILITY_EFFECT_CURSE,
        .icon = 90,
        .range = 1,
        .ppCost = 6,
    },
    // Condemn
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_EFFECT,
        .effect = ABILITY_EFFECT_INSTANT_DEATH,
        .icon = 89,
        .range = 1,
        .ppCost = 8,
    },
    // Drain
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_EFFECT,
        .element = ELEMENT_JUPITER,
        .effect = ABILITY_EFFECT_DRAIN_HP,
        .icon = 91,
        .range = 1,
        .ppCost = 3,
        .power = 50,
    },
    // Psy Drain
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_EFFECT,
        .element = ELEMENT_JUPITER,
        .effect = ABILITY_EFFECT_DRAIN_PP,
        .icon = 92,
        .range = 1,
        .power = 15,
    },
    // Break
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_EFFECT,
        .element = ELEMENT_MERCURY,
        .effect = ABILITY_EFFECT_BREAK,
        .icon = 93,
        .range = ABILITY_RANGE_ALL,
        .ppCost = 5,
    },
    // Regenerate
    {
        .targetMode = ABILITY_TARGET_ALLIES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_EFFECT,
        .element = ELEMENT_MERCURY,
        .effect = ABILITY_EFFECT_REGENERATE,
        .range = 1,
        .ppCost = 6,
        .power = 60,
    },
    // Reflect
    {
        .targetMode = ABILITY_TARGET_ALLIES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_EFFECT,
        .element = ELEMENT_MERCURY,
        .effect = ABILITY_EFFECT_REFLECT,
        .icon = 94,
        .range = 1,
        .ppCost = 5,
    },
    // ?
    {0},
    // ?
    {0},
    // ?
    {0},
    // Lash
    {
        .flags = ABILITY_FORMULA_NO_POWER,
        .element = ELEMENT_JUPITER,
        .icon = 159,
        .fieldPsynergy = FIELD_PSYNERGY_LASH,
        .range = 1,
        .ppCost = 1,
    },
    // Pound
    {
        .flags = ABILITY_FORMULA_NO_POWER,
        .element = ELEMENT_MARS,
        .icon = 160,
        .fieldPsynergy = FIELD_PSYNERGY_POUND,
        .range = 1,
        .ppCost = 2,
    },
    // Tremor
    {
        .flags = ABILITY_FORMULA_NO_POWER,
        .icon = 161,
        .fieldPsynergy = FIELD_PSYNERGY_TREMOR,
        .range = 1,
        .ppCost = 1,
    },
    // Scoop
    {
        .flags = ABILITY_FORMULA_NO_POWER,
        .icon = 162,
        .fieldPsynergy = FIELD_PSYNERGY_SCOOP,
        .range = 1,
        .ppCost = 1,
    },
    // Cyclone
    {
        .flags = ABILITY_FORMULA_NO_POWER,
        .element = ELEMENT_JUPITER,
        .icon = 167,
        .fieldPsynergy = FIELD_PSYNERGY_CYCLONE,
        .range = 1,
        .ppCost = 2,
    },
    // Parch
    {
        .flags = ABILITY_FORMULA_NO_POWER,
        .element = ELEMENT_MERCURY,
        .icon = 164,
        .fieldPsynergy = FIELD_PSYNERGY_PARCH,
        .range = 1,
        .ppCost = 2,
    },
    // Sand
    {
        .flags = ABILITY_FORMULA_NO_POWER,
        .icon = 165,
        .fieldPsynergy = FIELD_PSYNERGY_SAND,
        .range = 1,
        .ppCost = 2,
    },
    // Move
    {
        .flags = ABILITY_FORMULA_NO_POWER,
        .element = ELEMENT_MARS,
        .icon = 95,
        .fieldPsynergy = FIELD_PSYNERGY_MOVE,
        .range = 1,
        .ppCost = 2,
    },
    // Mind Read
    {
        .flags = ABILITY_FORMULA_NO_POWER,
        .element = ELEMENT_JUPITER,
        .icon = 96,
        .fieldPsynergy = FIELD_PSYNERGY_MIND_READ,
        .range = 1,
        .ppCost = 1,
    },
    // Force
    {
        .flags = ABILITY_FORMULA_NO_POWER,
        .element = ELEMENT_NONE,
        .icon = 97,
        .fieldPsynergy = FIELD_PSYNERGY_FORCE,
        .range = 1,
        .ppCost = 2,
    },
    // Lift
    {
        .flags = ABILITY_FORMULA_NO_POWER,
        .element = ELEMENT_MERCURY,
        .icon = 98,
        .fieldPsynergy = FIELD_PSYNERGY_LIFT,
        .range = 1,
        .ppCost = 2,
    },
    // Reveal
    {
        .flags = ABILITY_FORMULA_NO_POWER,
        .element = ELEMENT_JUPITER,
        .icon = 99,
        .fieldPsynergy = FIELD_PSYNERGY_REVEAL,
        .range = 1,
        .ppCost = 1,
    },
    // Halt
    {
        .flags = ABILITY_FORMULA_NO_POWER,
        .element = ELEMENT_JUPITER,
        .icon = 100,
        .fieldPsynergy = FIELD_PSYNERGY_HALT,
        .range = 1,
        .ppCost = 2,
    },
    // Cloak
    {
        .flags = ABILITY_FORMULA_NO_POWER,
        .element = ELEMENT_MERCURY,
        .icon = 101,
        .fieldPsynergy = FIELD_PSYNERGY_CLOAK,
        .range = 1,
        .ppCost = 1,
    },
    // Carry
    {
        .flags = ABILITY_FORMULA_NO_POWER,
        .icon = 102,
        .fieldPsynergy = FIELD_PSYNERGY_CARRY,
        .range = 1,
        .ppCost = 2,
    },
    // Catch
    {
        .flags = ABILITY_FORMULA_NO_POWER,
        .icon = 103,
        .fieldPsynergy = FIELD_PSYNERGY_CATCH,
        .range = 1,
        .ppCost = 1,
    },
    // Retreat
    {
        .flags = ABILITY_FORMULA_NO_POWER,
        .icon = 156,
        .fieldPsynergy = FIELD_PSYNERGY_RETREAT,
        .range = 1,
        .ppCost = 6,
    },
    // Avoid
    {
        .flags = ABILITY_FORMULA_NO_POWER,
        .element = ELEMENT_MERCURY,
        .icon = 157,
        .fieldPsynergy = FIELD_PSYNERGY_AVOID,
        .range = 1,
        .ppCost = 5,
    },
    // Burst
    {
        .flags = ABILITY_FORMULA_NO_POWER,
        .element = ELEMENT_MARS,
        .icon = 163,
        .fieldPsynergy = FIELD_PSYNERGY_BURST,
        .range = 1,
        .ppCost = 2,
    },
    // Grind
    {
        .flags = ABILITY_FORMULA_NO_POWER,
        .icon = 169,
        .fieldPsynergy = FIELD_PSYNERGY_GRIND,
        .range = 1,
        .ppCost = 2,
    },
    // Hover
    {
        .flags = ABILITY_FORMULA_NO_POWER,
        .element = ELEMENT_JUPITER,
        .icon = 166,
        .fieldPsynergy = FIELD_PSYNERGY_HOVER,
        .range = 1,
        .ppCost = 2,
    },
    // Blaze
    {
        .flags = ABILITY_FORMULA_NO_POWER,
        .element = ELEMENT_MARS,
        .icon = 168,
        .fieldPsynergy = FIELD_PSYNERGY_BLAZE,
        .range = 1,
        .ppCost = 1,
    },
    // Ma????
    {
        .flags = ABILITY_FORMULA_NO_POWER,
        .icon = 170,
        .fieldPsynergy = FIELD_PSYNERGY_UNNAMED_28,
        .range = 1,
        .ppCost = 1,
    },
    // Teleport
    {
        .flags = ABILITY_FORMULA_NO_POWER,
        .element = ELEMENT_JUPITER,
        .icon = 172,
        .fieldPsynergy = FIELD_PSYNERGY_TELEPORT,
        .range = 1,
        .ppCost = 3,
    },
    // A??
    {
        .flags = ABILITY_FORMULA_NO_POWER,
        .element = ELEMENT_JUPITER,
        .icon = 171,
        .fieldPsynergy = FIELD_PSYNERGY_UNNAMED_29,
        .range = 1,
        .ppCost = 2,
    },
    // ?
    {0},
    // ?
    {0},
    // Dragon Cloud
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_MARS,
        .icon = 117,
        .range = 1,
        .ppCost = 6,
        .power = 80,
    },
    // Demon Night
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .effect = ABILITY_EFFECT_HAUNT,
        .icon = 119,
        .range = 2,
        .ppCost = 12,
        .power = 60,
    },
    // Helm Splitter
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .effect = ABILITY_EFFECT_INSTANT_DEATH,
        .icon = 116,
        .range = 1,
        .ppCost = 8,
        .power = 30,
    },
    // Quick Strike
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK_MULTIPLIED,
        .element = ELEMENT_JUPITER,
        .icon = 118,
        .range = 1,
        .ppCost = 12,
        .power = 18,
    },
    // Rockfall
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .icon = 104,
        .range = 2,
        .ppCost = 5,
        .power = 30,
    },
    // Rockslide
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .icon = 105,
        .range = 3,
        .ppCost = 15,
        .power = 90,
    },
    // Avalanche
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .icon = 106,
        .range = 3,
        .ppCost = 30,
        .power = 160,
    },
    // Lava Shower
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_MARS,
        .icon = 107,
        .range = 1,
        .ppCost = 4,
        .power = 40,
    },
    // Molten Bath
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_MARS,
        .icon = 108,
        .range = 2,
        .ppCost = 12,
        .power = 70,
    },
    // Magma Storm
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_MARS,
        .icon = 109,
        .range = 3,
        .ppCost = 27,
        .power = 120,
    },
    // Demon Spear
    {
        .targetMode = ABILITY_TARGET_ALLIES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_EFFECT,
        .element = ELEMENT_JUPITER,
        .effect = ABILITY_EFFECT_ATTACK_UP_2,
        .icon = 110,
        .range = 1,
        .ppCost = 7,
    },
    // Angel Spear
    {
        .targetMode = ABILITY_TARGET_ALLIES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_EFFECT,
        .element = ELEMENT_JUPITER,
        .effect = ABILITY_EFFECT_ATTACK_UP,
        .icon = 111,
        .range = ABILITY_RANGE_ALL,
        .ppCost = 12,
    },
    // Guardian
    {
        .targetMode = ABILITY_TARGET_ALLIES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_EFFECT,
        .element = ELEMENT_MARS,
        .effect = ABILITY_EFFECT_DEFENSE_UP_2,
        .icon = 114,
        .range = 1,
        .ppCost = 3,
    },
    // Protector
    {
        .targetMode = ABILITY_TARGET_ALLIES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_EFFECT,
        .element = ELEMENT_MARS,
        .effect = ABILITY_EFFECT_DEFENSE_UP,
        .icon = 115,
        .range = ABILITY_RANGE_ALL,
        .ppCost = 5,
    },
    // Magic Shell
    {
        .targetMode = ABILITY_TARGET_ALLIES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_EFFECT,
        .element = ELEMENT_JUPITER,
        .effect = ABILITY_EFFECT_RESISTANCE_UP_2,
        .icon = 112,
        .range = 1,
        .ppCost = 3,
    },
    // Magic Shield
    {
        .targetMode = ABILITY_TARGET_ALLIES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_EFFECT,
        .element = ELEMENT_JUPITER,
        .effect = ABILITY_EFFECT_RESISTANCE_UP,
        .icon = 113,
        .range = ABILITY_RANGE_ALL,
        .ppCost = 5,
    },
    // Death Plunge
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_JUPITER,
        .effect = ABILITY_EFFECT_STUN,
        .icon = 134,
        .range = 1,
        .ppCost = 14,
        .power = 40,
    },
    // Shuriken
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK_MULTIPLIED,
        .element = ELEMENT_JUPITER,
        .icon = 135,
        .range = 2,
        .ppCost = 8,
        .power = 8,
    },
    // Annihilation
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK_MULTIPLIED,
        .effect = ABILITY_EFFECT_INSTANT_DEATH,
        .icon = 133,
        .range = 1,
        .ppCost = 18,
        .power = 15,
    },
    // Punji
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .icon = 120,
        .range = 2,
        .ppCost = 7,
        .power = 45,
    },
    // Punji Trap
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .icon = 121,
        .range = 2,
        .ppCost = 13,
        .power = 85,
    },
    // Punji Strike
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .icon = 122,
        .range = 3,
        .ppCost = 24,
        .power = 150,
    },
    // Fire Bomb
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_MARS,
        .icon = 123,
        .range = 2,
        .ppCost = 5,
        .power = 35,
    },
    // Cluster Bomb
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_MARS,
        .icon = 124,
        .range = 3,
        .ppCost = 11,
        .power = 65,
    },
    // Carpet Bomb
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_MARS,
        .icon = 125,
        .range = 4,
        .ppCost = 29,
        .power = 130,
    },
    // Gale
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_JUPITER,
        .icon = 126,
        .fieldPsynergy = FIELD_PSYNERGY_WHIRLWIND,
        .range = 2,
        .ppCost = 3,
        .power = 18,
    },
    // Typhoon
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_JUPITER,
        .icon = 127,
        .range = 3,
        .ppCost = 12,
        .power = 75,
    },
    // Hurricane
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_JUPITER,
        .icon = 128,
        .range = 3,
        .ppCost = 25,
        .power = 150,
    },
    // Thunderclap
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_JUPITER,
        .icon = 129,
        .range = 2,
        .ppCost = 9,
        .power = 50,
    },
    // Thunderbolt
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_JUPITER,
        .icon = 130,
        .range = 3,
        .ppCost = 19,
        .power = 110,
    },
    // Thunderhead
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_JUPITER,
        .icon = 131,
        .range = 4,
        .ppCost = 39,
        .power = 190,
    },
    // Mist
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_EFFECT,
        .element = ELEMENT_JUPITER,
        .effect = ABILITY_EFFECT_DELUSION,
        .icon = 132,
        .range = 2,
        .ppCost = 4,
    },
    // Ragnarok
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .icon = 137,
        .range = 1,
        .ppCost = 7,
        .power = 35,
    },
    // Cutting Edge
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_MERCURY,
        .icon = 136,
        .range = 1,
        .ppCost = 5,
        .power = 32,
    },
    // Heat Wave
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_MARS,
        .icon = 138,
        .range = 1,
        .ppCost = 6,
        .power = 33,
    },
    // Astral Blast
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_JUPITER,
        .icon = 158,
        .range = 1,
        .ppCost = 5,
        .power = 32,
    },
    // Planet Diver
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_MARS,
        .icon = 139,
        .range = 1,
        .ppCost = 7,
        .power = 36,
    },
    // Diamond Dust
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_MERCURY,
        .icon = 185,
        .range = 1,
        .ppCost = 6,
        .power = 34,
    },
    // Odyssey
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .icon = 242,
        .range = 1,
        .ppCost = 18,
        .power = 95,
    },
    // Liquifier
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_MARS,
        .icon = 243,
        .range = 1,
        .ppCost = 17,
        .power = 90,
    },
    // Plume Edge
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_MERCURY,
        .icon = 244,
        .range = 1,
        .ppCost = 15,
        .power = 80,
    },
    // Thunder Mine
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_JUPITER,
        .icon = 245,
        .range = 1,
        .ppCost = 16,
        .power = 85,
    },
    // Planetary
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_MARS,
        .icon = 246,
        .range = 1,
        .ppCost = 19,
        .power = 98,
    },
    // Diamond Berg
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_MERCURY,
        .icon = 247,
        .range = 1,
        .ppCost = 17,
        .power = 92,
    },
    // Death Leap
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_JUPITER,
        .effect = ABILITY_EFFECT_STUN,
        .icon = 248,
        .range = 1,
        .ppCost = 22,
        .power = 110,
    },
    // Epicenter
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_MARS,
        .icon = 249,
        .range = 1,
        .ppCost = 33,
        .power = 210,
    },
    // Thorny Grave
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .effect = ABILITY_EFFECT_HAUNT,
        .icon = 250,
        .range = 2,
        .ppCost = 24,
        .power = 170,
    },
    // Skull Splitter
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .effect = ABILITY_EFFECT_INSTANT_DEATH,
        .icon = 251,
        .range = 1,
        .ppCost = 8,
        .power = 30,
    },
    // ?
    {0},
    // ?
    {0},
    // Terra Strike
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .range = 1,
        .power = 20,
    },
    // Poison Cloud
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .effect = ABILITY_EFFECT_POISON,
        .range = 1,
        .power = 55,
    },
    // Poison Death
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .effect = ABILITY_EFFECT_DEADLY_POISON,
        .range = 1,
        .power = 53,
    },
    // Mortal Danger
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .effect = ABILITY_EFFECT_INSTANT_DEATH,
        .range = 1,
        .power = 28,
    },
    // Bad Omen
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .effect = ABILITY_EFFECT_HAUNT,
        .range = 1,
        .power = 43,
    },
    // Life Nourish
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .effect = ABILITY_EFFECT_DRAIN_HP,
        .range = 1,
        .power = 25,
    },
    // Aqua Sock
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_MERCURY,
        .range = 1,
        .power = 42,
    },
    // Blizzard
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_MERCURY,
        .effect = ABILITY_EFFECT_ATTACK_DOWN_2,
        .range = 1,
        .power = 18,
    },
    // Frost Bite
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_MERCURY,
        .effect = ABILITY_EFFECT_STUN,
        .range = 1,
        .power = 28,
    },
    // Drown
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_MERCURY,
        .effect = ABILITY_EFFECT_INSTANT_DEATH,
        .range = 1,
        .power = 47,
    },
    // Life Leech
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_MERCURY,
        .effect = ABILITY_EFFECT_DRAIN_HP,
        .range = 1,
        .power = 29,
    },
    // Psy Leech
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_MERCURY,
        .effect = ABILITY_EFFECT_PSY_LEECH,
        .range = 1,
        .power = 22,
    },
    // Broil
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_MARS,
        .range = 1,
        .power = 23,
    },
    // Meltdown
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_MARS,
        .effect = ABILITY_EFFECT_DEFENSE_DOWN_2,
        .range = 1,
        .power = 47,
    },
    // Heat Mirage
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_MARS,
        .effect = ABILITY_EFFECT_DELUSION,
        .range = 1,
        .power = 21,
    },
    // Barrage
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_MARS,
        .effect = ABILITY_EFFECT_STUN,
        .range = 1,
        .power = 20,
    },
    // Demonfire
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_MARS,
        .effect = ABILITY_EFFECT_HAUNT,
        .range = 1,
        .power = 60,
    },
    // Acid Bath
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_MARS,
        .effect = ABILITY_EFFECT_ATTACK_DOWN_2,
        .range = 1,
        .power = 34,
    },
    // Vorpal Slash
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_JUPITER,
        .range = 1,
        .power = 16,
    },
    // Stun Voltage
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_JUPITER,
        .effect = ABILITY_EFFECT_STUN,
        .range = 1,
        .power = 12,
    },
    // Blinding Smog
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_JUPITER,
        .effect = ABILITY_EFFECT_DELUSION,
        .range = 1,
        .power = 49,
    },
    // Murk
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_JUPITER,
        .effect = ABILITY_EFFECT_SLEEP,
        .range = 1,
        .power = 10,
    },
    // Cyclone Slash
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_JUPITER,
        .effect = ABILITY_EFFECT_RESISTANCE_DOWN_2,
        .range = 1,
        .power = 38,
    },
    // Psyphon Seal
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_JUPITER,
        .effect = ABILITY_EFFECT_SEAL,
        .range = 1,
        .power = 14,
    },
    // Rapid Smash
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK_MULTIPLIED,
        .element = ELEMENT_JUPITER,
        .effect = ABILITY_EFFECT_DOUBLE_BLOW_60,
        .range = 1,
        .power = 9,
    },
    // Sonic Smash
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_JUPITER,
        .effect = ABILITY_EFFECT_TRIPLE_BLOW_60,
        .range = 1,
    },
    // Asura
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_JUPITER,
        .effect = ABILITY_EFFECT_HALVE_DEFENSE,
        .range = 1,
        .power = 64,
    },
    // Titan Blade
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .range = 1,
        .power = 70,
    },
    // Shining Star
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_JUPITER,
        .effect = ABILITY_EFFECT_DELUSION,
        .range = 1,
        .power = 57,
    },
    // ?
    {0},
    // ?
    {0},
    // ?
    {0},
    // ?
    {0},
    // ?
    {0},
    // ?
    {0},
    // ?
    {0},
    // ?
    {0},
    // ?
    {0},
    // ?
    {0},
    // ?
    {0},
    // Herb
    {
        .targetMode = ABILITY_TARGET_ALLIES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_USABLE_IN_FIELD | ABILITY_FORMULA_HEAL_HP,
        .element = ELEMENT_NONE,
        .range = 1,
        .power = 50,
    },
    // Nut
    {
        .targetMode = ABILITY_TARGET_ALLIES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_USABLE_IN_FIELD | ABILITY_FORMULA_HEAL_HP,
        .element = ELEMENT_NONE,
        .range = 1,
        .power = 200,
    },
    // Vial
    {
        .targetMode = ABILITY_TARGET_ALLIES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_USABLE_IN_FIELD | ABILITY_FORMULA_HEAL_HP,
        .element = ELEMENT_NONE,
        .range = 1,
        .power = 500,
    },
    // Potion
    {
        .targetMode = ABILITY_TARGET_ALLIES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_USABLE_IN_FIELD | ABILITY_FORMULA_HEAL_HP,
        .element = ELEMENT_NONE,
        .range = 1,
        .power = 9999,
    },
    // Soothing Water
    {
        .targetMode = ABILITY_TARGET_ALLIES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_USABLE_IN_FIELD | ABILITY_FORMULA_HEAL_HP,
        .element = ELEMENT_NONE,
        .range = 1,
        .power = 9999,
    },
    // Psy Crystal
    {
        .targetMode = ABILITY_TARGET_ALLIES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_USABLE_IN_FIELD | ABILITY_FORMULA_HEAL_PP,
        .element = ELEMENT_NONE,
        .range = 1,
        .power = 9999,
    },
    // Antidote
    {
        .targetMode = ABILITY_TARGET_ALLIES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_USABLE_IN_FIELD | ABILITY_FORMULA_EFFECT,
        .element = ELEMENT_NONE,
        .effect = ABILITY_EFFECT_CURE_POISON,
        .range = 1,
    },
    // Elixir
    {
        .targetMode = ABILITY_TARGET_ALLIES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_EFFECT,
        .element = ELEMENT_NONE,
        .effect = ABILITY_EFFECT_RESTORE,
        .range = 1,
    },
    // Water of Life
    {
        .targetMode = ABILITY_TARGET_ALLIES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_USABLE_IN_FIELD | ABILITY_FORMULA_EFFECT,
        .element = ELEMENT_NONE,
        .effect = ABILITY_EFFECT_REVIVE,
        .range = 1,
    },
    // Mist Potion
    {
        .targetMode = ABILITY_TARGET_ALLIES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_USABLE_IN_FIELD | ABILITY_FORMULA_HEAL_HP,
        .element = ELEMENT_NONE,
        .range = ABILITY_RANGE_ALL,
        .power = 300,
    },
    // Power Bread
    {
        .targetMode = ABILITY_TARGET_ALLIES,
        .flags = ABILITY_USABLE_IN_FIELD | ABILITY_FORMULA_NO_POWER,
        .element = ELEMENT_NONE,
        .range = 1,
        .power = 8,
    },
    // Cookie
    {
        .targetMode = ABILITY_TARGET_ALLIES,
        .flags = ABILITY_USABLE_IN_FIELD | ABILITY_FORMULA_NO_POWER,
        .element = ELEMENT_NONE,
        .range = 1,
        .power = 6,
    },
    // Apple
    {
        .targetMode = ABILITY_TARGET_ALLIES,
        .flags = ABILITY_USABLE_IN_FIELD | ABILITY_FORMULA_NO_POWER,
        .element = ELEMENT_NONE,
        .range = 1,
        .power = 5,
    },
    // Hard Nut
    {
        .targetMode = ABILITY_TARGET_ALLIES,
        .flags = ABILITY_USABLE_IN_FIELD | ABILITY_FORMULA_NO_POWER,
        .element = ELEMENT_NONE,
        .range = 1,
        .power = 3,
    },
    // Mint
    {
        .targetMode = ABILITY_TARGET_ALLIES,
        .flags = ABILITY_USABLE_IN_FIELD | ABILITY_FORMULA_NO_POWER,
        .element = ELEMENT_NONE,
        .range = 1,
        .power = 5,
    },
    // Lucky Pepper
    {
        .targetMode = ABILITY_TARGET_ALLIES,
        .flags = ABILITY_USABLE_IN_FIELD | ABILITY_FORMULA_NO_POWER,
        .element = ELEMENT_NONE,
        .range = 1,
        .power = 2,
    },
    // ?
    {0},
    // Smoke Bomb
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_EFFECT,
        .element = ELEMENT_JUPITER,
        .effect = ABILITY_EFFECT_DELUSION,
        .range = 1,
    },
    // Sleep Bomb
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_EFFECT,
        .element = ELEMENT_JUPITER,
        .effect = ABILITY_EFFECT_SLEEP,
        .range = 1,
    },
    // Adept Ring
    {
        .targetMode = ABILITY_TARGET_SELF,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_USABLE_IN_FIELD | ABILITY_FORMULA_HEAL_PP,
        .element = ELEMENT_NONE,
        .range = 1,
        .power = 7,
    },
    // Corn
    {
        .targetMode = ABILITY_TARGET_ALLIES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_USABLE_IN_FIELD | ABILITY_FORMULA_HEAL_HP,
        .element = ELEMENT_NONE,
        .range = 1,
        .power = 100,
    },
    // ?
    {0},
    // ?
    {0},
    // ?
    {0},
    // ?
    {0},
    // ?
    {0},
    // ?
    {0},
    // ?
    {0},
    // ?
    {0},
    // ?
    {0},
    // ?
    {0},
    // ?
    {0},
    // ?
    {0},
    // ?
    {0},
    // ?
    {0},
    // ?
    {0},
    // ?
    {0},
    // ?
    {0},
    // ?
    {0},
    // ?
    {0},
    // ?
    {0},
    // ?
    {0},
    // ?
    {0},
    // ?
    {0},
    // ?
    {0},
    // ?
    {0},
    // ?
    {0},
    // ?
    {0},
    // Aurora Field
    {
        .targetMode = ABILITY_TARGET_ALLIES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_HEAL_HP,
        .element = ELEMENT_NONE,
        .effect = ABILITY_EFFECT_REVIVE,
        .range = ABILITY_RANGE_ALL,
        .power = 9999,
    },
    // Djinn Counter
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_MARS,
        .range = 1,
        .power = 20,
    },
    // Flint
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK_MULTIPLIED,
        .range = 1,
        .power = 16,
    },
    // Granite
    {
        .targetMode = ABILITY_TARGET_ALLIES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_EFFECT_ONLY,
        .effect = ABILITY_EFFECT_DEFEND,
        .range = ABILITY_RANGE_ALL,
    },
    // Quartz
    {
        .targetMode = ABILITY_TARGET_ALLIES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_EFFECT,
        .effect = ABILITY_EFFECT_REVIVE_HALF,
        .range = 1,
    },
    // Vine
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_EFFECT,
        .effect = ABILITY_EFFECT_AGILITY_DOWN,
        .range = ABILITY_RANGE_ALL,
    },
    // Sap
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .effect = ABILITY_EFFECT_DRAIN_HALF_HP,
        .range = 1,
        .power = 50,
    },
    // Ground
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_EFFECT,
        .effect = ABILITY_EFFECT_GROUND,
        .range = 1,
    },
    // Bane
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .effect = ABILITY_EFFECT_DEADLY_POISON,
        .range = 1,
        .power = 60,
    },
    // Echo
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK_MULTIPLIED,
        .range = 1,
        .power = 16,
    },
    // Iron
    {
        .targetMode = ABILITY_TARGET_ALLIES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_EFFECT,
        .effect = ABILITY_EFFECT_DEFENSE_UP_2,
        .range = ABILITY_RANGE_ALL,
    },
    // Steel
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK_MULTIPLIED,
        .effect = ABILITY_EFFECT_DRAIN_HALF_HP,
        .range = 1,
        .power = 13,
    },
    // Mud
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_EFFECT,
        .effect = ABILITY_EFFECT_AGILITY_DOWN,
        .range = ABILITY_RANGE_ALL,
    },
    // Flower
    {
        .targetMode = ABILITY_TARGET_ALLIES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_HEAL_HP,
        .effect = ABILITY_EFFECT_HEAL_HP_30,
        .range = ABILITY_RANGE_ALL,
    },
    // Meld
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .range = 1,
    },
    // Petra
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_EFFECT_ONLY,
        .effect = ABILITY_EFFECT_GROUND,
        .range = 1,
    },
    // Salt
    {
        .targetMode = ABILITY_TARGET_ALLIES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_EFFECT,
        .effect = ABILITY_EFFECT_CURE_ALL,
        .range = ABILITY_RANGE_ALL,
    },
    // Geode
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK_MULTIPLIED,
        .range = 1,
        .power = 19,
    },
    // Mold
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .range = 1,
        .power = 80,
    },
    // Crystal
    {
        .targetMode = ABILITY_TARGET_ALLIES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_HEAL_HP,
        .effect = 70,
        .range = ABILITY_RANGE_ALL,
    },
    // Earth18
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .range = 1,
    },
    // Earth19
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .range = 1,
    },
    // Fizz
    {
        .targetMode = ABILITY_TARGET_ALLIES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_HEAL_HP,
        .element = ELEMENT_MERCURY,
        .effect = ABILITY_EFFECT_HEAL_HP_60,
        .range = 1,
    },
    // Sleet
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_MERCURY,
        .effect = ABILITY_EFFECT_ATTACK_DOWN_2,
        .range = 1,
        .power = 30,
    },
    // Mist
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK_MULTIPLIED,
        .element = ELEMENT_MERCURY,
        .effect = ABILITY_EFFECT_SLEEP,
        .range = 1,
        .power = 13,
    },
    // Spritz
    {
        .targetMode = ABILITY_TARGET_ALLIES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_HEAL_HP,
        .element = ELEMENT_MERCURY,
        .effect = ABILITY_EFFECT_HEAL_HP_30,
        .range = ABILITY_RANGE_ALL,
    },
    // Hail
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_MERCURY,
        .effect = ABILITY_EFFECT_DEFENSE_DOWN_2,
        .range = 1,
        .power = 40,
    },
    // Tonic
    {
        .targetMode = ABILITY_TARGET_ALLIES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_EFFECT,
        .element = ELEMENT_MERCURY,
        .effect = ABILITY_EFFECT_CURE_ALL,
        .range = ABILITY_RANGE_ALL,
    },
    // Dew
    {
        .targetMode = ABILITY_TARGET_ALLIES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_EFFECT,
        .element = ELEMENT_MERCURY,
        .effect = ABILITY_EFFECT_REVIVE_80,
        .range = 1,
    },
    // Fog
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_MERCURY,
        .effect = ABILITY_EFFECT_DELUSION,
        .range = 1,
        .power = 30,
    },
    // Sour
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK_MULTIPLIED,
        .element = ELEMENT_MERCURY,
        .effect = ABILITY_EFFECT_RESISTANCE_DOWN_2,
        .range = 1,
        .power = 15,
    },
    // Spring
    {
        .targetMode = ABILITY_TARGET_ALLIES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_HEAL_HP,
        .element = ELEMENT_MERCURY,
        .effect = 71,
        .range = 1,
    },
    // Shade
    {
        .targetMode = ABILITY_TARGET_ALLIES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_EFFECT_ONLY,
        .element = ELEMENT_MERCURY,
        .effect = 72,
        .range = ABILITY_RANGE_ALL,
    },
    // Chill
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_MERCURY,
        .effect = ABILITY_EFFECT_DEFENSE_DOWN_2,
        .range = 1,
        .power = 40,
    },
    // Steam
    {
        .targetMode = ABILITY_TARGET_ALLIES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_EFFECT,
        .element = ELEMENT_MERCURY,
        .effect = ABILITY_EFFECT_RESISTANCE_UP_2,
        .range = ABILITY_RANGE_ALL,
    },
    // Rime
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_EFFECT,
        .element = ELEMENT_MERCURY,
        .effect = ABILITY_EFFECT_SEAL_2_TURNS,
        .range = 1,
    },
    // Gel
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK_MULTIPLIED,
        .element = ELEMENT_MERCURY,
        .effect = ABILITY_EFFECT_ATTACK_DOWN_2,
        .range = 1,
        .power = 15,
    },
    // Eddy
    {
        .targetMode = ABILITY_TARGET_ALLIES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_EFFECT_ONLY,
        .element = ELEMENT_MERCURY,
        .range = ABILITY_RANGE_ALL,
    },
    // Balm
    {
        .targetMode = ABILITY_TARGET_ALLIES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_EFFECT,
        .element = ELEMENT_MERCURY,
        .effect = 73,
        .range = ABILITY_RANGE_ALL,
    },
    // Serac
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_MERCURY,
        .effect = ABILITY_EFFECT_INSTANT_DEATH,
        .range = 1,
        .power = 70,
    },
    // Water18
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .range = 1,
    },
    // Water19
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .range = 1,
    },
    // Forge
    {
        .targetMode = ABILITY_TARGET_ALLIES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_EFFECT,
        .element = ELEMENT_MARS,
        .effect = ABILITY_EFFECT_ATTACK_UP_2,
        .range = ABILITY_RANGE_ALL,
    },
    // Fever
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_MARS,
        .effect = ABILITY_EFFECT_DELUSION,
        .range = 1,
        .power = 30,
    },
    // Corona
    {
        .targetMode = ABILITY_TARGET_ALLIES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_EFFECT,
        .element = ELEMENT_MARS,
        .effect = ABILITY_EFFECT_DEFENSE_UP_2,
        .range = ABILITY_RANGE_ALL,
    },
    // Scorch
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_MARS,
        .effect = ABILITY_EFFECT_STUN,
        .range = 1,
        .power = 50,
    },
    // Ember
    {
        .targetMode = ABILITY_TARGET_ALLIES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_HEAL_PP,
        .element = ELEMENT_MARS,
        .effect = ABILITY_EFFECT_HEAL_PP_7,
        .range = ABILITY_RANGE_ALL,
    },
    // Flash
    {
        .targetMode = ABILITY_TARGET_ALLIES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_EFFECT_ONLY,
        .element = ELEMENT_MARS,
        .effect = ABILITY_EFFECT_TOTAL_DEFENSE,
        .range = ABILITY_RANGE_ALL,
    },
    // Torch
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_MARS,
        .effect = ABILITY_EFFECT_HALVE_DEFENSE,
        .range = 1,
        .power = 30,
    },
    // Cannon
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_MARS,
        .range = 1,
        .power = 40,
    },
    // Spark
    {
        .targetMode = ABILITY_TARGET_ALLIES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_EFFECT,
        .element = ELEMENT_MARS,
        .effect = 73,
        .range = 1,
    },
    // Kindle
    {
        .targetMode = ABILITY_TARGET_ALLIES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_EFFECT,
        .element = ELEMENT_MARS,
        .effect = ABILITY_EFFECT_ATTACK_UP_2,
        .range = ABILITY_RANGE_ALL,
    },
    // Char
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_MARS,
        .effect = ABILITY_EFFECT_STUN,
        .range = 1,
        .power = 50,
    },
    // Coal
    {
        .targetMode = ABILITY_TARGET_ALLIES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_EFFECT,
        .element = ELEMENT_MARS,
        .effect = ABILITY_EFFECT_AGILITY_UP,
        .range = ABILITY_RANGE_ALL,
    },
    // Reflux
    {
        .targetMode = ABILITY_TARGET_ALLIES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_EFFECT_ONLY,
        .element = ELEMENT_MARS,
        .effect = 74,
        .range = 1,
    },
    // Core
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_MARS,
        .effect = ABILITY_EFFECT_HALVE_DEFENSE,
        .range = 1,
        .power = 40,
    },
    // Tinder
    {
        .targetMode = ABILITY_TARGET_ALLIES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_EFFECT,
        .element = ELEMENT_MARS,
        .effect = ABILITY_EFFECT_REVIVE,
        .range = 1,
    },
    // Shine
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_MARS,
        .effect = 75,
        .range = 1,
        .power = 60,
    },
    // Fury
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_MARS,
        .effect = ABILITY_EFFECT_HAUNT,
        .range = 1,
        .power = 70,
    },
    // Fugue
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_PP_DAMAGE,
        .element = ELEMENT_MARS,
        .range = ABILITY_RANGE_ALL,
        .power = 18,
    },
    // Fire18
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .range = 1,
    },
    // Fire19
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .range = 1,
    },
    // Gust
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_JUPITER,
        .effect = ABILITY_EFFECT_DOUBLE_BLOW_60,
        .range = 1,
        .power = 10,
    },
    // Breeze
    {
        .targetMode = ABILITY_TARGET_ALLIES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_EFFECT,
        .element = ELEMENT_JUPITER,
        .effect = ABILITY_EFFECT_RESISTANCE_UP_2,
        .range = ABILITY_RANGE_ALL,
    },
    // Zephyr
    {
        .targetMode = ABILITY_TARGET_ALLIES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_EFFECT,
        .element = ELEMENT_JUPITER,
        .effect = ABILITY_EFFECT_AGILITY_UP,
        .range = ABILITY_RANGE_ALL,
    },
    // Smog
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_JUPITER,
        .effect = ABILITY_EFFECT_DELUSION,
        .range = 1,
        .power = 60,
    },
    // Kite
    {
        .targetMode = ABILITY_TARGET_ALLIES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_EFFECT_ONLY,
        .element = ELEMENT_JUPITER,
        .effect = ABILITY_EFFECT_ACT_TWICE,
        .range = 1,
    },
    // Squall
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK_MULTIPLIED,
        .element = ELEMENT_JUPITER,
        .effect = ABILITY_EFFECT_STUN,
        .range = 1,
        .power = 14,
    },
    // Luff
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_EFFECT,
        .element = ELEMENT_JUPITER,
        .effect = ABILITY_EFFECT_SEAL_2_TURNS,
        .range = 1,
    },
    // Breath
    {
        .targetMode = ABILITY_TARGET_ALLIES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_HEAL_HP,
        .element = ELEMENT_JUPITER,
        .effect = 76,
        .range = 1,
    },
    // Blitz
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_JUPITER,
        .effect = ABILITY_EFFECT_STUN,
        .range = 1,
        .power = 30,
    },
    // Ether
    {
        .targetMode = ABILITY_TARGET_ALLIES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_HEAL_PP,
        .element = ELEMENT_JUPITER,
        .effect = 78,
        .range = 1,
    },
    // Waft
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_JUPITER,
        .effect = ABILITY_EFFECT_SLEEP,
        .range = 1,
        .power = 40,
    },
    // Haze
    {
        .targetMode = ABILITY_TARGET_ALLIES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_EFFECT_ONLY,
        .element = ELEMENT_JUPITER,
        .effect = 79,
        .range = 1,
    },
    // Wheeze
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_JUPITER,
        .effect = ABILITY_EFFECT_DEADLY_POISON,
        .range = 1,
        .power = 50,
    },
    // Aroma
    {
        .targetMode = ABILITY_TARGET_ALLIES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_HEAL_PP,
        .element = ELEMENT_JUPITER,
        .effect = 77,
        .range = ABILITY_RANGE_ALL,
    },
    // Whorl
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_JUPITER,
        .effect = ABILITY_EFFECT_INSTANT_DEATH,
        .range = 1,
        .power = 40,
    },
    // Gasp
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_EFFECT,
        .element = ELEMENT_JUPITER,
        .effect = 80,
        .range = ABILITY_RANGE_ALL,
    },
    // Lull
    {
        .targetMode = ABILITY_TARGET_SELF,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_EFFECT_ONLY,
        .element = ELEMENT_JUPITER,
        .effect = 81,
        .range = 1,
    },
    // Gale
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_JUPITER,
        .effect = 82,
        .range = 1,
        .power = 60,
    },
    // Wind18
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .range = 1,
    },
    // Wind19
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .range = 1,
    },
    // Venus
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_SUMMON,
        .icon = 140,
        .range = 6,
        .power = 30,
    },
    // Ramses
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_SUMMON,
        .icon = 141,
        .range = 6,
        .power = 60,
    },
    // Cybele
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_SUMMON,
        .icon = 142,
        .range = 6,
        .power = 120,
    },
    // Judgment
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_SUMMON,
        .icon = 143,
        .range = 6,
        .power = 240,
    },
    // Zagan
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_SUMMON,
        .effect = ABILITY_EFFECT_DEFENSE_DOWN_2,
        .icon = 186,
        .range = 6,
        .power = 50,
    },
    // Haures
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_SUMMON,
        .effect = ABILITY_EFFECT_DEADLY_POISON,
        .icon = 187,
        .range = 6,
        .power = 270,
    },
    // Charon
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_SUMMON,
        .effect = ABILITY_EFFECT_INSTANT_DEATH,
        .icon = 188,
        .range = 6,
        .power = 500,
    },
    // ?
    {0},
    // Mercury
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_SUMMON,
        .element = ELEMENT_MERCURY,
        .icon = 144,
        .range = 6,
        .power = 30,
    },
    // Nereid
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_SUMMON,
        .element = ELEMENT_MERCURY,
        .icon = 145,
        .range = 6,
        .power = 60,
    },
    // Neptune
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_SUMMON,
        .element = ELEMENT_MERCURY,
        .icon = 146,
        .range = 6,
        .power = 120,
    },
    // Boreas
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_SUMMON,
        .element = ELEMENT_MERCURY,
        .icon = 147,
        .range = 6,
        .power = 240,
    },
    // Moloch
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_SUMMON,
        .element = ELEMENT_MERCURY,
        .effect = ABILITY_EFFECT_AGILITY_DOWN,
        .icon = 189,
        .range = 6,
        .power = 100,
    },
    // Coatlicue
    {
        .targetMode = ABILITY_TARGET_ALLIES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_HEAL_HP,
        .element = ELEMENT_MERCURY,
        .effect = ABILITY_EFFECT_REGENERATE,
        .icon = 190,
        .range = ABILITY_RANGE_ALL,
        .power = 60,
    },
    // Azul
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_SUMMON,
        .element = ELEMENT_MERCURY,
        .effect = ABILITY_EFFECT_STUN,
        .icon = 191,
        .range = 6,
        .power = 330,
    },
    // ?
    {0},
    // Mars
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_SUMMON,
        .element = ELEMENT_MARS,
        .icon = 148,
        .range = 6,
        .power = 30,
    },
    // Kirin
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_SUMMON,
        .element = ELEMENT_MARS,
        .icon = 149,
        .range = 6,
        .power = 60,
    },
    // Tiamat
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_SUMMON,
        .element = ELEMENT_MARS,
        .icon = 150,
        .range = 6,
        .power = 120,
    },
    // Meteor
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_SUMMON,
        .element = ELEMENT_MARS,
        .icon = 151,
        .range = 6,
        .power = 240,
    },
    // Megaera
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_SUMMON,
        .element = ELEMENT_MARS,
        .icon = 192,
        .range = 6,
        .power = 40,
    },
    // Ulysses
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_SUMMON,
        .element = ELEMENT_MARS,
        .effect = 83,
        .icon = 193,
        .range = 6,
        .power = 160,
    },
    // Daedalus
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_SUMMON,
        .element = ELEMENT_MARS,
        .icon = 194,
        .range = 6,
        .power = 100,
    },
    // Daedalus
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_SUMMON,
        .element = ELEMENT_MARS,
        .icon = 194,
        .range = 6,
        .power = 250,
    },
    // Iris
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_SUMMON,
        .element = ELEMENT_MARS,
        .icon = 195,
        .range = 6,
        .power = 800,
    },
    // ?
    {0},
    // Jupiter
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_SUMMON,
        .element = ELEMENT_JUPITER,
        .icon = 152,
        .range = 6,
        .power = 30,
    },
    // Atalanta
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_SUMMON,
        .element = ELEMENT_JUPITER,
        .icon = 153,
        .range = 6,
        .power = 60,
    },
    // Procne
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_SUMMON,
        .element = ELEMENT_JUPITER,
        .icon = 154,
        .range = 6,
        .power = 120,
    },
    // Thor
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_SUMMON,
        .element = ELEMENT_JUPITER,
        .icon = 155,
        .range = 6,
        .power = 240,
    },
    // Flora
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_SUMMON,
        .element = ELEMENT_JUPITER,
        .effect = ABILITY_EFFECT_SLEEP,
        .icon = 196,
        .range = 6,
        .power = 80,
    },
    // Eclipse
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_SUMMON,
        .element = ELEMENT_JUPITER,
        .effect = ABILITY_EFFECT_ATTACK_DOWN_2,
        .icon = 197,
        .range = 6,
        .power = 300,
    },
    // Catastrophe
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_SUMMON,
        .element = ELEMENT_JUPITER,
        .effect = 84,
        .icon = 198,
        .range = 6,
        .power = 400,
    },
    // ?
    {0},
    // ?
    {0},
    // ?
    {0},
    // ?
    {0},
    // ?
    {0},
    // ?
    {0},
    // ?
    {0},
    // Fire Breath
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ENEMY_DAMAGE,
        .element = ELEMENT_MARS,
        .range = 2,
        .power = 60,
    },
    // Fire Breath
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ENEMY_DAMAGE,
        .element = ELEMENT_MARS,
        .range = 2,
        .power = 90,
    },
    // Fire Breath
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ENEMY_DAMAGE,
        .element = ELEMENT_MARS,
        .range = 2,
        .power = 130,
    },
    // Water Breath
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ENEMY_DAMAGE,
        .element = ELEMENT_MERCURY,
        .range = 2,
        .power = 25,
    },
    // Water Breath
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ENEMY_DAMAGE,
        .element = ELEMENT_MERCURY,
        .range = 2,
        .power = 50,
    },
    // Water Breath
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ENEMY_DAMAGE,
        .element = ELEMENT_MERCURY,
        .range = 2,
        .power = 100,
    },
    // Ice Breath
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ENEMY_DAMAGE,
        .element = ELEMENT_MERCURY,
        .range = 2,
        .power = 50,
    },
    // Ice Breath
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ENEMY_DAMAGE,
        .element = ELEMENT_MERCURY,
        .range = 2,
        .power = 90,
    },
    // Ice Breath
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ENEMY_DAMAGE,
        .element = ELEMENT_MERCURY,
        .range = 2,
        .power = 180,
    },
    // Dark Breath
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ENEMY_DAMAGE,
        .effect = ABILITY_EFFECT_DELUSION,
        .range = 2,
        .power = 70,
    },
    // Dark Breath
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ENEMY_DAMAGE,
        .effect = ABILITY_EFFECT_DELUSION,
        .range = 2,
        .power = 110,
    },
    // Acid Breath
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ENEMY_DAMAGE,
        .element = ELEMENT_MARS,
        .effect = ABILITY_EFFECT_DEFENSE_DOWN,
        .range = 2,
        .power = 170,
    },
    // Acid Breath
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ENEMY_DAMAGE,
        .element = ELEMENT_MARS,
        .effect = ABILITY_EFFECT_DEFENSE_DOWN,
        .range = 2,
        .power = 190,
    },
    // Storm Breath
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ENEMY_DAMAGE,
        .element = ELEMENT_JUPITER,
        .effect = ABILITY_EFFECT_STUN,
        .range = 2,
        .power = 190,
    },
    // Storm Breath
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ENEMY_DAMAGE,
        .element = ELEMENT_JUPITER,
        .effect = ABILITY_EFFECT_STUN,
        .range = 2,
        .power = 220,
    },
    // Sonic Wave
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ENEMY_DAMAGE,
        .element = ELEMENT_JUPITER,
        .range = 1,
        .power = 5,
    },
    // Sonic Wave
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ENEMY_DAMAGE,
        .element = ELEMENT_JUPITER,
        .range = 1,
        .power = 45,
    },
    // Shriek
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ENEMY_DAMAGE,
        .element = ELEMENT_JUPITER,
        .range = 1,
        .power = 30,
    },
    // Banshee Howl
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ENEMY_DAMAGE,
        .element = ELEMENT_JUPITER,
        .effect = ABILITY_EFFECT_STUN,
        .range = 1,
        .power = 70,
    },
    // Crazy Voice
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_PP_DAMAGE,
        .element = ELEMENT_JUPITER,
        .effect = 52,
        .range = 1,
        .power = 10,
    },
    // War Cry
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ENEMY_DAMAGE,
        .element = ELEMENT_JUPITER,
        .effect = ABILITY_EFFECT_GROUND,
        .range = 1,
        .power = 45,
    },
    // Wicked Howl
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ENEMY_DAMAGE,
        .element = ELEMENT_JUPITER,
        .effect = ABILITY_EFFECT_GROUND,
        .range = 1,
        .power = 60,
    },
    // Wing Beat
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ENEMY_DAMAGE,
        .element = ELEMENT_JUPITER,
        .range = 1,
        .power = 35,
    },
    // Wing Flutter
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ENEMY_DAMAGE,
        .element = ELEMENT_JUPITER,
        .range = 2,
        .power = 70,
    },
    // Wing Stroke
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ENEMY_DAMAGE,
        .element = ELEMENT_JUPITER,
        .range = 2,
        .power = 140,
    },
    // Evil Blessing
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ENEMY_DAMAGE,
        .element = ELEMENT_MARS,
        .effect = ABILITY_EFFECT_HAUNT,
        .range = 2,
        .power = 80,
    },
    // Deadly Gas
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ENEMY_DAMAGE,
        .effect = ABILITY_EFFECT_DEADLY_POISON,
        .range = 2,
        .power = 30,
    },
    // ?
    {0},
    // ?
    {0},
    // ?
    {0},
    // Rumble
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .range = 1,
        .power = 9,
    },
    // Bone Chiller
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_MERCURY,
        .range = 1,
        .power = 7,
    },
    // Slice
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK_MULTIPLIED,
        .element = ELEMENT_JUPITER,
        .effect = ABILITY_EFFECT_DOUBLE_BLOW_60,
        .range = 1,
        .power = 9,
    },
    // Bone Charge
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK_MULTIPLIED,
        .range = 1,
        .power = 17,
    },
    // Mystic Flame
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_MARS,
        .range = 1,
        .power = 16,
    },
    // Numbing Sting
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK_MULTIPLIED,
        .element = ELEMENT_JUPITER,
        .effect = ABILITY_EFFECT_STUN,
        .range = 1,
        .power = 8,
    },
    // Brute Force
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .range = 1,
        .power = 22,
    },
    // Sticky Goo
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_MERCURY,
        .effect = ABILITY_EFFECT_DEFENSE_DOWN,
        .range = 1,
        .power = 13,
    },
    // Cannibal Fang
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_EFFECT,
        .effect = ABILITY_EFFECT_DRAIN_HP,
        .range = 1,
        .power = 24,
    },
    // Bear Claw
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK_MULTIPLIED,
        .element = ELEMENT_NONE,
        .range = 1,
        .power = 16,
    },
    // Poisonous Bite
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .effect = ABILITY_EFFECT_POISON,
        .range = 1,
    },
    // Flying Attack
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_JUPITER,
        .effect = ABILITY_EFFECT_DOUBLE_BLOW_1_IN_4,
        .range = 1,
        .power = 8,
    },
    // Undead Sword
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .range = 1,
        .power = 15,
    },
    // Ransack
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .range = 1,
        .power = 35,
    },
    // Sticky Poison
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_MERCURY,
        .effect = ABILITY_EFFECT_POISON,
        .range = 1,
        .power = 16,
    },
    // Poison Fang
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .effect = ABILITY_EFFECT_POISON,
        .range = 1,
    },
    // Electric Bite
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_JUPITER,
        .effect = ABILITY_EFFECT_STUN,
        .range = 1,
    },
    // Poison Tail
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .effect = ABILITY_EFFECT_DEADLY_POISON,
        .range = 1,
        .power = 30,
    },
    // Onslaught
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .range = 1,
        .power = 24,
    },
    // Vampiric Fang
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_EFFECT,
        .effect = ABILITY_EFFECT_DRAIN_HP,
        .range = 1,
        .power = 35,
    },
    // Bacteria Rush
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .effect = ABILITY_EFFECT_POISON,
        .range = 1,
        .power = 16,
    },
    // Swift Strike
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_JUPITER,
        .effect = ABILITY_EFFECT_TRIPLE_BLOW_1_IN_4,
        .range = 1,
        .power = 15,
    },
    // Rotten Blood
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_MARS,
        .effect = ABILITY_EFFECT_DEFENSE_DOWN_2,
        .range = 1,
        .power = 44,
    },
    // Forcible Arm
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .range = 1,
        .power = 40,
    },
    // Double Fang
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .effect = ABILITY_EFFECT_DOUBLE_BLOW_1_IN_4,
        .range = 1,
        .power = 35,
    },
    // Mortal Blow
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .effect = ABILITY_EFFECT_SELF_DESTRUCT,
        .range = 1,
        .power = 35,
    },
    // Freebite Rush
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .effect = ABILITY_EFFECT_ATTACK_DOWN,
        .range = 1,
        .power = 40,
    },
    // Twin Beaks
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_JUPITER,
        .effect = ABILITY_EFFECT_DOUBLE_BLOW_1_IN_4,
        .range = 1,
        .power = 30,
    },
    // Rabid Fang
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .effect = ABILITY_EFFECT_DEADLY_POISON,
        .range = 1,
        .power = 36,
    },
    // Acid Bite
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_MARS,
        .effect = ABILITY_EFFECT_DEFENSE_DOWN,
        .range = 1,
        .power = 36,
    },
    // Dynamite
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .effect = ABILITY_EFFECT_SELF_DESTRUCT,
        .range = 2,
        .power = 200,
    },
    // Headbutt
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .range = 1,
        .power = 60,
    },
    // Poison Ink
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .effect = ABILITY_EFFECT_DEADLY_POISON,
        .range = 1,
    },
    // Truncheon Fist
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .effect = ABILITY_EFFECT_LEAVE_ONE_HP,
        .range = 1,
        .power = 60,
    },
    // Counterstrike
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .range = 1,
        .power = 50,
    },
    // Mad Dash
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .range = 1,
        .power = 70,
    },
    // Soothing Star
    {
        .targetMode = ABILITY_TARGET_ALLIES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_HEAL_HP,
        .range = 2,
        .power = 10,
    },
    // Spider Web
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_EFFECT,
        .effect = ABILITY_EFFECT_DEFENSE_DOWN_2,
        .range = 1,
    },
    // Heartrender
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_EFFECT,
        .element = ELEMENT_NONE,
        .effect = ABILITY_EFFECT_ATTACK_DOWN_2,
        .range = 1,
    },
    // Mad Spatter
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_EFFECT,
        .effect = ABILITY_EFFECT_DELUSION,
        .range = 2,
    },
    // Spasm
    {
        .targetMode = ABILITY_TARGET_SELF,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_HEAL_HP,
        .range = 1,
        .power = 40,
    },
    // Sleep Star
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_EFFECT,
        .element = ELEMENT_JUPITER,
        .effect = ABILITY_EFFECT_SLEEP,
        .range = 2,
    },
    // Decompose
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_EFFECT,
        .effect = ABILITY_EFFECT_ATTACK_DOWN,
        .range = 2,
    },
    // Haunting
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_EFFECT,
        .effect = ABILITY_EFFECT_HAUNT,
        .range = 1,
    },
    // Worms
    {
        .targetMode = ABILITY_TARGET_SELF,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_HEAL_HP,
        .range = 1,
        .power = 50,
    },
    // Berserk
    {
        .targetMode = ABILITY_TARGET_SELF,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_EFFECT,
        .effect = ABILITY_EFFECT_ATTACK_UP_2,
        .range = 1,
    },
    // Lucid Prophecy
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_EFFECT,
        .effect = ABILITY_EFFECT_CURSE,
        .range = 1,
    },
    // Recovery
    {
        .targetMode = ABILITY_TARGET_SELF,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_HEAL_HP,
        .element = ELEMENT_NONE,
        .range = 1,
        .power = 150,
    },
    // Flee
    {
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_NO_POWER,
        .element = ELEMENT_NONE,
        .effect = ABILITY_EFFECT_FLEE,
        .range = 1,
    },
    // Contain
    {
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_NO_POWER,
        .element = ELEMENT_NONE,
        .effect = ABILITY_EFFECT_THREATEN,
        .range = 1,
    },
    // Threaten
    {
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_NO_POWER,
        .element = ELEMENT_NONE,
        .effect = ABILITY_EFFECT_THREATEN,
        .range = 1,
    },
    // Tremble
    {
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_NO_POWER,
        .element = ELEMENT_NONE,
        .effect = ABILITY_EFFECT_THREATEN,
        .range = 1,
    },
    // Fortify
    {
        .targetMode = ABILITY_TARGET_SELF,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_NO_POWER,
        .element = ELEMENT_NONE,
        .effect = ABILITY_EFFECT_DEFEND,
        .range = 1,
    },
    // Speed Surge
    {
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_NO_POWER,
        .element = ELEMENT_NONE,
        .effect = ABILITY_EFFECT_CALL_ALLY,
        .range = 1,
    },
    // Ally Search
    {
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_NO_POWER,
        .element = ELEMENT_NONE,
        .effect = ABILITY_EFFECT_CALL_ALLY,
        .range = 1,
    },
    // Sidestep
    {
        .targetMode = ABILITY_TARGET_SELF,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_NO_POWER,
        .element = ELEMENT_NONE,
        .effect = 48,
        .range = 1,
    },
    // Total Defense
    {
        .targetMode = ABILITY_TARGET_SELF,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_NO_POWER,
        .element = ELEMENT_NONE,
        .effect = ABILITY_EFFECT_TOTAL_DEFENSE,
        .range = 1,
    },
    // Stand Ready
    {
        .targetMode = ABILITY_TARGET_SELF,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_NO_POWER,
        .element = ELEMENT_NONE,
        .effect = ABILITY_EFFECT_DEFEND,
        .range = 1,
    },
    // Battle Cry
    {
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_NO_POWER,
        .element = ELEMENT_NONE,
        .effect = ABILITY_EFFECT_CALL_HELP,
        .range = 1,
    },
    // Can't Use
    {
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .range = 1,
    },
    // Poison Beat
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_MERCURY,
        .effect = ABILITY_EFFECT_POISON,
        .range = 1,
        .power = 20,
    },
    // Spinning Beat
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_MERCURY,
        .effect = ABILITY_EFFECT_HALVE_DEFENSE,
        .range = 1,
        .power = 40,
    },
    // Heat Flash
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_MARS,
        .effect = ABILITY_EFFECT_DELUSION,
        .range = 1,
        .power = 20,
    },
    // Death Scythe
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .effect = ABILITY_EFFECT_INSTANT_DEATH,
        .range = 1,
        .power = 35,
    },
    // Outer Space
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_SUMMON,
        .element = ELEMENT_MARS,
        .range = 6,
        .power = 200,
    },
    // Dragon Driver
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_SUMMON,
        .element = ELEMENT_MARS,
        .range = 6,
        .power = 120,
    },
    // Drain Fang
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_EFFECT,
        .effect = ABILITY_EFFECT_DRAIN_HP,
        .range = 1,
        .power = 80,
    },
    // Severe Blow
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK_MULTIPLIED,
        .effect = ABILITY_EFFECT_LEAVE_ONE_HP,
        .range = 1,
        .power = 12,
    },
    // Thrash
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_MERCURY,
        .effect = ABILITY_EFFECT_LEAVE_ONE_HP,
        .range = 1,
        .power = 30,
    },
    // ?
    {0},
    // Stone Justice
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .effect = ABILITY_EFFECT_AGILITY_DOWN,
        .range = 1,
        .power = 11,
    },
    // Sarcophagus
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .effect = ABILITY_EFFECT_HAUNT,
        .range = 1,
        .power = 30,
    },
    // Evil Eye
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .effect = ABILITY_EFFECT_DELUSION,
        .range = 1,
        .power = 38,
    },
    // Vein Tap
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .effect = ABILITY_EFFECT_DRAIN_HP,
        .range = 1,
        .power = 43,
    },
    // Heavy Divide
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .effect = ABILITY_EFFECT_DEFENSE_DOWN_2,
        .range = 1,
        .power = 60,
    },
    // Hammersphere
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .effect = ABILITY_EFFECT_HALVE_DEFENSE,
        .range = 1,
        .power = 58,
    },
    // Mother Earth
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .effect = ABILITY_EFFECT_SLEEP,
        .range = 1,
        .power = 59,
    },
    // Wyrd Curse
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .effect = ABILITY_EFFECT_HAUNT,
        .range = 1,
        .power = 41,
    },
    // Heartbreak
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .effect = ABILITY_EFFECT_INSTANT_DEATH,
        .range = 1,
        .power = 42,
    },
    // Vengeance
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .effect = 89,
        .range = 1,
        .power = 71,
    },
    // Acheron's Grief
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .effect = ABILITY_EFFECT_DEADLY_POISON,
        .range = 1,
        .power = 76,
    },
    // Megiddo
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK_MULTIPLIED,
        .range = 1,
        .power = 30,
    },
    // ?
    {0},
    // ?
    {0},
    // ?
    {0},
    // Hurricane
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_MERCURY,
        .effect = ABILITY_EFFECT_ATTACK_DOWN_2,
        .range = 1,
        .power = 21,
    },
    // Dreamtide
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_MERCURY,
        .effect = ABILITY_EFFECT_SLEEP,
        .range = 1,
        .power = 12,
    },
    // Stun Cloud
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_MERCURY,
        .effect = ABILITY_EFFECT_STUN,
        .range = 1,
        .power = 31,
    },
    // Searing Fog
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_MERCURY,
        .effect = ABILITY_EFFECT_DELUSION,
        .range = 1,
        .power = 36,
    },
    // Ice Crush
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_MERCURY,
        .effect = ABILITY_EFFECT_DEFENSE_DOWN_2,
        .range = 1,
        .power = 34,
    },
    // Flash Force
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_MERCURY,
        .effect = ABILITY_EFFECT_SEAL,
        .range = 1,
        .power = 44,
    },
    // Sargasso
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_MERCURY,
        .effect = ABILITY_EFFECT_INSTANT_DEATH,
        .range = 1,
        .power = 39,
    },
    // Lethe Albion
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_MERCURY,
        .effect = ABILITY_EFFECT_SEAL,
        .range = 1,
        .power = 56,
    },
    // Reverse Star
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_MERCURY,
        .effect = ABILITY_EFFECT_PSY_LEECH,
        .range = 1,
        .power = 58,
    },
    // Rising Dragon
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_MERCURY,
        .effect = ABILITY_EFFECT_DOUBLE_BLOW_60,
        .range = 1,
        .power = 62,
    },
    // ?
    {0},
    // ?
    {0},
    // ?
    {0},
    // ?
    {0},
    // Power Drive
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_MARS,
        .range = 1,
        .power = 24,
    },
    // Boost Hack
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_MARS,
        .range = 1,
        .power = 17,
    },
    // Heat Smash
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_MARS,
        .effect = ABILITY_EFFECT_RESISTANCE_DOWN_2,
        .range = 1,
        .power = 41,
    },
    // Fire Dance
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_MARS,
        .effect = ABILITY_EFFECT_ATTACK_DOWN_2,
        .range = 1,
        .power = 51,
    },
    // Blaze Rush
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_MARS,
        .effect = ABILITY_EFFECT_DEFENSE_DOWN_2,
        .range = 1,
        .power = 54,
    },
    // Flare Burst
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_MARS,
        .effect = ABILITY_EFFECT_DRAIN_HP,
        .range = 1,
        .power = 56,
    },
    // Shred
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_MARS,
        .effect = ABILITY_EFFECT_DEFENSE_DOWN_2,
        .range = 1,
        .power = 33,
    },
    // Crucible Fire
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_MARS,
        .effect = ABILITY_EFFECT_AGILITY_DOWN,
        .range = 1,
        .power = 48,
    },
    // Scorpionfish
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_MARS,
        .effect = ABILITY_EFFECT_DEADLY_POISON,
        .range = 1,
        .power = 43,
    },
    // Purgatory
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_MARS,
        .effect = ABILITY_EFFECT_SLEEP,
        .range = 1,
        .power = 67,
    },
    // Soul Shatter
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_MARS,
        .effect = ABILITY_EFFECT_PSY_LEECH,
        .range = 1,
        .power = 50,
    },
    // Radiant Fire
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_MARS,
        .effect = ABILITY_EFFECT_HALVE_DEFENSE,
        .range = 1,
        .power = 66,
    },
    // Life Shear
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_MARS,
        .effect = ABILITY_EFFECT_INSTANT_DEATH,
        .range = 1,
        .power = 114,
    },
    // ?
    {0},
    // ?
    {0},
    // ?
    {0},
    // Moon Air
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_JUPITER,
        .effect = ABILITY_EFFECT_SLEEP,
        .range = 1,
        .power = 18,
    },
    // Flash Edge
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_JUPITER,
        .effect = ABILITY_EFFECT_DELUSION,
        .range = 1,
        .power = 46,
    },
    // Aging Gas
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_JUPITER,
        .effect = ABILITY_EFFECT_DRAIN_HP,
        .range = 1,
        .power = 45,
    },
    // Mad Zephyr
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_JUPITER,
        .effect = ABILITY_EFFECT_DOUBLE_BLOW_60,
        .range = 1,
        .power = 40,
    },
    // Lunar Slash
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_JUPITER,
        .effect = ABILITY_EFFECT_RESISTANCE_DOWN_2,
        .range = 1,
        .power = 25,
    },
    // Nirvana
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_JUPITER,
        .effect = ABILITY_EFFECT_PSY_LEECH,
        .range = 1,
        .power = 50,
    },
    // High Vitals
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_JUPITER,
        .effect = ABILITY_EFFECT_DRAIN_HP,
        .range = 1,
        .power = 52,
    },
    // Stun Bolt
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_JUPITER,
        .effect = ABILITY_EFFECT_STUN,
        .range = 1,
        .power = 47,
    },
    // Trident
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK_MULTIPLIED,
        .element = ELEMENT_JUPITER,
        .effect = 91,
        .range = 1,
        .power = 12,
    },
    // Light Surge
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_JUPITER,
        .effect = ABILITY_EFFECT_DELUSION,
        .range = 1,
        .power = 51,
    },
    // Raiden's Wrath
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_JUPITER,
        .effect = 90,
        .range = 1,
        .power = 39,
    },
    // Void Beam
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_JUPITER,
        .effect = ABILITY_EFFECT_SEAL,
        .range = 1,
        .power = 57,
    },
    // Supernova
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_JUPITER,
        .effect = ABILITY_EFFECT_STUN,
        .range = 1,
        .power = 63,
    },
    // Apocalypse
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_JUPITER,
        .effect = ABILITY_EFFECT_CURSE,
        .range = 1,
        .power = 125,
    },
    // Legend
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_JUPITER,
        .effect = ABILITY_EFFECT_TRIPLE_BLOW_60,
        .range = 1,
        .power = 73,
    },
    // ?
    {0},
    // ?
    {0},
    // ?
    {0},
    // ?
    {0},
    // Sabre Dance
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK_MULTIPLIED,
        .icon = 199,
        .range = 1,
        .ppCost = 7,
        .power = 15,
    },
    // Backstab
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_JUPITER,
        .effect = ABILITY_EFFECT_INSTANT_DEATH,
        .icon = 200,
        .range = 1,
        .ppCost = 16,
        .power = 35,
    },
    // Fire Breath
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ENEMY_DAMAGE,
        .element = ELEMENT_MARS,
        .icon = 201,
        .range = 2,
        .ppCost = 13,
        .power = 85,
    },
    // Juggle
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_MARS,
        .icon = 202,
        .range = 2,
        .ppCost = 7,
        .power = 40,
    },
    // Heat Juggle
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_MARS,
        .icon = 203,
        .range = 3,
        .ppCost = 13,
        .power = 75,
    },
    // Fiery Juggle
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_MARS,
        .icon = 204,
        .range = 3,
        .ppCost = 25,
        .power = 150,
    },
    // Flame Card
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_MARS,
        .icon = 205,
        .range = 2,
        .ppCost = 11,
        .power = 60,
    },
    // Thunder Card
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_JUPITER,
        .icon = 208,
        .range = 2,
        .ppCost = 17,
        .power = 100,
    },
    // Bramble Card
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .icon = 207,
        .range = 2,
        .ppCost = 22,
        .power = 130,
    },
    // Frost Card
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_MERCURY,
        .icon = 206,
        .range = 2,
        .ppCost = 28,
        .power = 175,
    },
    // Baffle Card
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_EFFECT,
        .element = ELEMENT_JUPITER,
        .effect = ABILITY_EFFECT_DELUSION,
        .icon = 209,
        .range = 2,
        .ppCost = 4,
    },
    // Sword Card
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_EFFECT,
        .element = ELEMENT_MARS,
        .effect = ABILITY_EFFECT_ATTACK_DOWN_2,
        .icon = 212,
        .range = 1,
        .ppCost = 6,
    },
    // Sleep Card
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_EFFECT,
        .element = ELEMENT_JUPITER,
        .effect = ABILITY_EFFECT_SLEEP,
        .icon = 210,
        .range = 2,
        .ppCost = 5,
    },
    // Death Card
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_EFFECT,
        .effect = ABILITY_EFFECT_INSTANT_DEATH,
        .icon = 211,
        .range = 1,
        .ppCost = 8,
    },
    // ?
    {0},
    // Whiplash
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK_MULTIPLIED,
        .element = ELEMENT_JUPITER,
        .icon = 213,
        .range = 1,
        .ppCost = 6,
        .power = 14,
    },
    // Wild Wolf
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_NONE,
        .icon = 214,
        .range = 1,
        .ppCost = 3,
        .power = 30,
    },
    // Emu
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_JUPITER,
        .icon = 215,
        .range = 1,
        .ppCost = 10,
        .power = 75,
    },
    // Roc
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_NONE,
        .icon = 216,
        .range = 1,
        .ppCost = 22,
        .power = 140,
    },
    // Salamander
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ENEMY_DAMAGE,
        .element = ELEMENT_MARS,
        .icon = 217,
        .range = 2,
        .ppCost = 7,
        .power = 45,
    },
    // Orc
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_NONE,
        .icon = 218,
        .range = 1,
        .ppCost = 3,
        .power = 35,
    },
    // Harpy
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_JUPITER,
        .icon = 219,
        .range = 1,
        .ppCost = 10,
        .power = 80,
    },
    // Grand Golem
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .icon = 220,
        .range = 1,
        .ppCost = 22,
        .power = 150,
    },
    // Cerberus
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ENEMY_DAMAGE,
        .element = ELEMENT_MARS,
        .icon = 221,
        .range = 2,
        .ppCost = 7,
        .power = 50,
    },
    // Wyvern
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ENEMY_DAMAGE,
        .element = ELEMENT_MARS,
        .icon = 222,
        .range = 3,
        .ppCost = 17,
        .power = 120,
    },
    // Pixie
    {
        .targetMode = ABILITY_TARGET_ALLIES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_HEAL_HP,
        .element = ELEMENT_MERCURY,
        .icon = 223,
        .range = 1,
        .ppCost = 5,
        .power = 115,
    },
    // Dinox
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .icon = 224,
        .range = 1,
        .ppCost = 3,
        .power = 40,
    },
    // Gryphon
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_JUPITER,
        .effect = ABILITY_EFFECT_DOUBLE_BLOW_60,
        .icon = 225,
        .range = 1,
        .ppCost = 10,
        .power = 85,
    },
    // Living Armor
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .icon = 226,
        .range = 1,
        .ppCost = 17,
        .power = 130,
    },
    // Chimera
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ENEMY_DAMAGE,
        .element = ELEMENT_MARS,
        .icon = 227,
        .range = 2,
        .ppCost = 7,
        .power = 55,
    },
    // Blue Dragon
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ENEMY_DAMAGE,
        .element = ELEMENT_MERCURY,
        .icon = 228,
        .range = 3,
        .ppCost = 17,
        .power = 130,
    },
    // Faery
    {
        .targetMode = ABILITY_TARGET_ALLIES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_HEAL_HP,
        .element = ELEMENT_MERCURY,
        .icon = 229,
        .range = 1,
        .ppCost = 5,
        .power = 120,
    },
    // Elder Wood
    {
        .targetMode = ABILITY_TARGET_ALLIES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_HEAL_HP,
        .element = ELEMENT_MERCURY,
        .icon = 230,
        .range = ABILITY_RANGE_ALL,
        .ppCost = 14,
        .power = 170,
    },
    // Lich
    {
        .targetMode = ABILITY_TARGET_ALLIES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_EFFECT,
        .effect = ABILITY_EFFECT_REVIVE,
        .icon = 231,
        .range = 1,
        .ppCost = 10,
    },
    // Troll
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .icon = 232,
        .range = 1,
        .ppCost = 3,
        .power = 45,
    },
    // Minotaur
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .icon = 233,
        .range = 1,
        .ppCost = 10,
        .power = 90,
    },
    // Ghost Soldier
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_JUPITER,
        .effect = ABILITY_EFFECT_HALVE_DEFENSE,
        .icon = 234,
        .range = 1,
        .ppCost = 22,
        .power = 170,
    },
    // Macetail
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ENEMY_DAMAGE,
        .element = ELEMENT_MARS,
        .icon = 235,
        .range = 2,
        .ppCost = 7,
        .power = 60,
    },
    // Fire Dragon
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ENEMY_DAMAGE,
        .element = ELEMENT_MARS,
        .icon = 236,
        .range = 3,
        .ppCost = 17,
        .power = 140,
    },
    // Weird Nymph
    {
        .targetMode = ABILITY_TARGET_ALLIES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_HEAL_HP,
        .element = ELEMENT_MERCURY,
        .icon = 237,
        .range = 1,
        .ppCost = 5,
        .power = 125,
    },
    // Succubus
    {
        .targetMode = ABILITY_TARGET_ALLIES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_HEAL_HP,
        .element = ELEMENT_MERCURY,
        .icon = 238,
        .range = 1,
        .ppCost = 9,
        .power = 250,
    },
    // Estre Wood
    {
        .targetMode = ABILITY_TARGET_ALLIES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_HEAL_HP,
        .element = ELEMENT_MERCURY,
        .icon = 239,
        .range = ABILITY_RANGE_ALL,
        .ppCost = 14,
        .power = 180,
    },
    // Manticore
    {
        .targetMode = ABILITY_TARGET_ALLIES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_HEAL_HP,
        .element = ELEMENT_MARS,
        .icon = 240,
        .range = ABILITY_RANGE_ALL,
        .ppCost = 18,
        .power = 300,
    },
    // Phoenix
    {
        .targetMode = ABILITY_TARGET_ALLIES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_EFFECT,
        .element = ELEMENT_MARS,
        .effect = ABILITY_EFFECT_REVIVE,
        .icon = 241,
        .range = 1,
        .ppCost = 10,
    },
    // ?
    {0},
    // Call Zombie
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_MERCURY,
        .icon = 252,
        .range = 1,
        .ppCost = 5,
        .power = 40,
    },
    // Call Demon
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK_MULTIPLIED,
        .effect = ABILITY_EFFECT_LEAVE_ONE_HP,
        .icon = 253,
        .range = 1,
        .ppCost = 13,
        .power = 19,
    },
    // Call Dullahan
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK_MULTIPLIED,
        .element = ELEMENT_JUPITER,
        .icon = 254,
        .range = 1,
        .ppCost = 21,
        .power = 30,
    },
    // Raging Heat
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_MARS,
        .icon = 255,
        .range = 3,
        .ppCost = 9,
        .power = 45,
    },
    // Fiery Abyss
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_MARS,
        .icon = 256,
        .range = 3,
        .ppCost = 18,
        .power = 90,
    },
    // Dire Inferno
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_MARS,
        .icon = 257,
        .range = 4,
        .ppCost = 32,
        .power = 200,
    },
    // Poison Flow
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_JUPITER,
        .effect = ABILITY_EFFECT_DEADLY_POISON,
        .icon = 258,
        .range = 3,
        .ppCost = 28,
        .power = 125,
    },
    // Fire Puppet
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_EFFECT,
        .effect = 85,
        .icon = 259,
        .range = 2,
        .ppCost = 7,
    },
    // ?
    {0},
    // ?
    {0},
    // ?
    {0},
    // Sand Breath
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ENEMY_DAMAGE,
        .range = 2,
        .power = 35,
    },
    // Desert Gasp
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ENEMY_DAMAGE,
        .effect = ABILITY_EFFECT_DEFENSE_DOWN,
        .range = 2,
        .power = 45,
    },
    // Black Ice
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ENEMY_DAMAGE,
        .range = 2,
        .power = 80,
    },
    // Toxic Breath
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ENEMY_DAMAGE,
        .effect = ABILITY_EFFECT_POISON,
        .range = 2,
        .power = 60,
    },
    // Typhoon Blow
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ENEMY_DAMAGE,
        .element = ELEMENT_MERCURY,
        .range = 2,
        .power = 150,
    },
    // Blast Breath
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ENEMY_DAMAGE,
        .element = ELEMENT_MARS,
        .effect = ABILITY_EFFECT_DEFENSE_DOWN,
        .range = 2,
        .power = 170,
    },
    // Gravel Blow
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ENEMY_DAMAGE,
        .range = 2,
        .power = 90,
    },
    // Darksol Gasp
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ENEMY_DAMAGE,
        .effect = ABILITY_EFFECT_HAUNT,
        .range = 2,
        .power = 230,
    },
    // Flame Breath
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ENEMY_DAMAGE,
        .element = ELEMENT_MARS,
        .range = 2,
        .power = 170,
    },
    // Aqua Breath
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ENEMY_DAMAGE,
        .element = ELEMENT_MERCURY,
        .range = 2,
        .power = 140,
    },
    // Fire Breath
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ENEMY_DAMAGE,
        .element = ELEMENT_MARS,
        .range = 2,
        .power = 180,
    },
    // Chill Breath
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ENEMY_DAMAGE,
        .element = ELEMENT_MERCURY,
        .range = 2,
        .power = 210,
    },
    // ?
    {0},
    // ?
    {0},
    // ?
    {0},
    // ?
    {0},
    // ?
    {0},
    // ?
    {0},
    // ?
    {0},
    // ?
    {0},
    // Flash Punch
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_JUPITER,
        .effect = ABILITY_EFFECT_DELUSION,
        .range = 1,
        .power = 10,
    },
    // Formic Acid
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_MARS,
        .effect = ABILITY_EFFECT_STUN,
        .range = 1,
        .power = 15,
    },
    // Claw Attack
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_NONE,
        .range = 1,
        .power = 20,
    },
    // Beat Dance
    {
        .targetMode = ABILITY_TARGET_SELF,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_EFFECT,
        .element = ELEMENT_NONE,
        .effect = ABILITY_EFFECT_ATTACK_UP_2,
        .range = 1,
    },
    // Twin Shear
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK_MULTIPLIED,
        .range = 1,
        .power = 18,
    },
    // Paralytail
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .effect = ABILITY_EFFECT_STUN,
        .range = 1,
        .power = 20,
    },
    // Echo Cut
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_NONE,
        .range = 1,
        .power = 30,
    },
    // Clarion Cry
    {
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_NO_POWER,
        .element = ELEMENT_NONE,
        .effect = 86,
        .range = 1,
    },
    // Triple Chomp
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK_MULTIPLIED,
        .element = ELEMENT_MERCURY,
        .range = 1,
        .power = 24,
    },
    // Raging Flood
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_MERCURY,
        .range = 6,
        .power = 110,
    },
    // Rising Venom
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_MERCURY,
        .effect = ABILITY_EFFECT_POISON,
        .range = 1,
        .power = 30,
    },
    // Slaver
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_EFFECT,
        .element = ELEMENT_MERCURY,
        .effect = ABILITY_EFFECT_DEFENSE_DOWN,
        .range = 3,
    },
    // Mighty Press
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .effect = ABILITY_EFFECT_INSTANT_DEATH,
        .range = 1,
        .power = 50,
    },
    // Star Mine
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_SUMMON,
        .element = ELEMENT_MARS,
        .range = 6,
        .power = 35,
    },
    // Heat Stun
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK_MULTIPLIED,
        .element = ELEMENT_MARS,
        .effect = ABILITY_EFFECT_STUN,
        .range = 1,
        .power = 13,
    },
    // Ocean Fist
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_NONE,
        .effect = ABILITY_EFFECT_LEAVE_ONE_HP,
        .range = 1,
        .power = 50,
    },
    // Watery Grave
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_NONE,
        .effect = ABILITY_EFFECT_INSTANT_DEATH,
        .range = 6,
        .power = 130,
    },
    // Counter-rush
    {
        .targetMode = ABILITY_TARGET_SELF,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_EFFECT_ONLY,
        .element = ELEMENT_MERCURY,
        .effect = 74,
        .range = 1,
    },
    // Counter
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_MERCURY,
        .range = 1,
        .power = 70,
    },
    // Bosca Hit
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK_MULTIPLIED,
        .element = ELEMENT_NONE,
        .range = 1,
        .power = 17,
    },
    // Strong Hit
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK_MULTIPLIED,
        .element = ELEMENT_NONE,
        .range = 1,
        .power = 14,
    },
    // Rolling Flame
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_MARS,
        .range = 2,
        .power = 100,
    },
    // Rising Dragon
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_MARS,
        .range = 1,
        .ppCost = 37,
        .power = 250,
    },
    // Meteor Blow
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_SUMMON,
        .element = ELEMENT_MARS,
        .range = 6,
        .ppCost = 21,
        .power = 120,
    },
    // Cage
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_EFFECT,
        .element = ELEMENT_MARS,
        .effect = ABILITY_EFFECT_GROUND,
        .range = 1,
        .power = 8,
    },
    // Stun Muscle
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK_MULTIPLIED,
        .element = ELEMENT_MARS,
        .effect = ABILITY_EFFECT_STUN,
        .range = 1,
        .power = 13,
    },
    // Djinnfest
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_EFFECT_ONLY,
        .element = ELEMENT_MARS,
        .range = ABILITY_RANGE_ALL,
    },
    // Heat Kiss
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_EFFECT,
        .element = ELEMENT_MARS,
        .effect = ABILITY_EFFECT_ATTACK_DOWN_2,
        .range = 2,
    },
    // Fatal Fang
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_NONE,
        .range = 1,
        .power = 11,
    },
    // Angle Spike
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_MARS,
        .range = 1,
        .power = 9,
    },
    // Quick Slash
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK_MULTIPLIED,
        .element = ELEMENT_JUPITER,
        .range = 1,
        .power = 13,
    },
    // Doublestep
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK_MULTIPLIED,
        .element = ELEMENT_NONE,
        .range = 1,
        .power = 18,
    },
    // Maneater
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_NONE,
        .effect = ABILITY_EFFECT_DRAIN_HP,
        .range = 1,
        .power = 50,
    },
    // Claw Slash
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_JUPITER,
        .range = 1,
        .power = 16,
    },
    // Mega Slash
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK_MULTIPLIED,
        .element = ELEMENT_JUPITER,
        .range = 1,
        .power = 15,
    },
    // Vanish Claw
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .effect = ABILITY_EFFECT_DELUSION,
        .range = 1,
        .power = 21,
    },
    // Crusher Grip
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK_MULTIPLIED,
        .effect = ABILITY_EFFECT_STUN,
        .range = 1,
        .power = 13,
    },
    // Ur Flash
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK_MULTIPLIED,
        .element = ELEMENT_JUPITER,
        .range = 1,
        .power = 14,
    },
    // Power Bite
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .range = 1,
        .power = 12,
    },
    // Terrible Bite
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK_MULTIPLIED,
        .effect = ABILITY_EFFECT_LEAVE_ONE_HP,
        .range = 1,
        .power = 13,
    },
    // Poison Sting
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK_MULTIPLIED,
        .effect = ABILITY_EFFECT_POISON,
        .range = 1,
        .power = 12,
    },
    // Kill Sting
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK_MULTIPLIED,
        .effect = ABILITY_EFFECT_INSTANT_DEATH,
        .range = 1,
        .power = 11,
    },
    // Beast Needle
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_MERCURY,
        .range = 1,
        .power = 15,
    },
    // Poison Sting
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_MERCURY,
        .effect = ABILITY_EFFECT_POISON,
        .range = 1,
        .power = 20,
    },
    // Stun Sting
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_MERCURY,
        .effect = ABILITY_EFFECT_STUN,
        .range = 1,
        .power = 25,
    },
    // Mucous Gel
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_EFFECT,
        .element = ELEMENT_MERCURY,
        .effect = ABILITY_EFFECT_DELUSION,
        .range = 1,
    },
    // Poison Gel
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_EFFECT,
        .element = ELEMENT_MERCURY,
        .effect = ABILITY_EFFECT_DEADLY_POISON,
        .range = 1,
    },
    // Hydro Slash
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK_MULTIPLIED,
        .element = ELEMENT_MERCURY,
        .range = 1,
        .power = 14,
    },
    // Regen Dance
    {
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_EFFECT,
        .element = ELEMENT_NONE,
        .effect = 87,
        .range = 1,
    },
    // Fire Dance
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK_MULTIPLIED,
        .element = ELEMENT_MARS,
        .range = 1,
        .power = 13,
    },
    // Power Crush
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK_MULTIPLIED,
        .effect = ABILITY_EFFECT_LEAVE_ONE_HP,
        .range = 1,
        .power = 12,
    },
    // Sack
    {
        .targetMode = ABILITY_TARGET_SELF,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_EFFECT,
        .effect = ABILITY_EFFECT_ATTACK_UP_2,
        .range = 1,
    },
    // Vital Moon
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK_MULTIPLIED,
        .effect = ABILITY_EFFECT_LEAVE_ONE_HP,
        .range = 1,
        .power = 14,
    },
    // Human Hunt
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK_MULTIPLIED,
        .effect = ABILITY_EFFECT_INSTANT_DEATH,
        .range = 1,
    },
    // Demon Eye
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_EFFECT,
        .effect = ABILITY_EFFECT_HAUNT,
        .range = ABILITY_RANGE_ALL,
    },
    // Stun Jip
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK_MULTIPLIED,
        .effect = ABILITY_EFFECT_STUN,
        .range = 1,
        .power = 16,
    },
    // Pike Assault
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK_MULTIPLIED,
        .element = ELEMENT_JUPITER,
        .effect = ABILITY_EFFECT_HALVE_DEFENSE,
        .range = 1,
        .power = 14,
    },
    // Formina Sage
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK_MULTIPLIED,
        .element = ELEMENT_JUPITER,
        .range = 1,
        .power = 28,
    },
    // True Collide
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .effect = ABILITY_EFFECT_DRAIN_HP,
        .range = 2,
        .power = 180,
    },
    // Armor Crush
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK_MULTIPLIED,
        .effect = ABILITY_EFFECT_DEFENSE_DOWN_2,
        .range = 1,
        .power = 14,
    },
    // Djinn Blast
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_EFFECT_ONLY,
        .element = ELEMENT_NONE,
        .range = 1,
    },
    // Djinn Storm
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_EFFECT_ONLY,
        .element = ELEMENT_NONE,
        .range = ABILITY_RANGE_ALL,
    },
    // Earth Force
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_EFFECT,
        .effect = ABILITY_EFFECT_GROUND,
        .range = 1,
    },
    // Guard Aura
    {
        .targetMode = ABILITY_TARGET_SELF,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_NO_POWER,
        .effect = 88,
        .range = 1,
    },
    // Cruel Ruin
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_SUMMON,
        .range = 6,
        .power = 200,
    },
    // Mystic Call
    {
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_NO_POWER,
        .element = ELEMENT_NONE,
        .effect = 86,
        .range = 1,
    },
    // Mine Ball
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK_MULTIPLIED,
        .element = ELEMENT_MERCURY,
        .range = 3,
        .power = 15,
    },
    // Angry Mine
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .effect = ABILITY_EFFECT_SELF_DESTRUCT,
        .range = 2,
        .power = 300,
    },
    // Djinn Stun
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_EFFECT_ONLY,
        .element = ELEMENT_NONE,
        .range = ABILITY_RANGE_ALL,
    },
    // Crucible
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_EFFECT_ONLY,
        .element = ELEMENT_NONE,
        .range = ABILITY_RANGE_ALL,
    },
    // Element Swap
    {
        .targetMode = ABILITY_TARGET_SELF,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_NO_POWER,
        .element = ELEMENT_NONE,
        .effect = ABILITY_EFFECT_THREATEN,
        .range = 1,
    },
    // Earnest Ply
    {
        .targetMode = ABILITY_TARGET_SELF,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_USABLE_IN_FIELD | ABILITY_FORMULA_HEAL_HP,
        .element = ELEMENT_MERCURY,
        .icon = 66,
        .fieldPsynergy = FIELD_PSYNERGY_PLY,
        .range = 1,
        .ppCost = 12,
        .power = 1000,
    },
    // Psy Boost
    {
        .targetMode = ABILITY_TARGET_SELF,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_HEAL_PP,
        .element = ELEMENT_NONE,
        .range = 1,
        .power = 20,
    },
    // Pressure
    {
        .targetMode = ABILITY_TARGET_SELF,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_NO_POWER,
        .element = ELEMENT_NONE,
        .effect = ABILITY_EFFECT_THREATEN,
        .range = 1,
    },
};
// clang-format on
