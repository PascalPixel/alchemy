// Near miss, differ 2 of 75 (tla-en, 2026-10-30): one reordered pair, the
// scheduler's tie-break. sched2 dump: after the resourceBase store, the count
// address's reload (movs r1, #142) and the row address's adds r3, #2 both
// have priority 44; the reload is independent of the last store (class 3),
// the add anti-dependent on it (class 2), so the reload goes first. With this
// allocation the two chains always tie (each runs through the count load).
// Tried again without gain: no count local, s16 count, width += 2, guarded
// do-while, a count pointer, reordered sums; permuter 4 minutes: no better.
// Under -mthumb-slow-loads (2026-10-26): differ 2 of 75 in tla-en, instruction
// order only. No empty asm before any one statement lowers it.
// Near miss, differ 2 of 75 (one reordered pair), 2026-10-15: The Broken
// Seal's Menu_CenterResourceEntries, written fresh from it. Remaining: the
// native stores the row before it builds the count's offset (movs r1, #142);
// this builds the offset first. An empty asm after the row store fixes the
// order but holds the prologue's sub sp above it; one-pass blocks break
// CSE (differ 38 to 42); operand-tied empty asms after each statement and
// the device search found nothing lower.

#include "heap_release.h"
#include "types.h"
#include "resource.h"
#include "resource_menu.h"
#include "ram_buffer.h"

struct Window *Window_Create(s32 x, s32 y, s32 width, s32 height,
                                 s32 style);

void Menu_CenterResourceEntries(s32 row, s32 width, s32 resourceBase) {
  struct ResourceMenuWork *menu = Ram_HeapSlots->menuSelectWork;
  s32 x;
  s32 i;
  s32 count;

  menu->width = width + 2;
  menu->resourceBase = resourceBase;
  menu->row = row;
  count = menu->count;
  x = 15 - (count * 3 + menu->width * 2 / 3) / 2;
  for (i = 0; i < menu->count; i++) {
    struct ResourceMenuEntry *entry = &menu->entries[i];

    entry->x = x * 8;
    entry->y = row * 8;
    x += 3;
  }
  menu->window = Window_Create(x, row, menu->width, 3, 2);
}
