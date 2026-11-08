// Draft, differ 11 of 137 (tla-en, 2026-11-08)
// Left: at the join after the column loop the native copies the menu state before its test (`adds r3, r4, #0; cmp r3, #0`); this tests r4 directly, and the one missing instruction shifts the literal pool offsets that make up the rest. Every edition measures the same 11 (tla-ja, de, es, fr and it by --symbol Func_080426e0, whose address differs there).
// Closed 2026-11-08: the first column entry is read through `table` as in the native (address loaded into r7, copied to r2, read through r2). The address must reach `table` through a block-local copy set before `i`: gcse puts its copy of the address right after that load, and cse2 then swaps the two so the load lands in gcse's register (r7) and `table`, the class head by then, carries the read (FAKEMATCH(temporary) below). `table = gWindow_PartyColumnOffsets;` set directly, before or after `i = first`, loads straight into `table` (15, or 78 when set before i); the previous plain form (the first entry read into an `offset` local before the table was set) measured 13 with the read through r7. Dead forms for the same effect stay at 13 or 15: an unused read of the first entry before `i`, a NULL default, `&gWindow_PartyColumnOffsets`, `(void)&table`. The goto loop is now a do-while in the guard; both measure the same.
// Tried for the state copy, none below 11: a second local for the test at the join, of every width (u8, u16, u32, s8, s32, combine folds the copy into the branch); the same with the state kept live after its test; the parameter or `i` reused for the test; `register`; `!= 0`; an s8 state; the field read again at the join after an early read nothing uses (cse1 deletes the early read, 63); the state reloaded inside the loop body (88); The Broken Seal's plain while/for loop with the early read (46 to 80). 8 permute minutes on this form found nothing below 11 (its best weighted, a volatile temporary for the reload, is differ 19).
// Lead for the copy: in matched code (DisplayBlend_RunScript, InitializeAnimationObjects) this shape comes from combine duplicating a load whose register stays live and reload_cse turning the duplicate into a copy, which needs the load and the test in one block with no label between. Here the pre-test's exit lands on the copy itself, so the native may have tested the state once per exit (the reload, then a copy, on the loop path) and had jump2 cross-jump the two tails into one; no form found yet produces that.
// 2026-11-08 before the offset local: differ 15 of 137. The column is read through `table` while both tests read the symbol, so gcse's copy of the table's address lives in r7 and is reloaded only after the row loop, as in the native. Reading the state only at the end, a second local for it, or other types for it measure 15 or worse. Also 15 or worse: once-only blocks around the table's or the state's assignments or before the tests, s8, u16, s32 or u32 state, u32 i, the table set before i or at its declaration, a non-const table; every mix of table and symbol in the two tests and the column read. 10 permute minutes on the offset form reached 11 only by taking the table's address (a dead device).
// 2026-11-05: differ 42 of 137, 20 normalized, 2 with registers ignored (tla-en, 2026-11-05): the guarded goto loop with the menu state read first and again at the loop's exit gives the native's shape. Left: two copies the native has and this lacks, the table's address through r7 into r2 and the state from r4 into r3 before its test; forms with a second local fold away.
// Draft, differ 80 of 137, 2026-10-15: The Broken Seal's
// Window_DrawColumnBorders on this game's window (signed position,
// tilemap rows indexed directly, the column added before it is doubled).
// Remaining: the native reads the menu state into r4 in its first block and
// reads it again at the loop's exit, so the pre-loop exit jumps past the
// reload; this reads it once after the loop and allocates the loop's
// registers differently. A state local read at the start is spilled
// (differ 52); four permuter minutes reached differ 47 with a copy of the
// work pointer in the loop. 2026-10-25: alchemy-gcc's jump pass does copy the
// exit test in front of the loop (the RTL dump of jump shows it), but jump2
// cross-jumps the copy back into the test at the foot, so the loop is
// entered by a branch to it; the native keeps both copies (cmp; blt before
// the loop). With the row's three-way store reduced to two the copies differ
// and both stay.
#include "types.h"
#include "ram_buffer.h"
#include "window.h"
#include "uiwindow.h"

extern const s8 gWindow_PartyColumnOffsets[];

void Func_080426e0(struct Window *window, u32 flags) {
  struct WindowWork *canvas =
      (struct WindowWork *)Ram_HeapSlots->windowTiles;
  s32 first = 1;
  s32 bias = 0;
  u32 max = window->width - 1;
  s32 i;
  u8 state = canvas->menuState;
  u32 rows = window->height;
  u32 col;
  u32 row;
  u16 *dest;
  const s8 *table;

  if ((flags & 1) == 0)
    flags &= ~2;
  if (flags & 2) {
    bias = 5;
    first = 0;
  }
  {
    /* FAKEMATCH(temporary): the table's address passes through a block-local
     * copy set before i; as table = gWindow_PartyColumnOffsets after
     * i = first, 15 of 137 instructions differ in tla-en. */
    const s8 *columns = gWindow_PartyColumnOffsets;

    i = first;
    table = columns;
  }
  if (gWindow_PartyColumnOffsets[i] >= 0) {
    do {
      col = table[i] + bias;
      if (col < max) {
        for (row = 0; row != rows; row++) {
          dest =
              canvas->tilemap[(s16)window->y + row] + ((s16)window->x + col);
          if (row == 0)
            *dest = 0xf018;
          else if (row == rows - 1)
            *dest = 0xf019;
          else
            *dest = 0xf00f;
        }
      }
      i++;
      table = gWindow_PartyColumnOffsets;
    } while (gWindow_PartyColumnOffsets[i] >= 0);
    state = canvas->menuState;
  }
  if (state) {
    dest =
        canvas->tilemap[(s16)window->y + window->height - 1] + (s16)window->x;
    col = 1;
    *dest++ = 0xf080;
    while (col < max) {
      col++;
      *dest++ = 0xf081;
    }
    *dest = 0xf082;
  }
  canvas->dirty = 1;
}
