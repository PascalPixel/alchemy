/// @file
/// The 16 summons and the order the menus list them in. Each name is the
/// English release's.

#include "game.h"
#include "ability_ids.h"

const u8 gSummon_OrderList[16] = {0, 1, 2,  3,  4,  5,  6,  7,
                                  8, 9, 10, 11, 12, 13, 14, 15};

// clang-format off
const struct SummonDefinition gSummon_DefinitionTable[] = {
    // Venus
    {
        .abilityId = ABILITY_VENUS,
        .djinnRequired = {1},
    },
    // Mercury
    {
        .abilityId = ABILITY_MERCURY,
        .djinnRequired = {0, 1},
    },
    // Mars
    {
        .abilityId = ABILITY_MARS,
        .djinnRequired = {0, 0, 1},
    },
    // Jupiter
    {
        .abilityId = ABILITY_JUPITER,
        .djinnRequired = {0, 0, 0, 1},
    },
    // Ramses
    {
        .abilityId = ABILITY_RAMSES,
        .djinnRequired = {2},
    },
    // Nereid
    {
        .abilityId = ABILITY_NEREID,
        .djinnRequired = {0, 2},
    },
    // Kirin
    {
        .abilityId = ABILITY_KIRIN,
        .djinnRequired = {0, 0, 2},
    },
    // Atalanta
    {
        .abilityId = ABILITY_ATALANTA,
        .djinnRequired = {0, 0, 0, 2},
    },
    // Cybele
    {
        .abilityId = ABILITY_CYBELE,
        .djinnRequired = {3},
    },
    // Neptune
    {
        .abilityId = ABILITY_NEPTUNE,
        .djinnRequired = {0, 3},
    },
    // Tiamat
    {
        .abilityId = ABILITY_TIAMAT,
        .djinnRequired = {0, 0, 3},
    },
    // Procne
    {
        .abilityId = ABILITY_PROCNE,
        .djinnRequired = {0, 0, 0, 3},
    },
    // Judgment
    {
        .abilityId = ABILITY_JUDGMENT,
        .djinnRequired = {4},
    },
    // Boreas
    {
        .abilityId = ABILITY_BOREAS,
        .djinnRequired = {0, 4},
    },
    // Meteor
    {
        .abilityId = ABILITY_METEOR,
        .djinnRequired = {0, 0, 4},
    },
    // Thor
    {
        .abilityId = ABILITY_THOR,
        .djinnRequired = {0, 0, 0, 4},
    },
};
// clang-format on
