/// @file
/// Two fades of BG palette 3: toward RGB(11, 6, 23), then, in Func_02000da0,
/// back to its saved colours.
#include "iwram_call.h"
#include "system/io_reg.h"
#include "actor.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "text/msg_ids.h"
#include "types.h"
#include "event_inline.h"
#include "frame.h"

extern u32 gData_020022cc;
extern s32 gData_020022c8;
void Func_02000c50(void);
void Func_02001fac(void);
s32 Func_02001fb4(void);

/// Fades BG palette 3's entries 1 to 13 toward RGB(11, 6, 23), one unit per
/// channel every 10 frames, then hands the rest of the event to Func_02000c50's
/// task.
s32 Func_02000c94(void) {
  volatile u16 *palette;
  s32 changed;
  s32 i;
  s32 frame;

  Func_02001fac();
  Event_ResolvePendingActions(0);
  GameFlag_SetBit(0x200);
  Sound_Play(0x22d);
  do {
    wait(10);
    changed = 0;
    for (i = 1, palette = &BG_PLTT_COLOR(3, 1); i != 14; i++, palette++) {
      u16 color = *palette;
      u16 r = color & 31;
      u16 g = (color >> 5) & 31;
      u16 b = (color >> 10) & 31;

      if (r < 11) {
        r++;
        changed = 1;
      }
      if (r > 11) {
        r--;
        changed = 1;
      }
      if (g < 6) {
        g++;
        changed = 1;
      }
      if (g > 6) {
        g--;
        changed = 1;
      }
      if (b < 23) {
        b++;
        changed = 1;
      }
      if (b > 23) {
        b--;
        changed = 1;
      }
      *palette = RGB(r, g, b);
    }
    frame = gFrameCount;
    gData_020022cc += frame & 1;
  } while (changed);
  gData_020022cc = 360;
  gData_020022c8 = frame;
  Task_Add(Func_02000c50, 0x480);
  return Func_02001fb4();
}

/// The 14 colours of BG palette 3 (entries 1 to 14) that the fade returns to,
/// copied from palette RAM by Func_02001cdc and rotated by Func_02001650 in
/// step with BG palette entries 49 to 62.
struct SavedColors {
  u16 colors[14];
};

extern struct SavedColors gData_020026c0;

s32 Func_02000da0(void) {
  s32 changed;
  volatile u16 *palette;
  struct SavedColors *saved;
  u32 i;
  u32 j;

  Task_RemoveFar(Func_02000c50);
  Func_02001fac();
  Event_ResolvePendingActions(0);
  Sound_Play(0x22c);
  gData_020022cc = 1;
  // Each of the 14 colours steps one unit per channel toward its saved colour
  // every 10 frames, until none changes.
fade:
  wait(10);
  changed = 0;
  palette = &BG_PLTT_COLOR(3, 0);
  // i counts palette entries from 1 and j saved colours from 0; the saved
  // colours are read through a pointer set after i, each pass.
  for (i = 1, saved = &gData_020026c0, j = 0; i != 15; i++, j++) {
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
  gData_020022cc = 1;
  GameFlag_ClearBit(0x200);
  /* FAKEMATCH(signature): typed to return s32, passing on Func_02001fb4's,
   * though its target Event_End returns nothing; as void, 6 of 150 instructions
   * differ in tla-en. */
  return Func_02001fb4();
}
