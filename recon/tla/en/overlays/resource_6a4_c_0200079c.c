// Near miss, differ 4 of 83 (tla-en, 2026-10-31): the colours' address
// (mov r7, sp) is built before the loop where the native builds it inside,
// and the loop's branch is 2 bytes shorter for it.
// A palette cycle: every fourth call rotates five colours of the table into
// palette offset 0x184, unless an event holds it. The table is read by a byte
// offset in a goto loop (as a for loop, loop.c hoists the table's address and
// strength reduction folds it into the offset: 14), and the colours' address
// goes through p, set after the event checks, so it takes r7 as the native's
// does. The slot and the table pointer are locals of the loop body, so the
// table's address is loaded after the __modsi3 call and added as
// [table, offset], as the native's is (read straight through gData_02003340,
// the sum is [offset, table]: 6).
// Left: the native sets r7 to sp inside the loop, before the __modsi3 call,
// and keeps it for Dma_Set after the loop. Here gcse's PRE always hoists the
// colours' address out of a bottom-tested loop: it is fully redundant with p
// (or, without p, partially redundant on the back edge and inserted on the
// entry edge), and gcse's final copy propagation then hands the address after
// the loop the hoisted register. It stays in the loop only when the loop is
// top-tested (next: if (i > 4) goto done; ... goto next: 35, an extra compare
// and branch). Tried without effect: register and volatile views of p and the
// colours, the counters set before the event checks, p = colors inside the
// loop (14), Dma_Set(colors) with no p (14, and 14 with this loop body), the
// store as *(colors + k) (35), i and offset initialised at their declarations
// with Dma_Set(colors) and a goto, while or for loop (28, 38, 38), a register
// asm pin of p to r7 (42), the table pointer set before the loop (34) or
// first in the body (41: its address then lives across the call); permute
// 12 minutes found nothing under 6, and 8 minutes on this form nothing
// under 4.
#include "../../../../games/tla/src/scene/jupiter_lighthouse_aerie/jupiter_lighthouse_aerie.h"
#include "iwram_call.h"
#include "dma.h"
#include "io_reg.h"

void Func_0200079c(void) {
  u16 colors[16];
  s32 phase = (gData_02003314 / 4) % 5;
  s32 i;
  s32 offset;
  u16 *p;

  if (gData_02003318 != 0) {
    u8 *work = Ram_HeapSlots->eventWork;
    s16 *state = (s16 *)(work + 358);

    if (*state != 0 && (*state & 0x3ff) != 153)
      return;
    if (*(s8 *)(work + 0xca4) != 0)
      return;
    if (*(s16 *)(work + 350) != 0)
      return;
  }
  p = colors;
  i = 0;
  offset = 4;
next:
  {
    /* FAKEMATCH(temporary): the slot and the table's address held in
     * locals; as colors[(phase + i) % 5] = *(u16 *)((u8 *)gData_02003340 +
     * offset), 6 of 83 instructions differ in tla-en. */
    s32 slot = (phase + i) % 5;
    u8 *table = (u8 *)gData_02003340;

    colors[slot] = *(u16 *)(table + offset);
  }
  i++;
  offset += 2;
  if (i <= 4)
    goto next;
  Dma_Set(p, (void *)0x05000184, 0x84000002, REG_DMA3);
  gData_02003314++;
}
