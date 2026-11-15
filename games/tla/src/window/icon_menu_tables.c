/// @file
/// The Djinn menu's element icon offsets and the icon menus' tables. They are
/// not const: read as const, BattleDjinnMenu_Run orders its instructions
/// otherwise, and the menus declare the rest without const.

#include "types.h"

/// Each element's icon offset in the Djinn menu, in tiles.
u8 gDjinnMenu_ElementXOffsets[4] = {0, 2, 0, 2};
u8 gDjinnMenu_ElementYOffsets[4] = {0, 0, 1, 1};

/// The icon menu's wait at each step, in frames, by the icons still to go.
u8 gMenu_SelectionStepDelays[8] = {46, 20, 28, 24, 30, 44, 29, 32};

/// The top menu's command at each position, with Djinn and without.
// clang-format off
s8 gMenu_TopEntryCommandByPosition[] = {
    0, 1, 2, 3, 4, 5,
    0, 1, 3, 4, 5, 0,
};

/// The top menu's position of each command, with Djinn and without.
s8 gMenu_TopEntryPositionByCommand[] = {
    0, 1, 2, 3, 4, 5,
    0, 1, 0, 2, 3, 0,
};

/// The save menu's action at each position.
s8 gMenu_SaveSlotActionByPosition[] = {
    0, 1, 2, 3, 4,
    5, 1, 3, 4, 5,
    1, 1, 3, 4, 5,
    1, 1, 1, 0, 2,
    3, 4, 5, 1, 0,
};
// clang-format on
