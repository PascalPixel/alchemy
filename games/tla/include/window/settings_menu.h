#ifndef ALCHEMY_SETTINGS_MENU_H
#define ALCHEMY_SETTINGS_MENU_H

/// @file
/// The settings menu's work.

#include "static_assert.h"
#include "types.h"
#include "window/cursor.h"

struct WindowSprite;

/// A menu cursor as a work block keeps it: its moving sprite, or the sprite
/// alone where only its picture is read.
union MenuCursorRef {
  struct WindowSprite *sprite; ///< the image alone; cursor.anchor
  struct MenuCursor cursor;
};

/// The settings menu's work (HEAP_SLOT_SETTINGS_MENU): the settings being
/// edited and how many values each takes, the page, and the cursor, markers
/// and icons. The Pause menu and the debug menus use its cursor, page and
/// icons too.
struct OptionMenuWork {
  u8 unused[1024];
  u8 entries[7][52]; ///< unused: nothing in this game reads them
  u8 unusedAfterEntries[8];
  /// The row the cursor is on: on the settings menu 0 to 4, indexing option
  /// and optionCount; on the Pause menu its entry, indexing icon.
  u16 page;
  u16 tint[3]; ///< the window tint's red, green and blue, 0 to 31
  /// Frames counted; its low three bits pick the picture of the marker of the
  /// slider in use.
  u16 cursorFrame;
  /// The colour preset SELECT last chose, 0 to 5, from
  /// gSettingsMenu_HuePresets and gSettingsMenu_BrightnessPresets.
  u16 preset;
  u8 unusedAfterPreset[20];
  /// The settings being edited: window hue (0 to 23), window brightness (0
  /// to 14), message speed (0 to 2), voice sounds and auto-sleep (0 or 1).
  s8 option[5];
  s8 optionCount[5]; ///< each setting's values: 24, 15, 3, 2 and 2; it wraps
  u8 unusedAfterOptions[6];
  union MenuCursorRef cursor;
  union MenuCursorRef marker[2]; ///< the hue and brightness sliders' markers
  u8 unusedAfterMarkers[24];
  /// Each choice's icon for message speed, voice sounds and auto-sleep
  /// (options 2 to 4); the chosen one pulses.
  struct WindowSprite *choiceIcons[3][3];
  struct WindowSprite *icon[6]; ///< the Pause menu's entries' icons
};

STATIC_ASSERT(sizeof(struct OptionMenuWork) == 1576);
STATIC_ASSERT(offsetof(struct OptionMenuWork, page) == 1396);
STATIC_ASSERT(offsetof(struct OptionMenuWork, cursor) == 1444);
STATIC_ASSERT(offsetof(struct OptionMenuWork, icon) == 1552);

/// The sound test's four values: music, jingle, sound effect and sound preset.
struct SoundTestValues {
  s32 values[4];
};

#endif
