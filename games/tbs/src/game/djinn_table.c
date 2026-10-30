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
        .abilityId = ABILITY_301,
        .hp = 9,
        .def = 2,
        .agi = 2,
        .luck = 1,
    },
    [DJINNI_QUARTZ] = {
        .abilityId = ABILITY_302,
        .hp = 10,
        .pp = 3,
        .agi = 3,
    },
    [DJINNI_VINE] = {
        .abilityId = ABILITY_303,
        .hp = 12,
        .pp = 4,
        .def = 3,
        .luck = 1,
    },
    [DJINNI_SAP] = {
        .abilityId = ABILITY_304,
        .hp = 10,
        .atk = 3,
        .luck = 1,
    },
    [DJINNI_GROUND] = {
        .abilityId = ABILITY_305,
        .hp = 9,
        .pp = 3,
        .agi = 3,
    },
    [DJINNI_BANE] = {
        .abilityId = ABILITY_306,
        .hp = 12,
        .atk = 4,
    },
    // =
    [DJINNI_07] = {
        .abilityId = ABILITY_307,
    },
    // =
    [DJINNI_08] = {
        .abilityId = ABILITY_308,
    },
    // =
    [DJINNI_09] = {
        .abilityId = ABILITY_309,
    },
    // =
    [DJINNI_10] = {
        .abilityId = ABILITY_310,
    },
    // =
    [DJINNI_11] = {
        .abilityId = ABILITY_311,
    },
    // =
    [DJINNI_12] = {
        .abilityId = ABILITY_312,
    },
    // =
    [DJINNI_13] = {
        .abilityId = ABILITY_313,
    },
    // =
    [DJINNI_14] = {
        .abilityId = ABILITY_314,
    },
    // =
    [DJINNI_15] = {
        .abilityId = ABILITY_315,
    },
    // =
    [DJINNI_16] = {
        .abilityId = ABILITY_316,
    },
    // =
    [DJINNI_17] = {
        .abilityId = ABILITY_317,
    },
    // =
    [DJINNI_18] = {
        .abilityId = ABILITY_318,
    },
    // =
    [DJINNI_19] = {
        .abilityId = ABILITY_319,
    },
    [DJINNI_FIZZ] = {
        .abilityId = ABILITY_320,
        .hp = 9,
        .pp = 4,
        .def = 3,
    },
    [DJINNI_SLEET] = {
        .abilityId = ABILITY_321,
        .hp = 12,
        .atk = 3,
        .luck = 1,
    },
    [DJINNI_MIST] = {
        .abilityId = ABILITY_322,
        .hp = 11,
        .atk = 4,
    },
    [DJINNI_SPRITZ] = {
        .abilityId = ABILITY_323,
        .hp = 8,
        .pp = 4,
        .agi = 3,
    },
    [DJINNI_HAIL] = {
        .abilityId = ABILITY_324,
        .hp = 9,
        .atk = 4,
        .luck = 1,
    },
    [DJINNI_TONIC] = {
        .abilityId = ABILITY_325,
        .hp = 8,
        .pp = 3,
        .def = 2,
        .luck = 2,
    },
    [DJINNI_DEW] = {
        .abilityId = ABILITY_326,
        .hp = 13,
        .pp = 4,
        .agi = 4,
    },
    // =
    [DJINNI_27] = {
        .abilityId = ABILITY_327,
    },
    // =
    [DJINNI_28] = {
        .abilityId = ABILITY_328,
    },
    // =
    [DJINNI_29] = {
        .abilityId = ABILITY_329,
    },
    // =
    [DJINNI_30] = {
        .abilityId = ABILITY_330,
    },
    // =
    [DJINNI_31] = {
        .abilityId = ABILITY_331,
    },
    // =
    [DJINNI_32] = {
        .abilityId = ABILITY_332,
    },
    // =
    [DJINNI_33] = {
        .abilityId = ABILITY_333,
    },
    // =
    [DJINNI_34] = {
        .abilityId = ABILITY_334,
    },
    // =
    [DJINNI_35] = {
        .abilityId = ABILITY_335,
    },
    // =
    [DJINNI_36] = {
        .abilityId = ABILITY_336,
    },
    // =
    [DJINNI_37] = {
        .abilityId = ABILITY_337,
    },
    // =
    [DJINNI_38] = {
        .abilityId = ABILITY_338,
    },
    // =
    [DJINNI_39] = {
        .abilityId = ABILITY_339,
    },
    [DJINNI_FORGE] = {
        .abilityId = ABILITY_340,
        .hp = 10,
        .atk = 2,
        .agi = 2,
        .luck = 2,
    },
    [DJINNI_FEVER] = {
        .abilityId = ABILITY_341,
        .hp = 8,
        .atk = 3,
        .agi = 2,
    },
    [DJINNI_CORONA] = {
        .abilityId = ABILITY_342,
        .hp = 12,
        .pp = 3,
        .def = 3,
        .luck = 1,
    },
    [DJINNI_SCORCH] = {
        .abilityId = ABILITY_343,
        .hp = 8,
        .atk = 3,
    },
    [DJINNI_EMBER] = {
        .abilityId = ABILITY_344,
        .hp = 9,
        .pp = 4,
        .def = 2,
        .agi = 2,
    },
    [DJINNI_FLASH] = {
        .abilityId = ABILITY_345,
        .hp = 14,
        .pp = 3,
        .def = 2,
    },
    [DJINNI_TORCH] = {
        .abilityId = ABILITY_346,
        .hp = 9,
        .atk = 3,
        .luck = 1,
    },
    // =
    [DJINNI_47] = {
        .abilityId = ABILITY_347,
    },
    // =
    [DJINNI_48] = {
        .abilityId = ABILITY_348,
    },
    // =
    [DJINNI_49] = {
        .abilityId = ABILITY_349,
    },
    // =
    [DJINNI_50] = {
        .abilityId = ABILITY_350,
    },
    // =
    [DJINNI_51] = {
        .abilityId = ABILITY_351,
    },
    // =
    [DJINNI_52] = {
        .abilityId = ABILITY_352,
    },
    // =
    [DJINNI_53] = {
        .abilityId = ABILITY_353,
    },
    // =
    [DJINNI_54] = {
        .abilityId = ABILITY_354,
    },
    // =
    [DJINNI_55] = {
        .abilityId = ABILITY_355,
    },
    // =
    [DJINNI_56] = {
        .abilityId = ABILITY_356,
    },
    // =
    [DJINNI_57] = {
        .abilityId = ABILITY_357,
    },
    // =
    [DJINNI_58] = {
        .abilityId = ABILITY_358,
    },
    // =
    [DJINNI_59] = {
        .abilityId = ABILITY_359,
    },
    [DJINNI_GUST] = {
        .abilityId = ABILITY_360,
        .hp = 9,
        .atk = 2,
        .agi = 2,
    },
    [DJINNI_BREEZE] = {
        .abilityId = ABILITY_361,
        .hp = 12,
        .pp = 5,
        .def = 2,
        .luck = 1,
    },
    [DJINNI_ZEPHYR] = {
        .abilityId = ABILITY_362,
        .hp = 11,
        .pp = 3,
        .agi = 2,
        .luck = 1,
    },
    [DJINNI_SMOG] = {
        .abilityId = ABILITY_363,
        .hp = 9,
        .atk = 3,
    },
    [DJINNI_KITE] = {
        .abilityId = ABILITY_364,
        .hp = 8,
        .pp = 4,
        .agi = 3,
    },
    [DJINNI_SQUALL] = {
        .abilityId = ABILITY_365,
        .hp = 10,
        .atk = 5,
    },
    [DJINNI_LUFF] = {
        .abilityId = ABILITY_366,
        .hp = 11,
        .pp = 5,
        .def = 2,
        .luck = 1,
    },
    // =
    [DJINNI_67] = {
        .abilityId = ABILITY_367,
    },
    // =
    [DJINNI_68] = {
        .abilityId = ABILITY_368,
    },
    // =
    [DJINNI_69] = {
        .abilityId = ABILITY_369,
    },
    // =
    [DJINNI_70] = {
        .abilityId = ABILITY_370,
    },
    // =
    [DJINNI_71] = {
        .abilityId = ABILITY_371,
    },
    // =
    [DJINNI_72] = {
        .abilityId = ABILITY_372,
    },
    // =
    [DJINNI_73] = {
        .abilityId = ABILITY_373,
    },
    // =
    [DJINNI_74] = {
        .abilityId = ABILITY_374,
    },
    // =
    [DJINNI_75] = {
        .abilityId = ABILITY_375,
    },
    // =
    [DJINNI_76] = {
        .abilityId = ABILITY_376,
    },
    // =
    [DJINNI_77] = {
        .abilityId = ABILITY_377,
    },
    // =
    [DJINNI_78] = {
        .abilityId = ABILITY_378,
    },
    // =
    [DJINNI_79] = {
        .abilityId = ABILITY_379,
    },
};
// clang-format on
