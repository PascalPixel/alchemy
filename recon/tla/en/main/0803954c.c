// Draft, differ 4 of 84 in tla-en under -mthumb-slow-loads (2026-10-26). No
// single empty asm, one-pass block, wrapping block or swap of neighbouring
// statements brings it closer, nor any pair of empty asm statements or
// one-pass blocks (device search).
// Draft, differ 4 of 84 (2026-10-24): an empty asm after found->unk26 = 0
// takes one reordered pair off. Left: instruction order only.
// Near miss, differ 6 of 84: ☀️'s UiText_QueueRenderEntries. The native
// starts the text-mode offset 0x138c (movs r1, #152) before the channel
// stores and finishes it (lsls, adds) after them; agscc builds it whole
// before them. The zero pinned to r2 behind a barrier loads from the pool
// before the spacing is addressed (differ 8 without them; the pin alone, or
// with a one-pass block, 8).
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

#if EDITION_INTERNATIONAL
#define UI_TEXT_MODE_OFFSET 0x138c
#define UI_PALETTE_OWNER_OFFSET 0x1360
#else
#define UI_TEXT_MODE_OFFSET 0x118c
#define UI_PALETTE_OWNER_OFFSET 0x1160
#endif

struct UiChannelSlot *UiText_QueueRenderEntries(struct UiWindow *window,
                                                s32 entry, s32 x, s32 y,
                                                u16 *spacing, s32 flags) {
  u8 *work = (u8 *)Ram_HeapSlots->windowTiles;
  struct UiChannelSlot *render = ((struct UiRenderWork *)work)->channels;
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
    __asm__ volatile("");
    if (*(s8 *)(work + UI_TEXT_MODE_OFFSET) != 0 && spacing != NULL)
      found->unk26 = spacing[3];
    if (spacing != NULL) {
      for (i = 0; i < 4; i++)
        found->spacing[i] = *spacing++;
    } else {
      /* FAKEMATCH: the zero pinned to r2 loads from the pool before the
       * spacing is addressed; the barrier holds it there. */
      register u16 zero asm("r2") = 0;

      /* FAKEMATCH: the barrier of the pin above. */
      asm volatile("" : "+l"(zero));
      for (i = 0; i < 4; i++)
        found->spacing[i] = zero;
    }
    found->line = 0;
  }
  return found;
}
