#ifndef ALCHEMY_SETTINGS_MENU_H
#define ALCHEMY_SETTINGS_MENU_H

/* The settings menu's state. */

#include "static_assert.h"
#include "types.h"
#include "window/cursor.h"

struct WINDOW_SPR;

/* A menu cursor as a menu's state keeps it: its moving sprite, or the sprite
   alone where only its picture is read. */
typedef union MENU_CURSOR_REF {
	struct WINDOW_SPR *sprite; /* the image alone; cursor.anchor */
	MENU_CURSOR		   cursor;
} MENU_CURSOR_REF;

/* The settings menu's state (HEAP_SLOT_SETTINGS_MENU): the settings being
   edited and how many values each takes, the page, and the cursor, markers and
   icons. The Pause menu and the debug menus use its cursor, page and icons
   too. */
typedef struct SETTINGS_MENU_STATE {
	Uint8 unused[1024];
	Uint8 entries[7][52]; /* unused: nothing in this game reads them */
	Uint8 unusedAfterEntries[8];
	/* The row the cursor is on: on the settings menu 0 to 4, indexing option
	   and optionCount; on the Pause menu its entry, indexing icon. */
	Uint16 page;
	Uint16 tint[3]; /* the window tint's red, green and blue, 0 to 31 */
	/* Ticks counted; its low three bits pick the picture of the marker of the
	   slider in use. */
	Uint16 cursorFrame;
	/* The colour preset SELECT last chose, 0 to 5, from SettingsMenuHuePresets
	   and SettingsMenuBrightnessPresets. */
	Uint16 preset;
	Uint8  unusedAfterPreset[20];
	/* The settings being edited: window hue (0 to 23), window brightness (0 to
	   14), message speed (0 to 2), voice sounds and auto-sleep (0 or 1). */
	Sint8 option[5];
	/* each setting's values: 24, 15, 3, 2 and 2; it wraps */
	Sint8			optionCount[5];
	Uint8			unusedAfterOptions[6];
	MENU_CURSOR_REF cursor;
	MENU_CURSOR_REF marker[2]; /* the hue and brightness sliders' markers */
	Uint8			unusedAfterMarkers[24];
	/* Each choice's icon for message speed, voice sounds and auto-sleep
	   (options 2 to 4); the chosen one pulses. */
	struct WINDOW_SPR *choiceIcons[3][3];
	struct WINDOW_SPR *icon[6]; /* the Pause menu's entries' icons */
} SETTINGS_MENU_STATE;

STATIC_ASSERT(sizeof(SETTINGS_MENU_STATE) == 1576);
STATIC_ASSERT(offsetof(SETTINGS_MENU_STATE, page) == 1396);
STATIC_ASSERT(offsetof(SETTINGS_MENU_STATE, cursor) == 1444);
STATIC_ASSERT(offsetof(SETTINGS_MENU_STATE, icon) == 1552);

/* The sound test's four values: music, jingle, sound effect and sound
   preset. */
typedef struct SND_TEST_VALUES {
	Sint32 values[4];
} SND_TEST_VALUES;

#endif
