#include "types.h"
#include "ram_buffer.h"
#include "djinn_menu.h"

void Func_08103218(s32 window, s32 x, s32 y, s32 owner, s32 a, s32 b, s32 c,
                   s32 pageIndex, s32 d);

#define ALT_PARAM 4

/// Draws both sides of a Djinn preview for page pageIndex: the second member
/// in the second window and the first in the first. Mode 0 draws the pair
/// members, both with preview mode 2; any other mode the shown members, with
/// modes 4 and 1. Returns 1.
s32 DjinnMenu_DrawPairPreview(s32 mode, s32 pageIndex) {
  s32 sp14;
  u8 first;
  u8 owner;
  struct DjinnMenuWork *menu;

  menu = Ram_HeapSlots->menuRuntime;
  if (mode == 0) {
    sp14 = mode;
    first = menu->pairMember[1];
    Func_08103218((s32)menu->secondWindow, 0, 0, first, 1, mode, 2, pageIndex,
                  1);
    Func_08103218(menu->window, 0, 0, menu->pairMember[0], mode, 1, 2,
                  pageIndex, mode);
  } else {
    owner = menu->shownMember[1];
    Func_08103218((s32)menu->secondWindow, 0, 0, owner, 1, 0, ALT_PARAM,
                  pageIndex, 1);
    Func_08103218(menu->window, 0, 0, menu->shownMember[0], 0, 0, 1, pageIndex,
                  0);
  }
  return 1;
}
