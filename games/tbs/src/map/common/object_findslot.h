#include "types.h"
#include "object_event.h"
#include "map_header.h"

// Colosseum Final 1, 2 and 3, and Venus Lighthouse.

/// Returns the placed object event (8 to 65) whose cell, and 16-pixel step of
/// height, is that of the 16.16 position, or 0. self is not read.
static void *MapData_FindSlotAtPosition(const struct FieldPosition *position,
                                        void *self) {
  const struct FieldPosition *pos = position;
  struct FieldWork *work = gFieldWork;
  struct ObjectEvent **slots = work->objectEvents;
  u32 i;

  for (i = LOCALID_FIRST_PLACED;
       i < sizeof(work->objectEvents) / sizeof(work->objectEvents[0]); i++) {
    struct ObjectEvent *p = slots[i];

    // UB: ObjectEvent_ClearAll leaves unused slots null. Skipping them grows
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
