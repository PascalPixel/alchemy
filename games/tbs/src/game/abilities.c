/// @file
/// The 519 abilities, by ability id: Psynergy, the party's and enemies'
/// moves, item uses, Djinn and summons. Each name is the English release's
/// (MsgAbilityName plus the ability's number).

#include "game.h"

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
        .element = ELEMENT_VENUS,
        .icon = 1,
        .range = 2,
        .ppCost = 4,
        .power = 12,
    },
    // Earthquake
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_VENUS,
        .icon = 2,
        .range = 3,
        .ppCost = 7,
        .power = 35,
    },
    // Quake Sphere
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_VENUS,
        .icon = 3,
        .range = 4,
        .ppCost = 15,
        .power = 65,
    },
    // Spire
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_VENUS,
        .icon = 4,
        .range = 1,
        .ppCost = 5,
        .power = 40,
    },
    // Clay Spire
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_VENUS,
        .icon = 5,
        .range = 2,
        .ppCost = 13,
        .power = 85,
    },
    // Stone Spire
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_VENUS,
        .icon = 6,
        .range = 2,
        .ppCost = 22,
        .power = 160,
    },
    // Gaia
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_VENUS,
        .icon = 7,
        .range = 2,
        .ppCost = 7,
        .power = 40,
    },
    // Mother Gaia
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_VENUS,
        .icon = 8,
        .range = 3,
        .ppCost = 17,
        .power = 100,
    },
    // Grand Gaia
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_VENUS,
        .icon = 9,
        .range = 3,
        .ppCost = 32,
        .power = 200,
    },
    // Growth
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_VENUS,
        .icon = 10,
        .range = 1,
        .ppCost = 4,
        .power = 25,
        .fieldPsynergy = 12,
    },
    // Mad Growth
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_VENUS,
        .icon = 11,
        .range = 2,
        .ppCost = 10,
        .power = 60,
    },
    // Wild Growth
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_VENUS,
        .icon = 12,
        .range = 3,
        .ppCost = 19,
        .power = 110,
    },
    // Thorn
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_VENUS,
        .icon = 13,
        .range = 2,
        .ppCost = 6,
        .power = 35,
    },
    // Briar
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_VENUS,
        .icon = 14,
        .range = 2,
        .ppCost = 11,
        .power = 70,
    },
    // Nettle
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_VENUS,
        .icon = 15,
        .range = 3,
        .ppCost = 23,
        .power = 140,
    },
    // ?
    {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // Frost
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_MERCURY,
        .icon = 16,
        .range = 2,
        .ppCost = 5,
        .power = 20,
        .fieldPsynergy = 6,
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
        .range = 2,
        .ppCost = 5,
        .power = 25,
        .fieldPsynergy = 5,
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
    // ?
    {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
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
    // ?
    {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
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
        .range = 2,
        .ppCost = 5,
        .power = 20,
        .fieldPsynergy = 14,
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
    {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // Cure
    {
        .targetMode = ABILITY_TARGET_ALLIES,
        .flags = ABILITY_USABLE_IN_FIELD | ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_HEAL_HP,
        .element = ELEMENT_VENUS,
        .icon = 61,
        .range = 1,
        .ppCost = 3,
        .power = 70,
    },
    // Cure Well
    {
        .targetMode = ABILITY_TARGET_ALLIES,
        .flags = ABILITY_USABLE_IN_FIELD | ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_HEAL_HP,
        .element = ELEMENT_VENUS,
        .icon = 62,
        .range = 1,
        .ppCost = 7,
        .power = 150,
    },
    // Potent Cure
    {
        .targetMode = ABILITY_TARGET_ALLIES,
        .flags = ABILITY_USABLE_IN_FIELD | ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_HEAL_HP,
        .element = ELEMENT_VENUS,
        .icon = 63,
        .range = 1,
        .ppCost = 10,
        .power = 300,
    },
    // Ply
    {
        .targetMode = ABILITY_TARGET_ALLIES,
        .flags = ABILITY_USABLE_IN_FIELD | ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_HEAL_HP,
        .element = ELEMENT_MERCURY,
        .icon = 64,
        .range = 1,
        .ppCost = 4,
        .power = 100,
        .fieldPsynergy = 3,
    },
    // Ply Well
    {
        .targetMode = ABILITY_TARGET_ALLIES,
        .flags = ABILITY_USABLE_IN_FIELD | ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_HEAL_HP,
        .element = ELEMENT_MERCURY,
        .icon = 65,
        .range = 1,
        .ppCost = 8,
        .power = 200,
        .fieldPsynergy = 3,
    },
    // Pure Ply
    {
        .targetMode = ABILITY_TARGET_ALLIES,
        .flags = ABILITY_USABLE_IN_FIELD | ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_HEAL_HP,
        .element = ELEMENT_MERCURY,
        .icon = 66,
        .range = 1,
        .ppCost = 12,
        .power = 1000,
        .fieldPsynergy = 3,
    },
    // Wish
    {
        .targetMode = ABILITY_TARGET_ALLIES,
        .flags = ABILITY_USABLE_IN_FIELD | ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_HEAL_HP,
        .element = ELEMENT_MERCURY,
        .icon = 67,
        .range = ABILITY_RANGE_ALL,
        .ppCost = 9,
        .power = 80,
    },
    // Wish Well
    {
        .targetMode = ABILITY_TARGET_ALLIES,
        .flags = ABILITY_USABLE_IN_FIELD | ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_HEAL_HP,
        .element = ELEMENT_MERCURY,
        .icon = 68,
        .range = ABILITY_RANGE_ALL,
        .ppCost = 13,
        .power = 160,
    },
    // Pure Wish
    {
        .targetMode = ABILITY_TARGET_ALLIES,
        .flags = ABILITY_USABLE_IN_FIELD | ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_HEAL_HP,
        .element = ELEMENT_MERCURY,
        .icon = 69,
        .range = ABILITY_RANGE_ALL,
        .ppCost = 20,
        .power = 400,
    },
    // Cure Poison
    {
        .targetMode = ABILITY_TARGET_ALLIES,
        .flags = ABILITY_USABLE_IN_FIELD | ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_EFFECT,
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
        .flags = ABILITY_USABLE_IN_FIELD | ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_EFFECT,
        .element = ELEMENT_VENUS,
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
        .element = ELEMENT_VENUS,
        .effect = ABILITY_EFFECT_POISON,
        .range = 1,
        .ppCost = 4,
    },
    // Poison
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_EFFECT,
        .element = ELEMENT_VENUS,
        .effect = ABILITY_EFFECT_VENOM,
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
        .element = ELEMENT_VENUS,
        .effect = ABILITY_EFFECT_HAUNT,
        .icon = 88,
        .range = 2,
        .ppCost = 5,
    },
    // Curse
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_EFFECT,
        .element = ELEMENT_VENUS,
        .effect = ABILITY_EFFECT_CURSE,
        .icon = 90,
        .range = 1,
        .ppCost = 6,
    },
    // Condemn
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_EFFECT,
        .element = ELEMENT_VENUS,
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
    {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // Move
    {
        .targetMode = ABILITY_TARGET_ONE,
        .flags = ABILITY_FORMULA_NO_POWER,
        .element = ELEMENT_MARS,
        .icon = 95,
        .range = 1,
        .ppCost = 2,
        .fieldPsynergy = 1,
    },
    // Mind Read
    {
        .targetMode = ABILITY_TARGET_ONE,
        .flags = ABILITY_FORMULA_NO_POWER,
        .element = ELEMENT_JUPITER,
        .icon = 96,
        .range = 1,
        .ppCost = 1,
        .fieldPsynergy = 2,
    },
    // Force
    {
        .targetMode = ABILITY_TARGET_ONE,
        .flags = ABILITY_FORMULA_NO_POWER,
        .element = ELEMENT_NONE,
        .icon = 97,
        .range = 1,
        .ppCost = 2,
        .fieldPsynergy = 4,
    },
    // Lift
    {
        .targetMode = ABILITY_TARGET_ONE,
        .flags = ABILITY_FORMULA_NO_POWER,
        .element = ELEMENT_MERCURY,
        .icon = 98,
        .range = 1,
        .ppCost = 2,
        .fieldPsynergy = 7,
    },
    // Reveal
    {
        .targetMode = ABILITY_TARGET_ONE,
        .flags = ABILITY_FORMULA_NO_POWER,
        .element = ELEMENT_JUPITER,
        .icon = 99,
        .range = 1,
        .ppCost = 1,
        .fieldPsynergy = 8,
    },
    // Halt
    {
        .targetMode = ABILITY_TARGET_ONE,
        .flags = ABILITY_FORMULA_NO_POWER,
        .element = ELEMENT_JUPITER,
        .icon = 100,
        .range = 1,
        .ppCost = 2,
        .fieldPsynergy = 9,
    },
    // Cloak
    {
        .targetMode = ABILITY_TARGET_ONE,
        .flags = ABILITY_FORMULA_NO_POWER,
        .element = ELEMENT_MERCURY,
        .icon = 101,
        .range = 1,
        .ppCost = 1,
        .fieldPsynergy = 10,
    },
    // Carry
    {
        .targetMode = ABILITY_TARGET_ONE,
        .flags = ABILITY_FORMULA_NO_POWER,
        .element = ELEMENT_VENUS,
        .icon = 102,
        .range = 1,
        .ppCost = 2,
        .fieldPsynergy = 11,
    },
    // Catch
    {
        .targetMode = ABILITY_TARGET_ONE,
        .flags = ABILITY_FORMULA_NO_POWER,
        .element = ELEMENT_VENUS,
        .icon = 103,
        .range = 1,
        .ppCost = 1,
        .fieldPsynergy = 13,
    },
    // Retreat
    {
        .targetMode = ABILITY_TARGET_ONE,
        .flags = ABILITY_FORMULA_NO_POWER,
        .element = ELEMENT_VENUS,
        .icon = 156,
        .range = 1,
        .ppCost = 6,
        .fieldPsynergy = 15,
    },
    // Avoid
    {
        .targetMode = ABILITY_TARGET_ONE,
        .flags = ABILITY_FORMULA_NO_POWER,
        .element = ELEMENT_MERCURY,
        .icon = 157,
        .range = 1,
        .ppCost = 5,
        .fieldPsynergy = 16,
    },
    // ?
    {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
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
        .element = ELEMENT_VENUS,
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
        .element = ELEMENT_VENUS,
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
        .element = ELEMENT_VENUS,
        .icon = 104,
        .range = 2,
        .ppCost = 5,
        .power = 30,
    },
    // Rockslide
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_VENUS,
        .icon = 105,
        .range = 3,
        .ppCost = 15,
        .power = 90,
    },
    // Avalanche
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_VENUS,
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
        .element = ELEMENT_VENUS,
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
        .element = ELEMENT_VENUS,
        .icon = 120,
        .range = 2,
        .ppCost = 7,
        .power = 45,
    },
    // Punji Trap
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_VENUS,
        .icon = 121,
        .range = 2,
        .ppCost = 13,
        .power = 85,
    },
    // Punji Strike
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_VENUS,
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
        .range = 2,
        .ppCost = 3,
        .power = 18,
        .fieldPsynergy = 14,
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
    // Thunderstorm
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
        .element = ELEMENT_VENUS,
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
    // ?
    {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // Terra Strike
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_VENUS,
        .range = 1,
        .power = 20,
    },
    // Poison Cloud
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_VENUS,
        .effect = ABILITY_EFFECT_POISON,
        .range = 1,
        .power = 55,
    },
    // Deadly Poison
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_VENUS,
        .effect = ABILITY_EFFECT_VENOM,
        .range = 1,
        .power = 53,
    },
    // Mortal Danger
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_VENUS,
        .effect = ABILITY_EFFECT_INSTANT_DEATH,
        .range = 1,
        .power = 28,
    },
    // Bad Omen
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_VENUS,
        .effect = ABILITY_EFFECT_HAUNT,
        .range = 1,
        .power = 43,
    },
    // Life Nourish
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_VENUS,
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
    // Demon Fire
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
        .element = ELEMENT_VENUS,
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
    {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // Herb
    {
        .targetMode = ABILITY_TARGET_ALLIES,
        .flags = ABILITY_USABLE_IN_FIELD | ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_HEAL_HP,
        .element = ELEMENT_NONE,
        .range = 1,
        .power = 50,
    },
    // Nut
    {
        .targetMode = ABILITY_TARGET_ALLIES,
        .flags = ABILITY_USABLE_IN_FIELD | ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_HEAL_HP,
        .element = ELEMENT_NONE,
        .range = 1,
        .power = 200,
    },
    // Vial
    {
        .targetMode = ABILITY_TARGET_ALLIES,
        .flags = ABILITY_USABLE_IN_FIELD | ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_HEAL_HP,
        .element = ELEMENT_NONE,
        .range = 1,
        .power = 500,
    },
    // Potion
    {
        .targetMode = ABILITY_TARGET_ALLIES,
        .flags = ABILITY_USABLE_IN_FIELD | ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_HEAL_HP,
        .element = ELEMENT_NONE,
        .range = 1,
        .power = 9999,
    },
    // Soothing Water
    {
        .targetMode = ABILITY_TARGET_ALLIES,
        .flags = ABILITY_USABLE_IN_FIELD | ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_HEAL_HP,
        .element = ELEMENT_NONE,
        .range = 1,
        .power = 9999,
    },
    // Psy Crystal
    {
        .targetMode = ABILITY_TARGET_ALLIES,
        .flags = ABILITY_USABLE_IN_FIELD | ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_HEAL_PP,
        .element = ELEMENT_NONE,
        .range = 1,
        .power = 9999,
    },
    // Antidote
    {
        .targetMode = ABILITY_TARGET_ALLIES,
        .flags = ABILITY_USABLE_IN_FIELD | ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_EFFECT,
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
        .flags = ABILITY_USABLE_IN_FIELD | ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_EFFECT,
        .element = ELEMENT_NONE,
        .effect = ABILITY_EFFECT_REVIVE,
        .range = 1,
    },
    // ?
    {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
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
    {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
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
        .flags = ABILITY_USABLE_IN_FIELD | ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_HEAL_PP,
        .element = ELEMENT_NONE,
        .range = 1,
        .power = 7,
    },
    // Corn
    {
        .targetMode = ABILITY_TARGET_ALLIES,
        .flags = ABILITY_USABLE_IN_FIELD | ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_HEAL_HP,
        .element = ELEMENT_NONE,
        .range = 1,
        .power = 100,
    },
    // ?
    {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // Flint
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK_MULTIPLIED,
        .element = ELEMENT_VENUS,
        .range = 1,
        .power = 16,
    },
    // Granite
    {
        .targetMode = ABILITY_TARGET_ALLIES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_EFFECT_ONLY,
        .element = ELEMENT_VENUS,
        .effect = ABILITY_EFFECT_DEFEND,
        .range = ABILITY_RANGE_ALL,
    },
    // Quartz
    {
        .targetMode = ABILITY_TARGET_ALLIES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_EFFECT,
        .element = ELEMENT_VENUS,
        .effect = ABILITY_EFFECT_REVIVE_HALF,
        .range = 1,
    },
    // Vine
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_EFFECT,
        .element = ELEMENT_VENUS,
        .effect = ABILITY_EFFECT_AGILITY_DOWN,
        .range = ABILITY_RANGE_ALL,
    },
    // Sap
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_VENUS,
        .effect = ABILITY_EFFECT_DRAIN_HALF_HP,
        .range = 1,
        .power = 50,
    },
    // Ground
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_EFFECT,
        .element = ELEMENT_VENUS,
        .effect = ABILITY_EFFECT_GROUND,
        .range = 1,
    },
    // Bane
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_VENUS,
        .effect = ABILITY_EFFECT_VENOM,
        .range = 1,
        .power = 60,
    },
    // =
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_VENUS,
        .range = 1,
    },
    // =
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_VENUS,
        .range = 1,
    },
    // =
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_VENUS,
        .range = 1,
    },
    // =
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_VENUS,
        .range = 1,
    },
    // =
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_VENUS,
        .range = 1,
    },
    // =
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_VENUS,
        .range = 1,
    },
    // =
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_VENUS,
        .range = 1,
    },
    // =
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_VENUS,
        .range = 1,
    },
    // =
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_VENUS,
        .range = 1,
    },
    // =
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_VENUS,
        .range = 1,
    },
    // =
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_VENUS,
        .range = 1,
    },
    // =
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_VENUS,
        .range = 1,
    },
    // =
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_VENUS,
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
    // =
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_VENUS,
        .range = 1,
    },
    // =
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_VENUS,
        .range = 1,
    },
    // =
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_VENUS,
        .range = 1,
    },
    // =
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_VENUS,
        .range = 1,
    },
    // =
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_VENUS,
        .range = 1,
    },
    // =
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_VENUS,
        .range = 1,
    },
    // =
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_VENUS,
        .range = 1,
    },
    // =
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_VENUS,
        .range = 1,
    },
    // =
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_VENUS,
        .range = 1,
    },
    // =
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_VENUS,
        .range = 1,
    },
    // =
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_VENUS,
        .range = 1,
    },
    // =
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_VENUS,
        .range = 1,
    },
    // =
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_VENUS,
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
    // =
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_VENUS,
        .range = 1,
    },
    // =
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_VENUS,
        .range = 1,
    },
    // =
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_VENUS,
        .range = 1,
    },
    // =
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_VENUS,
        .range = 1,
    },
    // =
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_VENUS,
        .range = 1,
    },
    // =
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_VENUS,
        .range = 1,
    },
    // =
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_VENUS,
        .range = 1,
    },
    // =
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_VENUS,
        .range = 1,
    },
    // =
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_VENUS,
        .range = 1,
    },
    // =
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_VENUS,
        .range = 1,
    },
    // =
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_VENUS,
        .range = 1,
    },
    // =
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_VENUS,
        .range = 1,
    },
    // =
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_VENUS,
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
    // =
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_VENUS,
        .range = 1,
    },
    // =
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_VENUS,
        .range = 1,
    },
    // =
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_VENUS,
        .range = 1,
    },
    // =
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_VENUS,
        .range = 1,
    },
    // =
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_VENUS,
        .range = 1,
    },
    // =
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_VENUS,
        .range = 1,
    },
    // =
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_VENUS,
        .range = 1,
    },
    // =
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_VENUS,
        .range = 1,
    },
    // =
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_VENUS,
        .range = 1,
    },
    // =
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_VENUS,
        .range = 1,
    },
    // =
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_VENUS,
        .range = 1,
    },
    // =
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_VENUS,
        .range = 1,
    },
    // =
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_VENUS,
        .range = 1,
    },
    // Venus
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_SUMMON,
        .element = ELEMENT_VENUS,
        .icon = 140,
        .range = 6,
        .power = 30,
    },
    // Ramses
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_SUMMON,
        .element = ELEMENT_VENUS,
        .icon = 141,
        .range = 6,
        .power = 60,
    },
    // Cybele
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_SUMMON,
        .element = ELEMENT_VENUS,
        .icon = 142,
        .range = 6,
        .power = 120,
    },
    // Judgment
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_SUMMON,
        .element = ELEMENT_VENUS,
        .icon = 143,
        .range = 6,
        .power = 240,
    },
    // ?
    {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
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
    // ?
    {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
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
    // ?
    {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
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
    // ?
    {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // Fire Blessing
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ENEMY_DAMAGE,
        .element = ELEMENT_MARS,
        .range = 2,
        .power = 60,
    },
    // Fire Blessing
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ENEMY_DAMAGE,
        .element = ELEMENT_MARS,
        .range = 2,
        .power = 90,
    },
    // Fire Blessing
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ENEMY_DAMAGE,
        .element = ELEMENT_MARS,
        .range = 2,
        .power = 130,
    },
    // Water Blessing
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ENEMY_DAMAGE,
        .element = ELEMENT_MERCURY,
        .range = 2,
        .power = 25,
    },
    // Water Blessing
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ENEMY_DAMAGE,
        .element = ELEMENT_MERCURY,
        .range = 2,
        .power = 50,
    },
    // Water Blessing
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ENEMY_DAMAGE,
        .element = ELEMENT_MERCURY,
        .range = 2,
        .power = 100,
    },
    // Ice Blessing
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ENEMY_DAMAGE,
        .element = ELEMENT_MERCURY,
        .range = 2,
        .power = 50,
    },
    // Ice Blessing
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ENEMY_DAMAGE,
        .element = ELEMENT_MERCURY,
        .range = 2,
        .power = 90,
    },
    // Ice Blessing
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ENEMY_DAMAGE,
        .element = ELEMENT_MERCURY,
        .range = 2,
        .power = 180,
    },
    // Dark Blessing
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ENEMY_DAMAGE,
        .element = ELEMENT_VENUS,
        .effect = ABILITY_EFFECT_DELUSION,
        .range = 2,
        .power = 70,
    },
    // Dark Blessing
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ENEMY_DAMAGE,
        .element = ELEMENT_VENUS,
        .effect = ABILITY_EFFECT_DELUSION,
        .range = 2,
        .power = 110,
    },
    // Dark Blessing
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ENEMY_DAMAGE,
        .element = ELEMENT_MARS,
        .effect = ABILITY_EFFECT_DEFENSE_DOWN,
        .range = 2,
        .power = 170,
    },
    // Acid Blessing
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ENEMY_DAMAGE,
        .element = ELEMENT_MARS,
        .effect = ABILITY_EFFECT_DEFENSE_DOWN,
        .range = 2,
        .power = 190,
    },
    // Storm Blessing
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ENEMY_DAMAGE,
        .element = ELEMENT_JUPITER,
        .effect = ABILITY_EFFECT_STUN,
        .range = 2,
        .power = 190,
    },
    // Storm Blessing
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
        .effect = ABILITY_EFFECT_CRAZY_VOICE,
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
        .element = ELEMENT_VENUS,
        .effect = ABILITY_EFFECT_VENOM,
        .range = 2,
        .power = 30,
    },
    // ?
    {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // Rumble
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_VENUS,
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
        .element = ELEMENT_VENUS,
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
        .element = ELEMENT_VENUS,
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
        .element = ELEMENT_VENUS,
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
        .element = ELEMENT_VENUS,
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
        .element = ELEMENT_VENUS,
        .range = 1,
        .power = 15,
    },
    // Ransack
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_VENUS,
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
        .element = ELEMENT_VENUS,
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
        .element = ELEMENT_VENUS,
        .effect = ABILITY_EFFECT_VENOM,
        .range = 1,
        .power = 30,
    },
    // Onslaught
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_VENUS,
        .range = 1,
        .power = 24,
    },
    // Vampiric Fang
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_EFFECT,
        .element = ELEMENT_VENUS,
        .effect = ABILITY_EFFECT_DRAIN_HP,
        .range = 1,
        .power = 35,
    },
    // Bacteria Rush
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_VENUS,
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
        .element = ELEMENT_VENUS,
        .range = 1,
        .power = 40,
    },
    // Double Fang
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_VENUS,
        .effect = ABILITY_EFFECT_DOUBLE_BLOW_1_IN_4,
        .range = 1,
        .power = 35,
    },
    // Mortal Blow
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_VENUS,
        .effect = ABILITY_EFFECT_SELF_DESTRUCT,
        .range = 1,
        .power = 35,
    },
    // Freebite Rush
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_VENUS,
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
        .element = ELEMENT_VENUS,
        .effect = ABILITY_EFFECT_VENOM,
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
        .element = ELEMENT_VENUS,
        .effect = ABILITY_EFFECT_SELF_DESTRUCT,
        .range = 2,
        .power = 200,
    },
    // Headbutt
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_VENUS,
        .range = 1,
        .power = 60,
    },
    // Poison Ink
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_VENUS,
        .effect = ABILITY_EFFECT_VENOM,
        .range = 1,
    },
    // Truncheon Fist
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_VENUS,
        .effect = ABILITY_EFFECT_LEAVE_ONE_HP,
        .range = 1,
        .power = 60,
    },
    // Counterstrike
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_VENUS,
        .range = 1,
        .power = 50,
    },
    // Mad Dash
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_VENUS,
        .range = 1,
        .power = 70,
    },
    // Soothing Star
    {
        .targetMode = ABILITY_TARGET_ALLIES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_HEAL_HP,
        .element = ELEMENT_VENUS,
        .range = 2,
        .power = 10,
    },
    // Spider Web
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_EFFECT,
        .element = ELEMENT_VENUS,
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
        .element = ELEMENT_VENUS,
        .effect = ABILITY_EFFECT_DELUSION,
        .range = 2,
    },
    // Spasm
    {
        .targetMode = ABILITY_TARGET_SELF,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_HEAL_HP,
        .element = ELEMENT_VENUS,
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
        .element = ELEMENT_VENUS,
        .effect = ABILITY_EFFECT_ATTACK_DOWN,
        .range = 2,
    },
    // Haunting
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_EFFECT,
        .element = ELEMENT_VENUS,
        .effect = ABILITY_EFFECT_HAUNT,
        .range = 1,
    },
    // Worms
    {
        .targetMode = ABILITY_TARGET_SELF,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_HEAL_HP,
        .element = ELEMENT_VENUS,
        .range = 1,
        .power = 50,
    },
    // Berserk
    {
        .targetMode = ABILITY_TARGET_SELF,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_EFFECT,
        .element = ELEMENT_VENUS,
        .effect = ABILITY_EFFECT_ATTACK_UP_2,
        .range = 1,
    },
    // Lucid Prophecy
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_EFFECT,
        .element = ELEMENT_VENUS,
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
        .targetMode = ABILITY_TARGET_ONE,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_NO_POWER,
        .element = ELEMENT_NONE,
        .effect = ABILITY_EFFECT_FLEE,
        .range = 1,
    },
    // Contain
    {
        .targetMode = ABILITY_TARGET_ONE,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_NO_POWER,
        .element = ELEMENT_NONE,
        .effect = ABILITY_EFFECT_THREATEN,
        .range = 1,
    },
    // Threaten
    {
        .targetMode = ABILITY_TARGET_ONE,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_NO_POWER,
        .element = ELEMENT_NONE,
        .effect = ABILITY_EFFECT_THREATEN,
        .range = 1,
    },
    // Tremble
    {
        .targetMode = ABILITY_TARGET_ONE,
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
        .targetMode = ABILITY_TARGET_ONE,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_NO_POWER,
        .element = ELEMENT_NONE,
        .effect = ABILITY_EFFECT_CALL_ALLY,
        .range = 1,
    },
    // Ally Search
    {
        .targetMode = ABILITY_TARGET_ONE,
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
        .effect = ABILITY_EFFECT_SIDESTEP,
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
    // Search for Help
    {
        .targetMode = ABILITY_TARGET_ONE,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_NO_POWER,
        .element = ELEMENT_NONE,
        .effect = ABILITY_EFFECT_CALL_HELP,
        .range = 1,
    },
    // Can't Use
    {
        .targetMode = ABILITY_TARGET_ONE,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_VENUS,
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
    // Death Size
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_VENUS,
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
        .element = ELEMENT_VENUS,
        .effect = ABILITY_EFFECT_DRAIN_HP,
        .range = 1,
        .power = 80,
    },
    // Severe Blow
    {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK_MULTIPLIED,
        .element = ELEMENT_VENUS,
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
};
// clang-format on
