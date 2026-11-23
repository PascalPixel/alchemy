/// @file
/// The Djinn menu's element icon offsets and the icon menus' tables: the
/// waits of a choice made for the player to watch, and the field menu's and
/// the File Screen's choices by icon position.

#include "types.h"
#include "window/screens.h"

/// Each element's icon column in the Djinn menu, in tiles.
u8 gDjinnMenu_ElementXOffsets[4] = {0, 2, 0, 2};
/// Each element's icon row in the Djinn menu, in tiles.
u8 gDjinnMenu_ElementYOffsets[4] = {0, 0, 1, 1};

/// The icon menu's wait at each step, in frames, by the icons still to go
/// (IconMenu_AnimateTo).
u8 gMenu_SelectionStepDelays[8] = {46, 20, 28, 24, 30, 44, 29, 32};

// clang-format off
/// The field menu's command at each icon position, read at position + 1: a row
/// of six with Djinn, then one without, where the Djinn icon is left out.
s8 gMenu_TopEntryCommandByPosition[] = {
    0, FIELD_MENU_PSYNERGY, FIELD_MENU_DJINN, FIELD_MENU_ITEM, FIELD_MENU_STATUS, 5,
    0, FIELD_MENU_PSYNERGY, FIELD_MENU_ITEM, FIELD_MENU_STATUS, 5, 0,
};

/// The icon position + 1 of each field menu command (enum FieldMenuCommand),
/// in the same two rows; without Djinn, Djinn starts on the first icon.
s8 gMenu_TopEntryPositionByCommand[] = {
    0, 1, 2, 3, 4, 5,
    0, 1, 0, 2, 3, 0,
};

/// The File Screen's action at each icon position, a row of six for each set
/// of icons FileList_SelectAction offers: every action; with three files, no
/// New Game or Copy; with three cleared files, only Erase, Battle and Update;
/// with fewer files, all cleared, no Continue. A row's places past its icons,
/// and the byte after the rows, are never read.
s8 gMenu_SaveSlotActionByPosition[] = {
    SAVE_ACTION_NEW_GAME, SAVE_ACTION_CONTINUE, SAVE_ACTION_COPY, SAVE_ACTION_ERASE, SAVE_ACTION_BATTLE, SAVE_ACTION_UPDATE,
    SAVE_ACTION_CONTINUE, SAVE_ACTION_ERASE, SAVE_ACTION_BATTLE, SAVE_ACTION_UPDATE, SAVE_ACTION_CONTINUE, SAVE_ACTION_CONTINUE,
    SAVE_ACTION_ERASE, SAVE_ACTION_BATTLE, SAVE_ACTION_UPDATE, SAVE_ACTION_CONTINUE, SAVE_ACTION_CONTINUE, SAVE_ACTION_CONTINUE,
    SAVE_ACTION_NEW_GAME, SAVE_ACTION_COPY, SAVE_ACTION_ERASE, SAVE_ACTION_BATTLE, SAVE_ACTION_UPDATE, SAVE_ACTION_CONTINUE,
    SAVE_ACTION_NEW_GAME,
};
// clang-format on
