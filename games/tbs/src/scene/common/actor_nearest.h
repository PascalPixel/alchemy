#include "types.h"
#include "actor.h"
#include "scene_tables.h"

// Mercury and Venus Lighthouses' aeries.

/// Returns the placed actor drawn as SPRITE_FLOATING_PLATFORM nearest
/// the leader, within 640 pixels, or 0.
s32 Actor_FindNearestFloatingPlatform(void) {
  struct FieldWork *work = gFieldWork;
  struct Actor **slot;
  struct Actor *origin;
  s32 nearestActorId = 0;
  s32 minDist;
  u32 actorId;

  minDist = 640;
  origin = Actor_GetById(ACTOR_PARTY_LEADER);
  actorId = ACTOR_FIRST_PLACED;
  slot = &work->actors[ACTOR_FIRST_PLACED];
  do {
    struct Actor *actor = *slot++;
    if (actor != 0) {
      if (actor->sprite->entries[0]->animationId == SPRITE_FLOATING_PLATFORM) {
        s32 dist =
            FixedPoint_Distance((const u8 *)origin + offsetof(struct Actor, x),
                                (const u8 *)actor + offsetof(struct Actor, x));
        if (dist < minDist) {
          minDist = dist;
          nearestActorId = actorId;
        }
      }
    }
    actorId++;
  } while (actorId < sizeof(work->actors) / sizeof(work->actors[0]));
  return nearestActorId;
}
