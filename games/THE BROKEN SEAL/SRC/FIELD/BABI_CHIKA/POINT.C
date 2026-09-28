/* The point left of actor zero. */
#include "BABI.H"

void SceneActor_ApplyPointLeftOfActorZero(void)
{
    s32 point[3];
    struct Actor_02000dc8 *actor = Actor_Get(ACTOR_PARTY_LEADER);

    point[0] = actor->f08 + 0xFFE00000;
    point[1] = actor->f0c;
    point[2] = actor->f10;
    SceneActor_MoveActorZeroToTarget(point);
}
