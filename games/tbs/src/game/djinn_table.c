/// @file
/// The 80 Djinn, 20 of each element in element order. Each name is the
/// English release's.

#include "game.h"
#include "ability_ids.h"
#include "djinn_ids.h"

// clang-format off
const struct DjinnDefinition gDjinn_DefinitionTable[] = {
    [DJINNI_FLINT] = {
        .abilityId = ABILITY_DJINNI_FIRST,
        .hp = 8,
        .pp = 4,
        .atk = 3,
    },
    [DJINNI_GRANITE] = {
        .abilityId = 301,
        .hp = 9,
        .def = 2,
        .agi = 2,
        .luck = 1,
    },
    [DJINNI_QUARTZ] = {
        .abilityId = 302,
        .hp = 10,
        .pp = 3,
        .agi = 3,
    },
    [DJINNI_VINE] = {
        .abilityId = 303,
        .hp = 12,
        .pp = 4,
        .def = 3,
        .luck = 1,
    },
    [DJINNI_SAP] = {
        .abilityId = 304,
        .hp = 10,
        .atk = 3,
        .luck = 1,
    },
    [DJINNI_GROUND] = {
        .abilityId = 305,
        .hp = 9,
        .pp = 3,
        .agi = 3,
    },
    [DJINNI_BANE] = {
        .abilityId = 306,
        .hp = 12,
        .atk = 4,
    },
    // =
    [DJINNI_07] = {
        .abilityId = 307,
    },
    // =
    [DJINNI_08] = {
        .abilityId = 308,
    },
    // =
    [DJINNI_09] = {
        .abilityId = 309,
    },
    // =
    [DJINNI_10] = {
        .abilityId = 310,
    },
    // =
    [DJINNI_11] = {
        .abilityId = 311,
    },
    // =
    [DJINNI_12] = {
        .abilityId = 312,
    },
    // =
    [DJINNI_13] = {
        .abilityId = 313,
    },
    // =
    [DJINNI_14] = {
        .abilityId = 314,
    },
    // =
    [DJINNI_15] = {
        .abilityId = 315,
    },
    // =
    [DJINNI_16] = {
        .abilityId = 316,
    },
    // =
    [DJINNI_17] = {
        .abilityId = 317,
    },
    // =
    [DJINNI_18] = {
        .abilityId = 318,
    },
    // =
    [DJINNI_19] = {
        .abilityId = 319,
    },
    [DJINNI_FIZZ] = {
        .abilityId = 320,
        .hp = 9,
        .pp = 4,
        .def = 3,
    },
    [DJINNI_SLEET] = {
        .abilityId = 321,
        .hp = 12,
        .atk = 3,
        .luck = 1,
    },
    [DJINNI_MIST] = {
        .abilityId = 322,
        .hp = 11,
        .atk = 4,
    },
    [DJINNI_SPRITZ] = {
        .abilityId = 323,
        .hp = 8,
        .pp = 4,
        .agi = 3,
    },
    [DJINNI_HAIL] = {
        .abilityId = 324,
        .hp = 9,
        .atk = 4,
        .luck = 1,
    },
    [DJINNI_TONIC] = {
        .abilityId = 325,
        .hp = 8,
        .pp = 3,
        .def = 2,
        .luck = 2,
    },
    [DJINNI_DEW] = {
        .abilityId = 326,
        .hp = 13,
        .pp = 4,
        .agi = 4,
    },
    // =
    [DJINNI_27] = {
        .abilityId = 327,
    },
    // =
    [DJINNI_28] = {
        .abilityId = 328,
    },
    // =
    [DJINNI_29] = {
        .abilityId = 329,
    },
    // =
    [DJINNI_30] = {
        .abilityId = 330,
    },
    // =
    [DJINNI_31] = {
        .abilityId = 331,
    },
    // =
    [DJINNI_32] = {
        .abilityId = 332,
    },
    // =
    [DJINNI_33] = {
        .abilityId = 333,
    },
    // =
    [DJINNI_34] = {
        .abilityId = 334,
    },
    // =
    [DJINNI_35] = {
        .abilityId = 335,
    },
    // =
    [DJINNI_36] = {
        .abilityId = 336,
    },
    // =
    [DJINNI_37] = {
        .abilityId = 337,
    },
    // =
    [DJINNI_38] = {
        .abilityId = 338,
    },
    // =
    [DJINNI_39] = {
        .abilityId = 339,
    },
    [DJINNI_FORGE] = {
        .abilityId = 340,
        .hp = 10,
        .atk = 2,
        .agi = 2,
        .luck = 2,
    },
    [DJINNI_FEVER] = {
        .abilityId = 341,
        .hp = 8,
        .atk = 3,
        .agi = 2,
    },
    [DJINNI_CORONA] = {
        .abilityId = 342,
        .hp = 12,
        .pp = 3,
        .def = 3,
        .luck = 1,
    },
    [DJINNI_SCORCH] = {
        .abilityId = 343,
        .hp = 8,
        .atk = 3,
    },
    [DJINNI_EMBER] = {
        .abilityId = 344,
        .hp = 9,
        .pp = 4,
        .def = 2,
        .agi = 2,
    },
    [DJINNI_FLASH] = {
        .abilityId = 345,
        .hp = 14,
        .pp = 3,
        .def = 2,
    },
    [DJINNI_TORCH] = {
        .abilityId = 346,
        .hp = 9,
        .atk = 3,
        .luck = 1,
    },
    // =
    [DJINNI_47] = {
        .abilityId = 347,
    },
    // =
    [DJINNI_48] = {
        .abilityId = 348,
    },
    // =
    [DJINNI_49] = {
        .abilityId = 349,
    },
    // =
    [DJINNI_50] = {
        .abilityId = 350,
    },
    // =
    [DJINNI_51] = {
        .abilityId = 351,
    },
    // =
    [DJINNI_52] = {
        .abilityId = 352,
    },
    // =
    [DJINNI_53] = {
        .abilityId = 353,
    },
    // =
    [DJINNI_54] = {
        .abilityId = 354,
    },
    // =
    [DJINNI_55] = {
        .abilityId = 355,
    },
    // =
    [DJINNI_56] = {
        .abilityId = 356,
    },
    // =
    [DJINNI_57] = {
        .abilityId = 357,
    },
    // =
    [DJINNI_58] = {
        .abilityId = 358,
    },
    // =
    [DJINNI_59] = {
        .abilityId = 359,
    },
    [DJINNI_GUST] = {
        .abilityId = 360,
        .hp = 9,
        .atk = 2,
        .agi = 2,
    },
    [DJINNI_BREEZE] = {
        .abilityId = 361,
        .hp = 12,
        .pp = 5,
        .def = 2,
        .luck = 1,
    },
    [DJINNI_ZEPHYR] = {
        .abilityId = 362,
        .hp = 11,
        .pp = 3,
        .agi = 2,
        .luck = 1,
    },
    [DJINNI_SMOG] = {
        .abilityId = 363,
        .hp = 9,
        .atk = 3,
    },
    [DJINNI_KITE] = {
        .abilityId = 364,
        .hp = 8,
        .pp = 4,
        .agi = 3,
    },
    [DJINNI_SQUALL] = {
        .abilityId = 365,
        .hp = 10,
        .atk = 5,
    },
    [DJINNI_LUFF] = {
        .abilityId = 366,
        .hp = 11,
        .pp = 5,
        .def = 2,
        .luck = 1,
    },
    // =
    [DJINNI_67] = {
        .abilityId = 367,
    },
    // =
    [DJINNI_68] = {
        .abilityId = 368,
    },
    // =
    [DJINNI_69] = {
        .abilityId = 369,
    },
    // =
    [DJINNI_70] = {
        .abilityId = 370,
    },
    // =
    [DJINNI_71] = {
        .abilityId = 371,
    },
    // =
    [DJINNI_72] = {
        .abilityId = 372,
    },
    // =
    [DJINNI_73] = {
        .abilityId = 373,
    },
    // =
    [DJINNI_74] = {
        .abilityId = 374,
    },
    // =
    [DJINNI_75] = {
        .abilityId = 375,
    },
    // =
    [DJINNI_76] = {
        .abilityId = 376,
    },
    // =
    [DJINNI_77] = {
        .abilityId = 377,
    },
    // =
    [DJINNI_78] = {
        .abilityId = 378,
    },
    // =
    [DJINNI_79] = {
        .abilityId = 379,
    },
};
// clang-format on
