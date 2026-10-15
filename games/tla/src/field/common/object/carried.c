#include "types.h"
#include "ram_buffer.h"
#include "object_runtime.h"

#define WORD_AT(base, offset) (*(s32 *)((u8 *)(base) + (offset)))

void Func_080246b8(void) {
  struct ObjectRuntime *object = Ram_HeapSlots->scriptObjects;
  s32 i;

  for (i = 63; i >= 0; i--, object++) {
    if (object->script != NULL) {
      s32 x = object->x;
      s32 y = object->y;
      s32 z = object->z;

      if (object->priorityFlags & 64) {
        struct ObjectRuntime *carrier =
            (struct ObjectRuntime *)WORD_AT(object, 0x7c);

        if (carrier != NULL) {
          if (WORD_AT(carrier, 0x74) != 0) {
            if (y == object->terrainHeight) {
              object->terrainHeight = carrier->y + 0x100000;
              y = object->terrainHeight;
            } else {
              object->terrainHeight += WORD_AT(carrier, 0x74);
              y += WORD_AT(carrier, 0x74);
            }
          }
          x += WORD_AT(carrier, 0x70);
          z += WORD_AT(carrier, 0x78);
        }
      }
      object->x = x;
      object->y = y;
      object->z = z;
    }
  }
}
