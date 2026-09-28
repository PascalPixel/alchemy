/* A step and the point two right of actor zero. */
#include "BABI.H"

void FieldScene_RunStepWith6(void)
{
    BattleFx_RunRisingObjectSequence(0, 6, 0);
}

void SceneActor_PassPointTwoRightOfActorZero(void)
{
    s32 pos[3];
    struct Actor_02000dc8 *actor = Actor_Get(ACTOR_PARTY_LEADER);

    pos[0] = actor->f08 + 0x200000;
    pos[1] = actor->f0c;
    pos[2] = actor->f10;
    SceneActor_MoveActorZeroToTarget(pos);
}
