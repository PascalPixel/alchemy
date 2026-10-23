#include "types.h"
#include "object_event.h"

// Bilibin, its cave, Kalay and Sol Sanctum's entrance.

/// Returns the placed object event (8 to 65) standing on cell (x, z), or 0.
static struct ObjectEvent *MapObjectEvent_FindAtCell(s32 x, s32 z) {
  struct ObjectEvent **objectEvents = gEventWork->placedObjectEvents - 8;
  u32 i;

  for (i = 8; i <= 65; i++) {
    struct ObjectEvent *objectEvent = objectEvents[i];

    if (x == objectEvent->x.fixed >> 20 && z == objectEvent->z.fixed >> 20) {
      return objectEvent;
    }
  }
  return 0;
}
