/// @file
/// Babi's palace and Vale's plaza.
#include "types.h"
#include "event.h"

/// An actor walks into the leader and is taken off the map, as the party
/// closes up into one walking sprite at the end of an event.
static inline void Scene_WalkIntoLeader(s32 actorId) {
  struct Actor *leader;

  Actor_SetAnimation(actorId, ANIM_WALK);
  leader = Actor_Get(ACTOR_PARTY_LEADER);
  if (leader != NULL) {
    Actor_SetDestination(actorId, leader->x.part.pixel, leader->z.part.pixel);
  }
  Actor_WaitForMoveById(actorId);
  Actor_SetPosition(actorId, 0, 0);
}
