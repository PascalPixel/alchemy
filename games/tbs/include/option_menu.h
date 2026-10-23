#ifndef ALCHEMY_OPTION_MENU_H
#define ALCHEMY_OPTION_MENU_H

/// @file
/// The option menu's work and the selection work.

#include "unit/window/window.h"
#include "unit/window/battle_command.h"
#include "types.h"
#include "render_input.h"
#include "cursor.h"

/// The option menu's work (heap block 20): the settings being edited and
/// how many values each takes, the page, and the cursor, markers and icons.
/// OptionMenu_InitializeWork takes and clears it and copies the settings in;
/// OptionMenu_Release frees it, and cancelling discards the edits. The pause
/// menu takes the same slot for its cursor, entry (page) and icons.
struct OptionMenuWork {
  u8 unused[0x400]; ///< no code names it
  /// Seven 52-byte entries (struct OptionMenuEntry in options.c), whose
  /// state only the unused OptionMenu_ResetEntries writes.
  u8 entries[7][52];
  u8 unusedAfterEntries[8]; ///< no code names it
  /// The row the cursor is on: on the option menu 0 to 4, indexing option
  /// and optionCount; on the pause menu its entry, indexing icon.
  u16 page;
  u16 tint[3]; ///< the window tint's red, green and blue, 0 to 31
  /// Frames counted; its low three bits pick the frame of the marker of the
  /// slider in use.
  u16 cursorFrame;
  /// The colour preset SELECT last chose, 0 to 5, from
  /// gOptionMenu_HuePresets and gOptionMenu_BrightnessPresets.
  u16 preset;
  u8 unusedAfterPreset[0x14]; ///< no code names it
  /// The settings being edited: window hue (0 to 23), window brightness (0
  /// to 14), message speed (0 to 2), voice sounds and auto-sleep (0 or 1).
  s8 option[5];
  s8 optionCount[5]; ///< each setting's values: 24, 15, 3, 2 and 2; it wraps
  u8 unusedAfterOptions[6]; ///< no code names it
  union MenuCursorRef cursor;
  union MenuCursorRef marker[2];
  u8 unusedAfterMarkers[0x18]; ///< no code names it
  /// Each choice's icon for message speed, voice sounds and auto-sleep
  /// (options 2 to 4); the chosen one pulses.
  struct WindowSprite *frame[3][3];
  struct WindowSprite *icon[6];
};

/// The option menu's work, HEAP_SLOT_OPTION_MENU under its own name.
extern struct OptionMenuWork *gOptionMenuWork;

#endif
