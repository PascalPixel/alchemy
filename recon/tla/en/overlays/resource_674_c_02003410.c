// Draft, differ 27 of 42 (tla-en, 2026-11-05). Older notes below may predate this figure.
// Under -Os, 5 November: differ 31 of 42 (tla-en); older figures below predate it.
// Draft of overlay 674's Func_02003410, its palette queue's allocation (the
// main image's PaletteQueue_Initialize with other loops), differ 30
// of 42. The native keeps the queue in r0, the entry in r4 and i in r5,
// rebuilds the loop limits 16 << 16 and 4 << 16 on every pass (no loop notes:
// goto loops), and steps j through its u16 value (lsls, lsrs; adds #1; lsls,
// asrs) and i as (i << 16) + 0x10000.
#include "types.h"
#include "call.h"
#include "heap_release.h"
#include "palette_snapshots.h"

void Func_02003410(void) {
  struct PaletteSnapshotWork *queue;
  struct PaletteSnapshot *entry;
  s16 i;
  u16 j;

  i = 0;
  queue = HeapSlot_AllocEwram(112, sizeof(struct PaletteSnapshotWork));
  entry = queue->entries;
outer:
  entry->source = 0;
  entry->position = 0;
  entry->timer = 0;
  entry->delay = 0;
  entry->count = 0;
  j = 0;
inner:
  /* FAKEMATCH(one-pass): the statement in a once-only block, whose end holds the
   * order around it; as the plain statement, 31 of 42 instructions differ
   * in tla-en. */
  do {
    entry->colors[j] = 0;
  } while (0);
  j++;
  if ((s16)j != 16)
    goto inner;
  /* FAKEMATCH(one-pass): the statement in a once-only block, whose end holds the
   * order around it; as the plain statement, 29 of 42 instructions differ
   * in tla-en. */
  do {
    entry++;
  } while (0);
  i++;
  if (i != 4)
    goto outer;
  queue->count = 0;
}
