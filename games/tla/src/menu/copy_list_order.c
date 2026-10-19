/// @file
/// Menu: copy the sorting order table.

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

/// Copies the order table for modes 0-2, or the default table, into order.
/// Tables end with 0xff; at most 32 bytes are copied, the terminator
/// included.
void Menu_CopyListOrder(s32 mode, u8 *order) {
  u8 *src;
  s32 n;

  src = gMenu_ListOrderDefault;
  switch (mode) {
  case 0:
    src = gMenu_ListOrderMode0;
    break;
  case 1:
    src = gMenu_ListOrderMode1;
    break;
  case 2:
    src = gMenu_ListOrderMode2;
    break;
  }
  *order = *src;
  n = 0;
  if (*order != 0xff) {
    do {
      if (++n > 31)
        break;
      src++;
      order++;
      *order = *src;
    } while (*order != 0xff);
  }
}
