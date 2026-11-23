/// @file
/// The leader's leap from stump to stump in Fuchin Falls Cave; Fuchin Temple
/// keeps it for three hops nothing calls.

/// The leader leaps the given pixels, two and a half a frame, and lands with a
/// spring.
static void Scene_HopLeaderBy(s32 dx, s32 dz) {
  Event_Start();
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 2.5);
  Actor_SetDestinationOffset(ACTOR_PARTY_LEADER, dx, dz);
  Actor_Jump(ACTOR_PARTY_LEADER, 4, 0);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_LEADER_LEAP);
  Actor_WaitForMoveById(ACTOR_PARTY_LEADER);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_LEADER_SPRING);
  Event_End();
}
