#include "HAIDIA.H"

void FieldScene_RunScene3a6_020014ac(void)
{

    u32 i;
    s32 record;
    s32 zero;

    Event_Begin();
    Event_Wait(10);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x8000, 0x1999);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 8);
    Event_Wait(15);
    Actor_SetDestinationOffset(ACTOR_PARTY_LEADER, 8, 0);
    Event_Wait(4);
    Audio_PlayCue(0x120);
    Audio_PlayCue(239);
    Actor_SetSpeed(9, 0x8000, 0x1999);
    Actor_SetAnimation(9, 2);
    zero = 0;
    *(u8 *)(Object_GetById(9) + 85) = zero;
    record = Actor_Get(9);
    *(s32 *)(record + 68) = zero;
    Actor_SetDestinationOffset(9, 12, 0);
    Actor_WaitForMove(ACTOR_PARTY_LEADER);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 1);
    Actor_WaitForMove(9);
    Audio_PlayCue(0x120);
    Audio_PlayCue(213);
    Actor_SetAnimation(9, 3);
    *(u8 *)(Object_GetById(9) + 85) = 3;
    Actor_SetDestinationOffset(9, 6, 0);
    Actor_Get(9);
    SceneActor_WaitActorDescent();
    Actor_SetAnimation(9, 8);
    Actor_SetSpritePriority(9, 3);
    *(u8 *)(Object_GetById(9) + 35) = 2;
    Value6(StagedActor_FillGridAttributeRectangle, 0, 12, 16, 1, 4, 0);
    Call6(StagedActor_FillGridAttributeRectangle, 0, 13, 16, 1, 4, 0);
    GameFlag_Set(0x202);
    Audio_PlayCue(240);
    Event_End();
}

