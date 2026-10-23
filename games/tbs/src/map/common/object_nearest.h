#include "types.h"
#include "object_event.h"
#include "map_tables.h"

// Mercury and Venus Lighthouses' aeries.

/// Returns the placed object event drawn as SPRITE_FLOATING_PLATFORM nearest
/// the leader, within 640 pixels, or 0.
s32 MapObjectEvent_FindNearestFloatingPlatform(void) {
  struct EventWork *work = gEventWork;
  struct ObjectEvent **slot;
  struct ObjectEvent *origin;
  s32 nearestObjectEvent = 0;
  s32 minDist;
  u32 objectEventId;

  minDist = 640;
  origin = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  objectEventId = LOCALID_FIRST_PLACED;
  slot = work->placedObjectEvents;
  do {
    struct ObjectEvent *objectEvent = *slot++;
    if (objectEvent != 0) {
      if (objectEvent->sprite->entries[0]->animationId ==
          SPRITE_FLOATING_PLATFORM) {
        s32 dist = FixedPoint_Distance(
            (const u8 *)origin + offsetof(struct ObjectEvent, x),
            (const u8 *)objectEvent + offsetof(struct ObjectEvent, x));
        if (dist < minDist) {
          minDist = dist;
          nearestObjectEvent = objectEventId;
        }
      }
    }
    objectEventId++;
  } while (objectEventId <
           LOCALID_FIRST_PLACED + sizeof(work->placedObjectEvents) /
                                      sizeof(work->placedObjectEvents[0]));
  return nearestObjectEvent;
}
