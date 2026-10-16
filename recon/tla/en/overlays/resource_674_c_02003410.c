// Draft of overlay 674's Func_02003410, its palette queue's allocation (the
// main image's Runtime_AllocateAndClearQueue with other loops), differ 30
// of 42. The native keeps the queue in r0, the entry in r4 and i in r5,
// rebuilds the loop limits 16 << 16 and 4 << 16 on every pass (no loop notes:
// goto loops), and steps j through its u16 value (lsls, lsrs; adds #1; lsls,
// asrs) and i as (i << 16) + 0x10000.
#include "types.h"
#include "mem.h"
#include "palqueue.h"

void Func_02003410(void) {
  struct PaletteSnapshotWork *queue;
  struct PaletteSnapshot *entry;
  s16 i;
  u16 j;

  queue = Runtime_AllocateBlock(112, sizeof(struct PaletteSnapshotWork));
  i = 0;
  entry = queue->entries;
outer:
  entry->source = 0;
  entry->position = 0;
  entry->timer = 0;
  entry->delay = 0;
  entry->count = 0;
  j = 0;
inner:
  entry->colors[j] = 0;
  j++;
  if ((s16)j != 16)
    goto inner;
  entry++;
  i++;
  if (i != 4)
    goto outer;
  queue->count = 0;
}
