/// @file
/// The summons: the order the Summon command lists them in, and each one's
/// ability and the Djinn it needs. Each name is its ability's.
// Part of game.c.

#include "game_internal.h"

/// The order the Summon command lists the summons in.
const u8 gSummon_OrderList[SUMMON_COUNT] = {
    0,  1,  2,  3,  4,  5,  6,  7,  16, 17, 8,  9,  10, 11, 18,
    19, 12, 13, 14, 15, 20, 21, 22, 23, 24, 25, 26, 27, 28,
};

// clang-format off
/// The 29 summons, by summon id, the numbers gSummon_OrderList lists; read
/// through SummonDefinition_Get.
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
    // Zagan
    {
        .abilityId = ABILITY_ZAGAN,
        .djinnRequired = {1, 0, 1},
    },
    // Megaera
    {
        .abilityId = ABILITY_MEGAERA,
        .djinnRequired = {0, 0, 1, 1},
    },
    // Flora
    {
        .abilityId = ABILITY_FLORA,
        .djinnRequired = {1, 0, 0, 2},
    },
    // Moloch
    {
        .abilityId = ABILITY_MOLOCH,
        .djinnRequired = {0, 2, 0, 1},
    },
    // Ulysses
    {
        .abilityId = ABILITY_ULYSSES,
        .djinnRequired = {0, 2, 2},
    },
    // Haures
    {
        .abilityId = ABILITY_HAURES,
        .djinnRequired = {3, 0, 2},
    },
    // Eclipse
    {
        .abilityId = ABILITY_ECLIPSE,
        .djinnRequired = {0, 2, 0, 3},
    },
    // Coatlicue
    {
        .abilityId = ABILITY_COATLICUE,
        .djinnRequired = {0, 3, 0, 3},
    },
    // Daedalus
    {
        .abilityId = ABILITY_DAEDALUS,
        .djinnRequired = {3, 0, 4},
    },
    // Azul
    {
        .abilityId = ABILITY_AZUL,
        .djinnRequired = {3, 4},
    },
    // Catastrophe
    {
        .abilityId = ABILITY_CATASTROPHE,
        .djinnRequired = {0, 0, 3, 5},
    },
    // Charon
    {
        .abilityId = ABILITY_CHARON,
        .djinnRequired = {8, 0, 0, 2},
    },
    // Iris
    {
        .abilityId = ABILITY_IRIS,
        .djinnRequired = {0, 4, 9},
    },
};
// clang-format on
