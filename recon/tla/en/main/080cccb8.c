// Near miss, differ 2 of 71 (The Broken Seal's BattleAction_FindDescriptor,
// ported as is): the native loads the event work (ldr r7, [r3, #108]) before
// it starts the -1 and 0 the loops keep in lr and ip; this draft loads it
// after them. The work in a local, declared first, or the table pointer
// stepped by hand change nothing; a one-pass block around the load differs
// 8.
#include "types.h"
#include "ram_buffer.h"

struct ActionDescriptor {
  u16 sprite;
  s16 condition;
  s32 behavior;
  s32 x;
  s32 y;
  s32 z;
  u16 facing;
  u8 talkFacing;
  u8 flags;
};

struct ActionDescriptorTables {
  struct ActionDescriptor *tables[4];
};

struct ActionDescriptor *BattleAction_FindDescriptor(s32 id) {
  struct ActionDescriptor *entry;
  s32 i;
  s32 group = 8;

  for (i = 0; i < 4; i++) {
    entry =
        ((struct ActionDescriptorTables *)Ram_HeapSlots->eventWork)->tables[i];
    if (entry == NULL)
      continue;
    if (id <= 7) {
      for (; (s16)entry->sprite != -1; entry++) {
        if ((s16)entry->sprite == id)
          goto found;
      }
    } else {
      for (; (s16)entry->sprite != -1; entry++) {
        if ((s16)entry->sprite > 7) {
          if (group == id)
            goto found;
          group++;
        }
      }
    }
  }
found:
  if ((s16)entry->sprite == -1)
    return NULL;
  return entry;
}
