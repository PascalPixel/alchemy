// Near miss, differ 4 of 84 (tla-en, 2026-10-31): the empty asm is replaced
// by a one-pass block around the text-mode test, a milder device with the same
// 4 (movs r1, #152 and the flags store each one row out of place).
// ☀️'s Message_Start, now read through WindowWork.textMode (the old 0x118c
// offset was the Japanese one, as EDITION_INTERNATIONAL is only defined for
// assembly); with that the spacing loops match with no pin or barrier. Left:
// instruction order only.
// The native starts the text-mode offset 0x138c (movs r1, #152) before the
// channel stores and finishes it (lsls, adds) after them. Without a device,
// sched2 builds it whole before them (differ 6): the flags load and
// movs r1, #152 tie at priority 8 and the class rule takes the movs, which the
// native could only skip with that chain at priority 6 or lower; with the asm
// or the one-pass block it comes whole after them. No statement order,
// parameter type, local or nested test brings it closer (permuter, 4 minutes,
// twice, no better candidate). In the plain form's sched2 dump the stores rank
// 5 and lsls r1 7, so the native order needs the stores to outrank lsls; the
// tla-ja listing (offset 0x118c) has the same order. Also measured: textMode
// read through a volatile pointer becomes a plain byte load, which brings
// lsls r1 nearer the native place but loses the asrs (differ 13); a volatile
// unk26 or ended store, a local s8 copy, a nested test or a pointer to
// textMode taken before the stores (34) do no better; with the one-pass block,
// flags stored before the zero stores swaps it with the window store instead
// (still 4), and flags moved before the window store reallocates (28).
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "window.h"
#include "ram_buffer.h"
#include "resource.h"
#include "iwram_call.h"

struct UiChannelSlot *Message_Start(struct Window *window, s32 entry, s32 x,
                                    s32 y, u16 *spacing, s32 flags) {
  struct WindowWork *work = (struct WindowWork *)Ram_HeapSlots->windowTiles;
  struct UiChannelSlot *render = work->channels;
  struct UiChannelSlot *found = NULL;
  u32 i;

  for (i = 0; i != 3; i++, render++) {
    if (render->work == NULL) {
      found = render;
      break;
    }
  }
  if (found != NULL) {
    found->work = window;
    found->startX = x << 8;
    found->x = x << 8;
    found->y = y << 8;
    found->entry = entry;
    found->colour = 15;
    found->lineSpacing = 10;
    found->countdown = 0;
    found->outline = 0;
    found->ended = 0;
    found->flags = flags;
    found->unk26 = 0;
    do {
      /* FAKEMATCH(one-pass): the form kept; as the bare if, 6 of 84 instructions differ in tla-en. */
      if (work->textMode != 0 && spacing != NULL)
        found->unk26 = spacing[3];
    } while (0);
    if (spacing != NULL) {
      for (i = 0; i < 4; i++)
        found->spacing[i] = *spacing++;
    } else {
      for (i = 0; i < 4; i++)
        found->spacing[i] = 0;
    }
    found->line = 0;
  }
  return found;
}
