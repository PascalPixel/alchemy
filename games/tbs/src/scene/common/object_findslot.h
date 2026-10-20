#include "types.h"
#include "field_obj.h"
#include "field_runtime.h"

// Colosseum Final 1, 2 and 3, and Venus Lighthouse.

/// Returns the placed actor (8 to 65) whose cell, and 16-pixel step of height,
/// is that of the 16.16 position, or 0. self is not read.
static void *SceneData_FindSlotAtPosition(const struct FieldPosition *position,
                                          void *self) {
  const struct FieldPosition *pos = position;
  // Field_RunScene owns all 66 slots. EventWork's actors view holds only the
  // first eight; the placed actors follow in its separate placedActors array.
  struct FieldWork *work = (struct FieldWork *)gEventWork;
  struct FieldActor **slots = work->actors;
  u32 i;

  for (i = ACTOR_FIRST_PLACED;
       i < sizeof(work->actors) / sizeof(work->actors[0]); i++) {
    struct FieldActor *p = slots[i];

    // UB: ObjectTable_ClearAll leaves unused slots null. Skipping them grows
    // this helper from 0x48 to 0x4c bytes in the native English trial.
#ifdef UBFIX
    if (p == NULL)
      continue;
#endif
    if ((pos->x >> 20) == (p->x.fixed >> 20) &&
        (pos->y >> 20) == (p->y.fixed >> 20) &&
        (pos->z >> 20) == (p->z.fixed >> 20)) {
      return p;
    }
  }
  return 0;
}
