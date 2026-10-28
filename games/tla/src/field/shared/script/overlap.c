#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "ram_buffer.h"
#include "object_runtime.h"

s32 ScriptObject_CheckOverlap(struct ObjectRuntime *object, s32 *values) {
  s32 index;
  u8 *flags;
  struct ObjectRuntime *entry = Ram_HeapSlots->scriptObjects;

  flags = &entry->collisionFlags;
  for (index = 0; index < 64; index++, entry++, flags += sizeof(*entry)) {
    if (entry->script != NULL && (1 & *flags) && entry != object) {
      if (Field_CheckSpheresOverlap(&entry->x, entry->radius - 2, values,
                                     object->radius - 2) >= 0)
        return -1;
    }
  }
  return 0;
}

struct ObjectRuntime *
ScriptObject_FindOverlappingEntry(struct ObjectRuntime *object, s32 *values) {
  s32 index;
  u8 *flags;
  struct ObjectRuntime *entry = Ram_HeapSlots->scriptObjects;

  flags = &entry->collisionFlags;
  for (index = 0; index < 64; index++, entry++, flags += sizeof(*entry)) {
    if (entry->script != NULL && (1 & *flags) && entry != object) {
      if (Field_CheckSpheresOverlap(&entry->x, entry->radius - 2, values,
                                     object->radius - 2) >= 0)
        return entry;
    }
  }
  return NULL;
}
