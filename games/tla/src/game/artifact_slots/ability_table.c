/// @file
/// The 734 abilities, by ability id. Each name is the English release's
/// (MsgAbilityNothing plus the ability id).
// Part of game.c.

#include "../artifact_slots_internal.h"

// clang-format off
const struct AbilityDefinition gAbilityDefinitions[] = {
    // Nothing
    {
        .targetMode = 1,
        .flags = 0x5,
        .element = ELEMENT_NONE,
        .range = 1,
    },
    // Attack
    {
        .targetMode = 1,
        .flags = 0x83,
        .element = ELEMENT_NONE,
        .range = 1,
    },
    // Defend
    {
        .targetMode = 4,
        .flags = 0x89,
        .element = ELEMENT_NONE,
        .effect = 46,
        .range = 1,
    },
    // Quake
    {
        .targetMode = 1,
        .flags = 0x85,
        .icon = 1,
        .range = 2,
        .ppCost = 4,
        .power = 12,
    },
    // Earthquake
    {
        .targetMode = 1,
        .flags = 0x85,
        .icon = 2,
        .range = 3,
        .ppCost = 7,
        .power = 35,
    },
    // Quake Sphere
    {
        .targetMode = 1,
        .flags = 0x85,
        .icon = 3,
        .range = 4,
        .ppCost = 15,
        .power = 65,
    },
    // Spire
    {
        .targetMode = 1,
        .flags = 0x85,
        .icon = 4,
        .range = 1,
        .ppCost = 5,
        .power = 40,
    },
    // Clay Spire
    {
        .targetMode = 1,
        .flags = 0x85,
        .icon = 5,
        .range = 2,
        .ppCost = 13,
        .power = 85,
    },
    // Stone Spire
    {
        .targetMode = 1,
        .flags = 0x85,
        .icon = 6,
        .range = 2,
        .ppCost = 22,
        .power = 160,
    },
    // Gaia
    {
        .targetMode = 1,
        .flags = 0x85,
        .icon = 7,
        .range = 2,
        .ppCost = 7,
        .power = 40,
    },
    // Mother Gaia
    {
        .targetMode = 1,
        .flags = 0x85,
        .icon = 8,
        .range = 3,
        .ppCost = 17,
        .power = 100,
    },
    // Grand Gaia
    {
        .targetMode = 1,
        .flags = 0x85,
        .icon = 9,
        .range = 3,
        .ppCost = 32,
        .power = 200,
    },
    // Growth
    {
        .targetMode = 1,
        .flags = 0x85,
        .icon = 10,
        .fieldPsynergy = FIELD_PSYNERGY_GROWTH,
        .range = 1,
        .ppCost = 4,
        .power = 25,
    },
    // Mad Growth
    {
        .targetMode = 1,
        .flags = 0x85,
        .icon = 11,
        .range = 2,
        .ppCost = 10,
        .power = 60,
    },
    // Wild Growth
    {
        .targetMode = 1,
        .flags = 0x85,
        .icon = 12,
        .range = 3,
        .ppCost = 19,
        .power = 110,
    },
    // Thorn
    {
        .targetMode = 1,
        .flags = 0x85,
        .icon = 13,
        .range = 2,
        .ppCost = 6,
        .power = 35,
    },
    // Briar
    {
        .targetMode = 1,
        .flags = 0x85,
        .icon = 14,
        .range = 2,
        .ppCost = 11,
        .power = 70,
    },
    // Nettle
    {
        .targetMode = 1,
        .flags = 0x85,
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
        .targetMode = 1,
        .flags = 0x85,
        .element = ELEMENT_MERCURY,
        .icon = 16,
        .fieldPsynergy = FIELD_PSYNERGY_FROST,
        .range = 2,
        .ppCost = 5,
        .power = 20,
    },
    // Tundra
    {
        .targetMode = 1,
        .flags = 0x85,
        .element = ELEMENT_MERCURY,
        .icon = 17,
        .range = 2,
        .ppCost = 8,
        .power = 45,
    },
    // Glacier
    {
        .targetMode = 1,
        .flags = 0x85,
        .element = ELEMENT_MERCURY,
        .icon = 18,
        .range = 2,
        .ppCost = 15,
        .power = 100,
    },
    // Ice
    {
        .targetMode = 1,
        .flags = 0x85,
        .element = ELEMENT_MERCURY,
        .icon = 19,
        .range = 1,
        .ppCost = 5,
        .power = 35,
    },
    // Ice Horn
    {
        .targetMode = 1,
        .flags = 0x85,
        .element = ELEMENT_MERCURY,
        .icon = 20,
        .range = 2,
        .ppCost = 11,
        .power = 70,
    },
    // Ice Missile
    {
        .targetMode = 1,
        .flags = 0x85,
        .element = ELEMENT_MERCURY,
        .icon = 21,
        .range = 2,
        .ppCost = 23,
        .power = 160,
    },
    // Prism
    {
        .targetMode = 1,
        .flags = 0x85,
        .element = ELEMENT_MERCURY,
        .icon = 22,
        .range = 2,
        .ppCost = 7,
        .power = 40,
    },
    // Hail Prism
    {
        .targetMode = 1,
        .flags = 0x85,
        .element = ELEMENT_MERCURY,
        .icon = 23,
        .range = 3,
        .ppCost = 16,
        .power = 90,
    },
    // Freeze Prism
    {
        .targetMode = 1,
        .flags = 0x85,
        .element = ELEMENT_MERCURY,
        .icon = 24,
        .range = 3,
        .ppCost = 31,
        .power = 190,
    },
    // Douse
    {
        .targetMode = 1,
        .flags = 0x85,
        .element = ELEMENT_MERCURY,
        .icon = 28,
        .fieldPsynergy = FIELD_PSYNERGY_DOUSE,
        .range = 2,
        .ppCost = 5,
        .power = 25,
    },
    // Drench
    {
        .targetMode = 1,
        .flags = 0x85,
        .element = ELEMENT_MERCURY,
        .icon = 29,
        .range = 2,
        .ppCost = 10,
        .power = 60,
    },
    // Deluge
    {
        .targetMode = 1,
        .flags = 0x85,
        .element = ELEMENT_MERCURY,
        .icon = 30,
        .range = 3,
        .ppCost = 20,
        .power = 120,
    },
    // Froth
    {
        .targetMode = 1,
        .flags = 0x85,
        .element = ELEMENT_MERCURY,
        .icon = 25,
        .range = 2,
        .ppCost = 5,
        .power = 28,
    },
    // Froth Sphere
    {
        .targetMode = 1,
        .flags = 0x85,
        .element = ELEMENT_MERCURY,
        .icon = 26,
        .range = 3,
        .ppCost = 12,
        .power = 65,
    },
    // Froth Spiral
    {
        .targetMode = 1,
        .flags = 0x85,
        .element = ELEMENT_MERCURY,
        .icon = 27,
        .range = 4,
        .ppCost = 31,
        .power = 150,
    },
    // Cool
    {
        .targetMode = 1,
        .flags = 0x85,
        .element = ELEMENT_MERCURY,
        .icon = 173,
        .range = 2,
        .ppCost = 6,
        .power = 35,
    },
    // Supercool
    {
        .targetMode = 1,
        .flags = 0x85,
        .element = ELEMENT_MERCURY,
        .icon = 174,
        .range = 3,
        .ppCost = 14,
        .power = 80,
    },
    // Megacool
    {
        .targetMode = 1,
        .flags = 0x85,
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
        .targetMode = 1,
        .flags = 0x85,
        .element = ELEMENT_MARS,
        .icon = 31,
        .range = 2,
        .ppCost = 4,
        .power = 15,
    },
    // Flare Wall
    {
        .targetMode = 1,
        .flags = 0x85,
        .element = ELEMENT_MARS,
        .icon = 32,
        .range = 2,
        .ppCost = 7,
        .power = 40,
    },
    // Flare Storm
    {
        .targetMode = 1,
        .flags = 0x85,
        .element = ELEMENT_MARS,
        .icon = 33,
        .range = 2,
        .ppCost = 12,
        .power = 80,
    },
    // Fire
    {
        .targetMode = 1,
        .flags = 0x85,
        .element = ELEMENT_MARS,
        .icon = 34,
        .range = 2,
        .ppCost = 6,
        .power = 35,
    },
    // Fireball
    {
        .targetMode = 1,
        .flags = 0x85,
        .element = ELEMENT_MARS,
        .icon = 35,
        .range = 3,
        .ppCost = 12,
        .power = 65,
    },
    // Inferno
    {
        .targetMode = 1,
        .flags = 0x85,
        .element = ELEMENT_MARS,
        .icon = 36,
        .range = 3,
        .ppCost = 23,
        .power = 140,
    },
    // Volcano
    {
        .targetMode = 1,
        .flags = 0x85,
        .element = ELEMENT_MARS,
        .icon = 37,
        .range = 1,
        .ppCost = 6,
        .power = 45,
    },
    // Eruption
    {
        .targetMode = 1,
        .flags = 0x85,
        .element = ELEMENT_MARS,
        .icon = 38,
        .range = 2,
        .ppCost = 14,
        .power = 90,
    },
    // Pyroclasm
    {
        .targetMode = 1,
        .flags = 0x85,
        .element = ELEMENT_MARS,
        .icon = 39,
        .range = 3,
        .ppCost = 29,
        .power = 180,
    },
    // Blast
    {
        .targetMode = 1,
        .flags = 0x85,
        .element = ELEMENT_MARS,
        .icon = 40,
        .range = 2,
        .ppCost = 5,
        .power = 25,
    },
    // Mad Blast
    {
        .targetMode = 1,
        .flags = 0x85,
        .element = ELEMENT_MARS,
        .icon = 41,
        .range = 2,
        .ppCost = 9,
        .power = 50,
    },
    // Fiery Blast
    {
        .targetMode = 1,
        .flags = 0x85,
        .element = ELEMENT_MARS,
        .icon = 42,
        .range = 3,
        .ppCost = 19,
        .power = 110,
    },
    // Blast
    {
        .targetMode = 1,
        .flags = 0x85,
        .element = ELEMENT_MARS,
        .icon = 43,
        .range = 2,
        .ppCost = 7,
        .power = 40,
    },
    // Nova
    {
        .targetMode = 1,
        .flags = 0x85,
        .element = ELEMENT_MARS,
        .icon = 44,
        .range = 3,
        .ppCost = 13,
        .power = 70,
    },
    // Supernova
    {
        .targetMode = 1,
        .flags = 0x85,
        .element = ELEMENT_MARS,
        .icon = 45,
        .range = 4,
        .ppCost = 31,
        .power = 150,
    },
    // Fume
    {
        .targetMode = 1,
        .flags = 0x85,
        .element = ELEMENT_MARS,
        .icon = 176,
        .range = 1,
        .ppCost = 6,
        .power = 50,
    },
    // Serpent Fume
    {
        .targetMode = 1,
        .flags = 0x85,
        .element = ELEMENT_MARS,
        .icon = 177,
        .range = 1,
        .ppCost = 14,
        .power = 130,
    },
    // Dragon Fume
    {
        .targetMode = 1,
        .flags = 0x85,
        .element = ELEMENT_MARS,
        .icon = 178,
        .range = 1,
        .ppCost = 35,
        .power = 230,
    },
    // Beam
    {
        .targetMode = 1,
        .flags = 0x85,
        .element = ELEMENT_MARS,
        .icon = 179,
        .range = 2,
        .ppCost = 7,
        .power = 45,
    },
    // Cycle Beam
    {
        .targetMode = 1,
        .flags = 0x85,
        .element = ELEMENT_MARS,
        .icon = 180,
        .range = 3,
        .ppCost = 14,
        .power = 80,
    },
    // Searing Beam
    {
        .targetMode = 1,
        .flags = 0x85,
        .element = ELEMENT_MARS,
        .icon = 181,
        .range = 4,
        .ppCost = 36,
        .power = 170,
    },
    // Bolt
    {
        .targetMode = 1,
        .flags = 0x85,
        .element = ELEMENT_JUPITER,
        .icon = 46,
        .range = 1,
        .ppCost = 4,
        .power = 20,
    },
    // Flash Bolt
    {
        .targetMode = 1,
        .flags = 0x85,
        .element = ELEMENT_JUPITER,
        .icon = 47,
        .range = 2,
        .ppCost = 7,
        .power = 40,
    },
    // Blue Bolt
    {
        .targetMode = 1,
        .flags = 0x85,
        .element = ELEMENT_JUPITER,
        .icon = 48,
        .range = 2,
        .ppCost = 14,
        .power = 90,
    },
    // Ray
    {
        .targetMode = 1,
        .flags = 0x85,
        .element = ELEMENT_JUPITER,
        .icon = 49,
        .range = 2,
        .ppCost = 6,
        .power = 35,
    },
    // Storm Ray
    {
        .targetMode = 1,
        .flags = 0x85,
        .element = ELEMENT_JUPITER,
        .icon = 50,
        .range = 2,
        .ppCost = 10,
        .power = 65,
    },
    // Destruct Ray
    {
        .targetMode = 1,
        .flags = 0x85,
        .element = ELEMENT_JUPITER,
        .icon = 51,
        .range = 2,
        .ppCost = 21,
        .power = 150,
    },
    // Plasma
    {
        .targetMode = 1,
        .flags = 0x85,
        .element = ELEMENT_JUPITER,
        .icon = 52,
        .range = 2,
        .ppCost = 8,
        .power = 45,
    },
    // Shine Plasma
    {
        .targetMode = 1,
        .flags = 0x85,
        .element = ELEMENT_JUPITER,
        .icon = 53,
        .range = 3,
        .ppCost = 18,
        .power = 100,
    },
    // Spark Plasma
    {
        .targetMode = 1,
        .flags = 0x85,
        .element = ELEMENT_JUPITER,
        .icon = 54,
        .range = 4,
        .ppCost = 37,
        .power = 180,
    },
    // Slash
    {
        .targetMode = 1,
        .flags = 0x85,
        .element = ELEMENT_JUPITER,
        .icon = 55,
        .range = 1,
        .ppCost = 4,
        .power = 25,
    },
    // Wind Slash
    {
        .targetMode = 1,
        .flags = 0x85,
        .element = ELEMENT_JUPITER,
        .icon = 56,
        .range = 2,
        .ppCost = 9,
        .power = 50,
    },
    // Sonic Slash
    {
        .targetMode = 1,
        .flags = 0x85,
        .element = ELEMENT_JUPITER,
        .icon = 57,
        .range = 3,
        .ppCost = 20,
        .power = 120,
    },
    // Whirlwind
    {
        .targetMode = 1,
        .flags = 0x85,
        .element = ELEMENT_JUPITER,
        .icon = 58,
        .fieldPsynergy = FIELD_PSYNERGY_WHIRLWIND,
        .range = 2,
        .ppCost = 5,
        .power = 20,
    },
    // Tornado
    {
        .targetMode = 1,
        .flags = 0x85,
        .element = ELEMENT_JUPITER,
        .icon = 59,
        .range = 3,
        .ppCost = 14,
        .power = 80,
    },
    // Tempest
    {
        .targetMode = 1,
        .flags = 0x85,
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
        .targetMode = 2,
        .flags = 0xc1,
        .element = ELEMENT_MARS,
        .icon = 182,
        .range = 255,
        .ppCost = 7,
        .power = 50,
    },
    // Healing Aura
    {
        .targetMode = 2,
        .flags = 0xc1,
        .element = ELEMENT_MARS,
        .icon = 183,
        .range = 255,
        .ppCost = 11,
        .power = 100,
    },
    // Cool Aura
    {
        .targetMode = 2,
        .flags = 0xc1,
        .element = ELEMENT_MARS,
        .icon = 184,
        .range = 255,
        .ppCost = 16,
        .power = 200,
    },
    // Cure
    {
        .targetMode = 2,
        .flags = 0xc1,
        .icon = 61,
        .range = 1,
        .ppCost = 3,
        .power = 70,
    },
    // Cure Well
    {
        .targetMode = 2,
        .flags = 0xc1,
        .icon = 62,
        .range = 1,
        .ppCost = 7,
        .power = 150,
    },
    // Potent Cure
    {
        .targetMode = 2,
        .flags = 0xc1,
        .icon = 63,
        .range = 1,
        .ppCost = 10,
        .power = 300,
    },
    // Ply
    {
        .targetMode = 2,
        .flags = 0xc1,
        .element = ELEMENT_MERCURY,
        .icon = 64,
        .fieldPsynergy = FIELD_PSYNERGY_PLY,
        .range = 1,
        .ppCost = 4,
        .power = 100,
    },
    // Ply Well
    {
        .targetMode = 2,
        .flags = 0xc1,
        .element = ELEMENT_MERCURY,
        .icon = 65,
        .fieldPsynergy = FIELD_PSYNERGY_PLY,
        .range = 1,
        .ppCost = 8,
        .power = 200,
    },
    // Pure Ply
    {
        .targetMode = 2,
        .flags = 0xc1,
        .element = ELEMENT_MERCURY,
        .icon = 66,
        .fieldPsynergy = FIELD_PSYNERGY_PLY,
        .range = 1,
        .ppCost = 12,
        .power = 1000,
    },
    // Wish
    {
        .targetMode = 2,
        .flags = 0xc1,
        .element = ELEMENT_MERCURY,
        .icon = 67,
        .range = 255,
        .ppCost = 9,
        .power = 80,
    },
    // Wish Well
    {
        .targetMode = 2,
        .flags = 0xc1,
        .element = ELEMENT_MERCURY,
        .icon = 68,
        .range = 255,
        .ppCost = 13,
        .power = 160,
    },
    // Pure Wish
    {
        .targetMode = 2,
        .flags = 0xc1,
        .element = ELEMENT_MERCURY,
        .icon = 69,
        .range = 255,
        .ppCost = 20,
        .power = 400,
    },
    // Cure Poison
    {
        .targetMode = 2,
        .flags = 0xc2,
        .element = ELEMENT_MERCURY,
        .effect = 3,
        .icon = 70,
        .range = 1,
        .ppCost = 2,
    },
    // Restore
    {
        .targetMode = 2,
        .flags = 0x82,
        .element = ELEMENT_MERCURY,
        .effect = 4,
        .icon = 71,
        .range = 1,
        .ppCost = 3,
    },
    // Revive
    {
        .targetMode = 2,
        .flags = 0xc2,
        .effect = 5,
        .icon = 72,
        .range = 1,
        .ppCost = 15,
    },
    // Impact
    {
        .targetMode = 2,
        .flags = 0x82,
        .element = ELEMENT_JUPITER,
        .effect = 6,
        .icon = 73,
        .range = 1,
        .ppCost = 7,
    },
    // High Impact
    {
        .targetMode = 2,
        .flags = 0x82,
        .element = ELEMENT_JUPITER,
        .effect = 7,
        .icon = 74,
        .range = 255,
        .ppCost = 12,
    },
    // Dull
    {
        .targetMode = 1,
        .flags = 0x82,
        .element = ELEMENT_JUPITER,
        .effect = 8,
        .icon = 75,
        .range = 1,
        .ppCost = 6,
    },
    // Blunt
    {
        .targetMode = 1,
        .flags = 0x82,
        .element = ELEMENT_JUPITER,
        .effect = 9,
        .icon = 76,
        .range = 2,
        .ppCost = 11,
    },
    // Guard
    {
        .targetMode = 2,
        .flags = 0x82,
        .element = ELEMENT_MARS,
        .effect = 10,
        .icon = 77,
        .range = 1,
        .ppCost = 3,
    },
    // Protect
    {
        .targetMode = 2,
        .flags = 0x82,
        .element = ELEMENT_MARS,
        .effect = 11,
        .icon = 78,
        .range = 255,
        .ppCost = 5,
    },
    // Impair
    {
        .targetMode = 1,
        .flags = 0x82,
        .element = ELEMENT_MARS,
        .effect = 12,
        .icon = 79,
        .range = 1,
        .ppCost = 4,
    },
    // Debilitate
    {
        .targetMode = 1,
        .flags = 0x82,
        .element = ELEMENT_MARS,
        .effect = 13,
        .icon = 80,
        .range = 2,
        .ppCost = 6,
    },
    // Ward
    {
        .targetMode = 2,
        .flags = 0x82,
        .element = ELEMENT_JUPITER,
        .effect = 14,
        .icon = 81,
        .range = 1,
        .ppCost = 3,
    },
    // Resist
    {
        .targetMode = 2,
        .flags = 0x82,
        .element = ELEMENT_JUPITER,
        .effect = 15,
        .icon = 82,
        .range = 255,
        .ppCost = 5,
    },
    // Weaken
    {
        .targetMode = 1,
        .flags = 0x82,
        .element = ELEMENT_JUPITER,
        .effect = 16,
        .icon = 83,
        .range = 1,
        .ppCost = 4,
    },
    // Enfeeble
    {
        .targetMode = 1,
        .flags = 0x82,
        .element = ELEMENT_JUPITER,
        .effect = 17,
        .icon = 84,
        .range = 2,
        .ppCost = 6,
    },
    // Taint
    {
        .targetMode = 1,
        .flags = 0x82,
        .effect = 18,
        .range = 1,
        .ppCost = 4,
    },
    // Poison
    {
        .targetMode = 1,
        .flags = 0x82,
        .effect = 19,
        .range = 1,
        .ppCost = 6,
    },
    // Delude
    {
        .targetMode = 1,
        .flags = 0x82,
        .element = ELEMENT_JUPITER,
        .effect = 20,
        .icon = 85,
        .range = 2,
        .ppCost = 4,
    },
    // Confuse
    {
        .targetMode = 1,
        .flags = 0x82,
        .element = ELEMENT_JUPITER,
        .effect = 21,
        .range = 2,
        .ppCost = 6,
    },
    // Charm
    {
        .targetMode = 1,
        .flags = 0x82,
        .element = ELEMENT_JUPITER,
        .effect = 22,
        .range = 1,
        .ppCost = 9,
    },
    // Paralyze
    {
        .targetMode = 1,
        .flags = 0x82,
        .element = ELEMENT_JUPITER,
        .effect = 23,
        .range = 1,
        .ppCost = 7,
    },
    // Sleep
    {
        .targetMode = 1,
        .flags = 0x82,
        .element = ELEMENT_JUPITER,
        .effect = 24,
        .icon = 86,
        .range = 2,
        .ppCost = 5,
    },
    // Bind
    {
        .targetMode = 1,
        .flags = 0x82,
        .element = ELEMENT_JUPITER,
        .effect = 25,
        .icon = 87,
        .range = 1,
        .ppCost = 4,
    },
    // Haunt
    {
        .targetMode = 1,
        .flags = 0x82,
        .effect = 26,
        .icon = 88,
        .range = 2,
        .ppCost = 5,
    },
    // Curse
    {
        .targetMode = 1,
        .flags = 0x82,
        .effect = 28,
        .icon = 90,
        .range = 1,
        .ppCost = 6,
    },
    // Condemn
    {
        .targetMode = 1,
        .flags = 0x82,
        .effect = 27,
        .icon = 89,
        .range = 1,
        .ppCost = 8,
    },
    // Drain
    {
        .targetMode = 1,
        .flags = 0x82,
        .element = ELEMENT_JUPITER,
        .effect = 31,
        .icon = 91,
        .range = 1,
        .ppCost = 3,
        .power = 50,
    },
    // Psy Drain
    {
        .targetMode = 1,
        .flags = 0x82,
        .element = ELEMENT_JUPITER,
        .effect = 32,
        .icon = 92,
        .range = 1,
        .power = 15,
    },
    // Break
    {
        .targetMode = 1,
        .flags = 0x82,
        .element = ELEMENT_MERCURY,
        .effect = 33,
        .icon = 93,
        .range = 255,
        .ppCost = 5,
    },
    // Regenerate
    {
        .targetMode = 2,
        .flags = 0x82,
        .element = ELEMENT_MERCURY,
        .effect = 29,
        .range = 1,
        .ppCost = 6,
        .power = 60,
    },
    // Reflect
    {
        .targetMode = 2,
        .flags = 0x82,
        .element = ELEMENT_MERCURY,
        .effect = 30,
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
        .flags = 0x9,
        .element = ELEMENT_JUPITER,
        .icon = 159,
        .fieldPsynergy = FIELD_PSYNERGY_LASH,
        .range = 1,
        .ppCost = 1,
    },
    // Pound
    {
        .flags = 0x9,
        .element = ELEMENT_MARS,
        .icon = 160,
        .fieldPsynergy = FIELD_PSYNERGY_POUND,
        .range = 1,
        .ppCost = 2,
    },
    // Tremor
    {
        .flags = 0x9,
        .icon = 161,
        .fieldPsynergy = FIELD_PSYNERGY_TREMOR,
        .range = 1,
        .ppCost = 1,
    },
    // Scoop
    {
        .flags = 0x9,
        .icon = 162,
        .fieldPsynergy = FIELD_PSYNERGY_SCOOP,
        .range = 1,
        .ppCost = 1,
    },
    // Cyclone
    {
        .flags = 0x9,
        .element = ELEMENT_JUPITER,
        .icon = 167,
        .fieldPsynergy = FIELD_PSYNERGY_CYCLONE,
        .range = 1,
        .ppCost = 2,
    },
    // Parch
    {
        .flags = 0x9,
        .element = ELEMENT_MERCURY,
        .icon = 164,
        .fieldPsynergy = FIELD_PSYNERGY_PARCH,
        .range = 1,
        .ppCost = 2,
    },
    // Sand
    {
        .flags = 0x9,
        .icon = 165,
        .fieldPsynergy = FIELD_PSYNERGY_SAND,
        .range = 1,
        .ppCost = 2,
    },
    // Move
    {
        .flags = 0x9,
        .element = ELEMENT_MARS,
        .icon = 95,
        .fieldPsynergy = FIELD_PSYNERGY_MOVE,
        .range = 1,
        .ppCost = 2,
    },
    // Mind Read
    {
        .flags = 0x9,
        .element = ELEMENT_JUPITER,
        .icon = 96,
        .fieldPsynergy = FIELD_PSYNERGY_MIND_READ,
        .range = 1,
        .ppCost = 1,
    },
    // Force
    {
        .flags = 0x9,
        .element = ELEMENT_NONE,
        .icon = 97,
        .fieldPsynergy = FIELD_PSYNERGY_FORCE,
        .range = 1,
        .ppCost = 2,
    },
    // Lift
    {
        .flags = 0x9,
        .element = ELEMENT_MERCURY,
        .icon = 98,
        .fieldPsynergy = FIELD_PSYNERGY_LIFT,
        .range = 1,
        .ppCost = 2,
    },
    // Reveal
    {
        .flags = 0x9,
        .element = ELEMENT_JUPITER,
        .icon = 99,
        .fieldPsynergy = FIELD_PSYNERGY_REVEAL,
        .range = 1,
        .ppCost = 1,
    },
    // Halt
    {
        .flags = 0x9,
        .element = ELEMENT_JUPITER,
        .icon = 100,
        .fieldPsynergy = FIELD_PSYNERGY_HALT,
        .range = 1,
        .ppCost = 2,
    },
    // Cloak
    {
        .flags = 0x9,
        .element = ELEMENT_MERCURY,
        .icon = 101,
        .fieldPsynergy = FIELD_PSYNERGY_CLOAK,
        .range = 1,
        .ppCost = 1,
    },
    // Carry
    {
        .flags = 0x9,
        .icon = 102,
        .fieldPsynergy = FIELD_PSYNERGY_CARRY,
        .range = 1,
        .ppCost = 2,
    },
    // Catch
    {
        .flags = 0x9,
        .icon = 103,
        .fieldPsynergy = FIELD_PSYNERGY_CATCH,
        .range = 1,
        .ppCost = 1,
    },
    // Retreat
    {
        .flags = 0x9,
        .icon = 156,
        .fieldPsynergy = FIELD_PSYNERGY_RETREAT,
        .range = 1,
        .ppCost = 6,
    },
    // Avoid
    {
        .flags = 0x9,
        .element = ELEMENT_MERCURY,
        .icon = 157,
        .fieldPsynergy = FIELD_PSYNERGY_AVOID,
        .range = 1,
        .ppCost = 5,
    },
    // Burst
    {
        .flags = 0x9,
        .element = ELEMENT_MARS,
        .icon = 163,
        .fieldPsynergy = FIELD_PSYNERGY_BURST,
        .range = 1,
        .ppCost = 2,
    },
    // Grind
    {
        .flags = 0x9,
        .icon = 169,
        .fieldPsynergy = FIELD_PSYNERGY_GRIND,
        .range = 1,
        .ppCost = 2,
    },
    // Hover
    {
        .flags = 0x9,
        .element = ELEMENT_JUPITER,
        .icon = 166,
        .fieldPsynergy = FIELD_PSYNERGY_HOVER,
        .range = 1,
        .ppCost = 2,
    },
    // Blaze
    {
        .flags = 0x9,
        .element = ELEMENT_MARS,
        .icon = 168,
        .fieldPsynergy = FIELD_PSYNERGY_BLAZE,
        .range = 1,
        .ppCost = 1,
    },
    // Ma????
    {
        .flags = 0x9,
        .icon = 170,
        .fieldPsynergy = FIELD_PSYNERGY_UNNAMED_28,
        .range = 1,
        .ppCost = 1,
    },
    // Teleport
    {
        .flags = 0x9,
        .element = ELEMENT_JUPITER,
        .icon = 172,
        .fieldPsynergy = FIELD_PSYNERGY_TELEPORT,
        .range = 1,
        .ppCost = 3,
    },
    // A??
    {
        .flags = 0x9,
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
        .targetMode = 1,
        .flags = 0x85,
        .element = ELEMENT_MARS,
        .icon = 117,
        .range = 1,
        .ppCost = 6,
        .power = 80,
    },
    // Demon Night
    {
        .targetMode = 1,
        .flags = 0x85,
        .effect = 26,
        .icon = 119,
        .range = 2,
        .ppCost = 12,
        .power = 60,
    },
    // Helm Splitter
    {
        .targetMode = 1,
        .flags = 0x83,
        .effect = 27,
        .icon = 116,
        .range = 1,
        .ppCost = 8,
        .power = 30,
    },
    // Quick Strike
    {
        .targetMode = 1,
        .flags = 0x84,
        .element = ELEMENT_JUPITER,
        .icon = 118,
        .range = 1,
        .ppCost = 12,
        .power = 18,
    },
    // Rockfall
    {
        .targetMode = 1,
        .flags = 0x85,
        .icon = 104,
        .range = 2,
        .ppCost = 5,
        .power = 30,
    },
    // Rockslide
    {
        .targetMode = 1,
        .flags = 0x85,
        .icon = 105,
        .range = 3,
        .ppCost = 15,
        .power = 90,
    },
    // Avalanche
    {
        .targetMode = 1,
        .flags = 0x85,
        .icon = 106,
        .range = 3,
        .ppCost = 30,
        .power = 160,
    },
    // Lava Shower
    {
        .targetMode = 1,
        .flags = 0x85,
        .element = ELEMENT_MARS,
        .icon = 107,
        .range = 1,
        .ppCost = 4,
        .power = 40,
    },
    // Molten Bath
    {
        .targetMode = 1,
        .flags = 0x85,
        .element = ELEMENT_MARS,
        .icon = 108,
        .range = 2,
        .ppCost = 12,
        .power = 70,
    },
    // Magma Storm
    {
        .targetMode = 1,
        .flags = 0x85,
        .element = ELEMENT_MARS,
        .icon = 109,
        .range = 3,
        .ppCost = 27,
        .power = 120,
    },
    // Demon Spear
    {
        .targetMode = 2,
        .flags = 0x82,
        .element = ELEMENT_JUPITER,
        .effect = 6,
        .icon = 110,
        .range = 1,
        .ppCost = 7,
    },
    // Angel Spear
    {
        .targetMode = 2,
        .flags = 0x82,
        .element = ELEMENT_JUPITER,
        .effect = 7,
        .icon = 111,
        .range = 255,
        .ppCost = 12,
    },
    // Guardian
    {
        .targetMode = 2,
        .flags = 0x82,
        .element = ELEMENT_MARS,
        .effect = 10,
        .icon = 114,
        .range = 1,
        .ppCost = 3,
    },
    // Protector
    {
        .targetMode = 2,
        .flags = 0x82,
        .element = ELEMENT_MARS,
        .effect = 11,
        .icon = 115,
        .range = 255,
        .ppCost = 5,
    },
    // Magic Shell
    {
        .targetMode = 2,
        .flags = 0x82,
        .element = ELEMENT_JUPITER,
        .effect = 14,
        .icon = 112,
        .range = 1,
        .ppCost = 3,
    },
    // Magic Shield
    {
        .targetMode = 2,
        .flags = 0x82,
        .element = ELEMENT_JUPITER,
        .effect = 15,
        .icon = 113,
        .range = 255,
        .ppCost = 5,
    },
    // Death Plunge
    {
        .targetMode = 1,
        .flags = 0x83,
        .element = ELEMENT_JUPITER,
        .effect = 23,
        .icon = 134,
        .range = 1,
        .ppCost = 14,
        .power = 40,
    },
    // Shuriken
    {
        .targetMode = 1,
        .flags = 0x84,
        .element = ELEMENT_JUPITER,
        .icon = 135,
        .range = 2,
        .ppCost = 8,
        .power = 8,
    },
    // Annihilation
    {
        .targetMode = 1,
        .flags = 0x84,
        .effect = 27,
        .icon = 133,
        .range = 1,
        .ppCost = 18,
        .power = 15,
    },
    // Punji
    {
        .targetMode = 1,
        .flags = 0x85,
        .icon = 120,
        .range = 2,
        .ppCost = 7,
        .power = 45,
    },
    // Punji Trap
    {
        .targetMode = 1,
        .flags = 0x85,
        .icon = 121,
        .range = 2,
        .ppCost = 13,
        .power = 85,
    },
    // Punji Strike
    {
        .targetMode = 1,
        .flags = 0x85,
        .icon = 122,
        .range = 3,
        .ppCost = 24,
        .power = 150,
    },
    // Fire Bomb
    {
        .targetMode = 1,
        .flags = 0x85,
        .element = ELEMENT_MARS,
        .icon = 123,
        .range = 2,
        .ppCost = 5,
        .power = 35,
    },
    // Cluster Bomb
    {
        .targetMode = 1,
        .flags = 0x85,
        .element = ELEMENT_MARS,
        .icon = 124,
        .range = 3,
        .ppCost = 11,
        .power = 65,
    },
    // Carpet Bomb
    {
        .targetMode = 1,
        .flags = 0x85,
        .element = ELEMENT_MARS,
        .icon = 125,
        .range = 4,
        .ppCost = 29,
        .power = 130,
    },
    // Gale
    {
        .targetMode = 1,
        .flags = 0x85,
        .element = ELEMENT_JUPITER,
        .icon = 126,
        .fieldPsynergy = FIELD_PSYNERGY_WHIRLWIND,
        .range = 2,
        .ppCost = 3,
        .power = 18,
    },
    // Typhoon
    {
        .targetMode = 1,
        .flags = 0x85,
        .element = ELEMENT_JUPITER,
        .icon = 127,
        .range = 3,
        .ppCost = 12,
        .power = 75,
    },
    // Hurricane
    {
        .targetMode = 1,
        .flags = 0x85,
        .element = ELEMENT_JUPITER,
        .icon = 128,
        .range = 3,
        .ppCost = 25,
        .power = 150,
    },
    // Thunderclap
    {
        .targetMode = 1,
        .flags = 0x85,
        .element = ELEMENT_JUPITER,
        .icon = 129,
        .range = 2,
        .ppCost = 9,
        .power = 50,
    },
    // Thunderbolt
    {
        .targetMode = 1,
        .flags = 0x85,
        .element = ELEMENT_JUPITER,
        .icon = 130,
        .range = 3,
        .ppCost = 19,
        .power = 110,
    },
    // Thunderhead
    {
        .targetMode = 1,
        .flags = 0x85,
        .element = ELEMENT_JUPITER,
        .icon = 131,
        .range = 4,
        .ppCost = 39,
        .power = 190,
    },
    // Mist
    {
        .targetMode = 1,
        .flags = 0x82,
        .element = ELEMENT_JUPITER,
        .effect = 20,
        .icon = 132,
        .range = 2,
        .ppCost = 4,
    },
    // Ragnarok
    {
        .targetMode = 1,
        .flags = 0x83,
        .icon = 137,
        .range = 1,
        .ppCost = 7,
        .power = 35,
    },
    // Cutting Edge
    {
        .targetMode = 1,
        .flags = 0x83,
        .element = ELEMENT_MERCURY,
        .icon = 136,
        .range = 1,
        .ppCost = 5,
        .power = 32,
    },
    // Heat Wave
    {
        .targetMode = 1,
        .flags = 0x83,
        .element = ELEMENT_MARS,
        .icon = 138,
        .range = 1,
        .ppCost = 6,
        .power = 33,
    },
    // Astral Blast
    {
        .targetMode = 1,
        .flags = 0x83,
        .element = ELEMENT_JUPITER,
        .icon = 158,
        .range = 1,
        .ppCost = 5,
        .power = 32,
    },
    // Planet Diver
    {
        .targetMode = 1,
        .flags = 0x83,
        .element = ELEMENT_MARS,
        .icon = 139,
        .range = 1,
        .ppCost = 7,
        .power = 36,
    },
    // Diamond Dust
    {
        .targetMode = 1,
        .flags = 0x83,
        .element = ELEMENT_MERCURY,
        .icon = 185,
        .range = 1,
        .ppCost = 6,
        .power = 34,
    },
    // Odyssey
    {
        .targetMode = 1,
        .flags = 0x83,
        .icon = 242,
        .range = 1,
        .ppCost = 18,
        .power = 95,
    },
    // Liquifier
    {
        .targetMode = 1,
        .flags = 0x83,
        .element = ELEMENT_MARS,
        .icon = 243,
        .range = 1,
        .ppCost = 17,
        .power = 90,
    },
    // Plume Edge
    {
        .targetMode = 1,
        .flags = 0x83,
        .element = ELEMENT_MERCURY,
        .icon = 244,
        .range = 1,
        .ppCost = 15,
        .power = 80,
    },
    // Thunder Mine
    {
        .targetMode = 1,
        .flags = 0x83,
        .element = ELEMENT_JUPITER,
        .icon = 245,
        .range = 1,
        .ppCost = 16,
        .power = 85,
    },
    // Planetary
    {
        .targetMode = 1,
        .flags = 0x83,
        .element = ELEMENT_MARS,
        .icon = 246,
        .range = 1,
        .ppCost = 19,
        .power = 98,
    },
    // Diamond Berg
    {
        .targetMode = 1,
        .flags = 0x83,
        .element = ELEMENT_MERCURY,
        .icon = 247,
        .range = 1,
        .ppCost = 17,
        .power = 92,
    },
    // Death Leap
    {
        .targetMode = 1,
        .flags = 0x83,
        .element = ELEMENT_JUPITER,
        .effect = 23,
        .icon = 248,
        .range = 1,
        .ppCost = 22,
        .power = 110,
    },
    // Epicenter
    {
        .targetMode = 1,
        .flags = 0x85,
        .element = ELEMENT_MARS,
        .icon = 249,
        .range = 1,
        .ppCost = 33,
        .power = 210,
    },
    // Thorny Grave
    {
        .targetMode = 1,
        .flags = 0x85,
        .effect = 26,
        .icon = 250,
        .range = 2,
        .ppCost = 24,
        .power = 170,
    },
    // Skull Splitter
    {
        .targetMode = 1,
        .flags = 0x83,
        .effect = 27,
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
        .targetMode = 1,
        .flags = 0x83,
        .range = 1,
        .power = 20,
    },
    // Poison Cloud
    {
        .targetMode = 1,
        .flags = 0x83,
        .effect = 18,
        .range = 1,
        .power = 55,
    },
    // Poison Death
    {
        .targetMode = 1,
        .flags = 0x83,
        .effect = 19,
        .range = 1,
        .power = 53,
    },
    // Mortal Danger
    {
        .targetMode = 1,
        .flags = 0x83,
        .effect = 27,
        .range = 1,
        .power = 28,
    },
    // Bad Omen
    {
        .targetMode = 1,
        .flags = 0x83,
        .effect = 26,
        .range = 1,
        .power = 43,
    },
    // Life Nourish
    {
        .targetMode = 1,
        .flags = 0x83,
        .effect = 31,
        .range = 1,
        .power = 25,
    },
    // Aqua Sock
    {
        .targetMode = 1,
        .flags = 0x83,
        .element = ELEMENT_MERCURY,
        .range = 1,
        .power = 42,
    },
    // Blizzard
    {
        .targetMode = 1,
        .flags = 0x83,
        .element = ELEMENT_MERCURY,
        .effect = 8,
        .range = 1,
        .power = 18,
    },
    // Frost Bite
    {
        .targetMode = 1,
        .flags = 0x83,
        .element = ELEMENT_MERCURY,
        .effect = 23,
        .range = 1,
        .power = 28,
    },
    // Drown
    {
        .targetMode = 1,
        .flags = 0x83,
        .element = ELEMENT_MERCURY,
        .effect = 27,
        .range = 1,
        .power = 47,
    },
    // Life Leech
    {
        .targetMode = 1,
        .flags = 0x83,
        .element = ELEMENT_MERCURY,
        .effect = 31,
        .range = 1,
        .power = 29,
    },
    // Psy Leech
    {
        .targetMode = 1,
        .flags = 0x83,
        .element = ELEMENT_MERCURY,
        .effect = 69,
        .range = 1,
        .power = 22,
    },
    // Broil
    {
        .targetMode = 1,
        .flags = 0x83,
        .element = ELEMENT_MARS,
        .range = 1,
        .power = 23,
    },
    // Meltdown
    {
        .targetMode = 1,
        .flags = 0x83,
        .element = ELEMENT_MARS,
        .effect = 12,
        .range = 1,
        .power = 47,
    },
    // Heat Mirage
    {
        .targetMode = 1,
        .flags = 0x83,
        .element = ELEMENT_MARS,
        .effect = 20,
        .range = 1,
        .power = 21,
    },
    // Barrage
    {
        .targetMode = 1,
        .flags = 0x83,
        .element = ELEMENT_MARS,
        .effect = 23,
        .range = 1,
        .power = 20,
    },
    // Demonfire
    {
        .targetMode = 1,
        .flags = 0x83,
        .element = ELEMENT_MARS,
        .effect = 26,
        .range = 1,
        .power = 60,
    },
    // Acid Bath
    {
        .targetMode = 1,
        .flags = 0x83,
        .element = ELEMENT_MARS,
        .effect = 8,
        .range = 1,
        .power = 34,
    },
    // Vorpal Slash
    {
        .targetMode = 1,
        .flags = 0x83,
        .element = ELEMENT_JUPITER,
        .range = 1,
        .power = 16,
    },
    // Stun Voltage
    {
        .targetMode = 1,
        .flags = 0x83,
        .element = ELEMENT_JUPITER,
        .effect = 23,
        .range = 1,
        .power = 12,
    },
    // Blinding Smog
    {
        .targetMode = 1,
        .flags = 0x83,
        .element = ELEMENT_JUPITER,
        .effect = 20,
        .range = 1,
        .power = 49,
    },
    // Murk
    {
        .targetMode = 1,
        .flags = 0x83,
        .element = ELEMENT_JUPITER,
        .effect = 24,
        .range = 1,
        .power = 10,
    },
    // Cyclone Slash
    {
        .targetMode = 1,
        .flags = 0x83,
        .element = ELEMENT_JUPITER,
        .effect = 16,
        .range = 1,
        .power = 38,
    },
    // Psyphon Seal
    {
        .targetMode = 1,
        .flags = 0x83,
        .element = ELEMENT_JUPITER,
        .effect = 25,
        .range = 1,
        .power = 14,
    },
    // Rapid Smash
    {
        .targetMode = 1,
        .flags = 0x84,
        .element = ELEMENT_JUPITER,
        .effect = 65,
        .range = 1,
        .power = 9,
    },
    // Sonic Smash
    {
        .targetMode = 1,
        .flags = 0x83,
        .element = ELEMENT_JUPITER,
        .effect = 68,
        .range = 1,
    },
    // Asura
    {
        .targetMode = 1,
        .flags = 0x83,
        .element = ELEMENT_JUPITER,
        .effect = 35,
        .range = 1,
        .power = 64,
    },
    // Titan Blade
    {
        .targetMode = 1,
        .flags = 0x83,
        .range = 1,
        .power = 70,
    },
    // Shining Star
    {
        .targetMode = 1,
        .flags = 0x83,
        .element = ELEMENT_JUPITER,
        .effect = 20,
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
        .targetMode = 2,
        .flags = 0xc1,
        .element = ELEMENT_NONE,
        .range = 1,
        .power = 50,
    },
    // Nut
    {
        .targetMode = 2,
        .flags = 0xc1,
        .element = ELEMENT_NONE,
        .range = 1,
        .power = 200,
    },
    // Vial
    {
        .targetMode = 2,
        .flags = 0xc1,
        .element = ELEMENT_NONE,
        .range = 1,
        .power = 500,
    },
    // Potion
    {
        .targetMode = 2,
        .flags = 0xc1,
        .element = ELEMENT_NONE,
        .range = 1,
        .power = 9999,
    },
    // Soothing Water
    {
        .targetMode = 2,
        .flags = 0xc1,
        .element = ELEMENT_NONE,
        .range = 1,
        .power = 9999,
    },
    // Psy Crystal
    {
        .targetMode = 2,
        .flags = 0xcb,
        .element = ELEMENT_NONE,
        .range = 1,
        .power = 9999,
    },
    // Antidote
    {
        .targetMode = 2,
        .flags = 0xc2,
        .element = ELEMENT_NONE,
        .effect = 3,
        .range = 1,
    },
    // Elixir
    {
        .targetMode = 2,
        .flags = 0x82,
        .element = ELEMENT_NONE,
        .effect = 4,
        .range = 1,
    },
    // Water of Life
    {
        .targetMode = 2,
        .flags = 0xc2,
        .element = ELEMENT_NONE,
        .effect = 5,
        .range = 1,
    },
    // Mist Potion
    {
        .targetMode = 2,
        .flags = 0xc1,
        .element = ELEMENT_NONE,
        .range = 255,
        .power = 300,
    },
    // Power Bread
    {
        .targetMode = 2,
        .flags = 0x49,
        .element = ELEMENT_NONE,
        .range = 1,
        .power = 8,
    },
    // Cookie
    {
        .targetMode = 2,
        .flags = 0x49,
        .element = ELEMENT_NONE,
        .range = 1,
        .power = 6,
    },
    // Apple
    {
        .targetMode = 2,
        .flags = 0x49,
        .element = ELEMENT_NONE,
        .range = 1,
        .power = 5,
    },
    // Hard Nut
    {
        .targetMode = 2,
        .flags = 0x49,
        .element = ELEMENT_NONE,
        .range = 1,
        .power = 3,
    },
    // Mint
    {
        .targetMode = 2,
        .flags = 0x49,
        .element = ELEMENT_NONE,
        .range = 1,
        .power = 5,
    },
    // Lucky Pepper
    {
        .targetMode = 2,
        .flags = 0x49,
        .element = ELEMENT_NONE,
        .range = 1,
        .power = 2,
    },
    // ?
    {0},
    // Smoke Bomb
    {
        .targetMode = 1,
        .flags = 0x82,
        .element = ELEMENT_JUPITER,
        .effect = 20,
        .range = 1,
    },
    // Sleep Bomb
    {
        .targetMode = 1,
        .flags = 0x82,
        .element = ELEMENT_JUPITER,
        .effect = 24,
        .range = 1,
    },
    // Adept Ring
    {
        .targetMode = 4,
        .flags = 0xcb,
        .element = ELEMENT_NONE,
        .range = 1,
        .power = 7,
    },
    // Corn
    {
        .targetMode = 2,
        .flags = 0xc1,
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
        .targetMode = 2,
        .flags = 0x81,
        .element = ELEMENT_NONE,
        .effect = 5,
        .range = 255,
        .power = 9999,
    },
    // Djinn Counter
    {
        .targetMode = 1,
        .flags = 0x83,
        .element = ELEMENT_MARS,
        .range = 1,
        .power = 20,
    },
    // Flint
    {
        .targetMode = 1,
        .flags = 0x84,
        .range = 1,
        .power = 16,
    },
    // Granite
    {
        .targetMode = 2,
        .flags = 0x87,
        .effect = 46,
        .range = 255,
    },
    // Quartz
    {
        .targetMode = 2,
        .flags = 0x82,
        .effect = 56,
        .range = 1,
    },
    // Vine
    {
        .targetMode = 1,
        .flags = 0x82,
        .effect = 58,
        .range = 255,
    },
    // Sap
    {
        .targetMode = 1,
        .flags = 0x83,
        .effect = 60,
        .range = 1,
        .power = 50,
    },
    // Ground
    {
        .targetMode = 1,
        .flags = 0x82,
        .effect = 53,
        .range = 1,
    },
    // Bane
    {
        .targetMode = 1,
        .flags = 0x83,
        .effect = 19,
        .range = 1,
        .power = 60,
    },
    // Echo
    {
        .targetMode = 1,
        .flags = 0x84,
        .range = 1,
        .power = 16,
    },
    // Iron
    {
        .targetMode = 2,
        .flags = 0x82,
        .effect = 10,
        .range = 255,
    },
    // Steel
    {
        .targetMode = 1,
        .flags = 0x84,
        .effect = 60,
        .range = 1,
        .power = 13,
    },
    // Mud
    {
        .targetMode = 1,
        .flags = 0x82,
        .effect = 58,
        .range = 255,
    },
    // Flower
    {
        .targetMode = 2,
        .flags = 0x81,
        .effect = 62,
        .range = 255,
    },
    // Meld
    {
        .targetMode = 1,
        .flags = 0x83,
        .range = 1,
    },
    // Petra
    {
        .targetMode = 1,
        .flags = 0x87,
        .effect = 53,
        .range = 1,
    },
    // Salt
    {
        .targetMode = 2,
        .flags = 0x82,
        .effect = 64,
        .range = 255,
    },
    // Geode
    {
        .targetMode = 1,
        .flags = 0x84,
        .range = 1,
        .power = 19,
    },
    // Mold
    {
        .targetMode = 1,
        .flags = 0x83,
        .range = 1,
        .power = 80,
    },
    // Crystal
    {
        .targetMode = 2,
        .flags = 0x81,
        .effect = 70,
        .range = 255,
    },
    // Earth18
    {
        .targetMode = 1,
        .flags = 0x85,
        .range = 1,
    },
    // Earth19
    {
        .targetMode = 1,
        .flags = 0x85,
        .range = 1,
    },
    // Fizz
    {
        .targetMode = 2,
        .flags = 0x81,
        .element = ELEMENT_MERCURY,
        .effect = 61,
        .range = 1,
    },
    // Sleet
    {
        .targetMode = 1,
        .flags = 0x83,
        .element = ELEMENT_MERCURY,
        .effect = 8,
        .range = 1,
        .power = 30,
    },
    // Mist
    {
        .targetMode = 1,
        .flags = 0x84,
        .element = ELEMENT_MERCURY,
        .effect = 24,
        .range = 1,
        .power = 13,
    },
    // Spritz
    {
        .targetMode = 2,
        .flags = 0x81,
        .element = ELEMENT_MERCURY,
        .effect = 62,
        .range = 255,
    },
    // Hail
    {
        .targetMode = 1,
        .flags = 0x83,
        .element = ELEMENT_MERCURY,
        .effect = 12,
        .range = 1,
        .power = 40,
    },
    // Tonic
    {
        .targetMode = 2,
        .flags = 0x82,
        .element = ELEMENT_MERCURY,
        .effect = 64,
        .range = 255,
    },
    // Dew
    {
        .targetMode = 2,
        .flags = 0x82,
        .element = ELEMENT_MERCURY,
        .effect = 57,
        .range = 1,
    },
    // Fog
    {
        .targetMode = 1,
        .flags = 0x83,
        .element = ELEMENT_MERCURY,
        .effect = 20,
        .range = 1,
        .power = 30,
    },
    // Sour
    {
        .targetMode = 1,
        .flags = 0x84,
        .element = ELEMENT_MERCURY,
        .effect = 16,
        .range = 1,
        .power = 15,
    },
    // Spring
    {
        .targetMode = 2,
        .flags = 0x81,
        .element = ELEMENT_MERCURY,
        .effect = 71,
        .range = 1,
    },
    // Shade
    {
        .targetMode = 2,
        .flags = 0x87,
        .element = ELEMENT_MERCURY,
        .effect = 72,
        .range = 255,
    },
    // Chill
    {
        .targetMode = 1,
        .flags = 0x83,
        .element = ELEMENT_MERCURY,
        .effect = 12,
        .range = 1,
        .power = 40,
    },
    // Steam
    {
        .targetMode = 2,
        .flags = 0x82,
        .element = ELEMENT_MERCURY,
        .effect = 14,
        .range = 255,
    },
    // Rime
    {
        .targetMode = 1,
        .flags = 0x82,
        .element = ELEMENT_MERCURY,
        .effect = 67,
        .range = 1,
    },
    // Gel
    {
        .targetMode = 1,
        .flags = 0x84,
        .element = ELEMENT_MERCURY,
        .effect = 8,
        .range = 1,
        .power = 15,
    },
    // Eddy
    {
        .targetMode = 2,
        .flags = 0x87,
        .element = ELEMENT_MERCURY,
        .range = 255,
    },
    // Balm
    {
        .targetMode = 2,
        .flags = 0x82,
        .element = ELEMENT_MERCURY,
        .effect = 73,
        .range = 255,
    },
    // Serac
    {
        .targetMode = 1,
        .flags = 0x83,
        .element = ELEMENT_MERCURY,
        .effect = 27,
        .range = 1,
        .power = 70,
    },
    // Water18
    {
        .targetMode = 1,
        .flags = 0x85,
        .range = 1,
    },
    // Water19
    {
        .targetMode = 1,
        .flags = 0x85,
        .range = 1,
    },
    // Forge
    {
        .targetMode = 2,
        .flags = 0x82,
        .element = ELEMENT_MARS,
        .effect = 6,
        .range = 255,
    },
    // Fever
    {
        .targetMode = 1,
        .flags = 0x83,
        .element = ELEMENT_MARS,
        .effect = 20,
        .range = 1,
        .power = 30,
    },
    // Corona
    {
        .targetMode = 2,
        .flags = 0x82,
        .element = ELEMENT_MARS,
        .effect = 10,
        .range = 255,
    },
    // Scorch
    {
        .targetMode = 1,
        .flags = 0x83,
        .element = ELEMENT_MARS,
        .effect = 23,
        .range = 1,
        .power = 50,
    },
    // Ember
    {
        .targetMode = 2,
        .flags = 0x8b,
        .element = ELEMENT_MARS,
        .effect = 63,
        .range = 255,
    },
    // Flash
    {
        .targetMode = 2,
        .flags = 0x87,
        .element = ELEMENT_MARS,
        .effect = 47,
        .range = 255,
    },
    // Torch
    {
        .targetMode = 1,
        .flags = 0x83,
        .element = ELEMENT_MARS,
        .effect = 35,
        .range = 1,
        .power = 30,
    },
    // Cannon
    {
        .targetMode = 1,
        .flags = 0x83,
        .element = ELEMENT_MARS,
        .range = 1,
        .power = 40,
    },
    // Spark
    {
        .targetMode = 2,
        .flags = 0x82,
        .element = ELEMENT_MARS,
        .effect = 73,
        .range = 1,
    },
    // Kindle
    {
        .targetMode = 2,
        .flags = 0x82,
        .element = ELEMENT_MARS,
        .effect = 6,
        .range = 255,
    },
    // Char
    {
        .targetMode = 1,
        .flags = 0x83,
        .element = ELEMENT_MARS,
        .effect = 23,
        .range = 1,
        .power = 50,
    },
    // Coal
    {
        .targetMode = 2,
        .flags = 0x82,
        .element = ELEMENT_MARS,
        .effect = 59,
        .range = 255,
    },
    // Reflux
    {
        .targetMode = 2,
        .flags = 0x87,
        .element = ELEMENT_MARS,
        .effect = 74,
        .range = 1,
    },
    // Core
    {
        .targetMode = 1,
        .flags = 0x83,
        .element = ELEMENT_MARS,
        .effect = 35,
        .range = 1,
        .power = 40,
    },
    // Tinder
    {
        .targetMode = 2,
        .flags = 0x82,
        .element = ELEMENT_MARS,
        .effect = 5,
        .range = 1,
    },
    // Shine
    {
        .targetMode = 1,
        .flags = 0x83,
        .element = ELEMENT_MARS,
        .effect = 75,
        .range = 1,
        .power = 60,
    },
    // Fury
    {
        .targetMode = 1,
        .flags = 0x83,
        .element = ELEMENT_MARS,
        .effect = 26,
        .range = 1,
        .power = 70,
    },
    // Fugue
    {
        .targetMode = 1,
        .flags = 0x8a,
        .element = ELEMENT_MARS,
        .range = 255,
        .power = 18,
    },
    // Fire18
    {
        .targetMode = 1,
        .flags = 0x85,
        .range = 1,
    },
    // Fire19
    {
        .targetMode = 1,
        .flags = 0x85,
        .range = 1,
    },
    // Gust
    {
        .targetMode = 1,
        .flags = 0x83,
        .element = ELEMENT_JUPITER,
        .effect = 65,
        .range = 1,
        .power = 10,
    },
    // Breeze
    {
        .targetMode = 2,
        .flags = 0x82,
        .element = ELEMENT_JUPITER,
        .effect = 14,
        .range = 255,
    },
    // Zephyr
    {
        .targetMode = 2,
        .flags = 0x82,
        .element = ELEMENT_JUPITER,
        .effect = 59,
        .range = 255,
    },
    // Smog
    {
        .targetMode = 1,
        .flags = 0x83,
        .element = ELEMENT_JUPITER,
        .effect = 20,
        .range = 1,
        .power = 60,
    },
    // Kite
    {
        .targetMode = 2,
        .flags = 0x87,
        .element = ELEMENT_JUPITER,
        .effect = 66,
        .range = 1,
    },
    // Squall
    {
        .targetMode = 1,
        .flags = 0x84,
        .element = ELEMENT_JUPITER,
        .effect = 23,
        .range = 1,
        .power = 14,
    },
    // Luff
    {
        .targetMode = 1,
        .flags = 0x82,
        .element = ELEMENT_JUPITER,
        .effect = 67,
        .range = 1,
    },
    // Breath
    {
        .targetMode = 2,
        .flags = 0x81,
        .element = ELEMENT_JUPITER,
        .effect = 76,
        .range = 1,
    },
    // Blitz
    {
        .targetMode = 1,
        .flags = 0x83,
        .element = ELEMENT_JUPITER,
        .effect = 23,
        .range = 1,
        .power = 30,
    },
    // Ether
    {
        .targetMode = 2,
        .flags = 0x8b,
        .element = ELEMENT_JUPITER,
        .effect = 78,
        .range = 1,
    },
    // Waft
    {
        .targetMode = 1,
        .flags = 0x83,
        .element = ELEMENT_JUPITER,
        .effect = 24,
        .range = 1,
        .power = 40,
    },
    // Haze
    {
        .targetMode = 2,
        .flags = 0x87,
        .element = ELEMENT_JUPITER,
        .effect = 79,
        .range = 1,
    },
    // Wheeze
    {
        .targetMode = 1,
        .flags = 0x83,
        .element = ELEMENT_JUPITER,
        .effect = 19,
        .range = 1,
        .power = 50,
    },
    // Aroma
    {
        .targetMode = 2,
        .flags = 0x8b,
        .element = ELEMENT_JUPITER,
        .effect = 77,
        .range = 255,
    },
    // Whorl
    {
        .targetMode = 1,
        .flags = 0x83,
        .element = ELEMENT_JUPITER,
        .effect = 27,
        .range = 1,
        .power = 40,
    },
    // Gasp
    {
        .targetMode = 1,
        .flags = 0x82,
        .element = ELEMENT_JUPITER,
        .effect = 80,
        .range = 255,
    },
    // Lull
    {
        .targetMode = 4,
        .flags = 0x87,
        .element = ELEMENT_JUPITER,
        .effect = 81,
        .range = 1,
    },
    // Gale
    {
        .targetMode = 1,
        .flags = 0x83,
        .element = ELEMENT_JUPITER,
        .effect = 82,
        .range = 1,
        .power = 60,
    },
    // Wind18
    {
        .targetMode = 1,
        .flags = 0x85,
        .range = 1,
    },
    // Wind19
    {
        .targetMode = 1,
        .flags = 0x85,
        .range = 1,
    },
    // Venus
    {
        .targetMode = 1,
        .flags = 0x88,
        .icon = 140,
        .range = 6,
        .power = 30,
    },
    // Ramses
    {
        .targetMode = 1,
        .flags = 0x88,
        .icon = 141,
        .range = 6,
        .power = 60,
    },
    // Cybele
    {
        .targetMode = 1,
        .flags = 0x88,
        .icon = 142,
        .range = 6,
        .power = 120,
    },
    // Judgment
    {
        .targetMode = 1,
        .flags = 0x88,
        .icon = 143,
        .range = 6,
        .power = 240,
    },
    // Zagan
    {
        .targetMode = 1,
        .flags = 0x88,
        .effect = 12,
        .icon = 186,
        .range = 6,
        .power = 50,
    },
    // Haures
    {
        .targetMode = 1,
        .flags = 0x88,
        .effect = 19,
        .icon = 187,
        .range = 6,
        .power = 270,
    },
    // Charon
    {
        .targetMode = 1,
        .flags = 0x88,
        .effect = 27,
        .icon = 188,
        .range = 6,
        .power = 500,
    },
    // ?
    {0},
    // Mercury
    {
        .targetMode = 1,
        .flags = 0x88,
        .element = ELEMENT_MERCURY,
        .icon = 144,
        .range = 6,
        .power = 30,
    },
    // Nereid
    {
        .targetMode = 1,
        .flags = 0x88,
        .element = ELEMENT_MERCURY,
        .icon = 145,
        .range = 6,
        .power = 60,
    },
    // Neptune
    {
        .targetMode = 1,
        .flags = 0x88,
        .element = ELEMENT_MERCURY,
        .icon = 146,
        .range = 6,
        .power = 120,
    },
    // Boreas
    {
        .targetMode = 1,
        .flags = 0x88,
        .element = ELEMENT_MERCURY,
        .icon = 147,
        .range = 6,
        .power = 240,
    },
    // Moloch
    {
        .targetMode = 1,
        .flags = 0x88,
        .element = ELEMENT_MERCURY,
        .effect = 58,
        .icon = 189,
        .range = 6,
        .power = 100,
    },
    // Coatlicue
    {
        .targetMode = 2,
        .flags = 0x81,
        .element = ELEMENT_MERCURY,
        .effect = 29,
        .icon = 190,
        .range = 255,
        .power = 60,
    },
    // Azul
    {
        .targetMode = 1,
        .flags = 0x88,
        .element = ELEMENT_MERCURY,
        .effect = 23,
        .icon = 191,
        .range = 6,
        .power = 330,
    },
    // ?
    {0},
    // Mars
    {
        .targetMode = 1,
        .flags = 0x88,
        .element = ELEMENT_MARS,
        .icon = 148,
        .range = 6,
        .power = 30,
    },
    // Kirin
    {
        .targetMode = 1,
        .flags = 0x88,
        .element = ELEMENT_MARS,
        .icon = 149,
        .range = 6,
        .power = 60,
    },
    // Tiamat
    {
        .targetMode = 1,
        .flags = 0x88,
        .element = ELEMENT_MARS,
        .icon = 150,
        .range = 6,
        .power = 120,
    },
    // Meteor
    {
        .targetMode = 1,
        .flags = 0x88,
        .element = ELEMENT_MARS,
        .icon = 151,
        .range = 6,
        .power = 240,
    },
    // Megaera
    {
        .targetMode = 1,
        .flags = 0x88,
        .element = ELEMENT_MARS,
        .icon = 192,
        .range = 6,
        .power = 40,
    },
    // Ulysses
    {
        .targetMode = 1,
        .flags = 0x88,
        .element = ELEMENT_MARS,
        .effect = 83,
        .icon = 193,
        .range = 6,
        .power = 160,
    },
    // Daedalus
    {
        .targetMode = 1,
        .flags = 0x88,
        .element = ELEMENT_MARS,
        .icon = 194,
        .range = 6,
        .power = 100,
    },
    // Daedalus
    {
        .targetMode = 1,
        .flags = 0x88,
        .element = ELEMENT_MARS,
        .icon = 194,
        .range = 6,
        .power = 250,
    },
    // Iris
    {
        .targetMode = 1,
        .flags = 0x88,
        .element = ELEMENT_MARS,
        .icon = 195,
        .range = 6,
        .power = 800,
    },
    // ?
    {0},
    // Jupiter
    {
        .targetMode = 1,
        .flags = 0x88,
        .element = ELEMENT_JUPITER,
        .icon = 152,
        .range = 6,
        .power = 30,
    },
    // Atalanta
    {
        .targetMode = 1,
        .flags = 0x88,
        .element = ELEMENT_JUPITER,
        .icon = 153,
        .range = 6,
        .power = 60,
    },
    // Procne
    {
        .targetMode = 1,
        .flags = 0x88,
        .element = ELEMENT_JUPITER,
        .icon = 154,
        .range = 6,
        .power = 120,
    },
    // Thor
    {
        .targetMode = 1,
        .flags = 0x88,
        .element = ELEMENT_JUPITER,
        .icon = 155,
        .range = 6,
        .power = 240,
    },
    // Flora
    {
        .targetMode = 1,
        .flags = 0x88,
        .element = ELEMENT_JUPITER,
        .effect = 24,
        .icon = 196,
        .range = 6,
        .power = 80,
    },
    // Eclipse
    {
        .targetMode = 1,
        .flags = 0x88,
        .element = ELEMENT_JUPITER,
        .effect = 8,
        .icon = 197,
        .range = 6,
        .power = 300,
    },
    // Catastrophe
    {
        .targetMode = 1,
        .flags = 0x88,
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
        .targetMode = 1,
        .flags = 0x86,
        .element = ELEMENT_MARS,
        .range = 2,
        .power = 60,
    },
    // Fire Breath
    {
        .targetMode = 1,
        .flags = 0x86,
        .element = ELEMENT_MARS,
        .range = 2,
        .power = 90,
    },
    // Fire Breath
    {
        .targetMode = 1,
        .flags = 0x86,
        .element = ELEMENT_MARS,
        .range = 2,
        .power = 130,
    },
    // Water Breath
    {
        .targetMode = 1,
        .flags = 0x86,
        .element = ELEMENT_MERCURY,
        .range = 2,
        .power = 25,
    },
    // Water Breath
    {
        .targetMode = 1,
        .flags = 0x86,
        .element = ELEMENT_MERCURY,
        .range = 2,
        .power = 50,
    },
    // Water Breath
    {
        .targetMode = 1,
        .flags = 0x86,
        .element = ELEMENT_MERCURY,
        .range = 2,
        .power = 100,
    },
    // Ice Breath
    {
        .targetMode = 1,
        .flags = 0x86,
        .element = ELEMENT_MERCURY,
        .range = 2,
        .power = 50,
    },
    // Ice Breath
    {
        .targetMode = 1,
        .flags = 0x86,
        .element = ELEMENT_MERCURY,
        .range = 2,
        .power = 90,
    },
    // Ice Breath
    {
        .targetMode = 1,
        .flags = 0x86,
        .element = ELEMENT_MERCURY,
        .range = 2,
        .power = 180,
    },
    // Dark Breath
    {
        .targetMode = 1,
        .flags = 0x86,
        .effect = 20,
        .range = 2,
        .power = 70,
    },
    // Dark Breath
    {
        .targetMode = 1,
        .flags = 0x86,
        .effect = 20,
        .range = 2,
        .power = 110,
    },
    // Acid Breath
    {
        .targetMode = 1,
        .flags = 0x86,
        .element = ELEMENT_MARS,
        .effect = 13,
        .range = 2,
        .power = 170,
    },
    // Acid Breath
    {
        .targetMode = 1,
        .flags = 0x86,
        .element = ELEMENT_MARS,
        .effect = 13,
        .range = 2,
        .power = 190,
    },
    // Storm Breath
    {
        .targetMode = 1,
        .flags = 0x86,
        .element = ELEMENT_JUPITER,
        .effect = 23,
        .range = 2,
        .power = 190,
    },
    // Storm Breath
    {
        .targetMode = 1,
        .flags = 0x86,
        .element = ELEMENT_JUPITER,
        .effect = 23,
        .range = 2,
        .power = 220,
    },
    // Sonic Wave
    {
        .targetMode = 1,
        .flags = 0x86,
        .element = ELEMENT_JUPITER,
        .range = 1,
        .power = 5,
    },
    // Sonic Wave
    {
        .targetMode = 1,
        .flags = 0x86,
        .element = ELEMENT_JUPITER,
        .range = 1,
        .power = 45,
    },
    // Shriek
    {
        .targetMode = 1,
        .flags = 0x86,
        .element = ELEMENT_JUPITER,
        .range = 1,
        .power = 30,
    },
    // Banshee Howl
    {
        .targetMode = 1,
        .flags = 0x86,
        .element = ELEMENT_JUPITER,
        .effect = 23,
        .range = 1,
        .power = 70,
    },
    // Crazy Voice
    {
        .targetMode = 1,
        .flags = 0x8a,
        .element = ELEMENT_JUPITER,
        .effect = 52,
        .range = 1,
        .power = 10,
    },
    // War Cry
    {
        .targetMode = 1,
        .flags = 0x86,
        .element = ELEMENT_JUPITER,
        .effect = 53,
        .range = 1,
        .power = 45,
    },
    // Wicked Howl
    {
        .targetMode = 1,
        .flags = 0x86,
        .element = ELEMENT_JUPITER,
        .effect = 53,
        .range = 1,
        .power = 60,
    },
    // Wing Beat
    {
        .targetMode = 1,
        .flags = 0x86,
        .element = ELEMENT_JUPITER,
        .range = 1,
        .power = 35,
    },
    // Wing Flutter
    {
        .targetMode = 1,
        .flags = 0x86,
        .element = ELEMENT_JUPITER,
        .range = 2,
        .power = 70,
    },
    // Wing Stroke
    {
        .targetMode = 1,
        .flags = 0x86,
        .element = ELEMENT_JUPITER,
        .range = 2,
        .power = 140,
    },
    // Evil Blessing
    {
        .targetMode = 1,
        .flags = 0x86,
        .element = ELEMENT_MARS,
        .effect = 26,
        .range = 2,
        .power = 80,
    },
    // Deadly Gas
    {
        .targetMode = 1,
        .flags = 0x86,
        .effect = 19,
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
        .targetMode = 1,
        .flags = 0x85,
        .range = 1,
        .power = 9,
    },
    // Bone Chiller
    {
        .targetMode = 1,
        .flags = 0x85,
        .element = ELEMENT_MERCURY,
        .range = 1,
        .power = 7,
    },
    // Slice
    {
        .targetMode = 1,
        .flags = 0x84,
        .element = ELEMENT_JUPITER,
        .effect = 65,
        .range = 1,
        .power = 9,
    },
    // Bone Charge
    {
        .targetMode = 1,
        .flags = 0x84,
        .range = 1,
        .power = 17,
    },
    // Mystic Flame
    {
        .targetMode = 1,
        .flags = 0x85,
        .element = ELEMENT_MARS,
        .range = 1,
        .power = 16,
    },
    // Numbing Sting
    {
        .targetMode = 1,
        .flags = 0x84,
        .element = ELEMENT_JUPITER,
        .effect = 23,
        .range = 1,
        .power = 8,
    },
    // Brute Force
    {
        .targetMode = 1,
        .flags = 0x83,
        .range = 1,
        .power = 22,
    },
    // Sticky Goo
    {
        .targetMode = 1,
        .flags = 0x85,
        .element = ELEMENT_MERCURY,
        .effect = 13,
        .range = 1,
        .power = 13,
    },
    // Cannibal Fang
    {
        .targetMode = 1,
        .flags = 0x82,
        .effect = 31,
        .range = 1,
        .power = 24,
    },
    // Bear Claw
    {
        .targetMode = 1,
        .flags = 0x84,
        .element = ELEMENT_NONE,
        .range = 1,
        .power = 16,
    },
    // Poisonous Bite
    {
        .targetMode = 1,
        .flags = 0x83,
        .effect = 18,
        .range = 1,
    },
    // Flying Attack
    {
        .targetMode = 1,
        .flags = 0x83,
        .element = ELEMENT_JUPITER,
        .effect = 42,
        .range = 1,
        .power = 8,
    },
    // Undead Sword
    {
        .targetMode = 1,
        .flags = 0x83,
        .range = 1,
        .power = 15,
    },
    // Ransack
    {
        .targetMode = 1,
        .flags = 0x83,
        .range = 1,
        .power = 35,
    },
    // Sticky Poison
    {
        .targetMode = 1,
        .flags = 0x85,
        .element = ELEMENT_MERCURY,
        .effect = 18,
        .range = 1,
        .power = 16,
    },
    // Poison Fang
    {
        .targetMode = 1,
        .flags = 0x83,
        .effect = 18,
        .range = 1,
    },
    // Electric Bite
    {
        .targetMode = 1,
        .flags = 0x83,
        .element = ELEMENT_JUPITER,
        .effect = 23,
        .range = 1,
    },
    // Poison Tail
    {
        .targetMode = 1,
        .flags = 0x83,
        .effect = 19,
        .range = 1,
        .power = 30,
    },
    // Onslaught
    {
        .targetMode = 1,
        .flags = 0x83,
        .range = 1,
        .power = 24,
    },
    // Vampiric Fang
    {
        .targetMode = 1,
        .flags = 0x82,
        .effect = 31,
        .range = 1,
        .power = 35,
    },
    // Bacteria Rush
    {
        .targetMode = 1,
        .flags = 0x83,
        .effect = 18,
        .range = 1,
        .power = 16,
    },
    // Swift Strike
    {
        .targetMode = 1,
        .flags = 0x83,
        .element = ELEMENT_JUPITER,
        .effect = 44,
        .range = 1,
        .power = 15,
    },
    // Rotten Blood
    {
        .targetMode = 1,
        .flags = 0x85,
        .element = ELEMENT_MARS,
        .effect = 12,
        .range = 1,
        .power = 44,
    },
    // Forcible Arm
    {
        .targetMode = 1,
        .flags = 0x83,
        .range = 1,
        .power = 40,
    },
    // Double Fang
    {
        .targetMode = 1,
        .flags = 0x83,
        .effect = 42,
        .range = 1,
        .power = 35,
    },
    // Mortal Blow
    {
        .targetMode = 1,
        .flags = 0x83,
        .effect = 55,
        .range = 1,
        .power = 35,
    },
    // Freebite Rush
    {
        .targetMode = 1,
        .flags = 0x83,
        .effect = 9,
        .range = 1,
        .power = 40,
    },
    // Twin Beaks
    {
        .targetMode = 1,
        .flags = 0x83,
        .element = ELEMENT_JUPITER,
        .effect = 42,
        .range = 1,
        .power = 30,
    },
    // Rabid Fang
    {
        .targetMode = 1,
        .flags = 0x83,
        .effect = 19,
        .range = 1,
        .power = 36,
    },
    // Acid Bite
    {
        .targetMode = 1,
        .flags = 0x83,
        .element = ELEMENT_MARS,
        .effect = 13,
        .range = 1,
        .power = 36,
    },
    // Dynamite
    {
        .targetMode = 1,
        .flags = 0x85,
        .effect = 55,
        .range = 2,
        .power = 200,
    },
    // Headbutt
    {
        .targetMode = 1,
        .flags = 0x83,
        .range = 1,
        .power = 60,
    },
    // Poison Ink
    {
        .targetMode = 1,
        .flags = 0x83,
        .effect = 19,
        .range = 1,
    },
    // Truncheon Fist
    {
        .targetMode = 1,
        .flags = 0x83,
        .effect = 34,
        .range = 1,
        .power = 60,
    },
    // Counterstrike
    {
        .targetMode = 1,
        .flags = 0x83,
        .range = 1,
        .power = 50,
    },
    // Mad Dash
    {
        .targetMode = 1,
        .flags = 0x83,
        .range = 1,
        .power = 70,
    },
    // Soothing Star
    {
        .targetMode = 2,
        .flags = 0x81,
        .range = 2,
        .power = 10,
    },
    // Spider Web
    {
        .targetMode = 1,
        .flags = 0x82,
        .effect = 12,
        .range = 1,
    },
    // Heartrender
    {
        .targetMode = 1,
        .flags = 0x82,
        .element = ELEMENT_NONE,
        .effect = 8,
        .range = 1,
    },
    // Mad Spatter
    {
        .targetMode = 1,
        .flags = 0x82,
        .effect = 20,
        .range = 2,
    },
    // Spasm
    {
        .targetMode = 4,
        .flags = 0x81,
        .range = 1,
        .power = 40,
    },
    // Sleep Star
    {
        .targetMode = 1,
        .flags = 0x82,
        .element = ELEMENT_JUPITER,
        .effect = 24,
        .range = 2,
    },
    // Decompose
    {
        .targetMode = 1,
        .flags = 0x82,
        .effect = 9,
        .range = 2,
    },
    // Haunting
    {
        .targetMode = 1,
        .flags = 0x82,
        .effect = 26,
        .range = 1,
    },
    // Worms
    {
        .targetMode = 4,
        .flags = 0x81,
        .range = 1,
        .power = 50,
    },
    // Berserk
    {
        .targetMode = 4,
        .flags = 0x82,
        .effect = 6,
        .range = 1,
    },
    // Lucid Prophecy
    {
        .targetMode = 1,
        .flags = 0x82,
        .effect = 28,
        .range = 1,
    },
    // Recovery
    {
        .targetMode = 4,
        .flags = 0x81,
        .element = ELEMENT_NONE,
        .range = 1,
        .power = 150,
    },
    // Flee
    {
        .flags = 0x89,
        .element = ELEMENT_NONE,
        .effect = 49,
        .range = 1,
    },
    // Contain
    {
        .flags = 0x89,
        .element = ELEMENT_NONE,
        .effect = 45,
        .range = 1,
    },
    // Threaten
    {
        .flags = 0x89,
        .element = ELEMENT_NONE,
        .effect = 45,
        .range = 1,
    },
    // Tremble
    {
        .flags = 0x89,
        .element = ELEMENT_NONE,
        .effect = 45,
        .range = 1,
    },
    // Fortify
    {
        .targetMode = 4,
        .flags = 0x89,
        .element = ELEMENT_NONE,
        .effect = 46,
        .range = 1,
    },
    // Speed Surge
    {
        .flags = 0x89,
        .element = ELEMENT_NONE,
        .effect = 50,
        .range = 1,
    },
    // Ally Search
    {
        .flags = 0x89,
        .element = ELEMENT_NONE,
        .effect = 50,
        .range = 1,
    },
    // Sidestep
    {
        .targetMode = 4,
        .flags = 0x89,
        .element = ELEMENT_NONE,
        .effect = 48,
        .range = 1,
    },
    // Total Defense
    {
        .targetMode = 4,
        .flags = 0x89,
        .element = ELEMENT_NONE,
        .effect = 47,
        .range = 1,
    },
    // Stand Ready
    {
        .targetMode = 4,
        .flags = 0x89,
        .element = ELEMENT_NONE,
        .effect = 46,
        .range = 1,
    },
    // Battle Cry
    {
        .flags = 0x89,
        .element = ELEMENT_NONE,
        .effect = 51,
        .range = 1,
    },
    // Can't Use
    {
        .flags = 0x85,
        .range = 1,
    },
    // Poison Beat
    {
        .targetMode = 1,
        .flags = 0x83,
        .element = ELEMENT_MERCURY,
        .effect = 18,
        .range = 1,
        .power = 20,
    },
    // Spinning Beat
    {
        .targetMode = 1,
        .flags = 0x83,
        .element = ELEMENT_MERCURY,
        .effect = 35,
        .range = 1,
        .power = 40,
    },
    // Heat Flash
    {
        .targetMode = 1,
        .flags = 0x83,
        .element = ELEMENT_MARS,
        .effect = 20,
        .range = 1,
        .power = 20,
    },
    // Death Scythe
    {
        .targetMode = 1,
        .flags = 0x83,
        .effect = 27,
        .range = 1,
        .power = 35,
    },
    // Outer Space
    {
        .targetMode = 1,
        .flags = 0x88,
        .element = ELEMENT_MARS,
        .range = 6,
        .power = 200,
    },
    // Dragon Driver
    {
        .targetMode = 1,
        .flags = 0x88,
        .element = ELEMENT_MARS,
        .range = 6,
        .power = 120,
    },
    // Drain Fang
    {
        .targetMode = 1,
        .flags = 0x82,
        .effect = 31,
        .range = 1,
        .power = 80,
    },
    // Severe Blow
    {
        .targetMode = 1,
        .flags = 0x84,
        .effect = 34,
        .range = 1,
        .power = 12,
    },
    // Thrash
    {
        .targetMode = 1,
        .flags = 0x83,
        .element = ELEMENT_MERCURY,
        .effect = 34,
        .range = 1,
        .power = 30,
    },
    // ?
    {0},
    // Stone Justice
    {
        .targetMode = 1,
        .flags = 0x83,
        .effect = 58,
        .range = 1,
        .power = 11,
    },
    // Sarcophagus
    {
        .targetMode = 1,
        .flags = 0x83,
        .effect = 26,
        .range = 1,
        .power = 30,
    },
    // Evil Eye
    {
        .targetMode = 1,
        .flags = 0x83,
        .effect = 20,
        .range = 1,
        .power = 38,
    },
    // Vein Tap
    {
        .targetMode = 1,
        .flags = 0x83,
        .effect = 31,
        .range = 1,
        .power = 43,
    },
    // Heavy Divide
    {
        .targetMode = 1,
        .flags = 0x83,
        .effect = 12,
        .range = 1,
        .power = 60,
    },
    // Hammersphere
    {
        .targetMode = 1,
        .flags = 0x83,
        .effect = 35,
        .range = 1,
        .power = 58,
    },
    // Mother Earth
    {
        .targetMode = 1,
        .flags = 0x83,
        .effect = 24,
        .range = 1,
        .power = 59,
    },
    // Wyrd Curse
    {
        .targetMode = 1,
        .flags = 0x83,
        .effect = 26,
        .range = 1,
        .power = 41,
    },
    // Heartbreak
    {
        .targetMode = 1,
        .flags = 0x83,
        .effect = 27,
        .range = 1,
        .power = 42,
    },
    // Vengeance
    {
        .targetMode = 1,
        .flags = 0x83,
        .effect = 89,
        .range = 1,
        .power = 71,
    },
    // Acheron's Grief
    {
        .targetMode = 1,
        .flags = 0x83,
        .effect = 19,
        .range = 1,
        .power = 76,
    },
    // Megiddo
    {
        .targetMode = 1,
        .flags = 0x84,
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
        .targetMode = 1,
        .flags = 0x83,
        .element = ELEMENT_MERCURY,
        .effect = 8,
        .range = 1,
        .power = 21,
    },
    // Dreamtide
    {
        .targetMode = 1,
        .flags = 0x83,
        .element = ELEMENT_MERCURY,
        .effect = 24,
        .range = 1,
        .power = 12,
    },
    // Stun Cloud
    {
        .targetMode = 1,
        .flags = 0x83,
        .element = ELEMENT_MERCURY,
        .effect = 23,
        .range = 1,
        .power = 31,
    },
    // Searing Fog
    {
        .targetMode = 1,
        .flags = 0x83,
        .element = ELEMENT_MERCURY,
        .effect = 20,
        .range = 1,
        .power = 36,
    },
    // Ice Crush
    {
        .targetMode = 1,
        .flags = 0x83,
        .element = ELEMENT_MERCURY,
        .effect = 12,
        .range = 1,
        .power = 34,
    },
    // Flash Force
    {
        .targetMode = 1,
        .flags = 0x83,
        .element = ELEMENT_MERCURY,
        .effect = 25,
        .range = 1,
        .power = 44,
    },
    // Sargasso
    {
        .targetMode = 1,
        .flags = 0x83,
        .element = ELEMENT_MERCURY,
        .effect = 27,
        .range = 1,
        .power = 39,
    },
    // Lethe Albion
    {
        .targetMode = 1,
        .flags = 0x83,
        .element = ELEMENT_MERCURY,
        .effect = 25,
        .range = 1,
        .power = 56,
    },
    // Reverse Star
    {
        .targetMode = 1,
        .flags = 0x83,
        .element = ELEMENT_MERCURY,
        .effect = 69,
        .range = 1,
        .power = 58,
    },
    // Rising Dragon
    {
        .targetMode = 1,
        .flags = 0x83,
        .element = ELEMENT_MERCURY,
        .effect = 65,
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
        .targetMode = 1,
        .flags = 0x83,
        .element = ELEMENT_MARS,
        .range = 1,
        .power = 24,
    },
    // Boost Hack
    {
        .targetMode = 1,
        .flags = 0x83,
        .element = ELEMENT_MARS,
        .range = 1,
        .power = 17,
    },
    // Heat Smash
    {
        .targetMode = 1,
        .flags = 0x83,
        .element = ELEMENT_MARS,
        .effect = 16,
        .range = 1,
        .power = 41,
    },
    // Fire Dance
    {
        .targetMode = 1,
        .flags = 0x83,
        .element = ELEMENT_MARS,
        .effect = 8,
        .range = 1,
        .power = 51,
    },
    // Blaze Rush
    {
        .targetMode = 1,
        .flags = 0x83,
        .element = ELEMENT_MARS,
        .effect = 12,
        .range = 1,
        .power = 54,
    },
    // Flare Burst
    {
        .targetMode = 1,
        .flags = 0x83,
        .element = ELEMENT_MARS,
        .effect = 31,
        .range = 1,
        .power = 56,
    },
    // Shred
    {
        .targetMode = 1,
        .flags = 0x83,
        .element = ELEMENT_MARS,
        .effect = 12,
        .range = 1,
        .power = 33,
    },
    // Crucible Fire
    {
        .targetMode = 1,
        .flags = 0x83,
        .element = ELEMENT_MARS,
        .effect = 58,
        .range = 1,
        .power = 48,
    },
    // Scorpionfish
    {
        .targetMode = 1,
        .flags = 0x83,
        .element = ELEMENT_MARS,
        .effect = 19,
        .range = 1,
        .power = 43,
    },
    // Purgatory
    {
        .targetMode = 1,
        .flags = 0x83,
        .element = ELEMENT_MARS,
        .effect = 24,
        .range = 1,
        .power = 67,
    },
    // Soul Shatter
    {
        .targetMode = 1,
        .flags = 0x83,
        .element = ELEMENT_MARS,
        .effect = 69,
        .range = 1,
        .power = 50,
    },
    // Radiant Fire
    {
        .targetMode = 1,
        .flags = 0x83,
        .element = ELEMENT_MARS,
        .effect = 35,
        .range = 1,
        .power = 66,
    },
    // Life Shear
    {
        .targetMode = 1,
        .flags = 0x83,
        .element = ELEMENT_MARS,
        .effect = 27,
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
        .targetMode = 1,
        .flags = 0x83,
        .element = ELEMENT_JUPITER,
        .effect = 24,
        .range = 1,
        .power = 18,
    },
    // Flash Edge
    {
        .targetMode = 1,
        .flags = 0x83,
        .element = ELEMENT_JUPITER,
        .effect = 20,
        .range = 1,
        .power = 46,
    },
    // Aging Gas
    {
        .targetMode = 1,
        .flags = 0x83,
        .element = ELEMENT_JUPITER,
        .effect = 31,
        .range = 1,
        .power = 45,
    },
    // Mad Zephyr
    {
        .targetMode = 1,
        .flags = 0x83,
        .element = ELEMENT_JUPITER,
        .effect = 65,
        .range = 1,
        .power = 40,
    },
    // Lunar Slash
    {
        .targetMode = 1,
        .flags = 0x83,
        .element = ELEMENT_JUPITER,
        .effect = 16,
        .range = 1,
        .power = 25,
    },
    // Nirvana
    {
        .targetMode = 1,
        .flags = 0x83,
        .element = ELEMENT_JUPITER,
        .effect = 69,
        .range = 1,
        .power = 50,
    },
    // High Vitals
    {
        .targetMode = 1,
        .flags = 0x83,
        .element = ELEMENT_JUPITER,
        .effect = 31,
        .range = 1,
        .power = 52,
    },
    // Stun Bolt
    {
        .targetMode = 1,
        .flags = 0x83,
        .element = ELEMENT_JUPITER,
        .effect = 23,
        .range = 1,
        .power = 47,
    },
    // Trident
    {
        .targetMode = 1,
        .flags = 0x84,
        .element = ELEMENT_JUPITER,
        .effect = 91,
        .range = 1,
        .power = 12,
    },
    // Light Surge
    {
        .targetMode = 1,
        .flags = 0x83,
        .element = ELEMENT_JUPITER,
        .effect = 20,
        .range = 1,
        .power = 51,
    },
    // Raiden's Wrath
    {
        .targetMode = 1,
        .flags = 0x83,
        .element = ELEMENT_JUPITER,
        .effect = 90,
        .range = 1,
        .power = 39,
    },
    // Void Beam
    {
        .targetMode = 1,
        .flags = 0x83,
        .element = ELEMENT_JUPITER,
        .effect = 25,
        .range = 1,
        .power = 57,
    },
    // Supernova
    {
        .targetMode = 1,
        .flags = 0x83,
        .element = ELEMENT_JUPITER,
        .effect = 23,
        .range = 1,
        .power = 63,
    },
    // Apocalypse
    {
        .targetMode = 1,
        .flags = 0x83,
        .element = ELEMENT_JUPITER,
        .effect = 28,
        .range = 1,
        .power = 125,
    },
    // Legend
    {
        .targetMode = 1,
        .flags = 0x83,
        .element = ELEMENT_JUPITER,
        .effect = 68,
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
        .targetMode = 1,
        .flags = 0x84,
        .icon = 199,
        .range = 1,
        .ppCost = 7,
        .power = 15,
    },
    // Backstab
    {
        .targetMode = 1,
        .flags = 0x83,
        .element = ELEMENT_JUPITER,
        .effect = 27,
        .icon = 200,
        .range = 1,
        .ppCost = 16,
        .power = 35,
    },
    // Fire Breath
    {
        .targetMode = 1,
        .flags = 0x86,
        .element = ELEMENT_MARS,
        .icon = 201,
        .range = 2,
        .ppCost = 13,
        .power = 85,
    },
    // Juggle
    {
        .targetMode = 1,
        .flags = 0x85,
        .element = ELEMENT_MARS,
        .icon = 202,
        .range = 2,
        .ppCost = 7,
        .power = 40,
    },
    // Heat Juggle
    {
        .targetMode = 1,
        .flags = 0x85,
        .element = ELEMENT_MARS,
        .icon = 203,
        .range = 3,
        .ppCost = 13,
        .power = 75,
    },
    // Fiery Juggle
    {
        .targetMode = 1,
        .flags = 0x85,
        .element = ELEMENT_MARS,
        .icon = 204,
        .range = 3,
        .ppCost = 25,
        .power = 150,
    },
    // Flame Card
    {
        .targetMode = 1,
        .flags = 0x85,
        .element = ELEMENT_MARS,
        .icon = 205,
        .range = 2,
        .ppCost = 11,
        .power = 60,
    },
    // Thunder Card
    {
        .targetMode = 1,
        .flags = 0x85,
        .element = ELEMENT_JUPITER,
        .icon = 208,
        .range = 2,
        .ppCost = 17,
        .power = 100,
    },
    // Bramble Card
    {
        .targetMode = 1,
        .flags = 0x85,
        .icon = 207,
        .range = 2,
        .ppCost = 22,
        .power = 130,
    },
    // Frost Card
    {
        .targetMode = 1,
        .flags = 0x85,
        .element = ELEMENT_MERCURY,
        .icon = 206,
        .range = 2,
        .ppCost = 28,
        .power = 175,
    },
    // Baffle Card
    {
        .targetMode = 1,
        .flags = 0x82,
        .element = ELEMENT_JUPITER,
        .effect = 20,
        .icon = 209,
        .range = 2,
        .ppCost = 4,
    },
    // Sword Card
    {
        .targetMode = 1,
        .flags = 0x82,
        .element = ELEMENT_MARS,
        .effect = 8,
        .icon = 212,
        .range = 1,
        .ppCost = 6,
    },
    // Sleep Card
    {
        .targetMode = 1,
        .flags = 0x82,
        .element = ELEMENT_JUPITER,
        .effect = 24,
        .icon = 210,
        .range = 2,
        .ppCost = 5,
    },
    // Death Card
    {
        .targetMode = 1,
        .flags = 0x82,
        .effect = 27,
        .icon = 211,
        .range = 1,
        .ppCost = 8,
    },
    // ?
    {0},
    // Whiplash
    {
        .targetMode = 1,
        .flags = 0x84,
        .element = ELEMENT_JUPITER,
        .icon = 213,
        .range = 1,
        .ppCost = 6,
        .power = 14,
    },
    // Wild Wolf
    {
        .targetMode = 1,
        .flags = 0x83,
        .element = ELEMENT_NONE,
        .icon = 214,
        .range = 1,
        .ppCost = 3,
        .power = 30,
    },
    // Emu
    {
        .targetMode = 1,
        .flags = 0x83,
        .element = ELEMENT_JUPITER,
        .icon = 215,
        .range = 1,
        .ppCost = 10,
        .power = 75,
    },
    // Roc
    {
        .targetMode = 1,
        .flags = 0x83,
        .element = ELEMENT_NONE,
        .icon = 216,
        .range = 1,
        .ppCost = 22,
        .power = 140,
    },
    // Salamander
    {
        .targetMode = 1,
        .flags = 0x86,
        .element = ELEMENT_MARS,
        .icon = 217,
        .range = 2,
        .ppCost = 7,
        .power = 45,
    },
    // Orc
    {
        .targetMode = 1,
        .flags = 0x83,
        .element = ELEMENT_NONE,
        .icon = 218,
        .range = 1,
        .ppCost = 3,
        .power = 35,
    },
    // Harpy
    {
        .targetMode = 1,
        .flags = 0x83,
        .element = ELEMENT_JUPITER,
        .icon = 219,
        .range = 1,
        .ppCost = 10,
        .power = 80,
    },
    // Grand Golem
    {
        .targetMode = 1,
        .flags = 0x83,
        .icon = 220,
        .range = 1,
        .ppCost = 22,
        .power = 150,
    },
    // Cerberus
    {
        .targetMode = 1,
        .flags = 0x86,
        .element = ELEMENT_MARS,
        .icon = 221,
        .range = 2,
        .ppCost = 7,
        .power = 50,
    },
    // Wyvern
    {
        .targetMode = 1,
        .flags = 0x86,
        .element = ELEMENT_MARS,
        .icon = 222,
        .range = 3,
        .ppCost = 17,
        .power = 120,
    },
    // Pixie
    {
        .targetMode = 2,
        .flags = 0x81,
        .element = ELEMENT_MERCURY,
        .icon = 223,
        .range = 1,
        .ppCost = 5,
        .power = 115,
    },
    // Dinox
    {
        .targetMode = 1,
        .flags = 0x83,
        .icon = 224,
        .range = 1,
        .ppCost = 3,
        .power = 40,
    },
    // Gryphon
    {
        .targetMode = 1,
        .flags = 0x83,
        .element = ELEMENT_JUPITER,
        .effect = 65,
        .icon = 225,
        .range = 1,
        .ppCost = 10,
        .power = 85,
    },
    // Living Armor
    {
        .targetMode = 1,
        .flags = 0x83,
        .icon = 226,
        .range = 1,
        .ppCost = 17,
        .power = 130,
    },
    // Chimera
    {
        .targetMode = 1,
        .flags = 0x86,
        .element = ELEMENT_MARS,
        .icon = 227,
        .range = 2,
        .ppCost = 7,
        .power = 55,
    },
    // Blue Dragon
    {
        .targetMode = 1,
        .flags = 0x86,
        .element = ELEMENT_MERCURY,
        .icon = 228,
        .range = 3,
        .ppCost = 17,
        .power = 130,
    },
    // Faery
    {
        .targetMode = 2,
        .flags = 0x81,
        .element = ELEMENT_MERCURY,
        .icon = 229,
        .range = 1,
        .ppCost = 5,
        .power = 120,
    },
    // Elder Wood
    {
        .targetMode = 2,
        .flags = 0x81,
        .element = ELEMENT_MERCURY,
        .icon = 230,
        .range = 255,
        .ppCost = 14,
        .power = 170,
    },
    // Lich
    {
        .targetMode = 2,
        .flags = 0x82,
        .effect = 5,
        .icon = 231,
        .range = 1,
        .ppCost = 10,
    },
    // Troll
    {
        .targetMode = 1,
        .flags = 0x83,
        .icon = 232,
        .range = 1,
        .ppCost = 3,
        .power = 45,
    },
    // Minotaur
    {
        .targetMode = 1,
        .flags = 0x83,
        .icon = 233,
        .range = 1,
        .ppCost = 10,
        .power = 90,
    },
    // Ghost Soldier
    {
        .targetMode = 1,
        .flags = 0x83,
        .element = ELEMENT_JUPITER,
        .effect = 35,
        .icon = 234,
        .range = 1,
        .ppCost = 22,
        .power = 170,
    },
    // Macetail
    {
        .targetMode = 1,
        .flags = 0x86,
        .element = ELEMENT_MARS,
        .icon = 235,
        .range = 2,
        .ppCost = 7,
        .power = 60,
    },
    // Fire Dragon
    {
        .targetMode = 1,
        .flags = 0x86,
        .element = ELEMENT_MARS,
        .icon = 236,
        .range = 3,
        .ppCost = 17,
        .power = 140,
    },
    // Weird Nymph
    {
        .targetMode = 2,
        .flags = 0x81,
        .element = ELEMENT_MERCURY,
        .icon = 237,
        .range = 1,
        .ppCost = 5,
        .power = 125,
    },
    // Succubus
    {
        .targetMode = 2,
        .flags = 0x81,
        .element = ELEMENT_MERCURY,
        .icon = 238,
        .range = 1,
        .ppCost = 9,
        .power = 250,
    },
    // Estre Wood
    {
        .targetMode = 2,
        .flags = 0x81,
        .element = ELEMENT_MERCURY,
        .icon = 239,
        .range = 255,
        .ppCost = 14,
        .power = 180,
    },
    // Manticore
    {
        .targetMode = 2,
        .flags = 0x81,
        .element = ELEMENT_MARS,
        .icon = 240,
        .range = 255,
        .ppCost = 18,
        .power = 300,
    },
    // Phoenix
    {
        .targetMode = 2,
        .flags = 0x82,
        .element = ELEMENT_MARS,
        .effect = 5,
        .icon = 241,
        .range = 1,
        .ppCost = 10,
    },
    // ?
    {0},
    // Call Zombie
    {
        .targetMode = 1,
        .flags = 0x83,
        .element = ELEMENT_MERCURY,
        .icon = 252,
        .range = 1,
        .ppCost = 5,
        .power = 40,
    },
    // Call Demon
    {
        .targetMode = 1,
        .flags = 0x84,
        .effect = 34,
        .icon = 253,
        .range = 1,
        .ppCost = 13,
        .power = 19,
    },
    // Call Dullahan
    {
        .targetMode = 1,
        .flags = 0x84,
        .element = ELEMENT_JUPITER,
        .icon = 254,
        .range = 1,
        .ppCost = 21,
        .power = 30,
    },
    // Raging Heat
    {
        .targetMode = 1,
        .flags = 0x85,
        .element = ELEMENT_MARS,
        .icon = 255,
        .range = 3,
        .ppCost = 9,
        .power = 45,
    },
    // Fiery Abyss
    {
        .targetMode = 1,
        .flags = 0x85,
        .element = ELEMENT_MARS,
        .icon = 256,
        .range = 3,
        .ppCost = 18,
        .power = 90,
    },
    // Dire Inferno
    {
        .targetMode = 1,
        .flags = 0x85,
        .element = ELEMENT_MARS,
        .icon = 257,
        .range = 4,
        .ppCost = 32,
        .power = 200,
    },
    // Poison Flow
    {
        .targetMode = 1,
        .flags = 0x85,
        .element = ELEMENT_JUPITER,
        .effect = 19,
        .icon = 258,
        .range = 3,
        .ppCost = 28,
        .power = 125,
    },
    // Fire Puppet
    {
        .targetMode = 1,
        .flags = 0x82,
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
        .targetMode = 1,
        .flags = 0x86,
        .range = 2,
        .power = 35,
    },
    // Desert Gasp
    {
        .targetMode = 1,
        .flags = 0x86,
        .effect = 13,
        .range = 2,
        .power = 45,
    },
    // Black Ice
    {
        .targetMode = 1,
        .flags = 0x86,
        .range = 2,
        .power = 80,
    },
    // Toxic Breath
    {
        .targetMode = 1,
        .flags = 0x86,
        .effect = 18,
        .range = 2,
        .power = 60,
    },
    // Typhoon Blow
    {
        .targetMode = 1,
        .flags = 0x86,
        .element = ELEMENT_MERCURY,
        .range = 2,
        .power = 150,
    },
    // Blast Breath
    {
        .targetMode = 1,
        .flags = 0x86,
        .element = ELEMENT_MARS,
        .effect = 13,
        .range = 2,
        .power = 170,
    },
    // Gravel Blow
    {
        .targetMode = 1,
        .flags = 0x86,
        .range = 2,
        .power = 90,
    },
    // Darksol Gasp
    {
        .targetMode = 1,
        .flags = 0x86,
        .effect = 26,
        .range = 2,
        .power = 230,
    },
    // Flame Breath
    {
        .targetMode = 1,
        .flags = 0x86,
        .element = ELEMENT_MARS,
        .range = 2,
        .power = 170,
    },
    // Aqua Breath
    {
        .targetMode = 1,
        .flags = 0x86,
        .element = ELEMENT_MERCURY,
        .range = 2,
        .power = 140,
    },
    // Fire Breath
    {
        .targetMode = 1,
        .flags = 0x86,
        .element = ELEMENT_MARS,
        .range = 2,
        .power = 180,
    },
    // Chill Breath
    {
        .targetMode = 1,
        .flags = 0x86,
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
        .targetMode = 1,
        .flags = 0x83,
        .element = ELEMENT_JUPITER,
        .effect = 20,
        .range = 1,
        .power = 10,
    },
    // Formic Acid
    {
        .targetMode = 1,
        .flags = 0x83,
        .element = ELEMENT_MARS,
        .effect = 23,
        .range = 1,
        .power = 15,
    },
    // Claw Attack
    {
        .targetMode = 1,
        .flags = 0x83,
        .element = ELEMENT_NONE,
        .range = 1,
        .power = 20,
    },
    // Beat Dance
    {
        .targetMode = 4,
        .flags = 0x82,
        .element = ELEMENT_NONE,
        .effect = 6,
        .range = 1,
    },
    // Twin Shear
    {
        .targetMode = 1,
        .flags = 0x84,
        .range = 1,
        .power = 18,
    },
    // Paralytail
    {
        .targetMode = 1,
        .flags = 0x83,
        .effect = 23,
        .range = 1,
        .power = 20,
    },
    // Echo Cut
    {
        .targetMode = 1,
        .flags = 0x83,
        .element = ELEMENT_NONE,
        .range = 1,
        .power = 30,
    },
    // Clarion Cry
    {
        .flags = 0x89,
        .element = ELEMENT_NONE,
        .effect = 86,
        .range = 1,
    },
    // Triple Chomp
    {
        .targetMode = 1,
        .flags = 0x84,
        .element = ELEMENT_MERCURY,
        .range = 1,
        .power = 24,
    },
    // Raging Flood
    {
        .targetMode = 1,
        .flags = 0x85,
        .element = ELEMENT_MERCURY,
        .range = 6,
        .power = 110,
    },
    // Rising Venom
    {
        .targetMode = 1,
        .flags = 0x83,
        .element = ELEMENT_MERCURY,
        .effect = 18,
        .range = 1,
        .power = 30,
    },
    // Slaver
    {
        .targetMode = 1,
        .flags = 0x82,
        .element = ELEMENT_MERCURY,
        .effect = 13,
        .range = 3,
    },
    // Mighty Press
    {
        .targetMode = 1,
        .flags = 0x83,
        .effect = 27,
        .range = 1,
        .power = 50,
    },
    // Star Mine
    {
        .targetMode = 1,
        .flags = 0x88,
        .element = ELEMENT_MARS,
        .range = 6,
        .power = 35,
    },
    // Heat Stun
    {
        .targetMode = 1,
        .flags = 0x84,
        .element = ELEMENT_MARS,
        .effect = 23,
        .range = 1,
        .power = 13,
    },
    // Ocean Fist
    {
        .targetMode = 1,
        .flags = 0x83,
        .element = ELEMENT_NONE,
        .effect = 34,
        .range = 1,
        .power = 50,
    },
    // Watery Grave
    {
        .targetMode = 1,
        .flags = 0x85,
        .element = ELEMENT_NONE,
        .effect = 27,
        .range = 6,
        .power = 130,
    },
    // Counter-rush
    {
        .targetMode = 4,
        .flags = 0x87,
        .element = ELEMENT_MERCURY,
        .effect = 74,
        .range = 1,
    },
    // Counter
    {
        .targetMode = 1,
        .flags = 0x83,
        .element = ELEMENT_MERCURY,
        .range = 1,
        .power = 70,
    },
    // Bosca Hit
    {
        .targetMode = 1,
        .flags = 0x84,
        .element = ELEMENT_NONE,
        .range = 1,
        .power = 17,
    },
    // Strong Hit
    {
        .targetMode = 1,
        .flags = 0x84,
        .element = ELEMENT_NONE,
        .range = 1,
        .power = 14,
    },
    // Rolling Flame
    {
        .targetMode = 1,
        .flags = 0x83,
        .element = ELEMENT_MARS,
        .range = 2,
        .power = 100,
    },
    // Rising Dragon
    {
        .targetMode = 1,
        .flags = 0x85,
        .element = ELEMENT_MARS,
        .range = 1,
        .ppCost = 37,
        .power = 250,
    },
    // Meteor Blow
    {
        .targetMode = 1,
        .flags = 0x88,
        .element = ELEMENT_MARS,
        .range = 6,
        .ppCost = 21,
        .power = 120,
    },
    // Cage
    {
        .targetMode = 1,
        .flags = 0x82,
        .element = ELEMENT_MARS,
        .effect = 53,
        .range = 1,
        .power = 8,
    },
    // Stun Muscle
    {
        .targetMode = 1,
        .flags = 0x84,
        .element = ELEMENT_MARS,
        .effect = 23,
        .range = 1,
        .power = 13,
    },
    // Djinnfest
    {
        .targetMode = 1,
        .flags = 0x87,
        .element = ELEMENT_MARS,
        .range = 255,
    },
    // Heat Kiss
    {
        .targetMode = 1,
        .flags = 0x82,
        .element = ELEMENT_MARS,
        .effect = 8,
        .range = 2,
    },
    // Fatal Fang
    {
        .targetMode = 1,
        .flags = 0x83,
        .element = ELEMENT_NONE,
        .range = 1,
        .power = 11,
    },
    // Angle Spike
    {
        .targetMode = 1,
        .flags = 0x83,
        .element = ELEMENT_MARS,
        .range = 1,
        .power = 9,
    },
    // Quick Slash
    {
        .targetMode = 1,
        .flags = 0x84,
        .element = ELEMENT_JUPITER,
        .range = 1,
        .power = 13,
    },
    // Doublestep
    {
        .targetMode = 1,
        .flags = 0x84,
        .element = ELEMENT_NONE,
        .range = 1,
        .power = 18,
    },
    // Maneater
    {
        .targetMode = 1,
        .flags = 0x85,
        .element = ELEMENT_NONE,
        .effect = 31,
        .range = 1,
        .power = 50,
    },
    // Claw Slash
    {
        .targetMode = 1,
        .flags = 0x83,
        .element = ELEMENT_JUPITER,
        .range = 1,
        .power = 16,
    },
    // Mega Slash
    {
        .targetMode = 1,
        .flags = 0x84,
        .element = ELEMENT_JUPITER,
        .range = 1,
        .power = 15,
    },
    // Vanish Claw
    {
        .targetMode = 1,
        .flags = 0x83,
        .effect = 20,
        .range = 1,
        .power = 21,
    },
    // Crusher Grip
    {
        .targetMode = 1,
        .flags = 0x84,
        .effect = 23,
        .range = 1,
        .power = 13,
    },
    // Ur Flash
    {
        .targetMode = 1,
        .flags = 0x84,
        .element = ELEMENT_JUPITER,
        .range = 1,
        .power = 14,
    },
    // Power Bite
    {
        .targetMode = 1,
        .flags = 0x83,
        .range = 1,
        .power = 12,
    },
    // Terrible Bite
    {
        .targetMode = 1,
        .flags = 0x84,
        .effect = 34,
        .range = 1,
        .power = 13,
    },
    // Poison Sting
    {
        .targetMode = 1,
        .flags = 0x84,
        .effect = 18,
        .range = 1,
        .power = 12,
    },
    // Kill Sting
    {
        .targetMode = 1,
        .flags = 0x84,
        .effect = 27,
        .range = 1,
        .power = 11,
    },
    // Beast Needle
    {
        .targetMode = 1,
        .flags = 0x83,
        .element = ELEMENT_MERCURY,
        .range = 1,
        .power = 15,
    },
    // Poison Sting
    {
        .targetMode = 1,
        .flags = 0x83,
        .element = ELEMENT_MERCURY,
        .effect = 18,
        .range = 1,
        .power = 20,
    },
    // Stun Sting
    {
        .targetMode = 1,
        .flags = 0x83,
        .element = ELEMENT_MERCURY,
        .effect = 23,
        .range = 1,
        .power = 25,
    },
    // Mucous Gel
    {
        .targetMode = 1,
        .flags = 0x82,
        .element = ELEMENT_MERCURY,
        .effect = 20,
        .range = 1,
    },
    // Poison Gel
    {
        .targetMode = 1,
        .flags = 0x82,
        .element = ELEMENT_MERCURY,
        .effect = 19,
        .range = 1,
    },
    // Hydro Slash
    {
        .targetMode = 1,
        .flags = 0x84,
        .element = ELEMENT_MERCURY,
        .range = 1,
        .power = 14,
    },
    // Regen Dance
    {
        .flags = 0x82,
        .element = ELEMENT_NONE,
        .effect = 87,
        .range = 1,
    },
    // Fire Dance
    {
        .targetMode = 1,
        .flags = 0x84,
        .element = ELEMENT_MARS,
        .range = 1,
        .power = 13,
    },
    // Power Crush
    {
        .targetMode = 1,
        .flags = 0x84,
        .effect = 34,
        .range = 1,
        .power = 12,
    },
    // Sack
    {
        .targetMode = 4,
        .flags = 0x82,
        .effect = 6,
        .range = 1,
    },
    // Vital Moon
    {
        .targetMode = 1,
        .flags = 0x84,
        .effect = 34,
        .range = 1,
        .power = 14,
    },
    // Human Hunt
    {
        .targetMode = 1,
        .flags = 0x84,
        .effect = 27,
        .range = 1,
    },
    // Demon Eye
    {
        .targetMode = 1,
        .flags = 0x82,
        .effect = 26,
        .range = 255,
    },
    // Stun Jip
    {
        .targetMode = 1,
        .flags = 0x84,
        .effect = 23,
        .range = 1,
        .power = 16,
    },
    // Pike Assault
    {
        .targetMode = 1,
        .flags = 0x84,
        .element = ELEMENT_JUPITER,
        .effect = 35,
        .range = 1,
        .power = 14,
    },
    // Formina Sage
    {
        .targetMode = 1,
        .flags = 0x84,
        .element = ELEMENT_JUPITER,
        .range = 1,
        .power = 28,
    },
    // True Collide
    {
        .targetMode = 1,
        .flags = 0x85,
        .effect = 31,
        .range = 2,
        .power = 180,
    },
    // Armor Crush
    {
        .targetMode = 1,
        .flags = 0x84,
        .effect = 12,
        .range = 1,
        .power = 14,
    },
    // Djinn Blast
    {
        .targetMode = 1,
        .flags = 0x87,
        .element = ELEMENT_NONE,
        .range = 1,
    },
    // Djinn Storm
    {
        .targetMode = 1,
        .flags = 0x87,
        .element = ELEMENT_NONE,
        .range = 255,
    },
    // Earth Force
    {
        .targetMode = 1,
        .flags = 0x82,
        .effect = 53,
        .range = 1,
    },
    // Guard Aura
    {
        .targetMode = 4,
        .flags = 0x89,
        .effect = 88,
        .range = 1,
    },
    // Cruel Ruin
    {
        .targetMode = 1,
        .flags = 0x88,
        .range = 6,
        .power = 200,
    },
    // Mystic Call
    {
        .flags = 0x89,
        .element = ELEMENT_NONE,
        .effect = 86,
        .range = 1,
    },
    // Mine Ball
    {
        .targetMode = 1,
        .flags = 0x84,
        .element = ELEMENT_MERCURY,
        .range = 3,
        .power = 15,
    },
    // Angry Mine
    {
        .targetMode = 1,
        .flags = 0x85,
        .effect = 55,
        .range = 2,
        .power = 300,
    },
    // Djinn Stun
    {
        .targetMode = 1,
        .flags = 0x87,
        .element = ELEMENT_NONE,
        .range = 255,
    },
    // Crucible
    {
        .targetMode = 1,
        .flags = 0x87,
        .element = ELEMENT_NONE,
        .range = 255,
    },
    // Element Swap
    {
        .targetMode = 4,
        .flags = 0x89,
        .element = ELEMENT_NONE,
        .effect = 45,
        .range = 1,
    },
    // Earnest Ply
    {
        .targetMode = 4,
        .flags = 0xc1,
        .element = ELEMENT_MERCURY,
        .icon = 66,
        .fieldPsynergy = FIELD_PSYNERGY_PLY,
        .range = 1,
        .ppCost = 12,
        .power = 1000,
    },
    // Psy Boost
    {
        .targetMode = 4,
        .flags = 0x8b,
        .element = ELEMENT_NONE,
        .range = 1,
        .power = 20,
    },
    // Pressure
    {
        .targetMode = 4,
        .flags = 0x89,
        .element = ELEMENT_NONE,
        .effect = 45,
        .range = 1,
    },
};
// clang-format on
