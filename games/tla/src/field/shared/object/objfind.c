#include "types.h"
#include "ram_buffer.h"
#include "object_runtime.h"

struct EventWork {
  u8 unk00[20];
  struct ObjectRuntime *objects[64];
};

struct ObjectRuntime *Func_080eaf28(s32 *position, s32 checkY) {
  u32 i;

  for (i = 8; i < 64; i++) {
    struct EventWork *event = Ram_HeapSlots->eventWork;
    struct ObjectRuntime *object = event->objects[i];

    if (checkY) {
      if (position[0] >> 20 == object->x >> 20 &&
          position[1] / 0x10000 == object->y / 0x10000 &&
          position[2] >> 20 == object->z >> 20)
        return object;
    } else {
      if (position[0] >> 20 == object->x >> 20 &&
          position[2] >> 20 == object->z >> 20)
        return object;
    }
  }
  return NULL;
}
