/// @file
/// The 80 Djinn, 20 of each element in element order. Each name is the
/// English release's (MsgDjinnName plus the Djinni's place).
// Part of game.c.

#include "../game_internal.h"

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
