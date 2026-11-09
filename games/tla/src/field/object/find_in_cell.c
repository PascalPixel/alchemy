#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "event_work.h"

/// Returns the first of objects 8 to 63 standing in the same 16-pixel cell
/// as position (x, y and z), or NULL. Callers pass the object asking; the
/// search does not use it.
// ☀️'s twin takes the object too.
struct Actor *Actor_FindInCell(s32 *position,
                                        struct Actor *self) {
  struct EventWork *work = Ram_HeapSlots->eventWork;
  u32 i;
  void **objects;
  struct Actor *object;

  i = 8;
  objects = &work->objects[8];
  for (; i < 64; i++) {
    object = *objects++;
    if (position[0] >> 20 == object->x >> 20 &&
        position[1] >> 20 == object->y >> 20 &&
        position[2] >> 20 == object->z >> 20)
      return object;
  }
  return NULL;
}
