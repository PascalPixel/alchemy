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

/// The Broken Seal's Scene_FindActorPlacement: finds actor id's placement in
/// the event work's four tables (ids up to 7 by sprite, later ones counted
/// in order among the rows past 7), or NULL.
struct ActionDescriptor *Scene_FindActorPlacement(s32 id) {
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
