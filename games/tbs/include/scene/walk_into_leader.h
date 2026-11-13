/// @file
/// A companion and the party leader: placed on the leader, walked toward the
/// leader, or walked into the leader and taken off the map, as the scenes'
/// events gather the party back into one walking sprite.
#include "field/event.h"

/// Puts an actor where the leader stands, if there is a leader.
static inline void Scene_PlaceAtLeader(s32 actorId) {
  struct Actor *leader = Actor_Get(ACTOR_PARTY_LEADER);

  if (leader != NULL) {
    Actor_SetPositionFixed(actorId, leader->x.fixed, leader->z.fixed);
  }
}

/// Sends an actor walking to where the leader stands, if there is a leader.
static inline void Scene_WalkTowardLeader(s32 actorId) {
  struct Actor *leader = Actor_Get(ACTOR_PARTY_LEADER);

  if (leader != NULL) {
    Actor_SetDestination(actorId, leader->x.part.pixel, leader->z.part.pixel);
  }
}

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
