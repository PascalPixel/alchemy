// Draft, differ 2 of 83 (2026-10-15), ported from The Broken Seal's
// Menu_RunPairedEntryAction with the first owner of each pair read into a
// temporary (each fixes one reordered load; differ 6 without). The native
// reloads the saved zero (ldr r4, [sp, #20]) between the two halves of the
// pair's offset 0x25c; here the offset is finished first. Three minutes of
// alchemy permute found nothing better.
#include "types.h"
#include "ram_buffer.h"

struct DjinnMenuWork {
  u8 unk000[0x28];
  s32 window;
  u8 unk02c[0x0c];
  s32 secondWindow;
  u8 unk03c[0x216 - 0x3c];
  u8 shownOwner[2];
  u8 unk218[0x25c - 0x218];
  u8 pairOwner[2];
};

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
    first = menu->pairOwner[1];
    Func_08103218(menu->secondWindow, 0, 0, first, 1, mode, 2, param, 1);
    Func_08103218(menu->window, 0, 0, menu->pairOwner[0], mode, 1, 2, param,
                  mode);
  } else {
    owner = menu->shownOwner[1];
    Func_08103218(menu->secondWindow, 0, 0, owner, 1, 0, ALT_PARAM, param, 1);
    Func_08103218(menu->window, 0, 0, menu->shownOwner[0], 0, 0, 1, param, 0);
  }
  return 1;
}
