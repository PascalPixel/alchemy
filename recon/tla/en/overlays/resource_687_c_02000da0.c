// Matches: differ 0 (tla-en, 2026-11-08)
// Differ 0 in all six editions (tla-en, ja, de, es, fr, it). From 2: two
// indexes, i for palette entries from 1 and j for saved colours from 0, stepped
// i then j, and the saved colours read as saved->colors[j] through a struct
// pointer set in the for's init after i. Why it works: loop.c's first pass
// reduces a giv only through a pseudo that existed before the loop, so the
// table base must come from a local; a struct member indexes as an array
// (offset + base, the native's [sl + fp]) where a u16 pointer or a u16 (*)[14]
// becomes pointer arithmetic and one pointer giv (104). j, a biv of its own
// stepped after i, is reduced first: its init lands before the palette's and
// its step after it, the order the native has. Measured: one index,
// saved->colors[i - 1] (2); the pointer set before the for (8); j++ before i++
// (4); for-init orders with saved after i, and the palette set before the for,
// all 0. extern struct SavedColors gData_020026c0 with saved = &gData_020026c0
// also matches; the scene's adopted files declare it u16[14], hence the cast.
// The same two indexes and struct pointer match Func_02001108 (differ 0 in a
// scratch copy). Earlier rounds, on one index: a stepped palette pointer (12),
// palette[i] from colour 0 (2), i < 15 (6), (gData_020026c0 - 1)[i] (99),
// the target read first (64), do-while; permuter 14 minutes: nothing below 2.
#include "types.h"
#include "call.h"
#include "system/io_reg.h"

/// The 14 colours of BG palette 3 (entries 1 to 14) that the fade returns to,
/// copied from palette RAM by Func_02001cdc.
struct SavedColors {
  u16 colors[14];
};

extern u8 Func_02000c50[];
extern s32 gData_020022cc[];
extern u16 gData_020026c0[];
void Task_RemoveFar();
void Func_02001fac();
void Func_02002044();
void Func_02002054();
void wait();
void GameFlag_ClearBit();
s32 Func_02001fb4();

s32 Func_02000da0(void) {
  s32 changed;
  volatile u16 *palette;
  struct SavedColors *saved;
  u32 i;
  u32 j;

  Task_RemoveFar(Func_02000c50);
  Func_02001fac();
  Func_02002044(0);
  Func_02002054(0x22c);
  *gData_020022cc = 1;
  // Each of the 14 colours steps one unit per channel toward its saved colour
  // every 10 frames, until none changes.
fade:
  wait(10);
  changed = 0;
  palette = &BG_PLTT_COLOR(3, 0);
  // i counts palette entries from 1 and j saved colours from 0; the saved
  // colours are read through a pointer set after i, each pass.
  for (i = 1, saved = (struct SavedColors *)gData_020026c0, j = 0; i != 15;
       i++, j++) {
    u16 color = palette[i];
    u16 target = saved->colors[j];
    u16 r = color & 31;
    u16 g = (color >> 5) & 31;
    u16 b = (color >> 10) & 31;
    u16 tr = target & 31;
    u16 tg = (target >> 5) & 31;
    u16 tb = (target >> 10) & 31;

    if (r < tr) {
      r++;
      changed = 1;
    }
    if (r > tr) {
      r--;
      changed = 1;
    }
    if (g < tg) {
      g++;
      changed = 1;
    }
    if (g > tg) {
      g--;
      changed = 1;
    }
    if (b < tb) {
      b++;
      changed = 1;
    }
    if (b > tb) {
      b--;
      changed = 1;
    }
    palette[i] = RGB(r, g, b);
  }
  if (changed)
    goto fade;
  *gData_020022cc = 1;
  GameFlag_ClearBit(0x200);
  return Func_02001fb4();
}
