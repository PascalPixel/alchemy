#include "types.h"
#include "field_obj.h"
#include "field_scene.h"

// Mercury and Venus Lighthouses' aeries.

/// Returns the placed actor drawn as SPRITE_FLOATING_PLATFORM nearest the
/// leader, within 640 pixels, or 0.
s32 SceneActor_FindNearestFloatingPlatform(void) {
  struct EventWork *work = gEventWork;
  struct FieldActor **slot;
  struct FieldActor *origin;
  s32 nearestActor = 0;
  s32 minDist;
  u32 actorId;

  minDist = 640;
  origin = Object_GetById(ACTOR_PARTY_LEADER);
  actorId = ACTOR_FIRST_PLACED;
  slot = work->placedActors;
  do {
    struct FieldActor *actor = *slot++;
    if (actor != 0) {
      if (actor->sprite->entries[0]->animationId == SPRITE_FLOATING_PLATFORM) {
        s32 dist = FixedPoint_Distance(
            (const u8 *)origin + offsetof(struct FieldActor, x),
            (const u8 *)actor + offsetof(struct FieldActor, x));
        if (dist < minDist) {
          minDist = dist;
          nearestActor = actorId;
        }
      }
    }
    actorId++;
  } while (actorId < ACTOR_FIRST_PLACED + sizeof(work->placedActors) /
                                              sizeof(work->placedActors[0]));
  return nearestActor;
}
