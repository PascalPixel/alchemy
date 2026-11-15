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
        .nameMessageId = ABILITY_VENUS,
        .djinnRequired = {1},
    },
    // Mercury
    {
        .nameMessageId = ABILITY_MERCURY,
        .djinnRequired = {0, 1},
    },
    // Mars
    {
        .nameMessageId = ABILITY_MARS,
        .djinnRequired = {0, 0, 1},
    },
    // Jupiter
    {
        .nameMessageId = ABILITY_JUPITER,
        .djinnRequired = {0, 0, 0, 1},
    },
    // Ramses
    {
        .nameMessageId = ABILITY_RAMSES,
        .djinnRequired = {2},
    },
    // Nereid
    {
        .nameMessageId = ABILITY_NEREID,
        .djinnRequired = {0, 2},
    },
    // Kirin
    {
        .nameMessageId = ABILITY_KIRIN,
        .djinnRequired = {0, 0, 2},
    },
    // Atalanta
    {
        .nameMessageId = ABILITY_ATALANTA,
        .djinnRequired = {0, 0, 0, 2},
    },
    // Cybele
    {
        .nameMessageId = ABILITY_CYBELE,
        .djinnRequired = {3},
    },
    // Neptune
    {
        .nameMessageId = ABILITY_NEPTUNE,
        .djinnRequired = {0, 3},
    },
    // Tiamat
    {
        .nameMessageId = ABILITY_TIAMAT,
        .djinnRequired = {0, 0, 3},
    },
    // Procne
    {
        .nameMessageId = ABILITY_PROCNE,
        .djinnRequired = {0, 0, 0, 3},
    },
    // Judgment
    {
        .nameMessageId = ABILITY_JUDGMENT,
        .djinnRequired = {4},
    },
    // Boreas
    {
        .nameMessageId = ABILITY_BOREAS,
        .djinnRequired = {0, 4},
    },
    // Meteor
    {
        .nameMessageId = ABILITY_METEOR,
        .djinnRequired = {0, 0, 4},
    },
    // Thor
    {
        .nameMessageId = ABILITY_THOR,
        .djinnRequired = {0, 0, 0, 4},
    },
    // Zagan
    {
        .nameMessageId = ABILITY_ZAGAN,
        .djinnRequired = {1, 0, 1},
    },
    // Megaera
    {
        .nameMessageId = ABILITY_MEGAERA,
        .djinnRequired = {0, 0, 1, 1},
    },
    // Flora
    {
        .nameMessageId = ABILITY_FLORA,
        .djinnRequired = {1, 0, 0, 2},
    },
    // Moloch
    {
        .nameMessageId = ABILITY_MOLOCH,
        .djinnRequired = {0, 2, 0, 1},
    },
    // Ulysses
    {
        .nameMessageId = ABILITY_ULYSSES,
        .djinnRequired = {0, 2, 2},
    },
    // Haures
    {
        .nameMessageId = ABILITY_HAURES,
        .djinnRequired = {3, 0, 2},
    },
    // Eclipse
    {
        .nameMessageId = ABILITY_ECLIPSE,
        .djinnRequired = {0, 2, 0, 3},
    },
    // Coatlicue
    {
        .nameMessageId = ABILITY_COATLICUE,
        .djinnRequired = {0, 3, 0, 3},
    },
    // Daedalus
    {
        .nameMessageId = ABILITY_DAEDALUS,
        .djinnRequired = {3, 0, 4},
    },
    // Azul
    {
        .nameMessageId = ABILITY_AZUL,
        .djinnRequired = {3, 4},
    },
    // Catastrophe
    {
        .nameMessageId = ABILITY_CATASTROPHE,
        .djinnRequired = {0, 0, 3, 5},
    },
    // Charon
    {
        .nameMessageId = ABILITY_CHARON,
        .djinnRequired = {8, 0, 0, 2},
    },
    // Iris
    {
        .nameMessageId = ABILITY_IRIS,
        .djinnRequired = {0, 4, 9},
    },
};
// clang-format on
