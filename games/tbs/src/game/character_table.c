/// @file
/// The eight party members' growth curves, element bonuses, starting levels
/// and starting equipment.

#include "game.h"
#include "item_ids.h"

// clang-format off
const struct CharacterDefinition gCharacter_DefinitionTable[] = {
    [ISAAC] = {
        .hp = {30, 182, 334, 486, 638, 790},
        .pp = {20, 80, 130, 170, 210, 250},
        .atk = {13, 86, 162, 237, 313, 388},
        .def = {6, 38, 69, 102, 134, 166},
        .agi = {8, 86, 163, 241, 318, 396},
        .luck = {3, 3, 3, 3, 3, 3},
        .elementBonus = {54, 3, 1, 2},
        .startingLevel = 1,
        .items = {ITEM_MACHETE, ITEM_COTTON_SHIRT, ITEM_PADDED_GLOVES, ITEM_HERB, ITEM_HERB, ITEM_HERB},
    },
    [GARET] = {
        .hp = {33, 191, 351, 510, 670, 830},
        .pp = {18, 76, 124, 162, 200, 238},
        .atk = {11, 83, 156, 228, 301, 373},
        .def = {8, 41, 75, 110, 144, 179},
        .agi = {6, 76, 144, 212, 281, 349},
        .luck = {2, 2, 2, 2, 2, 2},
        .elementBonus = {1, 2, 54, 3},
        .startingLevel = 1,
        .items = {ITEM_MACHETE, ITEM_COTTON_SHIRT, ITEM_PADDED_GLOVES, ITEM_HERB, ITEM_HERB},
    },
    [IVAN] = {
        .hp = {28, 166, 304, 442, 581, 719},
        .pp = {24, 92, 150, 196, 242, 288},
        .atk = {8, 76, 144, 211, 277, 344},
        .def = {4, 35, 65, 95, 124, 155},
        .agi = {11, 91, 173, 255, 337, 419},
        .luck = {4, 4, 4, 4, 4, 4},
        .elementBonus = {2, 1, 3, 54},
        .startingLevel = 4,
        .items = {ITEM_055, ITEM_090, ITEM_136, ITEM_166, ITEM_HERB},
    },
    [MIA] = {
        .hp = {29, 173, 317, 462, 606, 751},
        .pp = {23, 90, 146, 190, 235, 280},
        .atk = {9, 79, 150, 220, 289, 359},
        .def = {5, 37, 68, 100, 131, 163},
        .agi = {7, 80, 152, 224, 296, 369},
        .luck = {5, 5, 5, 5, 5, 5},
        .elementBonus = {3, 54, 2, 1},
        .startingLevel = 10,
        .items = {ITEM_057, ITEM_104, ITEM_128, ITEM_157, ITEM_HERB, ITEM_ANTIDOTE, ITEM_ELIXIR},
    },
    [FELIX] = {},
    [JENNA] = {
        .hp = {29, 182, 334, 486, 638, 790},
        .pp = {19, 80, 130, 170, 210, 250},
        .atk = {10, 81, 153, 224, 295, 366},
        .def = {7, 37, 68, 100, 131, 163},
        .agi = {9, 84, 160, 236, 312, 388},
        .luck = {4, 4, 4, 4, 4, 4},
        .elementBonus = {1, 2, 54, 3},
        .startingLevel = 1,
        .items = {ITEM_055, ITEM_103, ITEM_PADDED_GLOVES, ITEM_HERB},
    },
    [SHEBA] = {},
    [PIERS] = {},
};
// clang-format on
