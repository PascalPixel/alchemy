#include "types.h"
#include "call.h"
#include "heap_release.h"
#include "palette_snapshots.h"

void Func_02003410(void) {
  struct PaletteSnapshotWork *queue;
  struct PaletteSnapshot *entry;
  s16 i, j;

  queue = HeapSlot_AllocEwram(112, sizeof(struct PaletteSnapshotWork));
  entry = queue->entries;
  i = 0;
outer:
  entry->source = 0;
  entry->position = 0;
  entry->timer = 0;
  entry->delay = 0;
  entry->count = 0;
  j = 0;
inner:
  entry->colors[(u16)j] = 0;
  {
    s32 next = ((u16)j + 1) << 16;

    j = next >> 16;
    if (next != 16 << 16)
      goto inner;
  }
  entry++;
  {
    s32 next = (i << 16) + 0x10000;

    i = next >> 16;
    if (next != 4 << 16)
      goto outer;
  }
  queue->count = 0;
}
