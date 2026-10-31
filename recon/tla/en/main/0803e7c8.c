// Near miss, differ 4 of 140 (tla-en, 2026-10-30): scheduler order on the
// left side only. The listing issues `movs r4, #255` before the tile pointer's
// literal load and stores the x before it copies the left tiles to r11; this
// draft loads the literal one slot early and copies to r11 before the store.
// The plain form (no block) differs in 8. In sched2 the store's only way to
// the block's end is the load of `top` (priority 5), while the copy to r11 is
// anti-dependent on r0's reuse for top's offset (priority 7); with a store to
// load latency of 3 the two tie, the store's three dependents win and the
// listing's whole left-side order follows: a cost-model difference no source
// form reaches. Assigning the left tiles after the store moves reload's
// register rotation (50 to 63 differ), so the literal belongs first.
// An empty asm in place of the one-pass block reaches 2 (only the copy and
// the store, tied on priority and dependents and broken by RTL order).
// The permuter (4 minutes, 2 jobs, from the plain form) finds only one-pass
// blocks at 4.
// Earlier: The Broken Seal's Menu_SetupSelectionSide on this game's cursor
// tiles (gData_0805c5c4 right, gData_0805c1c4 left). A device search (each
// statement followed by an empty asm or a one-pass block, wrapped in one, or
// swapped with the next) finds nothing closer than the empty asm's 2.

#include "types.h"
#include "resource.h"
#include "select.h"

extern u8 gData_0805c5c4[];
extern u8 gData_0805c1c4[];

void Func_0803e7c8(struct SelectionScreen *state, s32 index) {
  struct SelectionSprite *entry = &state->records[index].sprite;
  u8 *frames = 0;
  u32 count;

  state->records[index].kind = 0;
  if (index != 0) {
    u8 *right = gData_0805c5c4;

    count = state->count;
    if (state->top != 0)
      count -= state->top;
    if (count > 5) {
      state->records[index].kind = 1;
      count = 5;
    }
    frames = right;
    state->records[1].x = state->baseX + 16 * (count - 1) + 17;
  } else {
    frames = gData_0805c1c4;
    state->records[0].x = state->baseX - 9;
    /* FAKEMATCH(one-pass): an empty block after the left x; as the plain
     * statement alone, 8 of 140 instructions differ in tla-en. */
    do {
    } while (0);
    if (state->top != 0)
      state->records[0].kind = 1;
  }
  if (state->records[index].y == 0) {
    state->records[index].slot = Resource_FindFreeEntry();
    state->records[index].tile =
        VramCache_Load(state->records[index].slot, 128, frames);
    state->records[index].y = state->baseY;
    state->records[index].base = 0;
    entry->mode = 0;
    entry->mosaic = 0;
    entry->colors = 1;
    entry->affine = 0;
    entry->param = 0;
    entry->size = 0;
    entry->shape = 2;
    entry->priority = 0;
  }
}
