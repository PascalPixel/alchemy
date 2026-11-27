// The leader's leap from stump to stump in Fuchin Falls Cave; Fuchin Temple keeps it for three hops
// nothing calls.

// The leader leaps the given pixels, two and a half a tick, and lands with a spring.
static void scHopLeaderBy(Sint32 dx, Sint32 dz)
{
    fdScriptStart();
    fdActorSetSpeed(ACTOR_PARTY_LEADER, 2.5);
    fdActorSetDestinationOffset(ACTOR_PARTY_LEADER, dx, dz);
    fdActorJump(ACTOR_PARTY_LEADER, 4, 0);
    fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_LEADER_LEAP);
    fdActorWaitForMoveById(ACTOR_PARTY_LEADER);
    fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_LEADER_SPRING);
    fdScriptEnd();
}
