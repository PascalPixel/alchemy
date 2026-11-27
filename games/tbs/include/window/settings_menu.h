#ifndef ALCHEMY_SETTINGS_MENU_H
#define ALCHEMY_SETTINGS_MENU_H

// The state of the settings menu and the Pause menu.

#include "static_assert.h"
#include "types.h"
#include "system.h"
#include "window/cursor.h"
#include "window/screens.h"
#include "window/window.h"
#include "window/window_sprite.h"

// The settings menu's state (heap block 20): the settings being edited and how many values each
// takes, the page, and the cursor, markers and icons.
typedef struct SETTINGS_MENU_STATE
{
	Uint8           unused[1024];
	// Seven 52-byte entries (OPTION_MENU_ENTRY in window_settings_menu.c), whose state only the
	// unused mnSettingsMenuResetEntries writes.
	Uint8           entries[7][52];
	Uint8           unusedAfterEntries[8];
	// The row the cursor is on: on the settings menu 0 to 4, indexing option and optionCount; on
	// the Pause menu its entry, indexing icon.
	Uint16          page;
	Uint16          tint[3]; // the window tint's red, green and blue, 0 to 31
	// Ticks counted; its low three bits pick the picture of the marker of the slider in use.
	Uint16          cursorFrame;
	// The colour preset SELECT last chose, 0 to 5, from SettingsMenuHuePresets and
	// SettingsMenuBrightnessPresets.
	Uint16          preset;
	Uint8           unusedAfterPreset[20];
	// The settings being edited: window hue (0 to 23), window brightness (0 to 14), message speed
	// (0 to 2), voice sounds and auto-sleep (0 or 1).
	Sint8           option[5];
	// each setting's values: 24, 15, 3, 2 and 2; it wraps
	Sint8           optionCount[5];
	Uint8           unusedAfterOptions[6];
	MENU_CURSOR_REF cursor;
	MENU_CURSOR_REF marker[2];
	Uint8           unusedAfterMarkers[24];
	// Each choice's icon for message speed, voice sounds and auto-sleep (options 2 to 4); the
	// chosen one pulses.
	WINDOW_SPR     *choiceIcons[3][3];
	WINDOW_SPR     *icon[6];
} SETTINGS_MENU_STATE;

STATIC_ASSERT(sizeof(SETTINGS_MENU_STATE) == 1576);
STATIC_ASSERT(offsetof(SETTINGS_MENU_STATE, entries) == 1024);

// The settings menu's state, HEAP_SLOT_SETTINGS_MENU under its own name.
#define OptionMenuState (*(SETTINGS_MENU_STATE **)&HeapSlots[HEAP_SLOT_SETTINGS_MENU])

#endif
