/// @file
/// The 80 Djinn, 20 of each element in element order. Each name is the
/// English release's.

#include "game.h"
#include "ability_ids.h"

// clang-format off
const struct DjinnDefinition gDjinn_DefinitionTable[] = {
    // Flint
    {
        .abilityId = ABILITY_DJINNI_FIRST,
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
    // =
    {
        .abilityId = 307,
    },
    // =
    {
        .abilityId = 308,
    },
    // =
    {
        .abilityId = 309,
    },
    // =
    {
        .abilityId = 310,
    },
    // =
    {
        .abilityId = 311,
    },
    // =
    {
        .abilityId = 312,
    },
    // =
    {
        .abilityId = 313,
    },
    // =
    {
        .abilityId = 314,
    },
    // =
    {
        .abilityId = 315,
    },
    // =
    {
        .abilityId = 316,
    },
    // =
    {
        .abilityId = 317,
    },
    // =
    {
        .abilityId = 318,
    },
    // =
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
    // =
    {
        .abilityId = 327,
    },
    // =
    {
        .abilityId = 328,
    },
    // =
    {
        .abilityId = 329,
    },
    // =
    {
        .abilityId = 330,
    },
    // =
    {
        .abilityId = 331,
    },
    // =
    {
        .abilityId = 332,
    },
    // =
    {
        .abilityId = 333,
    },
    // =
    {
        .abilityId = 334,
    },
    // =
    {
        .abilityId = 335,
    },
    // =
    {
        .abilityId = 336,
    },
    // =
    {
        .abilityId = 337,
    },
    // =
    {
        .abilityId = 338,
    },
    // =
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
    // =
    {
        .abilityId = 347,
    },
    // =
    {
        .abilityId = 348,
    },
    // =
    {
        .abilityId = 349,
    },
    // =
    {
        .abilityId = 350,
    },
    // =
    {
        .abilityId = 351,
    },
    // =
    {
        .abilityId = 352,
    },
    // =
    {
        .abilityId = 353,
    },
    // =
    {
        .abilityId = 354,
    },
    // =
    {
        .abilityId = 355,
    },
    // =
    {
        .abilityId = 356,
    },
    // =
    {
        .abilityId = 357,
    },
    // =
    {
        .abilityId = 358,
    },
    // =
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
    // =
    {
        .abilityId = 367,
    },
    // =
    {
        .abilityId = 368,
    },
    // =
    {
        .abilityId = 369,
    },
    // =
    {
        .abilityId = 370,
    },
    // =
    {
        .abilityId = 371,
    },
    // =
    {
        .abilityId = 372,
    },
    // =
    {
        .abilityId = 373,
    },
    // =
    {
        .abilityId = 374,
    },
    // =
    {
        .abilityId = 375,
    },
    // =
    {
        .abilityId = 376,
    },
    // =
    {
        .abilityId = 377,
    },
    // =
    {
        .abilityId = 378,
    },
    // =
    {
        .abilityId = 379,
    },
};
// clang-format on
