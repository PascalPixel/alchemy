#include "types.h"
#include "field_obj.h"
#include "field_scene.h"

struct SpriteNumber {
  u8 unk00[0x28];
  const s16 *number;
};

// Mercury and Venus Lighthouses' aeries.

// Return the placed actor drawn with sprite 0xf2 nearest the leader, within
// 640 pixels, or 0.
s32 SceneActor_FindNearestSlotOfKindF2(void) {
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
      if (*((struct SpriteNumber *)actor->sprite)->number == 0xf2) {
        s32 dist = FixedPoint_Distance(&origin->x.fixed, &actor->x.fixed);
        if (dist < minDist) {
          minDist = dist;
          nearestActor = actorId;
        }
      }
    }
    actorId++;
  } while (actorId <= 65);
  return nearestActor;
}
