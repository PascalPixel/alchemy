/// @file
/// The summons: the order the Summon command lists them in, and each one's
/// ability and the Djinn it needs. Each name is its ability's.

#include "game.h"

/// The order the Summon command lists the summons in.
const u8 gSummon_OrderList[29] = {
    0,  1,  2,  3,  4,  5,  6,  7,  16, 17, 8,  9,  10, 11, 18,
    19, 12, 13, 14, 15, 20, 21, 22, 23, 24, 25, 26, 27, 28,
};

// clang-format off
const struct SummonDefinition gSummon_DefinitionTable[] = {
    // Venus
    {
        .nameMessageId = 380,
        .djinnRequired = {1},
    },
    // Mercury
    {
        .nameMessageId = 388,
        .djinnRequired = {0, 1},
    },
    // Mars
    {
        .nameMessageId = 396,
        .djinnRequired = {0, 0, 1},
    },
    // Jupiter
    {
        .nameMessageId = 406,
        .djinnRequired = {0, 0, 0, 1},
    },
    // Ramses
    {
        .nameMessageId = 381,
        .djinnRequired = {2},
    },
    // Nereid
    {
        .nameMessageId = 389,
        .djinnRequired = {0, 2},
    },
    // Kirin
    {
        .nameMessageId = 397,
        .djinnRequired = {0, 0, 2},
    },
    // Atalanta
    {
        .nameMessageId = 407,
        .djinnRequired = {0, 0, 0, 2},
    },
    // Cybele
    {
        .nameMessageId = 382,
        .djinnRequired = {3},
    },
    // Neptune
    {
        .nameMessageId = 390,
        .djinnRequired = {0, 3},
    },
    // Tiamat
    {
        .nameMessageId = 398,
        .djinnRequired = {0, 0, 3},
    },
    // Procne
    {
        .nameMessageId = 408,
        .djinnRequired = {0, 0, 0, 3},
    },
    // Judgment
    {
        .nameMessageId = 383,
        .djinnRequired = {4},
    },
    // Boreas
    {
        .nameMessageId = 391,
        .djinnRequired = {0, 4},
    },
    // Meteor
    {
        .nameMessageId = 399,
        .djinnRequired = {0, 0, 4},
    },
    // Thor
    {
        .nameMessageId = 409,
        .djinnRequired = {0, 0, 0, 4},
    },
    // Zagan
    {
        .nameMessageId = 384,
        .djinnRequired = {1, 0, 1},
    },
    // Megaera
    {
        .nameMessageId = 400,
        .djinnRequired = {0, 0, 1, 1},
    },
    // Flora
    {
        .nameMessageId = 410,
        .djinnRequired = {1, 0, 0, 2},
    },
    // Moloch
    {
        .nameMessageId = 392,
        .djinnRequired = {0, 2, 0, 1},
    },
    // Ulysses
    {
        .nameMessageId = 401,
        .djinnRequired = {0, 2, 2},
    },
    // Haures
    {
        .nameMessageId = 385,
        .djinnRequired = {3, 0, 2},
    },
    // Eclipse
    {
        .nameMessageId = 411,
        .djinnRequired = {0, 2, 0, 3},
    },
    // Coatlicue
    {
        .nameMessageId = 393,
        .djinnRequired = {0, 3, 0, 3},
    },
    // Daedalus
    {
        .nameMessageId = 402,
        .djinnRequired = {3, 0, 4},
    },
    // Azul
    {
        .nameMessageId = 394,
        .djinnRequired = {3, 4},
    },
    // Catastrophe
    {
        .nameMessageId = 412,
        .djinnRequired = {0, 0, 3, 5},
    },
    // Charon
    {
        .nameMessageId = 386,
        .djinnRequired = {8, 0, 0, 2},
    },
    // Iris
    {
        .nameMessageId = 404,
        .djinnRequired = {0, 4, 9},
    },
};
// clang-format on
