/// @file
/// The 519 abilities, by ability id: Psynergy, the party's and enemies'
/// moves, item uses, Djinn and summons. Each name is the English release's
/// (MsgAbilityName plus the ability's number).

#include "game.h"
#include "ability_ids.h"

// clang-format off
const struct AbilityDefinition gAbilityDefinitions[] = {
    // Nothing
    [ABILITY_000] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_NONE,
        .range = 1,
    },
    [ABILITY_ATTACK] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_NONE,
        .range = 1,
    },
    // Defend
    [ABILITY_002] = {
        .targetMode = ABILITY_TARGET_SELF,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_NO_POWER,
        .element = ELEMENT_NONE,
        .effect = ABILITY_EFFECT_DEFEND,
        .range = 1,
    },
    // Quake
    [ABILITY_003] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_VENUS,
        .icon = 1,
        .range = 2,
        .ppCost = 4,
        .power = 12,
    },
    // Earthquake
    [ABILITY_004] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_VENUS,
        .icon = 2,
        .range = 3,
        .ppCost = 7,
        .power = 35,
    },
    // Quake Sphere
    [ABILITY_005] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_VENUS,
        .icon = 3,
        .range = 4,
        .ppCost = 15,
        .power = 65,
    },
    // Spire
    [ABILITY_006] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_VENUS,
        .icon = 4,
        .range = 1,
        .ppCost = 5,
        .power = 40,
    },
    // Clay Spire
    [ABILITY_007] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_VENUS,
        .icon = 5,
        .range = 2,
        .ppCost = 13,
        .power = 85,
    },
    // Stone Spire
    [ABILITY_008] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_VENUS,
        .icon = 6,
        .range = 2,
        .ppCost = 22,
        .power = 160,
    },
    // Gaia
    [ABILITY_009] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_VENUS,
        .icon = 7,
        .range = 2,
        .ppCost = 7,
        .power = 40,
    },
    // Mother Gaia
    [ABILITY_010] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_VENUS,
        .icon = 8,
        .range = 3,
        .ppCost = 17,
        .power = 100,
    },
    // Grand Gaia
    [ABILITY_011] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_VENUS,
        .icon = 9,
        .range = 3,
        .ppCost = 32,
        .power = 200,
    },
    [ABILITY_GROWTH] = {
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
    [ABILITY_013] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_VENUS,
        .icon = 11,
        .range = 2,
        .ppCost = 10,
        .power = 60,
    },
    // Wild Growth
    [ABILITY_014] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_VENUS,
        .icon = 12,
        .range = 3,
        .ppCost = 19,
        .power = 110,
    },
    // Thorn
    [ABILITY_015] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_VENUS,
        .icon = 13,
        .range = 2,
        .ppCost = 6,
        .power = 35,
    },
    // Briar
    [ABILITY_016] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_VENUS,
        .icon = 14,
        .range = 2,
        .ppCost = 11,
        .power = 70,
    },
    // Nettle
    [ABILITY_017] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_VENUS,
        .icon = 15,
        .range = 3,
        .ppCost = 23,
        .power = 140,
    },
    // ?
    [ABILITY_018] = {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    [ABILITY_019] = {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    [ABILITY_020] = {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    [ABILITY_021] = {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    [ABILITY_022] = {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    [ABILITY_023] = {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    [ABILITY_FROST] = {
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
    [ABILITY_025] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_MERCURY,
        .icon = 17,
        .range = 2,
        .ppCost = 8,
        .power = 45,
    },
    // Glacier
    [ABILITY_026] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_MERCURY,
        .icon = 18,
        .range = 2,
        .ppCost = 15,
        .power = 100,
    },
    // Ice
    [ABILITY_027] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_MERCURY,
        .icon = 19,
        .range = 1,
        .ppCost = 5,
        .power = 35,
    },
    // Ice Horn
    [ABILITY_028] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_MERCURY,
        .icon = 20,
        .range = 2,
        .ppCost = 11,
        .power = 70,
    },
    // Ice Missile
    [ABILITY_029] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_MERCURY,
        .icon = 21,
        .range = 2,
        .ppCost = 23,
        .power = 160,
    },
    // Prism
    [ABILITY_030] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_MERCURY,
        .icon = 22,
        .range = 2,
        .ppCost = 7,
        .power = 40,
    },
    // Hail Prism
    [ABILITY_031] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_MERCURY,
        .icon = 23,
        .range = 3,
        .ppCost = 16,
        .power = 90,
    },
    // Freeze Prism
    [ABILITY_032] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_MERCURY,
        .icon = 24,
        .range = 3,
        .ppCost = 31,
        .power = 190,
    },
    [ABILITY_DOUSE] = {
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
    [ABILITY_034] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_MERCURY,
        .icon = 29,
        .range = 2,
        .ppCost = 10,
        .power = 60,
    },
    // Deluge
    [ABILITY_035] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_MERCURY,
        .icon = 30,
        .range = 3,
        .ppCost = 20,
        .power = 120,
    },
    // Froth
    [ABILITY_036] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_MERCURY,
        .icon = 25,
        .range = 2,
        .ppCost = 5,
        .power = 28,
    },
    // Froth Sphere
    [ABILITY_037] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_MERCURY,
        .icon = 26,
        .range = 3,
        .ppCost = 12,
        .power = 65,
    },
    // Froth Spiral
    [ABILITY_038] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_MERCURY,
        .icon = 27,
        .range = 4,
        .ppCost = 31,
        .power = 150,
    },
    // ?
    [ABILITY_039] = {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    [ABILITY_040] = {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    [ABILITY_041] = {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    [ABILITY_042] = {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    [ABILITY_043] = {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    [ABILITY_044] = {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // Flare
    [ABILITY_045] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_MARS,
        .icon = 31,
        .range = 2,
        .ppCost = 4,
        .power = 15,
    },
    // Flare Wall
    [ABILITY_046] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_MARS,
        .icon = 32,
        .range = 2,
        .ppCost = 7,
        .power = 40,
    },
    // Flare Storm
    [ABILITY_047] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_MARS,
        .icon = 33,
        .range = 2,
        .ppCost = 12,
        .power = 80,
    },
    // Fire
    [ABILITY_048] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_MARS,
        .icon = 34,
        .range = 2,
        .ppCost = 6,
        .power = 35,
    },
    // Fireball
    [ABILITY_049] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_MARS,
        .icon = 35,
        .range = 3,
        .ppCost = 12,
        .power = 65,
    },
    // Inferno
    [ABILITY_050] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_MARS,
        .icon = 36,
        .range = 3,
        .ppCost = 23,
        .power = 140,
    },
    // Volcano
    [ABILITY_051] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_MARS,
        .icon = 37,
        .range = 1,
        .ppCost = 6,
        .power = 45,
    },
    // Eruption
    [ABILITY_052] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_MARS,
        .icon = 38,
        .range = 2,
        .ppCost = 14,
        .power = 90,
    },
    // Pyroclasm
    [ABILITY_053] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_MARS,
        .icon = 39,
        .range = 3,
        .ppCost = 29,
        .power = 180,
    },
    // Blast
    [ABILITY_054] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_MARS,
        .icon = 40,
        .range = 2,
        .ppCost = 5,
        .power = 25,
    },
    // Mad Blast
    [ABILITY_055] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_MARS,
        .icon = 41,
        .range = 2,
        .ppCost = 9,
        .power = 50,
    },
    // Fiery Blast
    [ABILITY_056] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_MARS,
        .icon = 42,
        .range = 3,
        .ppCost = 19,
        .power = 110,
    },
    // Blast
    [ABILITY_057] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_MARS,
        .icon = 43,
        .range = 2,
        .ppCost = 7,
        .power = 40,
    },
    // Nova
    [ABILITY_058] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_MARS,
        .icon = 44,
        .range = 3,
        .ppCost = 13,
        .power = 70,
    },
    // Supernova
    [ABILITY_059] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_MARS,
        .icon = 45,
        .range = 4,
        .ppCost = 31,
        .power = 150,
    },
    // ?
    [ABILITY_060] = {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    [ABILITY_061] = {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    [ABILITY_062] = {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    [ABILITY_063] = {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    [ABILITY_064] = {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    [ABILITY_065] = {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // Bolt
    [ABILITY_066] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_JUPITER,
        .icon = 46,
        .range = 1,
        .ppCost = 4,
        .power = 20,
    },
    // Flash Bolt
    [ABILITY_067] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_JUPITER,
        .icon = 47,
        .range = 2,
        .ppCost = 7,
        .power = 40,
    },
    // Blue Bolt
    [ABILITY_068] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_JUPITER,
        .icon = 48,
        .range = 2,
        .ppCost = 14,
        .power = 90,
    },
    // Ray
    [ABILITY_069] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_JUPITER,
        .icon = 49,
        .range = 2,
        .ppCost = 6,
        .power = 35,
    },
    // Storm Ray
    [ABILITY_070] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_JUPITER,
        .icon = 50,
        .range = 2,
        .ppCost = 10,
        .power = 65,
    },
    // Destruct Ray
    [ABILITY_071] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_JUPITER,
        .icon = 51,
        .range = 2,
        .ppCost = 21,
        .power = 150,
    },
    // Plasma
    [ABILITY_072] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_JUPITER,
        .icon = 52,
        .range = 2,
        .ppCost = 8,
        .power = 45,
    },
    // Shine Plasma
    [ABILITY_073] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_JUPITER,
        .icon = 53,
        .range = 3,
        .ppCost = 18,
        .power = 100,
    },
    // Spark Plasma
    [ABILITY_074] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_JUPITER,
        .icon = 54,
        .range = 4,
        .ppCost = 37,
        .power = 180,
    },
    // Slash
    [ABILITY_075] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_JUPITER,
        .icon = 55,
        .range = 1,
        .ppCost = 4,
        .power = 25,
    },
    // Wind Slash
    [ABILITY_076] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_JUPITER,
        .icon = 56,
        .range = 2,
        .ppCost = 9,
        .power = 50,
    },
    // Sonic Slash
    [ABILITY_077] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_JUPITER,
        .icon = 57,
        .range = 3,
        .ppCost = 20,
        .power = 120,
    },
    [ABILITY_WHIRLWIND] = {
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
    [ABILITY_079] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_JUPITER,
        .icon = 59,
        .range = 3,
        .ppCost = 14,
        .power = 80,
    },
    // Tempest
    [ABILITY_080] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_JUPITER,
        .icon = 60,
        .range = 3,
        .ppCost = 27,
        .power = 160,
    },
    // ?
    [ABILITY_081] = {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    [ABILITY_082] = {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    [ABILITY_083] = {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    [ABILITY_084] = {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    [ABILITY_085] = {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    [ABILITY_086] = {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    [ABILITY_087] = {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    [ABILITY_088] = {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    [ABILITY_089] = {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // Cure
    [ABILITY_090] = {
        .targetMode = ABILITY_TARGET_ALLIES,
        .flags = ABILITY_USABLE_IN_FIELD | ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_HEAL_HP,
        .element = ELEMENT_VENUS,
        .icon = 61,
        .range = 1,
        .ppCost = 3,
        .power = 70,
    },
    // Cure Well
    [ABILITY_091] = {
        .targetMode = ABILITY_TARGET_ALLIES,
        .flags = ABILITY_USABLE_IN_FIELD | ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_HEAL_HP,
        .element = ELEMENT_VENUS,
        .icon = 62,
        .range = 1,
        .ppCost = 7,
        .power = 150,
    },
    // Potent Cure
    [ABILITY_092] = {
        .targetMode = ABILITY_TARGET_ALLIES,
        .flags = ABILITY_USABLE_IN_FIELD | ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_HEAL_HP,
        .element = ELEMENT_VENUS,
        .icon = 63,
        .range = 1,
        .ppCost = 10,
        .power = 300,
    },
    [ABILITY_PLY] = {
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
    [ABILITY_094] = {
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
    [ABILITY_095] = {
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
    [ABILITY_096] = {
        .targetMode = ABILITY_TARGET_ALLIES,
        .flags = ABILITY_USABLE_IN_FIELD | ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_HEAL_HP,
        .element = ELEMENT_MERCURY,
        .icon = 67,
        .range = ABILITY_RANGE_ALL,
        .ppCost = 9,
        .power = 80,
    },
    // Wish Well
    [ABILITY_097] = {
        .targetMode = ABILITY_TARGET_ALLIES,
        .flags = ABILITY_USABLE_IN_FIELD | ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_HEAL_HP,
        .element = ELEMENT_MERCURY,
        .icon = 68,
        .range = ABILITY_RANGE_ALL,
        .ppCost = 13,
        .power = 160,
    },
    // Pure Wish
    [ABILITY_098] = {
        .targetMode = ABILITY_TARGET_ALLIES,
        .flags = ABILITY_USABLE_IN_FIELD | ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_HEAL_HP,
        .element = ELEMENT_MERCURY,
        .icon = 69,
        .range = ABILITY_RANGE_ALL,
        .ppCost = 20,
        .power = 400,
    },
    // Cure Poison
    [ABILITY_099] = {
        .targetMode = ABILITY_TARGET_ALLIES,
        .flags = ABILITY_USABLE_IN_FIELD | ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_EFFECT,
        .element = ELEMENT_MERCURY,
        .effect = ABILITY_EFFECT_CURE_POISON,
        .icon = 70,
        .range = 1,
        .ppCost = 2,
    },
    // Restore
    [ABILITY_100] = {
        .targetMode = ABILITY_TARGET_ALLIES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_EFFECT,
        .element = ELEMENT_MERCURY,
        .effect = ABILITY_EFFECT_RESTORE,
        .icon = 71,
        .range = 1,
        .ppCost = 3,
    },
    // Revive
    [ABILITY_101] = {
        .targetMode = ABILITY_TARGET_ALLIES,
        .flags = ABILITY_USABLE_IN_FIELD | ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_EFFECT,
        .element = ELEMENT_VENUS,
        .effect = ABILITY_EFFECT_REVIVE,
        .icon = 72,
        .range = 1,
        .ppCost = 15,
    },
    // Impact
    [ABILITY_102] = {
        .targetMode = ABILITY_TARGET_ALLIES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_EFFECT,
        .element = ELEMENT_JUPITER,
        .effect = ABILITY_EFFECT_ATTACK_UP_2,
        .icon = 73,
        .range = 1,
        .ppCost = 7,
    },
    // High Impact
    [ABILITY_103] = {
        .targetMode = ABILITY_TARGET_ALLIES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_EFFECT,
        .element = ELEMENT_JUPITER,
        .effect = ABILITY_EFFECT_ATTACK_UP,
        .icon = 74,
        .range = ABILITY_RANGE_ALL,
        .ppCost = 12,
    },
    // Dull
    [ABILITY_104] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_EFFECT,
        .element = ELEMENT_JUPITER,
        .effect = ABILITY_EFFECT_ATTACK_DOWN_2,
        .icon = 75,
        .range = 1,
        .ppCost = 6,
    },
    // Blunt
    [ABILITY_105] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_EFFECT,
        .element = ELEMENT_JUPITER,
        .effect = ABILITY_EFFECT_ATTACK_DOWN,
        .icon = 76,
        .range = 2,
        .ppCost = 11,
    },
    // Guard
    [ABILITY_106] = {
        .targetMode = ABILITY_TARGET_ALLIES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_EFFECT,
        .element = ELEMENT_MARS,
        .effect = ABILITY_EFFECT_DEFENSE_UP_2,
        .icon = 77,
        .range = 1,
        .ppCost = 3,
    },
    // Protect
    [ABILITY_107] = {
        .targetMode = ABILITY_TARGET_ALLIES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_EFFECT,
        .element = ELEMENT_MARS,
        .effect = ABILITY_EFFECT_DEFENSE_UP,
        .icon = 78,
        .range = ABILITY_RANGE_ALL,
        .ppCost = 5,
    },
    // Impair
    [ABILITY_108] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_EFFECT,
        .element = ELEMENT_MARS,
        .effect = ABILITY_EFFECT_DEFENSE_DOWN_2,
        .icon = 79,
        .range = 1,
        .ppCost = 4,
    },
    // Debilitate
    [ABILITY_109] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_EFFECT,
        .element = ELEMENT_MARS,
        .effect = ABILITY_EFFECT_DEFENSE_DOWN,
        .icon = 80,
        .range = 2,
        .ppCost = 6,
    },
    // Ward
    [ABILITY_110] = {
        .targetMode = ABILITY_TARGET_ALLIES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_EFFECT,
        .element = ELEMENT_JUPITER,
        .effect = ABILITY_EFFECT_RESISTANCE_UP_2,
        .icon = 81,
        .range = 1,
        .ppCost = 3,
    },
    // Resist
    [ABILITY_111] = {
        .targetMode = ABILITY_TARGET_ALLIES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_EFFECT,
        .element = ELEMENT_JUPITER,
        .effect = ABILITY_EFFECT_RESISTANCE_UP,
        .icon = 82,
        .range = ABILITY_RANGE_ALL,
        .ppCost = 5,
    },
    // Weaken
    [ABILITY_112] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_EFFECT,
        .element = ELEMENT_JUPITER,
        .effect = ABILITY_EFFECT_RESISTANCE_DOWN_2,
        .icon = 83,
        .range = 1,
        .ppCost = 4,
    },
    // Enfeeble
    [ABILITY_113] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_EFFECT,
        .element = ELEMENT_JUPITER,
        .effect = ABILITY_EFFECT_RESISTANCE_DOWN,
        .icon = 84,
        .range = 2,
        .ppCost = 6,
    },
    // Taint
    [ABILITY_114] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_EFFECT,
        .element = ELEMENT_VENUS,
        .effect = ABILITY_EFFECT_POISON,
        .range = 1,
        .ppCost = 4,
    },
    // Poison
    [ABILITY_115] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_EFFECT,
        .element = ELEMENT_VENUS,
        .effect = ABILITY_EFFECT_VENOM,
        .range = 1,
        .ppCost = 6,
    },
    // Delude
    [ABILITY_116] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_EFFECT,
        .element = ELEMENT_JUPITER,
        .effect = ABILITY_EFFECT_DELUSION,
        .icon = 85,
        .range = 2,
        .ppCost = 4,
    },
    // Confuse
    [ABILITY_117] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_EFFECT,
        .element = ELEMENT_JUPITER,
        .effect = ABILITY_EFFECT_CONFUSION,
        .range = 2,
        .ppCost = 6,
    },
    // Charm
    [ABILITY_118] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_EFFECT,
        .element = ELEMENT_JUPITER,
        .effect = ABILITY_EFFECT_CHARM,
        .range = 1,
        .ppCost = 9,
    },
    // Paralyze
    [ABILITY_119] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_EFFECT,
        .element = ELEMENT_JUPITER,
        .effect = ABILITY_EFFECT_STUN,
        .range = 1,
        .ppCost = 7,
    },
    // Sleep
    [ABILITY_120] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_EFFECT,
        .element = ELEMENT_JUPITER,
        .effect = ABILITY_EFFECT_SLEEP,
        .icon = 86,
        .range = 2,
        .ppCost = 5,
    },
    // Bind
    [ABILITY_121] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_EFFECT,
        .element = ELEMENT_JUPITER,
        .effect = ABILITY_EFFECT_SEAL,
        .icon = 87,
        .range = 1,
        .ppCost = 4,
    },
    // Haunt
    [ABILITY_122] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_EFFECT,
        .element = ELEMENT_VENUS,
        .effect = ABILITY_EFFECT_HAUNT,
        .icon = 88,
        .range = 2,
        .ppCost = 5,
    },
    // Curse
    [ABILITY_123] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_EFFECT,
        .element = ELEMENT_VENUS,
        .effect = ABILITY_EFFECT_CURSE,
        .icon = 90,
        .range = 1,
        .ppCost = 6,
    },
    // Condemn
    [ABILITY_124] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_EFFECT,
        .element = ELEMENT_VENUS,
        .effect = ABILITY_EFFECT_INSTANT_DEATH,
        .icon = 89,
        .range = 1,
        .ppCost = 8,
    },
    // Drain
    [ABILITY_125] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_EFFECT,
        .element = ELEMENT_JUPITER,
        .effect = ABILITY_EFFECT_DRAIN_HP,
        .icon = 91,
        .range = 1,
        .ppCost = 3,
        .power = 50,
    },
    [ABILITY_PSY_DRAIN] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_EFFECT,
        .element = ELEMENT_JUPITER,
        .effect = ABILITY_EFFECT_DRAIN_PP,
        .icon = 92,
        .range = 1,
        .power = 15,
    },
    // Break
    [ABILITY_127] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_EFFECT,
        .element = ELEMENT_MERCURY,
        .effect = ABILITY_EFFECT_BREAK,
        .icon = 93,
        .range = ABILITY_RANGE_ALL,
        .ppCost = 5,
    },
    // Regenerate
    [ABILITY_128] = {
        .targetMode = ABILITY_TARGET_ALLIES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_EFFECT,
        .element = ELEMENT_MERCURY,
        .effect = ABILITY_EFFECT_REGENERATE,
        .range = 1,
        .ppCost = 6,
        .power = 60,
    },
    // Reflect
    [ABILITY_129] = {
        .targetMode = ABILITY_TARGET_ALLIES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_EFFECT,
        .element = ELEMENT_MERCURY,
        .effect = ABILITY_EFFECT_REFLECT,
        .icon = 94,
        .range = 1,
        .ppCost = 5,
    },
    // ?
    [ABILITY_130] = {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    [ABILITY_131] = {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    [ABILITY_132] = {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    [ABILITY_133] = {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    [ABILITY_134] = {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    [ABILITY_135] = {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    [ABILITY_136] = {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    [ABILITY_137] = {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    [ABILITY_138] = {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    [ABILITY_139] = {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    [ABILITY_MOVE] = {
        .targetMode = ABILITY_TARGET_ONE,
        .flags = ABILITY_FORMULA_NO_POWER,
        .element = ELEMENT_MARS,
        .icon = 95,
        .range = 1,
        .ppCost = 2,
        .fieldPsynergy = 1,
    },
    [ABILITY_MIND_READ] = {
        .targetMode = ABILITY_TARGET_ONE,
        .flags = ABILITY_FORMULA_NO_POWER,
        .element = ELEMENT_JUPITER,
        .icon = 96,
        .range = 1,
        .ppCost = 1,
        .fieldPsynergy = 2,
    },
    [ABILITY_FORCE] = {
        .targetMode = ABILITY_TARGET_ONE,
        .flags = ABILITY_FORMULA_NO_POWER,
        .element = ELEMENT_NONE,
        .icon = 97,
        .range = 1,
        .ppCost = 2,
        .fieldPsynergy = 4,
    },
    [ABILITY_LIFT] = {
        .targetMode = ABILITY_TARGET_ONE,
        .flags = ABILITY_FORMULA_NO_POWER,
        .element = ELEMENT_MERCURY,
        .icon = 98,
        .range = 1,
        .ppCost = 2,
        .fieldPsynergy = 7,
    },
    [ABILITY_REVEAL] = {
        .targetMode = ABILITY_TARGET_ONE,
        .flags = ABILITY_FORMULA_NO_POWER,
        .element = ELEMENT_JUPITER,
        .icon = 99,
        .range = 1,
        .ppCost = 1,
        .fieldPsynergy = 8,
    },
    [ABILITY_HALT] = {
        .targetMode = ABILITY_TARGET_ONE,
        .flags = ABILITY_FORMULA_NO_POWER,
        .element = ELEMENT_JUPITER,
        .icon = 100,
        .range = 1,
        .ppCost = 2,
        .fieldPsynergy = 9,
    },
    [ABILITY_CLOAK] = {
        .targetMode = ABILITY_TARGET_ONE,
        .flags = ABILITY_FORMULA_NO_POWER,
        .element = ELEMENT_MERCURY,
        .icon = 101,
        .range = 1,
        .ppCost = 1,
        .fieldPsynergy = 10,
    },
    [ABILITY_CARRY] = {
        .targetMode = ABILITY_TARGET_ONE,
        .flags = ABILITY_FORMULA_NO_POWER,
        .element = ELEMENT_VENUS,
        .icon = 102,
        .range = 1,
        .ppCost = 2,
        .fieldPsynergy = 11,
    },
    [ABILITY_CATCH] = {
        .targetMode = ABILITY_TARGET_ONE,
        .flags = ABILITY_FORMULA_NO_POWER,
        .element = ELEMENT_VENUS,
        .icon = 103,
        .range = 1,
        .ppCost = 1,
        .fieldPsynergy = 13,
    },
    [ABILITY_RETREAT] = {
        .targetMode = ABILITY_TARGET_ONE,
        .flags = ABILITY_FORMULA_NO_POWER,
        .element = ELEMENT_VENUS,
        .icon = 156,
        .range = 1,
        .ppCost = 6,
        .fieldPsynergy = 15,
    },
    [ABILITY_AVOID] = {
        .targetMode = ABILITY_TARGET_ONE,
        .flags = ABILITY_FORMULA_NO_POWER,
        .element = ELEMENT_MERCURY,
        .icon = 157,
        .range = 1,
        .ppCost = 5,
        .fieldPsynergy = 16,
    },
    // ?
    [ABILITY_151] = {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    [ABILITY_152] = {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    [ABILITY_153] = {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    [ABILITY_154] = {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    [ABILITY_155] = {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    [ABILITY_156] = {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    [ABILITY_157] = {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    [ABILITY_158] = {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    [ABILITY_159] = {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // Dragon Cloud
    [ABILITY_160] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_MARS,
        .icon = 117,
        .range = 1,
        .ppCost = 6,
        .power = 80,
    },
    // Demon Night
    [ABILITY_161] = {
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
    [ABILITY_162] = {
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
    [ABILITY_163] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK_MULTIPLIED,
        .element = ELEMENT_JUPITER,
        .icon = 118,
        .range = 1,
        .ppCost = 12,
        .power = 18,
    },
    // Rockfall
    [ABILITY_164] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_VENUS,
        .icon = 104,
        .range = 2,
        .ppCost = 5,
        .power = 30,
    },
    // Rockslide
    [ABILITY_165] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_VENUS,
        .icon = 105,
        .range = 3,
        .ppCost = 15,
        .power = 90,
    },
    // Avalanche
    [ABILITY_166] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_VENUS,
        .icon = 106,
        .range = 3,
        .ppCost = 30,
        .power = 160,
    },
    // Lava Shower
    [ABILITY_167] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_MARS,
        .icon = 107,
        .range = 1,
        .ppCost = 4,
        .power = 40,
    },
    // Molten Bath
    [ABILITY_168] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_MARS,
        .icon = 108,
        .range = 2,
        .ppCost = 12,
        .power = 70,
    },
    // Magma Storm
    [ABILITY_169] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_MARS,
        .icon = 109,
        .range = 3,
        .ppCost = 27,
        .power = 120,
    },
    // Demon Spear
    [ABILITY_170] = {
        .targetMode = ABILITY_TARGET_ALLIES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_EFFECT,
        .element = ELEMENT_JUPITER,
        .effect = ABILITY_EFFECT_ATTACK_UP_2,
        .icon = 110,
        .range = 1,
        .ppCost = 7,
    },
    // Angel Spear
    [ABILITY_171] = {
        .targetMode = ABILITY_TARGET_ALLIES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_EFFECT,
        .element = ELEMENT_JUPITER,
        .effect = ABILITY_EFFECT_ATTACK_UP,
        .icon = 111,
        .range = ABILITY_RANGE_ALL,
        .ppCost = 12,
    },
    // Guardian
    [ABILITY_172] = {
        .targetMode = ABILITY_TARGET_ALLIES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_EFFECT,
        .element = ELEMENT_MARS,
        .effect = ABILITY_EFFECT_DEFENSE_UP_2,
        .icon = 114,
        .range = 1,
        .ppCost = 3,
    },
    // Protector
    [ABILITY_173] = {
        .targetMode = ABILITY_TARGET_ALLIES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_EFFECT,
        .element = ELEMENT_MARS,
        .effect = ABILITY_EFFECT_DEFENSE_UP,
        .icon = 115,
        .range = ABILITY_RANGE_ALL,
        .ppCost = 5,
    },
    // Magic Shell
    [ABILITY_174] = {
        .targetMode = ABILITY_TARGET_ALLIES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_EFFECT,
        .element = ELEMENT_JUPITER,
        .effect = ABILITY_EFFECT_RESISTANCE_UP_2,
        .icon = 112,
        .range = 1,
        .ppCost = 3,
    },
    // Magic Shield
    [ABILITY_175] = {
        .targetMode = ABILITY_TARGET_ALLIES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_EFFECT,
        .element = ELEMENT_JUPITER,
        .effect = ABILITY_EFFECT_RESISTANCE_UP,
        .icon = 113,
        .range = ABILITY_RANGE_ALL,
        .ppCost = 5,
    },
    // Death Plunge
    [ABILITY_176] = {
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
    [ABILITY_177] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK_MULTIPLIED,
        .element = ELEMENT_JUPITER,
        .icon = 135,
        .range = 2,
        .ppCost = 8,
        .power = 8,
    },
    [ABILITY_ANNIHILATION] = {
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
    [ABILITY_179] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_VENUS,
        .icon = 120,
        .range = 2,
        .ppCost = 7,
        .power = 45,
    },
    // Punji Trap
    [ABILITY_180] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_VENUS,
        .icon = 121,
        .range = 2,
        .ppCost = 13,
        .power = 85,
    },
    // Punji Strike
    [ABILITY_181] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_VENUS,
        .icon = 122,
        .range = 3,
        .ppCost = 24,
        .power = 150,
    },
    // Fire Bomb
    [ABILITY_182] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_MARS,
        .icon = 123,
        .range = 2,
        .ppCost = 5,
        .power = 35,
    },
    // Cluster Bomb
    [ABILITY_183] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_MARS,
        .icon = 124,
        .range = 3,
        .ppCost = 11,
        .power = 65,
    },
    // Carpet Bomb
    [ABILITY_184] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_MARS,
        .icon = 125,
        .range = 4,
        .ppCost = 29,
        .power = 130,
    },
    // Gale
    [ABILITY_185] = {
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
    [ABILITY_186] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_JUPITER,
        .icon = 127,
        .range = 3,
        .ppCost = 12,
        .power = 75,
    },
    // Hurricane
    [ABILITY_187] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_JUPITER,
        .icon = 128,
        .range = 3,
        .ppCost = 25,
        .power = 150,
    },
    // Thunderclap
    [ABILITY_188] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_JUPITER,
        .icon = 129,
        .range = 2,
        .ppCost = 9,
        .power = 50,
    },
    // Thunderbolt
    [ABILITY_189] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_JUPITER,
        .icon = 130,
        .range = 3,
        .ppCost = 19,
        .power = 110,
    },
    // Thunderstorm
    [ABILITY_190] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_JUPITER,
        .icon = 131,
        .range = 4,
        .ppCost = 39,
        .power = 190,
    },
    // Mist
    [ABILITY_191] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_EFFECT,
        .element = ELEMENT_JUPITER,
        .effect = ABILITY_EFFECT_DELUSION,
        .icon = 132,
        .range = 2,
        .ppCost = 4,
    },
    // Ragnarok
    [ABILITY_192] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_VENUS,
        .icon = 137,
        .range = 1,
        .ppCost = 7,
        .power = 35,
    },
    // Cutting Edge
    [ABILITY_193] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_MERCURY,
        .icon = 136,
        .range = 1,
        .ppCost = 5,
        .power = 32,
    },
    // Heat Wave
    [ABILITY_194] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_MARS,
        .icon = 138,
        .range = 1,
        .ppCost = 6,
        .power = 33,
    },
    // Astral Blast
    [ABILITY_195] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_JUPITER,
        .icon = 158,
        .range = 1,
        .ppCost = 5,
        .power = 32,
    },
    // Planet Diver
    [ABILITY_196] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_MARS,
        .icon = 139,
        .range = 1,
        .ppCost = 7,
        .power = 36,
    },
    // ?
    [ABILITY_197] = {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    [ABILITY_198] = {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    [ABILITY_199] = {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    [ABILITY_200] = {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    [ABILITY_201] = {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    [ABILITY_202] = {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    [ABILITY_203] = {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    [ABILITY_204] = {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    [ABILITY_205] = {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    [ABILITY_206] = {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    [ABILITY_207] = {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    [ABILITY_208] = {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    [ABILITY_209] = {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // Terra Strike
    [ABILITY_210] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_VENUS,
        .range = 1,
        .power = 20,
    },
    // Poison Cloud
    [ABILITY_211] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_VENUS,
        .effect = ABILITY_EFFECT_POISON,
        .range = 1,
        .power = 55,
    },
    // Deadly Poison
    [ABILITY_212] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_VENUS,
        .effect = ABILITY_EFFECT_VENOM,
        .range = 1,
        .power = 53,
    },
    // Mortal Danger
    [ABILITY_213] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_VENUS,
        .effect = ABILITY_EFFECT_INSTANT_DEATH,
        .range = 1,
        .power = 28,
    },
    // Bad Omen
    [ABILITY_214] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_VENUS,
        .effect = ABILITY_EFFECT_HAUNT,
        .range = 1,
        .power = 43,
    },
    // Life Nourish
    [ABILITY_215] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_VENUS,
        .effect = ABILITY_EFFECT_DRAIN_HP,
        .range = 1,
        .power = 25,
    },
    // Aqua Sock
    [ABILITY_216] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_MERCURY,
        .range = 1,
        .power = 42,
    },
    // Blizzard
    [ABILITY_217] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_MERCURY,
        .effect = ABILITY_EFFECT_ATTACK_DOWN_2,
        .range = 1,
        .power = 18,
    },
    // Frost Bite
    [ABILITY_218] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_MERCURY,
        .effect = ABILITY_EFFECT_STUN,
        .range = 1,
        .power = 28,
    },
    [ABILITY_DROWN] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_MERCURY,
        .effect = ABILITY_EFFECT_INSTANT_DEATH,
        .range = 1,
        .power = 47,
    },
    // Life Leech
    [ABILITY_220] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_MERCURY,
        .effect = ABILITY_EFFECT_DRAIN_HP,
        .range = 1,
        .power = 29,
    },
    // Psy Leech
    [ABILITY_221] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_MERCURY,
        .effect = ABILITY_EFFECT_PSY_LEECH,
        .range = 1,
        .power = 22,
    },
    // Broil
    [ABILITY_222] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_MARS,
        .range = 1,
        .power = 23,
    },
    // Meltdown
    [ABILITY_223] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_MARS,
        .effect = ABILITY_EFFECT_DEFENSE_DOWN_2,
        .range = 1,
        .power = 47,
    },
    [ABILITY_HEAT_MIRAGE] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_MARS,
        .effect = ABILITY_EFFECT_DELUSION,
        .range = 1,
        .power = 21,
    },
    // Barrage
    [ABILITY_225] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_MARS,
        .effect = ABILITY_EFFECT_STUN,
        .range = 1,
        .power = 20,
    },
    // Demon Fire
    [ABILITY_226] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_MARS,
        .effect = ABILITY_EFFECT_HAUNT,
        .range = 1,
        .power = 60,
    },
    // Acid Bath
    [ABILITY_227] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_MARS,
        .effect = ABILITY_EFFECT_ATTACK_DOWN_2,
        .range = 1,
        .power = 34,
    },
    // Vorpal Slash
    [ABILITY_228] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_JUPITER,
        .range = 1,
        .power = 16,
    },
    // Stun Voltage
    [ABILITY_229] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_JUPITER,
        .effect = ABILITY_EFFECT_STUN,
        .range = 1,
        .power = 12,
    },
    // Blinding Smog
    [ABILITY_230] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_JUPITER,
        .effect = ABILITY_EFFECT_DELUSION,
        .range = 1,
        .power = 49,
    },
    // Murk
    [ABILITY_231] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_JUPITER,
        .effect = ABILITY_EFFECT_SLEEP,
        .range = 1,
        .power = 10,
    },
    // Cyclone Slash
    [ABILITY_232] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_JUPITER,
        .effect = ABILITY_EFFECT_RESISTANCE_DOWN_2,
        .range = 1,
        .power = 38,
    },
    // Psyphon Seal
    [ABILITY_233] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_JUPITER,
        .effect = ABILITY_EFFECT_SEAL,
        .range = 1,
        .power = 14,
    },
    // Rapid Smash
    [ABILITY_234] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK_MULTIPLIED,
        .element = ELEMENT_JUPITER,
        .effect = ABILITY_EFFECT_DOUBLE_BLOW_60,
        .range = 1,
        .power = 9,
    },
    // Sonic Smash
    [ABILITY_235] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_JUPITER,
        .effect = ABILITY_EFFECT_TRIPLE_BLOW_60,
        .range = 1,
    },
    // Asura
    [ABILITY_236] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_JUPITER,
        .effect = ABILITY_EFFECT_HALVE_DEFENSE,
        .range = 1,
        .power = 64,
    },
    // Titan Blade
    [ABILITY_237] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_VENUS,
        .range = 1,
        .power = 70,
    },
    // Shining Star
    [ABILITY_238] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_JUPITER,
        .effect = ABILITY_EFFECT_DELUSION,
        .range = 1,
        .power = 57,
    },
    // ?
    [ABILITY_239] = {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    [ABILITY_240] = {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    [ABILITY_241] = {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    [ABILITY_242] = {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    [ABILITY_243] = {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    [ABILITY_244] = {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    [ABILITY_245] = {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    [ABILITY_246] = {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    [ABILITY_247] = {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    [ABILITY_248] = {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    [ABILITY_249] = {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // Herb
    [ABILITY_250] = {
        .targetMode = ABILITY_TARGET_ALLIES,
        .flags = ABILITY_USABLE_IN_FIELD | ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_HEAL_HP,
        .element = ELEMENT_NONE,
        .range = 1,
        .power = 50,
    },
    // Nut
    [ABILITY_251] = {
        .targetMode = ABILITY_TARGET_ALLIES,
        .flags = ABILITY_USABLE_IN_FIELD | ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_HEAL_HP,
        .element = ELEMENT_NONE,
        .range = 1,
        .power = 200,
    },
    // Vial
    [ABILITY_252] = {
        .targetMode = ABILITY_TARGET_ALLIES,
        .flags = ABILITY_USABLE_IN_FIELD | ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_HEAL_HP,
        .element = ELEMENT_NONE,
        .range = 1,
        .power = 500,
    },
    // Potion
    [ABILITY_253] = {
        .targetMode = ABILITY_TARGET_ALLIES,
        .flags = ABILITY_USABLE_IN_FIELD | ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_HEAL_HP,
        .element = ELEMENT_NONE,
        .range = 1,
        .power = 9999,
    },
    // Soothing Water
    [ABILITY_254] = {
        .targetMode = ABILITY_TARGET_ALLIES,
        .flags = ABILITY_USABLE_IN_FIELD | ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_HEAL_HP,
        .element = ELEMENT_NONE,
        .range = 1,
        .power = 9999,
    },
    // Psy Crystal
    [ABILITY_255] = {
        .targetMode = ABILITY_TARGET_ALLIES,
        .flags = ABILITY_USABLE_IN_FIELD | ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_HEAL_PP,
        .element = ELEMENT_NONE,
        .range = 1,
        .power = 9999,
    },
    // Antidote
    [ABILITY_256] = {
        .targetMode = ABILITY_TARGET_ALLIES,
        .flags = ABILITY_USABLE_IN_FIELD | ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_EFFECT,
        .element = ELEMENT_NONE,
        .effect = ABILITY_EFFECT_CURE_POISON,
        .range = 1,
    },
    // Elixir
    [ABILITY_257] = {
        .targetMode = ABILITY_TARGET_ALLIES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_EFFECT,
        .element = ELEMENT_NONE,
        .effect = ABILITY_EFFECT_RESTORE,
        .range = 1,
    },
    // Water of Life
    [ABILITY_258] = {
        .targetMode = ABILITY_TARGET_ALLIES,
        .flags = ABILITY_USABLE_IN_FIELD | ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_EFFECT,
        .element = ELEMENT_NONE,
        .effect = ABILITY_EFFECT_REVIVE,
        .range = 1,
    },
    // ?
    [ABILITY_259] = {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    [ABILITY_POWER_BREAD] = {
        .targetMode = ABILITY_TARGET_ALLIES,
        .flags = ABILITY_USABLE_IN_FIELD | ABILITY_FORMULA_NO_POWER,
        .element = ELEMENT_NONE,
        .range = 1,
        .power = 8,
    },
    [ABILITY_COOKIE] = {
        .targetMode = ABILITY_TARGET_ALLIES,
        .flags = ABILITY_USABLE_IN_FIELD | ABILITY_FORMULA_NO_POWER,
        .element = ELEMENT_NONE,
        .range = 1,
        .power = 6,
    },
    [ABILITY_APPLE] = {
        .targetMode = ABILITY_TARGET_ALLIES,
        .flags = ABILITY_USABLE_IN_FIELD | ABILITY_FORMULA_NO_POWER,
        .element = ELEMENT_NONE,
        .range = 1,
        .power = 5,
    },
    [ABILITY_HARD_NUT] = {
        .targetMode = ABILITY_TARGET_ALLIES,
        .flags = ABILITY_USABLE_IN_FIELD | ABILITY_FORMULA_NO_POWER,
        .element = ELEMENT_NONE,
        .range = 1,
        .power = 3,
    },
    [ABILITY_MINT] = {
        .targetMode = ABILITY_TARGET_ALLIES,
        .flags = ABILITY_USABLE_IN_FIELD | ABILITY_FORMULA_NO_POWER,
        .element = ELEMENT_NONE,
        .range = 1,
        .power = 5,
    },
    [ABILITY_LUCKY_PEPPER] = {
        .targetMode = ABILITY_TARGET_ALLIES,
        .flags = ABILITY_USABLE_IN_FIELD | ABILITY_FORMULA_NO_POWER,
        .element = ELEMENT_NONE,
        .range = 1,
        .power = 2,
    },
    // ?
    [ABILITY_266] = {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // Smoke Bomb
    [ABILITY_267] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_EFFECT,
        .element = ELEMENT_JUPITER,
        .effect = ABILITY_EFFECT_DELUSION,
        .range = 1,
    },
    // Sleep Bomb
    [ABILITY_268] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_EFFECT,
        .element = ELEMENT_JUPITER,
        .effect = ABILITY_EFFECT_SLEEP,
        .range = 1,
    },
    // Adept Ring
    [ABILITY_269] = {
        .targetMode = ABILITY_TARGET_SELF,
        .flags = ABILITY_USABLE_IN_FIELD | ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_HEAL_PP,
        .element = ELEMENT_NONE,
        .range = 1,
        .power = 7,
    },
    // Corn
    [ABILITY_270] = {
        .targetMode = ABILITY_TARGET_ALLIES,
        .flags = ABILITY_USABLE_IN_FIELD | ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_HEAL_HP,
        .element = ELEMENT_NONE,
        .range = 1,
        .power = 100,
    },
    // ?
    [ABILITY_271] = {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    [ABILITY_272] = {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    [ABILITY_273] = {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    [ABILITY_274] = {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    [ABILITY_275] = {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    [ABILITY_276] = {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    [ABILITY_277] = {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    [ABILITY_278] = {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    [ABILITY_279] = {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    [ABILITY_280] = {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    [ABILITY_281] = {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    [ABILITY_282] = {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    [ABILITY_283] = {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    [ABILITY_284] = {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    [ABILITY_285] = {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    [ABILITY_286] = {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    [ABILITY_287] = {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    [ABILITY_288] = {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    [ABILITY_289] = {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    [ABILITY_290] = {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    [ABILITY_291] = {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    [ABILITY_292] = {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    [ABILITY_293] = {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    [ABILITY_294] = {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    [ABILITY_295] = {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    [ABILITY_296] = {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    [ABILITY_297] = {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    [ABILITY_298] = {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    [ABILITY_299] = {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // Flint
    [ABILITY_DJINNI_FIRST] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK_MULTIPLIED,
        .element = ELEMENT_VENUS,
        .range = 1,
        .power = 16,
    },
    // Granite
    [ABILITY_301] = {
        .targetMode = ABILITY_TARGET_ALLIES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_EFFECT_ONLY,
        .element = ELEMENT_VENUS,
        .effect = ABILITY_EFFECT_DEFEND,
        .range = ABILITY_RANGE_ALL,
    },
    // Quartz
    [ABILITY_302] = {
        .targetMode = ABILITY_TARGET_ALLIES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_EFFECT,
        .element = ELEMENT_VENUS,
        .effect = ABILITY_EFFECT_REVIVE_HALF,
        .range = 1,
    },
    // Vine
    [ABILITY_303] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_EFFECT,
        .element = ELEMENT_VENUS,
        .effect = ABILITY_EFFECT_AGILITY_DOWN,
        .range = ABILITY_RANGE_ALL,
    },
    // Sap
    [ABILITY_304] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_VENUS,
        .effect = ABILITY_EFFECT_DRAIN_HALF_HP,
        .range = 1,
        .power = 50,
    },
    // Ground
    [ABILITY_305] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_EFFECT,
        .element = ELEMENT_VENUS,
        .effect = ABILITY_EFFECT_GROUND,
        .range = 1,
    },
    // Bane
    [ABILITY_306] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_VENUS,
        .effect = ABILITY_EFFECT_VENOM,
        .range = 1,
        .power = 60,
    },
    // =
    [ABILITY_307] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_VENUS,
        .range = 1,
    },
    // =
    [ABILITY_308] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_VENUS,
        .range = 1,
    },
    // =
    [ABILITY_309] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_VENUS,
        .range = 1,
    },
    // =
    [ABILITY_310] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_VENUS,
        .range = 1,
    },
    // =
    [ABILITY_311] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_VENUS,
        .range = 1,
    },
    // =
    [ABILITY_312] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_VENUS,
        .range = 1,
    },
    // =
    [ABILITY_313] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_VENUS,
        .range = 1,
    },
    // =
    [ABILITY_314] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_VENUS,
        .range = 1,
    },
    // =
    [ABILITY_315] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_VENUS,
        .range = 1,
    },
    // =
    [ABILITY_316] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_VENUS,
        .range = 1,
    },
    // =
    [ABILITY_317] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_VENUS,
        .range = 1,
    },
    // =
    [ABILITY_318] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_VENUS,
        .range = 1,
    },
    // =
    [ABILITY_319] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_VENUS,
        .range = 1,
    },
    // Fizz
    [ABILITY_320] = {
        .targetMode = ABILITY_TARGET_ALLIES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_HEAL_HP,
        .element = ELEMENT_MERCURY,
        .effect = ABILITY_EFFECT_HEAL_HP_60,
        .range = 1,
    },
    // Sleet
    [ABILITY_321] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_MERCURY,
        .effect = ABILITY_EFFECT_ATTACK_DOWN_2,
        .range = 1,
        .power = 30,
    },
    // Mist
    [ABILITY_322] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK_MULTIPLIED,
        .element = ELEMENT_MERCURY,
        .effect = ABILITY_EFFECT_SLEEP,
        .range = 1,
        .power = 13,
    },
    // Spritz
    [ABILITY_323] = {
        .targetMode = ABILITY_TARGET_ALLIES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_HEAL_HP,
        .element = ELEMENT_MERCURY,
        .effect = ABILITY_EFFECT_HEAL_HP_30,
        .range = ABILITY_RANGE_ALL,
    },
    // Hail
    [ABILITY_324] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_MERCURY,
        .effect = ABILITY_EFFECT_DEFENSE_DOWN_2,
        .range = 1,
        .power = 40,
    },
    // Tonic
    [ABILITY_325] = {
        .targetMode = ABILITY_TARGET_ALLIES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_EFFECT,
        .element = ELEMENT_MERCURY,
        .effect = ABILITY_EFFECT_CURE_ALL,
        .range = ABILITY_RANGE_ALL,
    },
    // Dew
    [ABILITY_326] = {
        .targetMode = ABILITY_TARGET_ALLIES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_EFFECT,
        .element = ELEMENT_MERCURY,
        .effect = ABILITY_EFFECT_REVIVE_80,
        .range = 1,
    },
    // =
    [ABILITY_327] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_VENUS,
        .range = 1,
    },
    // =
    [ABILITY_328] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_VENUS,
        .range = 1,
    },
    // =
    [ABILITY_329] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_VENUS,
        .range = 1,
    },
    // =
    [ABILITY_330] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_VENUS,
        .range = 1,
    },
    // =
    [ABILITY_331] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_VENUS,
        .range = 1,
    },
    // =
    [ABILITY_332] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_VENUS,
        .range = 1,
    },
    // =
    [ABILITY_333] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_VENUS,
        .range = 1,
    },
    // =
    [ABILITY_334] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_VENUS,
        .range = 1,
    },
    // =
    [ABILITY_335] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_VENUS,
        .range = 1,
    },
    // =
    [ABILITY_336] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_VENUS,
        .range = 1,
    },
    // =
    [ABILITY_337] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_VENUS,
        .range = 1,
    },
    // =
    [ABILITY_338] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_VENUS,
        .range = 1,
    },
    // =
    [ABILITY_339] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_VENUS,
        .range = 1,
    },
    // Forge
    [ABILITY_340] = {
        .targetMode = ABILITY_TARGET_ALLIES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_EFFECT,
        .element = ELEMENT_MARS,
        .effect = ABILITY_EFFECT_ATTACK_UP_2,
        .range = ABILITY_RANGE_ALL,
    },
    // Fever
    [ABILITY_341] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_MARS,
        .effect = ABILITY_EFFECT_DELUSION,
        .range = 1,
        .power = 30,
    },
    // Corona
    [ABILITY_342] = {
        .targetMode = ABILITY_TARGET_ALLIES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_EFFECT,
        .element = ELEMENT_MARS,
        .effect = ABILITY_EFFECT_DEFENSE_UP_2,
        .range = ABILITY_RANGE_ALL,
    },
    // Scorch
    [ABILITY_343] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_MARS,
        .effect = ABILITY_EFFECT_STUN,
        .range = 1,
        .power = 50,
    },
    // Ember
    [ABILITY_344] = {
        .targetMode = ABILITY_TARGET_ALLIES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_HEAL_PP,
        .element = ELEMENT_MARS,
        .effect = ABILITY_EFFECT_HEAL_PP_7,
        .range = ABILITY_RANGE_ALL,
    },
    // Flash
    [ABILITY_345] = {
        .targetMode = ABILITY_TARGET_ALLIES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_EFFECT_ONLY,
        .element = ELEMENT_MARS,
        .effect = ABILITY_EFFECT_TOTAL_DEFENSE,
        .range = ABILITY_RANGE_ALL,
    },
    // Torch
    [ABILITY_346] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_MARS,
        .effect = ABILITY_EFFECT_HALVE_DEFENSE,
        .range = 1,
        .power = 30,
    },
    // =
    [ABILITY_347] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_VENUS,
        .range = 1,
    },
    // =
    [ABILITY_348] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_VENUS,
        .range = 1,
    },
    // =
    [ABILITY_349] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_VENUS,
        .range = 1,
    },
    // =
    [ABILITY_350] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_VENUS,
        .range = 1,
    },
    // =
    [ABILITY_351] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_VENUS,
        .range = 1,
    },
    // =
    [ABILITY_352] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_VENUS,
        .range = 1,
    },
    // =
    [ABILITY_353] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_VENUS,
        .range = 1,
    },
    // =
    [ABILITY_354] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_VENUS,
        .range = 1,
    },
    // =
    [ABILITY_355] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_VENUS,
        .range = 1,
    },
    // =
    [ABILITY_356] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_VENUS,
        .range = 1,
    },
    // =
    [ABILITY_357] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_VENUS,
        .range = 1,
    },
    // =
    [ABILITY_358] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_VENUS,
        .range = 1,
    },
    // =
    [ABILITY_359] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_VENUS,
        .range = 1,
    },
    // Gust
    [ABILITY_360] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_JUPITER,
        .effect = ABILITY_EFFECT_DOUBLE_BLOW_60,
        .range = 1,
        .power = 10,
    },
    // Breeze
    [ABILITY_361] = {
        .targetMode = ABILITY_TARGET_ALLIES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_EFFECT,
        .element = ELEMENT_JUPITER,
        .effect = ABILITY_EFFECT_RESISTANCE_UP_2,
        .range = ABILITY_RANGE_ALL,
    },
    // Zephyr
    [ABILITY_362] = {
        .targetMode = ABILITY_TARGET_ALLIES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_EFFECT,
        .element = ELEMENT_JUPITER,
        .effect = ABILITY_EFFECT_AGILITY_UP,
        .range = ABILITY_RANGE_ALL,
    },
    // Smog
    [ABILITY_363] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_JUPITER,
        .effect = ABILITY_EFFECT_DELUSION,
        .range = 1,
        .power = 60,
    },
    // Kite
    [ABILITY_364] = {
        .targetMode = ABILITY_TARGET_ALLIES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_EFFECT_ONLY,
        .element = ELEMENT_JUPITER,
        .effect = ABILITY_EFFECT_ACT_TWICE,
        .range = 1,
    },
    // Squall
    [ABILITY_365] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK_MULTIPLIED,
        .element = ELEMENT_JUPITER,
        .effect = ABILITY_EFFECT_STUN,
        .range = 1,
        .power = 14,
    },
    // Luff
    [ABILITY_366] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_EFFECT,
        .element = ELEMENT_JUPITER,
        .effect = ABILITY_EFFECT_SEAL_2_TURNS,
        .range = 1,
    },
    // =
    [ABILITY_367] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_VENUS,
        .range = 1,
    },
    // =
    [ABILITY_368] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_VENUS,
        .range = 1,
    },
    // =
    [ABILITY_369] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_VENUS,
        .range = 1,
    },
    // =
    [ABILITY_370] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_VENUS,
        .range = 1,
    },
    // =
    [ABILITY_371] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_VENUS,
        .range = 1,
    },
    // =
    [ABILITY_372] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_VENUS,
        .range = 1,
    },
    // =
    [ABILITY_373] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_VENUS,
        .range = 1,
    },
    // =
    [ABILITY_374] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_VENUS,
        .range = 1,
    },
    // =
    [ABILITY_375] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_VENUS,
        .range = 1,
    },
    // =
    [ABILITY_376] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_VENUS,
        .range = 1,
    },
    // =
    [ABILITY_377] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_VENUS,
        .range = 1,
    },
    // =
    [ABILITY_378] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_VENUS,
        .range = 1,
    },
    // =
    [ABILITY_379] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_VENUS,
        .range = 1,
    },
    [ABILITY_VENUS] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_SUMMON,
        .element = ELEMENT_VENUS,
        .icon = 140,
        .range = 6,
        .power = 30,
    },
    [ABILITY_RAMSES] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_SUMMON,
        .element = ELEMENT_VENUS,
        .icon = 141,
        .range = 6,
        .power = 60,
    },
    [ABILITY_CYBELE] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_SUMMON,
        .element = ELEMENT_VENUS,
        .icon = 142,
        .range = 6,
        .power = 120,
    },
    [ABILITY_JUDGMENT] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_SUMMON,
        .element = ELEMENT_VENUS,
        .icon = 143,
        .range = 6,
        .power = 240,
    },
    // ?
    [ABILITY_384] = {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    [ABILITY_385] = {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    [ABILITY_MERCURY] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_SUMMON,
        .element = ELEMENT_MERCURY,
        .icon = 144,
        .range = 6,
        .power = 30,
    },
    [ABILITY_NEREID] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_SUMMON,
        .element = ELEMENT_MERCURY,
        .icon = 145,
        .range = 6,
        .power = 60,
    },
    [ABILITY_NEPTUNE] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_SUMMON,
        .element = ELEMENT_MERCURY,
        .icon = 146,
        .range = 6,
        .power = 120,
    },
    [ABILITY_BOREAS] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_SUMMON,
        .element = ELEMENT_MERCURY,
        .icon = 147,
        .range = 6,
        .power = 240,
    },
    // ?
    [ABILITY_390] = {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    [ABILITY_391] = {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    [ABILITY_MARS] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_SUMMON,
        .element = ELEMENT_MARS,
        .icon = 148,
        .range = 6,
        .power = 30,
    },
    [ABILITY_KIRIN] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_SUMMON,
        .element = ELEMENT_MARS,
        .icon = 149,
        .range = 6,
        .power = 60,
    },
    [ABILITY_TIAMAT] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_SUMMON,
        .element = ELEMENT_MARS,
        .icon = 150,
        .range = 6,
        .power = 120,
    },
    [ABILITY_METEOR] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_SUMMON,
        .element = ELEMENT_MARS,
        .icon = 151,
        .range = 6,
        .power = 240,
    },
    // ?
    [ABILITY_396] = {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    [ABILITY_397] = {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    [ABILITY_JUPITER] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_SUMMON,
        .element = ELEMENT_JUPITER,
        .icon = 152,
        .range = 6,
        .power = 30,
    },
    [ABILITY_ATALANTA] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_SUMMON,
        .element = ELEMENT_JUPITER,
        .icon = 153,
        .range = 6,
        .power = 60,
    },
    [ABILITY_PROCNE] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_SUMMON,
        .element = ELEMENT_JUPITER,
        .icon = 154,
        .range = 6,
        .power = 120,
    },
    [ABILITY_THOR] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_SUMMON,
        .element = ELEMENT_JUPITER,
        .icon = 155,
        .range = 6,
        .power = 240,
    },
    // ?
    [ABILITY_402] = {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    [ABILITY_403] = {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    [ABILITY_404] = {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    [ABILITY_405] = {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    [ABILITY_406] = {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    [ABILITY_407] = {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    [ABILITY_408] = {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    [ABILITY_409] = {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    [ABILITY_410] = {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    [ABILITY_411] = {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    [ABILITY_412] = {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    [ABILITY_413] = {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    [ABILITY_414] = {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    [ABILITY_415] = {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    [ABILITY_416] = {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    [ABILITY_417] = {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    [ABILITY_418] = {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    [ABILITY_419] = {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // Fire Blessing
    [ABILITY_420] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ENEMY_DAMAGE,
        .element = ELEMENT_MARS,
        .range = 2,
        .power = 60,
    },
    // Fire Blessing
    [ABILITY_421] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ENEMY_DAMAGE,
        .element = ELEMENT_MARS,
        .range = 2,
        .power = 90,
    },
    // Fire Blessing
    [ABILITY_422] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ENEMY_DAMAGE,
        .element = ELEMENT_MARS,
        .range = 2,
        .power = 130,
    },
    // Water Blessing
    [ABILITY_423] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ENEMY_DAMAGE,
        .element = ELEMENT_MERCURY,
        .range = 2,
        .power = 25,
    },
    // Water Blessing
    [ABILITY_424] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ENEMY_DAMAGE,
        .element = ELEMENT_MERCURY,
        .range = 2,
        .power = 50,
    },
    // Water Blessing
    [ABILITY_425] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ENEMY_DAMAGE,
        .element = ELEMENT_MERCURY,
        .range = 2,
        .power = 100,
    },
    // Ice Blessing
    [ABILITY_426] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ENEMY_DAMAGE,
        .element = ELEMENT_MERCURY,
        .range = 2,
        .power = 50,
    },
    // Ice Blessing
    [ABILITY_427] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ENEMY_DAMAGE,
        .element = ELEMENT_MERCURY,
        .range = 2,
        .power = 90,
    },
    // Ice Blessing
    [ABILITY_428] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ENEMY_DAMAGE,
        .element = ELEMENT_MERCURY,
        .range = 2,
        .power = 180,
    },
    // Dark Blessing
    [ABILITY_429] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ENEMY_DAMAGE,
        .element = ELEMENT_VENUS,
        .effect = ABILITY_EFFECT_DELUSION,
        .range = 2,
        .power = 70,
    },
    // Dark Blessing
    [ABILITY_430] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ENEMY_DAMAGE,
        .element = ELEMENT_VENUS,
        .effect = ABILITY_EFFECT_DELUSION,
        .range = 2,
        .power = 110,
    },
    // Dark Blessing
    [ABILITY_431] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ENEMY_DAMAGE,
        .element = ELEMENT_MARS,
        .effect = ABILITY_EFFECT_DEFENSE_DOWN,
        .range = 2,
        .power = 170,
    },
    // Acid Blessing
    [ABILITY_432] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ENEMY_DAMAGE,
        .element = ELEMENT_MARS,
        .effect = ABILITY_EFFECT_DEFENSE_DOWN,
        .range = 2,
        .power = 190,
    },
    // Storm Blessing
    [ABILITY_433] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ENEMY_DAMAGE,
        .element = ELEMENT_JUPITER,
        .effect = ABILITY_EFFECT_STUN,
        .range = 2,
        .power = 190,
    },
    // Storm Blessing
    [ABILITY_434] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ENEMY_DAMAGE,
        .element = ELEMENT_JUPITER,
        .effect = ABILITY_EFFECT_STUN,
        .range = 2,
        .power = 220,
    },
    [ABILITY_SONIC_WAVE] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ENEMY_DAMAGE,
        .element = ELEMENT_JUPITER,
        .range = 1,
        .power = 5,
    },
    // Sonic Wave
    [ABILITY_SONIC_WAVE_STRONG] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ENEMY_DAMAGE,
        .element = ELEMENT_JUPITER,
        .range = 1,
        .power = 45,
    },
    [ABILITY_SHRIEK] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ENEMY_DAMAGE,
        .element = ELEMENT_JUPITER,
        .range = 1,
        .power = 30,
    },
    [ABILITY_BANSHEE_HOWL] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ENEMY_DAMAGE,
        .element = ELEMENT_JUPITER,
        .effect = ABILITY_EFFECT_STUN,
        .range = 1,
        .power = 70,
    },
    [ABILITY_CRAZY_VOICE] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_PP_DAMAGE,
        .element = ELEMENT_JUPITER,
        .effect = ABILITY_EFFECT_CRAZY_VOICE,
        .range = 1,
        .power = 10,
    },
    [ABILITY_WAR_CRY] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ENEMY_DAMAGE,
        .element = ELEMENT_JUPITER,
        .effect = ABILITY_EFFECT_GROUND,
        .range = 1,
        .power = 45,
    },
    [ABILITY_WICKED_HOWL] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ENEMY_DAMAGE,
        .element = ELEMENT_JUPITER,
        .effect = ABILITY_EFFECT_GROUND,
        .range = 1,
        .power = 60,
    },
    [ABILITY_WING_BEAT] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ENEMY_DAMAGE,
        .element = ELEMENT_JUPITER,
        .range = 1,
        .power = 35,
    },
    [ABILITY_WING_FLUTTER] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ENEMY_DAMAGE,
        .element = ELEMENT_JUPITER,
        .range = 2,
        .power = 70,
    },
    [ABILITY_WING_STROKE] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ENEMY_DAMAGE,
        .element = ELEMENT_JUPITER,
        .range = 2,
        .power = 140,
    },
    // Evil Blessing
    [ABILITY_445] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ENEMY_DAMAGE,
        .element = ELEMENT_MARS,
        .effect = ABILITY_EFFECT_HAUNT,
        .range = 2,
        .power = 80,
    },
    // Deadly Gas
    [ABILITY_446] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ENEMY_DAMAGE,
        .element = ELEMENT_VENUS,
        .effect = ABILITY_EFFECT_VENOM,
        .range = 2,
        .power = 30,
    },
    // ?
    [ABILITY_447] = {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    [ABILITY_448] = {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // ?
    [ABILITY_449] = {
        .targetMode = ABILITY_TARGET_ONE,
        .element = ELEMENT_VENUS,
    },
    // Rumble
    [ABILITY_450] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_VENUS,
        .range = 1,
        .power = 9,
    },
    // Bone Chiller
    [ABILITY_451] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_MERCURY,
        .range = 1,
        .power = 7,
    },
    // Slice
    [ABILITY_452] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK_MULTIPLIED,
        .element = ELEMENT_JUPITER,
        .effect = ABILITY_EFFECT_DOUBLE_BLOW_60,
        .range = 1,
        .power = 9,
    },
    // Bone Charge
    [ABILITY_453] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK_MULTIPLIED,
        .element = ELEMENT_VENUS,
        .range = 1,
        .power = 17,
    },
    // Mystic Flame
    [ABILITY_454] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_MARS,
        .range = 1,
        .power = 16,
    },
    // Numbing Sting
    [ABILITY_455] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK_MULTIPLIED,
        .element = ELEMENT_JUPITER,
        .effect = ABILITY_EFFECT_STUN,
        .range = 1,
        .power = 8,
    },
    // Brute Force
    [ABILITY_456] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_VENUS,
        .range = 1,
        .power = 22,
    },
    // Sticky Goo
    [ABILITY_457] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_MERCURY,
        .effect = ABILITY_EFFECT_DEFENSE_DOWN,
        .range = 1,
        .power = 13,
    },
    // Cannibal Fang
    [ABILITY_458] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_EFFECT,
        .element = ELEMENT_VENUS,
        .effect = ABILITY_EFFECT_DRAIN_HP,
        .range = 1,
        .power = 24,
    },
    // Bear Claw
    [ABILITY_459] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK_MULTIPLIED,
        .element = ELEMENT_NONE,
        .range = 1,
        .power = 16,
    },
    // Poisonous Bite
    [ABILITY_460] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_VENUS,
        .effect = ABILITY_EFFECT_POISON,
        .range = 1,
    },
    // Flying Attack
    [ABILITY_461] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_JUPITER,
        .effect = ABILITY_EFFECT_DOUBLE_BLOW_1_IN_4,
        .range = 1,
        .power = 8,
    },
    // Undead Sword
    [ABILITY_462] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_VENUS,
        .range = 1,
        .power = 15,
    },
    // Ransack
    [ABILITY_463] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_VENUS,
        .range = 1,
        .power = 35,
    },
    // Sticky Poison
    [ABILITY_464] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_MERCURY,
        .effect = ABILITY_EFFECT_POISON,
        .range = 1,
        .power = 16,
    },
    // Poison Fang
    [ABILITY_465] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_VENUS,
        .effect = ABILITY_EFFECT_POISON,
        .range = 1,
    },
    // Electric Bite
    [ABILITY_466] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_JUPITER,
        .effect = ABILITY_EFFECT_STUN,
        .range = 1,
    },
    // Poison Tail
    [ABILITY_467] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_VENUS,
        .effect = ABILITY_EFFECT_VENOM,
        .range = 1,
        .power = 30,
    },
    // Onslaught
    [ABILITY_468] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_VENUS,
        .range = 1,
        .power = 24,
    },
    // Vampiric Fang
    [ABILITY_469] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_EFFECT,
        .element = ELEMENT_VENUS,
        .effect = ABILITY_EFFECT_DRAIN_HP,
        .range = 1,
        .power = 35,
    },
    // Bacteria Rush
    [ABILITY_470] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_VENUS,
        .effect = ABILITY_EFFECT_POISON,
        .range = 1,
        .power = 16,
    },
    // Swift Strike
    [ABILITY_471] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_JUPITER,
        .effect = ABILITY_EFFECT_TRIPLE_BLOW_1_IN_4,
        .range = 1,
        .power = 15,
    },
    [ABILITY_ROTTEN_BLOOD] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_MARS,
        .effect = ABILITY_EFFECT_DEFENSE_DOWN_2,
        .range = 1,
        .power = 44,
    },
    // Forcible Arm
    [ABILITY_473] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_VENUS,
        .range = 1,
        .power = 40,
    },
    // Double Fang
    [ABILITY_474] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_VENUS,
        .effect = ABILITY_EFFECT_DOUBLE_BLOW_1_IN_4,
        .range = 1,
        .power = 35,
    },
    // Mortal Blow
    [ABILITY_475] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_VENUS,
        .effect = ABILITY_EFFECT_SELF_DESTRUCT,
        .range = 1,
        .power = 35,
    },
    // Freebite Rush
    [ABILITY_476] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_VENUS,
        .effect = ABILITY_EFFECT_ATTACK_DOWN,
        .range = 1,
        .power = 40,
    },
    // Twin Beaks
    [ABILITY_477] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_JUPITER,
        .effect = ABILITY_EFFECT_DOUBLE_BLOW_1_IN_4,
        .range = 1,
        .power = 30,
    },
    // Rabid Fang
    [ABILITY_478] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_VENUS,
        .effect = ABILITY_EFFECT_VENOM,
        .range = 1,
        .power = 36,
    },
    // Acid Bite
    [ABILITY_479] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_MARS,
        .effect = ABILITY_EFFECT_DEFENSE_DOWN,
        .range = 1,
        .power = 36,
    },
    // Dynamite
    [ABILITY_480] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_VENUS,
        .effect = ABILITY_EFFECT_SELF_DESTRUCT,
        .range = 2,
        .power = 200,
    },
    // Headbutt
    [ABILITY_481] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_VENUS,
        .range = 1,
        .power = 60,
    },
    // Poison Ink
    [ABILITY_482] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_VENUS,
        .effect = ABILITY_EFFECT_VENOM,
        .range = 1,
    },
    // Truncheon Fist
    [ABILITY_483] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_VENUS,
        .effect = ABILITY_EFFECT_LEAVE_ONE_HP,
        .range = 1,
        .power = 60,
    },
    // Counterstrike
    [ABILITY_484] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_VENUS,
        .range = 1,
        .power = 50,
    },
    // Mad Dash
    [ABILITY_485] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_VENUS,
        .range = 1,
        .power = 70,
    },
    // Soothing Star
    [ABILITY_486] = {
        .targetMode = ABILITY_TARGET_ALLIES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_HEAL_HP,
        .element = ELEMENT_VENUS,
        .range = 2,
        .power = 10,
    },
    // Spider Web
    [ABILITY_487] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_EFFECT,
        .element = ELEMENT_VENUS,
        .effect = ABILITY_EFFECT_DEFENSE_DOWN_2,
        .range = 1,
    },
    [ABILITY_HEARTRENDER] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_EFFECT,
        .element = ELEMENT_NONE,
        .effect = ABILITY_EFFECT_ATTACK_DOWN_2,
        .range = 1,
    },
    // Mad Spatter
    [ABILITY_489] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_EFFECT,
        .element = ELEMENT_VENUS,
        .effect = ABILITY_EFFECT_DELUSION,
        .range = 2,
    },
    // Spasm
    [ABILITY_490] = {
        .targetMode = ABILITY_TARGET_SELF,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_HEAL_HP,
        .element = ELEMENT_VENUS,
        .range = 1,
        .power = 40,
    },
    // Sleep Star
    [ABILITY_491] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_EFFECT,
        .element = ELEMENT_JUPITER,
        .effect = ABILITY_EFFECT_SLEEP,
        .range = 2,
    },
    [ABILITY_DECOMPOSE] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_EFFECT,
        .element = ELEMENT_VENUS,
        .effect = ABILITY_EFFECT_ATTACK_DOWN,
        .range = 2,
    },
    // Haunting
    [ABILITY_493] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_EFFECT,
        .element = ELEMENT_VENUS,
        .effect = ABILITY_EFFECT_HAUNT,
        .range = 1,
    },
    [ABILITY_WORMS] = {
        .targetMode = ABILITY_TARGET_SELF,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_HEAL_HP,
        .element = ELEMENT_VENUS,
        .range = 1,
        .power = 50,
    },
    [ABILITY_BERSERK] = {
        .targetMode = ABILITY_TARGET_SELF,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_EFFECT,
        .element = ELEMENT_VENUS,
        .effect = ABILITY_EFFECT_ATTACK_UP_2,
        .range = 1,
    },
    // Lucid Prophecy
    [ABILITY_496] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_EFFECT,
        .element = ELEMENT_VENUS,
        .effect = ABILITY_EFFECT_CURSE,
        .range = 1,
    },
    // Recovery
    [ABILITY_497] = {
        .targetMode = ABILITY_TARGET_SELF,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_HEAL_HP,
        .element = ELEMENT_NONE,
        .range = 1,
        .power = 150,
    },
    // Flee
    [ABILITY_498] = {
        .targetMode = ABILITY_TARGET_ONE,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_NO_POWER,
        .element = ELEMENT_NONE,
        .effect = ABILITY_EFFECT_FLEE,
        .range = 1,
    },
    [ABILITY_CONTAIN] = {
        .targetMode = ABILITY_TARGET_ONE,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_NO_POWER,
        .element = ELEMENT_NONE,
        .effect = ABILITY_EFFECT_THREATEN,
        .range = 1,
    },
    [ABILITY_THREATEN] = {
        .targetMode = ABILITY_TARGET_ONE,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_NO_POWER,
        .element = ELEMENT_NONE,
        .effect = ABILITY_EFFECT_THREATEN,
        .range = 1,
    },
    [ABILITY_TREMBLE] = {
        .targetMode = ABILITY_TARGET_ONE,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_NO_POWER,
        .element = ELEMENT_NONE,
        .effect = ABILITY_EFFECT_THREATEN,
        .range = 1,
    },
    // Fortify
    [ABILITY_502] = {
        .targetMode = ABILITY_TARGET_SELF,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_NO_POWER,
        .element = ELEMENT_NONE,
        .effect = ABILITY_EFFECT_DEFEND,
        .range = 1,
    },
    [ABILITY_SPEED_SURGE] = {
        .targetMode = ABILITY_TARGET_ONE,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_NO_POWER,
        .element = ELEMENT_NONE,
        .effect = ABILITY_EFFECT_CALL_ALLY,
        .range = 1,
    },
    [ABILITY_ALLY_SEARCH] = {
        .targetMode = ABILITY_TARGET_ONE,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_NO_POWER,
        .element = ELEMENT_NONE,
        .effect = ABILITY_EFFECT_CALL_ALLY,
        .range = 1,
    },
    // Sidestep
    [ABILITY_505] = {
        .targetMode = ABILITY_TARGET_SELF,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_NO_POWER,
        .element = ELEMENT_NONE,
        .effect = ABILITY_EFFECT_SIDESTEP,
        .range = 1,
    },
    // Total Defense
    [ABILITY_506] = {
        .targetMode = ABILITY_TARGET_SELF,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_NO_POWER,
        .element = ELEMENT_NONE,
        .effect = ABILITY_EFFECT_TOTAL_DEFENSE,
        .range = 1,
    },
    // Stand Ready
    [ABILITY_507] = {
        .targetMode = ABILITY_TARGET_SELF,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_NO_POWER,
        .element = ELEMENT_NONE,
        .effect = ABILITY_EFFECT_DEFEND,
        .range = 1,
    },
    [ABILITY_SEARCH_FOR_HELP] = {
        .targetMode = ABILITY_TARGET_ONE,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_NO_POWER,
        .element = ELEMENT_NONE,
        .effect = ABILITY_EFFECT_CALL_HELP,
        .range = 1,
    },
    [ABILITY_CANT_USE] = {
        .targetMode = ABILITY_TARGET_ONE,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE,
        .element = ELEMENT_VENUS,
        .range = 1,
    },
    // Poison Beat
    [ABILITY_510] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_MERCURY,
        .effect = ABILITY_EFFECT_POISON,
        .range = 1,
        .power = 20,
    },
    // Spinning Beat
    [ABILITY_511] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_MERCURY,
        .effect = ABILITY_EFFECT_HALVE_DEFENSE,
        .range = 1,
        .power = 40,
    },
    // Heat Flash
    [ABILITY_512] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_MARS,
        .effect = ABILITY_EFFECT_DELUSION,
        .range = 1,
        .power = 20,
    },
    // Death Size
    [ABILITY_513] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_VENUS,
        .effect = ABILITY_EFFECT_INSTANT_DEATH,
        .range = 1,
        .power = 35,
    },
    // Outer Space
    [ABILITY_514] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_SUMMON,
        .element = ELEMENT_MARS,
        .range = 6,
        .power = 200,
    },
    // Dragon Driver
    [ABILITY_515] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_SUMMON,
        .element = ELEMENT_MARS,
        .range = 6,
        .power = 120,
    },
    // Drain Fang
    [ABILITY_516] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_EFFECT,
        .element = ELEMENT_VENUS,
        .effect = ABILITY_EFFECT_DRAIN_HP,
        .range = 1,
        .power = 80,
    },
    [ABILITY_SEVERE_BLOW] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK_MULTIPLIED,
        .element = ELEMENT_VENUS,
        .effect = ABILITY_EFFECT_LEAVE_ONE_HP,
        .range = 1,
        .power = 12,
    },
    [ABILITY_THRASH] = {
        .targetMode = ABILITY_TARGET_FOES,
        .flags = ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_ATTACK,
        .element = ELEMENT_MERCURY,
        .effect = ABILITY_EFFECT_LEAVE_ONE_HP,
        .range = 1,
        .power = 30,
    },
};
// clang-format on
