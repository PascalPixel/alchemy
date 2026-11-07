// Func_08103168: The Broken Seal's Menu_RunPairedEntryAction, with the first
// owner of each pair read into a temporary.
#include "types.h"
#include "ram_buffer.h"
#include "djinn_menu.h"

void Func_08103218(s32 window, s32 x, s32 y, s32 owner, s32 a, s32 b, s32 c,
                   s32 param, s32 d);

#define ALT_PARAM 4

s32 Func_08103168(s32 mode, s32 param) {
  s32 sp14;
  u8 first;
  u8 owner;
  struct DjinnMenuWork *menu;

  menu = Ram_HeapSlots->menuRuntime;
  if (mode == 0) {
    sp14 = mode;
    first = menu->pairMember[1];
    Func_08103218((s32)menu->secondWindow, 0, 0, first, 1, mode, 2, param, 1);
    Func_08103218(menu->window, 0, 0, menu->pairMember[0], mode, 1, 2, param,
                  mode);
  } else {
    owner = menu->shownMember[1];
    Func_08103218((s32)menu->secondWindow, 0, 0, owner, 1, 0, ALT_PARAM, param,
                  1);
    Func_08103218(menu->window, 0, 0, menu->shownMember[0], 0, 0, 1, param, 0);
  }
  return 1;
}
