/// @file
/// Debug: format a right-aligned number in debug characters.

#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "types.h"

// Both format into a right-aligned buffer and write digits characters from its
// end.

/// Hexadecimal, digits places (1-8, or 8 if out of range).
void DebugText_PrintHex(u32 value, s32 digits) {
  s32 n;

  n = digits;
  if ((n - 1) > 7U) {
    n = 8;
  }
  NumberText_FormatHex(value);
  DebugText_Print((const u8 *)(((u32)&gNumberTextHexEnd) - n));
}

/// Signed decimal, digits places (1-10, or 10 if out of range).
void DebugText_PrintDecimal(s32 value, s32 digits) {
  s32 n;

  n = digits;
  if ((n - 1) > 9U) {
    n = 10;
  }
  NumberText_FormatDecimal(value);
  DebugText_Print((const u8 *)(((u32)&gNumberTextDecimalEnd) - n));
}
