#include "types.h"
#include "field_obj.h"

// Colosso Log, River and Wall, and Venus Lighthouse.

/// Returns the placed actor (8 to 65) whose cell, and 16-pixel step of height,
/// is that of the 16.16 position, or 0. self is not read.
static void *SceneData_FindSlotAtPosition(void *position, void *self) {
  s32 *pos = position;
  struct FieldActor **slots = gEventWork->actors;
  u32 i;

  for (i = 8; i <= 65; i++) {
    struct FieldActor *p = slots[i];

    if ((pos[0] >> 20) == (p->x.fixed >> 20) &&
        (pos[1] >> 20) == (p->y.fixed >> 20) &&
        (pos[2] >> 20) == (p->z.fixed >> 20)) {
      return p;
    }
  }
  return 0;
}
