// The Djinn menu's element icon offsets and the icon menus' tables: the waits of a choice made for
// the player to watch, and the field menu's and the File Screen's choices by icon position.

#include "window/screens.h"

// Each element's icon column in the Djinn menu, in tiles.
Uint8 DjinnMenuElementXOffsets[4] = {0, 2, 0, 2};
// Each element's icon row in the Djinn menu, in tiles.
Uint8 DjinnMenuElementYOffsets[4] = {0, 0, 1, 1};

// The icon menu's wait at each step, in ticks, by the icons still to go (mnIconMenuAnimateTo).
Uint8 MenuSelectionStepDelays[8]  = {46, 20, 28, 24, 30, 44, 29, 32};

// clang-format off
// The field menu's command at each icon position, read at position + 1: a row of six with Djinn, then one without, where the Djinn icon is left out.
Sint8 MenuTopEntryCommandByPosition[] = {
	0, FIELD_MENU_PSYNERGY, FIELD_MENU_DJINN, FIELD_MENU_ITEM, FIELD_MENU_STATUS, 5,
	0, FIELD_MENU_PSYNERGY, FIELD_MENU_ITEM, FIELD_MENU_STATUS, 5, 0,
};

// The icon position + 1 of each field menu command (FIELD_MENU_COMMAND), in the same two rows; without Djinn, Djinn starts on the first icon.
Sint8 MenuTopEntryPositionByCommand[] = {
	0, 1, 2, 3, 4, 5,
	0, 1, 0, 2, 3, 0,
};

// The File Screen's action at each icon position, a row of six for each set of icons mnFileListSelectAction offers: every action; with three files, no New Game or Copy; with three cleared files, only Erase, Battle and Update; with fewer files, all cleared, no Continue. A row's places past its icons, and the byte after the rows, are never read.
Sint8 MenuSaveSlotActionByPosition[] = {
	SAVE_ACTION_NEW_GAME, SAVE_ACTION_CONTINUE, SAVE_ACTION_COPY, SAVE_ACTION_ERASE, SAVE_ACTION_BATTLE, SAVE_ACTION_UPDATE,
	SAVE_ACTION_CONTINUE, SAVE_ACTION_ERASE, SAVE_ACTION_BATTLE, SAVE_ACTION_UPDATE, SAVE_ACTION_CONTINUE, SAVE_ACTION_CONTINUE,
	SAVE_ACTION_ERASE, SAVE_ACTION_BATTLE, SAVE_ACTION_UPDATE, SAVE_ACTION_CONTINUE, SAVE_ACTION_CONTINUE, SAVE_ACTION_CONTINUE,
	SAVE_ACTION_NEW_GAME, SAVE_ACTION_COPY, SAVE_ACTION_ERASE, SAVE_ACTION_BATTLE, SAVE_ACTION_UPDATE, SAVE_ACTION_CONTINUE,
	SAVE_ACTION_NEW_GAME,
};
// clang-format on
